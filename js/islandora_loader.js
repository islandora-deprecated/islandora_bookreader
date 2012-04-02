//function to parse urls

$.urlParam = function(name){
  var results = new RegExp('[\?&]' + name + '=([^&#]*)').exec(window.location.href);
  if (!results)
  {
    return 0;
  }
  return results[1] || 0;
}

PID = $.urlParam('pid');

//determine base of Drupal installation

var here = window.location.toString();
var splitter = here.indexOf('/sites/');
if(splitter > 0){
  splitter = '/sites/';
}else{
  splitter = '/modules/';
}
var base = here.split(splitter);
base = base[0];


// retreive setup info from Drupal callback

$.ajax({
  url: base + '/bookreader/setup/' + PID,
  async:false,
  success: function(data, status, xhr) {
    islandora_params = data;
  },
  error: function() {
    alert("AJAX call failed");
  },
  dataType: 'json'
});