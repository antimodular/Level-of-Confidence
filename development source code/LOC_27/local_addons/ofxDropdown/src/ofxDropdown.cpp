//
//  ofxDropdown.cpp
//  dropdown
//
//  Created by Roy Macdonald on 10/14/18.
//
//


#include "ofxDropdown.h"
#include "ofGraphics.h"
#include "ofxGui.h"

template<class T>
bool ofxDropdown_<T>::bShowOnOver = false;

//--------------------------------------------------------------
template<class T>
ofxDropdown_<T>::ofxDropdown_(ofParameter<T> param, float width , float height){
    setup(param,width,height);
}
template<class T>
ofxDropdown_<T>::ofxDropdown_(ofParameter<T> param, const map<T,string>& dropDownOptions, float width , float height){
    setup(param,width,height);
    add(dropDownOptions);
}
template<class T>
ofxDropdown_<T>::ofxDropdown_(ofParameter<T> param, const std::vector<T> & dropDownOptions, float width , float height){
    setup(param,width,height);
    add(dropDownOptions);
}
template<class T>
ofxDropdown_<T>::ofxDropdown_(std::string name, float width, float height){
    setup(name, width, height);
}

template<class T>
ofxDropdown_<T>::~ofxDropdown_(){
    
}

//--------------------------------------------------------------
template<class T>
ofxDropdown_<T> * ofxDropdown_<T>::setup(std::string name, float width , float height ){
#ifdef USE_OFX_GUI_TOOLTIP
        guiElement = this;
#endif
    if (bIsSetup){
        ofLogWarning("ofxDropdown_<T>::setup" ) << "Dropdown \"" << name << "\" is already setup. Nothing will be done";
        return this;
    }
        
	ofxDropdownOption::setup(name, false , width, height);
	buttonListener = value.newListener(this,&ofxDropdown_::buttonClicked);

	selectedValue.setName(name);
	setlectedValueListener = selectedValue.newListener(this, &ofxDropdown_<T>::selectedValueChanged);
    
    
    groupParams.setName(name);
    group.setup(groupParams);
	
	group.disableHeader();
	group.setParent(this);
	group.unregisterMouseEvents();

    dropdownParams.setName(name + " params");
	dropdownParams.add(bMultiselection);
	dropdownParams.add(bCollapseOnSelection);
    dropdownParams.add(bSetChildrenCollapseOnSelection);
    dropdownParams.add(bSetChildrenMultiSelection);
    dropdownParams.add(bDrawDarkBg);
    
    
    paramsListeners.push(bCollapseOnSelection.newListener(this, &ofxDropdown_<T>::collapseOnSelectionChanged));
    paramsListeners.push(bMultiselection.newListener(this, &ofxDropdown_<T>::multiSelectionChanged));
    
    
    updateSelectedValue();

    bIsSetup = true;
    
	return this;
}

//--------------------------------------------------------------
template<class T>
ofxDropdown_<T> * ofxDropdown_<T>::setup(ofParameter<T> param, float width, float height){
    if (bIsSetup){
        ofLogWarning("ofxDropdown_<T>::setup" ) << "Dropdown \"" << param.getName() << "\" is already setup. Nothing will be done";
        return this;
    }
	selectedValue.makeReferenceTo(param);
	return setup(param.getName(), width, height);
}

//--------------------------------------------------------------
template<class T>
void ofxDropdown_<T>::updateSelectedValue(){
    auto it = find(values.begin(), values.end(), selectedValue.get());
    if(it != values.end()){// it was found. it should be found anyways but better to double check
        auto index = std::distance(values.begin(), it);
        setSelectedValueByIndex(index, true);
        
        auto element = dynamic_cast <ofxDropdownOption *>(group.getControl(options[index]));
        if(element){
            element->enableElement();
        }
        setNeedsRedraw();
    }
}

//--------------------------------------------------------------
template<class T>
void ofxDropdown_<T>::selectedValueChanged(T & newvalue){
	updateSelectedValue();
}
//--------------------------------------------------------------
template<class T>
void ofxDropdown_<T>::setSelectedValueByIndex( const size_t& index, bool bNotify){
//    std::cout << "ofxDropdown_<T>::setSelectedValueByIndex " << index << "  notify: " << std::boolalpha << bNotify <<"\n";
	if(index < values.size()){
		selectedValue = values[index];
		selectedOption = options[index];
		if(!bMultiselection){
            auto control = group.getControl(options[index]);
			if(control != nullptr){
				disableSiblings(&group,control);
			}
//            allSelectedValues.clear();
//            allSelectedValues.push_back(selectedValue);
        }
//        else{
        _setAllSelectedValues();
//            allSelectedValues.clear();
//            for(auto& c: ownedChildren){
//                if(c->isSelected()){
//                    allSelectedValues.push_back(_getDropdownOptionValue(c.get()));
//                }
//            }
            
//        }
		if(bCollapseOnSelection){
			hideDropdown();
		}
        
        if(bNotify) {
            ofNotifyEvent(change_E, options[index], this);
            ofNotifyEvent(change_E_value, values[index], this);
        }
	}
}

//--------------------------------------------------------------
template<class T>
void ofxDropdown_<T>::_setAllSelectedValues(){
    allSelectedValues.clear();
    for(auto& c: ownedChildren){
        if(c->isSelected()){
            allSelectedValues.push_back(_getDropdownOptionValue(c.get()));
        }
    }
    if(allSelectedValues.size() == 0 ){
        setDeselectedValue();
    }else{
        for(auto& c: ownedChildren){
            if(c->isSelected()){
                selectedValue = _getDropdownOptionValue(c.get());
                break;
            }
        }
    }
    
}
//--------------------------------------------------------------
template<class T>
void ofxDropdown_<T>::setSelectedValueByName( const std::string& valueName, bool bNotify){

		auto it = find(options.begin(), options.end(), valueName);
		if(it != options.end()){// it was found. it should be found anyways but better to double check
			auto index = std::distance(options.begin(), it);
			setSelectedValueByIndex(index, bNotify);
		}
	
}
//--------------------------------------------------------------
template<class T>
void ofxDropdown_<T>::groupChanged(const void * sender,bool& b){
//    std::cout << "groupChanged " << b << "\n";
    if(b){
        if(sender){
            auto& g = group.getParameter().castGroup();
            
            int foundIndex = -1;
            for(int i = 0; i <g.size(); i++){
                //			std::cout <<  i  << "  -  ";
                if(g.getBool(i).getInternalObject() == ((ofParameter<bool> *)(sender))->getInternalObject()){
                    foundIndex = i;
                    break;
                }
            }
            if(foundIndex >= 0){
                setSelectedValueByName(g.getVoid(foundIndex).getName(), true);
            }else if(bCollapseOnSelection){
                hideDropdown();
            }
        }else{
            ofLogVerbose("ofxDropdown_::groupChanged(...)")  << "sender = null";
        }
    }else{
//        if(!bMultiselection){
//            allSelectedValues.clear();
//            setDeselectedValue();
//        }else{
            _setAllSelectedValues();
//            if(allSelectedValues.size() == 0){
//                setDeselectedValue();
//            }else{
//                for(auto& c: ownedChildren){
//                    if(c->isSelected()){
//                        selectedValue = _getDropdownOptionValue(c.get());
//                        break;
//                    }
//                }
//            }
//        }
    }
    
}
//template class ofxDropdown_<uint8_t>;
#ifndef TARGET_WIN32
template<>
void ofxDropdown_<ofFile>::setDeselectedValue(){
	selectedValue = ofFile();
    selectedOption = "";
}
#endif
template<>
void ofxDropdown_<std::string>::setDeselectedValue(){
	selectedValue = std::string();
    selectedOption = "";
}
template<> 
void ofxDropdown_<int8_t>::setDeselectedValue(){
	selectedValue = std::numeric_limits<int8_t>::max();
    selectedOption = "";
}
template<> 
void ofxDropdown_<uint8_t>::setDeselectedValue(){
	selectedValue = std::numeric_limits<uint8_t>::max();
    selectedOption = "";
}
template<> 
void ofxDropdown_<int16_t>::setDeselectedValue(){
	selectedValue = std::numeric_limits<int16_t>::max();
    selectedOption = "";
}
template<> 
void ofxDropdown_<uint16_t>::setDeselectedValue(){
	selectedValue = std::numeric_limits<uint16_t>::max();
    selectedOption = "";
}
template<> 
void ofxDropdown_<int32_t>::setDeselectedValue(){
	selectedValue = std::numeric_limits<int32_t>::max();
    selectedOption = "";
}
template<> 
void ofxDropdown_<uint32_t>::setDeselectedValue(){
	selectedValue = std::numeric_limits<uint32_t>::max();
    selectedOption = "";
}
template<> 
void ofxDropdown_<int64_t>::setDeselectedValue(){
	selectedValue = std::numeric_limits<int64_t>::max();
    selectedOption = "";
}
template<> 
void ofxDropdown_<uint64_t>::setDeselectedValue(){
	selectedValue = std::numeric_limits<uint64_t>::max();
    selectedOption = "";
}

//for some reason osx errors if this isn't defined
#ifdef TARGET_OSX

template<> 
void ofxDropdown_<typename std::conditional<std::is_same<uint32_t, size_t>::value || std::is_same<uint64_t, size_t>::value, bool, size_t>::type>::setDeselectedValue(){
    selectedValue = std::numeric_limits<size_t>::max();
    selectedOption = "";
}
#endif

//--------------------------------------------------------------
template<class T>
ofxDropdown_<T> * ofxDropdown_<T>::add(const T& value) {
    return add(value, ofToString(value));
}

#ifndef TARGET_WIN32
template<>
ofxDropdown_<ofFile> * ofxDropdown_<ofFile>::add(const ofFile& value) {
    return add(value, value.getFileName());
}
#endif
//--------------------------------------------------------------
template<>
ofxDropdown_<string> * ofxDropdown_<string>::add(const string& value) {
    return add(value, value);
}


//--------------------------------------------------------------
template<class T>
ofxDropdown_<T> * ofxDropdown_<T>::add(const T& value, const string& option) {
    options.push_back(option);
    values.push_back(value);
    	
	ownedChildren.emplace_back(std::make_unique<ofxDropdownOption>());
	auto o = ownedChildren.back().get();
	if(o){
		o->setup(option, value == selectedValue.get());
        _setDropdownOptionValue(o, value);
        
		groupListeners.push(o->getParameter().template cast<bool>().newListener(this, &ofxDropdown_::groupChanged));
    
		group.add(o);
		_updateGroupWidth();
	}else
	{
		ofLogError("ofxDropdown_<T>::add") << "created children is nullptr";
	}
	
	if(selectedValue.get() == value){
        updateSelectedValue();
    }

    return this;
}

//--------------------------------------------------------------
#ifndef TARGET_WIN32
template<>
void ofxDropdown_<ofFile>::_setDropdownOptionValue(ofxDropdownOption* o, const ofFile& val){
    o->dropdownValue = val.path();
}
#endif

//--------------------------------------------------------------
template<class T>
void ofxDropdown_<T>::_setDropdownOptionValue(ofxDropdownOption* o, const T& val){
    o->dropdownValue = ofToString(val);
}

//--------------------------------------------------------------
template<class T>
T ofxDropdown_<T>::_getDropdownOptionValue(ofxDropdownOption* o){
    return ofFromString<T>(o->dropdownValue);
}

//--------------------------------------------------------------
#ifndef TARGET_WIN32
template<>
ofFile ofxDropdown_<ofFile>::_getDropdownOptionValue(ofxDropdownOption* o){
    return ofFile(o->dropdownValue);
}
#endif

//--------------------------------------------------------------
template<class T>
ofxDropdown_<T> * ofxDropdown_<T>::add(const vector<T> & options){
	for(auto& option: options){
		add(option);
	}
	return this;
}
//--------------------------------------------------------------
template<class T>
ofxDropdown_<T> * ofxDropdown_<T>::add(const map<T,string> & options){
    for(auto& option: options){
        add(option.first, option.second);
    }
    return this;
}
//--------------------------------------------------------------
template<class T>
ofxDropdown_<T> * ofxDropdown_<T>::addDropdown(ofxDropdown_& dd){
	return  addDropdown(&dd);
}
//--------------------------------------------------------------
template<class T>
ofxDropdown_<T> * ofxDropdown_<T>::addDropdown(ofxDropdown_ * dd){
	if(dd){
//		cout << typeid(dd).name() <<"\n";
		group.add(dd);
		childDropdowns.push_back(dd);
		childDropdownListeners.push(dd->dropdownHidden_E.newListener(this, &ofxDropdown_::childDropdownHidden));
		_updateGroupWidth();
	}else{
		ofLogWarning("ofxDropdown_::addDropdown", "cant add nullptr dropdown");
	}
	return dd;
}
//--------------------------------------------------------------
template<class T>
ofxDropdown_<T> * ofxDropdown_<T>::newDropdown(std::string name){
//	auto dd = make_unique<ofxDropdown_<T>>(name, getWidth(), getHeight());
	//ownedChildren.push_back(std::move(dd));
//	cout<< "newDropdown: " << name << "\n";
	ownedDropdowns.emplace_back(std::make_unique<ofxDropdown_<T>>(name, getWidth(), getHeight()));
	auto dd = ownedDropdowns.back().get();
	addDropdown(dd);
	return dd;
}
//--------------------------------------------------------------
template<class T>
ofxDropdown_<T> * ofxDropdown_<T>::newDropdown(ofParameter<T> param){
//	cout<< "newDropdown: " << param.getName() << "\n";
	ownedDropdowns.emplace_back(std::make_unique<ofxDropdown_<T>>(param, getWidth(), getHeight()));
	auto dd = ownedDropdowns.back().get();
	addDropdown(dd);
	return dd;
//	auto dd = make_unique<ofxDropdown_<T>>(param, getWidth(), getHeight());
//	ownedChildren.push_back(std::move(dd));
//	addDropdown( static_cast<ofxDropdown_<T>*>(ownedChildren.back().get()));
//	return dd.get();
//
	
}
//--------------------------------------------------------------
template<class T>
void ofxDropdown_<T>::childDropdownHidden(const void * sender, std::string& s){
	hideDropdown();
}
//--------------------------------------------------------------
template<class T>
void ofxDropdown_<T>::clear(){
	group.clear();
    for(size_t i = 0; i < groupParams.size(); i++){
        groupParams.remove(i);
    }
    group.setup(groupParams);
	childDropdowns.clear();
	ownedChildren.clear();
	ownedDropdowns.clear();
	values.clear();
	options.clear();
    groupListeners.unsubscribeAll();
    guiGroupListeners.unsubscribeAll();
    childDropdownListeners.unsubscribeAll();
    
}
//--------------------------------------------------------------
template<class T>
string ofxDropdown_<T>::getOptionAt(size_t index){
	if(index < group.getNumControls()){
		return group.getControl(index)->getName();
	}
	ofLogNotice("ofxDropdown_::getOptionAt", "index is out of bounds");
	return "";
}
//--------------------------------------------------------------
template<class T>
void ofxDropdown_<T>::enableGuiGroupMouseAndDraw(){
//    int prio = int(defaultEventsPriority) - 200;
    guiGroupListeners.push(ofEvents().draw.newListener(this, &ofxDropdown_::drawGuiGroup, int(OF_EVENT_ORDER_AFTER_APP) + 100 ));
    
//    guiGroupListeners.push(ofEvents().mouseDragged.newListener(this, &ofxDropdown_::groupMouseDragged, prio));
//    guiGroupListeners.push(ofEvents().mouseMoved.newListener(&group, &ofxGuiGroup::mouseMoved, prio));
//    guiGroupListeners.push(ofEvents().mousePressed.newListener(this, &ofxDropdown_::groupMousePressed, prio));
//    guiGroupListeners.push(ofEvents().mouseReleased.newListener(this, &ofxDropdown_::groupMouseReleased, prio));
//    guiGroupListeners.push(ofEvents().mouseScrolled.newListener(this, &ofxDropdown_::scrollGroup, prio));
    
}

//--------------------------------------------------------------
template<class T>
void ofxDropdown_<T>::disableGuiGroupMouseAndDraw(){
    guiGroupListeners.unsubscribeAll();
}
//--------------------------------------------------------------
template<class T>
void ofxDropdown_<T>::_setGroupAutoPosition(){
    auto s = group.getShape();
    glm::vec2 p;
    if(ofGetWidth() - b.getMaxX() >= s.width){
        p.x = b.getMaxX();
    }else if( b.getMinX() >= s.width){
        p.x = b.getMinX() - s.width;
    }else{
        p.x = 0;
    }
    if(ofGetHeight() - b.getMaxY() >= s.height){
        p.y = b.getMaxY();
    }else if( b.getMinY() >= s.height){
        p.y = b.getMinY() - s.height;
    }else{
        p.y = 0;
    }

    group.setPosition(p.x, p.y);
}

//--------------------------------------------------------------
template<class T>
void ofxDropdown_<T>::showDropdown(bool bDisableSiblings){
	if(!bGroupEnabled){
        ofNotifyEvent(dropdownWillShow_E, this);
		bGroupEnabled = true;
		switch(dropDownPosition){
            case DD_AUTO:
                _setGroupAutoPosition();
                break;
			case DD_BELOW:
				group.setPosition(b.x, b.getMaxY());	
				break;
			case DD_ABOVE:
				group.setPosition(b.x, b.y - group.getShape().height);
				break;
			case DD_RIGHT:
				group.setPosition(b.getMaxX(), b.y);
				break;
			case DD_LEFT:
				group.setPosition(b.x - group.getShape().width, b.y);
				break;
		}
		if(bDisableSiblings){			
			disableSiblings(getParent(), this);
		}
        enableGuiGroupMouseAndDraw();
	}
}

//--------------------------------------------------------------
template<class T>
void ofxDropdown_<T>::disableSiblings(ofxBaseGui * parent, ofxBaseGui * child){
	if(parent != nullptr){
		auto p = dynamic_cast<ofxGuiGroup*>(parent);
		if(p){
			for(int i = 0; i < p->getNumControls(); i++ ){
				auto* dd = dynamic_cast <ofxDropdown_*>(p->getControl(i));
				if(dd && dd != child){
					dd->hideDropdown(false);
				}else{
					auto element = dynamic_cast <ofxDropdownOption *>(p->getControl(i));
					if(child != p->getControl(i)){
						disableElement(element, true);
					}else{
						if(element){
							element->enableElement();
						}
					}
				}
			}
		}
	}
}
//--------------------------------------------------------------
template<class T>
void ofxDropdown_<T>::disableElement(ofxDropdownOption* e, bool bCheckAgainstThis){
	if(e){
		if(bCheckAgainstThis && e == this){
			return;
		}
        e->disableElement();
	}
}

//--------------------------------------------------------------
template<class T>
void ofxDropdown_<T>::deselect()
{
	for(auto& c: childDropdowns)
	{
		if(c) c->deselect();
	}
	for(auto& c: ownedDropdowns)
	{
		if(c) c->deselect();
	}
	for(auto&c: ownedChildren)
	{
		if(c) c->deselect();
	}
	selectedOption = "";
	ofxDropdownOption::deselect();
}
//--------------------------------------------------------------
template<class T>
bool ofxDropdown_<T>::scrollGroup(ofMouseEventArgs& args){
    if(isShowingDropdown()){
        if( !group.isMinimized() && group.getShape().inside(args.x, args.y)){
            move({0, args.scrollY * 2, 0});
            return true;
        }else{
            group.mouseScrolled(args);
        }
        return true;
    }
    return false;
}
//--------------------------------------------------------------
template<class T>
bool ofxDropdown_<T>::groupMouseDragged(ofMouseEventArgs & args){
    if(isShowingDropdown()){
        if(group.mouseDragged(args)){
            
            return true;
        }
        return true;
    }
    return false;
}
//--------------------------------------------------------------
template<class T>
bool ofxDropdown_<T>::groupMouseReleased(ofMouseEventArgs & args){
    if(isShowingDropdown()){
        if(group.mouseReleased(args)){
            return true;
        }
        if(!b.inside(args)){
            hideDropdown();
            return true;
        }
        return true;
    }
    return false;
}
//--------------------------------------------------------------
template<class T>
bool ofxDropdown_<T>::groupMousePressed(ofMouseEventArgs & args){
    if(isShowingDropdown()){
        
        if(!group.mousePressed(args))
        {
            hideDropdown();
        }
        return true;
    }
    return false;
}

//--------------------------------------------------------------
template<class T>
bool ofxDropdown_<T>::mouseReleased(ofMouseEventArgs & args){
    if(groupMouseReleased(args)){
        return true;
    }
    if(!bGuiActive) {
        return b.inside(args);
    }
	if(ofxDropdownOption::mouseReleased(args) || b.inside(args)){
		return true;
	}
	return false;
}
//--------------------------------------------------------------
template<class T>
bool ofxDropdown_<T>::mousePressed(ofMouseEventArgs & args){
    if(groupMousePressed(args)){
        return true;
    }
    if(setValue(args.x, args.y, true)){
        return true;
    }
	return false;
}
//--------------------------------------------------------------
template<class T>
bool ofxDropdown_<T>::mouseMoved(ofMouseEventArgs & args){
    if(isShowingDropdown()){
        if(group.mouseMoved(args)){
            return true;
        }
    }
    if(bShowOnOver){
        ofxDropdownOption::mouseMoved(args);
        
        if(!bGroupEnabled && bIsOver){
            showDropdown();
        }else if(!bIsOver && bGroupEnabled){
            hideDropdown( true);
        }
        
        return bIsOver;
        
    }else{
        if(ofxDropdownOption::mouseMoved(args)){
            return true;
        }
        return false;
    }
    
}
//--------------------------------------------------------------
template<class T>
bool ofxDropdown_<T>::mouseDragged(ofMouseEventArgs & args){
    if(groupMouseDragged(args)){
        return true;
    }
	if(ofxDropdownOption::mouseDragged(args)){
		return true;
	}
	return false;
}
//--------------------------------------------------------------
template<class T>
bool ofxDropdown_<T>::mouseScrolled(ofMouseEventArgs & args){
    if(scrollGroup(args)){
        return true;
    }
    if(ofxDropdownOption::mouseScrolled(args)){
		return true;
	}
	return false;
}


//--------------------------------------------------------------
template<class T>
void ofxDropdown_<T>::move(const glm::vec3& offset)
{
	group.setPosition(group.getPosition() + offset);
	for(auto& c: childDropdowns)
	{
		c->move(offset);
	}
	for(auto& c: ownedDropdowns)
	{
		c->move(offset);
	}
}

//--------------------------------------------------------------
template<class T>
void ofxDropdown_<T>::hideDropdown(bool bNotifyEvent){
	if(bGroupEnabled){
		bGroupEnabled = false;
		auto n = value.getName();
		disableElement(this);
		for(auto& c: childDropdowns)
		{
			c->hideDropdown(false);
		}
		for(auto& c: ownedDropdowns)
		{
			c->hideDropdown(false);
		}
		if(bNotifyEvent){
			ofNotifyEvent(dropdownHidden_E, n, this);
		}
        disableGuiGroupMouseAndDraw();
	}
}
//--------------------------------------------------------------
template<class T>
bool ofxDropdown_<T>::isShowingDropdown(){
	return bGroupEnabled;
}
//--------------------------------------------------------------
template<class T>
void ofxDropdown_<T>::buttonClicked(bool &v){
	if(v && !bGroupEnabled){
		showDropdown();
	}else if(!v && bGroupEnabled){
		hideDropdown( false);
	}
}
//--------------------------------------------------------------
template<class T>
void ofxDropdown_<T>::setDropDownPosition(DropDownPosition pos){
	dropDownPosition = pos;
}
//--------------------------------------------------------------
template<class T>
void ofxDropdown_<T>::generateDraw(){
	bg.clear();
	bg.rectangle(b);
	
	generateNameTextMesh(ofRectangle(b.x, b.y, b.width, b.height/2));
	
	arrow.clear();
	auto h = b.getHeight()/2;
	auto x2 = b.getMaxX();
	
	auto y = b.getY();
	
	arrow.setStrokeColor(thisTextColor);
	arrow.setStrokeWidth(2);
	arrow.setFilled(false);
	arrow.moveTo(x2 - h /2,  y  + textPadding);
	arrow.lineTo(x2 - textPadding,  y  + h/2 );
	arrow.lineTo(x2 - h /2,  y  + h - textPadding);
	
	
	ofRectangle optionsTextRect = b;
	optionsTextRect.height *= 0.5;
	optionsTextRect.y = optionsTextRect.getMaxY();
	
	optionTextMesh = getTextMesh(selectedOption, x2 - getTextBoundingBox(selectedOption, 0, 0).width - textPadding , getTextVCenteredInRect(optionsTextRect));
//	ofRectangle textRect = b;
//	textRect.width -= h*2.5;
//	textRect.x += h*1.5;
//	ofRemove(optionTextMesh.getVertices(), [&](const glm::vec3& p){
//		return !textRect.inside(p);
//	});	
	
}

//--------------------------------------------------------------
template<class T>
void ofxDropdown_<T>::drawGuiGroup(ofEventArgs&){
    if(bDrawDarkBg){
        ofPushStyle();
        ofSetColor(0, 170);
        ofDrawRectangle(0, 0, ofGetWidth(), ofGetHeight());
        ofPopStyle();
    }
    if(bGroupEnabled ){
        group.draw();
    }
}


//--------------------------------------------------------------
template<class T>
void ofxDropdown_<T>::render(){
	ofxDropdownOption::render();
	
	arrow.draw();
	ofSetColor(thisTextColor, 200);
	
	bindFontTexture();
	optionTextMesh.draw();
	unbindFontTexture();
	
}
//--------------------------------------------------------------
template<class T>
void ofxDropdown_<T>::setDropdownElementsWidth(float width)
{
	group.setSize(width, group.getHeight());
	group.setWidthElements(width);
	
}
//--------------------------------------------------------------
template<class T>
ofAbstractParameter & ofxDropdown_<T>::getParameter(){
    return group.getParameter();
//	return selectedValue;
}

//--------------------------------------------------------------
template<class T>
void ofxDropdown_<T>::collapseOnSelectionChanged(bool&){
    if(bSetChildrenCollapseOnSelection){
        for(auto& c: childDropdowns){
            if(bCollapseOnSelection){
                c->enableCollapseOnSelection(bSetChildrenCollapseOnSelection);
            }else{
                c->disableCollapseOnSelection(bSetChildrenCollapseOnSelection);
            }
        }
    } 
}
//--------------------------------------------------------------
template<class T>
void ofxDropdown_<T>::multiSelectionChanged(bool&){
    if(bSetChildrenMultiSelection){
        for(auto& c: childDropdowns){
            if(bMultiselection){
                c->enableMultipleSelection(bSetChildrenMultiSelection);
            }else{
                c->disableMultipleSelection(bSetChildrenMultiSelection);
            }
        }
    } 
}


//--------------------------------------------------------------
template<class T>
void ofxDropdown_<T>::enableCollapseOnSelection(bool bPropagateToChildren){
    bSetChildrenCollapseOnSelection = bPropagateToChildren;
	bCollapseOnSelection = true;
}
//--------------------------------------------------------------
template<class T>
void ofxDropdown_<T>::disableCollapseOnSelection(bool bPropagateToChildren){
    bSetChildrenCollapseOnSelection = bPropagateToChildren;
    bCollapseOnSelection = false;
}
//--------------------------------------------------------------
template<class T>
bool ofxDropdown_<T>::isEnabledCollapseOnSelection(){
	return bCollapseOnSelection;
}
//--------------------------------------------------------------
template<class T>
void ofxDropdown_<T>::enableMultipleSelection(bool bPropagateToChildren){
    bSetChildrenMultiSelection = bPropagateToChildren;
	bMultiselection = true;
}
//--------------------------------------------------------------
template<class T>
void ofxDropdown_<T>::disableMultipleSelection(bool bPropagateToChildren){
    bSetChildrenMultiSelection = bPropagateToChildren;
	bMultiselection = false;
}
//--------------------------------------------------------------
template<class T>
bool ofxDropdown_<T>::isEnabledMultipleSelection(){
	return bMultiselection;
}
//--------------------------------------------------------------
template<class T>
bool ofxDropdown_<T>::setValue(float mx, float my, bool bCheck){
    
    if( !isGuiDrawing() ){
        bGuiActive = false;
        return false;
    }
    if( bCheck ){
        if( b.inside(mx, my) ){
            bGuiActive = true;
        }else{
            bGuiActive = false;
            
        }
    }
    if( bGuiActive ){
        value = !value;
        return true;
    }
    return false;
}

//--------------------------------------------------------------
template<class T>
void ofxDropdown_<T>::registerMouseEvents(){
    if(bRegisteredForMouseEvents == true){
        return; // already registered.
    }
    bRegisteredForMouseEvents = true;

    int prio = int(defaultEventsPriority) - 100;

    mouseListeners.push(ofEvents().mouseDragged.newListener(this, &ofxDropdown_<T>::mouseDragged, prio));
    mouseListeners.push(ofEvents().mouseMoved.newListener(this, &ofxDropdown_<T>::mouseMoved, prio));
    mouseListeners.push(ofEvents().mousePressed.newListener(this, &ofxDropdown_<T>::mousePressed, prio));
    mouseListeners.push(ofEvents().mouseReleased.newListener(this, &ofxDropdown_<T>::mouseReleased, prio));
    mouseListeners.push(ofEvents().mouseScrolled.newListener(this, &ofxDropdown_<T>::mouseScrolled, prio));

}


//--------------------------------------------------------------
template<class T>
void ofxDropdown_<T>::unregisterMouseEvents(){
    if(bRegisteredForMouseEvents == false){
        return; // not registered.
    }

    mouseListeners.unsubscribeAll();

    bRegisteredForMouseEvents = false;
}
//--------------------------------------------------------------
template<class T>
void ofxDropdown_<T>::_updateGroupWidth()
{
	
	float mx = ofxBaseGui::defaultWidth;
	float arrowW =  b.getHeight()/2;
	
	for(auto& c: childDropdowns)
	{
		if(c)
			mx = std::max(mx, getTextBoundingBox(c->getName(), 0,0).width + arrowW);
	}
	for(auto& c: ownedDropdowns)
	{
		if(c)
			mx = std::max(mx, getTextBoundingBox(c->getName(), 0,0).width + arrowW);
	}
	
	for(auto& c: ownedChildren)
	{
		if(c)
			mx = std::max(mx, getTextBoundingBox(c->getName(), 0,0).width);
	}
	
	setDropdownElementsWidth(mx);
	
	
}



#ifndef TARGET_WIN32
bool checkExtensionIsAllowed(const ofFile& file, const vector<string>& allowedExtensions){
    string ext = file.getExtension();
    for(auto& e: allowedExtensions){
        if( ext == e) return true;
    }
    return false;
}
#else
bool checkExtensionIsAllowed(const string& filename, const vector<string>& allowedExtensions){
    string ext = ofFilePath::getFileExt(filename);
    for(auto& e: allowedExtensions){
        if( ext == e) return true;
    }
    return false;
}
#endif
//--------------------------------------------------------------
template<class T>
void ofxDropdown_<T>::addFromDir(ofxDropdown_* currentDD, const string& dirpath, const vector<string>& allowedExtensions, bool bRecursive)
{
	ofLogError("ofxDropdown_<T>::addFromDir" ) << "This function only works with ofxDirDropdown";
}
template<>
void
#ifndef TARGET_WIN32
ofxDropdown_<ofFile>
#else
ofxDropdown_<string>
#endif
::addFromDir(ofxDropdown_* currentDD, const string& dirpath, const vector<string>& allowedExtensions, bool bRecursive)
{
	if(currentDD != nullptr)
	{
	
		ofDirectory dir(dirpath);
//		for(auto& ext: allowedExtensions)
//		{
//			dir.allowExt(ext);
//		}
		dir.listDir();
        dir.sort();

		for(size_t i = 0; i < dir.size(); i++)
		{
			ofFile f(dir.getPath(i));
			if(f.exists())
			{
				if(f.isDirectory())
				{
                    if(bRecursive){
                        auto dd = newDropdown(f.getBaseName());
                        addFromDir(dd, f.getAbsolutePath(), allowedExtensions, bRecursive);
                    }
				}
				else
				{
#ifndef TARGET_WIN32
                    if(checkExtensionIsAllowed(f, allowedExtensions)){
                        currentDD->add(f);
                    }
#else
                    if(checkExtensionIsAllowed(f.getAbsolutePath(), allowedExtensions)){
                        currentDD->add(f.getAbsolutePath(), f.getFileName());
                    }
#endif
				}
			}
		}
	}
	else
	{
		ofLogError("ofxDropdown_<ofFile>::addFromDir") << "Cannot add dir to null dropdown";
	}

}


//--------------------------------------------------------------
template<class T>
ofParameter<bool> & ofxDropdown_<T>::getMultiSelectionParameter(){
    return bMultiselection;
}
//--------------------------------------------------------------
template<class T>
ofParameter<bool> & ofxDropdown_<T>::getCollapseOnSelectionParameter(){
    return bCollapseOnSelection;
}

//--------------------------------------------------------------
template<class T>
ofParameterGroup& ofxDropdown_<T>::getDropdownParameters(){
    return dropdownParams;
}

#ifdef USE_OFX_GUI_TOOLTIP
//--------------------------------------------------------------
template<class T>
void ofxDropdown_<T>::setupTooltip(ofJson &json){
    auto& j0 = json[getName()];
    
    if(j0.contains("tooltip")){
        setTooltipText( j0["tooltip"]);
    }else{
        j0["tooltip"] = "";
    }
    auto& j = j0["group"];
    for(auto& c: childDropdowns)
    {
        if(c) c->setupTooltip(j);
    }
    for(auto& c: ownedDropdowns)
    {
        if(c) c->setupTooltip(j);
    }
    for(auto&c: ownedChildren)
    {
        if(c) c->setupTooltip(j);
    }
    enableTooltip();
}


//--------------------------------------------------------------
template<class T>
void ofxDropdown_<T>::resetTooltips(){
    for(auto& c: childDropdowns)
    {
        if(c) c->removeTooltip();
    }
    for(auto& c: ownedDropdowns)
    {
        if(c) c->removeTooltip();
    }
    for(auto&c: ownedChildren)
    {
        if(c) c->removeTooltip();
    }
}

//--------------------------------------------------------------
template<class T>
void ofxDropdown_<T>::addTooltip(T value, const string& text){
    auto o = getOptionByValue(value);
    if(o) o->setTooltipText(text);
}

//--------------------------------------------------------------
template<class T>
void ofxDropdown_<T>::addTooltip(const string& option, const string& text){
    auto o = getOptionByName(option);
    if(o) o->setTooltipText(text);
}

//--------------------------------------------------------------
template<class T>
void ofxDropdown_<T>::enableTooltip(){
    if(!bTooltipsEnabled){
        bTooltipsEnabled = true;
        for(auto& c: childDropdowns)
        {
            if(c) c->enableTooltip();
        }
        for(auto& c: ownedDropdowns)
        {
            if(c) c->enableTooltip();
        }
        for(auto&c: ownedChildren)
        {
            if(c) c->enableTooltip();
        }
    }
}
//--------------------------------------------------------------
template<class T>
void ofxDropdown_<T>::disableTooltip(){
    if(bTooltipsEnabled){
        bTooltipsEnabled = false;
        for(auto& c: childDropdowns)
        {
            if(c) c->disableTooltip();
        }
        for(auto& c: ownedDropdowns)
        {
            if(c) c->disableTooltip();
        }
        for(auto&c: ownedChildren)
        {
            if(c) c->disableTooltip();
        }
    }
}

//--------------------------------------------------------------
template<class T>
void ofxDropdown_<T>::drawTooltip(){
//    cout << "ofxDropdown_<T>::drawTooltip " << getName() << "\n";
    if(bGroupEnabled){
        
        for(auto& c: childDropdowns)
        {
            if(c) c->drawTooltip();
        }
        for(auto& c: ownedDropdowns)
        {
            if(c) c->drawTooltip();
        }
        for(auto&c: ownedChildren)
        {
            if(c) c->drawTooltip();
        }
    }else{
        ofxDropdownOption::drawTooltip();
    }
    
}
#endif
//--------------------------------------------------------------
template<class T>
ofxDropdownOption* ofxDropdown_<T>::getOptionByName(const string& name){
    auto it = find(options.begin(), options.end(), name);
    if(it != options.end()){// it was found. it should be found anyways but better to double check
        auto index = std::distance(options.begin(), it);
        return getOptionByIndex(index);
//        auto c  = group.getControl(index);
//        if(c) return dynamic_cast<ofxDropdownOption*>(c);
    }
    return nullptr;
}
//--------------------------------------------------------------
template<class T>
ofxDropdownOption* ofxDropdown_<T>::getOptionByValue(const T& value){
    auto it = find(values.begin(), values.end(), value);
    if(it != values.end()){// it was found. it should be found anyways but better to double check
        auto index = std::distance(values.begin(), it);
        return getOptionByIndex(index);
//        auto c  = group.getControl(index);
//        if(c) return dynamic_cast<ofxDropdownOption*>(c);
    }
    return nullptr;
}
//--------------------------------------------------------------
template<class T>
ofxDropdownOption* ofxDropdown_<T>::getOptionByIndex(const size_t& index){
    auto c  = group.getControl(index);
    if(c) return dynamic_cast<ofxDropdownOption*>(c);
    return nullptr;
}

//--------------------------------------------------------------
template<class T>
const vector<T> & ofxDropdown_<T>::getAllSelected(){
    return allSelectedValues;
}

//--------------------------------------------------------------
template<class T>
bool ofxDropdown_<T>::containsName(const std::string& name) const {
    auto it = find(options.begin(), options.end(), name);
    return (it != options.end());
}

//--------------------------------------------------------------
template<class T>
bool ofxDropdown_<T>::containsValue(const T& value) const {
    auto it = find(values.begin(), values.end(), value);
    return (it != values.end());
}

//--------------------------------------------------------------
template<class T>
bool ofxDropdown_<T>::setOptionNameByValue(const T& value, const string& newName){
    auto it = find(values.begin(), values.end(), value);
    if(it != values.end()){// it was found. it should be found anyways but better to double check
        auto index = std::distance(values.begin(), it);
        return setOptionNameByIndex(index, newName);
    }
    return false;
}

//--------------------------------------------------------------
template<class T>
bool ofxDropdown_<T>::setOptionNameByIndex(const size_t& index, const string& newName){
    
    if(index < options.size()){
        auto control = group.getControl(options[index]);
        
        if(control != nullptr){
            control->getParameter().setName(newName);
            options[index] = newName;
            setNeedsRedraw();
            return  true;
        }
    }
    return false;
}

//--------------------------------------------------------------
template<class T>
bool ofxDropdown_<T>::updateOptionName(const string& currentName, const string& newName){
    auto it = find(options.begin(), options.end(), currentName);
    if(it != options.end()){// it was found. it should be found anyways but better to double check
        auto index = std::distance(options.begin(), it);
        return setOptionNameByIndex(index, newName);
    }
    return false;
}

//--------------------------------------------------------------
template<class T>
void ofxDropdown_<T>::setEnableSelectOnMouseRelease(bool bEnable){
    _bSelectOnMouseRelease = bEnable;
    for(auto c: childDropdowns){
        if(c)c->setEnableSelectOnMouseRelease(bEnable);
    }
    for(auto& c: ownedDropdowns){
        if(c)c->setEnableSelectOnMouseRelease(bEnable);
    }
    for(auto& c: ownedChildren){
        if(c) c->_bSelectOnMouseRelease = bEnable;
    }
    
}

//--------------------------------------------------------------
template<class T>
void ofxDropdown_<T>::enableSelectOnMouseRelease(){
    setEnableSelectOnMouseRelease(true);
}

//--------------------------------------------------------------
template<class T>
void ofxDropdown_<T>::disableSelectOnMouseRelease(){
    setEnableSelectOnMouseRelease(false);
}

//--------------------------------------------------------------
template<class T>
bool ofxDropdown_<T>::isEnabledSelectOnMouseRelease(){
    return _bSelectOnMouseRelease;
}


//--------------------------------------------------------------
template<class T>
void ofxDropdown_<T>::enableDarkenedBackground(){
    bDrawDarkBg = true;
}
//--------------------------------------------------------------
template<class T>
void ofxDropdown_<T>::disableDarkenedBackground(){
    bDrawDarkBg = false;
}
//--------------------------------------------------------------
template<class T>
bool ofxDropdown_<T>::isDarkenedBackgroundEnabled(){
    return bDrawDarkBg.get();
}

//--------------------------------------------------------------
template<class T>
void ofxDropdown_<T>::enableShowWhenOver(){
    bShowOnOver = true;
}
//--------------------------------------------------------------
template<class T>
void ofxDropdown_<T>::disableShowWhenOver(){
    bShowOnOver = false;
}
//--------------------------------------------------------------
template<class T>
bool ofxDropdown_<T>::isShowWhenOverEnabled(){
    return bShowOnOver;
}




//template class ofxDropdown_<uint8_t>;
#ifndef TARGET_WIN32
template class ofxDropdown_<ofFile>;
#endif
template class ofxDropdown_<string>;
//template class ofxDropdown_<int>;

template class ofxDropdown_<int8_t>;
template class ofxDropdown_<uint8_t>;
template class ofxDropdown_<int16_t>;
template class ofxDropdown_<uint16_t>;
template class ofxDropdown_<int32_t>;
template class ofxDropdown_<uint32_t>;
template class ofxDropdown_<int64_t>;
template class ofxDropdown_<uint64_t>;

//for some reason osx errors if this isn't defined
#ifdef TARGET_OSX
template class ofxDropdown_<typename std::conditional<std::is_same<uint32_t, size_t>::value || std::is_same<uint64_t, size_t>::value, bool, size_t>::type>;
#endif

