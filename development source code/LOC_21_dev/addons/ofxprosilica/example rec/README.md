# README #

openframeworks addon for use with Allied Vision Prosilica cameras.
For compatibility with OSX it is based on the legacy GigE SDK from Allied Vision Technology PvAPI.

Tested with the Prosilica GC750 (monochrome) on both OSX and Windows (Visual Studio 2012)

Download the legacy SDK from http://www.alliedvisiontec.com/emea/products/legacy.html
and put the files from the SDK in the addon, according to the stand-in txt files.

for OSX: Extract the SDK and move the files:

* The files from /inc-pc/ (ImageLib.h, PvApi.h, PvRegIo.h) go into /ofxProsilica/libs/PvAPI/include
* The files from /lib-pc/x86/4.2/ (libImagelib.a, libPvAPI.a) go into  /ofxProsilica/libs/PvAPI/lib/osx/

for Windows: Install the SDK (appears in C:/Program Files/Allied Vision Technologies) and move the files:

*  The files from /inc-pc/ (ImageLib.h, PvApi.h, PvRegIo.h) go into /ofxProsilica/libs/PvAPI/include
* The files from /lib-pc/ (Imagelib.lib, PvAPI.lib) go into  /ofxProsilica/libs/PvAPI/lib/win32/

For Visual Studio copy PvAPI.dll to the Project Directory:

* In you can use Project + Properties-> Build events-> Post-Build Event to xcopy it
* xcopy /e /i /y "$(ProjectDir)..\..\..\addons\ofxProsilica\libs\PvAPI\lib\win32\PvAPI.dll" "$(ProjectDir)bin"


The SampleViewer in /bin-pc/ can be very useful for troubleshooting

# TODO #

- Ask Allied Vision Technologies to use their SDK
- Add Color Cam