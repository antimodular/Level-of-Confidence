#include "ofApp.h"

/*
 2015
 artist: Rafael Lozano-Hemmer
 developeder: Stephan Schulz

 special thanks to Kyle McDonald for allowing us to use the ofXFaceTracker
 */


//todo list
/*
 - camRotate does not like it to be set to 2
 - reject the location of false faces

 */



void ofApp::setup() {
    
    versionNum = "v_21_dev";
    
    ofSetVerticalSync(true);
    // ofBackground(255,0,0);
    
    ofSetWindowPosition(-500, 0);
    ofSetFullscreen(true);
    
    
    //-----------image grid
    
    theImageGrid.grid_cols = 8;
    theImageGrid.grid_rows = 6;
    theImageGrid.setup(8,6);
    
    pickedImage.setup();
    
    //-----------GUI
    int gui_x = 10;
    int gui_y = 20;
    int gui_w = 210;
    int gui_h = 350;
    
    gui_main.setup("LOC "+versionNum);
    gui_main.setPosition(gui_x,gui_y);
    gui_main.setDefaultHeaderBackgroundColor(ofColor(255,0,0));
    gui_main.add(fps.set("fps",0,0,120));

    
    gui_tracking.setup("tracking");
    gui_tracking.setPosition(gui_x+=gui_w,gui_y);
    gui_tracking.add(bShowCamera.set("showCamera",true));
    gui_tracking.add(bMirror.set("mirror",false));
    gui_tracking.add(bFlip.set("flip",false));
    gui_tracking.add(camRotateAmt.set("rotate cam",1,0,3));
    
    
    // gui_tracking.add(livePortrait_ratio.set("livePortrait_ratio",1.59,1,3));
    // gui_tracking.getFloatSlider("livePortrait_ratio").setFineStep(0.01);
    // gui_tracking.add(livePortrait_margin.set("livePortrait_margin",20,0,300));
    
    gui_tracking.add(haarMinSize.set("haarMinSize",50,0,2000));
    gui_tracking.add(faceTracker_iterrations.set("iterrations",10,1,25));
    gui_tracking.add(faceTracker_attempts.set("attempts",1,1,4));
    gui_tracking.add(faceTracker_tolerance.set("tolerance",0.1,0.1,1));
    gui_tracking.getFloatSlider("tolerance").setFineStep(0.1);
    
    gui_tracking.add(bLiveFaceFound.set("liveFaceFound",false));
    gui_tracking.add(faceLostDuration.set("faceLostDuration",5,1,30));
    gui_tracking.add(faceLostZoomWait.set("faceLostZoomWait",1,0,30));
    gui_tracking.add(faceLostTimeDiff.set("faceLostTimeDiff",5,1,30));
    gui_tracking.add(faceFoundDuration.set("faceFoundDuration",5,1,30));
    gui_tracking.add(faceFoundTimeDiff.set("faceFoundTimeDiff",5,1,30));
    gui_tracking.add(exclusionDuration.set("exclusionDuration",20,0,100));
    
    
    gui_livePortrait.setup("live portrait");
    gui_livePortrait.setPosition(gui_x+=gui_w,gui_y);
    gui_livePortrait.add(bShowLivePortrait.set("showLivePortrait",true));
    gui_livePortrait.add(livePortrait_x.set("livePortrait_x",ofGetWidth()/2,0,ofGetWidth()));
    gui_livePortrait.add(livePortrait_y.set("livePortrait_y",ofGetHeight()/2,0,1200));
    gui_livePortrait.add(livePortrait_w.set("livePortrait_w",ofGetHeight()/2,0,ofGetWidth()));
    gui_livePortrait.add(livePortrait_h.set("livePortrait_h",ofGetHeight()/2,0,ofGetHeight()));
    gui_livePortrait.add(horizPadding.set("horizPadding",40,0,80));
    
    gui_livePortrait.add(growRectSizeIn.set("liveZoomIn",0.1,0,1));
    gui_livePortrait.getFloatSlider("liveZoomIn").setFineStep(0.001);
    gui_livePortrait.add(growRectSizeOut.set("liveZoomOut",0.1,0,1));
    gui_livePortrait.getFloatSlider("liveZoomOut").setFineStep(0.001);
    
    
    gui_grid.setup("grid");
    gui_grid.setPosition(gui_x+=gui_w,gui_y);
    
    gui_grid.add(theImageGrid.portraitFont_y.set("font_y",0,0,900));
    gui_grid.add(theImageGrid.portrait_fontSize.set("font_size",50,0,60));
    gui_grid.add(theImageGrid.portraitFont_lineHeight.set("font_lineHeight",10,0,100));
    gui_grid.add(theImageGrid.portraitFont_ySmall.set("font_ySmall",0,0,900));
    gui_grid.add(theImageGrid.portrait_fontSizeSmall.set("font_sizeSmall",20,0,60));
    gui_grid.add(theImageGrid.portraitFont_lineHeightSmall.set("font_lineHeightSmall",10,0,100));
    gui_grid.add(smallGrid_y.set("smallGrid_y",850,0,ofGetHeight()));
    
    gui_gridMovement.setup("gridMovement");
    gui_gridMovement.setPosition(gui_x+=gui_w,gui_y);
    gui_gridMovement.add(goToID.set("goToID",0,0,42));
    gui_gridMovement.add(zoomStage.set("zoomStage",0,0,5));
    gui_gridMovement.add(gridSwitch_duration.set("gridSwitch_duration",5,1,20));
    gui_gridMovement.getFloatSlider("gridSwitch_duration").setFineStep(0.01);
    gui_gridMovement.add(grid_tweenStepping.set("grid_tweenStepping",0.02,0.001,0.04));
    gui_gridMovement.getFloatSlider("grid_tweenStepping").setFineStep(0.001);
    gui_gridMovement.add(defaultZoom_in.set("defaultZoom_in",2.43,0.1,3.0));
    gui_gridMovement.getFloatSlider("defaultZoom_in").setFineStep(0.01);
    gui_gridMovement.add(zoomIn_oneStep.set("zoomIn_oneStep",0.01,0.001,0.05));
    gui_gridMovement.getFloatSlider("zoomIn_oneStep").setFineStep(0.001);
    
    gui_gridMovement.add(defaultZoom_mid.set("defaultZoom_mid",0.8125,0.1,2.0));
    gui_gridMovement.getFloatSlider("defaultZoom_mid").setFineStep(0.01);
    gui_gridMovement.add(zoomMid_oneStep.set("zoomMid_oneStep",0.01,0.001,0.05));
    gui_gridMovement.getFloatSlider("zoomMid_oneStep").setFineStep(0.001);
    
    gui_gridMovement.add(defaultZoom_out.set("defaultZoom_out",0.2075,0.1,2.0));
    gui_gridMovement.getFloatSlider("defaultZoom_out").setFineStep(0.01);
    gui_gridMovement.add(zoomOut_oneStep.set("zoomOut_oneStep",0.01,0.001,1));
    gui_gridMovement.getFloatSlider("zoomOut_oneStep").setFineStep(0.001);
    
    
    
    gui_image.setup("picked portrait");
    gui_image.setPosition(gui_x=10,gui_y+=gui_h);
    gui_image.add(pickedPortrait_x.set("pickedPortrait_x",0,-500,ofGetWidth()));
    gui_image.add(pickedPortrait_y.set("pickedPortrait_y",ofGetHeight()/2,0,ofGetHeight()));
    gui_image.add(pickedPortrait_scaler.set("pickedP_scaler",1,0.3,3));
    gui_image.getFloatSlider("pickedP_scaler").setFineStep(0.001);
    
    gui_image.add(pickedImage.bigFont_y.set("bigFont_y",100,0,1000));
    gui_image.add(pickedImage.bigFontSize.set("bigFontSize",20,0,60));
    gui_image.add(pickedImage.bigFont_lineHeight.set("bigFont_lineHeight",10,0,100));
    
    gui_infoText.setup("info text");
    gui_infoText.setPosition(gui_x+=gui_w,gui_y);
    gui_infoText.add(infoText_x.set("infoText_x",ofGetWidth()/2,0,ofGetWidth()));
    gui_infoText.add(infoText_y.set("infoText_y",ofGetHeight()/2,0,ofGetHeight()));
    gui_infoText.add(infoText_fontSize.set("infoText_fontSize",10,0,100));
    gui_infoText.add(infoText_lineHeight.set("infoText_lineHeight",10,0,100));
    gui_infoText.add(infoText_language.set("language",0,0,1));
    
    
#ifdef USE_PROSILICA
    gui_prosilica.setup("prosilica");
    gui_prosilica.setPosition(gui_x+=gui_w,gui_y);
    gui_prosilica.add(vidGrabber.parameters);
#endif
    cout<<"load gui"<<endl;
    
    //load gui data
    gui_main.loadFromFile("gui_main.xml");
    gui_grid.loadFromFile("gui_grid.xml");
    gui_gridMovement.loadFromFile("gui_gridMovement.xml");
    gui_livePortrait.loadFromFile("gui_livePortrait.xml");
    gui_tracking.loadFromFile("gui_tracking.xml");
    gui_image.loadFromFile("gui_image.xml");
    gui_infoText.loadFromFile("gui_infoText.xml");
#ifdef USE_PROSILICA
    gui_prosilica.loadFromFile("gui_prosilica.xml");
#endif
    bShowGUI = false;//true;
    bShowCamera = false;
    ofHideCursor();
    
    cout<<"done load gui"<<endl;
    
    initDone = false;
    
    old_camRotateAmt = camRotateAmt;
    
    ofImageType imageType = OF_IMAGE_COLOR;
    
#ifdef USE_PROSILICA
    //vector<>    vidGrabber.listDevices();
    // pre setup features
    vidGrabber.setDeviceID(6002281); // for the interface to work correctly set the deviceID
    //tym mako 6002281
    //mako g-030C ID= 6001010  648x488
    //mako g125c id=6002134 1292 x 964
    //GE1910c  id=115230
    //gc0650c id=33339
    
    //	cameraA.setPersistentIpAdress(10.0.0.66);
    //	cameraA.setPersistentIpSubnetMask(255.0.0.0);
    //	cameraA.setPersistentIp(true);
#ifndef VIDEO_GRAYSCALE
    vidGrabber.setPixelFormat(OF_PIXELS_RGB);
#endif
    
    vidGrabber.setup();
    // OF_IMAGE_GRAYSCALE
    
    camWidth  = 648; //vidGrabber.getWidth();
    camHeight = 488; //vidGrabber.getHeight();
    
    
    while(vidGrabber.isInitialized() == false){
        vidGrabber.update();
        //  cout<<"camWidth "<<camWidth<<" , "<<camHeight<<endl;
    }
    
    
#else
    //use regular USB camera
    
    
    camWidth = 640;
    camHeight = 480;
    
    
    vidGrabber.listDevices();
    
    //we can now get back a list of devices.
    vector<ofVideoDevice> devices = vidGrabber.listDevices();
    int deviceID = 0;
    ofLog()<<"found "<<devices.size()<<" cameras attached";
    //    if(devices.size() > 1){
    //        cout<<"found more than one camera"<<endl;
    for(int i = 0; i < devices.size(); i++){
        cout << devices[i].id << ": " << devices[i].deviceName;
        if( devices[i].bAvailable ){
            
            //                vector<string> deviceNameSplit;
            //                deviceNameSplit = ofSplitString(devices[i].deviceName, " ");
            //                if(deviceNameSplit[0] != "FaceTime" ){
            if(ofIsStringInString(devices[i].deviceName, "FaceTime") == false){
                deviceID = i;
                cout<<"found a camera that is not a FaceTime camera; i.e. not a build in camera"<<endl;
            }
        }else{
            cout << " - unavailable " << endl;
        }
    }
    //    }
    cout<<endl;


    vidGrabber.setDeviceID(deviceID);
    vidGrabber.initGrabber(camWidth, camHeight);
    
#endif
    
    cout<<"camRotateAmt "<<camRotateAmt<<endl;
    if(camRotateAmt == 1 || camRotateAmt == 3){
        cout<<"camera in portrait mode with"<<endl;
        int temp_w = camWidth;
        camWidth = camHeight;
        camHeight = temp_w;
    }
    
    
#ifdef VIDEO_GRAYSCALE
    rotatedInput.allocate(camHeight, camWidth, OF_PIXELS_MONO);
#else
    rotatedInput.allocate(camHeight, camWidth, OF_PIXELS_RGB);
#endif
    
    
    vidGrabberImage.allocate(camWidth, camHeight, imageType);
    
    
    
    ofTrueTypeFont::setGlobalDpi(72);
    helveticaSmall2.loadFont("HELVETICALTSTD-COND2.OTF", 10, false);
    
    helveticaSmall.loadFont("HELVETICALTSTD-COND2.OTF", infoText_fontSize, false);
    helveticaCondBoldSmall.loadFont("HELVETICALTSTD-BOLDCOND2.OTF", infoText_fontSize, false);
    
    
    growRectSizeIn = ofClamp(growRectSizeIn, 0, 1);
    growRectSizeOut = ofClamp(growRectSizeOut, 0, 1);
    
    
    theImageGrid.updateFBO();
    
    
    pickedImage.setWidth(theImageGrid.getPortraitWidth());
    pickedImage.setHeight(theImageGrid.getPortraitHeight());
    //    pickedImage.setTop(theImageGrid.portraitFrame_top);
    //    pickedImage.setBottom(theImageGrid.portraitFrame_bottom);
    //    pickedImage.setLeft(theImageGrid.portraitFrame_left);
    //    pickedImage.setRight(theImageGrid.portraitFrame_right);
    pickedImage.setWidthMargin(theImageGrid.getPortraitWidthMargin());
    pickedImage.setHeightMargin(theImageGrid.getPortraitHeightMargin());
    
    
    
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
    
    int t_cnt = 0;
    int tt = 0;
    float t_cut = theFaceRecognizer.getUniquePersonCount() / 43;
    
    cout<<"t_cut "<<t_cut<<endl;
    
    for(int i=0; i< theFaceRecognizer.getUniquePersonCount();i++){
        lookUpTable.push_back(t_cnt);
        t_cnt++;
        if(t_cnt >= 43) t_cnt = 0;
    }
    
    cout<<"lookUpTable.size "<<lookUpTable.size()<<endl;
    cout<<"theFaceRecognizer.getUniquePersonCount() "<<theFaceRecognizer.getUniquePersonCount()<<endl;
    
    /*
    for(int i=0; i<lookUpTable.size();i++){
        cout<<i<<" "<<lookUpTable[i]<<endl;
    }
     */
    //-----------facetracker
#ifdef VIDEO_GRAYSCALE
    faceCropped.allocate(theFaceRecognizer.getImageWidth(), theFaceRecognizer.getImageHeight(), OF_IMAGE_GRAYSCALE); //
#else
    faceCropped.allocate(theFaceRecognizer.getImageWidth(), theFaceRecognizer.getImageHeight(), OF_IMAGE_COLOR);
#endif
    faceTracker.setup();
    bGotReset = false;
    resetFaces();
    
    
    //-----------live portrait
    croppedPortrait_height = 860;
#ifdef VIDEO_GRAYSCALE
    portraitCropped.allocate(360, croppedPortrait_height, OF_IMAGE_GRAYSCALE);
#else
    portraitCropped.allocate(360, croppedPortrait_height, OF_IMAGE_COLOR);
#endif
    //------grid movement
    goToID = 0;
    
    nextZoomStage = 1;
    zoomStage = -1;
    newZoomValue = defaultZoom_out;
    zoomFrom = defaultZoom_mid;
    zoomTo = defaultZoom_out;
    
    //--------info text
    cout<<"load info text"<<endl;
    ifstream infoFile; //declare a file stream
    //infoFile.open( ofToDataPath("infoText_eng.txt").c_str() ); //open your text file
    infoFile.open( ofToDataPath("infoText.txt").c_str() );
    while(infoFile!=NULL) //as long as theres still text to be read
    {
        string line; //declare a string for storage
        getline(infoFile, line); //get a line from the file, put it in the string
        
        //if(line.length() >= 0){
        cout<<infoText.size()<<" - "<<line<<" - "<<endl;
        infoText.push_back(line);
        
        ofSleepMillis(10);
        //}
        // cout<<"infoText.size "<<infoText.size()<<endl;
        //zonesNodes.push_back(str); //push the string onto a vector of strings
    }
    
    infoFile.close();
    
    /*
     faceTracker.setAttempts(faceTracker_attempts);
     faceTracker.setIterations(faceTracker_iterrations);
     faceTracker.setTolerance(faceTracker_tolerance);
     croppedPortrait_height = 860 - livePortrait_margin*2;
     portraitCropped.resize(croppedPortrait_height/livePortrait_ratio, croppedPortrait_height);
     helveticaCondBoldSmall.setSize(infoText_fontSize);
     helveticaSmall.setSize(infoText_fontSize);
     */
    
    faceLostTimer = ofGetElapsedTimef() - faceLostDuration;
    
    newStudentCnt = -1;
    
    bUseVideo = true;
    
    pickedID = 0;
    bFinale = false;
    
    
    last_leftEye = ofVec2f(0, 0);
    
    initDuration = 10;
    
    initTimer = ofGetElapsedTimef();
    //ofResetElapsedTimeCounter();
}

void ofApp::exit(){
    
    if(old_camRotateAmt != camRotateAmt){
        saveGui();
    }
    
    
    faceTracker.waitForThread();
}

void ofApp::update() {
    
    ofSetWindowTitle(ofToString(ofGetFrameRate()));
    fps = ofGetFrameRate();
    

    
    
    if(old_livePortrait_w != livePortrait_w || old_livePortrait_h != livePortrait_h){
        old_livePortrait_w = livePortrait_w;
        old_livePortrait_h = livePortrait_h;
        
        livePortrait_ratio = livePortrait_h/ (float) livePortrait_w;
        portraitCropped.resize(livePortrait_w, livePortrait_h);
        
        float t_w = camHeight / livePortrait_ratio;
        if(t_w < camWidth){
            maxRatioRect = ofRectangle((camWidth-t_w)/2,0, t_w,camHeight);
        }else{
            float t_h = camHeight / livePortrait_ratio;
            maxRatioRect = ofRectangle(0,(camHeight-t_h)/2, camWidth, t_h);
            
        }
        
    }
    
    
    
    if(old_faceTracker_attempts != faceTracker_attempts){
        old_faceTracker_attempts = faceTracker_attempts;
        faceTracker.setAttempts(faceTracker_attempts);
        cout<<"faceTracker_attempts "<<faceTracker_attempts<<endl;
    }
    if(old_faceTracker_iterrations != faceTracker_iterrations){
        old_faceTracker_iterrations = faceTracker_iterrations;
        faceTracker.setIterations(faceTracker_iterrations);
        cout<<"faceTracker_iterrations "<<faceTracker_iterrations<<endl;
    }
    if(old_faceTracker_tolerance != faceTracker_tolerance){
        old_faceTracker_tolerance = faceTracker_tolerance;
        faceTracker.setTolerance(faceTracker_tolerance);
        cout<<"faceTracker_tolerance "<<faceTracker_tolerance<<endl;
    }
    
    
    if(old_infoText_fontSize != infoText_fontSize){
        old_infoText_fontSize = infoText_fontSize;
        helveticaSmall.setSize(infoText_fontSize);
        helveticaCondBoldSmall.setSize(infoText_fontSize);
    }
    
    if(old_haarMinSize != haarMinSize){
        old_haarMinSize = haarMinSize;
        faceTracker.setHaarMinSize(haarMinSize);
    }
    
    
    //  }
    
    
    vidGrabber.update();
    if(vidGrabber.isFrameNew()) {
        
        
        // copy(vidGrabber,vidGrabberImage);
        Mat liveMat;
        
        
        vidGrabber.getPixelsRef().rotate90To(rotatedInput,camRotateAmt);
        
        
        
#ifdef VIDEO_GRAYSCALE
        liveMat = Mat(camHeight,camWidth,CV_8UC1,rotatedInput.getPixels(),0);
#else
        liveMat = Mat(camHeight,camWidth,CV_8UC3,rotatedInput.getPixels(),0);
#endif
        
        
        ////vidGrabber.getPixelsRef().rotate90To(rotatedInput,3);
        // vidGrabberImage.setFromPixels(vidGrabber.getPixelsRef());
        
        int temp_mirror;
        if(bMirror == true && bFlip == true) temp_mirror = -1;
        if(bMirror == true && bFlip == false) temp_mirror = 0;
        if(bMirror == false && bFlip == true) temp_mirror = 1;
        if(bMirror == false && bFlip == false){
        }else{
            flip(liveMat,liveMat,temp_mirror); //0 flip x-axis, 1 means flip y-axis, -1 flip x&y axis
        }
        
        for(int i=0; i<exclusionArray.size();i++){
            cv::circle(liveMat, cvPoint(exclusionArray[i].x,exclusionArray[i].y),2, toCv(ofColor(255)), -1);
            //cv:rectangle(liveMat, cvPoint(exclusionArray[i].x,exclusionArray[i].y),4, toCv(ofColor(255)), -1);
        }
        
        copy(liveMat, vidGrabberImage);
        vidGrabberImage.update();
        
        // vidGrabberImage.mirror(bMirror, bFlip);
        
        faceTracker.update(toCv(vidGrabberImage));
        
        if(faceTracker.getFound()) {
            
            
            //            leftEye_line = faceTracker.getImageFeature(faceTracker.LEFT_EYE);
            //            rightEye_line = faceTracker.getImageFeature(faceTracker.RIGHT_EYE);
            
            
            LEFT_EYE_line = faceTracker.getImageFeature(faceTracker.LEFT_EYE);
            RIGHT_EYE_line = faceTracker.getImageFeature(faceTracker.RIGHT_EYE);
            LEFT_EYEBROW_line = faceTracker.getImageFeature(faceTracker.LEFT_EYEBROW);
            RIGHT_EYEBROW_line = faceTracker.getImageFeature(faceTracker.RIGHT_EYEBROW);
            
            NOSE_BRIDGE_line = faceTracker.getImageFeature(faceTracker.NOSE_BRIDGE);
            NOSE_BASE_line = faceTracker.getImageFeature(faceTracker.NOSE_BASE);
            INNER_MOUTH_line = faceTracker.getImageFeature(faceTracker.INNER_MOUTH);
            OUTER_MOUTH_line = faceTracker.getImageFeature(faceTracker.OUTER_MOUTH);
            JAW_line = faceTracker.getImageFeature(faceTracker.JAW);
            
            
            leftEye_center = LEFT_EYE_line.getCentroid2D();
            rightEye_center = RIGHT_EYE_line.getCentroid2D();
            
            distTo_leftEye_center = last_leftEye.distance(leftEye_center);
            //            cout<<"distTo_leftEye_center "<<distTo_leftEye_center<<endl;
            //            cout<<"ofGetElapsedTimef() - exclusionTimer "<<ofGetElapsedTimef() - exclusionTimer<<endl;
            last_leftEye = leftEye_center;
            
            
            
            
            
            //faceTracker does not return live bounding box. so we try to get it from the array fo feature points
            vector <ofPoint> minMaxPoints = getMinimumBoundingBox(faceTracker.getImagePoints());
            faceRect = ofRectangle(minMaxPoints[0],minMaxPoints[1]);
            
            
            if(bUseVideo){
                Mat croppedCamMat(toCv(vidGrabberImage), toCv(faceRect));
                //    cout<<"faceRect "<<faceRect<<endl;
                resize(croppedCamMat, faceCropped);
                faceCropped.update();
            }else{
                copy(newStudentImage,faceCropped);
                faceCropped.update();
            }
            
            //we now make the face rect a bit bigger since we want to display a nice protrait of the live person
            ofRectangle temp_portraitRect;
            
            
            temp_portraitRect = getBiggerBox(faceRect, horizPadding, livePortrait_ratio, camWidth,camHeight);
            
            //  cout<<"portraitRect A "<<portraitRect<<endl;
            //   cout<<"t_portraitRect "<<temp_portraitRect<<endl;
            
            //grow portrait rect from big to small
            float tt = growRectSizeOut; //ofMap(mouseX, 0, ofGetWidth(), 0, 1);
            portraitRect = growRect(portraitRect,temp_portraitRect,tt, camWidth,camHeight);
            
            // cout<<"portraitRect B "<<portraitRect<<endl;
            //            Mat croppedCamMat2(toCv(vidGrabberImage), toCv(portraitRect));
            Mat croppedCamMat2(liveMat, toCv(portraitRect));
            
            //cout<<"portraitRect "<<portraitRect<<endl;
            resize(croppedCamMat2, portraitCropped);
            portraitCropped.update();
            
            
            //as long as we have not collected enough data we keep on querying the faceRegonizer
            //once one face got 100 hits we stop that process
            if(bFinale == false){
                
                Mat face;
                if(bUseVideo){
#ifdef VIDEO_GRAYSCALE
                    copyGray(faceCropped, face);
#else
                    cvtColor(toCv(faceCropped), face, CV_BGR2GRAY);
#endif
                    
                }else{
                    copy(faceCropped, face);
                }
                
                theFaceRecognizer.update(face);
                
                
                //                if(theFaceRecognizer.getConfidence() < minConfidence) minConfidence = theFaceRecognizer.getConfidence();
                //                if(theFaceRecognizer.getConfidence() > maxConfidence ) maxConfidence = theFaceRecognizer.getConfidence();
                
                /*
                 //only show newly picked portrait
                 //does not chause portrait switch if portrait was already picked
                 if(prediction_array[theFaceRecognizer.getPrediction()] == 0){
                 pickedID = lookUpTable[theFaceRecognizer.getPrediction()];
                 pickedPortrait_confi = theFaceRecognizer.getConfidence();
                 }
                 */
                
                
                
                /*
                 //every time a face matches it gets picked
                 pickedID = lookUpTable[theFaceRecognizer.getPrediction()];
                 pickedPortrait_confi = theFaceRecognizer.getConfidence();
                 */
                
                //                cout<<"theFaceRecognizer.getPrediction() "<<theFaceRecognizer.getPrediction()<<endl;
                //                cout<<"minConfidence "<<minConfidence<<endl;
                //                cout<<"maxConfidence "<<maxConfidence<<endl;
                
                //these arrays refer to training database
                prediction_array[theFaceRecognizer.getPrediction()] += 1;
                
                //                if(avgConfidence_array[theFaceRecognizer.getPrediction()] > 0){
                //                    avgConfidence_array[theFaceRecognizer.getPrediction()] += theFaceRecognizer.getConfidence();
                //                    avgConfidence_array[theFaceRecognizer.getPrediction()] /= 2.0;
                //                }else{
                //                    avgConfidence_array[theFaceRecognizer.getPrediction()] += theFaceRecognizer.getConfidence();
                //                }
                
                avgConfidence_array[theFaceRecognizer.getPrediction()] += theFaceRecognizer.getConfidence();
                avgConfidenceCnt_array[theFaceRecognizer.getPrediction()] += 1;
                
                
                //combine all data from training databse in to the 43-database
                memset(prediction_array43, 0, sizeof(prediction_array43));
                memset(avgConfidence_array43, 0, sizeof(avgConfidence_array43));
                
                float temp_minConfi = 100000;
                float temp_maxConfi = -1;
                
                //                cout<<endl;
                //                cout<<endl;
                for(int i=0; i<theFaceRecognizer.getUniquePersonCount(); i++){
                    
                    if(prediction_array[i] > 0){
                        int temp_studentID = lookUpTable[i];
                        
                        prediction_array43[temp_studentID] += prediction_array[i];
                        
                        
                        //  cout<<"i "<<i<<", temp_studentID "<<temp_studentID<<endl;
                        //  cout<<"avgConfidence_array[i] "<<avgConfidence_array[i]<<" / "<<(float)avgConfidenceCnt_array[i]<<" =  "<<avgConfidence_array[i] / (float)avgConfidenceCnt_array[i]<<endl;
                        
                        
                        if(avgConfidence_array43[temp_studentID] > 0){
                            avgConfidence_array43[temp_studentID] += avgConfidence_array[i] / (float)avgConfidenceCnt_array[i];
                            avgConfidence_array43[temp_studentID] /= 2.0;
                            // cout<<">0 avgConfidence_array43[temp_studentID] "<<avgConfidence_array43[temp_studentID]<<endl;
                        }else{
                            avgConfidence_array43[temp_studentID] += avgConfidence_array[i] / (float)avgConfidenceCnt_array[i];
                            // cout<<"  avgConfidence_array43[temp_studentID] "<<avgConfidence_array43[temp_studentID]<<endl;
                            
                        }
                        
                        if(avgConfidence_array43[temp_studentID] < minConfidence && avgConfidence_array43[temp_studentID] > 1) minConfidence = avgConfidence_array43[temp_studentID];
                        if(avgConfidence_array43[temp_studentID] > maxConfidence) maxConfidence = avgConfidence_array43[temp_studentID];
                    }
                }
                
                //  cout<<"---minConfidence "<<minConfidence<<" max "<<maxConfidence<<endl;
                
                minConfidence = 0;
                maxConfidence = 2000;
                
                if(prediction_array[theFaceRecognizer.getPrediction()] % 10 == 0){
                    pickedID = lookUpTable[theFaceRecognizer.getPrediction()];
                    // pickedPortrait_confi = prediction_array43[pickedID]; //theFaceRecognizer.getConfidence();
                    pickedPortrait_confi = avgConfidence_array43[pickedID];
                }
                
                if(pickedID < 0 || pickedID >= 43) pickedID = 0;
                
                //simple passes the data on to the imageGrid drawer
                for(int i=0; i<43; i++){
                    theImageGrid.prediction_array43[i] = prediction_array43[i];
                }
                
                
                for(int i=0; i<43; i++){
                    if(prediction_array43[i] > 100){
                        bFinale = true;
                        finalePickedID = i;
                    }
                }
            }else{
                pickedID = finalePickedID;
                pickedPortrait_confi = avgConfidence_array43[pickedID];
            }
            
            
            
            
            
            bGotReset = false;
        }//end if(faceTracker.getFound())
        else{
            
            if(faceLostTimeDiff > faceLostZoomWait){
                // cout<<"growRectSize "<<growRectSize<<endl;
                float tt = growRectSizeIn; //ofMap(mouseX, 0, ofGetWidth(), 0, 1);
                
                portraitRect = growRect(portraitRect,maxRatioRect,tt, camWidth,camHeight);
                
                if(bGotReset == false) resetFaces();
            }
            Mat croppedCamMat2(toCv(vidGrabberImage), toCv(portraitRect));
            //cout<<"portraitRect "<<portraitRect<<endl;
            resize(croppedCamMat2, portraitCropped);
            portraitCropped.update();
            
            
            
            
            bFinale = false;
            
        }
        
        if(exclusionDuration > 0){
            if(distTo_leftEye_center >= 0 && distTo_leftEye_center < 1 ){
                if(ofGetElapsedTimef() - exclusionTimer > exclusionDuration && ofGetElapsedTimef() - exclusionTimer < exclusionDuration+0.1){
                    exclusionArray.push_back(leftEye_center);
                    exclusionArray.push_back(rightEye_center);
                }
            }else{
                exclusionTimer = ofGetElapsedTimef();
            }
        }
        
        
    }//end vidgrabber.newFrame
    
    
    
    faceLostTimeDiff = ofGetElapsedTimef() - faceLostTimer;
    faceFoundTimeDiff = ofGetElapsedTimef() - faceFoundTimer;
    
    
    if(faceTracker.getFound()){
        faceLostTimer = ofGetElapsedTimef();
        
        //if face has been found for long enough we zoom out to see full grid
        if(faceFoundTimeDiff > faceFoundDuration/2){
            if(bLiveFaceFound == false){
                bLiveFaceFound = true;
                zoomStage = 4;
                nextZoomStage = 4;
                cout<<"zoomStage "<<zoomStage<<endl;
            }
            //
        }
    }else{
        faceFoundTimer = ofGetElapsedTimef();
    }
    
    //if face was lost for too long we zoom back in to grid
    if(faceLostTimeDiff > faceLostDuration){
        if(bLiveFaceFound == true){
            bLiveFaceFound = false;
            zoomStage = 2;
            nextZoomStage = 2;
        }
    }
    //we round just for display purpose
    faceLostTimeDiff = roundf(faceLostTimeDiff);
    faceFoundTimeDiff = roundf(faceFoundTimeDiff);
    
    
    //the different zoom stages
    if(ofGetElapsedTimef() - zoomTimer > zoomRestDuration){
        zoomStage = nextZoomStage;
    }
    
    float temp_Pw = theImageGrid.getPortraitWidth();
    float temp_Ph = theImageGrid.getPortraitHeight();
    
    if(zoomStage == 1){
        cout<<"go to all the way zoomed in; i.e. super big portrait of student"<<endl;
        zoomFrom = zoomCurrent;
        zoomTo = defaultZoom_in;
        
        portraits_gridPos_offset = ofPoint(temp_Pw/2.0,temp_Ph/2.0);
        portraits_gridPos_offset2 = ofPoint(0,0);
        
        zoom_oneStep = zoomIn_oneStep;
        
        zoomTimer = ofGetElapsedTimef();
        zoomRestDuration = 10;
        
        portraits_gridPos_offset = ofPoint(temp_Pw/2.0,temp_Ph/2.0 + 100);
        portraits_gridPos_offset2 = ofPoint(0,0);
        
        theImageGrid.setDrawExtra(true);
        zoomStage = -1;
        nextZoomStage = 2;
    }
    if(zoomStage == 2){
        //cout<<"go to all mid zoom level; i.e. see 3 portraits of students"<<endl;
        zoomFrom = zoomCurrent;
        zoomTo = defaultZoom_mid;
        
        zoom_oneStep = zoomMid_oneStep;
        
        zoomTimer = ofGetElapsedTimef();
        zoomRestDuration = 5;
        
        portraits_gridPos_offset = ofPoint(temp_Pw/2.0,temp_Ph/2.0);
        portraits_gridPos_offset2 = ofPoint(0,0);
        
        theImageGrid.setDrawExtra(true);
        zoomStage = -1;
        nextZoomStage = 3;
    }
    if(zoomStage == 3){
        cout<<"go to new student"<<endl;
        goToID = (int)ofRandom(theImageGrid.getNumberOfImages());
        
        zoomTimer = ofGetElapsedTimef();
        zoomRestDuration = 5;
        
        portraits_gridPos_offset = ofPoint(temp_Pw/2.0,temp_Ph/2.0);
        portraits_gridPos_offset2 = ofPoint(0,0);
        
        theImageGrid.setDrawExtra(true);
        zoomStage = -1;
        nextZoomStage = 1;
    }
    
    if(zoomStage == 4){
        cout<<"go to overview; i.e see all portraits"<<endl;
        zoomFrom = zoomCurrent;
        zoomTo = defaultZoom_out;
        
        goToID = 0;
        
        
        zoom_oneStep = zoomOut_oneStep;
        
        zoomTimer = ofGetElapsedTimef();
        zoomRestDuration = 4;
        
        portraits_gridPos_offset = ofPoint(0,0);
        portraits_gridPos_offset2 = ofPoint(ofGetWidth()/2.0,ofGetHeight()/2.0);
        
        theImageGrid.setDrawExtra(false);
        zoomStage = -1;
        nextZoomStage = -1;
    }
    
    zoomCurrent -= (zoomCurrent - zoomTo) * zoom_oneStep;
    
    if(zoomTo == defaultZoom_out){
        ofPoint temp_p = theImageGrid.getPortraitPos(goToID);
        temp_p = temp_p * ofPoint(-1,-1);
        temp_p = temp_p - portraits_gridPos_offset;
        temp_p = temp_p - portraits_gridPos_offset2/zoomCurrent;
        portraits_gridPos -= (portraits_gridPos - temp_p) * 1;
        
    }else{
        ofPoint temp_p = theImageGrid.getPortraitPos(goToID);
        temp_p = temp_p * ofPoint(-1,-1);
        temp_p = temp_p - portraits_gridPos_offset;
        temp_p = temp_p - portraits_gridPos_offset2/zoomCurrent;
        portraits_gridPos -= (portraits_gridPos - temp_p) * grid_tweenStepping;
        
    }
    
    theImageGrid.update();
    pickedImage.update();
}

void ofApp::draw() {
    
    ofBackground(255);
    
    
    
    //ofBackgroundGradient(BGcolor, BGcolor2,OF_GRADIENT_BAR);
    //OF_GRADIENT_BAR
    //OF_GRADIENT_LINEAR
    //OF_GRADIENT_CIRCULAR
    
  //  initDuration = 1;
    if(ofGetElapsedTimef() - initTimer < initDuration ){ //&& zoomCurrent < 0.1
        
        ofPushMatrix();
        {
            ofTranslate(ofGetWidth()/2,500);
            
        
            string loadingText[2][5];
            
            loadingText[0][0] = "Nivel de Confianza";
            loadingText[0][1] = "versión ";
            loadingText[0][2] = "cargando";
            loadingText[0][3] = "Favor mover la cámara en ángulo";
            loadingText[0][4] = "para alinear al espectador con la pantalla.";

            loadingText[1][0] = "Level of Confidence";
            loadingText[1][1] = "version ";
            loadingText[1][2] = "loading";
            loadingText[1][3] = "Please angle the camera so that viewers";
            loadingText[1][4] = "are in line with the display.";
 
            
            int t_h = -80;
            ofSetColor(0);
            string temp_str = loadingText[infoText_language][0];
            int temp_strW = helveticaSmall.stringWidth(temp_str);
            int temp_x = - temp_strW/2;
            helveticaSmall.drawString(temp_str, temp_x, t_h);
            
            temp_str = loadingText[infoText_language][1]+versionNum;
            temp_strW = helveticaSmall.stringWidth(temp_str);
            temp_x = - temp_strW/2;
            helveticaSmall.drawString(temp_str, temp_x, t_h+=30);
            
            temp_str = loadingText[infoText_language][2];
            temp_strW = helveticaSmall.stringWidth(temp_str);
            temp_x = - temp_strW/2;
            helveticaSmall.drawString(temp_str, temp_x, t_h+=30);
            
      
            int maxDots = ofMap(ofGetElapsedTimef() - initTimer, 0, initDuration, 60, 0);
          
            temp_x = temp_strW/2+10;
            //ofLine(temp_x+60,  t_h-8 , temp_x+60-maxDots,  t_h-8);
            ofRect(temp_x+60,  t_h-8, -maxDots, 2);
            
            
            temp_str = loadingText[infoText_language][3];
            temp_strW = helveticaSmall.stringWidth(temp_str);
            temp_x = - temp_strW/2;
            helveticaSmall.drawString(temp_str, temp_x, t_h+= vidGrabberImage.getHeight() + 60);

            temp_str = loadingText[infoText_language][4];
            temp_strW = helveticaSmall.stringWidth(temp_str);
            temp_x = - temp_strW/2;
            helveticaSmall.drawString(temp_str, temp_x, t_h+=30);
            
            
            ofPushMatrix();
            {
                ofTranslate(-vidGrabberImage.getWidth()/2, 0);
                ofSetColor(255);
                vidGrabberImage.draw(0,0);
                
                ofSetColor(255);
                ofNoFill();
                ofRect(portraitRect);
                
                if(faceTracker.getFound()) {
                    faceTracker.draw();
                    
                    ofSetColor(255);
                    ofNoFill();
                    ofRect(faceRect);
                }//end if(faceTracker.getFound())
            }
            ofPopMatrix();
        }
        ofPopMatrix();
        
   
    } else{
        
        
        //-----------cropped live portrait + lines of fraceTracker features
        if(bLiveFaceFound){
            if(bShowLivePortrait){
                //draw face features scaled to cropped and resized portrait
                ofPushMatrix();
                {
                    ofTranslate(livePortrait_x,livePortrait_y);
                    
                    ofSetColor(255,255,255,255);
                    portraitCropped.draw(0,0,livePortrait_w,livePortrait_h);
                    
                    
                    ofPoint temp_offset1 = faceRect.getPosition() - portraitRect.getPosition();
                    ofPoint temp_offset2 = -faceRect.getPosition();
                    
                    float temp_scaleY = portraitRect.getHeight() / portraitCropped.getHeight();
                    temp_scaleY = 1.0 / temp_scaleY;
                    
                    if(faceTracker.getFound()) {
                        ofPoint temp_p = (faceRect.getPosition() - portraitRect.getPosition())*temp_scaleY;
                        
                        if(infoText_language == 0){
                            
                            ofSetColor(200,200,200,255);
                            ofDrawBitmapString("prediccion # "+ofToString(pickedID),  temp_p.x+1,temp_p.y - 20);
                            ofDrawBitmapString("confianza # "+ofToString(theFaceRecognizer.getConfidence()),  temp_p.x+1,temp_p.y - 5);
                            
                            ofSetColor(255,255,255,255);
                            ofDrawBitmapString("prediccion # "+ofToString(pickedID),  temp_p.x,temp_p.y - 20);
                            ofDrawBitmapString("confianza # "+ofToString(theFaceRecognizer.getConfidence()),  temp_p.x,temp_p.y - 5);
                            
                        }else if(infoText_language == 1){
                            
                            ofSetColor(200,200,200,255);
                            ofDrawBitmapString("prediction # "+ofToString(pickedID),  temp_p.x+1,temp_p.y - 20);
                            ofDrawBitmapString("confidence # "+ofToString(theFaceRecognizer.getConfidence()),  temp_p.x+1,temp_p.y - 5);
                            
                            ofSetColor(255,255,255,255);
                            ofDrawBitmapString("prediction # "+ofToString(pickedID),  temp_p.x,temp_p.y - 20);
                            ofDrawBitmapString("confidence # "+ofToString(theFaceRecognizer.getConfidence()),  temp_p.x,temp_p.y - 5);
                        }
                    }
                    
                    
                    ofScale(temp_scaleY, temp_scaleY);
                    
                    ofTranslate(faceRect.getPosition() - portraitRect.getPosition());
                    ofTranslate(-faceRect.getPosition());
                    
                    int t_grow = 2;
                    ofRectangle temp_faceRect = ofRectangle(faceRect.x-t_grow,faceRect.y-t_grow,faceRect.getWidth()-t_grow*2,faceRect.getHeight()-t_grow*2);
                    
                    if(faceTracker.getFound()) {
                        featureLine_alpha = 255;
                        
                        //drop shadow
                        ofSetColor(200,200,200,featureLine_alpha);
                        ofNoFill();
                        ofRect(faceRect);
                        faceTracker.draw();
                        
                        //white face features
                        ofTranslate(-0.2, -0.2);
                        ofSetColor(255,255,255,featureLine_alpha);
                        ofRect(faceRect);
                        faceTracker.draw();
                        
                        
                    } else{
                        
                        featureLine_alpha = ofMap(ofGetElapsedTimef() - faceLostTimer, 0, faceLostZoomWait, 255, 0);
                        
                        ofPushStyle();
                        ofEnableAlphaBlending();
                        ofNoFill();
                        
                        //drop shadow
                        ofSetColor(200,200,200,featureLine_alpha);
                        LEFT_EYE_line.draw();
                        RIGHT_EYE_line.draw();
                        LEFT_EYEBROW_line.draw();
                        RIGHT_EYEBROW_line.draw();
                        NOSE_BRIDGE_line.draw();
                        NOSE_BASE_line.draw();
                        INNER_MOUTH_line.draw();
                        OUTER_MOUTH_line.draw();
                        JAW_line.draw();
                        
                        
                        ofRect(faceRect);
                        
                        //white face features
                        ofTranslate(-0.2, -0.2);
                        ofSetColor(255, 255,255,featureLine_alpha);
                        LEFT_EYE_line.draw();
                        RIGHT_EYE_line.draw();
                        LEFT_EYEBROW_line.draw();
                        RIGHT_EYEBROW_line.draw();
                        NOSE_BRIDGE_line.draw();
                        NOSE_BASE_line.draw();
                        INNER_MOUTH_line.draw();
                        OUTER_MOUTH_line.draw();
                        JAW_line.draw();
                        
                        ofRect(faceRect);
                        
                        ofDisableAlphaBlending();
                        ofPopStyle();
                        
                    }
                    
                    
                }
                ofPopMatrix();
                
                
                
                
                //draw line up of 43 students with prediction bars
                /*
                 ofNoFill();
                 float t_w = ofGetWidth()/(float)43;
                 
                 for(int i=0; i<43; i++){
                 ofSetColor(0);
                 ofRect(i*t_w, ofGetHeight() - 100, t_w, -prediction_array43[i]);
                 
                 float normalizedConfidence = ofNormalize(avgConfidence_array43[i], minConfidence, maxConfidence);
                 normalizedConfidence = 1 - normalizedConfidence;
                 if(normalizedConfidence == 1) normalizedConfidence = 0;
                 ofRect(i*t_w, ofGetHeight() - 40, t_w, normalizedConfidence*40);
                 
                 ofSetColor(255);
                 
                 theImageGrid.drawOnePortrait(i,ofPoint(i*t_w, ofGetHeight() - 80),t_w,t_w);
                 }
                 */
            }//end if(bShowLivePortrait){
            
        }// if(bLiveFaceFound){
        
        
        //-----------image grid stuff
        
        ofPushMatrix();
        {
            // ofTranslate(mouseX,mouseY);
            if(bLiveFaceFound == false){
                ofTranslate(ofGetWidth()/2,ofGetHeight()/2);
                ofScale(zoomCurrent, zoomCurrent);
                ofTranslate(portraits_gridPos.x,portraits_gridPos.y);
                
                ofSetColor(255);
                theImageGrid.draw(0,0);
            }else{
                ofTranslate(0, smallGrid_y);
                theImageGrid.drawSmall(0,0);
            }
        }
        ofPopMatrix();
        
        
        //------------tracking
        if(bShowCamera) {
            ofPushMatrix();
            {
                ofTranslate(0,0);
                
                ofSetColor(255);
                vidGrabberImage.draw(0, 0);
                // drawMat(vidGrabberImage, 0, 0);
                // vidGrabberImage.draw(0,0);
                faceTracker.draw();
                
                ofSetColor(255);
                ofNoFill();
                ofRect(portraitRect);
                
                if(faceTracker.getFound()) {
                    ofSetColor(255);
                    ofNoFill();
                    ofRect(faceRect);
                    // ofRect(portraitRect);
                    
                    if(theFaceRecognizer.getPrediction() > 0){
                        theFaceRecognizer.drawImageOfPerson(theFaceRecognizer.getPrediction(), faceRect.position + ofPoint(-50,-50),50,50);
                        
                        ofDrawBitmapString("prediction # "+ofToString(theFaceRecognizer.getPrediction()), faceRect.position + ofPoint(0,-70));
                        ofDrawBitmapString("confidence # "+ofToString(theFaceRecognizer.getConfidence()), faceRect.position + ofPoint(0,-50));
                    }
                }//end if(faceTracker.getFound())
                
                ofSetColor(255);
                faceCropped.draw(0, camHeight);
            }
            ofPopMatrix();
        }//end if bShowCamera
        
        
        //-------------picked portrait
        //---------------draw live video face and picked student
        if(bLiveFaceFound){
            
            int temp_id = pickedID;
            
            pickedImage.drawOnePortrait(theImageGrid.portrait_images[temp_id], temp_id, ofPoint(pickedPortrait_x,pickedPortrait_y), pickedPortrait_scaler);
            
            pickedImage.drawOneBigFont(temp_id, ofPoint(pickedPortrait_x,pickedPortrait_y), pickedPortrait_scaler, theImageGrid.getFirstName(temp_id), theImageGrid.getLastName(temp_id));
            
            drawInfoText(infoText_language);
            
        }
        
        
        //complete database
        if(bShowCamera) {
            ofNoFill();
            float t_w = ofGetWidth()/(float)theFaceRecognizer.getUniquePersonCount();
            for(int i=0; i<theFaceRecognizer.getUniquePersonCount(); i++){
                ofSetColor(0);
                ofRect(i*t_w, ofGetHeight() - 100, t_w, -prediction_array[i]);
                float normalizedConfidence = ofNormalize(avgConfidence_array[i], minConfidence, maxConfidence);
                ofRect(i*t_w, ofGetHeight() - 40, t_w, normalizedConfidence*40);
                
                ofSetColor(255);
                // img_rep[i].draw(i*t_w, ofGetHeight() - 80, t_w, t_w);
                theFaceRecognizer.drawImageOfPerson(i, i*t_w, ofGetHeight() - 80, t_w, t_w);
            }
            
            //draw selection of database with heighst ratings
            float temp_x = 0;
            float temp_y = 0;
            ofSetColor(255);
            ofPushMatrix();
            {
                ofTranslate(640, 0);
                
                for(int i=0; i<theFaceRecognizer.getUniquePersonCount(); i++){
                    if(prediction_array[i] > 0){
                        theFaceRecognizer.drawImageOfPerson(i, temp_x,temp_y, 50, 50);
                        temp_x += 50;
                        if(temp_x > 500){
                            temp_x = 0;
                            temp_y +=50;
                        }
                    }
                }
            }
            ofPopMatrix();
        }
        
        if(bShowGUI == true){
            gui_main.draw();
            gui_usbCam.draw();
            gui_livePortrait.draw();
            gui_tracking.draw();
            gui_image.draw();
            gui_grid.draw();
            gui_gridMovement.draw();
            gui_infoText.draw();
#ifdef USE_PROSILICA
            gui_prosilica.draw();
#endif
            ofSetColor(0);
            ofDrawBitmapString("used method "+theFaceRecognizer.getMethodName() + " with "+ofToString(theFaceRecognizer.getUniquePersonCount()) + " people", ofPoint(10,ofGetHeight() - 155));
            
            //        ofDrawBitmapString("predictions # ", ofPoint(10,ofGetHeight() - 85));
            //        ofDrawBitmapString("confidences # ", ofPoint(10,ofGetHeight() - 45));
            //
            //        ofDrawBitmapString("min Confidence # "+ofToString(minConfidence), ofPoint(10,ofGetHeight() - 200));
            //        ofDrawBitmapString("max Confidence # "+ofToString(maxConfidence), ofPoint(10,ofGetHeight() - 180));
            
            //        ofSetColor(0, 0, 255);
            //        ofPoint c = leftEye_line.getCentroid2D();
            //        ofCircle(c, 3);
        }
        
        
        if(faceTracker.getFound()){
            ofSetColor(0);
            ofFill();
            ofRect(0, ofGetHeight()-5, 5, 5);
        }
        if(bFinale){
            ofSetColor(0);
            ofFill();
            ofRect(15, ofGetHeight()-5, 5, 5);
        }
        // helveticaSmall2.drawString(" alkhdlh  lahsdlahs",mouseX,mouseY);
        
        if(bShowGUI){
            int old_oddEven = old_camRotateAmt % 2;
            int new_oddEven = camRotateAmt % 2;
            
            if(old_oddEven != new_oddEven){
                int t_h = ofGetHeight()/2;
                ofSetColor(255,0,0);
                helveticaCondBoldSmall.drawString("You selected a new camera rotation value.", 40, t_h);
                helveticaCondBoldSmall.drawString("Pleas exit the software and restart it, only then will the new setting take effect.", 40, t_h+30);
                
                
            }
        }
    }//end else if elaspeTime <
}

void ofApp::saveGui(){
    gui_mainPos = gui_main.getPosition();
    
    gui_main.saveToFile("gui_main.xml");
    gui_livePortrait.saveToFile("gui_livePortrait.xml");
    gui_usbCam.saveToFile("gui_usbCam.xml");
    gui_tracking.saveToFile("gui_tracking.xml");
    gui_image.saveToFile("gui_image.xml");
    gui_grid.saveToFile("gui_grid.xml");
    gui_gridMovement.saveToFile("gui_gridMovement.xml");
    gui_infoText.saveToFile("gui_infoText.xml");
#ifdef USE_PROSILICA
    gui_prosilica.saveToFile("gui_prosilica.xml");
#endif
    
    
}
void ofApp::keyPressed(int key) {
    
    
}
void ofApp::keyReleased(int key) {
    
    
    if(key == ' '){
        infoText_language++;
        if(infoText_language > infoText_language.getMax()) infoText_language = 0;
        gui_infoText.saveToFile("gui_infoText.xml");
    }
    
    if(key == OF_KEY_TAB){
        //ofResetElapsedTimeCounter();
        initTimer = ofGetElapsedTimef();
        initDuration = 30;
    }
    if(ofGetKeyPressed(OF_KEY_CONTROL) && ofGetKeyPressed(OF_KEY_ALT)){
        
        if(key == 7){ //'g'
            bShowGUI = !bShowGUI;
            
            if(bShowGUI == false){
                ofHideCursor();
                saveGui();
            }else{
                ofShowCursor();
            }
        }
        
        if(key == 3){//'c'
            bShowCamera = !bShowCamera;
        }
        
    }
    
    
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button)
{
    
    
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button)
{

    ofRectangle specialRect(ofGetWidth() - 200, ofGetHeight() - 200 ,200,200);
    
    unsigned long curTap = ofGetElapsedTimeMillis();
    
    if(specialRect.inside(x, y)){
    
        if(lastTap != 0 && curTap - lastTap < 250){
            
            bShowGUI = !bShowGUI;
            
            if(bShowGUI){
                ofShowCursor();
                bShowCamera = true;
            }
            else{
                saveGui();
                ofHideCursor();
                bShowCamera = false;
            }
            
            //mouseDoublePressed = true;
        }
        lastTap = curTap;
    }
}

void ofApp::resetFaces(){
    cout<<"resetFaces() "<<endl;
    bGotReset = true;
    faceTracker.reset();
    faceTracker.setHaarMinSize(haarMinSize);
    
    minConfidence = 10000000;
    maxConfidence = -1;
    memset(prediction_array, 0, sizeof(prediction_array));
    memset(avgConfidence_array, 0, sizeof(avgConfidence_array));
    memset(avgConfidenceCnt_array, 0, sizeof(avgConfidenceCnt_array));
}
void ofApp::getNewStudent(){
    newStudentCnt++;
    if(newStudentCnt >= 43) cout<<"got all students"<<endl;
    
    //load student portraits
    string folderName = "CroppedStudents";
    string imagePath = ofToDataPath(folderName);
    cout<<"imagePath "<<imagePath<<endl;
    
    ofDirectory students_dir(imagePath);
    //  dir.allowExt("pgm");
    students_dir.allowExt("tiff");
    students_dir.listDir();
    
    cout<<"load new student "<<newStudentCnt<<" of "<< students_dir.size()<<endl;
    
    newStudentFileName = students_dir.getName(newStudentCnt);
    cout<<"newStudentFileName "<<newStudentFileName<<endl;
    
    cout<<"loadImage "<<students_dir.getPath(newStudentCnt)<<endl;
    
    newStudentImage.loadImage(students_dir.getPath(newStudentCnt));
    cout<<"newStudentImage "<<newStudentImage.getWidth()<<" "<<newStudentImage.getHeight()<<endl;
    
}

vector<ofPoint> ofApp::getMinimumBoundingBox(vector <ofVec2f> _imagePoints){
    
    
    int minX = 10000;
    int minY = 10000;
    int maxX = -10000;
    int maxY = -10000;
    //from the points of which the facial feautres (eyes, mouth, nose) are defined
    //we get the extrem points so we can use it to crop out the face
    //faceTracker does not provide a simple rect that discribes the face dimensions
    for(int i=0; i<_imagePoints.size(); i++){
        int temp_x = _imagePoints[i].x;
        int temp_y = _imagePoints[i].y;
        
        if(temp_x < minX) minX = temp_x;
        if(temp_y < minY) minY = temp_y;
        if(temp_x > maxX) maxX = temp_x;
        if(temp_y > maxY) maxY = temp_y;
        
    }
    
    //we need the face rect to be a square
    
    //if face rect is heigher then wide crop at the bottom; i.e. the chin and mouth
    float temp_w = maxX - minX;
    float temp_h = maxY - minY;
    if(temp_h > temp_w){
        float diff = temp_h - temp_w;
        maxY -= diff;
    }
    
    //if face rect is wider then high then crop on left and right
    if(temp_w > temp_h){
        float diff = temp_w - temp_h;
        minX += diff/2.0;
        maxX -= diff/2.0;
    }
    
    
    vector<ofPoint> boundingPoints;
    boundingPoints.push_back(ofPoint(minX,minY));
    boundingPoints.push_back(ofPoint(maxX,maxY));
    float midX = minX + (maxX-minX)/2.0;
    float midY = minY + (maxY-minY)/2.0;
    boundingPoints.push_back(ofPoint(midX,midY));
    
    return boundingPoints;
    
}

ofRectangle ofApp::getBiggerBox(ofRectangle _rect, int _padding, float _ratio, int maxWidth, int maxHeight){
    
    ofRectangle rectForReturn;
    
    float newWidth = _rect.getWidth()+_padding;
    float newHeight = newWidth * _ratio;
    rectForReturn.setFromCenter(_rect.getCenter(),newWidth, newHeight);
    
    
    // cout<<"A rectForReturn "<<rectForReturn<<endl;
    
    if(rectForReturn.getTop() < 0){
        rectForReturn.y +=  ABS(rectForReturn.getTop());
        //cout<<"B rectForReturn "<<rectForReturn<<endl;
    }
    if(rectForReturn.getBottom() >= maxHeight){
        // cout<<"rectForReturn.getBottom() "<<rectForReturn.getBottom()<<" "<<maxHeight<<endl;
        rectForReturn.y -= rectForReturn.getBottom() - maxHeight;
        //cout<<"C rectForReturn "<<rectForReturn<<endl;
    }
    
    if(rectForReturn.getHeight() >= maxHeight){
        rectForReturn.y = 0;
        rectForReturn.setHeight(maxHeight);
        float old_w = rectForReturn.getWidth();
        rectForReturn.setWidth(maxHeight / _ratio);
        rectForReturn.x -= (rectForReturn.getWidth() - old_w)/2.0;
        //cout<<"D rectForReturn "<<rectForReturn<<endl;
    }
    
    
    if(rectForReturn.getLeft() < 0){
        rectForReturn.x += ABS(rectForReturn.getLeft());
        //cout<<"E rectForReturn "<<rectForReturn<<endl;
    }
    if(rectForReturn.getRight() >= maxWidth){
        rectForReturn.x -= rectForReturn.getRight() - maxWidth;
        //cout<<"F rectForReturn "<<rectForReturn<<endl;
    }
    
    //cout<<"Z rectForReturn "<<rectForReturn<<endl;
    
    return rectForReturn;
    //return _rect;
}

void ofApp::drawInfoText(int _language){
    //_language
    //0 = spanish
    //1 = eng
    
    /*
     0 - Facial recognition engine: -
     1 - Fisher, Eigen, LBPN -
     2 - Detected similarity score: -
     3 - points -
     4 - With the face of disappeared Ayotzinapa student: -
     5 -  -
     6 - Level of confidence: -
     7 - % -
     8 - Result: -
     9 - student not found -
     10 -  -
     
     
     0 - Identificación de rasgos faciales: -
     1 - Fisher, Eigen, LBPN -
     2 - El sistema detecta un parecido de: -
     3 - puntos -
     4 - Con el rostro del estudiante normalista desaparecido -
     5 - en Ayotzinapa: -
     6 -  -
     7 - Nivel de confianza: -
     8 - % -
     9 - Resultado: -
     10 - estudiante no encontrado. -
     11 -  -
     */
    
    //----------------------spanish------------------------
    if(_language == 0){
        int line_x = infoText_x;
        int line = 0;
        float temp_strW = 0;
        string line_str;
        
        //Identificación de rasgos faciales:
        line_str = infoText[0];
        helveticaCondBoldSmall.drawString(line_str, line_x, infoText_y+(0*infoText_lineHeight));
        //Fisher, Eigen, LBPN
        temp_strW = helveticaCondBoldSmall.stringWidth(line_str);
        line_x += temp_strW;
        line_str = " " + infoText[1];
        helveticaSmall.drawString(line_str, line_x, infoText_y+(line*infoText_lineHeight));
        
        //El sistema detecta un parecido de:
        temp_strW = helveticaCondBoldSmall.stringWidth(line_str);
        line_x += temp_strW;
        line_str = "  " + infoText[2];
        helveticaCondBoldSmall.drawString(line_str, line_x, infoText_y+(line*infoText_lineHeight));
        //puntos
        temp_strW = helveticaCondBoldSmall.stringWidth(line_str);
        line_x += temp_strW;
        line_str = " " + ofToString(pickedPortrait_confi)+ " " + infoText[3];
        helveticaSmall.drawString(line_str,line_x, infoText_y+(line*infoText_lineHeight));
        
        //Con el rostro del estudiante normalista desaparecido
        line = 1;
        temp_strW = helveticaCondBoldSmall.stringWidth(line_str);
        line_x = infoText_x;
        line_str = infoText[4];
        helveticaCondBoldSmall.drawString(line_str, line_x, infoText_y+(line*infoText_lineHeight));
        
        //en Ayotzinapa:
        temp_strW = helveticaCondBoldSmall.stringWidth(line_str);
        line_x += temp_strW;
        line_str = " "+infoText[5];
        helveticaCondBoldSmall.drawString(line_str, line_x, infoText_y+(line*infoText_lineHeight));
        
        //first and last name
        temp_strW = helveticaCondBoldSmall.stringWidth(line_str);
        line_x += temp_strW;
        line_str = " "+theImageGrid.getFirstName(pickedID)+" "+theImageGrid.getLastName(pickedID);
        helveticaSmall.drawString(line_str, line_x, infoText_y+(line*infoText_lineHeight));
        
        
        line = 2;
        temp_strW = helveticaCondBoldSmall.stringWidth(line_str);
        line_x = infoText_x;
        line_str = infoText[7];
        //Nivel de confianza:
        helveticaCondBoldSmall.drawString(line_str, line_x, infoText_y+(line*infoText_lineHeight));
        //%
        int temp_percent = ofMap(pickedPortrait_confi, minConfidence, maxConfidence, 4, 75);
        temp_strW = helveticaCondBoldSmall.stringWidth(line_str);
        line_x += temp_strW;
        line_str = " "+ofToString(temp_percent)+" " +infoText[8];
        helveticaSmall.drawString(line_str, line_x, infoText_y+(line*infoText_lineHeight));
        
        
        //Resultado:
        temp_strW = helveticaCondBoldSmall.stringWidth(line_str);
        line_x += temp_strW;
        line_str = "  " +infoText[9];
        helveticaCondBoldSmall.drawString(line_str, line_x, infoText_y+(line*infoText_lineHeight));
        
        //estudiante no encontrado.
        temp_strW = helveticaCondBoldSmall.stringWidth(line_str);
        line_x += temp_strW;
        line_str = " " +infoText[10];
        
        temp_strW = helveticaCondBoldSmall.stringWidth(infoText[9]);
        if(bFinale) helveticaSmall.drawString(line_str, line_x, infoText_y+(line*infoText_lineHeight));
    }
    
    //----------------------english------------------------
    if(_language == 1){
        
        int line_x = infoText_x;
        int line = 0;
        string line_str;
        //Facial recognition engine:
        line_str = infoText[20];
        helveticaCondBoldSmall.drawString(line_str, line_x, infoText_y+(0*infoText_lineHeight));
        //Fisher, Eigen, LBPN
        float temp_strW = helveticaCondBoldSmall.stringWidth(line_str);
        line_x += temp_strW;
        line_str = " " + infoText[21];
        helveticaSmall.drawString(line_str, line_x, infoText_y+(line*infoText_lineHeight));
        
        //Detected similarity score:
        temp_strW = helveticaCondBoldSmall.stringWidth(line_str);
        line_x += temp_strW;
        line_str = "  " + infoText[22];
        helveticaCondBoldSmall.drawString(line_str, line_x, infoText_y+(line*infoText_lineHeight));
        //points
        temp_strW = helveticaCondBoldSmall.stringWidth(line_str);
        line_x += temp_strW;
        line_str = " " + ofToString(theFaceRecognizer.getConfidence())+ " " + infoText[23];
        // line_str = " " + ofToString(pickedPortrait_confi)+ " " + infoText[23];
        helveticaSmall.drawString(line_str, line_x, infoText_y+(line*infoText_lineHeight));
        
        
        line_x = infoText_x;
        line = 1;
        line_str = infoText[24];
        //With the face of disappeared Ayotzinapa student:
        helveticaCondBoldSmall.drawString(line_str, line_x, infoText_y+(line*infoText_lineHeight));
        
        //en Ayotzinapa:
        //helveticaCondBoldSmall.drawString(infoText[25], infoText_x, infoText_y+(line*infoText_lineHeight));
        //first and last name
        temp_strW = helveticaCondBoldSmall.stringWidth(line_str);
        line_x += temp_strW;
        line_str = " "+theImageGrid.getFirstName(pickedID)+" "+theImageGrid.getLastName(pickedID);
        helveticaSmall.drawString(line_str, line_x, infoText_y+(line*infoText_lineHeight));
        
        
        line = 2;
        line_x = infoText_x;
        line_str = infoText[26];
        //Level of confidence:
        helveticaCondBoldSmall.drawString(line_str, line_x, infoText_y+(line*infoText_lineHeight));
        //%
        int temp_percent = ofMap(pickedPortrait_confi, minConfidence, maxConfidence, 4, 75);
        temp_strW = helveticaCondBoldSmall.stringWidth(line_str);
        line_x += temp_strW;
        line_str = " "+ofToString(temp_percent)+" " +infoText[27];
        helveticaSmall.drawString(line_str, line_x, infoText_y+(line*infoText_lineHeight));
        
        //Result
        temp_strW = helveticaCondBoldSmall.stringWidth(line_str);
        line_x += temp_strW;
        line_str = "  "+infoText[28];
        helveticaCondBoldSmall.drawString(line_str, line_x, infoText_y+(line*infoText_lineHeight));
        //student not found
        temp_strW = helveticaCondBoldSmall.stringWidth(line_str);
        line_x += temp_strW;
        line_str = " " + infoText[29];
        if(bFinale) helveticaSmall.drawString(line_str, line_x, infoText_y+(line*infoText_lineHeight));
    }
    
}

void ofApp::drawInfoText2(int _language){
    //_language
    //0 = spanish
    //1 = eng
    
    /*
     0 - Facial recognition engine: -
     1 - Fisher, Eigen, LBPN -
     2 - Detected similarity score: -
     3 - points -
     4 - With the face of disappeared Ayotzinapa student: -
     5 -  -
     6 - Level of confidence: -
     7 - % -
     8 - Result: -
     9 - student not found -
     10 -  -
     
     
     0 - Identificación de rasgos faciales: -
     1 - Fisher, Eigen, LBPN -
     2 - El sistema detecta un parecido de: -
     3 - puntos -
     4 - Con el rostro del estudiante normalista desaparecido -
     5 - en Ayotzinapa: -
     6 -  -
     7 - Nivel de confianza: -
     8 - % -
     9 - Resultado: -
     10 - estudiante no encontrado. -
     11 -  -
     */
    
    //----------------------spanish------------------------
    if(_language == 0){
        int line = 0;
        //Identificación de rasgos faciales:
        helveticaCondBoldSmall.drawString(infoText[0], infoText_x, infoText_y+(0*infoText_lineHeight));
        //Fisher, Eigen, LBPN
        float temp_strW = helveticaCondBoldSmall.stringWidth(infoText[0]);
        helveticaSmall.drawString(" " + infoText[1], temp_strW+infoText_x, infoText_y+(line*infoText_lineHeight));
        
        line = 1;
        //El sistema detecta un parecido de:
        helveticaCondBoldSmall.drawString(infoText[2], infoText_x, infoText_y+(line*infoText_lineHeight));
        //puntos
        temp_strW = helveticaCondBoldSmall.stringWidth(infoText[2]);
        helveticaSmall.drawString(" " + ofToString(pickedPortrait_confi)+ " " + infoText[3], temp_strW+infoText_x, infoText_y+(line*infoText_lineHeight));
        
        
        line = 2;
        //Con el rostro del estudiante normalista desaparecido
        helveticaCondBoldSmall.drawString(infoText[4], infoText_x, infoText_y+(line*infoText_lineHeight));
        
        line = 3;
        //en Ayotzinapa:
        helveticaCondBoldSmall.drawString(infoText[5], infoText_x, infoText_y+(line*infoText_lineHeight));
        //first and last name
        temp_strW = helveticaCondBoldSmall.stringWidth(infoText[5]);
        helveticaSmall.drawString(" "+theImageGrid.getFirstName(pickedID)+" "+theImageGrid.getLastName(pickedID), temp_strW+infoText_x, infoText_y+(line*infoText_lineHeight));
        
        
        line = 4;
        //Nivel de confianza:
        helveticaCondBoldSmall.drawString(infoText[7], infoText_x, infoText_y+(line*infoText_lineHeight));
        //%
        temp_strW = helveticaCondBoldSmall.stringWidth(infoText[7]);
        
        int temp_percent = ofMap(pickedPortrait_confi, minConfidence, maxConfidence, 4, 75);
        //cout<<"temp_percent "<<temp_percent<<" pickedPortrait_confi "<<pickedPortrait_confi<<endl;
        
        helveticaSmall.drawString(" "+ofToString(temp_percent)+" " +infoText[8], temp_strW+infoText_x, infoText_y+(line*infoText_lineHeight));
        
        
        line = 5;
        //Resultado:
        helveticaCondBoldSmall.drawString(infoText[9], infoText_x, infoText_y+(line*infoText_lineHeight));
        //estudiante no encontrado.
        temp_strW = helveticaCondBoldSmall.stringWidth(infoText[9]);
        if(bFinale) helveticaSmall.drawString(" " + infoText[10], temp_strW+infoText_x, infoText_y+(line*infoText_lineHeight));
    }
    
    //----------------------english------------------------
    if(_language == 1){
        int line = 0;
        //Identificación de rasgos faciales:
        helveticaCondBoldSmall.drawString(infoText[20], infoText_x, infoText_y+(0*infoText_lineHeight));
        //Fisher, Eigen, LBPN
        float temp_strW = helveticaCondBoldSmall.stringWidth(infoText[20]);
        helveticaSmall.drawString(" " + infoText[21], temp_strW+infoText_x, infoText_y+(line*infoText_lineHeight));
        
        line = 1;
        //El sistema detecta un parecido de:
        helveticaCondBoldSmall.drawString(infoText[22], infoText_x, infoText_y+(line*infoText_lineHeight));
        //puntos
        temp_strW = helveticaCondBoldSmall.stringWidth(infoText[22]);
        helveticaSmall.drawString(" " + ofToString(pickedPortrait_confi)+ " " + infoText[23], temp_strW+infoText_x, infoText_y+(line*infoText_lineHeight));
        
        
        line = 2;
        //Con el rostro del estudiante normalista desaparecido
        helveticaCondBoldSmall.drawString(infoText[24], infoText_x, infoText_y+(line*infoText_lineHeight));
        
        line = 3;
        //en Ayotzinapa:
        helveticaCondBoldSmall.drawString(infoText[25], infoText_x, infoText_y+(line*infoText_lineHeight));
        //first and last name
        temp_strW = helveticaCondBoldSmall.stringWidth(infoText[25]);
        helveticaSmall.drawString(theImageGrid.getFirstName(pickedID)+" "+theImageGrid.getLastName(pickedID), temp_strW+infoText_x, infoText_y+(line*infoText_lineHeight));
        
        
        line = 4;
        //Nivel de confianza:
        helveticaCondBoldSmall.drawString(infoText[26], infoText_x, infoText_y+(line*infoText_lineHeight));
        //%
        temp_strW = helveticaCondBoldSmall.stringWidth(infoText[26]);
        
        int temp_percent = ofMap(pickedPortrait_confi, minConfidence, maxConfidence, 4, 75);
        
        
        helveticaSmall.drawString(" "+ofToString(temp_percent)+" " +infoText[27], temp_strW+infoText_x, infoText_y+(line*infoText_lineHeight));
        
        
        line = 5;
        //Resultado:
        helveticaCondBoldSmall.drawString(infoText[28], infoText_x, infoText_y+(line*infoText_lineHeight));
        //estudiante no encontrado.
        temp_strW = helveticaCondBoldSmall.stringWidth(infoText[28]);
        if(bFinale) helveticaSmall.drawString(" " + infoText[29], temp_strW+infoText_x, infoText_y+(line*infoText_lineHeight));
    }
    
}

ofRectangle ofApp::growRect(ofRectangle _from, ofRectangle _to, float _step, float _maxW, float _maxH){
    
    //  cout<<"_step "<<_step<<endl;
    
    float temp_x = ofLerp(_from.x, _to.x, _step);
    float temp_y = ofLerp(_from.y, _to.y, _step);
    float temp_w = ofLerp(_from.getWidth(), _to.getWidth(), _step);
    float temp_h = ofLerp(_from.getHeight(), _to.getHeight(), _step);
    
    ofRectangle temp_rect(temp_x,temp_y,temp_w,temp_h);
    // cout<<"1 temp_rect "<<temp_rect<<endl;
    
    temp_x = ofClamp(temp_x, 0, _maxW);
    temp_y = ofClamp(temp_y, 0, _maxH);
    temp_rect = ofRectangle (temp_x,temp_y,temp_w,temp_h);
    //  cout<<"2 temp_rect "<<temp_rect<<endl;
    temp_w = ofClamp(temp_w, 0, _maxW);
    temp_h = ofClamp(temp_h, 0, _maxH);
    temp_rect = ofRectangle (temp_x,temp_y,temp_w,temp_h);
    //  cout<<"3 temp_rect "<<temp_rect<<endl;
    return temp_rect;
}


