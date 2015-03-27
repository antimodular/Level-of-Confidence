//
//  imageGrid.cpp
//  LOC_13
//
//  Created by Stephan Schulz on 2015-02-11.
//
//


#include "imageGrid.h"


imageGrid::imageGrid()
:portrait_fontSize(20)
,portraitFont_y(700)
,portraitFont_lineHeight(20)
//,portraitFrame_top(0)
//,portraitFrame_bottom(0)
//,portraitFrame_left(0)
//,portraitFrame_right(0)
{
}


imageGrid::~imageGrid(){
    
}

void imageGrid::setup(int _cols, int _rows){

    
    /// \param filename The name of the font file to load.
    /// \param fontsize The size in pixels to load the font.
    /// \param _bAntiAliased true iff the font should be anti-aliased.
    /// \param _bFullCharacterSet true iff the full character set should be cached.
    /// \param makeControus true iff the vector contours should be cached.
    /// \param simplifyAmt the amount to simplify the vector contours.  Larger number means more simplified.
    /// \param dpi the dots per inch used to specify rendering size.
    /// \returns true if the font was loaded correctly.
//    bool loadFont(string filename,
//                  int fontsize,
//                  bool _bAntiAliased=true,
//                  bool _bFullCharacterSet=false,
//                  bool makeContours=false,
//                  float simplifyAmt=0.3,
//                  int dpi=0);
    
//    portrait_gridFont.loadFont("HELVETICALTSTD-BOLDCOND.OTF", portrait_fontSize, false);
    portrait_gridFont.loadFont("HELVETICALTSTD-COND.OTF", portrait_fontSize);
    portrait_gridFontSmall.loadFont("HELVETICALTSTD-COND.OTF", portrait_fontSizeSmall);
    
 //   portrait_gridFont.loadFont("HELVETICALTSTD-BOLDCOND.OTF", 50, true,false,false,0.1,300);
//    helveticaSmall.loadFont("HELVETICALTSTD-COND.OTF", 27, false);
//    helveticaCondBoldSmall.loadFont("HELVETICALTSTD-BOLDCOND.OTF", 27, false);


    loadStudentNames();
    
    number_of_images = 43;
    
    ofDirectory dir;
    dir.listDir("portraits/"); //"sounds/");
    
    grid_cols = _cols;
    grid_rows = _rows;
    
    portrait_width = 650; //467; //
    portrait_height = 841; //676; //
    
    portrait_widthMargin = 183; //650 - 467;
    portrait_heightMargin = 165; //841 - 676;
    
    ofImage temp_img;
   // ofImage temp_img2;
  //  ofFbo temp_fbo;
  //  ofPixels temp_pixels;
    
   // temp_fbo.allocate(portrait_width, portrait_height,GL_RGBA);
  //  temp_img2.allocate(portrait_width, portrait_height,OF_IMAGE_COLOR_ALPHA);
    
    float stageW = portrait_width * grid_cols;
    float temp_widthPercent = ofGetWidth() / stageW;
    
    /*
    float smallWidth = ofGetWidth() / (float)grid_cols;
    float temp_marginPercent = portrait_widthMargin/(float)portrait_width;
    smallWidth = smallWidth - (smallWidth*temp_marginPercent);
    
    float smallHeight = portrait_height * (portrait_height/(float)portrait_width);
 */
    
     smallWidth = portrait_width * temp_widthPercent;
     smallHeight = portrait_height * temp_widthPercent;
     smallWidthMargin = portrait_widthMargin * temp_widthPercent;
     smallHeightMargin = portrait_heightMargin * temp_widthPercent;
    
   // temp_img.allocate(467, 676,OF_IMAGE_COLOR_ALPHA);

    
    for (int i =0; i< number_of_images; i++) {
        string temp_fileName = "portraits/"+dir.getName(i);
        // cout<<i<<" , "<<temp_fileName<<endl;
        temp_img.clear();
        temp_img.loadImage(temp_fileName);
       // temp_img.resize(portrait_width, portrait_height);
        /*
        temp_fbo.begin();
        ofClear(255, 255, 255,0);
        temp_img.draw(portrait_widthMargin/2,portrait_heightMargin/2);
      //  temp_img.drawSubsection(portrait_widthMargin/2,portrait_heightMargin/2, temp_img.getWidth(), temp_img.getHeight(), 1, 1,temp_img.getWidth()-1,temp_img.getHeight()-1);
        temp_fbo.end();
        
        temp_fbo.readToPixels(temp_pixels);
        temp_img2.setFromPixels(temp_pixels);
        */
        portrait_images.push_back(temp_img);
        portrait_Pos.push_back(ofPoint(0,0));
        
        temp_img.resize(smallWidth-smallWidthMargin, smallHeight-smallHeightMargin);
        portrait_imagesSmall.push_back(temp_img);
        
    }

    bDrawExtras = true;
    old_bDrawExtras != bDrawExtras;
   
      memset(prediction_array43, 0, sizeof(prediction_array43));
    
    cout<<"imageGrid::setup "<<endl;
}
void imageGrid::update(){
    
    if(old_grid_cols != grid_cols || old_grid_rows != grid_rows){
        old_grid_cols = grid_cols;
        old_grid_rows = grid_rows;
        updateFBO();
    }
    if(old_portraitFont_y != portraitFont_y){
        old_portraitFont_y = portraitFont_y;
        updateFBO();
    }
    if(old_portraitFont_lineHeight != portraitFont_lineHeight){
        old_portraitFont_lineHeight = portraitFont_lineHeight;
        updateFBO();
    }
    /*
    if(old_portraitFrame_top != portraitFrame_top){
        portraitFrame_top = ofClamp(portraitFrame_top, 0, portrait_height/3);
        old_portraitFrame_top = portraitFrame_top;
        updateFBO();
    }
    if(old_portraitFrame_bottom != portraitFrame_bottom){
         portraitFrame_bottom = ofClamp(portraitFrame_bottom, portrait_height/3.0*2.0, portrait_height);
        old_portraitFrame_bottom = portraitFrame_bottom;
        updateFBO();
    }
    if(old_portraitFrame_left != portraitFrame_left){
        portraitFrame_left = ofClamp(portraitFrame_left, 0, portrait_width/3);
        old_portraitFrame_left = portraitFrame_left;
        updateFBO();
    }
    if(old_portraitFrame_right != portraitFrame_right){
          portraitFrame_right = ofClamp(portraitFrame_right,portrait_width/3*2,portrait_width);
        old_portraitFrame_right = portraitFrame_right;
        updateFBO();
    }
    */
    if(old_portrait_fontSize != portrait_fontSize){
        old_portrait_fontSize = portrait_fontSize;
        portrait_gridFont.setSize(portrait_fontSize);
        updateFBO();
    }
    if(old_portrait_fontSizeSmall != portrait_fontSizeSmall){
        old_portrait_fontSizeSmall = portrait_fontSizeSmall;
        portrait_gridFontSmall.setSize(portrait_fontSizeSmall);
        updateFBO();
    }
    if(old_portraitFont_ySmall != portraitFont_ySmall){
        old_portraitFont_ySmall = portraitFont_ySmall;
    }
    if(old_portraitFont_lineHeightSmall != portraitFont_lineHeightSmall){
        old_portraitFont_lineHeightSmall = portraitFont_lineHeightSmall;
    }
    if(old_bDrawExtras != bDrawExtras){
        old_bDrawExtras = bDrawExtras;
        updateFBO();
    }

}
void imageGrid::draw(float x, float y){
    
//    ofSetColor(255, 0, 0);
//    ofFill();
//    ofRect(x,y, 1000, 1000);
    
    ofSetColor(255);
    //portrait_grid.draw(x,y);
    updateFBO();
}
void imageGrid::drawSmall(float x, float y){
    
    //build grid of the 43 portrait images
    ofSetColor(255,255,255,255);
    ofEnableAlphaBlending();
    int ii = 0;
    for(int r=0; r<grid_rows;r++){
        for(int c=0; c<grid_cols;c++){
            if(ii < number_of_images){
                drawOnePortraitSmall(ii,ofPoint(c*smallWidth, r*smallHeight),1);
                ii++;
            }
        }
    }
    
    //build grid of 43 portrait names
    ii = 0;
    for(int r=0; r<grid_rows;r++){
        for(int c=0; c<grid_cols;c++){
            if(ii < number_of_images){
                drawOneFontSmall(ii,ofPoint(c*smallWidth, r*smallHeight));
                ii++;
            }
        }
    }
    ofDisableAlphaBlending();
    
    
}

void imageGrid::updateFBO(){
   // cout<<ofGetTimestampString()<< "------------- make portrait grid FBO -------------"<<endl;
    
  //  portrait_grid.allocate(portrait_width*(grid_cols+2), portrait_height*(grid_rows+2),GL_RGBA);
    
 //   portrait_grid.begin();
 //   ofClear(255,255,255, 0);
    
    
   
    
    ofFill();
    
    //build grid of the 43 portrait images
    ofSetColor(255,255,255,255);
    int ii = 0;
    for(int r=1; r<grid_rows+1;r++){
        for(int c=1; c<grid_cols+1;c++){
            if(ii < number_of_images){
                portrait_Pos[ii] = ofPoint(c*portrait_width, r*portrait_height);
                drawOnePortrait(ii,portrait_Pos[ii],1);
                ii++;
            }
        }
    }
    
    if(bDrawExtras){
        //draw extra portrait images on edges of grid
        //ofSetColor(255,0,0);
        ofSetColor(255);
        ii = 0;
        for(int r=1; r<grid_rows+1;r++){
            for(int c=1; c<grid_cols+1;c++){
                if(ii < number_of_images){
                    if(c == 1){
                        //draw extra at far left edge of grid
                        int t_ii = ii + grid_cols-1;
                        if(t_ii >= number_of_images){
                            //fills far left image of last row that only has 3 images
                            drawOnePortrait(10,ofPoint(0,r*portrait_height),1);
                        }else{
                            drawOnePortrait(t_ii,ofPoint(0,r*portrait_height),1);
                        }
                        //draw extra at far right edge of grid
                        drawOnePortrait(ii,ofPoint((grid_cols+1)*portrait_width,r*portrait_height),1);
                        
                        
                    }
                    if(r == 1){
                        //fills top row with images from 5th row
                        int t_ii = ii + grid_cols*4;
                        //draw extra at top edge of grid
                        drawOnePortrait(t_ii,ofPoint(c*portrait_width,0*portrait_height),1);
                        //draw extra at bottom edge of grid
                        drawOnePortrait(ii,ofPoint(c*portrait_width,(grid_rows+1)*portrait_height),1);
                        
                        if(c == 1){
                            //draw extra at top left corner
                            drawOnePortrait(t_ii+grid_cols-1,ofPoint(0*portrait_width,0*portrait_height),1);
                            //draw extra at bottom left corner
                            drawOnePortrait(ii+grid_cols-1,ofPoint(0*portrait_width,(grid_rows+1)*portrait_height),1);
                            
                        }
                        if(c == grid_rows+1){
                            //draw extra top right conrner
                            drawOnePortrait(t_ii-grid_rows,ofPoint((c+2)*portrait_width,0*portrait_height),1);
                            //draw extra bottom right conrner
                            drawOnePortrait(ii-grid_rows,ofPoint((c+2)*portrait_width,(grid_rows+1)*portrait_height),1);
                        }
                        
                    }
                    if(r == 3 && c > 3){
                        //fill the gap after the last 3 portraits
                        int t_r = grid_rows;
                        drawOnePortrait(ii,ofPoint(c*portrait_width, t_r*portrait_height),1);
                    }
                    
                    ii++;
                }
            }
        }
    }//  if(bDrawExtras){
    
    //build grid of 43 portrait names
    ofSetColor(255);
    for (int i=0; i<number_of_images; i++) {
        drawOneFont(i,portrait_Pos[i]);
    }
    
    if(bDrawExtras){
        //draw extra portrait names on edges of grid
       // ofSetColor(255,0,0);
        ofSetColor(255);
        ii = 0;
        for(int r=1; r<grid_rows+1;r++){
            for(int c=1; c<grid_cols+1;c++){
                if(ii < number_of_images){
                    if(c == 1){
                        //draw extra at far left edge of grid
                        int t_ii = ii + grid_cols-1;
                        if(t_ii >= number_of_images){
                            //fills far left image of last row that only has 3 images
                            drawOneFont(10,ofPoint(0,r*portrait_height));
                        }else{
                            drawOneFont(t_ii,ofPoint(0,r*portrait_height));
                        }
                        //draw extra at far right edge of grid
                        drawOneFont(ii,ofPoint((grid_cols+1)*portrait_width,r*portrait_height));
                        
                        
                    }
                    if(r == 1){
                        //fills top row with images from 5th row
                        int t_ii = ii + grid_cols*4;
                        //draw extra at top edge of grid
                        drawOneFont(t_ii,ofPoint(c*portrait_width,0*portrait_height));
                        //draw extra at bottom edge of grid
                        drawOneFont(ii,ofPoint(c*portrait_width,(grid_rows+1)*portrait_height));
                        
                        if(c == 1){
                            //draw extra at top left corner
                            drawOneFont(t_ii+grid_cols-1,ofPoint(0*portrait_width,0*portrait_height));
                            //draw extra at bottom left corner
                            drawOneFont(ii+grid_cols-1,ofPoint(0*portrait_width,(grid_rows+1)*portrait_height));
                            
                        }
                        if(c == grid_rows+1){
                            //draw extra top right conrner
                            drawOneFont(t_ii-grid_rows,ofPoint((c+2)*portrait_width,0*portrait_height));
                            //draw extra bottom right conrner
                            drawOneFont(ii-grid_rows,ofPoint((c+2)*portrait_width,(grid_rows+1)*portrait_height));
                        }
                        
                    }
                    if(r == 3 && c > 3){
                        //fill the gap after the last 3 portraits
                        int t_r = grid_rows;
                        drawOneFont(ii,ofPoint(c*portrait_width, t_r*portrait_height));
                    }
                    
                    ii++;
                }
            }
        }
    }
    

    
   // portrait_grid.end();
}

void imageGrid::drawOneFont(int i, ofPoint pos){
    
    ofPushMatrix();
    ofTranslate(pos.x, pos.y);
    
    ofSetColor(0, 0, 0);
    
    float temp0_stringWidth = portrait_gridFont.stringWidth(portrait_firstNames[i]);
    float temp1_stringWidth = portrait_gridFont.stringWidth(portrait_lastNames[i]);
    
    portrait_gridFont.drawString(portrait_firstNames[i], (portrait_width/2.0) - (temp0_stringWidth/2.0),portraitFont_y);
    portrait_gridFont.drawString(portrait_lastNames[i], (portrait_width/2.0) - (temp1_stringWidth/2.0),portraitFont_y+portraitFont_lineHeight);
    
    ofPopMatrix();
}
void imageGrid::drawOneFontSmall(int i, ofPoint pos){
    
    ofPushMatrix();
    ofTranslate(pos.x, pos.y);
    
    int temp_predictionAmt_alpha = ofMap(prediction_array43[i], 0, 100, 100, 255,true);
    
    ofSetColor(0, 0, 0,temp_predictionAmt_alpha);
    
    float temp0_stringWidth = portrait_gridFontSmall.stringWidth(portrait_firstNames[i]);
    float temp1_stringWidth = portrait_gridFontSmall.stringWidth(portrait_lastNames[i]);
    
    portrait_gridFontSmall.drawString(portrait_firstNames[i], (smallWidth/2.0) - (temp0_stringWidth/2.0),portraitFont_ySmall);
    portrait_gridFontSmall.drawString(portrait_lastNames[i], (smallWidth/2.0) - (temp1_stringWidth/2.0),portraitFont_ySmall+portraitFont_lineHeightSmall);
    
    ofPopMatrix();
}


void imageGrid::drawOnePortrait(int i, ofPoint pos,float scaler){
    //  portraits[0].draw(0,0);
    
    ofSetColor(255,255,255,255);
    // cout<<i<<" ,"<<temp_x<<" , "<<temp_y<<endl;
    
    ofPushMatrix();
    ofTranslate(pos.x, pos.y);
    ofTranslate(portrait_widthMargin/2,portrait_heightMargin/2);
    portrait_images[i].draw(0,0,portrait_images[i].getWidth()*scaler,portrait_images[i].getHeight()*scaler);
    
   /*
    ofSetColor(0);
    ofFill();
    int temp_predictionAmt = ofMap(prediction_array43[i], 0, 100, 0, portrait_images[i].getHeight()*scaler,true);
    //ofRect(0, portrait_images[i].getHeight()*scaler, 5, temp_predictionAmt,true);
    ofRect(portrait_images[i].getWidth()*scaler, portrait_images[i].getHeight()*scaler, 30, -temp_predictionAmt);
*/
    ofPopMatrix();
}

void imageGrid::drawOnePortraitSmall(int i, ofPoint pos,float scaler){
    //  portraits[0].draw(0,0);
    
    ofSetColor(255,255,255,255);
    // cout<<i<<" ,"<<temp_x<<" , "<<temp_y<<endl;

    ofPushMatrix();
    ofTranslate(pos.x, pos.y);
    ofTranslate(smallWidthMargin/2.0,smallHeightMargin/2.0);
    int temp_predictionAmt_alpha = ofMap(prediction_array43[i], 0, 100, 100, 255,true);

    ofSetColor(255,255,255, temp_predictionAmt_alpha);
    portrait_imagesSmall[i].draw(0,0,portrait_imagesSmall[i].getWidth()*scaler,portrait_imagesSmall[i].getHeight()*scaler);
    
    
    ofSetColor(160,0,0,255);
    ofFill();
    int temp_predictionAmt = ofMap(prediction_array43[i], 0, 100, 1, portrait_imagesSmall[i].getHeight()*scaler,true);
 
    ofRect(portrait_imagesSmall[i].getWidth()*scaler, portrait_imagesSmall[i].getHeight()*scaler, 3, -temp_predictionAmt);
    
    ofPopMatrix();
}

void imageGrid::drawOnePortrait(int i, ofPoint pos,float _w, float _h){
    //  portraits[0].draw(0,0);
    //  ofSetColor(255);
    // cout<<i<<" ,"<<temp_x<<" , "<<temp_y<<endl;
    
    ofPushMatrix();
    ofTranslate(pos.x, pos.y);
    portrait_images[i].draw(0,0,_w,_h);
    
    ofPopMatrix();
}

void imageGrid::loadStudentNames(){
    //-------read names from text file
    //http://forum.openframeworks.cc/t/accents-with-ofxftgl/5752/3?u=stephanschulz
    //save cpp as western iso latin to avoid complains about special chars
    ifstream nameFile; //declare a file stream
    nameFile.open( ofToDataPath("names.txt").c_str() ); //open your text file
    int lineCnt = 0;
    int name_cnt = 0;
    while(nameFile!=NULL) //as long as theres still text to be read
    {
        string line; //declare a string for storage
        getline(nameFile, line); //get a line from the file, put it in the string
        
        if(line.length() > 0){
            
            //cout<<"line = "<<line<<endl;
            vector<string> lineParts;
            lineParts = ofSplitString(line, ":");
            
            
            string firstName = lineParts[0];
            string lastName = "";
            if(lineParts.size() > 1)  lastName = lineParts[1];
            //            int temp_node1 = ofToInt(lineParts[2]);
            //   cout<<"name "<<name_cnt<<" "<<lineParts[0]<<endl;
            cout<<"name "<<name_cnt<<" "<<line<<":"<<endl;
            portrait_firstNames.push_back(firstName);
            portrait_lastNames.push_back(lastName); //line; //lineParts[0];
            
            name_cnt++;
            ofSleepMillis(10);
        }
        //zonesNodes.push_back(str); //push the string onto a vector of strings
    }
    
    nameFile.close();
    
    cout<<"done loadStudentNames"<<endl;
}

void imageGrid::setDrawExtra(bool _b){
    bDrawExtras = _b;
}
int imageGrid::getNumberOfImages(){
    return number_of_images;
}
int imageGrid::getPortraitWidth(){
    return portrait_width;
}
int imageGrid::getPortraitWidthMargin(){
    return portrait_widthMargin;
}
int imageGrid::getPortraitHeightMargin(){
    return portrait_heightMargin;
}
int imageGrid::getPortraitHeight(){
    return portrait_height;
}
ofPoint imageGrid::getPortraitPos(int _id){
  //  cout<<"getPortraitPos "<<_id<<endl;
    return portrait_Pos[_id];
}
string imageGrid::getLastName(int _id){
    return portrait_lastNames[_id];
}
string imageGrid::getFirstName(int _id){
    return portrait_firstNames[_id];
}