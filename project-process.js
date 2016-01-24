$(document).ready(function () {
  var md = new Remarkable();

  // Show loading spinner
  $("#project-process").html("<div class=\"spinner\"><i class=\"fa fa-spinner fa-spin\"></i></div>");

  // Fetch the file contents.
  $.ajax({
    type: "GET",
    url: "https://api.github.com/repos/pendla/formula-ml/contents/documentation/progress_report.md",
    beforeSend: function (xhr) {
      xhr.setRequestHeader("Authorization", "Basic cHJhc2FkLWZvcm11bGEtbWw6cHJhc2FkMDc1NA==");
    },
    success: function (file) {
      var content = "<div id=\"entry-0\">" + md.render(atob(file.content)) + "</div>";
      $("#project-process").html(content);
    }
  });
});
