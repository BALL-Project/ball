class Options
{
%HeaderCode
#include <BALL/DATATYPE/options.h>
using namespace BALL;
%End
  public:
  Options();
  Options(Options&, bool);
  //virtual ~Options();
  bool isInteger(String&) const;
  bool isBool(String&) const;
  bool isReal(String&) const;
  bool isVector(String&) const;
  bool isSet(String&) const;
  void setName(String&);
  String getName() const;
  String get(String&) const;
  bool getBool(String& ) const;
  double getReal(String&) const;
  Vector3 getVector(String&) const;
  long getInteger(String&) const;
  void set(String&, String&);
  void setReal(String&, double);
  void setVector(String&, Vector3&);
  void setInteger(String&, long);
  void setBool(String&, bool);
  String setDefault(String&, String&);
  long setDefaultInteger(String&, long);
  double setDefaultReal(String&, double);
  bool setDefaultBool(String&, bool);
  bool readOptionFile(String&);
  // virtual void dump (std::ostream&, Size) const;
 };
