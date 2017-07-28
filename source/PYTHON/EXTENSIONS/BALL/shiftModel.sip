class ShiftModel
	: ShiftModule
{
%TypeHeaderCode
	#include <BALL/NMR/shiftModel.h>
	#include <BALL/NMR/shiftModule.h>
%End
	public:

	static const char* MODULE_LIST_SECTION;
	ShiftModel() throw();
	ShiftModel(String filename) throw();
	ShiftModel(const ShiftModel& model) throw();
	virtual ~ShiftModel() throw();

	void clear() throw();
	Parameters& getParameters() throw();
	void setFilename(const String& filename) throw(FileNotFound);
	const String& getFilename() const throw();
	bool isValid() const throw();
	bool isRegistered(const String& name) const throw();
	Options options;
	bool start() throw();
	bool finish() throw();
	Processor::Result operator () (Composite& composite) throw();
};
