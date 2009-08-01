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
		ParameterSection(const ParameterSection& /parameter_section/) throw();
		virtual ~ParameterSection() throw();

		bool extractSection(Parameters& /parameters/, const String& /section_name/)	throw();
		const String& getSectionName() const throw();
		const String& getValue(const String& /key/, const String& /variable/) const	throw();
		bool has(const String& /key/, const String& /variable/) const throw();
		bool has(const String& /key/) const throw();
		bool hasVariable(const String& /variable/) const throw();
		Position getColumnIndex(const String& /variable/) const throw();
		Size getNumberOfVariables() const throw();
		Size getNumberOfKeys() const throw();
		const String& getValue(Position /key_index/, Position /variable_index/)	const throw();
		const String& getKey(Position /key_index/) const throw();

		virtual void clear() throw();
		// const ParameterSection& operator = (const ParameterSection& section) throw();
		virtual bool isValid() const throw();
		bool operator == (const ParameterSection& /parameter_section/) const throw();

		Options	options;
	};
