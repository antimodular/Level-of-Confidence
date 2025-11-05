# Introduction

ofxFaceRecognizer an addon for openframeworks that implements the face recognition tutorial from the opencv site:
http://docs.opencv.org/modules/contrib/doc/facerec/facerec_tutorial.html
https://github.com/opencv/opencv_3rdparty/tree/contrib_face_alignment_20170818

You extract a face from a video or a static image and feed it in to the recognizer. Make sure the extracted face that gets handed of to the recognizer is 150x150 pix of size.

At the start the recognizer either loads an already exciting .yml file that contains the recognition model or it creates a new model from the images it finds in the faces folder.

You can select between 3 different recognition methods: eigenfaces, fisherfaces or LBPHFaces via the defines.

I am supplying some example faces created by the Tarrlab at Brown University.
http://wiki.cnbc.cmu.edu/Face_Place

You notice that the image file names (MF0907_1100_HA.tiff) contain unique IDs to label each person.
Each person has more then one image but always the same ID. This ID is used by ofxFaceRecognizer to label each person separately.

I used ofxCv align eyes and crop functions to prepare the training images before placing them in the faces folder.


# Installation

Download the source from GitHub, unzip the folder, rename it from ofxFaceRecognizer-master to ofxFaceRecognizer and place it in your `openFrameworks/addons` folder.

You can use the projectGenerator to add this addon to other projects.

# Dependencies
ofxCv
https://github.com/kylemcdonald/ofxCv
ofxOpenCv

# Operating systems

This new version ofxFaceRecognizer has only been tested on osx 10.14.6 with OF 0.11

# Contributions
Thanks Andreas Schmelas <https://github.com/m9dfukc> for making this work with the normal ofxOpenCv addon which comes without the opencv_contrib modules.

## Images
This addon has been used for "Level of Confidence":
<https://github.com/antimodular/Level-of-Confidence>

Screen shot:

![](https://raw.githubusercontent.com/antimodular/ofxFaceRecognizer/master/Screen_Shot.png)
