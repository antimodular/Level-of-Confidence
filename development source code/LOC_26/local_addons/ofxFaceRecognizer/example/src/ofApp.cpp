#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){

    ofSetVerticalSync(true);
    ofSetFrameRate(120);
    objectFinder.setup("haarcascade_frontalface_default.xml");
    objectFinder.setPreset(ObjectFinder::Fast);
    cam.setDeviceID(1);
    cam.setup(640, 480);
    cropped.allocate(150,150, OF_IMAGE_COLOR);

    
    //-----------faceRecognizer
    // 0 = Eigenfaces_method ; has much bigger database (.yml). takes longer to load
    // 1 = Fisherfaces_method; faster to load
    // 2 = LBPHFaces_method
    
    //int maxFaces, bool bAlreadySavedModel, string folderName
    //true means we already have a model.yml file in the data folder
    //false the addon will generate one based on the images found in data/faces folder
    recognizer.setup(132,true,"faces");
    cout<<recognizer.getUniquePersonCount()<<" seperate people are in the training database"<<endl;
    
}

//--------------------------------------------------------------
void ofApp::update(){

    cam.update();
    if(cam.isFrameNew()) {
        objectFinder.update(cam);
        
        if(objectFinder.size() > 0) {
            //when using the haarcascade method of face detection the returned images are 150x150 pix
            //the face regonizer is also trained on this size of images
            //make sure that training images and newly detected faces always have the same res
            cv::Rect roi = toCv(objectFinder.getObject(0));
            Mat camMat = toCv(cam);
            Mat croppedCamMat(camMat, roi);
            resize(croppedCamMat, cropped);
            cropped.update();
            
            Mat face;
            //recognizer wants grayscale image
            cvtColor(toCv(cropped), face, CV_BGR2GRAY);
             ofLog()<<"face rows "<<face.rows<<" cols "<<face.cols;
            recognizer.update(face);
            
            cout<<"recognizer.getConfidence() "<<recognizer.getConfidence()<<endl;
            predictedPerson = recognizer.getPrediction();
            cout<<"you look most like this peson from the training database "<<predictedPerson<<endl;
        }
        
    }
    
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofSetColor(255);
    cam.draw(0, 0);
    objectFinder.draw();
    cropped.draw(0 , cam.getHeight());
    if(objectFinder.size() > 0) recognizer.drawImageOfPerson(predictedPerson, 150,cam.getHeight(), 150,150 );
    
    
    ofDrawBitmapStringHighlight(ofToString(objectFinder.size()), 10, 20);

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

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
