//
//  oneImage.h
//  LOC_13
//
//  Created by Stephan Schulz on 2015-02-11.
//
//


#include <stdio.h>

#include "ofMain.h"
#include "ofxFTGLFont.h"

class oneImage {
public:
    
    oneImage();
    virtual ~oneImage();
    void setup();
    void update();
    
    void drawOnePortrait(ofImage _img,int i, ofPoint pos,float scaler);
    void drawOneBigFont(int i, ofPoint pos, float scaler,string _first, string _last);
    
    void setWidth(int _d);
    void setHeight(int _d);
    void setBottom(int _d);
    void setTop(int _d);
    void setLeft(int _d);
    void setRight(int _d);
    void setWidthMargin(int _d);
    void setHeightMargin(int _d);
    
    ofParameter<int> bigFontSize;
    ofParameter<int> bigFont_y;
    ofParameter<int> bigFont_lineHeight;
    
    
protected:
    
    //ofTrueTypeFont bigFont;
    ofxFTGLFont bigFont;
    
    int portrait_width,portrait_height;
    int portraitFrame_bottom,portraitFrame_top,portraitFrame_left,portraitFrame_right;
    
    int old_bigFontSize;
    int old_bigFont_lineHeight;
    
    int portrait_widthMargin;
    int portrait_heightMargin;
};