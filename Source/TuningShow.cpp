/*
  ==============================================================================

  This is an automatically generated GUI class created by the Projucer!

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Created with Projucer version: 5.4.7

  ------------------------------------------------------------------------------

  The Projucer is part of the JUCE library.
  Copyright (c) 2017 - ROLI Ltd.

  ==============================================================================
*/

//[Headers] You can add your own extra header files here...
//[/Headers]

#include "TuningShow.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
//[/MiscUserDefs]

//==============================================================================
TuningShow::TuningShow ()
{
    //[Constructor_pre] You can add your own custom stuff here..
    //[/Constructor_pre]

    tabbedComponent.reset (new TabbedComponent (TabbedButtonBar::TabsAtTop));
    addAndMakeVisible (tabbedComponent.get());
    tabbedComponent->setTabBarDepth (30);
    tabbedComponent->addTab (TRANS("Tuning"), Colours::lightgrey, new TableListBox(), true);
    tabbedComponent->addTab (TRANS("SCL"), Colours::lightgrey, new TextEditor(), true);
    tabbedComponent->addTab (TRANS("KBM"), Colours::lightgrey, new TextEditor(), true);
    tabbedComponent->setCurrentTabIndex (0);

    tabbedComponent->setBounds (0, 0, 500, 400);


    //[UserPreSize]
    //[/UserPreSize]

    setSize (500, 400);


    //[Constructor] You can add your own custom stuff here..
    sclt = dynamic_cast<TextEditor*>( tabbedComponent->getTabContentComponent(1) );
    kbmt = dynamic_cast<TextEditor*>( tabbedComponent->getTabContentComponent(2) );
    sclt->setReadOnly(true);
    kbmt->setReadOnly(true);

    table = dynamic_cast<TableListBox*>( tabbedComponent->getTabContentComponent(0) );

    //[/Constructor]
}

TuningShow::~TuningShow()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]

    tabbedComponent = nullptr;


    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void TuningShow::paint (Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    g.fillAll (Colour (0xff323e44));

    //[UserPaint] Add your own custom painting code here..
    //[/UserPaint]
}

void TuningShow::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    //[/UserPreResize]

    //[UserResized] Add your own custom resize handling here..
    //[/UserResized]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...
void TuningShow::setTuning( const Tunings::Tuning &t )
{
    tuning = t;
    if( sclt )
        sclt->setText( tuning.scale.rawText, false );
    if( kbmt )
        kbmt->setText( tuning.keyboardMapping.rawText, false );

    mod = std::make_unique<surgesynthteam::TuningTableListBoxModel>();
    mod->setTableListBox( table );
    mod->setupDefaultHeaders( table );
    mod->tuningUpdated( tuning );

    table->setModel( mod.get() );

    table->getViewport()->setScrollBarsShown(true,false);
    table->getViewport()->setViewPositionProportionately( 0.0,  60.0 / 127.0 );
}
//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Projucer information section --

    This is where the Projucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="TuningShow" componentName=""
                 parentClasses="public Component" constructorParams="" variableInitialisers=""
                 snapPixels="8" snapActive="1" snapShown="1" overlayOpacity="0.330"
                 fixedSize="1" initialWidth="500" initialHeight="400">
  <BACKGROUND backgroundColour="ff323e44"/>
  <TABBEDCOMPONENT name="new tabbed component" id="de33c460f07f5cdd" memberName="tabbedComponent"
                   virtualName="" explicitFocusOrder="0" pos="0 0 500 400" orientation="top"
                   tabBarDepth="30" initialTab="0">
    <TAB name="Tuning" colour="ffd3d3d3" useJucerComp="0" contentClassName="TableListBox"
         constructorParams="" jucerComponentFile=""/>
    <TAB name="SCL" colour="ffd3d3d3" useJucerComp="0" contentClassName="TextEditor"
         constructorParams="" jucerComponentFile=""/>
    <TAB name="KBM" colour="ffd3d3d3" useJucerComp="0" contentClassName="TextEditor"
         constructorParams="" jucerComponentFile=""/>
  </TABBEDCOMPONENT>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]

