<?php

/**
 * @file
 *
 * The IslandoraBookViewer class.
 *
 * @todo change to be procedural.
 */


/**
 *
 */
class IslandoraBookviewer {

  /**
   * Instantiate this object.
   *
   * @param FedoraObject $object
   *   The object the viewer will display.
   */
  function __construct(FedoraObject $object) {
    $this->object = $object;
  }

  /**
   * An ajax callback has been added in the module
   */
  function showAjaxBookViewer($page = 'mainpage.php', $start_page='1') {
    global $base_url;
    $path = drupal_get_path('module', 'islandora_bookviewer');
    $fullPath = $path . '/';

    // Check for a query string - this will be a search-inside query
    $query = $_GET["solrq"];

    // Currently we are loading this as a iframe.
    // can be embeded but it looks terrible when embedded.  the css would have to be rewritten as most things are set as absolute
    // this will take some time but eventually would be worthwhile as the index php does not have access to drupal functions/variables when loaded as
    // an iframe
    $viewer_url = $base_url . '/' . $fullPath . "$page?pid=" . $this->object->id . '&label=' . $this->object->label .
      '&showIcon=' . variable_get('islandora_bookviewer_show_ia_logo', '1');
    // if we have a search query then pass it to the viewer (to parse in book)
    if ($query != "") {
      $viewer_url = $viewer_url . '&solrq=' . $query;
    }
    $viewer_url .= '#page/' . $start_page . '/mode/' . variable_get('islandora_bookviewer_default_view', '2up');
    $html = '<iframe id="iabookviewer" name="iabookviewer" src="' . $viewer_url . '" scrolling="0" frameborder="0" style="width: 100%; height: 800px;">Errors: unable to load viewer</iframe>';
    return $html;
  }

  /**
   * tabs: the html to display, in the form of a set of tabs
   *
   * @return array
   */
  function showBookStreams() {
    //get the book viewer iframe
    $viewer = $this->showBookViewer();
    //set up tabs
    $tabs = array('#type' => 'tabset');
    // note the fedora item
    // get metadata info
    $metadata = $this->getMetaData();

    $tabs['View'] = array(
      '#type' => 'tabpage',
      '#title' => t('View'),
      '#content' => $viewer,
      '#weight' => -1
    );
    // return the tabs for display
    return $tabs;
  }

  /**
   *
   * @return output: the html of data to display
   *
   * Small modification of UoM's exif plugin
   *
   * @todo Theme this
   */
  function getMetaData() {
    $output = '';
    $mods = isset($this->object['MODS']) ? $this->object['MODS']->content; '';
    if (trim($mods) != '') {
      $document = DOMDocument::loadXML($mods);
      if ($document != NULL) {
        $description = $document->getElementsByTagName('*');
        if ($description->length > 0) {
          $description = $description->item(0);
          $output .= '<div class="fedora_technical_metadata"><ul>';
          for ($i = 0; $i < $description->childNodes->length; $i++) {
            $name = $description->childNodes->item($i)->nodeName;
            $value = $description->childNodes->item($i)->nodeValue;
            if ($name != '#text' && !preg_match('/^System\:.*$/', $name) && trim($value) != '') {
              list($type, $name) = preg_split('/\:/', $name);
              $name = trim(preg_replace('/(?<!^)([A-Z][a-z]|(?<=[a-z])[A-Z])/', " $1", $name));
              $output .= '<li><b>' . $name . '</b>:  ' . $value . ' </li>';
            }
          }
          $output .= '</ul></div>';
        }
      }
    }
    return $output;
  }

}
