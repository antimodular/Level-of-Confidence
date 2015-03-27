//
//  ofxFaceRecognizer.cpp
//  LOC_10
//
//  Created by Stephan Schulz on 2015-02-10.
//
//
#include "ofMain.h"
#include "ofxFaceRecognizer.h"


ofxFaceRecognizer::ofxFaceRecognizer()
{
}

ofxFaceRecognizer::~ofxFaceRecognizer(){
    //close();
}

void ofxFaceRecognizer::setup(int method_used, int _maxFaces, bool bAlreadySavedModel, string folderName) {

    //eigen take much longer to load and longer to generate. also makes much larger yml file
    string method_names[3] = {"eigen","fisher","lbph"};
    
    // Create a FaceRecognizer and train it on the given images:
    methodId = method_used;
    methodName = method_names[method_used];
    if(methodId == 0){
        model = createEigenFaceRecognizer();
    }
    if(methodId == 1){
        model = createFisherFaceRecognizer();
    }
    if(methodId == 2){
        model = createLBPHFaceRecognizer();
    }
    
    //if(_maxFaces > 0){
    
    maxFaces = _maxFaces;
   
 
    //by default my training images should be 150x150 pixels
    //will reset if you use loadTrainingImages()
    image_width = 150;
    image_height = 150;
    //load in training images
    loadTrainingImages(folderName,maxFaces);
    
    
    string compiledDatabasePath = ofToDataPath("model_"+methodName+"_"+ofToString(maxFaces)+"_"+folderName+".yml");
   
    if(bAlreadySavedModel){
        cout<<"model .yml supposedly existing"<<endl;
        model->load(compiledDatabasePath);
        
        cout<<"loaded "<<maxFaces<<" faces with model "<<methodName<<endl;
    }else{
        cout<<"start training new model. this might take a very long time"<<endl;
        cout<<"compiledDatabasePath "<<compiledDatabasePath<<endl;
        cout<<"more so for fisher than eigen"<<endl;
        model->train(allTrainingMats, allTrainingLabels);
        model->save(ofToDataPath(compiledDatabasePath));
        cout<<"trained and saved .yml with "<<maxFaces<<" faces with model "<<methodName<<endl;
    }
    
    // Quit if there are not enough images for this demo.
    if(allTrainingMats.size() <= 1) {
        string error_message = "This demo needs at least 2 images to work. Please add more images to your data set!";
        CV_Error(CV_StsError, error_message);
    }
    

}

void ofxFaceRecognizer::update(ofImage _img){
    
}
void ofxFaceRecognizer::update(Mat _mat){
    model->predict(_mat, prediction, confidence_double);
}


void ofxFaceRecognizer::loadTrainingImages(string _folderName, int _maxFaces){
    
    //folderName = "Tarrlab_rot+crop";
    string imagePath = ofToDataPath(_folderName);
    cout<<"imagePath "<<imagePath<<endl;
    ofDirectory dir(imagePath);
    //  dir.allowExt("pgm");
    dir.allowExt("tiff");
    dir.listDir();
    
    cout<<"dir.numFiles() "<<dir.numFiles()<<endl;
    int lableCnt = 0;
    int imgCountPerPerson = 0;
    string lastLabel = "";
    
    
    for(int i = 0; i < _maxFaces; i++){
        
     //   cout<<"i "<<i;
        string fileName = dir.getName(i);
      //  cout<<" fileName "<<fileName<<endl;
        //file containts ID for each person
        string temp_label = fileName.substr(0,6);
     //   cout<<i<<" "<<lableCnt<<" , "<<temp_label<<endl;
        
        ofImage temp_img;
        temp_img.loadImage(dir.getPath(i));
        allTrainingImages.push_back(temp_img); //stores all the images from database
        allTrainingLabels.push_back(lableCnt); //stores the label for each image. each person has a different label but all the images of the same person have the same label
        
        //since we have more then one pose/image of the same person
        //we only store the first image to reppresent that person
        if(imgCountPerPerson == 0) oneImagePerPerson.push_back(temp_img);
        imgCountPerPerson++;
        
        if(temp_label != lastLabel){
            lastLabel = temp_label;
            
            imgCountPerPerson = 0;
            lableCnt++;
        }

    }
    
    dir.close();
    
    //tried to read files right away in to Mat array but something did not work well.
    //so now first read into ofimage array then to mat array
    for(int i=0; i<allTrainingImages.size();i++){
        allTrainingMats.push_back(toCv(allTrainingImages[i]));
    }
    
    cout<<"image source "<<_folderName<<endl;
    cout<<"found "<<lableCnt<<" different people"<<endl;
    
    uniquePersonCount =  lableCnt; //231
    
    image_width = oneImagePerPerson[0].getWidth();
    image_height = oneImagePerPerson[0].getHeight();
    
    
}

int ofxFaceRecognizer::getPrediction(){
    return prediction;
}
double ofxFaceRecognizer::getConfidence(){
    return confidence_double;
}
float ofxFaceRecognizer::getImageWidth(){
    return image_width;
}
float ofxFaceRecognizer::getImageHeight(){
    return image_height;
}
ofImage ofxFaceRecognizer::getImageOfPerson(int _id){
    return oneImagePerPerson[_id];
}
void ofxFaceRecognizer::drawImageOfPerson(int _id, float _x, float _y, float _w, float _h){
    oneImagePerPerson[_id].draw(_x, _y, _w, _h);
}
void ofxFaceRecognizer::drawImageOfPerson(int _id, ofPoint _p, float _w, float _h){
    oneImagePerPerson[_id].draw(_p, _w, _h);
}
int ofxFaceRecognizer::getUniquePersonCount(){
    return uniquePersonCount;
}
int ofxFaceRecognizer::getMethodId(){
    return methodId;
}
string ofxFaceRecognizer::getMethodName(){
    return methodName;
}