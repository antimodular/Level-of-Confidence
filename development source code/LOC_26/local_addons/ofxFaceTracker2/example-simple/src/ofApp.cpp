#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    // All examples share data files from example-data, so setting data path to this folder
    // This is only relevant for the example apps
//    ofSetDataPathRoot(ofFile(__BASE_FILE__).getEnclosingDirectory()+"../../model/");
    
    // Setup grabber
    grabber.setDeviceID(1);
    grabber.setup(1920,1080);
    
    // Setup tracker
    tracker.setup(ofToDataPath("model_dlib/shape_predictor_68_face_landmarks.dat"));
    
    //--generate all 16:9 resolutions up to camWidth
    int hori_ratio = 16;
    int verti_ratio = 9;
    
    int rotatedWidth  = 1920;
    int _c = 1;
    
    while ((hori_ratio*_c) <=rotatedWidth) {
        //        if(hori_ratio*_c >= 320){
        if(hori_ratio*_c >= 160){
            allResolutions.push_back(ofVec2f(hori_ratio*_c,verti_ratio*_c));
            ofLog()<<_c<<" allResolutions "<<allResolutions.back();
        }
        _c++;
    }
    
    detectorImageScaler = 0.35;
    int tracker_persistence = 2;
    int tracker_maxDistance = 130;
    
    ofLog()<<"min tracking resolution in array = "<<allResolutions[0];
    ofLog()<<"max tracking resolution in array = "<<allResolutions[allResolutions.size()-1];
    int c = ofMap(detectorImageScaler, 0, 1, 0, allResolutions.size()-1,true);

    detectorWidth = allResolutions[c].x; // camWidth; //1280; //
    detectorHeight = allResolutions[c].y; //720; //camHeight; //720; //
    //    detectorImageScaler = 1;
//    tracker.setFaceDetectorImageSize(detectorWidth*detectorHeight);
    
    tracker.setPersistence(tracker_persistence);
    tracker.setMaximumDistance(tracker_maxDistance);
    
}

//--------------------------------------------------------------
void ofApp::update(){
    grabber.update();
    
    // Update tracker when there are new frames
    if(grabber.isFrameNew()){
        tracker.update(grabber);
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    // Draw camera image
    grabber.draw(0, 0);
    
    // Draw tracker landmarks
    tracker.drawDebug();
    
    // Draw estimated 3d pose
    tracker.drawDebugPose();
    
    // Draw text UI
    ofDrawBitmapStringHighlight("Framerate : "+ofToString(ofGetFrameRate()), 10, 20);
    ofDrawBitmapStringHighlight("Tracker thread framerate : "+ofToString(tracker.getThreadFps()), 10, 40);
    
#ifndef __OPTIMIZE__
    ofSetColor(ofColor::red);
    ofDrawBitmapString("Warning! Run this app in release mode to get proper performance!",10,60);
    ofSetColor(ofColor::white);
#endif
}
