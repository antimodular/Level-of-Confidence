#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){

//	ofSetVerticalSync(false);
	
	ofSetLogLevel(OF_LOG_VERBOSE);
	cameraA.listDevices();
	ofSetLogLevel(OF_LOG_NOTICE);
	
	// pre setup features
	cameraA.setDeviceID(117771); // for the interface to work correctly set the deviceID
     //tym mako 6002281
//	cameraA.setPersistentIpAdress(10.0.0.66);
//	cameraA.setPersistentIpSubnetMask(255.0.0.0);
//	cameraA.setPersistentIp(true);
    cameraA.setPixelFormat(OF_PIXELS_RGB);
	cameraA.setup();
	 
    
    while(cameraA.isInitialized() == false){
        cameraA.update();
        //  cout<<"camWidth "<<camWidth<<" , "<<camHeight<<endl;
    }

    
    
	gui.setup("settings");
	gui.add(fps.set("FPS", 0, 0, 100));
	gui.add(bShowGui.set("Show Gui (tilde)", true));
	gui.add(cameraA.parameters);
	gui.loadFromFile("settings.xml");
	
}

//--------------------------------------------------------------
void ofApp::update(){
	cameraA.update();
	fps.set(ofGetFrameRate() + 0.5);
}

//--------------------------------------------------------------
void ofApp::draw(){
	ofClear(0);
	if (cameraA.isFrameNew()){
		int w = cameraA.getWidth();
		int h = cameraA.getHeight();
		int glFormat = ofGetGLInternalFormatFromPixelFormat(cameraA.getPixelFormat());
       // ofLog()<<"glFormat "<<glFormat;
		if (tex.getWidth() != w || tex.getHeight() != h || tex.getTextureData().glTypeInternal != glFormat) {
			tex.clear();
			tex.allocate(w, h, glFormat);
		}
				
		tex.loadData(cameraA.getPixels(), w, h, glFormat);
	}
	
	tex.draw(220,0);
	
	if (bShowGui.get())
		gui.draw();
	
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	switch (key) {
		case ' ':
			//			cameraA.close();
			break;
		case '~':
			bShowGui.set(1 - bShowGui.get());
			break;
			
		default:
			break;
	}
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
