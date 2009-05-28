// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id$
// general parameter file class
//
	class Parameters
	{
%TypeHeaderCode
	#include <BALL/FORMAT/parameters.h>
	using namespace BALL;
%End
		public:
		Parameters();
		Parameters(const String& /filename/);
		Parameters(const Parameters& /force_field_parameter/);
		virtual ~Parameters();
		virtual void clear() throw();

		// const Parameters& operator = (const Parameters& parameters);
		void setFilename(const String& /filename/);
		const String& getFilename() const;
		INIFile& getParameterFile();
		bool init();
		virtual bool isValid() const;
		bool operator == (const Parameters& /parameters/) const;
	};
