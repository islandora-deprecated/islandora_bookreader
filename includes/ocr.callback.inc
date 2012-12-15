<?php

/**
 * @file
 *
 * Handles the OCR menu callback.
 */

/**
 * Callback used by the BookReader for displaying the given object's OCR
 * data-streams.
 *
 * This function exits.
 *
 * @param FedoraObject $object
 *   The object whose OCR data-stream will be used for display.
 */
function islandora_bookreader_get_ocr(FedoraObject $object) {
  echo (isset($object['OCR']) ? htmlspecialchars($object['OCR']->content) : '');
  drupal_exit();
}
