class RegularExpression
{
%TypeHeaderCode
	#include <BALL/DATATYPE/regularExpression.h>
%End
	public:
	static const String ALPHA;
	static const String ALPHANUMERIC;
	static const String REAL;
	static const String IDENTIFIER;
	static const String INTEGER;
	static const String HEXADECIMAL_INTEGER;
	static const String LOWERCASE;
	static const String NON_ALPHA;
	static const String NON_ALPHANUMERIC;
	static const String NON_NUMERIC;
	static const String NON_WHITESPACE;
	static const String UPPERCASE;
	static const String WHITESPACE;
	RegularExpression();
	RegularExpression(const RegularExpression&);
	RegularExpression(const String&, bool wildcard_pattern = false);
	~RegularExpression() throw();
	virtual void clear() throw();
	void destroy();
	void set(const RegularExpression&);
	void set(const String&, bool wildcard_pattern = false);
	void get(RegularExpression&) const;
	const String& getPattern() const;
	Size countSubexpressions() const;
	bool match(const char*, const char*, int compile_flags = REG_EXTENDED | REG_NOSUB, int execute_flags = 0 );
	bool match(const String&, int from = 0, int execute_flags = 0) const throw(NullPointer, IndexUnderflow, IndexOverflow);
	bool match(const Substring&, int from = 0, int execute_flags = 0) const;
	bool match(const char*, int execute_flags = 0) const;
	bool find (const String&, Substring&, int from = 0, int execute_flags = 0) const;
	bool find (const Substring&, Substring&, int from = 0, int execute_flags = 0) const;
	bool isEmpty() const;
	bool operator == (const RegularExpression&) const;
	bool operator != (const RegularExpression&) const;
	bool operator < (const RegularExpression&) const;
	bool operator <= (const RegularExpression&) const;
	bool operator >= (const RegularExpression&) const;
	bool operator > (const RegularExpression&) const;
	virtual bool isValid() const throw();
};
