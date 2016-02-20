#pragma once

#include "ofMain.h"
#include "ofxProsilica.h"



namespace ofxProsilica {
	
	class ConnectorWithInterface : public Connector {
	public:
		
		bool	setup();
		void	update();
		
        bool    showMe() { return p_ShowMe.get(); }
		
		ofParameterGroup parameters;
		
	private:
        
		int		frameRate;
      //  int frameRateMin, frameRateMax;
        
		int		ROIWidth;
		int		ROIHeight;
		float	ROIX;
		float	ROIY;
		float	exposure;
		bool	autoExposure;
		bool	autoExposureOnce;
		int		autoExposureTarget;
		int		autoExposureRate;
		int		autoExposureAdjustTol;
		int		autoExposureOutliers;
		int		autoExposureMin;
		int		autoExposureMax;
		
		float	gain;
		bool	autoGain;
		bool	autoGainOnce;
		int		autoGainTarget;
		int		autoGainRate;
		int		autoGainAdjustTol;
		int		autoGainOutliers;
		int		autoGainMin;
		int		autoGainMax;
		
		int		whiteBalanceRed;
		int		whiteBalanceBlue;
		bool	autoWhiteBalance;
		bool	autoWhiteBalanceOnce;
		int		autoWhiteBalanceRate;
		int		autoWhiteBalanceAdjustTol;
		
		float	gamma;
		float	hue;
		float	saturation;
		
        ofParameter<int>    dID;
		ofParameter<bool>	p_ShowMe;
		ofParameter<bool>	p_Reset;
		ofParameter<bool>	p_PrintAttributes;
		ofParameter<bool>	p_PrintIpSettings;
//		ofParameter<bool>	p_PersistentIp;
//      bool				persistentIp;
		
		ofParameter<int>	p_FrameRate;
//        ofParameter<int>	p_FrameRateMin;
//        ofParameter<int>	p_FrameRateMax;
        
		ofParameter<int>	p_ROIWidth;
		ofParameter<int>	p_ROIHeight;
		ofParameter<float>	p_ROIX;
		ofParameter<float>	p_ROIY;
		
		ofParameterGroup	exposureParameters;
		ofParameter<int>	p_Exposure;
		ofParameter<bool>	p_AutoExposure;
		ofParameter<bool>	p_AutoExposureOnce;
		ofParameter<int>	p_AutoExposureTarget;
		ofParameter<int>	p_AutoExposureRate;
		ofParameter<int>	p_AutoExposureAdjustTol;
		ofParameter<bool>	p_AutoExposureMaxForFrameRate;
		ofParameter<int>	p_AutoExposureOutliers;
		ofParameter<int>	p_AutoExposureMin;
		ofParameter<int>	p_AutoExposureMax;
		
		ofParameterGroup	gainParameters;
		ofParameter<int>	p_Gain;
		ofParameter<bool>	p_AutoGain;
		ofParameter<bool>	p_AutoGainOnce;
		ofParameter<int>	p_AutoGainTarget;
		ofParameter<int>	p_AutoGainRate;
		ofParameter<int>	p_AutoGainAdjustTol;
		ofParameter<int>	p_AutoGainOutliers;
		ofParameter<int>	p_AutoGainMin;
		ofParameter<int>	p_AutoGainMax;
		
		ofParameterGroup	whiteBalanceParameters;
		ofParameter<int>	p_WhiteBalanceRed;
		ofParameter<int>	p_WhiteBalanceBlue;
		ofParameter<bool>	p_AutoWhiteBalance;
		ofParameter<bool>	p_AutoWhiteBalanceOnce;
		ofParameter<int>	p_AutoWhiteBalanceRate;
		ofParameter<int>	p_AutoWhiteBalanceAdjustTol;
		
		ofParameter<float>	p_Gamma;
		ofParameter<float>	p_Hue;
		ofParameter<float>	p_Saturation;
		
		bool	initInterface();
		void	setupParameters();
		void	updateParameters();
		void	setAllParametersFromCam();
		void	setAllParametersFromInterface();
        
        bool    bLoadFromInterface;
		
	};
}