#include "ofxDropdownOption.hpp"
#include "ofGraphics.h"
using namespace std;

ofxDropdownOption::ofxDropdownOption(ofParameter<bool> _bVal, float width, float height){
#ifdef USE_OFX_GUI_TOOLTIP
        guiElement = this;
#endif
	setup(_bVal,width,height);
}

ofxDropdownOption::~ofxDropdownOption(){
}

ofxDropdownOption * ofxDropdownOption::setup(ofParameter<bool> _bVal, float width, float height){
	b.x = 0;
	b.y = 0;
	b.width = width;
	b.height = height;
	bGuiActive = false;
	value.makeReferenceTo(_bVal);
    
    listener = value.newListener(this,&ofxDropdownOption::valueChanged);
	registerMouseEvents();
	setNeedsRedraw();

	return this;

}

ofxDropdownOption * ofxDropdownOption::setup(const std::string& toggleName, bool _bVal, float width, float height){
	value.set(toggleName,_bVal);
	return setup(value,width,height);
}


bool ofxDropdownOption::mouseMoved(ofMouseEventArgs & args){
	if(isGuiDrawing() && b.inside(args)){
		bIsOver = true;
	}else{
		bIsOver = false;
	}
	return false;
}

bool ofxDropdownOption::mousePressed(ofMouseEventArgs & args){
    if(_bSelectOnMouseRelease){
        bIsOver = bGuiActive = (isGuiDrawing() && b.inside(args));
         return bIsOver;
    }else{
        if(setValue(args.x, args.y, true)){
            bIsOver = true;
            return true;
        }else{
            bIsOver = false;
            return false;
        }
    }
}

bool ofxDropdownOption::mouseDragged(ofMouseEventArgs & args){
	if(bGuiActive && b.inside(args)){
		bIsOver = true;
		return true;
	}else{
		bIsOver = false;
		return false;
	}
}

bool ofxDropdownOption::mouseReleased(ofMouseEventArgs & args){
    if(_bSelectOnMouseRelease){
        bIsOver = false;
        auto ret = setValue(args.x, args.y, true);
        bGuiActive = false;
        return ret;
    }else{
        bool wasGuiActive = bGuiActive;
        bGuiActive = false;
        if(wasGuiActive && b.inside(args)){
            bIsOver = true;
            return true;
        }else{
            bIsOver = false;
            return false;
        }
    }
}

void ofxDropdownOption::generateDraw(){
	bg.clear();
	bg.rectangle(b);
    if(_bRenderName){
        generateNameTextMesh(b);
    }
}

void ofxDropdownOption::generateNameTextMesh(const ofRectangle& rect)
{
	std::string name;
	auto textX = rect.x + textPadding;//+ checkboxRect.width;
	if(getTextBoundingBox(getName(), textX, 0).getMaxX() > rect.getMaxX() - textPadding){
		for(auto c: ofUTF8Iterator(getName())){
			auto next = name;
			ofUTF8Append(next, c);
			if(getTextBoundingBox(next,textX,0).getMaxX() > rect.getMaxX() - textPadding){
				break;
			}else{
				name = next;
			}
		}
	}else{
		name = getName();
	}

	textMesh = getTextMesh(name, textX, getTextVCenteredInRect(rect));
}

void ofxDropdownOption::render(){
	if(value)
	{
		bg.setFillColor(thisFillColor);
	}
	else
	{
		if(bIsOver)
		{
			bg.setFillColor(thisBorderColor);
		}
		else
		{
			bg.setFillColor(thisBackgroundColor);
		}
	}

    bg.draw();


	ofColor c = ofGetStyle().color;
	ofBlendMode blendMode = ofGetStyle().blendingMode;
	if(blendMode!=OF_BLENDMODE_ALPHA){
		ofEnableAlphaBlending();
	}
	ofSetColor(thisTextColor);
    if(_bUseTexture){
        texture.draw(b);
    }
    if(_bRenderName){
        bindFontTexture();
        textMesh.draw();
        unbindFontTexture();
    }
	ofSetColor(c);
	if(blendMode!=OF_BLENDMODE_ALPHA){
		ofEnableBlendMode(blendMode);
	}
}



ofAbstractParameter & ofxDropdownOption::getParameter(){
	return value;
}

void ofxDropdownOption::valueChanged(bool & value){
    setNeedsRedraw();
}

void ofxDropdownOption::enableElement() {
	value.setWithoutEventNotifications(true);
	setNeedsRedraw();
}

void ofxDropdownOption::disableElement() {
    bGuiActive = false;
    value.setWithoutEventNotifications(false);
    setNeedsRedraw();
}

void ofxDropdownOption::select()
{
    value.setWithoutEventNotifications(true);
    setNeedsRedraw();
}

void ofxDropdownOption::deselect()
{
    value.setWithoutEventNotifications(false);
    setNeedsRedraw();
}

bool ofxDropdownOption::isSelected(){
    return value.get();
}


bool ofxDropdownOption::setValue(float mx, float my, bool bCheck){
    
    if( !isGuiDrawing() ){
        bGuiActive = false;
        return false;
    }
    if(_bSelectOnMouseRelease){
        if(b.inside(mx, my)){
              if( bCheck ){
                  if( bGuiActive ){
                      value =  !value;
                  }
              }
              return true;
          }
    }else{
        if( bCheck ){
            if( b.inside(mx, my) ){
                bGuiActive = true;
            }else{
                bGuiActive = false;
            }
        }
        if( bGuiActive ){
            value =  !value;
            return true;
        }
    }
    return false;
}

void ofxDropdownOption::setUseTexture(bool useTexture){
    _bUseTexture = useTexture;
}

bool ofxDropdownOption::isUseTexture(){
    return _bUseTexture;
}
bool ofxDropdownOption::saveTexture(const string& filepath){
    if(texture.isAllocated()){
        if(texture.save(filepath)){
            return true;
        }
    }
    return false;
}

bool ofxDropdownOption::loadTexture(const string& filepath){
    if(ofFile::doesFileExist(filepath) && texture.load(filepath)){
        this->setSize(this->getWidth(), texture.getHeight());
        _bUseTexture = true;
        return true;
    }
    return false;
}


void ofxDropdownOption::setTexture(ofTexture& tex){
    if(tex.isAllocated()){
//        texture.allocate(tex.getTextureData());
        tex.readToPixels(texture.getPixels());
        texture.update();
        this->setSize(this->getWidth(), tex.getHeight());
        
        _bUseTexture = true;
    }
}

void ofxDropdownOption::setRenderName(bool bRender){
    _bRenderName = bRender;
}

bool ofxDropdownOption::isRenderingName(){
    return _bRenderName;
}
