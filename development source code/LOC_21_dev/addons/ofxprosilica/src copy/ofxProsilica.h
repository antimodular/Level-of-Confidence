/*	--------------------------------------------------------------------
	ofxProsilica is an Open Frameworks wrapper for Allied Vision GigE 
	Cameras
*///--------------------------------------------------------------------


/*	--------------------------------------------------------------------
	ofxProsilica::BaseGrabber is modelled after ofBaseVideoGrabber.
*///--------------------------------------------------------------------
#include "Camera.h"

/*	--------------------------------------------------------------------
	ofxProsilica::Connector deals with the (long) time it can take for 
	the	camera to be found and available if it was just use by an
	(other) application.
	Also tries to reconnect to the camera when connection is lost.
*///--------------------------------------------------------------------
#include "Connector.h"

/*	--------------------------------------------------------------------
	ofxProsilica::ConnectorWithInterface adds an ofParameterGroup to 
	the Connector. (for easy use with ofxGui)
*///--------------------------------------------------------------------
#include "ConnectorWithInterface.h"

/*	--------------------------------------------------------------------
	ofxProsilica::ThreadedConnector is an experimental version of the 
	ConnectorWithInterface, in which the Connector runs threaded
	Note: Most Attributes are only accesible through the 
	ofParameterGroup 'parameters'
 *///--------------------------------------------------------------------
#include "ThreadedConnector.h"