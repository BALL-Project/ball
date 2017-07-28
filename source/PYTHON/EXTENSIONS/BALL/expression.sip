class ExpressionPredicate
	: UnaryAtomPredicate
{
%TypeHeaderCode
	#include <BALL/KERNEL/expression.h>
	using UnaryAtomPredicate = UnaryPredicate<Atom>;
%End
	public:
	ExpressionPredicate();
	ExpressionPredicate(const String&);
	~ExpressionPredicate() throw();
	bool operator () (const Atom&) const throw();
%MethodCode
	sipRes = sipCpp->operator () (*a0);
%End

	virtual void setArgument(const String&) throw();
};

class Expression
{
%TypeHeaderCode
	#include <BALL/KERNEL/expression.h>
%End
	public:
	Expression();
	Expression(const Expression&);
	Expression(const String&);
	~Expression() throw();
	bool hasPredicate(const String&);
	bool operator () (const Atom&) const throw();
%MethodCode
	sipRes = sipCpp->operator () (*a0);
%End

	ExpressionPredicate* getPredicate(const String&, const String& args = "") const;
	void setExpression(const String&);
	const String& getExpressionString() const;
};
