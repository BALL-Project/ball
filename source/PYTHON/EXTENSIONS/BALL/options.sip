class Options
{
%TypeHeaderCode
	#include <BALL/DATATYPE/options.h>
%End
	public:
	static const Size MAX_ENTRY_LENGTH;
	Options();
	Options(const Options&);
	~Options();
	bool isInteger(const String&) const;
	bool isBool(const String&) const;
	bool isReal(const String&) const;
	bool isVector(const String&) const;
	bool isSet(const String&) const;
	void setName(const String&);
	const String& getName() const;
	String get(const String&) const;
	bool getBool(const String&) const;
	double getReal(const String&) const;
	Vector3 getVector(const String&) const;
	long getInteger(const String&) const;
	void set(const String&, const String&);
	void setReal(const String&,  double);
	void setVector(const String&, const Vector3&);
	void setInteger(const String&, long);
	void setBool(const String&, bool);
	String setDefault(const String&, const String&);
	long setDefaultInteger(const String&, long);
	double setDefaultReal(const String&, double);
	bool setDefaultBool(const String&, bool);
	bool readOptionFile(const String&);

	const String& operator [] (const String&) const	throw(StringHashMapString::IllegalKey);
%MethodCode
	sipRes = new String(sipCpp->operator [] (*a0));
%End

	SIP_PYOBJECT __str__();
%MethodCode
	sipRes = PyString_FromString(String(String("Options ") + sipCpp->getName()
		+ " { " + String(sipCpp->getSize()) + " entries }").c_str());
%End
};
