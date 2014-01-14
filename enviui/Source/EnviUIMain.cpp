#include "EnviUIMain.h"
#include "EnviTabSummary.h"

Typeface::Ptr EnviUILookAndFeel::getTypefaceForFont (const Font &font)
{
	_DBG("EnviUILookAndFeel::getTypefaceForFont");
	return (Typeface::createSystemTypefaceFor (BinaryData::DejaVuSans_ttf, BinaryData::DejaVuSans_ttfSize));
}

EnviUIMain::EnviUIMain() : uiStatus(UIConnecting)
{
	LocalisedStrings::setCurrentMappings (new LocalisedStrings (String::fromUTF8(BinaryData::EnviTranslations_txt, BinaryData::EnviTranslations_txtSize),true));
	LookAndFeel::setDefaultLookAndFeel (enviUILookAndFeel = new EnviUILookAndFeel());

	addAndMakeVisible (enviTabs = new TabbedComponent (TabbedButtonBar::TabsAtTop));
	enviTabs->setTabBarDepth (18);
	enviTabs->addTab ("Summ", Colours::lightgrey, new EnviTabSummary(*this), true, 0);
	enviTabs->addTab ("Solr", Colours::lightgrey, nullptr, false, 1);
	enviTabs->addTab ("Batt", Colours::lightgrey, nullptr, false, 2);
	enviTabs->addTab ("Envi", Colours::lightgrey, nullptr, false, 3);
	enviTabs->addTab ("Hist", Colours::lightgrey, nullptr, false, 4);
	enviTabs->addTab ("Misc", Colours::lightgrey, nullptr, false, 5);
	enviTabs->addTab ("Opts", Colours::lightgrey, nullptr, false, 6);

	enviIPCClient 	= new EnviIPCClient(*this);
	addAndMakeVisible (enviUIStatus	= new EnviUIStatus(*this));

	setSize (200, 320);
}

EnviUIMain::~EnviUIMain()
{
	LookAndFeel::setDefaultLookAndFeel (nullptr);
	deleteAndZero (enviUILookAndFeel);
}

void EnviUIMain::paint (Graphics& g)
{
	g.fillAll (Colours::grey);
}

void EnviUIMain::resized()
{
	enviUIStatus->setBounds (0,0,getWidth(),getHeight());
	enviTabs->setBounds (0,0,getWidth(),getHeight());
}

const EnviUIMain::UIStatus EnviUIMain::getStatus() const
{
	return (uiStatus);
}

void EnviUIMain::setStatus(const UIStatus newStatus)
{
	uiStatus = newStatus;
}
