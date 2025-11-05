#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
    songDropdown.populateFromDirectory(
        ofToDataPath("sounds"), { "mp3" });

    songDropdown.addListener(this, &ofApp::onSongChange);

    gui.setup("gui", "DropdownsSettings.xml");
    gui.add(& songDropdown);
}

//--------------------------------------------------------------
void ofApp::draw() {
    gui.draw();
}

//--------------------------------------------------------------
#ifndef TARGET_WIN32
void ofApp::onSongChange(ofFile & file) {
    std::cout << file.path() << std::endl;
}

#else
void ofApp::onSongChange(string & filepath) {
    std::cout << filepath << std::endl;
}
#endif
