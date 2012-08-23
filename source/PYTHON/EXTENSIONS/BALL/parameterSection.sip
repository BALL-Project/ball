// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id$
//
%ModuleCode
	#include <BALL/FORMAT/parameterSection.h>
	using namespace BALL;
%End

	class ParameterSection 
	{
%TypeHeaderCode
	#include <BALL/FORMAT/parameterSection.h>
	using namespace BALL;
%End
		public:
		static const String UNDEFINED;
		enum { MAX_FIELDS = 20};

		ParameterSection() throw();
		ParameterSection(const ParameterSection&) throw();
		virtual ~ParameterSection() throw();

		bool extractSection(Parameters&, const String&)	throw();
		const String& getSectionName() const throw();
		const String& getValue(const String&, const String&) const	throw();
		bool has(const String&, const String&) const throw();
		bool has(const String&) const throw();
		bool hasVariable(const String&) const throw();
		Position getColumnIndex(const String&) const throw();
		Size getNumberOfVariables() const throw();
		Size getNumberOfKeys() const throw();
		const String& getValue(Position, Position)	const throw();
		const String& getKey(Position) const throw();

		virtual void clear() throw();
		// const ParameterSection& operator = (const ParameterSection& section) throw();
		virtual bool isValid() const throw();
		bool operator == (const ParameterSection&) const throw();

		Options	options;
	};
