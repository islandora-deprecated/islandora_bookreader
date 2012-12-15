<?php

/**
 * @file
 *
 * Handles the search callback.
 */

/**
 * Performs a solr search for the given term within the given book object's
 * pages using SOLR. Returns a JSON object for use in the Internet Archive
 * BookReader.
 *
 * This function exits.
 *
 * @param string $object_id
 *   The PID of the book object.
 * @param type $ocr_term
 *   The text to search for within the OCR'ed text.
 */
function islandora_internet_archive_bookreader_search($object_id, $term) {
  if (!module_exists('islandora_solr')) {
    drupal_exit();
  }
  $results = islandora_internet_archive_bookreader_query_solr($object_id, $term);
  $results = islandora_internet_archive_bookreader_map_solr_results_to_bookreader_results($results, $term);
  drupal_json_output($results);
  drupal_exit();
}

/**
 * Queries SOLR for all occurrences of the given term within the given book.
 *
 * @param string $object_id
 *  The PID of the book to search within.
 * @param string $term
 *  The term to search within the given book.
 *
 * @return array
 *  The results of the SOLR search.
 */
function islandora_internet_archive_bookreader_query_solr($object_id, $term) {
  $url = islandora_internet_archive_bookreader_get_solr_search_url($object_id, $term);
  $request = drupal_http_request($url);
  if ($request->code == '200') {
    return drupal_json_decode($request->data);
  }
  return array();
}

/**
 * Maps the given SOLR results to BookReader results, also it generates word
 * coordinates where applicable.
 *
 * @param array $results
 *   The SOLR results to map.
 * @param string $term
 *   The term used to generate the SOLR results.
 *
 * @return array
 *   The results in BookReader format including word coordinates.
 */
function islandora_internet_archive_bookreader_map_solr_results_to_bookreader_results(array $results, $term) {
  module_load_include('inc', 'islandora_internet_archive_bookreader', 'includes/utilities');
  $ocr_field = variable_get('islandora_internet_archive_bookreader_ocr_field', 'OCR.OCR');
  $sequence_field = variable_get('islandora_internet_archive_bookreader_sequence_number', 'rels.isSequenceNumber');
  $output = array('matches' => array());
  foreach ($results['response']['docs'] as $doc) {
    // @todo Index the page width and height in solr, rather than fetch from
    // djatoka
    $pid = $doc['PID'];
    $page_data = islandora_internet_archive_bookreader_get_page_data($pid);
    $page = $doc["$sequence_field"][0];
    $highlighting = $results['highlighting'];
    $highlighted_text = $highlighting[$pid][$ocr_field][0];
    $output['matches'][] = array(
      'text' => preg_replace('/[^a-zA-Z0-9-\n {}]/', '', $highlighted_text),
      'par' => array(
        array(
          'page' => intval($page),
          'page_width' => $page_data['width'],
          'page_height' => $page_data['height'],
          'boxes' => islandora_internet_archive_bookreader_populate_boxes($pid, $page, $term),
        ),
      )
    );
  }
  return $output;
}

/**
 * Gets the SOLR search URL for the given search parameters.
 *
 * @param string $object_id
 *  The PID of the book to search within.
 * @param string $term
 *  The term to search within the given book.
 *
 * @return string
 *   The SOLR search URL that will query for all occurances of the given term
 *   within the given book.
 */
function islandora_internet_archive_bookreader_get_solr_search_url($object_id, $term) {
  $solr_url = variable_get('islandora_solr_url', 'localhost:8080/solr');
  $request_handler = variable_get('islandora_solr_request_handler', '');
  $sequence_field = variable_get('islandora_internet_archive_bookreader_sequence_number', 'rels.isSequenceNumber');
  $ocr_field = variable_get('islandora_internet_archive_bookreader_ocr_field', 'OCR.OCR');
  $ocr_filter_field = variable_get('islandora_internet_archive_bookreader_ocr_filter_field', 'rels.isMemberOf');
  $num_rows = variable_get('islandora_internet_archive_bookreader_solr_result_count', 32);
  // Allow multiple words
  $term = htmlspecialchars(drupal_encode_path($term), ENT_QUOTES, 'utf-8', FALSE);
  $params = array(
    "qt={$request_handler}",
    "q={$ocr_field}:\"{$term}\"",
    "fq={$ocr_filter_field}:\"info:fedora/{$object_id}\"",
    'version=2.2',
    'start=0',
    "rows={$num_rows}",
    "fl=PID,{$sequence_field}",
    'hl=true',
    "hl.fl={$ocr_field}",
    'hl.snippets=200',
    'hl.simple.pre={{{',
    'hl.simple.post=}}}',
    'hl.fragsize=200',
    'wt=json'
  );
  $url = "{$solr_url}/select/?" . implode('&', $params);
  $url_components = parse_url($url);
  // Default to http if they need https they should set it in the islandora solr
  // config form.
  $url = isset($url_components['scheme']) ? '' : 'http://' . $url;
  return $url;
}

/**
 * Get the word coordinates for the given term and page.
 *
 * @param string $pid
 *   The PID of the page object to check.
 * @param string $page
 *   The page index.
 * @param string $term
 *   The term to fetch the coordinates for.
 *
 * @return array
 *   The word coordinates for the given term and page.
 */
function islandora_internet_archive_bookreader_populate_boxes($pid, $page, $term) {
  $object = islandora_object_load($pid);
  if (!isset($object['ENCODED_OCR'])) {
    return array();
  }
  if (!($xml = @simplexml_load_string($object['ENCODED_OCR']->content))) {
    return array();
  }
  $namespaces = $xml->getDocNamespaces();
  // tesseract does not add a namespace so check for one
  $span = 'span';
  if (count($namespaces) > 0) {
    $xml->registerXPathNamespace('ns', $namespaces['']);
    $span = 'ns:span';
  }
  $lower_term = strtolower($term);
  $lower_term = urldecode($lower_term);
  $terms = explode(' ', $lower_term);
  $boxes = array();
  foreach ($terms as $term) {
    $matches = $xml->xpath("//{$span}[.='{$term}']");
    foreach ($matches as $match) {
      $bbox = (string) $match['title'];
      $values = explode(' ', $bbox);
      $boxes[] = array(
        'l' => intval($values[1]),
        't' => intval($values[2]) - 15,
        'r' => intval($values[3]) + 20,
        'b' => intval($values[4]) + 15,
        'page' => $page
      );
    }
  }
  return $boxes;
}
