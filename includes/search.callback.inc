<?php

/**
 * @file
 *
 * Handles the search callback.
 */

/**
 * Performs a solr search for the given term within the given book object's
 * pages using SOLR. Returns a JSON object for use in the BookReader.
 *
 * This function exits.
 *
 * @param string $object_id
 *   The PID of the book object.
 * @param type $ocr_term
 *   The text to search for within the OCR'ed text.
 */
function islandora_bookviewer_search($object_id, $term) {
  if (!module_exists('islandora_solr')) {
    drupal_exit();
  }
  $results = islandora_bookviewer_query_solr($object_id, $term);
  $results = islandora_bookviewer_map_solr_results_to_bookviewer_results($results, $term);
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
function islandora_bookviewer_query_solr($object_id, $term) {
  $url = islandora_bookviewer_get_solr_search_url($object_id, $term);
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
function islandora_bookviewer_map_solr_results_to_bookreader_results(array $results, $term) {
  $ocr_field = variable_get('islandora_bookreader_ocr_field', 'OCR.OCR');
  $sequence_field = variable_get('islandora_bookreader_sequence_number', 'rels.isSequenceNumber');
  $output = array('matches' => array());
  foreach ($results['response']['docs'] as $doc) {
    // @todo Index the page width and height in solr, rather than fetch from
    // djatoka
    $pid = $doc['PID'];
    $page_data = islandora_bookviewer_get_page_data($pid);
    $page = $doc["$sequence_field"][0];
    $highlighting = $results['highlighting'];
    $highlighted_text = $highlighting[$pid][$ocr_field][0];
    $output['matches'][] = array(
      'text' => preg_replace('/[^a-zA-Z0-9-\n {}]/', '', $highlighted_text),
      'par' => array(
        array(
          'page' => intval($page),
          'page_width' => $page_data->width,
          'page_height' => $page_data->height,
          'boxes' => islandora_bookviewer_populate_boxes($pid, $page, $term),
        ),
      )
    );
  }
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
function islandora_bookviewer_get_solr_search_url($object_id, $term) {
  $request_handler = variable_get('islandora_solr_request_handler', '');
  $solr_url = variable_get('islandora_solr_url', 'localhost:8080/solr');
  $request_handler = variable_get('islandora_solr_search_block_request_handler', 'standard');
  // Allow multiple words
  $term = htmlspecialchars(drupal_encode_path($term), ENT_QUOTES, 'utf-8', FALSE);
  // Not configurable yet, not exposed in the form.
  $sequence_field = variable_get('islandora_bookviewer_sequence_number', 'rels.isSequenceNumber');
  $ocr_field = variable_get('islandora_bookviewer_ocr_field', 'OCR.OCR');
  $ocr_filter_field = variable_get('islandora_bookviewer_ocr_filter_field', 'rels.isMemberOf');
  $num_rows = variable_get('islandora_bookviewer_solr_result_count', 32);
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
 * Fetches the given page's metadata via Djatoka.
 *
 * @todo Index page width and height in solr so we don't need to call this for
 * every result
 *
 * @param string $object_id
 *   The PID of the page to fetch the metadata for.
 *
 * @return array
 *   The given page's metadata.
 */
function islandora_bookviewer_get_page_data($object_id) {
  $datastream_url = url("islandora/object/{$object_id}/datastream/JP2/view", array('absolute' => TRUE));
  $djatoka_params = array(
    'url_ver=Z39.88-2004',
    "rft_id={$datastream_url}",
    'svc_id=info:lanl-repo/svc/getMetadata'
  );
  $djatoka_params = implode('&', $djatoka_params);
  $djatoka_url = variable_get('islandora_bookviewer_djatoka_path', 'http://localhost:8080/adore-djatoka');
  $djatoka_url .= ($djatoka_url[strlen($djatoka_url)-1] == '/') ? '' : '/';
  $djatoka_url .= "resolver?{$djatoka_params}";
  $request = drupal_http_request($djatoka_url);
  if ($request->code == '200') {
    return drupal_json_decode($request->data);
  }
  return array();
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
function islandora_bookviewer_populate_boxes($pid, $page, $term) {
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
  foreach ($termerms as $term) {
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
