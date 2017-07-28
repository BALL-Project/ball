class RuleProcessor
	: AtomProcessor
{
%TypeHeaderCode
	#include <BALL/PYTHON/pyUnaryProcessors.h>
	#include <BALL/MOLMEC/COMMON/ruleProcessor.h>
	using namespace BALL;
%End
	public:
	RuleProcessor();
	RuleProcessor(INIFile&, const String&);
	RuleProcessor(const RuleProcessor&);
	~RuleProcessor();
	void clear();
	void destroy();   
	bool initialize(INIFile&, const String&);
	void set(const RuleProcessor&);
	virtual bool start();
	virtual bool finish();
	Processor::Result operator () (Atom&);
%MethodCode
	sipRes = sipCpp->operator () (*a0);
%End

	String evaluate(const Atom&);
	bool isValid() const;
};
