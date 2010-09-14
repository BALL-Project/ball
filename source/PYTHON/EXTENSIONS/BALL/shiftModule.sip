// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
class ShiftModule 
		: CompositeProcessor
{
%TypeHeaderCode
		#include <BALL/NMR/shiftModule.h>
		using namespace BALL;
		using namespace BALL::Exception;
		typedef UnaryProcessor<Composite> CompositeProcessor;
%End
		public:	

		static const char* PROPERTY__SHIFT;
		static const char* PROPERTY__EXPERIMENTAL__SHIFT;

		ShiftModule() throw();

		ShiftModule(Parameters& parameters, const String& name = "") throw();

		ShiftModule(const ShiftModule& module) throw();

		virtual ~ShiftModule() throw();
		virtual void clear() throw();

		// const ShiftModule& operator = (const ShiftModule& module) throw();

		void setName(const String& name) throw();
		const String& getName() const throw();

		void setParameters(Parameters& parameters) throw();
		const Parameters* getParameters() const throw();

		virtual void init() throw();

		virtual bool start() throw();

		virtual bool finish() throw();

		bool isValid() const throw();

};
