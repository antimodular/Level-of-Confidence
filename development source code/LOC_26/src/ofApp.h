#pragma once

#include "ofMain.h"

#include "ofxCv.h"

#include "ofxFaceTracker2.h"

#include "ofxFontStash.h"

#include "ofxGui.h"

#include "ofxFaceRecognizer.h"

//#define VIDEO_GRAYSCALE

#include "imageGrid.h"
#include  "oneImage.h"


using namespace cv;
using namespace cv::face;
using namespace ofxCv;

class ofApp : public ofBaseApp{
    
public:
    
    string versionNum;
    
    void setup();
    void loadDatabase();
    void exit();
    void update();
    void draw();
    void keyReleased(int key);
    void keyPressed(int key);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void getNewStudent();
    void resetFaces();
    void saveGui();
    
    vector<ofPoint> getMinimumBoundingBox(vector <glm::vec2> _imagePoints);
    ofRectangle getBiggerBox(ofRectangle _rect, int _padding, float _ratio, int maxWidth, int maxHeight);
    
    void drawInfoText(int _language);
    
    ofRectangle growRect(ofRectangle _from, ofRectangle _to, float _step, float _maxW, float _maxH);
    
#ifdef USE_PROSILICA
    ofxProsilica::ConnectorWithInterface	vidGrabber;
#else
    ofVideoGrabber vidGrabber;
#endif
    ofxFaceTracker2 faceTracker;
    
    ofImage vidGrabberImage;
       ofPixels rotatedInput;

    ofParameter<int> camRotateAmt;
    int old_camRotateAmt;
    
    //----------GUI
    bool bShowGUI;
    ofParameter<bool> bShowCamera;
    bool isFullScreen;
    
    ofxPanel gui_main,gui_usbCam, gui_tracking, gui_image;
    ofxPanel gui_grid,gui_livePortrait,gui_gridMovement;
    ofxPanel gui_infoText;

#ifdef USE_PROSILICA
    ofxPanel gui_prosilica;
#endif
    ofParameter<ofPoint> gui_mainPos;
    
    ofParameter<int> fps;

    ofxFontStash helveticaCondBoldSmall,helveticaSmall;

    
    
    //----------tracking
    int camHeight, camWidth;
    ofRectangle faceRect;
    ofImage faceCropped;
    
    ofParameter<bool> bMirror;
    ofParameter<bool> bFlip;
    ofParameter<int> faceTracker_iterrations;
    ofParameter<int> faceTracker_attempts;
    ofParameter<float> faceTracker_tolerance;
    int old_faceTracker_iterrations;
    int old_faceTracker_attempts;
    float old_faceTracker_tolerance;
    
    ofParameter<int> haarMinSize;
    int old_haarMinSize;
    
    ofParameter<bool> bLiveFaceFound;
    ofParameter<float> faceLostDuration;
    ofParameter<float> faceLostZoomWait;
    ofParameter<float> faceFoundDuration;
    ofParameter<float> faceLostTimeDiff;
    ofParameter<float> faceFoundTimeDiff;
    float faceLostTimer;
    float faceFoundTimer;
    
    vector<ofVec2f> exclusionArray;
    ofVec2f last_leftEye,last_rightEye;
    ofVec2f leftEye_center, rightEye_center;
    ofParameter<float> exclusionDuration;
    
    float exclusionTimer;

    float featureLine_alpha;
    
    
    float distTo_leftEye_center;
    
    bool bGotReset;
    //----------cropped live portrait
    ofImage portraitCropped;
    ofRectangle portraitRect;
    ofParameter<int> horizPadding;
    float livePortrait_ratio;
 
    
    ofParameter<bool> bShowLivePortrait;
    ofParameter<int> croppedPortrait_height;
    ofParameter<int> livePortrait_x,livePortrait_y;
    ofParameter<int> livePortrait_w,livePortrait_h;
    int old_livePortrait_w, old_livePortrait_h;
    
    ofParameter<float> growRectSizeIn;
    ofParameter<float> growRectSizeOut;
    ofRectangle maxRatioRect;
    
    //----------picked portrait
   oneImage pickedImage;
    int pickedID;
    ofParameter<int> pickedPortrait_x,pickedPortrait_y;
    ofParameter<float> pickedPortrait_scaler;
    float pickedPortrait_confi;
    

    int old_bigFontSize;
    vector<int> lookUpTable;
    
    //----------recognition
    ofxFaceRecognizer theFaceRecognizer;

  //maximum of 2319 training images
    int prediction_array[2319];
    float avgConfidence_array[2319];
    int avgConfidenceCnt_array[2319];
    float minConfidence, maxConfidence;

    //43 missing students
    int prediction_array43[43];
    float avgConfidence_array43[43];
    int avgConfidenceCnt_array43[43];
    
    bool bFinale;
    int finalePickedID;
    
    //-------info text
    vector<string> infoText;
    ofParameter<int> infoText_y,infoText_x;
    ofParameter<int> infoText_lineHeight;
    ofParameter<int> infoText_fontSize;
    ofParameter<int> infoText_language;
    int old_infoText_fontSize;

    

    //----------grid of students
    imageGrid theImageGrid;
    ofParameter<int> goToID;
    ofPoint portraits_gridPos;
    ofPoint portraits_gridPos_offset,portraits_gridPos_offset2;
    
    float gridSwitch_timer;
    ofParameter<float> gridSwitch_duration;
    ofParameter<float> grid_tweenStepping;
    ofParameter<int> smallGrid_y;
    
    //--zoom
    ofParameter<float> zoomIn_oneStep,zoomMid_oneStep,zoomOut_oneStep;
    ofParameter<float> defaultZoom_out,defaultZoom_in,defaultZoom_mid;
    float old_defaultZoom_out,old_defaultZoom_in,old_defaultZoom_mid;
    ofParameter<int> zoomStage;
    int nextZoomStage;
    
    float newZoomValue;
    float zoomFrom, zoomTo, zoomCurrent;
    
    float zoomTimer;
    float zoomRestDuration;
    float zoom_oneStep;
    
    int newStudentCnt;
    ofImage newStudentImage;
    string newStudentFileName;
    
    
    bool bUseVideo;
    
   // vector<int> accum;
    
    bool initDone;
    float initDuration;
    float initTimer;
    unsigned long lastTap;
    
    Mat croppedCamMat;
};
