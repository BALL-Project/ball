// $Id: options.sip,v 1.3 2003/03/25 17:08:59 anhi Exp $

class IllegalKey
	:	GeneralException
{
%HeaderCode
	#include <BALL/DATATYPE/hashMap.h>
	typedef StringHashMap<String>::IllegalKey IllegalKey;
	using namespace BALL;
%End
	public:
	IllegalKey(const char* /file/, int /line/);
};

class Options
  // : public StringHashMap<String> 
{
%HeaderCode
	#include <BALL/DATATYPE/options.h>
	using namespace BALL;
%End
  public:
  static const Size MAX_ENTRY_LENGTH;
  Options();
  Options(const Options& /options/, bool /deep/ = true);
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
	String& operator [] (const String& /key/) 
		throw();
	const String& operator [] (const String& /key/) const
		throw(IllegalKey);

	char *__str__() /NoLazy/;
%MemberCode
  Options* ptr;

	if (sipParseArgs(&sipArgsParsed,sipArgs,"n",sipClass_Options,&ptr))
	{
  return PyString_FromString(String(String("Options ") + ptr->getName()
        + " { " + String(ptr->getSize()) + " entries }").c_str());
	}
	else
	{
		return NULL;
	}
%End

	
};
 
