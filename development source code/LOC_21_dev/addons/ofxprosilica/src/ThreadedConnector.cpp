//
//  ThreadedConnector.cpp
//  example
//
//  Created by Ties East on 10/22/14.
//
//

#include "ThreadedConnector.h"

namespace ofxProsilica {
	
	ThreadedConnector::ThreadedConnector() {
        
       		;
	}
	
	ThreadedConnector::~ThreadedConnector() {
		close();
	}
	
	
    bool ThreadedConnector::setup() {
        return initGrabber();
    }
    
	bool ThreadedConnector::initGrabber(){
		Tgrabber.setDeviceID(TdeviceID);
		TdeviceID = Tgrabber.getDeviceID();
		Tgrabber.setPixelFormat(TpixelFormat);
		TpixelFormat = Tgrabber.getPixelFormat();
		
	//	Tgrabber.initGrabber();
		
		setupParameters();
		startThread();
		
		return true;
	}
	
	void ThreadedConnector::close() {
		
		waitForThread();
		Tgrabber.close();
		
	}
	
	
	void ThreadedConnector::update() {
		
		if(lock()) {
			TbIsFrameNew = Tgrabber.isFrameNew();
			if(TbIsFrameNew) {
				// copy pixels
				ofPixelsRef pRef = Tgrabber.getPixelsRef();
				Tpixels.setFromPixels(pRef.getPixels(), pRef.getWidth(), pRef.getHeight(), pRef.getNumChannels());
				
				Twidth = Tgrabber.getWidth();
				Theight = Tgrabber.getHeight();
				TbInitialized = Tgrabber.isInitialized();
				updateParameters();
			}
			unlock();
            
        }
        
        
        if (isMainThread()) {
            if (p_FlipH.get()) {
                if (p_FlipV.get()) {
                    Tpixels.mirror(true, true);
                }
                else {
                    Tpixels.mirror(false, true);
                }
            }
            else if (p_FlipV.get()) {
                Tpixels.mirror(true, false);
            }
            if (TbIsFrameNew){
                
                
                if (texture.getWidth() != Twidth || texture.getHeight() != Theight || texture.getTextureData().glTypeInternal != GL_LUMINANCE) {
                    texture.clear();
                    texture.allocate(Twidth, Theight, GL_LUMINANCE);
                }
				
                texture.loadData(Tpixels.getPixels(), Twidth, Theight, GL_LUMINANCE);
           
            }
        }
        
            
	}
	
	void ThreadedConnector::threadedFunction() {
		while(isThreadRunning()) {
			Tgrabber.update();
		//	getThreadName()
			//	float sleepTime = 500 / frameRate;
			//	printf(" %f \n", sleepTime);
			
//			yield();
			sleep(10);
			
		}
	}	
	
	
	bool ThreadedConnector::isFrameNew() {
		return TbIsFrameNew;
	}
	
	
	unsigned char*  ThreadedConnector::getPixels() {
		return Tpixels.getPixels();
	}
	
	
	ofPixelsRef  ThreadedConnector::getPixelsRef() {
		return Tpixels;
	}
	
	
	void ThreadedConnector::setDeviceID(int _deviceID) {
		TdeviceID = _deviceID;
	}
	
	int ThreadedConnector::getDeviceID() {
		return TdeviceID;
	}
	
	
	float ThreadedConnector::getWidth() {
		return Twidth;
	}
	
	float ThreadedConnector::getHeight() {
		return Theight;
	}
	
	bool ThreadedConnector::setPixelFormat(ofPixelFormat _pixelFormat) {
		if (TbInitialized)
			ofLog(OF_LOG_ERROR, "BaseGrabber: %i: setPixelFormat(): can't set pixel format while grabber is running", TdeviceID);
		else {
			TpixelFormat = _pixelFormat;
			return true;
		}
		return false;
	}
	
	ofPixelFormat ThreadedConnector::getPixelFormat() {
		return TpixelFormat;
	}
	
	
	
	bool ThreadedConnector::isInitialized() {
		return TbInitialized;
	}
	
	vector<ofVideoDevice> ThreadedConnector::listDevices(){
		
		vector <ofVideoDevice> devices;
		if(isThreadRunning()) {
			if (lock()) {
				devices = Tgrabber.listDevices();
				unlock();
			}
		}
		else
			devices = Tgrabber.listDevices();
		
		return devices;
	}
	
	
	void ThreadedConnector::setupParameters() {
		parameters.clear();
		parameters.setName("camera " + ofToString(TdeviceID));
		parameters.add(p_PrintAttributes.set("print features", false));
		
		parameters.add(p_ShowMe.set("show", false));
		parameters.add(p_FrameRate.set("framerate", 30, 1, 60));
		parameters.add(p_ROIWidth.set("ROIwidth", 1, 0, 1));
		parameters.add(p_ROIHeight.set("ROIheight", 1, 0, 1));
		parameters.add(p_ROIX.set("ROIx", .5, 0, 1));
		parameters.add(p_ROIY.set("ROIy", .5, 0, 1));
		parameters.add(p_Exposure.set("exposure", 0, 0, 1));
		parameters.add(p_AutoExposure.set("Auto Exposure", false));
		parameters.add(p_AutoExposureOnce.set("ae once", false));
		parameters.add(p_AutoExposureTarget.set("ae target", .2, 0, 1));
		parameters.add(p_AutoExposureMax.set("ae max", .5, 0, 1));
		parameters.add(p_AutoExposureMaxForFrameRate.set("ae max for framerate", false));
		parameters.add(p_AutoExposureRate.set("ae rate", .1, 0, 1));
		parameters.add(p_AutoExposureAdjustTol.set("ae tolerance", .05, 0, 1));
		parameters.add(p_AutoExposureOutliers.set("ae outliers", .05, 0, 1));
		parameters.add(p_Gain.set("gain", 0, 0, 1));
		parameters.add(p_FlipH.set("flip H", false));
		parameters.add(p_FlipV.set("flip V", false));
		parameters.add(p_Reset.set("reset", false));
		
		frameRate = 50;
	}
	
	
	void ThreadedConnector::setAllParametersFromCam() {
		p_FrameRate.setMax(Tgrabber.getFrameRateMax());
		frameRate = p_FrameRate = Tgrabber.getFrameRate();
		ROIWidth = p_ROIWidth = Tgrabber.getROIWidth();
		ROIHeight = p_ROIHeight = Tgrabber.getROIHeight();
		ROIX = p_ROIX = Tgrabber.getROIX();
		ROIY = p_ROIY = Tgrabber.getROIY();
		exposure = p_Exposure = Tgrabber.getExposure();
		autoExposure = p_AutoExposure = Tgrabber.getAutoExposure();
		autoExposureAdjustTol = p_AutoExposureAdjustTol = Tgrabber.getAutoExposureAdjustTol();
		autoExposureMax = p_AutoExposureMax = Tgrabber.getAutoExposureMax();
		autoExposureOutliers = p_AutoExposureOutliers = Tgrabber.getAutoExposureOutliers();
		autoExposureRate = p_AutoExposureRate = Tgrabber.getAutoExposureRate();
		autoExposureTarget = p_AutoExposureTarget = Tgrabber.getAutoExposureTarget();
		gain = p_Gain = Tgrabber.getGain();
	}
	
	void ThreadedConnector::updateParameters() {
		if (Tgrabber.isInitialized() && exposure != p_Exposure) {
			exposure = p_Exposure;
			Tgrabber.setExposure(exposure); }
		
		if( Tgrabber.isInitialized()) exposure = p_Exposure = Tgrabber.getExposure();
		
		if ( Tgrabber.isInitialized() && autoExposure != p_AutoExposure) {
			autoExposure = p_AutoExposure;
			Tgrabber.setAutoExposure(autoExposure); }
		
		if( Tgrabber.isInitialized()) autoExposure = p_AutoExposure = Tgrabber.getAutoExposure();
		
		if ( Tgrabber.isInitialized() && p_AutoExposureOnce) {
			p_AutoExposureOnce = false;
			Tgrabber.setAutoExposureOnce(); }
		
		if ( Tgrabber.isInitialized() && autoExposureAdjustTol != p_AutoExposureAdjustTol) {
			autoExposureAdjustTol = p_AutoExposureAdjustTol;
			Tgrabber.setAutoExposureAdjustTol(autoExposureAdjustTol); }
		
		if ( Tgrabber.isInitialized() && autoExposureMax != p_AutoExposureMax) {
			autoExposureMax = p_AutoExposureMax;
			Tgrabber.setAutoExposureMax(autoExposureMax); }
		
		if ( Tgrabber.isInitialized() && p_AutoExposureMaxForFrameRate) {
			p_AutoExposureMaxForFrameRate = false;
			Tgrabber.setAutoExposureMaxForCurrentFrameRate();
			autoExposureMax = p_AutoExposureMax = Tgrabber.getAutoExposureMax(); }
		
		if ( Tgrabber.isInitialized() && autoExposureOutliers != p_AutoExposureOutliers) {
			autoExposureOutliers = p_AutoExposureOutliers;
			Tgrabber.setAutoExposureOutliers(autoExposureOutliers); }
		
		if ( Tgrabber.isInitialized() && autoExposureRate != p_AutoExposureRate) {
			autoExposureRate = p_AutoExposureRate;
			Tgrabber.setAutoExposureRate(autoExposureRate); }
		
		if ( Tgrabber.isInitialized() && autoExposureTarget != p_AutoExposureTarget) {
			autoExposureTarget = p_AutoExposureTarget;
			Tgrabber.setAutoExposureTarget(autoExposureTarget); }
		
		if ( Tgrabber.isInitialized() && gain != p_Gain) {
			gain = p_Gain;
			Tgrabber.setGain(gain); }
		
		if ( Tgrabber.isInitialized() && p_PrintAttributes.get()) {
			p_PrintAttributes.set(false);
			Tgrabber.listAttributes(); }
		
		if ( Tgrabber.isInitialized() && frameRate != p_FrameRate) {
			frameRate = p_FrameRate;
			Tgrabber.setFrameRate(frameRate); }
		
		if ( Tgrabber.isInitialized() && ROIWidth!= p_ROIWidth) {
			ROIWidth = p_ROIWidth;
			Tgrabber.setROIWidth(ROIWidth); }
		
		if ( Tgrabber.isInitialized() && ROIHeight != p_ROIHeight) {
			ROIHeight = p_ROIHeight;
			Tgrabber.setROIHeight(ROIHeight); }
		
		if ( Tgrabber.isInitialized() && ROIX != p_ROIX) {
			ROIX = p_ROIX;
			Tgrabber.setROIX(ROIX); }
		
		if ( Tgrabber.isInitialized() && ROIY != p_ROIY) {
			ROIY = p_ROIY;
			Tgrabber.setROIY(ROIY); }
		
		if ( Tgrabber.isInitialized() && p_Reset.get()) {
			p_Reset.set(false);
			Tgrabber.resetAttributes();
			setAllParametersFromCam(); }
	}
}