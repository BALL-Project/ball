// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

class ShiftModel 
		: ShiftModule		
{
%TypeHeaderCode
		#include <BALL/NMR/shiftModel.h>
		#include <BALL/NMR/shiftModule.h>
		using namespace BALL;
		using namespace BALL::Exception;
%End
	public:	

		static const char* MODULE_LIST_SECTION;
		ShiftModel() throw();

		ShiftModel(String filename) throw();
//		ShiftModel(const String& filename) throw();
		ShiftModel(const ShiftModel& model) throw();

		virtual ~ShiftModel() throw();

		void clear() throw();

		Parameters& getParameters() throw();

		//		ModuleList& getModuleList() throw();

		void setFilename(const String& filename) throw(FileNotFound);

		const String& getFilename() const throw();

//		void registerModule(const String& name, CreateMethod method) throw(NullPointer);
//		void unregisterModule(const String& name) throw();

		bool isValid() const throw();
		bool isRegistered(const String& name) const throw();

//		const ShiftModel& operator = (const ShiftModel& model) throw();
//		const ShiftModel& operator = (const String& filename) throw();

		Options options;

		bool start() throw();
		bool finish() throw();

		Processor::Result operator () (Composite& composite) throw();
};
