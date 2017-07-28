class TypeRuleProcessor
	: RuleProcessor
{
%TypeHeaderCode
	#include <BALL/MOLMEC/COMMON/typeRuleProcessor.h>
%End
	public:

	TypeRuleProcessor();
	TypeRuleProcessor(INIFile&, const String&);
	TypeRuleProcessor(const TypeRuleProcessor&);
	~TypeRuleProcessor();

	Processor::Result operator () (Atom&);
%MethodCode
	sipRes = sipCpp->operator () (*a0);
%End
};
