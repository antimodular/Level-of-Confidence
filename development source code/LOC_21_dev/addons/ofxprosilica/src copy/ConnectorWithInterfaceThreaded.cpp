#include "ConnectorWithInterface.h"


namespace ofxProsilica {
	
	
	
	bool ConnectorWithInterface::setup() {
		return initInterface();
	}
	
	bool ConnectorWithInterface::initInterface() {
		Connector::initConnector();
		setupParameters();
        bLoadFromInterface = false;
		
		return true;
	}
	
	void ConnectorWithInterface::update(){
		Connector::update();
        
        if (bInitialized) {
            if (bLoadFromInterface) { // reload from interface if camera is (re)initialized
                bLoadFromInterface = false;
                setAllParametersFromInterface();
            }
            updateParameters();
           // cout<<"getIntAttribute rate "<< getFloatAttribute("rate")<<endl;
        }
		else 
            bLoadFromInterface = true;
	}
	
	void ConnectorWithInterface::setupParameters() {
		parameters.clear();
		//string dID = "unknown";
       // int dID = -1;
		if (requestedDeviceID != -1)
            dID = (int)requestedDeviceID; //ofToString(requestedDeviceID);
		parameters.setName("camera ");
        parameters.add(dID.set("ID", -1, -1, 700000));
        parameters.add(p_FrameRate.set("framerate", 25, 1, 60));
		parameters.add(p_PrintAttributes.set("print features", false));
		parameters.add(p_PrintIpSettings.set("print IP settings", false));
		parameters.add(p_Reset.set("reset", false));
//		parameters.add(p_PersistentIp.set("persistent IP", false));
		
		parameters.add(p_FrameRate.set("framerate", 25, 1, 60));
//        parameters.add(p_FrameRateMin.set("framerateMin", 25, 1, 60));
//        parameters.add(p_FrameRateMax.set("framerateMax", 25, 1, 60));
        
		parameters.add(p_ROIWidth.set("ROIwidth", 648, 1, 1152)); //648
		parameters.add(p_ROIHeight.set("ROIheight", 488, 1, 900)); //488
		parameters.add(p_ROIX.set("ROIx", .5, 0, 1));
		parameters.add(p_ROIY.set("ROIy", .5, 0, 1));
		
		if(getPixelFormat() == OF_PIXELS_RGB) {
			parameters.add(p_Gamma.set("gamma", 0, 0.5, 1.5));
			parameters.add(p_Hue.set("hue", 0, -10, 10));
			parameters.add(p_Saturation.set("saturation", 0, 0, 2));			
		}
		
		exposureParameters.setName("exposure");
		exposureParameters.add(p_Exposure.set("exposure", 30, 0, 1000000));
		exposureParameters.add(p_AutoExposure.set("Auto Exposure", true));
		exposureParameters.add(p_AutoExposureOnce.set("ae once", false));
		exposureParameters.add(p_AutoExposureTarget.set("ae target", 33, 0, 100));
		exposureParameters.add(p_AutoExposureRate.set("ae rate", 10, 1, 100));
		exposureParameters.add(p_AutoExposureAdjustTol.set("ae tolerance", 5, 0, 50));
		exposureParameters.add(p_AutoExposureOutliers.set("ae outliers", 50, 0, 1000));
		exposureParameters.add(p_AutoExposureMin.set("ae min", 100, 100, 1000000));
		//exposureParameters.add(p_AutoExposureMax.set("ae max", 40000, 30, 1000000));
        exposureParameters.add(p_AutoExposureMax.set("ae max", 20000, 100, 40000));
		exposureParameters.add(p_AutoExposureMaxForFrameRate.set("ae max for framerate", false));
		parameters.add(exposureParameters);
		
		if(getPixelFormat() != OF_PIXELS_RGB) {
			parameters.add(p_Gain.set("gain", 0, 0, 1));
		}
		else {
			gainParameters.setName("gain");
			gainParameters.add(p_Gain.set("gain", 30, 0, 1000000));
			gainParameters.add(p_AutoGain.set("Auto Gain", true));
			gainParameters.add(p_AutoGainOnce.set("ag once", false));
			gainParameters.add(p_AutoGainTarget.set("ag target", 10, 1, 100));
			gainParameters.add(p_AutoGainRate.set("ag rate", 10, 1, 100));
			gainParameters.add(p_AutoGainAdjustTol.set("ag tolerance", 5, 0, 50));
			gainParameters.add(p_AutoGainOutliers.set("ag outliers", 50, 0, 1000));
			gainParameters.add(p_AutoGainMin.set("ag min", 0, 0, 30));
			gainParameters.add(p_AutoGainMax.set("ag max", 0, 0, 30));
			parameters.add(gainParameters);
			
			whiteBalanceParameters.setName("white balance");
			whiteBalanceParameters.add(p_WhiteBalanceRed.set("Red", 30, 80, 300));
			whiteBalanceParameters.add(p_WhiteBalanceBlue.set("Blue", 30, 80, 300));
			whiteBalanceParameters.add(p_AutoWhiteBalance.set("Auto", false));
			whiteBalanceParameters.add(p_AutoWhiteBalanceOnce.set("Auto Once", false));
			whiteBalanceParameters.add(p_AutoWhiteBalanceRate.set("Auto Rate", 10, 1, 100));
			whiteBalanceParameters.add(p_AutoWhiteBalanceAdjustTol.set("Auto Ajust Tolerance", 10, 1, 100));
			parameters.add(whiteBalanceParameters);
		}
	}
	
	void ConnectorWithInterface::setAllParametersFromCam() {
//        persistentIp = p_PersistentIp = isIpPersistent();
		p_FrameRate.setMin(getFrameRateMin());
		p_FrameRate.setMax(getFrameRateMax());
		
		p_ROIWidth.setMin(getROIWidthRawMin());
		p_ROIWidth.setMax(getROIWidthRawMax());
		p_ROIHeight.setMin(getROIHeightRawMin());
		p_ROIHeight.setMax(getROIHeightRawMax());
		
		p_Exposure.setMin(getExposureRawMin());
	//	p_Exposure.setMax(getExposureRawMax());
		p_Exposure.setMax(getExposureRawMin() + 100000);
		p_AutoExposureTarget.setMin(getAutoExposureTargetRawMin());
		p_AutoExposureTarget.setMax(getAutoExposureTargetRawMax());
		p_AutoExposureAdjustTol.setMin(getAutoExposureAdjustTolRawMin());
		p_AutoExposureAdjustTol.setMax(getAutoExposureAdjustTolRawMax());
		p_AutoExposureMin.setMin(getAutoExposureMinRawMin());
		p_AutoExposureMin.setMax(getAutoExposureMinRawMax());
		p_AutoExposureMax.setMin(getAutoExposureMaxRawMin());
        p_AutoExposureMax.setMax(getAutoExposureMaxRawMin() + 200000); //getAutoExposureMaxRawMax());
		p_AutoExposureOutliers.setMin(getAutoExposureOutliersRawMin());
		p_AutoExposureOutliers.setMax(getAutoExposureOutliersRawMax());
		p_AutoExposureRate.setMin(getAutoExposureRateRawMin());
		p_AutoExposureRate.setMax(getAutoExposureRateRawMax());
		
		p_Gain.setMin(getGainRawMin());
		p_Gain.setMax(getGainRawMax());
		if (getPixelFormat() == OF_PIXELS_RGB) {
			p_Gamma.setMin(getGammaRawMin());
			p_Gamma.setMax(getGammaRawMax());
			p_Hue.setMin(getHueRawMin());
			p_Hue.setMax(getHueRawMax());
			p_Saturation.setMin(getSaturationRawMin());
			p_Saturation.setMax(getSaturationRawMax());
			
			p_AutoGainTarget.setMin(getAutoGainTargetRawMin());
			p_AutoGainTarget.setMax(getAutoGainTargetRawMax());
			p_AutoGainAdjustTol.setMin(getAutoGainAdjustTolRawMin());
			p_AutoGainAdjustTol.setMax(getAutoGainAdjustTolRawMax());
			p_AutoGainMin.setMin(getAutoGainMinRawMin());
			p_AutoGainMin.setMax(getAutoGainMinRawMax());
			p_AutoGainMax.setMin(getAutoGainMaxRawMin());
			p_AutoGainMax.setMax(getAutoGainMaxRawMax());
			p_AutoGainOutliers.setMin(getAutoGainOutliersRawMin());
			p_AutoGainOutliers.setMax(getAutoGainOutliersRawMax());
			p_AutoGainRate.setMin(getAutoGainRateRawMin());
			p_AutoGainRate.setMax(getAutoGainRateRawMax());
			
			p_WhiteBalanceRed.setMin(getWhiteBalanceRedRawMin());
			p_WhiteBalanceRed.setMax(getWhiteBalanceRedRawMax());
			p_WhiteBalanceBlue.setMin(getWhiteBalanceBlueRawMin());
			p_WhiteBalanceBlue.setMax(getWhiteBalanceBlueRawMax());
			p_AutoWhiteBalanceRate.setMin(getAutoWhiteBalanceRateRawMin());
			p_AutoWhiteBalanceRate.setMax(getAutoWhiteBalanceRateRawMax());
			p_AutoWhiteBalanceAdjustTol.setMin(getAutoWhiteBalanceAdjustTolRawMin());
			p_AutoWhiteBalanceAdjustTol.setMax(getAutoWhiteBalanceAdjustTolRawMax());
		}
		
		frameRate = p_FrameRate = getFrameRate();
		ROIWidth = p_ROIWidth = getROIWidthRaw();
		ROIHeight = p_ROIHeight = getROIHeightRaw();
		ROIX = p_ROIX = getROIX();
		ROIY = p_ROIY = getROIY();
		
		exposure = p_Exposure = getExposureRaw();
		autoExposure = p_AutoExposure = getAutoExposure();
		autoExposureAdjustTol = p_AutoExposureAdjustTol = getAutoExposureAdjustTolRaw();
		autoExposureMin = p_AutoExposureMin = getAutoExposureMinRaw();
		autoExposureMax = p_AutoExposureMax = getAutoExposureMaxRaw();
		autoExposureOutliers = p_AutoExposureOutliers = getAutoExposureOutliersRaw();
		autoExposureRate = p_AutoExposureRate = getAutoExposureRateRaw();
		autoExposureTarget = p_AutoExposureTarget = getAutoExposureTargetRaw();
				
		gain = p_Gain = getGainRaw();
		if (getPixelFormat() == OF_PIXELS_RGB) {
			gamma = p_Gamma = getGammaRaw();
			hue = p_Hue = getHueRaw();
			saturation = p_Saturation = getSaturationRaw();
			
			gain = p_Gain = getGainRaw();
			autoGain = p_AutoGain = getAutoGain();
			autoGainAdjustTol = p_AutoGainAdjustTol = getAutoGainAdjustTolRaw();
			autoGainMin = p_AutoGainMin = getAutoGainMinRaw();
			autoGainMax = p_AutoGainMax = getAutoGainMaxRaw();
			autoGainOutliers = p_AutoGainOutliers = getAutoGainOutliersRaw();
			autoGainRate = p_AutoGainRate = getAutoGainRateRaw();
			autoGainTarget = p_AutoGainTarget = getAutoGainTargetRaw();
			
			whiteBalanceRed = p_WhiteBalanceRed = getWhiteBalanceRedRaw();
			whiteBalanceBlue = p_WhiteBalanceBlue = getWhiteBalanceBlueRaw();
			autoWhiteBalance = p_AutoWhiteBalance = getAutoWhiteBalance();
			autoWhiteBalanceRate = p_AutoWhiteBalanceRate = getAutoWhiteBalanceRateRaw();
			autoWhiteBalanceAdjustTol = p_AutoWhiteBalanceAdjustTol = getAutoWhiteBalanceAdjustTolRaw();
			
		}
		
//        persistentIp = p_PersistentIp = isIpPersistent();
	}
	
	void ConnectorWithInterface::setAllParametersFromInterface() {
//        persistentIp = p_PersistentIp = isIpPersistent();
		p_FrameRate.setMin(getFrameRateMin());
		p_FrameRate.setMax(getFrameRateMax());
		
        cout<<"getFrameRateMin() "<<getFrameRateMin()<<endl;
        cout<<"getFrameRateMax() "<<getFrameRateMax()<<endl;
        
		p_ROIWidth.setMin(getROIWidthRawMin());
		p_ROIWidth.setMax(getROIWidthRawMax());
		p_ROIHeight.setMin(getROIHeightRawMin());
		p_ROIHeight.setMax(getROIHeightRawMax());
		
		p_Exposure.setMin(getExposureRawMin());
	//	p_Exposure.setMax(getExposureRawMax());
		p_Exposure.setMax(getExposureRawMin() + 100000);
		p_AutoExposureAdjustTol.setMin(getAutoExposureAdjustTolRawMin());
		p_AutoExposureAdjustTol.setMax(getAutoExposureAdjustTolRawMax());
		p_AutoExposureMax.setMin(getAutoExposureMaxRawMin());
        p_AutoExposureMax.setMax(getAutoExposureMaxRawMin() + 200000); //getAutoExposureMaxRawMax());
		p_AutoExposureOutliers.setMin(getAutoExposureOutliersRawMin());
		p_AutoExposureOutliers.setMax(getAutoExposureOutliersRawMax());
		p_AutoExposureRate.setMin(getAutoExposureRateRawMin());
		p_AutoExposureRate.setMax(getAutoExposureRateRawMax());
		p_AutoExposureTarget.setMin(getAutoExposureTargetRawMin());
        p_AutoExposureTarget.setMax(getAutoExposureTargetRawMax());
		
        //cout<<"getAutoExposureMaxRawMax() "<<getAutoExposureMaxRawMax()<<endl;
        
		p_Gain.setMin(getGainRawMin());
		p_Gain.setMax(getGainRawMax());
		if (getPixelFormat() == OF_PIXELS_RGB) {
			p_Gamma.setMin(getGammaRawMin());
			p_Gamma.setMax(getGammaRawMax());
			p_Hue.setMin(getHueRawMin());
			p_Hue.setMax(getHueRawMax());
			p_Saturation.setMin(getSaturationRawMin());
			p_Saturation.setMax(getSaturationRawMax());
			
			p_AutoGainTarget.setMin(getAutoGainTargetRawMin());
			p_AutoGainTarget.setMax(getAutoGainTargetRawMax());
			p_AutoGainAdjustTol.setMin(getAutoGainAdjustTolRawMin());
			p_AutoGainAdjustTol.setMax(getAutoGainAdjustTolRawMax());
			p_AutoGainMin.setMin(getAutoGainMinRawMin());
			p_AutoGainMin.setMax(getAutoGainMinRawMax());
			p_AutoGainMax.setMin(getAutoGainMaxRawMin());
			p_AutoGainMax.setMax(getAutoGainMaxRawMax());
			p_AutoGainOutliers.setMin(getAutoGainOutliersRawMin());
			p_AutoGainOutliers.setMax(getAutoGainOutliersRawMax());
			p_AutoGainRate.setMin(getAutoGainRateRawMin());
			p_AutoGainRate.setMax(getAutoGainRateRawMax());
			
			p_WhiteBalanceRed.setMin(getWhiteBalanceRedRawMin());
			p_WhiteBalanceRed.setMax(getWhiteBalanceRedRawMax());
			p_WhiteBalanceBlue.setMin(getWhiteBalanceBlueRawMin());
			p_WhiteBalanceBlue.setMax(getWhiteBalanceBlueRawMax());
			p_AutoWhiteBalanceRate.setMin(getAutoWhiteBalanceRateRawMin());
			p_AutoWhiteBalanceRate.setMax(getAutoWhiteBalanceRateRawMax());
			p_AutoWhiteBalanceAdjustTol.setMin(getAutoWhiteBalanceAdjustTolRawMin());
			p_AutoWhiteBalanceAdjustTol.setMax(getAutoWhiteBalanceAdjustTolRawMax());
		}
        
		frameRate = p_FrameRate;
        setFrameRate(frameRate);
		
		ROIWidth = p_ROIWidth;
        setROIWidthRaw(ROIWidth);
		ROIHeight = p_ROIHeight;
        setROIHeightRaw(ROIHeight);
		ROIX = p_ROIX;
        setROIX(ROIX);
		ROIY = p_ROIY;
        setROIY(ROIY);
		
		exposure = p_Exposure;
        setExposureRaw(exposure);
		autoExposure = p_AutoExposure;
        setAutoExposure(autoExposure);
		autoExposureTarget = p_AutoExposureTarget;
        setAutoExposureTargetRaw(autoExposureTarget);
		autoExposureRate = p_AutoExposureRate;
        setAutoExposureRateRaw(autoExposureRate);
		autoExposureAdjustTol = p_AutoExposureAdjustTol;
        setAutoExposureAdjustTolRaw(autoExposureAdjustTol);
		autoExposureMax = p_AutoExposureMax;
        setAutoExposureMaxRaw(autoExposureMax);
		autoExposureOutliers = p_AutoExposureOutliers;
        setAutoExposureOutliersRaw(autoExposureOutliers);
		
		gain = p_Gain;
        setGainRaw(gain);
		if (getPixelFormat() == OF_PIXELS_RGB) {
			gamma = p_Gamma;
			setGammaRaw(gamma);
			hue = p_Hue;
			setHueRaw(hue);
			saturation = p_Hue;
			setSaturationRaw(saturation);
			
			autoGain = p_AutoGain;
			setAutoGain(autoGain);
			autoGainTarget = p_AutoGainTarget;
			setAutoGainTargetRaw(autoGainTarget);
			autoGainRate = p_AutoGainRate;
			setAutoGainRateRaw(autoGainRate);
			autoGainAdjustTol = p_AutoGainAdjustTol;
			setAutoGainAdjustTolRaw(autoGainAdjustTol);
			autoGainMax = p_AutoGainMax;
			setAutoGainMaxRaw(autoGainMax);
			autoGainOutliers = p_AutoGainOutliers;
			setAutoGainOutliersRaw(autoGainOutliers);
			
			whiteBalanceRed = p_WhiteBalanceRed;
			setWhiteBalanceRedRaw(whiteBalanceRed);
			whiteBalanceBlue = p_WhiteBalanceBlue;
			setWhiteBalanceBlueRaw(whiteBalanceBlue);
			autoWhiteBalance = p_AutoWhiteBalance;
			setAutoWhiteBalance(autoWhiteBalance);
			autoWhiteBalanceRate = p_AutoWhiteBalanceRate;
			setAutoWhiteBalanceRateRaw(autoWhiteBalanceRate);
			autoWhiteBalanceAdjustTol = p_AutoWhiteBalanceAdjustTol;
			setAutoWhiteBalanceAdjustTolRaw(autoWhiteBalanceAdjustTol);
		}
	}
	
	void ConnectorWithInterface::updateParameters() {
        if (exposure != p_Exposure) {
            exposure = p_Exposure;
            setExposureRaw(exposure); }
        
        exposure = p_Exposure = getExposureRaw();
        
        if (autoExposure != p_AutoExposure) {
            autoExposure = p_AutoExposure;
            setAutoExposure(autoExposure); }
        
        autoExposure = p_AutoExposure = getAutoExposure();
        
        if (p_AutoExposureOnce) {
            p_AutoExposureOnce = false;
            setAutoExposureOnce(); }        
        
        if (autoExposureTarget != p_AutoExposureTarget) {
            autoExposureTarget = p_AutoExposureTarget;
            setAutoExposureTargetRaw(autoExposureTarget); }
		
        if (autoExposureRate != p_AutoExposureRate) {
            autoExposureRate = p_AutoExposureRate;
            setAutoExposureRateRaw(autoExposureRate); }
        
        if (autoExposureAdjustTol != p_AutoExposureAdjustTol) {
            autoExposureAdjustTol = p_AutoExposureAdjustTol;
            setAutoExposureAdjustTolRaw(autoExposureAdjustTol); }
        
        if (autoExposureOutliers != p_AutoExposureOutliers) {
            autoExposureOutliers = p_AutoExposureOutliers;
            setAutoExposureOutliersRaw(autoExposureOutliers); }
        
        if (autoExposureMin != p_AutoExposureMin) {
            autoExposureMin = p_AutoExposureMin;
            setAutoExposureMinRaw(autoExposureMin); }
        
        if (autoExposureMax != p_AutoExposureMax) {
            autoExposureMax = p_AutoExposureMax;
            setAutoExposureMaxRaw(autoExposureMax); }
        
        if (p_AutoExposureMaxForFrameRate) {
            p_AutoExposureMaxForFrameRate = false;
            setAutoExposureMaxForCurrentFrameRate();
            autoExposureMax = p_AutoExposureMax = getAutoExposureMaxRaw(); }
		
        
        if (gain != p_Gain) {
            gain = p_Gain;
            setGainRaw(gain); }
		
		if (getPixelFormat() == OF_PIXELS_RGB) {
			
			gain = p_Gain = getGainRaw();
			
			if (autoGain != p_AutoGain) {
				autoGain = p_AutoGain;
				setAutoGain(autoGain); }
			
			autoGain = p_AutoGain = getAutoGain();
			
			if (p_AutoGainOnce) {
				p_AutoGainOnce = false;
				setAutoGainOnce(); }
			
			if (autoGainTarget != p_AutoGainTarget) {
				autoGainTarget = p_AutoGainTarget;
				setAutoGainTargetRaw(autoGainTarget); }
			
			if (autoGainRate != p_AutoGainRate) {
				autoGainRate = p_AutoGainRate;
				setAutoGainRateRaw(autoGainRate); }
			
			if (autoGainAdjustTol != p_AutoGainAdjustTol) {
				autoGainAdjustTol = p_AutoGainAdjustTol;
				setAutoGainAdjustTolRaw(autoGainAdjustTol); }
			
			if (autoGainOutliers != p_AutoGainOutliers) {
				autoGainOutliers = p_AutoGainOutliers;
				setAutoGainOutliersRaw(autoGainOutliers); }
			
			if (autoGainMin != p_AutoGainMin) {
				autoGainMin = p_AutoGainMin;
				setAutoGainMinRaw(autoGainMin); }
			
			if (autoGainMax != p_AutoGainMax) {
				autoGainMax = p_AutoGainMax;
				setAutoGainMaxRaw(autoGainMax); }
			
			if (gamma != p_Gamma) {
				gamma = p_Gamma;
				setGammaRaw(gamma);
			}
			
			if (hue != p_Hue) {
				hue = p_Hue;
				setHueRaw(hue);
			}
			
			if (saturation != p_Saturation) {
				saturation = p_Saturation;
				setSaturationRaw(saturation);
			}
			
			
			if (whiteBalanceRed != p_WhiteBalanceRed) {
				whiteBalanceRed = p_WhiteBalanceRed;
				setWhiteBalanceRedRaw(whiteBalanceRed); }
			
			whiteBalanceRed = p_WhiteBalanceRed = getWhiteBalanceRedRaw();
			
			if (whiteBalanceBlue != p_WhiteBalanceBlue) {
				whiteBalanceBlue = p_WhiteBalanceBlue;
				setWhiteBalanceBlueRaw(whiteBalanceBlue); }
			
			whiteBalanceBlue = p_WhiteBalanceBlue = getWhiteBalanceBlueRaw();
			
			if (autoWhiteBalance != p_AutoWhiteBalance) {
				autoWhiteBalance = p_AutoWhiteBalance;
				setAutoWhiteBalance(autoWhiteBalance); }
			
			autoWhiteBalance = p_AutoWhiteBalance = getAutoWhiteBalance();
			
			if (p_AutoWhiteBalanceOnce) {
				p_AutoWhiteBalanceOnce = false;
				setAutoWhiteBalanceOnce(); }
			
			if (autoWhiteBalanceRate !=  p_AutoWhiteBalanceRate) {
				autoWhiteBalanceRate = p_AutoWhiteBalanceRate;
				setAutoWhiteBalanceRateRaw(autoWhiteBalanceRate); }
			
			if (autoWhiteBalanceAdjustTol != p_AutoWhiteBalanceAdjustTol) {
				autoWhiteBalanceAdjustTol = p_AutoWhiteBalanceAdjustTol;
				setAutoWhiteBalanceAdjustTolRaw(autoWhiteBalanceAdjustTol); }
			
		}
		
		
        
        if (p_PrintAttributes.get()) {
            p_PrintAttributes.set(false);
            listAttributes(); }
        
        if (p_PrintIpSettings.get()) {
            p_PrintIpSettings.set(false);
            listIpSettings(); }
        
 /*       if (persistentIp != p_PersistentIp.get()) {
            persistentIp = p_PersistentIp.get();
            setPersistentIp(persistentIp);
        //    if (persistentIp)setPersistentIp(persistentIp);
        }
*/        
        if (frameRate != p_FrameRate) {
            frameRate = p_FrameRate;
            setFrameRate(frameRate);
          //  frameRateMin  = p_FrameRateMin;
          //   frameRateMax  = p_FrameRateMax;
        }
        
        if (ROIWidth!= p_ROIWidth) {
            ROIWidth = p_ROIWidth;
            setROIWidthRaw(ROIWidth); }
        
        if (ROIHeight != p_ROIHeight) {
            ROIHeight = p_ROIHeight;
            setROIHeightRaw(ROIHeight); }
        
        if (ROIX != p_ROIX) {
            ROIX = p_ROIX;
            setROIX(ROIX); }
        
        if (ROIY != p_ROIY) {
            ROIY = p_ROIY;
            setROIY(ROIY); }
        
        if (p_Reset.get()) {
            p_Reset.set(false);
            resetAttributes();
            setAllParametersFromCam(); }
    }
}
