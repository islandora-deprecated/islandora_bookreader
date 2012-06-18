<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01//EN" "http://www.w3.org/TR/html4/strict.dtd">

<html>
  <head>
    <title>Islandora Reader</title>

    <link rel="stylesheet" type="text/css" href="css/BookReader.css"/>
    <!-- Custom CSS overrides -->
    <link rel="stylesheet" type="text/css" href="css/BookReaderDemo.css"/>
    <link rel="stylesheet" type="text/css" href="css/mods2html.css"/>

    <script type="text/javascript" src="js/jquery-1.4.2.min.js"></script>
    <script type="text/javascript" src="js/jquery-ui-1.8.5.custom.min.js"></script>
    <script type="text/javascript" src="js/dragscrollable.js"></script>
    <script type="text/javascript" src="js/jquery.colorbox-min.js"></script>
    <script type="text/javascript" src="js/jquery.ui.ipad.js"></script>
    <script type="text/javascript" src="js/jquery.bt.min.js"></script>
    <script type="text/javascript" src="js/BookReader.js"></script>
    <script type="text/javascript" src="js/islandora_loader.js"></script>

  </head>
  <body style="background-color: #939598;">

    <div id="BookReader" style="left:10px; right:10px; top:10px; bottom:2em;">Loading Bookreader, please wait...</div>

    <script type="text/javascript">
      //
      // This file shows the minimum you need to provide to BookReader to display a book
      //
      // Copyright(c)2008-2009 Internet Archive. Software license AGPL version 3.

      // Create the BookReader object
      br = new BookReader();
      br.structMap = new Array();
      br.djatoka_prefix = islandora_params.DJATOKA_PREFIX;
      br.islandora_prefix = islandora_params.ISLANDORA_PREFIX;
      br.fedora_prefix = islandora_params.FEDORA_PREFIX;
      br.width = parseInt(islandora_params.page_width);
      br.height = parseInt(islandora_params.page_height);
      br.pageProgression = islandora_params.page_progression;
      br.structMap = islandora_params.book_pids;
      br.compression = islandora_params.COMPRESSION;
      br.baseUrl = islandora_params.base_url;
      br.module_path = islandora_params.module_path;
      br.getPageWidth = function(index) {
        return br.width;      
      }

      // Return the height of a given page.
      br.getPageHeight = function(index) {
        return br.height;
      }  

      // We load the images from fedora
      // using a different URL structure
      br.getPageURI = function(index, reduce, rotate) {
        // reduce and rotate are ignored in this simple implementation, but we
        // could e.g. look at reduce and load images from a different directory
        // or pass the information to an image server
        var leafStr = br.structMap[index+1];//get the pid of the object from the struct map islandora specific
       // var url = br.djatoka_prefix + br.islandora_prefix + leafStr + '/JP2/&svc_id=info:lanl-repo/svc/getRegion&svc_val_fmt=info:ofi/fmt:kev:mtx:jpeg2000&svc.format=image/png&svc.level=' + br.compression + '&svc.rotate=0';
       var url = br.djatoka_prefix + br.fedora_prefix + '/objects/' + leafStr + '/datastreams/JP2/content&svc_id=info:lanl-repo/svc/getRegion&svc_val_fmt=info:ofi/fmt:kev:mtx:jpeg2000&svc.format=image/png&svc.level=' + br.compression + '&svc.rotate=0';
        
        return url;
      }
      
      br.getOcrURI = function (index){
        var indices = br.getSpreadIndices(index);
        var pidL = br.structMap[indices[0]+1]; // pid for left page
        var pidR = br.structMap[indices[1]+1]; // pid for right page
        if (typeof pidL == 'undefined') { pidL = '-'; }
        if (typeof pidR == 'undefined') { pidR = '-'; }
        return br.baseUrl+"/bookreader/ocr/" + pidL + '/' + pidR ;
      }

      br.getModsURI = function(index) {
        //var leafStr = br.structMap[index+1];//get the pid of the object from the struct map islandora specific
        //return br.islandora_prefix + leafStr + "/MODS";
        //return "/mods2html/" + leafStr;
        var indices = br.getSpreadIndices(index);
        var pidL = br.structMap[indices[0]+1]; // pid for left page
        var pidR = br.structMap[indices[1]+1]; // pid for right page
        if (typeof pidL == 'undefined') { pidL = '-'; }
        if (typeof pidR == 'undefined') { pidR = '-'; }
        return br.baseUrl+"/mods2html/" + br.bookPid ;
      }

      br.getPid = function (index) {
        var leafStr = br.structMap[index+1];//get the pid of the object from the struct map islandora specific
        return leafStr;
      }

      // Return which side, left or right, that a given page should be displayed on
      br.getPageSide = function(index) {
        //$vals = ["R", "L"];
        //return $vals[index & 0x1];
        return br.pageProgression.toUpperCase()[1-(index & 0x1)]
      }

      // This function returns the left and right indices for the user-visible
      // spread that contains the given index.  The return values may be
      // null if there is no facing page or the index is invalid.
      br.getSpreadIndices = function(pindex) {
        var spreadIndices = [null, null];
        if ('rl' == this.pageProgression) {
          // Right to Left
          if (this.getPageSide(pindex) == 'R') {
            spreadIndices[1] = pindex;
            spreadIndices[0] = pindex + 1;
          } else {
            // Given index was LHS
            spreadIndices[0] = pindex;
            spreadIndices[1] = pindex - 1;
          }
        } else {
          // Left to right
          if (this.getPageSide(pindex) == 'L') {
            spreadIndices[0] = pindex;
            spreadIndices[1] = pindex + 1;
          } else {
            // Given index was RHS
            spreadIndices[1] = pindex;
            spreadIndices[0] = pindex - 1;
          }
        }

        return spreadIndices;
      }

      br.search = function(term) {

        var url = br.baseUrl + "/ocrsearch/" + br.bookPid + "/" + encodeURI(term)
        term = term.replace(/\//g, ' '); // strip slashes, since this goes in the url
        this.searchTerm = term;

        this.removeSearchResults();
        this.showProgressPopup('<img id="searchmarker" src="'+this.imagesBaseURL + 'marker_srch-on.png'+'"> Search results will appear below...');
        $.ajax({url:url, dataType:'json',
          success: function(data, status, xhr) {
            br.BRSearchCallback(data);
          },
          error: function() {
            alert("Search call to " + url + " failed");
          }
        });
      }

      // For a given "accessible page index" return the page number in the book.
      //
      // For example, index 5 might correspond to "Page 1" if there is front matter such
      // as a title page and table of contents.
      // for now we just show the image number
      br.getPageNum = function(index) {
        return index+1;
      }

      br.leafNumToIndex = function(index) {
        return index-1;
      }

      // Total number of leafs
      br.numLeafs = islandora_params.page_count;

      // Book title and the URL used for the book title link
      br.bookTitle = islandora_params.label;
      if (br.bookTitle.length > 100){
        br.bookTitle =  br.bookTitle.substring(0,97)+'...';
      }
      // book url should be created dynamically
      br.bookUrl = br.islandora_prefix + PID;
      br.bookPid = PID;
      // Override the path used to find UI images
      br.imagesBaseURL = 'images/';
      br.logoURL = ""; //don't want to go to LOC so init it empty, the title already takes us back to the book
      br.getEmbedCode = function(frameWidth, frameHeight, viewParams) {
        return "Embed code not supported in bookreader demo.";
      }

      
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
        br.search(query);
      }
      
      br.initUIStrings = function() {
    // Navigation handlers will be bound after all UI is in place -- makes moving icons between
    // the toolbar and nav bar easier

    // Setup tooltips -- later we could load these from a file for i18n
        var titles = { '.logo': 'Go to Archive.org', // $$$ update after getting OL record
                   '.zoom_in': 'Zoom in',
                   '.zoom_out': 'Zoom out',
                   '.onepg': 'One-page view',
                   '.twopg': 'Two-page view',
                   '.thumb': 'Thumbnail view',
                   '.print': 'Print this page',
                   '.embed': 'Embed BookReader',
                   '.link': 'Link to this book (and page)',
                   '.bookmark': 'Bookmark this page',
                   '.read': 'Read this book aloud',
                   '.share': 'Share this book',
                   '.info': 'Page Text',
                   '.full': 'Show fullscreen',
                   //'.book_left': 'Previous Page',
                   //'.book_right': 'Next Page',
                   '.book_up': 'Page up',
                   '.book_down': 'Page down',
                   '.play': 'Play',
                   '.pause': 'Pause',
                   '.BRdn': 'Show/hide nav bar', // Would have to keep updating on state change to have just "Hide nav bar"
                   '.BRup': 'Show/hide nav bar',
                   '.book_top': 'First page',
                   '.book_bottom': 'Last page',
                   '.full_text' : 'Full Text'
                  };
        if ('rl' == this.pageProgression) {
            titles['.book_leftmost'] = 'Last page';
            titles['.book_rightmost'] = 'First page';
            titles['.book_left'] = 'Next Page';
            titles['.book_right'] = 'Previous Page';
        } else { // LTR
            titles['.book_leftmost'] = 'First page';
            titles['.book_rightmost'] = 'Last page';
            titles['.book_left'] = 'Previous Page';
            titles['.book_right'] = 'Next Page';
        }

        for (var icon in titles) {
           if (titles.hasOwnProperty(icon)) {
               $('#BookReader').find(icon).attr('title', titles[icon]);
            }
        }
    }
      
     
      // Let's go!
      br.init();
      
      // read-aloud and search need backend compenents and are not supported in the demo
      $('#BRtoolbar').find('.read').hide();
      //$('#textSrch').hide();
      //$('#btnSrch').hide();

    </script>

  </body>
</html>
