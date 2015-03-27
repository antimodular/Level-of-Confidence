//
//  imageGrid.h
//  LOC_13
//
//  Created by Stephan Schulz on 2015-02-11.
//
//

#include <stdio.h>
#include "ofMain.h"
#include "ofxFTGLFont.h"

class imageGrid {
    public:
    imageGrid();
    virtual ~imageGrid();
    
    void setup(int _cols, int _rows);
    void update();
    void draw(float x, float y);
    void drawSmall(float x, float y);
    void updateFBO();
    
    void loadStudentNames();
    
    void drawOnePortrait(int i, ofPoint pos, float scaler);
     void drawOnePortrait(int i, ofPoint pos,float _w, float _h);
    void drawOneFont(int i, ofPoint pos);
     void drawOneFontSmall(int i, ofPoint pos);
    void drawOnePortraitSmall(int i, ofPoint pos,float scaler);

    void setDrawExtra(bool _b);
    int getNumberOfImages();
    int getPortraitWidth();
    int getPortraitHeight();
    ofPoint getPortraitPos(int _id);
   
    int getPortraitWidthMargin();
    int getPortraitHeightMargin();
    
    string getLastName(int _id);
    string getFirstName(int _id);
    ofImage getPortrait(int _id);

    
    ofParameter<int> grid_cols,grid_rows;
    ofParameter<int> portraitFont_y;
    ofParameter<int> portraitFont_lineHeight;
   // ofParameter<int> portraitFrame_top,portraitFrame_bottom,portraitFrame_left,portraitFrame_right;
    ofParameter<int> portrait_fontSize;
    
    ofParameter<int> portraitFont_ySmall;
    ofParameter<int> portraitFont_lineHeightSmall;
    ofParameter<int> portrait_fontSizeSmall;
    
    vector<ofImage> portrait_images;
    vector<ofImage> portrait_imagesSmall;
    
      int prediction_array43[43];
    
    protected:
    
    ofxFTGLFont	portrait_gridFont,portrait_gridFontSmall;
   // ofTrueTypeFont portrait_gridFont;
    
    int number_of_images;
    
    vector<string> portrait_firstNames;
    vector<string> portrait_lastNames;
  
    vector<ofPoint> portrait_Pos;
    
  //  ofFbo portrait_grid;
    
    float portrait_width;
    float portrait_height;
    float portrait_widthMargin;
    float portrait_heightMargin;
    
    int old_grid_cols,old_grid_rows;
    int old_portraitFont_y;
    int old_portraitFont_lineHeight;
  //  int old_portraitFrame_top,old_portraitFrame_bottom,old_portraitFrame_left,old_portraitFrame_right;
    int old_portrait_fontSize;

    int old_portraitFont_ySmall;
    int old_portraitFont_lineHeightSmall;
    int old_portrait_fontSizeSmall;
    
    bool bDrawExtras;
    bool old_bDrawExtras;

    float smallWidth;
    float smallHeight;
    float smallWidthMargin;
    float smallHeightMargin;
    
  };
