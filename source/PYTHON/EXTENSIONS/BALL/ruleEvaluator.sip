class RuleEvaluator
{
%TypeHeaderCode
	#include <BALL/MOLMEC/COMMON/ruleEvaluator.h>
%End

	public:
	RuleEvaluator() throw();
	RuleEvaluator(INIFile&, const String&) throw();
	RuleEvaluator(const RuleEvaluator&) throw();
	~RuleEvaluator() throw();

	bool initialize(INIFile&, const String&) throw();
	const String& getPrefix() const throw();
	void setPrefix(const String&) throw();
	virtual void clear() throw();
	String operator () (const Atom&) const;
%MethodCode
	sipRes = new String(sipCpp->operator () (*a0));
%End

	bool operator == (const RuleEvaluator&) const;
	bool isValid() const throw();
};
