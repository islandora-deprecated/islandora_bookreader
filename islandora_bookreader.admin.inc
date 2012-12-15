<?php

/**
 * @file
 *
 * Handles the display/submission of the admin settings form for this module.
 */

/**
 * Defines the admin settings form.
 *
 * @param array $form
 *   The Drupal form definition.
 * @param array $form_state
 *   The Drupal form state.
 *
 * @return array
 *   The Drupal form definition.
 */
function islandora_bookreader_admin_settings_form(array $form, array &$form_state) {
  $get_default_value = function($name, $default) use($form_state) {
    return isset($form_state['values'][$name]) ? $form_state['values'][$name] : variable_get($name, $default);
  };
  $djatoka_url = $get_default_value('islandora_bookreader_djatoka_path', 'http://localhost:8080/adore-djatoka/');
  $djatoka_availible_message = islandora_bookreader_admin_settings_form_djatoka_availible_message($djatoka_url);
  $form = array(
    'islandora_bookreader_show_ia_logo' => array(
      '#type' => 'checkbox',
      '#title' => t('Display Internet Archive BookReader icon'),
      '#description' => t('Display icon and link in the upper left corner.'),
      '#default_value' => $get_default_value('islandora_bookreader_show_ia_logo', TRUE)
    ),
    'islandora_bookreader_djatoka_path' => array(
      '#type' => 'textfield',
      '#prefix' => '<div id="djatoka-path-wrapper">',
      '#suffix' => '</div>',
      '#title' => t('Djatoka URL'),
      '#description' => t('URL to Djatoka instance.<br/>!msg', array('!msg' => $djatoka_availible_message)),
      '#default_value' => $djatoka_url,
      '#ajax' => array(
        'callback' => 'islandora_bookreader_admin_settings_form_djatoka_ajax_callback',
        'wrapper' => 'djatoka-path-wrapper',
        'effect' => 'fade',
        'event' => 'change'
      )
    ),
    'islandora_bookreader_ocr_field' => array(
      '#type' => 'textfield',
      '#title' => t('SOLR full text field name'),
      '#description' => t('The SOLR field used for full text search.'),
      '#default_value' => $get_default_value('islandora_bookreader_ocr_field', 'OCR.OCR')
    ),
    'islandora_bookreader_ocr_filter_field' => array(
      '#type' => 'textfield',
      '#title' => t('Solr field that has the relationship to the book object pid'),
      '#description' => t('When we search within a book we need to know the pid of the book. This would be the solr field that contains the book pid for each page object.'),
      '#default_value' => $get_default_value('islandora_bookreader_ocr_filter_field', 'rels.isMemberOf')
    ),
    'islandora_bookreader_sequence_number' => array(
      '#type' => 'textfield',
      '#title' => t('A field that holds the page sequence numbers'),
      '#description' => t('This field should contain the page or sequence number of each page/image.'),
      '#default_value' => $get_default_value('islandora_bookreader_sequence_number', 'rels.isSequenceNumber')
    ),
    'islandora_bookreader_compression' => array(
      '#type' => 'textfield',
      '#title' => t('The djatoka image compression level'),
      '#description' => t('The level of compression we want djatoka to use when creating the image, usually set at 4 or 5.'),
      '#element_validate' => array('element_validate_number'),
      '#default_value' => $get_default_value('islandora_bookreader_compression', '4')
    ),
    'islandora_bookreader_solr_result_count' => array(
      '#type' => 'textfield',
      '#title' => t('The maximum number of results to return in a solr query'),
      '#description' => t('The maximum number of pages that will be returned by a full text search.'),
      '#element_validate' => array('element_validate_number'),
      '#default_value' => $get_default_value('islandora_bookreader_solr_result_count', '32')
    )
  );
  return system_settings_form($form);
}

/**
 * Gets a message to display if the djatoka is accessible or not.
 *
 * @param string $djatoka_url
 *   The url to an djatoka instance typically
 *   http://localhost:8080/adore-djatoka.
 *
 * @return string
 *   A message in html detailing if the djatoka is accessible.
 */
function islandora_bookreader_admin_settings_form_djatoka_availible_message($djatoka_url) {
  $file_headers = @get_headers($djatoka_url);
  $djatoka_availible = strstr($file_headers[0], '200') !== FALSE;
  if ($djatoka_availible) {
    $image = theme_image(array('path' => 'misc/watchdog-ok.png', 'attributes' => array()));
    $message = t('Djatoka url is valid.');
  }
  else {
    $image = theme_image(array('path' => 'misc/watchdog-error.png', 'attributes' => array()));
    $message = t('Unable to locate djatoka installation at @path', array('@path' => $djatoka_url));
  }
  return $image . $message;
}

/**
 * Ajax callback for the djatoka url textfield
 *
 * @param array $form
 *   The Drupal form definition.
 * @param array $form_state
 *   The Drupal form state.
 *
 * @return array
 *   The element to render as part the ajax callback.
 */
function islandora_bookreader_admin_settings_form_djatoka_ajax_callback(array $form, array &$form_state) {
  return $form['islandora_bookreader_djatoka_path'];
}
