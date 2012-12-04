<?php

/**
 * @file
 *
 * Any hooks/functions used by this modules theme's.
 */

/**
 * Implements hook_preprocess_theme().
 */
function islandora_bookviewer_preprocess_islandora_bookreader_mods(array &$variables) {
  $variables['mods'] = '';
  $object = $variables['object'];
  if (isset($object['MODS'])) {
    $stylesheet = new DOMDocument();
    $stylesheet->load(drupal_get_path('module', 'islandora_bookviewer') . "/mods2html.xsl");
    $proc = new XSLTProcessor();
    $proc->importStyleSheet($stylesheet);
    $mods = new DOMDocument();
    $mods->loadXML($object['MODS']->content);
    $variables['mods'] = $proc->transformToXML($mods);
  }
}

/**
 * Implements hook_preprocess_theme().
 */
function islandora_bookviewer_preprocess_islandora_bookreader_ocr(array &$variables) {
  $variables['left_ocr'] = $variables['right_ocr'] = '';
  $left_object = $variables['left_object'];
  $right_object = $variables['right_object'];
  if (isset($left_object['OCR'])) {
    $variables['left_ocr'] = htmlspecialchars($left_object['OCR']);
  }
  if (isset($right_object['OCR'])) {
    $variables['right_ocr'] = htmlspecialchars($right_object['OCR']);
  }
}
