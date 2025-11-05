#include "ofxVideoGrabber.h"
#include "ofVideoBaseTypes.h"
#include "ofGLUtils.h"
#include "ofAppRunner.h"
#include "ofConstants.h"
#include "ofPixels.h"



using std::shared_ptr;
using std::vector;

//--------------------------------------------------------------------
ofxVideoGrabber::ofxVideoGrabber(){
	bUseTexture			= true;
	requestedDeviceID	= -1;
	internalPixelFormat = OF_PIXELS_RGB;
	desiredFramerate 	= -1;
	tex.resize(1);
}

//--------------------------------------------------------------------
ofxVideoGrabber::~ofxVideoGrabber(){
}

//--------------------------------------------------------------------
void ofxVideoGrabber::setGrabber(shared_ptr<ofBaseVideoGrabber> newGrabber){
    
	grabber = std::dynamic_pointer_cast<OFX_VID_GRABBER_TYPE>( newGrabber);
}

//--------------------------------------------------------------------
shared_ptr<ofBaseVideoGrabber> ofxVideoGrabber::getGrabber(){
	if(!grabber){
		grabber = std::make_shared<OFX_VID_GRABBER_TYPE>();
	}
	return grabber;
}

const shared_ptr<ofBaseVideoGrabber> ofxVideoGrabber::getGrabber() const{
	return grabber;
}

//--------------------------------------------------------------------
bool ofxVideoGrabber::setup(int w, int h, bool setUseTexture){

#ifndef OFX_VID_GRABBER_TYPE
#error OFX_VID_GRABBER_TYPE is not #defined!
#endif

	if(!grabber){
		grabber =std::make_shared<OFX_VID_GRABBER_TYPE>();
	}

	bUseTexture = setUseTexture;

    if(!requestedUniqueID.empty()){
        grabber->setUniqueID(requestedUniqueID);
        
    }
	if( requestedDeviceID >= 0 ){
		grabber->setDeviceID(requestedDeviceID);
	}
    
    
	setPixelFormat(internalPixelFormat); //this safely handles checks for supported format

	if( desiredFramerate!=-1 ){
		grabber->setDesiredFrameRate(desiredFramerate);
	}

	grabber->setup(w, h);

	if( grabber->isInitialized() && bUseTexture ){
		if(!grabber->getTexturePtr()){
			for(std::size_t i=0;i<grabber->getPixels().getNumPlanes();i++){
				ofPixels plane = grabber->getPixels().getPlane(i);
				tex.push_back(ofTexture());
				tex[i].allocate(plane);
			}
		}
	}

	return grabber->isInitialized();
}

//--------------------------------------------------------------------
bool ofxVideoGrabber::initGrabber(int w, int h, bool setUseTexture){
	return setup(w,h,setUseTexture);
}

//--------------------------------------------------------------------
bool ofxVideoGrabber::setPixelFormat(ofPixelFormat pixelFormat) {
	if(grabber){
		if( grabber->isInitialized() ){
			ofLogWarning("ofxVideoGrabber") << "setPixelFormat(): can't set pixel format while grabber is running";
			internalPixelFormat = grabber->getPixelFormat(); 
			return false;
		}else{
			if( grabber->setPixelFormat(pixelFormat) ){		
				internalPixelFormat = grabber->getPixelFormat();  //we do this as either way we want the grabbers format
			}else{
				internalPixelFormat = grabber->getPixelFormat();  //we do this as either way we want the grabbers format
				return false; 					
			}
		}
	}else{
		internalPixelFormat = pixelFormat;	
	}
	return true;
}

//---------------------------------------------------------------------------
ofPixelFormat ofxVideoGrabber::getPixelFormat() const{
	if(grabber){
		internalPixelFormat = grabber->getPixelFormat();
	}
	return internalPixelFormat;
}

//--------------------------------------------------------------------
vector<ofVideoDevice> ofxVideoGrabber::listDevices() const{
	if(!grabber){
		ofxVideoGrabber * mutThis = const_cast<ofxVideoGrabber*>(this);
		mutThis->grabber =std::make_shared<OFX_VID_GRABBER_TYPE>();
	}
	return grabber->listDevices();
}

//--------------------------------------------------------------------
void ofxVideoGrabber::setVerbose(bool bTalkToMe){
	if(grabber){
		grabber->setVerbose(bTalkToMe);
	}
}

//--------------------------------------------------------------------
void ofxVideoGrabber::setDeviceID(int _deviceID){
	requestedDeviceID = _deviceID;
	if( grabber && grabber->isInitialized() ){
		ofLogWarning("ofxVideoGrabber") << "setDeviceID(): can't set device while grabber is running.";
	}
}

//--------------------------------------------------------------------
void ofxVideoGrabber::setUniqueID(const std::string& uid){
    requestedUniqueID = uid;
    if( grabber && grabber->isInitialized() ){
        ofLogWarning("ofxVideoGrabber") << "setUniqueID(): can't set device while grabber is running.";
    }
}

//--------------------------------------------------------------------
void ofxVideoGrabber::setDesiredFrameRate(int framerate){
	desiredFramerate = framerate;
	if(grabber){
		grabber->setDesiredFrameRate(framerate);
	}
}

//---------------------------------------------------------------------------
ofPixels & ofxVideoGrabber::getPixels(){
	return getGrabber()->getPixels();
}

//---------------------------------------------------------------------------
const ofPixels & ofxVideoGrabber::getPixels() const{
	return getGrabber()->getPixels();
}

//---------------------------------------------------------------------------
ofPixels& ofxVideoGrabber::getPixelsRef(){
	return getGrabber()->getPixels();
}

//---------------------------------------------------------------------------
const ofPixels& ofxVideoGrabber::getPixelsRef() const{
	return getGrabber()->getPixels();
}

//------------------------------------
ofTexture & ofxVideoGrabber::getTexture(){
	if(grabber->getTexturePtr() == nullptr){
		return tex[0];
	}
	else{
		return *grabber->getTexturePtr();
	}
}

//------------------------------------
const ofTexture & ofxVideoGrabber::getTexture() const{
	if(grabber->getTexturePtr() == nullptr){
		return tex[0];
	}
	else{
		return *grabber->getTexturePtr();
	}
}

//------------------------------------
ofTexture & ofxVideoGrabber::getTextureReference(){
	return getTexture();
}

//------------------------------------
const ofTexture & ofxVideoGrabber::getTextureReference() const{
	return getTexture();
}

//------------------------------------
vector<ofTexture> & ofxVideoGrabber::getTexturePlanes(){
	if(grabber->getTexturePtr() != nullptr){
		tex.clear();
		tex.push_back(*grabber->getTexturePtr());
	}
	return tex;
}

//------------------------------------
const vector<ofTexture> & ofxVideoGrabber::getTexturePlanes() const{
	if(grabber->getTexturePtr() != nullptr){
		ofxVideoGrabber* mutThis = const_cast<ofxVideoGrabber*>(this);
		mutThis->tex.clear();
		mutThis->tex.push_back(*grabber->getTexturePtr());
	}
	return tex;
}

//---------------------------------------------------------------------------
bool  ofxVideoGrabber::isFrameNew() const{
	if(grabber){
		return grabber->isFrameNew();
	}
	return false;
}

//--------------------------------------------------------------------
void ofxVideoGrabber::update(){
	if(grabber){
		grabber->update();
		if( bUseTexture && !grabber->getTexturePtr() && grabber->isFrameNew() ){
			if(tex.size()!=grabber->getPixels().getNumPlanes()){
				tex.resize(grabber->getPixels().getNumPlanes());
			}
			for(std::size_t i=0;i<grabber->getPixels().getNumPlanes();i++){
				ofPixels plane = grabber->getPixels().getPlane(i);
				bool bDiffPixFormat = ( tex[i].isAllocated() && tex[i].texData.glInternalFormat != ofGetGLInternalFormatFromPixelFormat(plane.getPixelFormat()) );
				if(bDiffPixFormat || !tex[i].isAllocated() ){
					tex[i].allocate(plane);
				}else{
					tex[i].loadData(plane);
				}
			}
		}
	}
}

//--------------------------------------------------------------------
void ofxVideoGrabber::close(){
	if(grabber){
		grabber->close();
	}
	tex.clear();
}

//--------------------------------------------------------------------
void ofxVideoGrabber::videoSettings(void){
	if(grabber){
		grabber->videoSettings();
	}
}

//------------------------------------
void ofxVideoGrabber::setUseTexture(bool bUse){
	bUseTexture = bUse;
}

//------------------------------------
bool ofxVideoGrabber::isUsingTexture() const{
	return bUseTexture;
}


//----------------------------------------------------------
void ofxVideoGrabber::setAnchorPercent(float xPct, float yPct){
	getTexture().setAnchorPercent(xPct, yPct);
}

//----------------------------------------------------------
void ofxVideoGrabber::setAnchorPoint(float x, float y){
	getTexture().setAnchorPoint(x, y);
}

//----------------------------------------------------------
void ofxVideoGrabber::resetAnchor(){
	getTexture().resetAnchor();
}

//------------------------------------
void ofxVideoGrabber::draw(float _x, float _y, float _w, float _h) const{
	ofGetCurrentRenderer()->draw(*this,_x,_y,_w,_h);
}

//------------------------------------
void ofxVideoGrabber::draw(float _x, float _y) const{
	draw(_x, _y,getWidth(),getHeight());
}


//------------------------------------
void ofxVideoGrabber::bind() const{
	shared_ptr<ofBaseGLRenderer> renderer = ofGetGLRenderer();
	if(renderer){
		renderer->bind(*this);
	}
}

//------------------------------------
void ofxVideoGrabber::unbind() const{
	shared_ptr<ofBaseGLRenderer> renderer = ofGetGLRenderer();
	if(renderer){
		renderer->unbind(*this);
	}
}

//----------------------------------------------------------
float ofxVideoGrabber::getHeight() const{
	if(grabber){
		return grabber->getHeight();
	}else{
		return 0;
	}
}

//----------------------------------------------------------
float ofxVideoGrabber::getWidth() const{
	if(grabber){
		return grabber->getWidth();
	}else{
		return 0;
	}
}

//----------------------------------------------------------
bool ofxVideoGrabber::isInitialized() const{
	return grabber && grabber->isInitialized() && (!bUseTexture || tex[0].isAllocated() || grabber->getTexturePtr());
}
