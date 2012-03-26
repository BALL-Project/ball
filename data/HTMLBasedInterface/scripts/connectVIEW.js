// include this file (connectVIEW.js) to every webpage that
// shall be able to react on VIEW messages

function onJSCompositeMessage(i) {
	
	alert("Received Composite Message " + i);

}

try
{
	// connect handler method to the fireJSCompositeMessage signal
	// of the exposed HTMLBasedInterface object
	mywebview.fireJSCompositeMessage.connect(onJSCompositeMessage);
}
catch (e)
{
	// if connection does not work an exception is thrown
	alert(e);
}

function onJSRepresentationMessage(i) {
	
	alert("Received Representation Message " + i);

}

try
{
	// connect handler method to the fireJSRepresentationMessage signal
	// of the exposed HTMLBasedInterface object
	mywebview.fireJSRepresentationMessage.connect(onJSRepresentationMessage);
}
catch (e)
{
	// if connection does not work an exception is thrown
	alert(e);
}

function onJSSceneMessage(i) {
	
	alert("Received Scene Message " + i);

}

try
{
	// connect handler method to the fireJSSceneMessage signal
	// of the exposed HTMLBasedInterface object
	mywebview.fireJSSceneMessage.connect(onJSSceneMessage);
}
catch (e)
{
	// if connection does not work an exception is thrown
	alert(e);
}