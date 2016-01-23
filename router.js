$(document).ready(function () {
	loadDynamicContent('header', 'header');

	var page = getUrlParameter('page');
	if (page) {
		if (page == "meetings") {
			return loadDynamicContent('meetings');
		} else if (page == "meetings-prasad") {
			return loadDynamicContent('meetings-prasad');
		} else if (page == "mails") {
			return loadDynamicContent('mails');
		}
	}
	return loadDynamicContent('project-process');
});

/**
 * Helper method to load content from different html files
 * dynamically.
 */
function loadDynamicContent (fileName, wrapperId) {
	var wrapper = wrapperId ? "#" + wrapperId : "#content";
	$(wrapper).load(fileName + ".html"); 
}

/**
 * Props to Sameer Kazi and Rob Evans from stack overflow.
 * http://stackoverflow.com/questions/19491336/get-url-parameter-jquery
 * because I was lazy enough not to do it mysel
 */
function getUrlParameter(sParam) {
    var sPageURL = decodeURIComponent(window.location.search.substring(1)),
        sURLVariables = sPageURL.split('&'),
        sParameterName,
        i;

    for (i = 0; i < sURLVariables.length; i++) {
        sParameterName = sURLVariables[i].split('=');

        if (sParameterName[0] === sParam) {
            return sParameterName[1] === undefined ? true : sParameterName[1];
        }
    }
};