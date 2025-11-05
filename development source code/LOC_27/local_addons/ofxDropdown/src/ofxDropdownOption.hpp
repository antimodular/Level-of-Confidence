#pragma once

#include "ofxToggle.h"
#include "ofImage.h"
#include "ofJson.h"
#ifdef USE_OFX_GUI_TOOLTIP
#include "ofxGuiTooltipBase.h"
#endif

template<typename T>
class ofxDropdown_;

class ofxDropdownOption : public ofxBaseGui
#ifdef USE_OFX_GUI_TOOLTIP
, public ofxGuiTooltipBase
#endif
{
  
public:
	ofxDropdownOption(){
#ifdef USE_OFX_GUI_TOOLTIP
        guiElement = this;
#endif
    }
	virtual ~ofxDropdownOption();
    
    template<typename T>
    friend class ofxDropdown_;
	
	void enableElement();
    void disableElement();

	ofxDropdownOption(ofParameter<bool> _bVal, float width = defaultWidth, float height = defaultHeight);
	ofxDropdownOption * setup(ofParameter<bool> _bVal, float width = defaultWidth, float height = defaultHeight);
	ofxDropdownOption * setup(const std::string& toggleName, bool _bVal, float width = defaultWidth, float height = defaultHeight);
	

	virtual bool mouseMoved(ofMouseEventArgs & args)override;
	virtual bool mousePressed(ofMouseEventArgs & args)override;
	virtual bool mouseDragged(ofMouseEventArgs & args)override;
	virtual bool mouseReleased(ofMouseEventArgs & args)override;
	virtual bool mouseScrolled(ofMouseEventArgs & args)override{return false;}
	

	template<class ListenerClass, typename ListenerMethod>
	void addListener(ListenerClass * listener, ListenerMethod method){
		value.addListener(listener,method);
	}

	template<class ListenerClass, typename ListenerMethod>
	void removeListener(ListenerClass * listener, ListenerMethod method){
		value.removeListener(listener,method);
	}

	void select();
	
	virtual void deselect();
    bool isSelected();

	virtual ofAbstractParameter & getParameter() override;

    
    void setUseTexture(bool useTexture);
    bool isUseTexture();
    
    void setTexture(ofTexture& tex);
    
    void setRenderName(bool bRender);
    bool isRenderingName();
    
    bool loadTexture(const std::string& filepath);
    bool saveTexture(const std::string& filepath);

protected:
    
    bool _bSelectOnMouseRelease = false;
    
    
	void generateNameTextMesh(const ofRectangle& rect);
    
    std::string dropdownValue;
    
#ifdef USE_OFX_GUI_TOOLTIP
    virtual bool isOver() override{
        return bIsOver;
    }
#endif

	virtual void render() override;

	ofParameter<bool> value;
	bool bGuiActive;
	bool bIsOver = false;
	
	bool setValue(float mx, float my, bool bCheck) override;
	void generateDraw() override;
	void valueChanged(bool & value);
	ofPath bg;
	ofVboMesh textMesh;
    
    ofImage texture;
    bool _bUseTexture = false;
    bool _bRenderName = true;
private:
    ofEventListener listener;
};
