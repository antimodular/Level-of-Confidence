/*
 *  AVFoundationVideoGrabber.h
 */

#pragma once

#include "ofConstants.h"

//------
#include "ofVideoBaseTypes.h"
#include "ofTexture.h"
#include "ofThread.h"
#include "ofPixels.h"
#include <mutex>
#include <atomic>

#ifdef __OBJC__

#import <Foundation/Foundation.h>
#import <AVFoundation/AVFoundation.h>
#import <Accelerate/Accelerate.h>
#import <CoreMedia/CoreMedia.h>
#import <CoreVideo/CoreVideo.h>

class ofxAVFoundationGrabber;

@interface OSXxVideoGrabber : NSObject <AVCaptureVideoDataOutputSampleBufferDelegate> {

	@public
	CGImageRef currentFrame;

	int width;
	int height;

	BOOL bInitCalled;
	int deviceID;

	AVCaptureDeviceInput		*captureInput;
	AVCaptureVideoDataOutput    *captureOutput;
	AVCaptureDevice				*device;
	AVCaptureSession			*captureSession;

	ofxAVFoundationGrabber * grabberPtr;
}

-(BOOL)initCapture:(int)framerate capWidth:(int)w capHeight:(int)h uniqueID:(NSString*)uid;
-(void)startCapture;
-(void)stopCapture;
-(void)lockExposureAndFocus;
-(std::vector <ofVideoDevice>)listDevices;
-(NSArray*) getAvailableDevices;
-(void)setDevice:(int)_device;
-(void)eraseGrabberPtr;

-(CGImageRef)getCurrentFrame;

@end

//------

#endif


class ofxAVFoundationGrabber : virtual public ofBaseVideoGrabber{

	public:
		ofxAVFoundationGrabber();
		~ofxAVFoundationGrabber();
        
        ///\brief sets the unique id of the camera which is given when listing the devices.
    /// this ensures you that the same device is used every time.
    /// If the set unique ID is not found, then the device ID will be used if found, otherwise the first device found.
    ///\param uid the unique ID you'd like to be used for this grabber.
        void setUniqueID(const std::string& uid);
		void setDeviceID(int deviceID);
		void setDesiredFrameRate(int capRate);
		bool setPixelFormat(ofPixelFormat PixelFormat);

        bool setup(int w, int h);
		void update();
		bool isFrameNew() const;
		void close();

		ofPixels&		 		getPixels();
        const ofPixels&		    getPixels() const;

		float getWidth() const{
			return width;
		}
		float getHeight() const{
			return height;
		}

        bool isInitialized() const;

		void updatePixelsCB();
		std::vector <ofVideoDevice> listDevices() const;
		ofPixelFormat getPixelFormat() const;

	protected:
		bool newFrame = false;
    std::atomic<bool> bHavePixelsChanged;
		void clear();
		int width, height;

		int device = 0;
    std::string uniqueID = "";
        bool bIsInit = false;

		int fps  = -1;
		ofTexture tex;
		ofPixels pixels;

		#ifdef __OBJC__
			OSXxVideoGrabber * grabber;
		#else
			void * grabber;
		#endif

	public:
		ofPixelFormat pixelFormat;
		ofPixels pixelsTmp;
		bool bLock = false;
		std::mutex capMutex;

};
