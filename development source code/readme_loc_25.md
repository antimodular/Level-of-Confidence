#Level of Confidence - development branch

## Introduction
The software version that needs to be used for exhibition purposes can be downloaded here:

<http://lozano-hemmer.com/software/level_of_confidence/Nivel_de_Confianza_v25.zip>


## Dependencies
The software was programmed with openFrameworks 0.11.0 which is an open source C++ toolkit.

<http://openframeworks.cc/download>

We used Apple's Xcode development environment.

The software depends on you also downloading and adding the following files to openFramework's addon folder.

ofxFaceTracker2
<https://github.com/HalfdanJ/ofxFaceTracker2>

ofxFontStash
<https://github.com/armadillu/ofxFontStash>

ofxFaceRecognizer
<https://github.com/antimodular/ofxFaceRecognizer/tree/OF0.11>

ofxCv
<https://github.com/kylemcdonald/ofxCv>


## Development Notes
The above mentioned exhibition version ships with a pre-complied database which is used for the face recognition.
The database file is named like this: "model_eigen_500_Tarrlab_rot+crop.yml"


To adjust most of the settings you can use the GUI. Check `void ofApp::keyReleased(int key)` for the specific key combinations.
Press key g to see the GUI.



