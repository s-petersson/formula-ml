$(document).ready(function () {
  var md = new Remarkable();

  $.ajax({
    type: "GET",
    url: "https://api.github.com/repos/pendla/formula-ml/contents/documentation/meetings_with_prasad",
    beforeSend: function (xhr) {
      xhr.setRequestHeader("Authorization", "Basic cHJhc2FkLWZvcm11bGEtbWw6cHJhc2FkMDc1NA==");
    },
    success: function (entries) {
      var fullEntries = [];
      // Sort them by name (should be dates)
      entries.sort(function (left, right) {
        var leftName = left.name;
        var rightName = right.name;
        if (leftName < rightName) {
          return -1;
        } else if (rightName < leftName) {
          return 1;
        } else {
          return 0;
        }
      });

      // Loop over all entries and create new entries in the markup for
      // them. So that we can show a spinner and display the entries
      // in the correct order, independent of which ones load faster.
      for(var i = 0; i < entries.length; i++) {
        var id = "entry-" + entries[i].name.substr(0, 6);
        $("#meetings-prasad").append("<div id=\"" + id + "\"" + "></div>");
        $("#" + id).html("<div class=\"spinner\"><i class=\"fa fa-spinner fa-spin\"></i></div>");
      }

      for(var i = 0; i < entries.length; i++) {
        $.ajax({
          type: "GET",
          url: entries[i].url,
          beforeSend: function (xhr) {
            xhr.setRequestHeader("Authorization", "Basic cHJhc2FkLWZvcm11bGEtbWw6cHJhc2FkMDc1NA==");
          },
          success: function (entry) {
            var id = "entry-"+entry.name.substr(0, 6);
            $("#" + id).html(md.render(atob(entry.content)));
          }
        });
      }
    }
  });
});
