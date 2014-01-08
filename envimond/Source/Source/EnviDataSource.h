/*
  ==============================================================================

    EnviDataSource.h
    Created: 2 Dec 2013 1:40:26pm
    Author:  rk09631

  ==============================================================================
*/

#ifndef ENVIDATASOURCE_H_INCLUDED
#define ENVIDATASOURCE_H_INCLUDED

#include "EnviData.h"

#define SHOULD_WE_EXIT()	if (threadShouldExit()) { _INF(getType()+" ["+getName()+"] thread should exit"); return; }

class EnviDataSource : public ChangeBroadcaster
{
	public:
		class EnviExpScope : public Expression::Scope
		{
			public:
				EnviExpScope(EnviDataSource &_owner);
				String getScopeUID() const;
				Expression getSymbolValue(const String &symbol) const;
				double evaluateFunction (const String &functionName, const double *parameters, int numParameters) const;
				EnviExpScope &setData (const double _inputData);
			private:
				EnviDataSource &owner;
				double inputData;
		};

		EnviDataSource(EnviApplication &_owner, const Identifier &_type);
		virtual ~EnviDataSource() {}
		virtual const Result execute() 		= 0;
		virtual const Result initialize(const ValueTree _instanceConfig);

		void collectFinished(const Result collectStatus);
		void setValues (const bool finishCollectNow, const Result collectStatus, const var value0);
		void setValues (const bool finishCollectNow, const Result collectStatus, const var value0, const var value1);
		void setValues (const bool finishCollectNow, const Result collectStatus, const var value0, const var value1, const var value2);
		void setValue (const unsigned int valueIndex, const var value);
		void setType (const Identifier _type);
		const int addValue (const String &valueName, const EnviData::Unit unit);
		void copyValues (const EnviData &dataToCopyFrom);
		const var getProperty (const Identifier &identifier) const;
		const var getProperty (const Identifier &identifier, const var defaultReturnValue) const;
		void setProperty (const Identifier identifier, const var &value);
		const int getInterval() const;
		const int getInstanceNumber() const;
		void setInstanceNumber(const int instanceNumber);
		const String getName() const;
		void setName(const String &name);
		const String getType() const;
		const int getTimeout() const;
		const EnviData getResult() const;
		void setResult (const EnviData &_result);
		ValueTree getConfig() const;
		const Result startSource();
		void stopSource();
		void setIndex(const int64 sourceIndex);
		const int64 getIndex() const ;
		void setDisabled(const bool shouldBeDisabled);
		bool isDisabled() const;
		const double evaluateExpression (const double inputData, const String &valueName);
		const Result setValueExpression (const String &valueName, const String &expressionString);
		const Result setAllExpressions();
		const Result evaluateAllExpressions(Array <double> inputData);
		const bool hasExpression(const String &valueName);
		virtual const int getDataCacheSize();
		virtual const var getSummary();
		Array <EnviData> getHistory();

		JUCE_LEAK_DETECTOR(EnviDataSource);

	protected:
		ValueTree instanceConfig;
		EnviApplication &owner;
		CriticalSection dataSourceLock;
		HashMap <String,Expression> valueExpressions;
		Array <EnviData> history;

	private:
		EnviData result;
		Time startTime, endTime;
		bool disabled;
		int instanceNumber;
		int dataCacheSize;
		EnviExpScope enviExpScope;
};
#endif  // ENVIDATASOURCE_H_INCLUDED
