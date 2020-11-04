#Level of Confidence - development branch

## Introduction
The software version that needs to be used for exhibition purposes can be downloaded here:

<http://lozano-hemmer.com/software/level_of_confidence/Nivel_de_Confianza_v21.zip>


## Dependencies
The software was programmed with openFrameworks 0.8.4 which is an open source C++ toolkit.

<http://openframeworks.cc/download>

We used Apple's Xcode development environment.

The software depends on you also downloading and adding the following files to openFramework's addon folder.

ofxFaceTracker
<https://github.com/kylemcdonald/ofxFaceTracker>

ofxprosilica
<https://bitbucket.org/moostrik/ofxprosilica>

ofxOpenCV2461
<https://github.com/pkmital/ofxOpenCV2461>
(Requires your project to link against ZLib and OpenCL.)

ofxCv
<https://github.com/kylemcdonald/ofxCv>

ofxFTGL
<https://github.com/Flightphase/ofxFTGL>


## Development Notes
The above mentioned exhibition version ships with a pre-complied database which is used for the face recognition.
The database file is named like this: "model_fisher_2319_Tarrlab_rot+crop.yml"

The development version ships **without** a pre-complied database and generates a new database every time it runs.
Due to the extra processing that this requires the loading process takes much longer.

The following lines of code determine if a new database should be generated or if the software can just load an already existing database.

```
//-----------faceRecognizer

// method_used
// 0 = Eigenfaces_method ; has much bigger database. takes longer to load
// 1 = Fisherfaces_method; faster to load
// 2 = LBPHFaces_method //Local Binary Patterns Histograms
// theFaceRecognizer.setup(int method_used, int maxFaces, bool bAlreadySavedModel, string folderName)
// theFaceRecognizer.setup(1,500,true,"Tarrlab_rot+crop");
// theFaceRecognizer.setup(1,2319,true,"Tarrlab_rot+crop");

//bool bAlreadySavedModel is set to false, this means a new database will be generated
theFaceRecognizer.setup(1,500,false,"Tarrlab_rot+crop");
```

To adjust most of the settings you can use the GUI. Check `void ofApp::keyReleased(int key)` for the specific key combinations.



