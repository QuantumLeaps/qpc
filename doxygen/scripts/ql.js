// tooltips ----------------------------------------------------------------
$(function() {
  $('.questionmark_help').tooltip({
    delay: 0,
      showURL: false,
	  track: true,
	  showBody: " - ", 
      fade: 250
  });
});
$(function() {
  $('.preview_board').tooltip({
    delay: 0,
    fade: 250,
    showURL: false,
    bodyHandler: function() {
      return $("<img/>").attr("src", this.href);
    }
  });
});
