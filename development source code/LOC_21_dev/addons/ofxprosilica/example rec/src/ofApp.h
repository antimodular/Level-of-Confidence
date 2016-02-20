#pragma once

#include "ofMain.h"
#include "ofxProsilica.h"
#include "ofxGui.h"

class ofApp : public ofBaseApp{

public:
	void setup();
	void update();
	void draw();
	
	ofxProsilica::ConnectorWithInterface	cameraA;
	
	ofxPanel gui;
	ofParameter<int> fps;
	ofParameter<bool> bShowGui;
	
	ofTexture tex;
	
	void keyPressed(int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y );
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void windowResized(int w, int h);
	void dragEvent(ofDragInfo dragInfo);
	void gotMessage(ofMessage msg);
		
    ofImage recImg;
    bool bRecording;
};
