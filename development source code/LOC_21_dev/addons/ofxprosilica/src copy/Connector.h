#pragma once

#include "ofMain.h"
#include "Camera.h"


namespace ofxProsilica {
	
	class Connector : public Camera {
	public:
		
		Connector();
		virtual ~Connector();
		
		bool	setup();
		
		void	update();
		
	protected:
		bool	initConnector();
		void	connect();
		int		maxReconnectAttempts;
		int		reconnectCounter;
		float	reconnectInterval;
		float	nextReconnectAttemptTime;
		bool	bTryingToConnect;
		
	};
}