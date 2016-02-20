#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    
    //	ofSetVerticalSync(false);
    
    ofSetFrameRate(30);
    ofSetLogLevel(OF_LOG_VERBOSE);
    cameraA.listDevices();
    ofSetLogLevel(OF_LOG_NOTICE);
    
    // pre setup features
    cameraA.setDeviceID(117771); // for the interface to work correctly set the deviceID
    //tym mako 6002281
    //network settings are at DHCP and we get a self assigned IP of 196.254.?.?
    //the camera's ip should be set to 169.254.x.x probably via a windows PC with gige config on it
    //ge1910 115230 marked with ip 169.254.28.66
    //ge1910 115234 SN:02-2083C-06190
    //ge1910 117771 SN:
    //	cameraA.setPersistentIpAdress(10.0.0.66);
    //	cameraA.setPersistentIpSubnetMask(255.0.0.0);
    //	cameraA.setPersistentIp(true);
    cameraA.setPixelFormat(OF_PIXELS_RGB);
    cameraA.setup();
    
    while(cameraA.isInitialized() == false){
        cameraA.update();
        //  cout<<"camWidth "<<camWidth<<" , "<<camHeight<<endl;
    }
    
    recImg.allocate(cameraA.getWidth(), cameraA.getHeight(), OF_IMAGE_COLOR);
    
    cout<<"camWidth "<<cameraA.getWidth()<<" , "<<cameraA.getHeight()<<endl;
    
    gui.setup("settings");
    gui.add(fps.set("FPS", 0, 0, 100));
    gui.add(bShowGui.set("Show Gui (tilde)", true));
    gui.add(cameraA.parameters);
    gui.loadFromFile("settings.xml");
    
}

//--------------------------------------------------------------
void ofApp::update(){
    
    fps.set(ofGetFrameRate() + 0.5);
    cameraA.update();
    
    if (cameraA.isFrameNew()){
        int w = cameraA.getWidth();
        int h = cameraA.getHeight();
        int glFormat = ofGetGLInternalFormatFromPixelFormat(cameraA.getPixelFormat());
        // ofLog()<<"glFormat "<<glFormat;
        if (tex.getWidth() != w || tex.getHeight() != h || tex.getTextureData().glTypeInternal != glFormat) {
            tex.clear();
            tex.allocate(w, h, glFormat);
        }
        
        recImg.setFromPixels(cameraA.getPixelsRef());
        // tex.loadData(cameraA.getPixels(), w, h, glFormat);
        
        if(bRecording){
            recImg.saveImage("images/"+ofGetTimestampString()+".jpg");
        }
    }
    
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofClear(0);
    
    ofSetColor(255);
    recImg.draw(220,0);
    
    if (bShowGui.get()){
        gui.draw();
    }
    
    if(bRecording){
        ofSetColor(255, 0, 0);
        ofCircle(ofGetWidth()-30, 30, 10);
    }
    
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    switch (key) {
        case ' ':
            //			cameraA.close();
            break;
        case 'g':
            bShowGui.set(1 - bShowGui.get());
            break;
            
        case 'r':
            bRecording = true;
            break;
            
        case 'x':
            bRecording = false;
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
