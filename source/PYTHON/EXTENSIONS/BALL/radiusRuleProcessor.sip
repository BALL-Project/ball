class RadiusRuleProcessor
	: RuleProcessor
{
%TypeHeaderCode
	#include <BALL/MOLMEC/COMMON/radiusRuleProcessor.h>
%End
	public:
	RadiusRuleProcessor();
	RadiusRuleProcessor(INIFile&, const String& prefix = "RadiusRules");
	RadiusRuleProcessor(const RadiusRuleProcessor&);
	~RadiusRuleProcessor();
	Processor::Result operator () (Atom&);
%MethodCode
	sipRes = sipCpp->operator () (*a0);
%End
};
