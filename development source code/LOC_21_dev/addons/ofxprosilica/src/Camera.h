#pragma once

#include "ofMain.h"
#include "PvApi.h"
#include "PvRegIo.h"
#include "ImageLib.h"

#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

namespace ofxProsilica {
	
	class Camera {//:  public ofBaseVideoGrabber  {
		public :
		
		Camera();
		virtual ~Camera();
		
		
		bool			setup();
		void			update();
		bool			isInitialized();
		bool 			isFrameNew();
		void			close();
		
		vector<ofVideoDevice> listDevices();
		void			setDeviceID(int _deviceID) { requestDeviceID(_deviceID); }
		void			requestDeviceID(int _deviceID);
		int 			getDeviceID();
		int             getDeviceIDFromIpAdress(string _IpAdress);
		
		unsigned char*  getPixels();
		ofPixelsRef     getPixelsRef();
		bool            setPixelFormat(ofPixelFormat _pixelFormat);
		ofPixelFormat   getPixelFormat();
				
		void            videoSettings() { listAttributes();}
		void			listAttributes();
		void			resetAttributes();
		
        //-- WIDTH & HEIGHT (absolute values) --------------------------------
		float			getWidth();
		float			getHeight();
		
        //-- FRAMERATE (absolute values) -------------------------------------
		void            setDesiredFrameRate(int  _framerate) {setFrameRate(_framerate);}
		void            setFrameRate(float rate);
		float           getFrameRate();
		float			getFrameRateMax();
		float			getFrameRateMin();
		
        //-- REGION OF INTEREST (normalized values) --------------------------
		void 			setROIWidth(float w);
		float 			getROIWidth();
		void 			setROIHeight(float h);
		float 			getROIHeight();
		void            setROIX(float x);
		float           getROIX();
		void            setROIY(float y);
		float           getROIY();
		
        //-- REGION OF INTEREST (raw values) --------------------------
		void 			setROIWidthRaw(int w);
		int 			getROIWidthRaw();
		void 			setROIHeightRaw(int h);
		int 			getROIHeightRaw();
		void            setROIXRaw(int x);
		int				getROIXRaw();
		void            setROIYRaw(int y);
		int				getROIYRaw();
		
		int 			getROIWidthRawMin();
		int 			getROIWidthRawMax();
		int 			getROIHeightRawMin();
		int 			getROIHeightRawMax();
		int				getROIXRawMin();
		int				getROIXRawMax();
		int				getROIYRawMin();
		int				getROIYRawMax();
		
        //-- EXPOSURE ------------------------------------		
		void            setAutoExposure(bool state);
		bool            getAutoExposure();
		void            setAutoExposureOnce();
		void            setAutoExposureMaxForCurrentFrameRate();
		
        //-- EXPOSURE (normalized values) ------------------------------------
		void            setExposure(float _value);
		float           getExposure();
		void            setAutoExposureTarget(float _value);
		float           getAutoExposureTarget();
		void            setAutoExposureAdjustTol(float _value);
		float           getAutoExposureAdjustTol();
		void            setAutoExposureMax(float _value);
		float			getAutoExposureMax();
		void            setAutoExposureMin(float _value);
		float			getAutoExposureMin();
		void            setAutoExposureOutliers(float _value);
		float           getAutoExposureOutliers();
		void            setAutoExposureRate(float _value);
		float           getAutoExposureRate();
		
        //-- EXPOSURE (raw values) ------------------------------------------------------------        
		void            setExposureRaw(int _value);
		int				getExposureRaw();
		void            setAutoExposureTargetRaw(int _value);
		int				getAutoExposureTargetRaw();
		void            setAutoExposureAdjustTolRaw(int _value);
		int				getAutoExposureAdjustTolRaw();
		void            setAutoExposureMaxRaw(int _value);
		int				getAutoExposureMaxRaw();
		void            setAutoExposureMinRaw(int _value);
		int				getAutoExposureMinRaw();
		void            setAutoExposureOutliersRaw(int _value);
		int				getAutoExposureOutliersRaw();
		void            setAutoExposureRateRaw(int _value);
		int				getAutoExposureRateRaw();
		
		int				getExposureRawMin();
		int				getExposureRawMax();
		int				getAutoExposureTargetRawMin();
		int				getAutoExposureTargetRawMax();
		int				getAutoExposureAdjustTolRawMin();
		int				getAutoExposureAdjustTolRawMax();
		int				getAutoExposureMaxRawMin();
		int				getAutoExposureMaxRawMax();
		int				getAutoExposureMinRawMin();
		int				getAutoExposureMinRawMax();
		int				getAutoExposureOutliersRawMin();
		int				getAutoExposureOutliersRawMax();
		int				getAutoExposureRateRawMin();
		int				getAutoExposureRateRawMax();
		
        //-- GAIN ------------------------------------		
		void            setAutoGain(bool state);
		bool            getAutoGain();
		void            setAutoGainOnce();
		
        //-- GAIN (normalized values) ------------------------------------
		void            setGain(float _value);
		float           getGain();
		void            setAutoGainTarget(float _value);
		float           getAutoGainTarget();
		void            setAutoGainAdjustTol(float _value);
		float           getAutoGainAdjustTol();
		void            setAutoGainMax(float _value);
		float			getAutoGainMax();
		void            setAutoGainMin(float _value);
		float			getAutoGainMin();
		void            setAutoGainOutliers(float _value);
		float           getAutoGainOutliers();
		void            setAutoGainRate(float _value);
		float           getAutoGainRate();
		
        //-- GAIN (raw values) ------------------------------------------------------------        
		void            setGainRaw(int _value);
		int				getGainRaw();
		void            setAutoGainTargetRaw(int _value);
		int				getAutoGainTargetRaw();
		void            setAutoGainAdjustTolRaw(int _value);
		int				getAutoGainAdjustTolRaw();
		void            setAutoGainMaxRaw(int _value);
		int				getAutoGainMaxRaw();
		void            setAutoGainMinRaw(int _value);
		int				getAutoGainMinRaw();
		void            setAutoGainOutliersRaw(int _value);
		int				getAutoGainOutliersRaw();
		void            setAutoGainRateRaw(int _value);
		int				getAutoGainRateRaw();
		
		int				getGainRawMin();
		int				getGainRawMax();
		int				getAutoGainTargetRawMin();
		int				getAutoGainTargetRawMax();
		int				getAutoGainAdjustTolRawMin();
		int				getAutoGainAdjustTolRawMax();
		int				getAutoGainMinRawMin();
		int				getAutoGainMinRawMax();
		int				getAutoGainMaxRawMin();
		int				getAutoGainMaxRawMax();
		int				getAutoGainOutliersRawMin();
		int				getAutoGainOutliersRawMax();
		int				getAutoGainRateRawMin();
		int				getAutoGainRateRawMax();
		
        //-- GAMMA HUE STURATION (normalized values) ------------------------------------
		void            setGammaRaw(float _value);
		float           getGammaRaw();
		void            setHueRaw(float _value);
		float           getHueRaw();
		void            setSaturationRaw(float _value);
		float           getSaturationRaw();
		
		int				getHueRawMin();
		int				getHueRawMax();
		int				getGammaRawMin();
		int				getGammaRawMax();
		int				getSaturationRawMin();
		int				getSaturationRawMax();
		
        //-- WHITE BALANCE ------------------------------------		
		void            setAutoWhiteBalance(bool state);
		bool            getAutoWhiteBalance();
		void            setAutoWhiteBalanceOnce();
		
        //-- WHITE BALANCE (normalized values) ------------------------------------
		void            setWhiteBalanceBlue(float _value);
		float           getWhiteBalanceBlue();
		void            setWhiteBalanceRed(float _value);
		float           getWhiteBalanceRed();
		void            setAutoWhiteBalanceAdjustTol(float _value);
		float           getAutoWhiteBalanceAdjustTol();
		void            setAutoWhiteBalanceRate(float _value);
		float           getAutoWhiteBalanceRate();		
		
        //-- WHITE BALANCE (raw values) ------------------------------------
		void            setWhiteBalanceBlueRaw(int _value);
		int				getWhiteBalanceBlueRaw();
		void            setWhiteBalanceRedRaw(int _value);
		int				getWhiteBalanceRedRaw();
		void            setAutoWhiteBalanceAdjustTolRaw(int _value);
		int				getAutoWhiteBalanceAdjustTolRaw();
		void            setAutoWhiteBalanceRateRaw(int _value);
		int				getAutoWhiteBalanceRateRaw();
		
		int				getWhiteBalanceBlueRawMin();
		int				getWhiteBalanceBlueRawMax();
		int				getWhiteBalanceRedRawMin();
		int				getWhiteBalanceRedRawMax();
		int				getAutoWhiteBalanceAdjustTolRawMin();
		int				getAutoWhiteBalanceAdjustTolRawMax();
		int				getAutoWhiteBalanceRateRawMin();
		int				getAutoWhiteBalanceRateRawMax();
		
        
        //-- IP SETTINGS -----------------------------------------------------
        void            setPersistentIp(bool enable);
        void            setPersistentIpAdress(string _IpAdress);
        void            setPersistentIpSubnetMask(string _IpSubnet);
        void            setPersistentIpGateway(string _IpGateway);
        string          getIpAdress();
        string          getIpSubnet();
        string          getIpGateway();
        bool            isIpPersistent();
        void            listIpSettings();
		
		
	protected:
		void			PvApiInitialize();
		void			PvApiUnInitialize();
		static bool     bPvApiInitiated;
		static int		numCamerasInUse;
		tPvHandle       cameraHandle;
		tPvFrame        cameraFrame;
		
		bool 			bInitialized;
		bool 			bIsFrameNew;
		bool            bWaitingForFrame;
        
        //-- ACQUISITION -----------------------------------------------------
		bool			initCamera(int cameraUid);
		bool			openCamera();
		bool			closeCamera();
		bool			startCapture();
		bool			stopCapture();
		bool			startAcquisition();
		bool			stopAcquisition();
		bool			abortAcquisition();
		void			queueFrame();
		bool			clearQueue();
        
        //-- DEVICES ---------------------------------------------------------
		unsigned long 	deviceID;
		unsigned long 	requestedDeviceID;
    
        //-- PIXELS ----------------------------------------------------------
		ofPixelFormat	internalPixelFormat;
		ofPixels		pixels;
		bool			allocatePixels();
		
        //-- REGION OF INTEREST-----------------------------------------------
		float			regionX;
		float			regionY;
		
        //-- ATTRIBUTES-------------------------------------------------------
		void			setNormalizedAttribute(string _name, float _value);
		float			getNormalizedAttribute(string _name);
		
		bool			setIntAttribute(string _name, int _value);
		int				getIntAttribute(string _name);
		int				getIntAttributeMax(string _name);
		int				getIntAttributeMin(string _name);
		
		bool			setFloatAttribute(string _name, float _value);
		float			getFloatAttribute(string _name);
		float			getFloatAttributeMax(string _name);
		float			getFloatAttributeMin(string _name);
		
		bool			setEnumAttribute(string _name, string _value);
		string			getEnumAttribute(string _name);
		
        //-- IP SETTINGS -----------------------------------------------------
        string          persistentIpAdress;
        string          persistentIpSubnetMask;
        string          persistentIpGateway;
        unsigned long   IPStringToLong(string _IpAdress);
        string          IPLongToString(unsigned long  _IpAdress);
        
		bool			setPacketSizeToMax();
		
		void			logError(tPvErr _msg);
		
	};
	
}
