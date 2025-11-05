#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	
	gui.setup("DROPDOWNS", "DropdownsSettings.json");
	gui.setPosition(20,20);
	
	strDropdown =  make_unique<ofxDropdown>("String Dropdown");
      gui.add(strDropdown->getDropdownParameters());
//    gui.add(strDropdown->getMultiSelectionParameter());
//    gui.add(strDropdown->getCollapseOnSelectionParameter());
	
    intOptions.setName("INT Options");
    intDropdown =  make_unique<ofxIntDropdown>(intOptions);
    gui.add(intDropdown->getDropdownParameters());
//    gui.add(intDropdown->getMultiSelectionParameter());
//    gui.add(intDropdown->getCollapseOnSelectionParameter());
	
	
	options.setName("STR Options");

    

	
	
	
	for(int i = 0; i < 15; i++){
//		ofxDropdown* dd2 = new ofxDropdown("Options" + ofToString(i));
		for(int j = 0; j < 5; j++){
			strDropdown->add(ofToString(i)+"_"+string(1, j+'A'));
		}
//		if(i == 0){
//			gui.add(dd2);
//		}else{
//			strDropdown->addDropdown(dd2);
//		}
//		if(i%3==0) strDropdown = dd2;
	}
    for(int i = 0; i < 15; i++){
            intDropdown->add(i, "Opcion " + ofToString(i));
    }
	gui.add(strDropdown.get());
    gui.add(intDropdown.get());
	
}

//--------------------------------------------------------------
void ofApp::update(){
	
}

//--------------------------------------------------------------
void ofApp::draw(){
	gui.draw();
	
    ofDrawBitmapStringHighlight(ofToString(intOptions.get()), 20, ofGetHeight() - 50);
    
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    if(key >= '0' && key <= '9'){
        intOptions = key - '0';
	}else if(key == 's'){
		gui.saveToFile("DropdownsSettings.json");
	}else if(key == 'l'){
		gui.loadFromFile("DropdownsSettings.json");
	}
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
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

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
