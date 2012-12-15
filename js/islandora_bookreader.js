/**
 * @file
 *
 * Defines initializing/attaching the Book Reader to the defined element.
 */
(function ($) {
  Drupal.behaviors.islandoraBookReader = {
    attach: function(context, settings) {
      $('.islandora-bookreader', context).once('islandora-bookreader', function () {
        var bookReader = new IslandoraBookReader(settings.islandoraBookReader);

        /**
         * @todo reorganize this
         *
        function getURLParam(name) {
          // get query string part of url into its own variable
          var url = window.location.href;
          var query_string = url.split("?");
          // make array of all name/value pairs in query string
          var params = query_string[1].split(/\&|#/);
          // loop through the parameters
          var i = 0;
          while (params.length > i) {
            // compare param name against arg passed in
            var param_item = params[i].split("=");
            if (param_item[0] == name) {
              // if they match, return the value
              return param_item[1];
            }
            i++;
          }
          return "";
        }
         var query = getURLParam("solrq");
        if (query != "") {
          bookReader.search(query);
        }*/

        // Initialize and Render the BookReader.
        bookReader.init();
        // bookReader.search('church');
        // We currently don't support read-aloud.
        $('#BRtoolbar').find('.read').hide();
        if (!bookReader.searchEnabled()) {
          $('#textSrch').hide();
          $('#btnSrch').hide();
        }
      });
    }
  };
})(jQuery);
