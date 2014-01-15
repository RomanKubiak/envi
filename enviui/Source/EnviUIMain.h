#ifndef ENVIUIMAIN_H
#define ENVIUIMAIN_H

#include "EnviUIIncludes.h"
#include "EnviIPCClient.h"
#include "EnviUIStatus.h"

class EnviUILookAndFeel : public LookAndFeel_V3
{
	public:
		Typeface::Ptr getTypefaceForFont (const Font &font);
};

class EnviUIMain   : public Component
{
	public:
		enum UIStatus
		{
			UIConnected,
			UIConnecting,
			UIError,
			UINormal
		};

		EnviUIMain();
		~EnviUIMain();
		void paint (Graphics&);
		void resized();
		void moved();
		const UIStatus getStatus() const;
		void setStatus(const UIStatus newStatus);

		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (EnviUIMain)

	private:
		UIStatus uiStatus;
		EnviUILookAndFeel *enviUILookAndFeel;
		ScopedPointer <TabbedComponent> enviTabs;
		ScopedPointer <EnviIPCClient> enviIPCClient;
		ScopedPointer <EnviUIStatus> enviUIStatus;

};


#endif
