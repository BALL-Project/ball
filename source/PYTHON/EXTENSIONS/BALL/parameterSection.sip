class ParameterSection
{
%TypeHeaderCode
	#include <BALL/FORMAT/parameterSection.h>
%End
	public:
	static const String UNDEFINED;
	enum { MAX_FIELDS = 20};

	ParameterSection() throw();
	ParameterSection(const ParameterSection&) throw();
	virtual ~ParameterSection() throw();

	bool extractSection(Parameters&, const String&)	throw();
	const String& getSectionName() const throw();
	const String& getValue(const String&, const String&) const	throw();
	bool has(const String&, const String&) const throw();
	bool has(const String&) const throw();
	bool hasVariable(const String&) const throw();
	Position getColumnIndex(const String&) const throw();
	Size getNumberOfVariables() const throw();
	Size getNumberOfKeys() const throw();
	const String& getValue(Position, Position)	const throw();
	const String& getKey(Position) const throw();

	virtual void clear() throw();
	virtual bool isValid() const throw();
	bool operator == (const ParameterSection&) const throw();

	Options	options;
};
