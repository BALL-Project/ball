// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id$
//

class IllegalKey
	:	GeneralException
{
%TypeHeaderCode
	#include <BALL/COMMON/exception.h>
	#include <BALL/DATATYPE/stringHashMap.h>
	using namespace BALL;
	using namespace BALL::Exception;
	typedef StringHashMap<String>::IllegalKey IllegalKey;
%End
	public:
	IllegalKey(const char* /file/, int /line/);
};

class Options
  // : public StringHashMap<String> 
{
%TypeHeaderCode
	#include <BALL/DATATYPE/options.h>
	using namespace BALL;
%End
  public:
  static const Size MAX_ENTRY_LENGTH;
  Options();
  Options(const Options& /options/);
  ~Options();
  bool isInteger(const String& /key/) const;
  bool isBool(const String& /key/) const;
  bool isReal(const String& /key/) const;
  bool isVector(const String& /key/) const;
  bool isSet(const String& /key/) const;
  void setName(const String& /name/);
  const String& getName() const;
  String get(const String& /key/) const;
  bool getBool(const String& /key/) const;
  double getReal(const String& /key/) const;
  Vector3 getVector(const String& /key/) const;
  long getInteger(const String& /key/) const;
  void set(const String& /key/, const String& /value/);
  void setReal(const String& /key/,  double /value/);
  void setVector(const String& /key/, const Vector3& /value/);
  void setInteger(const String& /key/, long /value/);
  void setBool(const String& /key/, bool /value/);
  String setDefault(const String& /key/, const String& /value/);
  long setDefaultInteger(const String& /key/, long /value/);
  double setDefaultReal(const String& /key/, double /value/);
  bool setDefaultBool(const String& /key/, bool /value/);
  bool readOptionFile(const String& /filename/);
  // virtual void dump (std::ostream& s = std::cout, Size depth = 0) const;
	// methods inherited from HashMap<String, String>
	// String& operator [] (const String& /key/) throw();

	const String& operator [] (const String& /key/) const	throw(IllegalKey);
%MethodCode
  sipRes = new String(sipCpp->operator [] (*a0));
%End

  SIP_PYOBJECT __str__();
%MethodCode
	sipRes = PyString_FromString(String(String("Options ") + sipCpp->getName()
    			    + " { " + String(sipCpp->getSize()) + " entries }").c_str());
%End

	
};
 
