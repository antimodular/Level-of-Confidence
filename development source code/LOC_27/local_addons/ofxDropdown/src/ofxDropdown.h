//
//  ofxDropdown.hpp
//  dropdown
//
//  Created by Roy Macdonald on 10/14/18.
//
//
//
#pragma once


#include "ofParameter.h"
#include "ofxGuiGroup.h"

#include "ofxDropdownOption.hpp"

#ifdef USE_OFX_GUI_TOOLTIP
#include "ofxGuiTooltipBase.h"
#endif

#include "ofJson.h"

using namespace std;



template<class T>
class ofxDropdown_: public ofxDropdownOption
{

public:
    virtual ~ofxDropdown_();
    // ---------------------------------------------------
    // ----------------- Constructors
    // ---------------------------------------------------
    
    /// default constructor.
    /// \warning You will need to call the one of the setup class methods to be able to use this dropdown
	ofxDropdown_(){
#ifdef USE_OFX_GUI_TOOLTIP
        guiElement = this;
#endif
    };
    
    /// \brief constructor that calls setup function with same parameters
    /// Dropdown will be empty. you should add values with the add(...) function
    /// @param name the name of the dropdown.
    /// @param width = the width of the drawn element. Defaults to ofxGui's default
    /// @param height = the height of the drawn element. Defaults to twice of ofxGui's default. This is because one line is used to draw the dropdown name and the second for the selected value it has, if any.
	ofxDropdown_(std::string name, float width = defaultWidth, float height = defaultHeight*2);
    
    /// \brief constructor that calls setup function with same parameters
    /// Dropdown will be empty. You should add values with the add(...) function
    /// @param param The passed ofParameter will be "linked" to the dropdown and will change as you select different elements from the dropdown
    /// The name of this dropdown is taken from the passed ofParameter's name
    /// @param width = the width of the drawn element. Defaults to ofxGui's default
    /// @param height = the height of the drawn element. Defaults to twice of ofxGui's default. This is because one line is used to draw the dropdown name and the second for the selected value it has, if any.
    ofxDropdown_(ofParameter<T> param, float width = defaultWidth, float height = defaultHeight*2);
    
    /// \brief Construct and add elements
    /// @param param The passed ofParameter will be "linked" to the dropdown and will change as you select different elements from the dropdown
    /// The name of this dropdown is taken from the passed ofParameter's name
    /// \param dropDownOptions  std::map which maps  whatever type the passed ofParameter has to a string representation of it. Dropdown will be populated with it.
    /// @param width = the width of the drawn element. Defaults to ofxGui's default
    /// @param height = the height of the drawn element. Defaults to twice of ofxGui's default. This is because one line is used to draw the dropdown name and the second for the selected value it has, if any.
    /// This is useful when for instance you have an ofParameter<int> but you want to display different names for each value, rather than the value itself.
    /// You still can add more values with the add(...) function
    ofxDropdown_(ofParameter<T> param, const map<T,string>& dropDownOptions, float width = defaultWidth, float height = defaultHeight*2);
    
    
    /// Construct and add elements
    /// \brief Construct and add elements
    /// \param dropDownOptions  std:vector with values of the same type as the ofParameter passed. Dropdown will be populated with this values.
    /// @param param The passed ofParameter will be "linked" to the dropdown and will change as you select different elements from the dropdown
    /// The name of this dropdown is taken from the passed ofParameter's name
    /// @param width = the width of the drawn element. Defaults to ofxGui's default
    /// @param height = the height of the drawn element. Defaults to twice of ofxGui's default. This is because one line is used to draw the dropdown name and the second for the selected value it has, if any.
    /// This is useful when for instance you have an ofParameter<int> but you want to display different names for each value, rather than the value itself.
    /// You still can add more values with the add(...) function
	ofxDropdown_(ofParameter<T> param, const std::vector<T> & dropDownOptions, float width = defaultWidth, float height = defaultHeight*2);
	
    
    //---- Setup function.
    // Only call setup if the default constructor was used.
    /// \brief Setup this dropdonw
    /// Dropdown will be empty. You should add values with the add(...) function
    /// @param param The passed ofParameter will be "linked" to the dropdown and will change as you select different elements from the dropdown
    /// The name of this dropdown is taken from the passed ofParameter's name
    /// @param width = the width of the drawn element. Defaults to ofxGui's default
    /// @param height = the height of the drawn element. Defaults to twice of ofxGui's default. This is because one line is used to draw the dropdown name and the second for the selected value it has, if any.
	ofxDropdown_ * setup(ofParameter<T> param, float width = defaultWidth, float height = defaultHeight*2);
	
    /// \brief Setup this dropdonw
    /// \warning use only when you instanced this classs and used its default constructor.
    /// @param name the name of the dropdown.
    /// @param width = the width of the drawn element. Defaults to ofxGui's default
    /// @param height = the height of the drawn element. Defaults to twice of ofxGui's default. This is because one line is used to draw the dropdown name and the second for the selected value it has, if any.
    /// Dropdown will be empty. you should add values with the add(...) function
    ofxDropdown_ * setup(std::string name, float width = defaultWidth, float height = defaultHeight*2);
	
    
    // ---------------------------------------------------
    // ----------------- Collapse on Selection Option
    // ---------------------------------------------------
	
    /// \brief The dropdown will collapse (close) when you choose an element.
    /// \param bPropagateToChildren recursivelly call this function on the dropdown's children
	void enableCollapseOnSelection(bool bPropagateToChildren = true);
    
    /// \brief The dropdown will NOT collapse (close) when you choose an element.
    /// \param bPropagateToChildren recursivelly call this function on the dropdown's children
	void disableCollapseOnSelection(bool bPropagateToChildren = true);
    
    /// \brief Will the dropdown collapse (close)  or not when you choose an element?
    /// \return bool. true if it will collapse, false otherwise.
    /// \param bPropagateToChildren recursivelly call this function on the dropdown's children
	bool isEnabledCollapseOnSelection();
    
    /// \brief get the ofParameter that indicates if collapse on selection is enabled or not.
    /// Use this to add this option to a GUI.
	ofParameter<bool> & getCollapseOnSelectionParameter();
	
    
    // ---------------------------------------------------
    // ----------------- Multiple Selection Option
    // ---------------------------------------------------
    
    /// \brief Enable Multiple Items selection. This is mutually exclusive with collapse on selection
    /// \param bPropagateToChildren recursivelly call this function on the dropdown's children
	void enableMultipleSelection(bool bPropagateToChildren = true);
    
    /// \brief Disable Multiple Items selection. This is mutually exclusive with collapse on selection
    /// \param bPropagateToChildren recursivelly call this function on the dropdown's children
    void disableMultipleSelection(bool bPropagateToChildren = true);
    
    /// \brief get if multiple selection is enabled.
    /// \return true if multiple selection is enabled, false otherwise
	bool isEnabledMultipleSelection();
    
    /// \brief get the ofParameter that indicates if multiple selection is enabled or not.
    /// Use this to add this option to a GUI.
	ofParameter<bool> & getMultiSelectionParameter();
		

    // ---------------------------------------------------
    // ----------------- Adding/creating dropdown elements
    // ---------------------------------------------------
    
    /// \brief add a dropdown option.
    /// This will create a new dropdown element which is owend by the dropdown.
    /// @param value the value that the this new option will have
    /// @param option The string  that will be shown when selecting this value
    /// \return pointer to the newly created dropdown element
    ofxDropdown_ * add(const T& value, const string& option);

    /// \brief add a dropdown option.
    /// This will create a new dropdown element which is owend by the dropdown.
    /// @param value the value that the this new option will have.
    /// The string shown in the dropdown will be the automatic string representation of this value
    /// \return pointer to the newly created dropdown element
    ofxDropdown_ * add(const T& value);
    
    /// \brief add several dropdown option at once.
    /// This will create new dropdown elements which will be owend by the dropdown.
    /// @param options a vector with several options.
    /// The string shown in the dropdown will be the automatic string representation of this value
    /// \return pointer to the newly created dropdown element
    ofxDropdown_ * add(const std::vector<T> & options);
    
    /// \brief add several dropdown option at once.
    /// This will create new dropdown elements which will be owend by the dropdown.
    /// @param options a map containing the values and their string representations to be added.
    /// This is the same as calling  add(const T& value, const string& option) for each of the map's values.
    /// \return pointer to the newly created dropdown element
    ofxDropdown_ * add(const std::map<T, std::string> & options);
    
    /// \brief Adds an existing dropdown to this one.
    /// \warning the added dropdown will NOT be owned by this dropdown
    /// This will "nest" the new dropdown inside the current one.
    /// @param dd the existing dropdown to be added passed by reference.
    /// \return pointer to the added dropdown
    ofxDropdown_ * addDropdown(ofxDropdown_& dd);
    
    /// \brief Adds an existing dropdown to this one.
    /// \warning the added dropdown will NOT be owned by this dropdown
    /// This will "nest" the new dropdown inside the current one.
    /// @param dd the existing dropdown to be added passed as a pointer.
    /// \return pointer to the added dropdown
    ofxDropdown_ * addDropdown(ofxDropdown_* dd);
    
    /// \brief Creates a new dropdown within this one
    /// This will create a new dropdown which will be owend by this dropdown.
    /// The new dropdown will be "nested" inside the this one.
    /// @param name The name of the new dropdown.
    /// \return pointer to the added dropdown
    ofxDropdown_ * newDropdown(std::string name);
    
    /// \brief Creates a new dropdown within this one.
    /// This will create a new dropdown which will be owend by this dropdown.
    /// The new dropdown will be "nested" inside the this one.
    /// @param param The ofParameter that will be linked to the newly created dropdown.
    /// \return pointer to the added dropdown
	ofxDropdown_ * newDropdown(ofParameter<T> param);
	
    
    
    /// \brief populate this dropdown with the files from a directory
    /// @param dirpath The path to the directory used for populating
    /// \param allowedExtensions (optional) std::vector<string> with the extensions of files you want to be shown in the dropdown
    /// \param bRecursive Make a recursive search within the folder. Meaning it will add the whole folder structure, folders within folders, etc
    /// \return true if it was capable to populate the dropdown, false otherwise
    bool populateFromDirectory(const string& dirpath, const vector<string>& allowedExtensions = {}, bool bRecursive = true)
    {
        ofFile d(dirpath);
        if(!d.isDirectory() || !d.exists())
        {
            return false;
        }
        
        addFromDir(this, dirpath, allowedExtensions, bRecursive);
        disableMultipleSelection();
        enableCollapseOnSelection();
        
        return true;
    }
    
		
    /// \brief clear all the elements from this dropdown.
    /// \warning the correct destruction of dropdown elements will happen ONLY for the ones  owned by this dropdown will be carried out.
	void clear();
	
	
	
	
	
    // ---------------------------------------------------
    // ----------------- Events and listeners
    // ---------------------------------------------------
    
    /// \brief Helper function to add a listenerr that gets called whenever the dropdown selection changes
    /// \param listener pointer to the object that is listening
    /// \param method pointer to the listener's method to be called
	template<class ListenerClass, typename ListenerMethod>
	void addListener(ListenerClass * listener, ListenerMethod method){
		selectedValue.addListener(listener,method);
	}
	
    /// \brief Helper function to remove a previously added listener.
    ///  The parameters passed to this function must be exactly the same as the ones passed to addListener in order to successfuly remove the listener
    /// \param listener pointer to the object that is listening
    /// \param method pointer to the listener's method to be called
	template<class ListenerClass, typename ListenerMethod>
	void removeListener(ListenerClass * listener, ListenerMethod method){
		selectedValue.removeListener(listener,method);
	}

    
    /// \brief ofEvent called whenever the dropdown selection changes. The data passed along with itis the displayed name of the option that changed
	ofEvent<std::string> change_E;
    /// \brief ofEvent called whenever the dropdown selection changes. The data passed along with it is the of the value that changed (not necesarily what is shown in the dropdown)
    ofEvent<T> change_E_value;
    
    /// \brief ofEvent called whenever the dropdown gets hidden. The data passed along with it is the name of the dropdown.
	ofEvent<std::string> dropdownHidden_E;
    /// \brief ofEvent called immediately BEFORE a dropdown will show. This is useful to call for whatever update needs to be done before the dropdown starts drawing itself.
    ofEvent<void> dropdownWillShow_E;
	
    
    // ---------------------------------------------------
    // ----------------- ofParameters
    // ---------------------------------------------------
    ///\brief this is just a needed function, as it overrides a virtual abstract base function.
    ///\return the selectedValue ofParam, but as it is an ofAbstractParam you will need to cast it properly in order to make it useful.
    ///As such, it is way much easier to simply acces selectedValue directly.
    virtual ofAbstractParameter & getParameter() override;

    ///\brief get the parameter group for the dropdown paramaters. Use this if you want to add to a gui these parameters.
    ///\return ofParameterGroup with the dropdown params.
    ofParameterGroup& getDropdownParameters();
	
	
    
    ///\brief selected value parameter. This is the one that actually changes when a selection in the dropdown is made.
	ofParameter<T> selectedValue;
	
	
    
    // ---------------------------------------------------
    // ----------------- Show / Hide / Draw
    // ---------------------------------------------------
    /// \brief Show this dropdown.
    /// \param bDisableSiblings if true the siblings of this dropdown will be disabled. This is the default. The idea is to avoid unwanted mouse interactions
    void showDropdown(bool bDisableSiblings = true);
    
    /// \brief Hide this dropdown
    /// \param bNotifyEvent if true (default) dropdownHidden_E will be notified.
    void hideDropdown(bool bNotifyEvent = true);
    
    /// \brief get if the dropdown is being shown
    /// \return true if showind the dropdown, false otherwise
    bool isShowingDropdown();
    
    ///\brief Force the redrawing of this gui widget. This automatically happens by itself but there are some edge case where you might want to trigger it.
    /// Don't call this function on every frame or very often as you will be using computational resources unnecesarily.
	void forceRedraw(){
		setNeedsRedraw();
	}
    
    ///\brief Enumeration of the possible positions where you can make the dropdown to show.
    ///These are relative to the gui that contains the dropdown, so if you set it to DD_RIGHT, the dropdown will show at the right of the container gui.
    enum DropDownPosition{
        DD_AUTO = 0,
        DD_RIGHT,
        DD_BELOW,
        DD_ABOVE,
        DD_LEFT
    };
    
    ///\brief set the dropdown position relative to the GUI group that contains it.
    ///\param pos (DropDownPosition) one of the values of the DropDownPosition  enumeration.
    void setDropDownPosition(DropDownPosition pos);
    
    ///\brief move the dropdown by an offset of its default position.
    ///This is very useful for scrolling a dropdown.
    void move(const glm::vec3& offset);
    
    ///\brief Set the width of the dropdown elements.
    ///This automatically changes to fit the values of the dropdown, but you can changing to something else if you wish.
    ///\param width the new width to set in pixels.
    void setDropdownElementsWidth(float width);
    
    ///\brief enables drawing a translucent black rectangle over everything but the dropdown.
    /// Useful on  touch devices.
    void enableDarkenedBackground();
    ///\brief disables drawing a translucent black rectangle over everything but the dropdown.
    void disableDarkenedBackground();
    ///\brief returns if it has enabled drawing a translucent black rectangle over everything but the dropdown.
    ///\return true if it is enable false if not
    bool isDarkenedBackgroundEnabled();
    
    ///\brief enables showing the dropdown when the mouse is over without having to clic.
    static void enableShowWhenOver();
    ///\brief disables showing the dropdown when the mouse is over without having to clic. This is the default
    static void disableShowWhenOver();
    ///\brief get if showing when over is enabled.
    ///\return true when enabled, false otherwise.
    static bool isShowWhenOverEnabled();
    
    // ---------------------------------------------------
    // ----------------- Selection
    // ---------------------------------------------------
	void setSelectedValueByName( const std::string& valueName, bool bNotify);
	void setSelectedValueByIndex( const size_t& index, bool bNotify);

	virtual void deselect() override;

    std::string getOptionAt(size_t index);
    
    // ---------------------------------------------------
    // ----------------- Children
    // ---------------------------------------------------
	
    std::vector<ofxDropdown_ *>& getChildDropdowns(){return childDropdowns;}
    std::vector<std::unique_ptr<ofxDropdown_>>& getOwnedDropdowns(){return ownedDropdowns;}
    std::vector<std::unique_ptr<ofxDropdownOption>>& getOwnedChildren(){return ownedChildren;}
    
    ofxGuiGroup& getChildGuiGroup(){ return group;}
    const ofxGuiGroup& getChildGuiGroup() const{ return group;}
    
    // ---------------------------------------------------
    // ----------------- Overriden mouse callbacks. Dont call these manually
    // ---------------------------------------------------
    
    virtual bool mouseReleased(ofMouseEventArgs & args) override;
    virtual bool mousePressed(ofMouseEventArgs & args) override;
    virtual bool mouseMoved(ofMouseEventArgs & args) override;
    virtual bool mouseDragged(ofMouseEventArgs & args) override;
    virtual bool mouseScrolled(ofMouseEventArgs & args) override;
    
    void registerMouseEvents() override;
    void unregisterMouseEvents() override;

    
    // ---------------------------------------------------
    // ----------------- getters
    // ---------------------------------------------------
    ///\brief returns the options for this dropdown.
    ///This does not include nested dropdowns.
    ///\returns an std::vector<std::string> with the available options in the dropdown. Each of these strings are what you see in the dropdown.
    
    const vector<string> & getOptions(){return options;}
    
    ///\brief returns the values for this dropdown.
    ///This does not include nested dropdowns.
    ///\returns an std::vector<T> with the value that is mapped to an option. Values and options share their index.
    const vector<T> & getValues(){return values;}
    
    ///\brief returns the number of options or valuesthis dropdown.
    ///This does not include nested dropdowns.
    ///\returns std::size_t with the amount of options or values.
    size_t getNumOptions(){return options.size(); }
    
    ///\brief Returns the option gui using the option's  name
    ///\param name the option name you want to get
    ///\returns an ofxDropdownOption pointer
    ofxDropdownOption* getOptionByName(const string& name);
    
    ///\brief Returns the option gui using the option's  value
    ///\param value the option value you want to get
    ///\returns an ofxDropdownOption pointer
    ofxDropdownOption* getOptionByValue(const T& value);
    
    ///\brief Returns the option gui using the option's index
    ///\param index the option's index you want to get
    ///be careful with this as the index only takes into accout the options, but not any nested dropdown there might be. It is safer to get by name or value.
    ///\returns an ofxDropdownOption pointer
    ofxDropdownOption* getOptionByIndex(const size_t& index);

    ///\brief returns all the selected values when multiselection is enabled
    ///\return a vector with all the selected values
    const vector<T>&  getAllSelected();
    
    
    ///\Checks if an element exists with the passed name
    ///\param name the name of the element to check
    ///\returns true if the dropdown holds the element with the passed name
    bool containsName(const std::string& name) const;
       
    ///\Checks if an element exists with the passed value
    ///\param value  the value of the element to check
    ///\returns true if the dropdown holds the element with the passed value
    bool containsValue(const T& value) const;


    // ---------------------------------------------------
    // ----------------- setters
    // ---------------------------------------------------
    
    ///\brief Sets the name (the string that gets drawn in the gui) by passing the value of the option
    ///\param value the value of the option. This is used to search for the option
    ///\param newName the new name you want to give to the option
    ///\return boolean. True if it was able to change the name, false otherwise. False most probably will happen when the passed value is not found as part of the dropdown
    bool setOptionNameByValue(const T& value, const string& newName);
    
    
    ///\brief Sets the name (the string that gets drawn in the gui) by passing the value of the option
    ///\param index the index (position) of the option. index 0 is the first option shown.
    ///\param newName the new name you want to give to the option
    ///\return boolean. True if it was able to change the name, false otherwise. False most probably will happen when the passed index is out of bounds
    bool setOptionNameByIndex(const size_t& index, const string& newName);
    
    
    ///\brief Sets the name (the string that gets drawn in the gui) by passing the name of the option
    ///\param currentName the current name of the option you want to update
    ///\param newName the new name you want to give to the option
    ///\return boolean. True if it was able to change the name, false otherwise. False most probably will happen when the passed name is not found as part of the dropdown
    bool updateOptionName(const string& currentName, const string& newName);
    
    
    // ---------------------------------------------------
    // ----------------- select on mouse release
    // ---------------------------------------------------
    ///\brief enables or disables select on mouse release.
    ///This is mostly useful when using this addon on an iPad or touch device, as it feels more natural
    ///This feature is disabled by default
    ///\param bEnable boolean when true enables this feature, on false it disables it
    void setEnableSelectOnMouseRelease(bool bEnable);
    
    ///\brief enables select on mouse release.
    /// This is mostly useful when using this addon on an iPad or touch device, as it feels more natural
    void enableSelectOnMouseRelease();
    
    ///\brief disables select on mouse release.
    void disableSelectOnMouseRelease();
    ///\brief check if the select on mouse release feature is either enabled or disabled;
    ///\return boolean true if enable, false disabled
    bool isEnabledSelectOnMouseRelease();
    
    
#ifdef USE_OFX_GUI_TOOLTIP

    // ---------------------------------------------------
    // ----------------- Tooltips
    // ---------------------------------------------------
    
    ///\brief set tooltips for this dropdown.
    ///If there is no tooltip data for any of the tooltip options, including nested dropdowns
    ///the json object will get populated with empty strings so it is easier to fill out.
    ///Make sure to save the json back to disk in order to save this auto generated json.
    ///\param json the json object containing the tooltip data
    virtual void setupTooltip(ofJson & json) override;
    
    
    ///\ reset all tooltips. This works recursively with any nested dropdown
    virtual void resetTooltips()override;
    
    
    ///\brief Add a tooltip for the passed value
    ///\param value The value for which you want to add tthe tooltip
    void addTooltip(T value, const string& text);
    
    ///\brief Add a tooltip for the passed value
    ///\param option The name for the option for which you want to add tthe tooltip
    void addTooltip(const string& option, const string& text);
    
    
    ///\brief Enable tooltips. This works recursively with any nested dropdown
    virtual void enableTooltip() override;
    ///\brief Disable tooltips. This works recursively with any nested dropdown
    virtual void disableTooltip() override;
    
    
    ///\brief Draw the tooltips.
    ///This needs to be called independently and after the dropdown and gui are drawn,
    ///otherwise the tooltips might get occluded by the gui.
    virtual void drawTooltip() override;
#endif
    
    
protected:
	
    ofEventListeners paramsListeners;
    
    void collapseOnSelectionChanged(bool&);
    void multiSelectionChanged(bool&);
    
	ofParameter<bool> bCollapseOnSelection = { "Collapse On Selection", false};
	ofParameter<bool> bMultiselection = { "Multi Selection", true};
	ofParameter<bool> bSetChildrenCollapseOnSelection = {"Children Collapse On Selection", true};
    ofParameter<bool> bSetChildrenMultiSelection = {"Children Multi Selection", true};
    static bool bShowOnOver;
    
    
    ofParameter<bool> bDrawDarkBg = {"Darken background when showing", false};
    
    ofParameterGroup dropdownParams;
    
    ofParameterGroup groupParams;
	
	virtual bool setValue(float mx, float my, bool bCheck) override;
	
	void disableElement(ofxDropdownOption* e, bool bCheckAgainstThis = false);
	
	int myMouseEventsPriority;
	
	void disableSiblings(ofxBaseGui* parent, ofxBaseGui* child);
	
    DropDownPosition dropDownPosition = DD_RIGHT;
    
    void _setGroupAutoPosition();
	
	virtual void generateDraw() override;
		
	virtual void render() override;

	vector<string> options;
    vector<T> values;
	
	
	void groupChanged( const void * sender,bool&);
	bool bGroupEnabled = false;
	
	void buttonClicked(bool &);
	
	void childDropdownHidden(const void * sender, std::string&);
	
	void selectedValueChanged(T & newvalue);
	
    
	ofxGuiGroup group;
	ofEventListeners groupListeners;
	ofEventListeners childDropdownListeners;
    ofEventListeners mouseListeners;
	
	ofEventListener buttonListener;
    ofEventListener setlectedValueListener;
    
    ofEventListeners guiGroupListeners;
    
    void enableGuiGroupMouseAndDraw();
    void disableGuiGroupMouseAndDraw();
    
    void drawGuiGroup(ofEventArgs&);
    bool scrollGroup(ofMouseEventArgs&);
    bool groupMousePressed(ofMouseEventArgs & args);
    bool groupMouseReleased(ofMouseEventArgs & args);
    bool groupMouseDragged(ofMouseEventArgs & args);
    
    
	ofPath arrow;
	ofVboMesh optionTextMesh;
	
	std::string selectedOption;
	
	void addFromDir(ofxDropdown_<T>* currentDD, const string& dirpath, const vector<string>& allowedExtensions, bool bRecursive);
	
	void _updateGroupWidth();
	
    
    vector<T> allSelectedValues;
    
    void _setDropdownOptionValue(ofxDropdownOption* o, const T& val);
    T _getDropdownOptionValue(ofxDropdownOption* o);
    
    
    void updateSelectedValue();

    void _setAllSelectedValues();

    void setDeselectedValue();
    
private:
	
	std::vector<ofxDropdown_ *> childDropdowns;
	std::vector<std::unique_ptr<ofxDropdown_>> ownedDropdowns;
	std::vector<std::unique_ptr<ofxDropdownOption>> ownedChildren;
    
    bool bRegisteredForMouseEvents = false;
    bool bIsSetup = false;
    bool _bSelectOnMouseRelease = false;
};






typedef ofxDropdown_<string> ofxDropdown;
typedef ofxDropdown_<int> ofxIntDropdown;
#ifndef TARGET_WIN32
typedef ofxDropdown_<ofFile> ofxDirDropdown;
#else
typedef ofxDropdown_<string> ofxDirDropdown;
#endif
