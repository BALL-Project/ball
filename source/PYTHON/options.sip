// $Id: options.sip,v 1.2 2000/07/06 14:08:49 oliver Exp $
class Options
  // : public StringHashMap<String> 
{
%HeaderCode
	#include <BALL/DATATYPE/options.h>
	using namespace BALL;
%End
  public:
  // static const int MaxEntryLength = 1024;
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
};
 
