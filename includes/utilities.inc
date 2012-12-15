<?php

/**
 * @file
 *
 */

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
function islandora_internet_archive_bookreader_get_page_data($object_id) {
  $datastream_url = url("islandora/object/{$object_id}/datastream/JP2/view", array('absolute' => TRUE));
  $djatoka_params = array(
    'url_ver=Z39.88-2004',
    "rft_id={$datastream_url}",
    'svc_id=info:lanl-repo/svc/getMetadata'
  );
  $djatoka_params = implode('&', $djatoka_params);
  $djatoka_url = variable_get('islandora_internet_archive_bookreader_djatoka_url', 'http://localhost:8080/adore-djatoka');
  $djatoka_url .= ($djatoka_url[strlen($djatoka_url)-1] == '/') ? '' : '/';
  $djatoka_url .= "resolver?{$djatoka_params}";
  $request = drupal_http_request($djatoka_url);
  if ($request->code == '200') {
    return drupal_json_decode($request->data);
  }
  return array();
}
