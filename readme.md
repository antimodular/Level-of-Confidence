#Level of Confidence

## Introduction
**“Level of Confidence” (“Nivel de Confianza” in Spanish)** is an art project featuring a face-recognition camera that has been trained with the faces of the 43 disappeared students from Ayotzinapa school in Iguala, Mexico. As you stand in front of the camera, the system uses algorithms to find which student's facial features look most like yours and gives a "Level of Confidence" on how accurate the match is, in percent. The piece will always fail to make a positive match, as we know that the students were likely murdered and burnt, but the commemorative side of the project is the relentless search for them and their overlap with the public's own facial features. The project software is available for free download so that any university, cultural centre, gallery or institution can setup the piece. In addition, if the project is sold commercially, all proceeds will go to the affected community, in the form of scholarships for example.

You need to supply four pieces of hardware to show this project:


## 1. Display
The display can be any flat screen that has a native HD resolution of 1920x1080 pixels and can be hung in a portrait (vertical) orientation. This can be anything from a small 15-inch computer screen all the way to a large LED screen. The center of the display must be hung at around 160 cm from the ground.

## 2. USB Camera
A small USB webcam should be hung at a height of 150 cm (measured from the centre of the camera) and in a vertical orientation. Any USB web camera that works with Skype or other video applications should work with this software. We successfully tested these cameras:
- Logitech QuickCam Pro 9000
- Logitech C910
- Logitech C920
- Logitech C930E
- Microsoft 180P HD
- Apple iSight (Firewire 400)

## 3. Computer
"Level of Confidence" requires an Apple Macintosh computer with OSX 10.8 or higher. The software has been successfully run on the following models:
- Mac Book Pro 2.8 GHz Indel Core i7 processor, 16GB RAM, Intel Iris Pro 1536MB graphics card
- Mac Mini 2.3 GHz Intel Core i7, 4GB RAM, Intel HD 4000 512MB graphics card
- Mac Mini 2.6 GHz Intel Core i5, 8GB RAM, IRIS 5100 graphics card
- Mac Mini 2011 2.7 GHz Intel Core i7, 8GB RAM, 1333MHz DDR3, AMD Radeon HD 6630M 256 MB (with macOS 10.12.6 )

The computer must be connected to the screen by an HDMI, VGA or DVI cable and to the webcam by USB.

This software ran slowly on:
- Mac Mini 2.3 GHz Intel Core i5, 6GB RAM, Intel GRAFX HD3000 graphics card, OSx 10.8.2
- Mac Mini 2.4 GHz Core2Duo, 2GB RAM, Geforce 320M graphics card, OSx 10.8.2 / OSx 10.10

## 4. Light
The public standing in front of the screen should be illuminated with a diffuse light. This is important so that the camera can capture the details of the public’s face. Typically a fluorescent or quartz halogen wash light can be hung on the very same wall and over the display. Ideally the piece will be shown so that the camera faces a blank wall instead of a busy window with natural light, since the sunlight will overpower the camera, this helps the system concentrate in searching for faces which are close to the screen.

## Installation
Download the software from this link: 

<http://lozano-hemmer.com/pieces/level_of_confidence/software/Nivel_de_Confianza.zip>

If the above version does not work try this one. Our tests show it works with macOS 10.12.6

<http://lozano-hemmer.com/pieces/level_of_confidence/software/Nivel_de_Confianza_v22.zip>

Version v25 has been tested on macOS 10.14.6
<https://www.lozano-hemmer.com/pieces/level_of_confidence/software/Nivel_de_Confianza_v25.zip>

Once you have that Unzip the folder and place it on the desktop or anywhere you want. Inside the "Level of Confidence" folder find the LOC_xx.app icon (xx stands for the version number) that looks like this:

![](https://raw.githubusercontent.com/antimodular/Level-of-Confidence/master/images/icon_small.png)

Drag this icon in to the Dock for convenience. Open "System Preferences"
go to "User &Groups"->"Login Items" and add the LOC_xx.app icon to the list of items that will automatically start on log in.
- Go to "Desktop & Screen Saver" and set Screen Saver to "Start after" never.
- Go to "Energy Saver" and unselect "Put hard disks to sleep ...".
- Go to "Displays" and set "Rotation" to 90 ̊. 
- While still in "Display" check that the resolution is set to 1920x1080.
- If you use a laptop stay in the "Display" section and click on the "Arrangement" tab and ensure the external display is the one on the far left. Take a look at [this image](https://raw.githubusercontent.com/antimodular/Level-of-Confidence/master/images/displayArrangement.png).
- If you use a laptop with OSX 10.9 or higher go to “Mission Control” and unselect “Automatically rearrange ... “ and “Displays have separate Spaces”, otherwise the software will not run on your external Display.
- If you get a warning that the project is from an “unidentified developer” go to the preferences -> “security and privacy” and select “ Allow apps dowloaded from anywhere”.

### macOS 10.14
It might happen that the LOC_xx.app does not open and report an error. There is a quick work around that still allows you to run the software.
- right click on LOC_22.app
- select "Show Package Contents"
- navigate to Contents/MacOS/LOC_22 
- double click it
This will first open the Terminal.app and then the artwork app.

## Calibration
When the software starts the live camera view will be visible for 10 seconds. You can use this time to point the camera. After these 10 seconds the camera view disappears and the artwork will be displayed. If you need more then 10 seconds simply press the TAB key on the keyboard and the timer will be reset to give you another 30 seconds.

The current version (v_22) comes in Spanish and English. Simply press the SPACE bar to switch between languages.

To exit the software press the ESC key.

## Text
Please put the following text next to the work:

“Level of Confidence” by Rafael Lozano-Hemmer, 2015. A face-recognition camera that has been trained with the faces of the 43 disappeared students from Ayotzinapa school in Iguala, Mexico. To acquire or exhibit this work please contact <karine@antimodular.com> all proceeds will go to fund scholarships in the affected community.


## Information
For more information on this project, videos, photos and press, please visit: 

<http://www.lozano-hemmer.com/level_of_confidence.php>

To download the source code please visit our GitHub. If you program on OpenFrameworks, you can modify the project to look for other missing people. 

<https://github.com/antimodular/Level-of-Confidence>

## Images
Screen shot:

![](https://raw.githubusercontent.com/antimodular/Level-of-Confidence/master/images/Screen_Shot.png)

3D-render of the setup:

![](https://raw.githubusercontent.com/antimodular/Level-of-Confidence/master/images/3d_model.png)
