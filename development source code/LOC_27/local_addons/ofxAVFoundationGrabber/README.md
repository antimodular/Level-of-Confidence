# ofxAVFoundationGrabber

This is just an implementation that it is almost an exact copy of what there is in openFrameworks' ofAVFoundationGrabber. The main difference is that this version allows you to set the unique ID of the device you want to use, so the same device is opened each time you use it.

Using `ofVideoGrabber::setDeviceID(...)` proved to be unreliable on newer Apple computer with M2 silicon. 
This version uses the unique ID that is provided by AVFoundation:
The unique ID is described as follows.
    ````
    Every available capture device has a unique ID that persists on
    one system across device connections and disconnections,
    application restarts, and reboots of the system itself.
    Applications can store the value returned by this property 
    to recall or track the status of a specific device in the future
    ```` 
    
The idea is that this improvements get into OF's core code so this addon does not need to exist anymore.