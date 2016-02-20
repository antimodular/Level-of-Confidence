#include "Connector.h"


namespace ofxProsilica {
	
	Connector::Connector(){
	}
	
	
	Connector::~Connector(){
	}
	
	
	bool Connector::setup() {
		return initConnector();
	}
	
	bool Connector::initConnector() {
		maxReconnectAttempts = 100;
		reconnectCounter = 0;
		reconnectInterval = 2; // in seconds
		
		bTryingToConnect = true;
		nextReconnectAttemptTime = ofGetElapsedTimef() + reconnectInterval;
		
		connect();
		return true;
	}
	
	void Connector::update(){
		Camera::update();
		if (!bInitialized)
			connect();
	}
	
	void Connector::connect() {
		if (bTryingToConnect) {
			if (reconnectCounter < maxReconnectAttempts) {
				if (nextReconnectAttemptTime < ofGetElapsedTimef()) {
					reconnectCounter++;
					reconnectInterval += reconnectInterval;
					if (reconnectInterval > 64) reconnectInterval = 64;
					nextReconnectAttemptTime = ofGetElapsedTimef() + reconnectInterval;
					ofLogNotice("Connector") << deviceID << " Trying to (re)connect to camera";
					listDevices();
					if (Camera::setup()) {
						ofLogVerbose("Connector") << deviceID << " Connection succeeded";
						reconnectCounter = 0;
						reconnectInterval = 2;
					}
					else
						ofLogNotice("Connector") << deviceID << " Connection failed, trying again in " << reconnectInterval << " seconds";
				}
			}
			else {
				bTryingToConnect = false;
				ofLogNotice("Connector") << deviceID << " Stopped Trying to Connect after " <<reconnectCounter << " Times";
			}
		}
	}
	
}
