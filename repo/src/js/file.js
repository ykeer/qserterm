// QML AJAX file read support

// read UTF8 text file
var readText = function(fn, onload){
	var req          = new XMLHttpRequest();
    req.onreadystatechange = function(){
    	if (req.readyState === XMLHttpRequest.DONE){
    		if (onload) onload( req.responseText );
		}
	}
	req.open('GET', fn);
	req.setRequestHeader('Content-Type', 'text/plain; charset=utf8');
	req.send();
}

// read binary file
var readBytes = function(fn, onload){
	var req          = new XMLHttpRequest();
    req.onreadystatechange = function(){
    	if (req.readyState === XMLHttpRequest.DONE){
    		if (onload){
    			var buf = [];
    			for (var i=0; i<req.responseText.length; i++) buf.push( req.responseText.charCodeAt(i) & 0xFF );
    			onload( buf );
			}
		}
	}
	req.open('GET', fn);
	req.setRequestHeader('Content-Type', 'text/plain; charset=x-user-defined');
	req.send();
}

