//function to parse urls

$.urlParam = function(name){
  var results = new RegExp('[\?&]' + name + '=([^&#]*)').exec(window.location.href);
 
  return results[1] || 0;
}

var here = window.location.toString();
var splitter = here.indexOf('/sites/');
if (splitter > 0) {
  splitter = '/sites';
}
else {
  splitter = '/modules/';
}
var base = here.split(splitter)[0];

PID = $.urlParam('pid');

$.ajax({
  url:base + '/bookreader/setup/' + PID,
  async:false,
  success: function(data, status, xhr) {
    islandora_params = data;
  },
  error: function() {
    alert("AJAX call failed");
  },
  dataType: 'json'
});
