#pragma once

#include "ofMain.h"
#include "Connector.h"


namespace ofxProsilica {
	
	class ThreadedConnector : public ofThread {
	public:
		
		ThreadedConnector();
		virtual~ThreadedConnector();
		
        bool setup();
		bool			initGrabber();
		void			update();
		void			threadedFunction();
		
		bool 			isFrameNew();
		
		unsigned char*  getPixels();
		ofPixelsRef     getPixelsRef();
		
		vector<ofVideoDevice> listDevices();
		
		void			close();
		
		void			setDeviceID(int _deviceID);
		int 			getDeviceID();
		
		
		bool            setPixelFormat(ofPixelFormat _pixelFormat);
		ofPixelFormat   getPixelFormat();
		
		
		
		bool			isInitialized();
		
		void            videoSettings();
		void			printAttributes();
		void			resetAttributes();
		
		// absolute values
		float			getWidth();
		float			getHeight();
		
		
		void            setDesiredFrameRate(int  _framerate) {setFrameRate(_framerate);}
		void            setFrameRate(float rate);
		float           getFrameRate();
		float			getMaxFrameRate();
        
        ofTexture&      getTextureReference() { return texture; }
        bool            showMe() { return p_ShowMe.get(); }
        
		ofParameterGroup parameters;
		
	protected:
		
		ofPixels		Tpixels;
		int				Twidth;
		int				Theight;
		bool 			TbIsFrameNew;
		int				TdeviceID;
		ofPixelFormat	TpixelFormat;
		bool			TbInitialized;
		
		
	private:
		Connector	Tgrabber;
        ofTexture   texture;
		
		int		frameRate;
		float	ROIWidth;
		float	ROIHeight;
		float	ROIX;
		float	ROIY;
		float	exposure;
		bool	autoExposure;
		bool	autoExposureOnce;
		float	autoExposureAdjustTol;
		float	autoExposureMax;
		float	autoExposureOutliers;
		float	autoExposureRate;
		float	autoExposureTarget;
		float	gain;
		
		ofParameter<int>	p_FrameRate;
		ofParameter<float>	p_ROIWidth;
		ofParameter<float>	p_ROIHeight;
		ofParameter<float>	p_ROIX;
		ofParameter<float>	p_ROIY;
		ofParameter<float>	p_Exposure;
		ofParameter<bool>	p_AutoExposure;
		ofParameter<bool>	p_AutoExposureOnce;
		ofParameter<float>	p_AutoExposureAdjustTol;
		ofParameter<float>	p_AutoExposureMax;
		ofParameter<bool>	p_AutoExposureMaxForFrameRate;
		ofParameter<float>	p_AutoExposureOutliers;
		ofParameter<float>	p_AutoExposureRate;
		ofParameter<float>	p_AutoExposureTarget;
		ofParameter<float>	p_Gain;
		ofParameter<bool>	p_ShowMe;
		ofParameter<bool>	p_Reset;
		ofParameter<bool>	p_PrintAttributes;
		ofParameter<bool>	p_FlipH;
		ofParameter<bool>	p_FlipV;
		
		void	setupParameters();
		void	updateParameters();
		void	setAllParametersFromCam();
		void	setAllParametersFromInterface();
		
	};
	
}