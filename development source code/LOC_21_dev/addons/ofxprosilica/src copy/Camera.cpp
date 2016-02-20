#include "Camera.h"

namespace ofxProsilica {
	
	bool Camera::bPvApiInitiated = false;
    int Camera::numCamerasInUse = 0;
	
    
	//--------------------------------------------------------------------
    
	Camera::Camera() :
	bInitialized(false),
	bIsFrameNew(false),
	bWaitingForFrame(false),
	internalPixelFormat(OF_PIXELS_MONO),
	deviceID(0),
	requestedDeviceID(-1),
//	bDeviceIDRequested(false),
	regionX(0.5),
	regionY(0.5),
	persistentIpAdress(""),
    persistentIpGateway(""),
    persistentIpSubnetMask("0.0.0.0") {
        
		if (!bPvApiInitiated) PvApiInitialize() ;
	}
	
	Camera::~Camera(){
		close();
		if (numCamerasInUse == 0 && bPvApiInitiated)
			PvApiUnInitialize();
	}
	
	void Camera::PvApiInitialize() {
		
		if( bPvApiInitiated ){
            ofLog(OF_LOG_VERBOSE, "PvAPI already initialized");
		}
		else {
			tPvErr error = PvInitialize();
			if( error == ePvErrSuccess ) {
				ofSleepMillis(500); // Need some time to actually be initialized;
				ofLog(OF_LOG_NOTICE, "Camera: PvAPI initialized");
				
			} else {
				ofLog(OF_LOG_ERROR, "Camera: unable to initialize PvAPI");
				logError(error);
				return;
			}
			bPvApiInitiated = true;
		}
		
	}
	
	void Camera::PvApiUnInitialize() {
		if (numCamerasInUse == 0 && bPvApiInitiated) {
			PvUnInitialize();
			bPvApiInitiated = false;
			ofLog(OF_LOG_NOTICE, "Camera: PvAPI uninitialized");
		}
	}
	
	
	//--------------------------------------------------------------------
	
	bool Camera::setup() {
		vector<ofVideoDevice> deviceList = listDevices();
        if (deviceList.size() == 0) {
			ofLog(OF_LOG_WARNING, "Camera: no cameras found", deviceID);
			return false;
		}
        
		if (requestedDeviceID == -1) {
			bool foundAvailableCamera = false;
            for (int i=0; i<deviceList.size(); i++) {
                if (deviceList[i].bAvailable) {
                    requestedDeviceID = deviceList[i].id;
                    foundAvailableCamera = true;
                    break;
                }
            }
            if (foundAvailableCamera){
                ofLog(OF_LOG_NOTICE, "Camera: no camera ID specified, defaulting to camera %i", (int)requestedDeviceID);
            }
            else {
                ofLog(OF_LOG_WARNING, "Camera: found no camera available ");
                return false;
            }
        }
		
		bool requestedDeviceFound = false;
		bool requestedDeviceAvailable = false;
		for (int i=0; i<deviceList.size(); i++) {
			if (requestedDeviceID == deviceList[i].id) {
				requestedDeviceFound = true;
				if (deviceList[i].bAvailable) {
					requestedDeviceAvailable = true;
					ofLog(OF_LOG_VERBOSE, "Camera: %i found", (int)requestedDeviceID);
				}
			}
		}
		if (!requestedDeviceFound) {
			ofLog(OF_LOG_WARNING, "Camera: %i not found", (int)requestedDeviceID);
			return false;
		}
		if (!requestedDeviceAvailable) {
			ofLog(OF_LOG_WARNING, "Camera: %i not available", (int)requestedDeviceID);
			return false;
		}
		
		return initCamera(requestedDeviceID);
	}
	
	bool Camera::initCamera(int _cameraUid){
		
		deviceID = _cameraUid;
		
        if (!openCamera()) return false;
		if (!setPacketSizeToMax()) return false;
		if (!startCapture()) return false;
		if (!setEnumAttribute("FrameStartTriggerMode","FixedRate")) return false;
		if (!setEnumAttribute( "AcquisitionMode", "Continuous")) return false;
		
		string pf = "Mono8";
		if (internalPixelFormat == OF_PIXELS_RGB) pf = "Rgb24";
		if (!setEnumAttribute("PixelFormat", pf)) return false;
		
		if (!startAcquisition()) return false;
		if (!allocatePixels()) return false;
		
		bInitialized = true;
		numCamerasInUse++;
		ofLog(OF_LOG_NOTICE,"Camera: %i up and running", deviceID);
		return true;
	}
	
    void Camera::update(){
    
        bIsFrameNew = false;
        if (bInitialized) {
            if( !bWaitingForFrame ) {
                queueFrame();
            }
            else {
                
                tPvErr error = PvCaptureWaitForFrameDone(cameraHandle, &cameraFrame, 1); // in MiliSeconds
                if( error == ePvErrTimeout ) {
                    bIsFrameNew = false;
                } else if( error == ePvErrSuccess ){
                    bIsFrameNew = true;
                    bWaitingForFrame = false;
                    queueFrame();
                } else if (error == ePvErrUnplugged) {
                    
                    ofLogWarning("Camera " + ofToString(deviceID) + " connection lost");
                    close();
                } else {
                    logError(error);
                    close();
                }
                
            }
        }
    }
    
	bool Camera::isInitialized() {
         return bInitialized;
    }
    
	bool Camera::isFrameNew(){
		return bIsFrameNew;
	}
	
	void Camera::close(){
		
		if( bInitialized ) {
			// stop the streaming
			clearQueue();
			//stopAcquisition();
			stopCapture();
			closeCamera();
			
			pixels.clear();
			
			bInitialized = false;
			numCamerasInUse--;
			
			ofLog(OF_LOG_NOTICE, "Camera: %i closed", deviceID);
		}
	}
	
	
	//--------------------------------------------------------------------
	//-- ACQUISITION -----------------------------------------------------
	
	bool Camera::openCamera() {
		tPvErr error = PvCameraOpen( deviceID, ePvAccessMaster, &cameraHandle );
		if( error == ePvErrSuccess ){
			ofLog(OF_LOG_VERBOSE, "Camera: %i opened", deviceID);
			return true;
		} else {
			ofLog(OF_LOG_ERROR, "Camera: %i failed to open", deviceID);
			logError(error);
			return false;
		}
	}
    
	bool Camera::closeCamera() {
		tPvErr error = PvCameraClose(cameraHandle);
		if( error == ePvErrSuccess ){
			ofLog(OF_LOG_VERBOSE, "Camera: %i camera closed", deviceID);
			bInitialized = false;
			return true;
		} else {
			ofLog(OF_LOG_ERROR, "Camera: %i failed to close", deviceID);
			logError(error);
			return false;
		}
	}
	
	bool Camera::startCapture() {
		
		tPvErr error = PvCaptureStart(cameraHandle);
		if( error == ePvErrSuccess ){
			ofLog(OF_LOG_VERBOSE, "Camera: %i set to capture mode", deviceID);
			return true;
		} else {
			logError(error);
			ofLog(OF_LOG_ERROR, "Camera: %i not set to capture mode", deviceID);
			return false;
		}
	}
	
	bool Camera::stopCapture() {
		tPvErr error = PvCaptureEnd(cameraHandle);
		if( error == ePvErrSuccess ){
			ofLog(OF_LOG_VERBOSE, "Camera: %i stopped capture mode", deviceID);
			return true;
		} else {
			ofLog(OF_LOG_ERROR, "Camera: %i stopped capture mode", deviceID);
			logError(error);
			return false;
		}
	}
	
	bool Camera::startAcquisition() {
		tPvErr error = PvCommandRun(cameraHandle,"AcquisitionStart");
		if( error == ePvErrSuccess ){
			ofLog(OF_LOG_VERBOSE, "Camera: %i continuous acquisition started", deviceID);
			return true;
		} else {
			ofLog(OF_LOG_ERROR, "Camera: %i can not start continuous acquisition", deviceID);
			logError(error);
			return false;
		}
	}
	
	bool Camera::stopAcquisition() {
		
		tPvErr error = PvCommandRun(cameraHandle,"AcquisitionStop");
		if( error == ePvErrSuccess ){
			ofLog(OF_LOG_VERBOSE, "Camera: %i continuous acquisition stopped", deviceID);
			return true;
		} else {
			// WHYWHYWHYWHYWHYWHY
			PvCaptureEnd(cameraHandle) ;
			ofLog(OF_LOG_ERROR, "Camera: %i can not stop continuous acquisition", deviceID);
			logError(error);
			return false;
		}
	}
	
	bool Camera::abortAcquisition() {
		
		tPvErr error = PvCommandRun(cameraHandle,"AcquisitionAbort");
		if( error == ePvErrSuccess ){
			ofLog(OF_LOG_VERBOSE, "Camera: %i continuous acquisition aborted", deviceID);
			return true;
		} else {
			// WHYWHYWHYWHYWHYWHY
			PvCaptureEnd(cameraHandle) ;
			ofLog(OF_LOG_ERROR, "Camera: %i can not abort continuous acquisition", deviceID);
			logError(error);
			return false;
		}
	}
	
	void Camera::queueFrame(){
		if( PvCaptureQueueFrame( cameraHandle, &cameraFrame, NULL) != ePvErrSuccess ){
			bWaitingForFrame = false;
			ofLog(OF_LOG_NOTICE, "Camera " + ofToString(deviceID) + " failed to queue frame buffer -> no worries just try again");
		} else {
			bWaitingForFrame = true;
		}
	}
	
	bool Camera::clearQueue() {
		tPvErr error = PvCaptureQueueClear(cameraHandle);
		if (error != ePvErrSuccess ){
			logError(error);
			return false;
		}
		else false;
	}
	
    
	//--------------------------------------------------------------------
	//-- DEVICES ---------------------------------------------------------
	
	vector<ofVideoDevice> Camera::listDevices(){
		
		vector <ofVideoDevice> devices;
		
		tPvUint32 count, connected;
		unsigned long cameraCount = PvCameraCount();;
		tPvCameraInfoEx* devList = new tPvCameraInfoEx[cameraCount];
        
        
		count = PvCameraListEx( devList, cameraCount, &connected, sizeof(tPvCameraInfoEx) );
		
		if( connected > cameraCount ) {
			ofLog(OF_LOG_VERBOSE, "Camera: more cameras connected than will be listed");
		}
		
		//ofLog(OF_LOG_VERBOSE, "Camera: listing available capture devices:");
        printf("Camera: listing available capture devices:\n");
		for(int i = 0; i < count; i++){
			ofVideoDevice vd;
			vd.id = devList[i].UniqueId;
			vd.deviceName = devList[i].CameraName;
			vd.hardwareName = devList[i].ModelName;
			vd.bAvailable = devList[i].PermittedAccess > 2;
			devices.push_back(vd);
		
            printf("%i: %s | model: %s | id: %i | available: %i\n", i, vd.deviceName.c_str(), vd.hardwareName.c_str(), vd.id, vd.bAvailable);
		//	ofLog(OF_LOG_VERBOSE, "%i: %s | model: %s | id: %i | available: %i", i, vd.deviceName.c_str(), vd.hardwareName.c_str(), vd.id, vd.bAvailable);
		}
		
		if (cameraCount == 0)
			//ofLog(OF_LOG_VERBOSE, "Camera: no cameras found");
        printf("Camera: no cameras found\n");
		
		delete devList;
		return devices;
	}
    
	int Camera::getDeviceIDFromIpAdress(string _IPAdress) {
        deviceID = 0;
        
        tPvCameraInfoEx pInfo;
        tPvIpSettings   pIpSettings;
        
        tPvErr error = PvCameraInfoByAddrEx(IPStringToLong(_IPAdress),
                                            &pInfo,
                                            &pIpSettings,
                                            sizeof(pInfo));
        
        if (error == ePvErrSuccess) {
            deviceID = pInfo.UniqueId;
            ofLog(OF_LOG_NOTICE, "Camera: found camera %i on IP adress %s", deviceID, _IPAdress.c_str());
        }
        else {
            ofLog(OF_LOG_WARNING, "Camera: no camera found on IP adress %s", _IPAdress.c_str());
            logError(error);
        }
        return deviceID;
	}
	
	void Camera::requestDeviceID(int _deviceID) {
		if (bInitialized)
			ofLog(OF_LOG_ERROR, "Camera: %i: setDeviceID(): can't set ID while grabber is running", deviceID);
		else {
			requestedDeviceID = _deviceID;
		}
	}
	
	int Camera::getDeviceID() {
		return deviceID;
	}
	
	
	//--------------------------------------------------------------------
	//-- PIXELS ----------------------------------------------------------
	
	bool Camera::allocatePixels() {
		
		clearQueue();
		
		pixels.clear();
		
        
		//   delete[] pixels;
		unsigned long frameSize = 0;
		tPvErr error = PvAttrUint32Get( cameraHandle, "TotalBytesPerFrame", &frameSize );
		
		int width = getIntAttribute("Width");
		int height = getIntAttribute("Height");
		
        cout<<"allocatePixels "<<endl;
        cout<<"width "<<width<<", height "<<height<<endl;
        cout<<"internalPixelFormat "<<internalPixelFormat<<endl;
        
		pixels.allocate(width, height, internalPixelFormat);
		
		if( error == ePvErrSuccess ){
			
			
			//	pixels = new unsigned char[frameSize];
			cameraFrame.ImageBuffer = pixels.getPixels();
			cameraFrame.ImageBufferSize = frameSize;
		} else {
			ofLog(OF_LOG_ERROR, "Camera: %i failed to allocate capture buffer", deviceID);
			return false;
		}
		
		//	allocateTexture();
		
		return true;
		
	}
    
	ofPixelsRef Camera::getPixelsRef(){
		return pixels;
	}
	
	unsigned char * Camera::getPixels(){
		return pixels.getPixels();
	}
    
	bool Camera::setPixelFormat(ofPixelFormat _pixelFormat) {
		if (bInitialized)
			ofLog(OF_LOG_ERROR, "Camera: %i: setPixelFormat(): can't set pixel format while grabber is running", deviceID);
		else {
			if (_pixelFormat == OF_PIXELS_MONO || _pixelFormat == OF_PIXELS_RGB) {
				internalPixelFormat = _pixelFormat;
				return true;
			}
			else {
				ofLogWarning("Camera") << "setPixelFormat(): requested pixel format " << _pixelFormat << " not supported";
				return false;
			}
		}
		return false;
	}
	
	ofPixelFormat Camera::getPixelFormat() {
		return internalPixelFormat;
	}
    
	
	//--------------------------------------------------------------------
	//-- ATTRIBUTES (GENERAL) --------------------------------------------
	
	void Camera::listAttributes(){
		tPvAttrListPtr    listPtr;
		unsigned long     listLength;
		if (PvAttrList(cameraHandle, &listPtr, &listLength) == ePvErrSuccess) {
			for (int i = 0; i < listLength; i++)
			{
				const char* attributeName = listPtr[i];
				printf("Attribute: %s", attributeName);
				tPvAttributeInfo pInfo;
				PvAttrInfo(cameraHandle, attributeName, &pInfo);
				tPvDatatype pDatatype = pInfo.Datatype;
				
				
				switch (pDatatype) {
					case 0 :
						printf(", Datatype: Unknown \n");
						break;
					case 1 :
						printf(", Datatype: Command \n");
						break;
					case 2 :
						printf(", Datatype: Raw \n");
						break;
					case 3 :
						char pString[128];
						PvAttrStringGet(cameraHandle, attributeName, pString, sizeof(pString), NULL);
						printf(", Datatype: String, Value: %s \n", pString);
						break;
					case 4 :
						char pEnum[128];
						char pEnumRange[2048];
							
						PvAttrEnumGet(cameraHandle, attributeName, pEnum, sizeof(pEnum), NULL);
						PvAttrRangeEnum(cameraHandle, attributeName, pEnumRange, sizeof(pEnum), NULL);
						printf(", Datatype: Enum, Value: %s, Set: %s \n", pEnum, pEnumRange);
						break;
					case 5 :
						tPvUint32 pUIValue, pUIMin, pUIMax;
						PvAttrUint32Get(cameraHandle, attributeName, &pUIValue);
						PvAttrRangeUint32(cameraHandle, attributeName, &pUIMin, &pUIMax);
						printf(", Datatype: Uint32, Value: %li, Min: %li, Max: %li \n", pUIValue, pUIMin, pUIMax);
						break;
					case 6 :
						tPvFloat32 pFValue, pFMin, pFMax;
						PvAttrFloat32Get(cameraHandle, attributeName, &pFValue);
						PvAttrRangeFloat32(cameraHandle, attributeName, &pFMin, &pFMax);
						printf(", Datatype: Float32, Value: %f, Min: %f, Max: %f \n", pFValue, pFMin, pFMax);
						break;
					case 7 :
						tPvInt64 pIValue, pIMin, pIMax;
						PvAttrInt64Get(cameraHandle, attributeName, &pIValue);
						PvAttrRangeInt64(cameraHandle, attributeName, &pIMin, &pIMax);
						printf(", Datatype: Int64, Value: %lld, Min: %lld, Max: %lld  \n", pIValue, pIMin, pIMax);
						break;
					case 8 :
						tPvBoolean pBValue;
						PvAttrBooleanGet(cameraHandle, attributeName, &pBValue);
						printf(", Datatype: Boolean, Value: %hhd \n", pBValue);
						break;
					default :
						printf("No Datatype?? \n");
						break;
				}
				
				
				
			}
		}
	}
    
	void Camera::resetAttributes() {
		if (bInitialized) {
			tPvUint32 max_width, max_height, t_min;
			
			PvAttrRangeUint32(cameraHandle, "Width", &t_min, &max_width);
			PvAttrRangeUint32(cameraHandle, "Height", &t_min, &max_height);
			setROIWidth(1.0);
			setROIHeight(1.0);
			setROIX(0.5);
			setROIY(0.5);
			
			setFrameRate(30);
			setAutoExposure(true);
			//		setAutoExposureOnce();
			setAutoExposureAdjustTol(0.05);
			setAutoExposureMaxForCurrentFrameRate();
			setAutoExposureOutliers(0);
			setAutoExposureRate(0.1);
			setAutoExposureTarget(0.2);
			setGain(0);
			
			ofLog(OF_LOG_VERBOSE,"Camera: attributes set to default");
		}
	}
	
	
	//--------------------------------------------------------------------
	//-- WIDTH & HEIGHT --------------------------------------------------
	
	float Camera::getWidth() {
		return getIntAttribute("Width");
	}
	
	float Camera::getHeight() {
		return getIntAttribute("Height");
	}
    
	
	//--------------------------------------------------------------------
	//-- FRAMERATE -------------------------------------------------------
    
	void Camera::setFrameRate(float rate){
	//	setFloatAttribute("FrameRate", rate);
		
		string _name = "FrameRate";
	//	float _value = 25;
		tPvErr error = PvAttrFloat32Set(cameraHandle, _name.c_str(), rate);
		
      //  cout<<PvAttrFloat32Get(cameraHandle, _name.c_str(), rate)<<endl;
        
		if (error == ePvErrSuccess) {
			ofLog(OF_LOG_VERBOSE, "Camera: %i set attribute %s to %f", deviceID, _name.c_str(), rate);
			return true;
		}
		else {
			logError(error);
		}
	}
	
	float Camera::getFrameRate(){
		return getFloatAttribute("FrameRate");
	}
	
	float Camera::getFrameRateMax() {
		return getFloatAttributeMax("FrameRate");
	}
	
	float Camera::getFrameRateMin() {
		return getFloatAttributeMin("FrameRate");
	}
	
	
	//--------------------------------------------------------------------
	//-- REGION OF INTEREST-----------------------------------------------
	
	void Camera::setROIWidth(float w){
		setNormalizedAttribute ("Width", w);
		float rx = regionX * (1 - w);
		setNormalizedAttribute ("RegionX", rx);
		allocatePixels();
		//	allocateTexture();
	}
	
	float Camera::getROIWidth(){
		return getNormalizedAttribute("Width");
	}
	
	void Camera::setROIHeight(float h){
		setNormalizedAttribute ("Height", h);
		float ry = regionY * (1 - h);
		setNormalizedAttribute ("RegionY", ry);
		allocatePixels();
	}
	
	float Camera::getROIHeight(){
		return getNormalizedAttribute("Height");
	}
	
	void Camera::setROIX(float x){
		regionX = x;
		float w = getNormalizedAttribute("Width");
		float rx = regionX * (1 - w);
		setNormalizedAttribute ("RegionX", rx);
	}
	
	float Camera::getROIX(){
		return regionX;
	}
	
	void Camera::setROIY(float y){
		regionY  = y;
		float h = getNormalizedAttribute("Height");
		float ry = regionY * (1 - h);
		setNormalizedAttribute ("RegionY", ry);
	}
	
	float Camera::getROIY(){
		return regionY;
	}
    
		
	void Camera::setROIWidthRaw(int _value) {
		setIntAttribute("Width", _value);
		allocatePixels();
	}

	int Camera::getROIWidthRaw() {
		return getIntAttribute("Width");
	}

	void Camera::setROIHeightRaw(int _value) {
		return setIntAttribute("Height", _value);
		allocatePixels();
	}
	
	int Camera::getROIHeightRaw() {
		return getIntAttribute("Height");
	}

	void Camera::setROIXRaw(int _value) {
		return setIntAttribute("RegionX", _value);
	}
	
	int Camera::getROIXRaw() {
		return getIntAttribute("RegionX");
	}

	void Camera::setROIYRaw(int _value) {
		return setIntAttribute("RegionY", _value);
	}
	
	int Camera::getROIYRaw() {
		return getIntAttribute("RegionY");
	}
	
	int Camera::getROIWidthRawMin() {
		return getIntAttributeMin("Width");
	}
	
	int Camera::getROIWidthRawMax() {
		return getIntAttributeMax("Width");
	}
	
	int Camera::getROIHeightRawMin() {
		return getIntAttributeMin("Height");
	}

	int Camera::getROIHeightRawMax() {
		return getIntAttributeMax("Height");
	}
	
	int Camera::getROIXRawMin() {
		return getIntAttributeMin("RegionX");
	}
	
	int Camera::getROIXRawMax() {
		return getIntAttributeMax("RegionX");
	}
	
	int Camera::getROIYRawMin() {
		return getIntAttributeMin("RegionY");
	}
	
	int Camera::getROIYRawMax() {
		return getIntAttributeMax("RegionY");
	}
	
	//-------------------------------------------------------------------
	//-- EXPOSURE -------------------------------------------------------
	void Camera::setAutoExposure(bool state) {
		if (state == true)
			setEnumAttribute("ExposureMode", "Auto");
		else
			setEnumAttribute("ExposureMode", "Manual");
	}
	
	bool Camera::getAutoExposure() {
		if (getEnumAttribute("ExposureMode") == "Auto")
			return true;
		else
			return false;
	}
	
	void Camera::setAutoExposureOnce() {
		setEnumAttribute("ExposureMode", "AutoOnce");
	}
	
	
	void Camera::setExposure(float _value) {
		setNormalizedAttribute("ExposureValue", _value);
	}
	
	float Camera::getExposure() {
		return getNormalizedAttribute("ExposureValue");
	}
	
	void Camera::setAutoExposureMaxForCurrentFrameRate(){
		int aem = 1000000 / getFrameRate();
		setIntAttribute("ExposureAutoMax", aem);}
		
	void Camera::setAutoExposureAdjustTol(float _value) {
		// Range: [0 – 50] Default: 5 Units: percent
		// Tolerance in variation from ExposureAutoTarget in which the auto exposure algorithm will not respond.
		// Can be used to limit exposure setting changes to only larger variations in scene lighting.
		setNormalizedAttribute("ExposureAutoAdjustTol", _value);
	}
	
	float Camera::getAutoExposureAdjustTol() {
		return getNormalizedAttribute("ExposureAutoAdjustTol");
	}
	
	void Camera::setAutoExposureMin(float _value){
		
		// Range: [Camera dependent - 1000000] Default: 500000 Units: μs
		// The upper bound to the exposure setting in Autoexposure mode. This is useful in situations where frame rate is important.
		// This value would normally be set to something less than 1x10^6/ (desired frame rate).
		setNormalizedAttribute("ExposureAutoMin", _value);
	}
	
	float Camera::getAutoExposureMin() {
		return getNormalizedAttribute("ExposureAutoMin");
	}
	
	void Camera::setAutoExposureMax(float _value){
		
		// Range: [Camera dependent - 1000000] Default: 500000 Units: μs
		// The upper bound to the exposure setting in Autoexposure mode. This is useful in situations where frame rate is important.
		// This value would normally be set to something less than 1x10^6/ (desired frame rate).
		setNormalizedAttribute("ExposureAutoMax", _value);
	}
	
	float Camera::getAutoExposureMax() {
		return getNormalizedAttribute("ExposureAutoMax");
	}
	
	void Camera::setAutoExposureOutliers(float _value){
		// Range: [0 - 1000] Default: 0 Units: 0.01% i.e. 1000 = 10%
		// The total pixels from top of the distribution that are ignored by the auto exposure algorithm.
		setNormalizedAttribute("ExposureAutoOutliers", _value);
	}
	
	float Camera::getAutoExposureOutliers() {
		return getNormalizedAttribute("ExposureAutoOutliers");
	}
	
	void Camera::setAutoExposureRate(float _value) {
		// Range: [1 - 100] Default: 100 Units: percent
		// The rate at which the auto exposure function changes the exposure setting.
		setNormalizedAttribute("ExposureAutoRate", _value);
	}
	
	float Camera::getAutoExposureRate() {
		return getNormalizedAttribute("ExposureAutoRate");
	}
	
	void Camera::setAutoExposureTarget(float _value) {
		// Range: [1 - 100] Default: 50 Units: percent
		// The general lightness or darkness of the auto exposure feature; specifically, the target mean histogram level of the image, 0 being black, 100 being white.
		setNormalizedAttribute("ExposureAutoTarget", _value);
	}
	
	float Camera::getAutoExposureTarget() {
		return getNormalizedAttribute("ExposureAutoTarget");
	}
	
	
	void Camera::setExposureRaw(int _value) {
		setIntAttribute("ExposureValue", _value);
	}
	
	int Camera::getExposureRaw() {
		return getIntAttribute("ExposureValue");
	}
	
	void Camera::setAutoExposureAdjustTolRaw(int _value) {
		setIntAttribute("ExposureAutoAdjustTol", _value);
	}
	
	int Camera::getAutoExposureAdjustTolRaw() {
		return getIntAttribute("ExposureAutoAdjustTol");
	}
	
	void Camera::setAutoExposureMinRaw(int _value) {
		setIntAttribute("ExposureAutoMin", _value);
	}
	
	int Camera::getAutoExposureMinRaw() {
		return getIntAttribute("ExposureAutoMin");
	}
	
	void Camera::setAutoExposureMaxRaw(int _value) {
		setIntAttribute("ExposureAutoMax", _value);
	}
	
	int Camera::getAutoExposureMaxRaw() {
		return getIntAttribute("ExposureAutoMax");
	}
	
	void Camera::setAutoExposureOutliersRaw(int _value) {
		setIntAttribute("ExposureAutoOutliers", _value);
	}
	
	int Camera::getAutoExposureOutliersRaw() {
		return getIntAttribute("ExposureAutoOutliers");
	}
	
	void Camera::setAutoExposureRateRaw(int _value) {
		setIntAttribute("ExposureAutoRate", _value);
	}
	
	int Camera::getAutoExposureRateRaw() {
		return getIntAttribute("ExposureAutoRate");
	}
	
	void Camera::setAutoExposureTargetRaw(int _value) {
		setIntAttribute("ExposureAutoTarget", _value);
	}
	
	int Camera::getAutoExposureTargetRaw() {
		return getIntAttribute("ExposureAutoTarget");
	}
	
	
	int Camera::getExposureRawMin() {
		return getIntAttributeMin("ExposureValue");
	}
	
	int Camera::getExposureRawMax() {
		return getIntAttributeMax("ExposureValue");
	}
	
	int Camera::getAutoExposureAdjustTolRawMin() {
		return getIntAttributeMin("ExposureAutoAdjustTol");
	}
	
	int Camera::getAutoExposureAdjustTolRawMax() {
		return getIntAttributeMax("ExposureAutoAdjustTol");
	}
	
	int Camera::getAutoExposureMinRawMin() {
		return getIntAttributeMin("ExposureAutoMin");
	}
	
	int Camera::getAutoExposureMinRawMax() {
		return getIntAttributeMax("ExposureAutoMin");
	}
	
	int Camera::getAutoExposureMaxRawMin() {
		return getIntAttributeMin("ExposureAutoMax");
	}
	
	int Camera::getAutoExposureMaxRawMax() {
		return getIntAttributeMax("ExposureAutoMax");
	}
	
	int Camera::getAutoExposureOutliersRawMin() {
		return getIntAttributeMin("ExposureAutoOutliers");
	}
	
	int Camera::getAutoExposureOutliersRawMax() {
		return getIntAttributeMax("ExposureAutoOutliers");
	}
	
	int Camera::getAutoExposureRateRawMin() {
		return getIntAttributeMin("ExposureAutoRate");
	}
	
	int Camera::getAutoExposureRateRawMax() {
		return getIntAttributeMax("ExposureAutoRate");
	}
	
	int Camera::getAutoExposureTargetRawMin() {
		return getIntAttributeMin("ExposureAutoTarget");
	}
	
	int Camera::getAutoExposureTargetRawMax() {
		return getIntAttributeMax("ExposureAutoTarget");
	}
	
	//--------------------------------------------------------------------
	//-- GAIN ------------------------------------------------------------
	void Camera::setAutoGain(bool state) {
		if (state == true)
			setEnumAttribute("GainMode", "Auto");
		else
			setEnumAttribute("GainMode", "Manual");
	}
	
	bool Camera::getAutoGain() {
		if (getEnumAttribute("GainMode") == "Auto")
			return true;
		else
			return false;
	}
	
	void Camera::setAutoGainOnce() {
		setEnumAttribute("GainMode", "AutoOnce");
	}
	
	
	void Camera::setGain(float _value) {
		setNormalizedAttribute("GainValue", _value);
	}
	
	float Camera::getGain() {
		return getNormalizedAttribute("GainValue");
	}
		
	void Camera::setAutoGainAdjustTol(float _value) {
		setNormalizedAttribute("GainAutoAdjustTol", _value);
	}
	
	float Camera::getAutoGainAdjustTol() {
		return getNormalizedAttribute("GainAutoAdjustTol");
	}
	
	void Camera::setAutoGainMin(float _value){
		setNormalizedAttribute("GainAutoMin", _value);
	}
	
	float Camera::getAutoGainMin() {
		return getNormalizedAttribute("GainAutoMin");
	}
	
	void Camera::setAutoGainMax(float _value){
		setNormalizedAttribute("GainAutoMax", _value);
	}
	
	float Camera::getAutoGainMax() {
		return getNormalizedAttribute("GainAutoMax");
	}
	
	void Camera::setAutoGainOutliers(float _value){
		setNormalizedAttribute("GainAutoOutliers", _value);
	}
	
	float Camera::getAutoGainOutliers() {
		return getNormalizedAttribute("GainAutoOutliers");
	}
	
	void Camera::setAutoGainRate(float _value) {
		setNormalizedAttribute("GainAutoRate", _value);
	}
	
	float Camera::getAutoGainRate() {
		return getNormalizedAttribute("GainAutoRate");
	}
	
	void Camera::setAutoGainTarget(float _value) {
		setNormalizedAttribute("GainAutoTarget", _value);
	}
	
	float Camera::getAutoGainTarget() {
		return getNormalizedAttribute("GainAutoTarget");
	}
	
	
	void Camera::setGainRaw(int _value) {
		setIntAttribute("GainValue", _value);
	}
	
	int Camera::getGainRaw() {
		return getIntAttribute("GainValue");
	}
	
	void Camera::setAutoGainAdjustTolRaw(int _value) {
		setIntAttribute("GainAutoAdjustTol", _value);
	}
	
	int Camera::getAutoGainAdjustTolRaw() {
		return getIntAttribute("GainAutoAdjustTol");
	}
	
	void Camera::setAutoGainMinRaw(int _value) {
		setIntAttribute("GainAutoMin", _value);
	}
	
	int Camera::getAutoGainMinRaw() {
		return getIntAttribute("GainAutoMin");
	}
	
	void Camera::setAutoGainMaxRaw(int _value) {
		setIntAttribute("GainAutoMax", _value);
	}
	
	int Camera::getAutoGainMaxRaw() {
		return getIntAttribute("GainAutoMax");
	}
	
	void Camera::setAutoGainOutliersRaw(int _value) {
		setIntAttribute("GainAutoOutliers", _value);
	}
	
	int Camera::getAutoGainOutliersRaw() {
		return getIntAttribute("GainAutoOutliers");
	}
	
	void Camera::setAutoGainRateRaw(int _value) {
		setIntAttribute("GainAutoRate", _value);
	}
	
	int Camera::getAutoGainRateRaw() {
		return getIntAttribute("GainAutoRate");
	}
	
	void Camera::setAutoGainTargetRaw(int _value) {
		setIntAttribute("GainAutoTarget", _value);
	}
	
	int Camera::getAutoGainTargetRaw() {
		return getIntAttribute("GainAutoTarget");
	}
	
	
	int Camera::getGainRawMin() {
		return getIntAttributeMin("GainValue");
	}
	
	int Camera::getGainRawMax() {
		return getIntAttributeMax("GainValue");
	}
	
	int Camera::getAutoGainAdjustTolRawMin() {
		return getIntAttributeMin("GainAutoAdjustTol");
	}
	
	int Camera::getAutoGainAdjustTolRawMax() {
		return getIntAttributeMax("GainAutoAdjustTol");
	}
	
	int Camera::getAutoGainMinRawMin() {
		return getIntAttributeMin("GainAutoMin");
	}
	
	int Camera::getAutoGainMinRawMax() {
		return getIntAttributeMax("GainAutoMin");
	}
	
	int Camera::getAutoGainMaxRawMin() {
		return getIntAttributeMin("GainAutoMax");
	}
	
	int Camera::getAutoGainMaxRawMax() {
		return getIntAttributeMax("GainAutoMax");
	}
	
	int Camera::getAutoGainOutliersRawMin() {
		return getIntAttributeMin("GainAutoOutliers");
	}
	
	int Camera::getAutoGainOutliersRawMax() {
		return getIntAttributeMax("GainAutoOutliers");
	}
	
	int Camera::getAutoGainRateRawMin() {
		return getIntAttributeMin("GainAutoRate");
	}
	
	int Camera::getAutoGainRateRawMax() {
		return getIntAttributeMax("GainAutoRate");
	}
	
	int Camera::getAutoGainTargetRawMin() {
		return getIntAttributeMin("GainAutoTarget");
	}
	
	int Camera::getAutoGainTargetRawMax() {
		return getIntAttributeMax("GainAutoTarget");
	}
	
	
	//--------------------------------------------------------------------
	//-- HUE STURATION ------------------------------------------------------------
	
	
	void Camera::setGammaRaw(float _value) {
		tPvErr error = PvAttrFloat32Set(cameraHandle, "Gamma", _value);
		
		if (error == ePvErrSuccess) {
			ofLog(OF_LOG_VERBOSE, "Camera: %i set attribute %s to %f", deviceID, "Gamma", _value);
			return true;
		}
		else {
			logError(error);
		}
	}
	
	float Camera::getGammaRaw() {
		return getFloatAttribute("Gamma");
	}
	
	void Camera::setHueRaw(float _value) {
		tPvErr error = PvAttrFloat32Set(cameraHandle, "Hue", _value);
		
		if (error == ePvErrSuccess) {
			ofLog(OF_LOG_VERBOSE, "Camera: %i set attribute %s to %f", deviceID, "Hue", _value);
			return true;
		}
		else {
			logError(error);
		}
	}
	
	float Camera::getHueRaw() {
		return getFloatAttribute("Hue");
	}
	
	void Camera::setSaturationRaw(float _value) {
		tPvErr error = PvAttrFloat32Set(cameraHandle, "Saturation", _value);
		
		if (error == ePvErrSuccess) {
			ofLog(OF_LOG_VERBOSE, "Camera: %i set attribute %s to %f", deviceID, "Saturation", _value);
			return true;
		}
		else {
			logError(error);
		}
	}
	
	float Camera::getSaturationRaw() {
		return getFloatAttribute("Saturation");
	}
	
	
	int Camera::getGammaRawMin() {
		return getFloatAttributeMin("Gamma");
	}
	
	int Camera::getGammaRawMax() {
		return getFloatAttributeMax("Gamma");
	}
	
	int Camera::getHueRawMin() {
		return getFloatAttributeMin("Hue");
	}
	
	int Camera::getHueRawMax() {
		return getFloatAttributeMax("Hue");
	}
	
	int Camera::getSaturationRawMin() {
		return getFloatAttributeMin("Saturation");
	}
	
	int Camera::getSaturationRawMax() {
		return getFloatAttributeMax("Saturation");
	}
	
	//--------------------------------------------------------------------
	//-- WHITE BALANCE ------------------------------------------------------------
	void Camera::setAutoWhiteBalance(bool state) {
		if (state == true)
			setEnumAttribute("WhitebalMode", "Auto");
		else
			setEnumAttribute("WhitebalMode", "Manual");
	}
	
	bool Camera::getAutoWhiteBalance() {
		if (getEnumAttribute("WhitebalMode") == "Auto")
			return true;
		else
			return false;
	}
	
	void Camera::setAutoWhiteBalanceOnce() {
		setEnumAttribute("WhitebalMode", "AutoOnce");
	}
	
	
	void Camera::setWhiteBalanceBlue(float _value) {
		setNormalizedAttribute("WhitebalValueBlue", _value);
	}
	
	float Camera::getWhiteBalanceBlue() {
		return getNormalizedAttribute("WhitebalValueBlue");
	}
	
	void Camera::setWhiteBalanceRed(float _value) {
		setNormalizedAttribute("WhitebalValueRed", _value);
	}
	
	float Camera::getWhiteBalanceRed() {
		return getNormalizedAttribute("WhitebalValueRed");
	}
	
	void Camera::setAutoWhiteBalanceAdjustTol(float _value) {
		setNormalizedAttribute("WhitebalAutoAdjustTol", _value);
	}
	
	float Camera::getAutoWhiteBalanceAdjustTol() {
		return getNormalizedAttribute("WhitebalAutoAdjustTol");
	}
	
	void Camera::setAutoWhiteBalanceRate(float _value) {
		setNormalizedAttribute("WhitebalAutoRate", _value);
	}
	
	float Camera::getAutoWhiteBalanceRate() {
		return getNormalizedAttribute("WhitebalAutoRate");
	}
	
	
	void Camera::setWhiteBalanceBlueRaw(int _value) {
		setIntAttribute("WhitebalValueBlue", _value);
	}
	
	int Camera::getWhiteBalanceBlueRaw() {
		return getIntAttribute("WhitebalValueBlue");
	}
	
	void Camera::setWhiteBalanceRedRaw(int _value) {
		setIntAttribute("WhitebalValueRed", _value);
	}
	
	int Camera::getWhiteBalanceRedRaw() {
		return getIntAttribute("WhitebalValueRed");
	}
	
	void Camera::setAutoWhiteBalanceAdjustTolRaw(int _value) {
		setIntAttribute("WhitebalAutoAdjustTol", _value);
	}
	
	int Camera::getAutoWhiteBalanceAdjustTolRaw() {
		return getIntAttribute("WhitebalAutoAdjustTol");
	}
	
	void Camera::setAutoWhiteBalanceRateRaw(int _value) {
		setIntAttribute("WhitebalAutoRate", _value);
	}
	
	int Camera::getAutoWhiteBalanceRateRaw() {
		return getIntAttribute("WhitebalAutoRate");
	}
	
	
	int Camera::getWhiteBalanceBlueRawMin() {
		return getIntAttributeMin("WhitebalValueBlue");
	}
	
	int Camera::getWhiteBalanceBlueRawMax() {
		return getIntAttributeMax("WhitebalValueBlue");
	}
	
	int Camera::getWhiteBalanceRedRawMin() {
		return getIntAttributeMin("WhitebalValueRed");
	}
	
	int Camera::getWhiteBalanceRedRawMax() {
		return getIntAttributeMax("WhitebalValueRed");
	}
	
	int Camera::getAutoWhiteBalanceAdjustTolRawMin() {
		return getIntAttributeMin("WhitebalAutoAdjustTol");
	}
	
	int Camera::getAutoWhiteBalanceAdjustTolRawMax() {
		return getIntAttributeMax("WhitebalAutoAdjustTol");
	}
	
	int Camera::getAutoWhiteBalanceRateRawMin() {
		return getIntAttributeMin("WhitebalAutoRate");
	}
	
	int Camera::getAutoWhiteBalanceRateRawMax() {
		return getIntAttributeMax("WhitebalAutoRate");
	}

	
	
	//--------------------------------------------------------------------
	//-- ATTRIBUTES (PROTECTED) ------------------------------------------
	
	void Camera::setNormalizedAttribute(string _name, float _value) {
		
		if (!ofInRange(_value, 0.0, 1.0)) {
			ofLog(OF_LOG_NOTICE, "Camera: %i normalized attribute %s value %f out of range (0.0, 1.0), clamping...", deviceID, _name.c_str(), _value);
			_value = ofClamp(_value, 0.0, 1.0);
		}
		
		tPvAttributeInfo pInfo;
		tPvErr error = PvAttrInfo(cameraHandle, _name.c_str(), &pInfo);
		if (error != ePvErrSuccess) {
			ofLog(OF_LOG_WARNING, "Camera: %i, Attribute %s Not Found ", deviceID, _name.c_str());
			return;
		}
		tPvDatatype pDatatype = pInfo.Datatype;
		
		
		int minI, maxI, attribI;
		float minF, maxF, attribF;
		
		switch (pDatatype) {
			case ePvDatatypeUint32:
				minI = getIntAttributeMin(_name);
				maxI = getIntAttributeMax(_name);
				attribI = _value * (maxI - minI) + minI;
				setIntAttribute(_name, attribI);
				break;
				
			case ePvDatatypeFloat32:
				minF = getIntAttributeMin(_name);
				maxF = getIntAttributeMax(_name);
				attribF = _value * (maxF - minF) + minF;
				setFloatAttribute(_name, attribF);
				ofLog(OF_LOG_NOTICE, "min " + ofToString(minF) + " max " + ofToString(maxF));
				break;
				
			default:
				ofLog(OF_LOG_WARNING, "Camera: Normalized Attribute Not Supported");
				break;
		}
	}
	
	float Camera::getNormalizedAttribute(string _name) {
		
		int min = getIntAttributeMin(_name);
		int max = getIntAttributeMax(_name);
		int attrib = getIntAttribute(_name);
		
		return float(attrib - min) / float(max - min);
	}
	
	
	bool Camera::setIntAttribute(string _name, int _value) {
		tPvUint32 min, max;
		PvAttrRangeUint32(cameraHandle, _name.c_str(), &min, &max);
		
		if (!ofInRange(_value, min, max)) {
			ofLog(OF_LOG_NOTICE, "Camera: %i attribute %s value %i out of range (%i, %i), clamping...", deviceID, _name.c_str(), _value, min, max);
			ofClamp(_value, min, max);
		}
		
		tPvErr error = PvAttrUint32Set(cameraHandle, _name.c_str(), _value);
		
		if (error == ePvErrSuccess) {
			ofLog(OF_LOG_VERBOSE, "Camera: %i set attribute %s to %i in range %i to %i", deviceID, _name.c_str(), _value, min, max);
			return true;
		}
		
		
		ofLog(OF_LOG_ERROR, "Camera: %i failed to set attribute %s to %i", deviceID, _name.c_str(), _value);
		logError(error);
		return false;
	}
	
	int Camera::getIntAttribute(string _name) {
		
		tPvUint32 value;
		tPvErr error = PvAttrUint32Get(cameraHandle, _name.c_str(), &value);
		
		if (error != ePvErrSuccess) {
			ofLog(OF_LOG_ERROR, "Camera: %i failed to get attribute %s", deviceID, _name.c_str());
			logError(error);
			return 0;
		}
		
		return value ;
	}
	
	int Camera::getIntAttributeMax(string _name) {
		tPvUint32 t_min, t_max;
		tPvErr error = PvAttrRangeUint32(cameraHandle, _name.c_str(), &t_min, &t_max);
		
		if (error != ePvErrSuccess) {
			ofLog(OF_LOG_ERROR, "Camera: %i failed to get maximum for attribute %s", deviceID, _name.c_str());
			logError(error);
			return 1;
		}
		
		return t_max;
	}
	
	int Camera::getIntAttributeMin(string _name) {
		tPvUint32 t_min, t_max;
		tPvErr error = PvAttrRangeUint32(cameraHandle, _name.c_str(), &t_min, &t_max);
		
		if (error != ePvErrSuccess) {
			ofLog(OF_LOG_ERROR, "Camera: %i failed to get minimum for attribute %s", deviceID, _name.c_str());
			logError(error);
			return 0;
		}
		
		return t_min;
	}
	
	
	bool Camera::setFloatAttribute(string _name, float _value) {
		
		tPvFloat32 min, max;
		PvAttrRangeFloat32(cameraHandle, _name.c_str(), &min, &max);
		
		if (!ofInRange(_value, min, max)) {
			ofLog(OF_LOG_NOTICE, "Camera: %i attribute %s value %i out of range (%f, %f), clamping...", deviceID, _name.c_str(), _value, min, max);
			ofClamp(_value, min, max);
		}
		
		tPvErr error = PvAttrFloat32Set(cameraHandle, _name.c_str(), _value);
		
		if (error == ePvErrSuccess) {
			ofLog(OF_LOG_VERBOSE, "Camera: %i set attribute %s to %f in range %f to %f", deviceID, _name.c_str(), _value, min, max);
			return true;
		}
		
		ofLog(OF_LOG_ERROR, "Camera: %i failed to set attribute %s to %f", deviceID, _name.c_str(), _value);
		logError(error);
		return false;
	}
	
	float Camera::getFloatAttribute(string _name) {
	    tPvFloat32 value;
	    tPvErr error = PvAttrFloat32Get(cameraHandle, _name.c_str(), &value);
		
		if (error != ePvErrSuccess) {
			ofLog(OF_LOG_ERROR, "Camera: %i failed to get attribute %s", deviceID, _name.c_str());
			logError(error);
			return 0;
		}
		
		return value ;
	}
	
	float Camera::getFloatAttributeMax(string _name) {
		tPvFloat32 t_min, t_max;
		tPvErr error = PvAttrRangeFloat32(cameraHandle, _name.c_str(), &t_min, &t_max);
		
		if (error != ePvErrSuccess) {
			ofLog(OF_LOG_ERROR, "Camera: %i failed to get maximum for attribute %s", deviceID, _name.c_str());
			logError(error);
			return 1;
		}
		
		return t_max;
		
	}
	
	float Camera::getFloatAttributeMin(string _name) {
		tPvFloat32 t_min, t_max;
		tPvErr error = PvAttrRangeFloat32(cameraHandle, _name.c_str(), &t_min, &t_max);
		
		if (error != ePvErrSuccess) {
			ofLog(OF_LOG_ERROR, "Camera: %i failed to get minimum for attribute %s", deviceID, _name.c_str());
			logError(error);
			return 0;
		}
		
		return t_min;
	}
	
	
	bool Camera::setEnumAttribute(string _name, string _value) {
		tPvErr error = PvAttrEnumSet(cameraHandle, _name.c_str(), _value.c_str());
		
		if (error == ePvErrSuccess) {
			ofLog(OF_LOG_VERBOSE, "Camera: %i set %s to %s", deviceID, _name.c_str(), _value.c_str());
			return true;
		}
		
		ofLog(OF_LOG_ERROR, "Camera: %i failed to set %s to %s", deviceID, _name.c_str(), _value.c_str());
		logError(error);
		return false;
	}
	
	string Camera::getEnumAttribute(string _name) {
		
		char attribute[128];
		tPvErr error = PvAttrEnumGet(cameraHandle, _name.c_str(), attribute, sizeof(attribute), NULL);
		
		if (error != ePvErrSuccess) {
			ofLog(OF_LOG_ERROR, "Camera: %i failed to get enumeration attribute %s", deviceID, _name.c_str());
			logError(error);
			return "none";
		}
		
		return attribute ;
	}
	
	
	//--------------------------------------------------------------------
	//-- IP SETTINGS -----------------------------------------------------
    
    void Camera::listIpSettings() {
        
        tPvIpSettings ipSettings;
        PvCameraIpSettingsGet(deviceID, &ipSettings);
        
        ofLogNotice("Printing IP Settings");
        
        if(ipSettings.ConfigMode == ePvIpConfigPersistent)
            printf("Config Mode: Persistent");
        if(ipSettings.ConfigMode == ePvIpConfigDhcp)
            printf("Config Mode: DHCP, fallback to AutoIP");
        if(ipSettings.ConfigMode == ePvIpConfigAutoIp)
            printf("Config Mode: AutoIP only");
        printf("Current Ip Address: %s \n", IPLongToString(ipSettings.CurrentIpAddress).c_str());
        printf("Current Ip Subnet Mask: %s \n", IPLongToString(ipSettings.CurrentIpSubnet).c_str());
        printf("Current Ip Gateway: %s \n", IPLongToString(ipSettings.CurrentIpGateway).c_str());
        printf("Persistent Ip Addr: %s \n", IPLongToString(ipSettings.PersistentIpAddr).c_str());
        printf("Persistent Ip Subnet Mask: %s \n", IPLongToString(ipSettings.PersistentIpSubnet).c_str());
        printf("Persistent Ip Gateway: %s \n", IPLongToString(ipSettings.PersistentIpGateway).c_str());
    }
    
    string Camera::getIpAdress() {
        tPvIpSettings ipSettings;
        PvCameraIpSettingsGet(deviceID, &ipSettings);
        return IPLongToString(ipSettings.CurrentIpAddress);
    }
    
    string Camera::getIpSubnet() {
        tPvIpSettings ipSettings;
        PvCameraIpSettingsGet(deviceID, &ipSettings);
        return IPLongToString(ipSettings.CurrentIpSubnet);
    }
    
    string Camera::getIpGateway() {
        tPvIpSettings ipSettings;
        PvCameraIpSettingsGet(deviceID, &ipSettings);
        return IPLongToString(ipSettings.CurrentIpGateway);
    }
    
    bool Camera::isIpPersistent() {
        tPvIpSettings ipSettings;
        tPvErr error;
        error = PvCameraIpSettingsGet(deviceID, &ipSettings);
        if (error != ePvErrSuccess ){
			logError(error);
			return false;
		}
        if (ipSettings.ConfigMode == ePvIpConfigPersistent)
            return true;
        return false;
    }
    
    
    void Camera::setPersistentIp(bool enable) {
        
        tPvIpSettings ipSettings;
        PvCameraIpSettingsGet(deviceID, &ipSettings);
        if (!enable) {
			clearQueue();
			//stopAcquisition();
			stopCapture();
			closeCamera();
            bInitialized = false;
            
            ipSettings.ConfigMode = ePvIpConfigDhcp;
            
        }
        else {
            
            struct in_addr addr, sn, gw;
            
            if (!inet_pton(AF_INET, persistentIpAdress.c_str(), &addr)) {
                ofLogWarning("Camera: ") << deviceID << ", IP Adress " << persistentIpAdress <<  " is not valid";
                return false;
            }
            if (!inet_pton(AF_INET, persistentIpSubnetMask.c_str(), &sn)) {
                ofLogWarning("Camera: ") << deviceID << ", Subnet Mask " << persistentIpSubnetMask <<  " is not valid";
                return false;
            }
            if (!inet_pton(AF_INET, persistentIpGateway.c_str(), &gw));{
                ofLogWarning("Camera: ") << deviceID << ", Gatway " << persistentIpGateway <<  " is not valid";
                //    return false;
            }
            
			clearQueue();
			//stopAcquisition();
			stopCapture();
			closeCamera();
            bInitialized = false;
            
            ipSettings.ConfigMode = ePvIpConfigPersistent;
            ipSettings.PersistentIpAddr = addr.s_addr;
            ipSettings.PersistentIpSubnet = sn.s_addr;
            ipSettings.PersistentIpGateway = gw.s_addr;
        }
        
        tPvErr error;
        error = PvCameraIpSettingsChange(deviceID, &ipSettings);
        if (error == ePvErrSuccess) {
            ofLogNotice("Camera:") << deviceID << " Camera IP Settings Changed";
        }
        else {
            ofLogNotice("Camera:") << deviceID << " Failed to Change Camera IP Settings";
            logError(error);
        }
        
        ofLogNotice("Camera:") << deviceID << " Reinitializing...";
        initCamera(deviceID);
    }
    
    void Camera::setPersistentIpAdress(string _IpAdress) {
        persistentIpAdress = _IpAdress;
    }
    
    void Camera::setPersistentIpSubnetMask(string _IpSubnet) {
        persistentIpSubnetMask = _IpSubnet;
    }
    
    void Camera::setPersistentIpGateway(string _IpGateway) {
        persistentIpGateway = _IpGateway;
    }
    
    
    unsigned long Camera::IPStringToLong(string _IpAdress) {
        unsigned long addr = inet_addr(_IpAdress.c_str());
        return addr;
    }
    
    string Camera::IPLongToString(unsigned long  _IpAdress) {
        struct in_addr addr;
        char *dot_ip;
        
        addr.s_addr = _IpAdress;
        dot_ip = inet_ntoa(addr);
        
        return dot_ip;
    }
	
    
	bool Camera::setPacketSizeToMax() {
		tPvErr error = PvCaptureAdjustPacketSize(cameraHandle, getIntAttributeMax("PacketSize"));
		
		if( error == ePvErrSuccess ){
			ofLog(OF_LOG_VERBOSE, "Camera: %i packet size set to %i", deviceID, getIntAttribute("PacketSize"));
			return true;
		} else {
			ofLog(OF_LOG_ERROR, "Camera: %i packet is not set", deviceID);
			logError(error);
			return false;
		}
		
	}
	
	
	//--------------------------------------------------------------------
	//-- ERROR LOGGING----------------------------------------------------
	
	void Camera::logError(tPvErr _msg) {
		switch (_msg) {
			case ePvErrCameraFault:
				ofLog(OF_LOG_ERROR, "Camera: %i Unexpected camera fault", deviceID);
				break;
			case ePvErrInternalFault:
				ofLog(OF_LOG_ERROR, "Camera: %i Unexpected fault in PvApi or driver", deviceID);
				break;
			case ePvErrBadHandle:
				ofLog(OF_LOG_ERROR, "Camera: %i handle is invalid", deviceID);
				break;
			case ePvErrBadParameter:
				ofLog(OF_LOG_ERROR, "Camera: %i Bad parameter to API call", deviceID);
				break;
			case ePvErrBadSequence:
				ofLog(OF_LOG_ERROR, "Camera: %i Sequence of API calls is incorrect", deviceID);
				break;
			case ePvErrNotFound:
				ofLog(OF_LOG_ERROR, "Camera: %i camera or attribute not found", deviceID);
				break;
			case ePvErrAccessDenied:
				ofLog(OF_LOG_ERROR, "Camera: %i can not be opened in the specified mode", deviceID);
				break;
			case ePvErrUnplugged:
				ofLog(OF_LOG_ERROR, "Camera: %i was unplugged", deviceID);
				
				close();
				
				break;
			case ePvErrInvalidSetup:
				ofLog(OF_LOG_ERROR, "Camera: %i Setup is invalid (an attribute is invalid)", deviceID);
				break;
			case ePvErrResources:
				ofLog(OF_LOG_ERROR, "Camera: %i System/network resources or memory not available", deviceID);
				break;
			case ePvErrBandwidth:
				ofLog(OF_LOG_ERROR, "Camera: %i 1394??? bandwidth not available", deviceID);
				break;
			case ePvErrQueueFull:
				ofLog(OF_LOG_ERROR, "Camera: %i Too many frames on queue", deviceID);
				break;
			case ePvErrBufferTooSmall:
				ofLog(OF_LOG_ERROR, "Camera: %: Frame buffer is too small", deviceID);
				break;
			case ePvErrCancelled:
				ofLog(OF_LOG_ERROR, "Camera: %i: Frame cancelled by user", deviceID);
				break;
			case ePvErrDataLost:
				ofLog(OF_LOG_ERROR, "Camera: %i The data for the frame was lost", deviceID);
				break;
			case ePvErrDataMissing:
				ofLog(OF_LOG_ERROR, "Camera: %i Some data in the frame is missing", deviceID);
				break;
			case ePvErrTimeout:
				ofLog(OF_LOG_ERROR, "Camera: %i Timeout during wait", deviceID);
				break;
			case ePvErrOutOfRange:
				ofLog(OF_LOG_ERROR, "Camera: %i Attribute value is out of the expected range", deviceID);
				break;
			case ePvErrWrongType:
				ofLog(OF_LOG_ERROR, "Camera: %i Attribute is not this type (wrong access function)", deviceID);
				break;
			case ePvErrForbidden:
				ofLog(OF_LOG_ERROR, "Camera: %i Attribute write forbidden at this time", deviceID);
				break;
			case ePvErrUnavailable:
				ofLog(OF_LOG_ERROR, "Camera: %i Attribute is not available at this time", deviceID);
				break;
			case ePvErrFirewall:
				ofLog(OF_LOG_ERROR, "Camera: %i A firewall is blocking the traffic (Windows only)", deviceID);
				break;
			default:
				break;
		}
		
	}
}