/*
  ==============================================================================

    EnviDataSource.h
    Created: 2 Dec 2013 1:40:26pm
    Author:  rk09631

  ==============================================================================
*/

#ifndef ENVIDATASOURCE_H_INCLUDED
#define ENVIDATASOURCE_H_INCLUDED

#include "EnviIncludes.h"

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
		void setValue (const unsigned int valueIndex, const var value);
		void setType (const Identifier _type);
		const int addValue (const String &valueName, const Unit unit);
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
		ValueTree getConfig() const;
		const Result startSource();
		void stopSource();
		void setSourceStorageId(const int64 sourceStorageId);
		const int64 getSourceStorageId() const ;
		void setDisabled(const bool shouldBeDisabled);
		bool isDisabled() const;
		const double evaluateExpression (const double inputData, const String &valueName);
		const Result setValueExpression (const String &valueName, const String &expressionString);
		const Result setAllExpressions();
		const Result evaluateAllExpressions(Array <double> inputData);
		const bool hasExpression(const String &valueName);
		virtual const int getDataCacheSize();
		const int getNumValues() const;
		const String getValueName(const int valueIndex) const;
		const Unit getValueUnit(const int valueIndex) const;
		void setValueStorageId(const int valueIndex, const int64 storageId);
		var getHistory();
		var &getResultRef();
		var getResult() const;
		JUCE_LEAK_DETECTOR(EnviDataSource);

	protected:
		ValueTree instanceConfig;
		EnviApplication &owner;
		CriticalSection dataSourceLock;
		HashMap <String,Expression> valueExpressions;

	private:
		void setValueProperty(const int valueIndex, const Identifier &propertyName, const var &propertyValue);
		const var getValueProperty(const int valueIndex, const Identifier &propertyName) const;
		var info;
		var values;
		var history;
		Time startTime, endTime;
		bool disabled;
		int instanceNumber;
		int dataCacheSize;
		EnviExpScope enviExpScope;
};
#endif  // ENVIDATASOURCE_H_INCLUDED
