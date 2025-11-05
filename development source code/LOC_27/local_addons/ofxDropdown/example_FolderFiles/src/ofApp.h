#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "ofxDropdown.h"

class ofApp : public ofBaseApp {
    public:
        void setup();
        void draw();
#ifndef TARGET_WIN32
    void onSongChange(ofFile & file);
#else
    void onSongChange(string & file);
#endif
        ofxPanel gui;
#ifndef TARGET_WIN32
    ofxDropdown_<ofFile> songDropdown { "Song" };
#else
    // ofxDropdown_<ofFile> will not work on windows. It will throw an error related to ofFile and the << operator.
    // Using instead ofxDropdown_<string> gives you a basic usage but without all the goodies that ofFile provides.
    ofxDropdown_<string> songDropdown { "Song" };
#endif
};
