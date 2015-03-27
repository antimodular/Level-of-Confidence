//
//  oneImage.cpp
//  LOC_13
//
//  Created by Stephan Schulz on 2015-02-11.
//
//


#include "oneImage.h"


oneImage::oneImage(){
    
}

oneImage::~oneImage(){
    
}

void oneImage::setup(){
        bigFont.loadFont("HELVETICALTSTD-COND.OTF", 44, false);
   // bigFont.loadFont("HELVETICALTSTD-BOLDCOND.OTF", 44, false);

    cout<<"done oneImage::setup"<<endl;
}

void oneImage::update(){
        if(old_bigFontSize != bigFontSize){
            old_bigFontSize = bigFontSize;
            bigFont.setSize(bigFontSize);
          //  bigFont.
        }
    if(old_bigFont_lineHeight != bigFont_lineHeight){
        old_bigFont_lineHeight = bigFont_lineHeight;
    }
}
void oneImage::drawOnePortrait(ofImage _img, int i, ofPoint pos,float scaler){
    //  portraits[0].draw(0,0);
    //  ofSetColor(255);
    // cout<<i<<" ,"<<temp_x<<" , "<<temp_y<<endl;
    
   
    ofPushMatrix();
    ofTranslate(pos.x, pos.y);
    
     ofSetColor(255);
    _img.draw(portrait_widthMargin/2,portrait_heightMargin/2,_img.getWidth()*scaler,_img.getHeight()*scaler);
    
    /*
    // ofSetColor(0);
    //draw rects on all sides of portrait to visually crop it
    ofFill();
    
    ofRectangle top_rect = ofRectangle(ofPoint(0,0),ofPoint(portrait_width,portraitFrame_top)*scaler); //top rect
    //cout<<"top_rect "<<top_rect<<endl;
    ofRect(top_rect);
    
    ofRectangle bottom_rect = ofRectangle(ofPoint(0,portraitFrame_bottom)*scaler,ofPoint(portrait_width,portrait_height)*scaler); //bottom rect
    ofRect(bottom_rect);
    
    ofRectangle left_rect = ofRectangle(ofPoint(0,0),ofPoint(portraitFrame_left,portrait_height)*scaler); // left rect
    ofRect(left_rect);
    
    ofRectangle right_rect = ofRectangle(ofPoint(portraitFrame_right, 0)*scaler,ofPoint(portrait_width, portrait_height)*scaler); // right rect
    ofRect(right_rect);
    //ofRect(portraitFrame_top, portraitFrame_y, portraitFrame_width, portraitFrame_height);
    */
    ofPopMatrix();
}

void oneImage::drawOneBigFont(int i, ofPoint pos, float scaler, string _first, string _last){
    
    ofPushMatrix();
    ofTranslate(pos.x, pos.y);
    
    ofSetColor(0, 0, 0);
    

    
    float temp0_stringWidth = bigFont.stringWidth(_first);
    float temp1_stringWidth = bigFont.stringWidth(_last);
    
    float temp_portraitWidth = portrait_width * scaler;
    temp_portraitWidth = (temp_portraitWidth/2.0); // * scaler;
    
    /*
    cout<<"i "<<i<<endl;
    cout<<temp0_stringWidth<<endl;
    cout<<temp1_stringWidth<<endl;
    cout<<pos<<endl;
    cout<<scaler<<endl;
    cout<<_first<<endl;
    cout<<_last<<endl;
    cout<<"bigFont_y "<<bigFont_y<<endl;
    cout<<"bigFont_lineHeight "<<bigFont_lineHeight<<endl;
    cout<<"temp_portraitWidth - (temp0_stringWidth/2.0 "<<temp_portraitWidth - (temp0_stringWidth/2.0)<<endl;
    cout<<"end "<<endl;
    */
//    bigFont.drawString(_first,0,0);

 bigFont.drawString(_first,temp_portraitWidth - (temp0_stringWidth/2.0),bigFont_y);
bigFont.drawString(_last, temp_portraitWidth - (temp1_stringWidth/2.0),bigFont_y+bigFont_lineHeight);

    
    ofPopMatrix();
}


void oneImage::setWidthMargin(int _d){
    portrait_widthMargin = _d;
}
void oneImage::setHeightMargin(int _d){
    portrait_heightMargin = _d;
}

void oneImage::setWidth(int _d){
    portrait_width = _d;
}
void oneImage::setHeight(int _d){
    portrait_height = _d;
}
void oneImage::setTop(int _d){
    portraitFrame_top = _d;
}
void oneImage::setBottom(int _d){
    portraitFrame_bottom = _d;
}
void oneImage::setLeft(int _d){
    portraitFrame_left = _d;
}
void oneImage::setRight(int _d){
    portraitFrame_right = _d;
}