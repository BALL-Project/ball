// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: compositeDescriptor.sip,v 1.7 2003/10/29 22:54:38 oliver Exp $
//

class CompositeDescriptor
{
%TypeHeaderCode
	#include <BALL/VIEW/GUI/KERNEL/compositeDescriptor.h>
  #include <BALL/PYTHON/pyBALLSipHelper.h> 
	using namespace BALL;
	using namespace VIEW;
%End
  public:
  // BALL_CREATE(CompositeDescriptor)
  CompositeDescriptor();
  CompositeDescriptor
    (const CompositeDescriptor& /composite_descriptor/, bool /deep/ = true);
  ~CompositeDescriptor() throw();
  virtual void clear() throw();
  virtual void destroy() throw();
  void set
    (CompositeDescriptor& /composite_descriptor/, bool /deep/ = true);
  // CompositeDescriptor& operator = (CompositeDescriptor& composite_descriptor);
  // void get(CompositeDescriptor& /composite_descriptor/, bool /deep/ = true);
//  void swap(CompositeDescriptor& /composite_descriptor/);
  void setName(const String& /name/);
  String& getName();
  const String& getName() const;
  void setQuaternion(const Quaternion& /quaternion/);
  void setQuaternion(const Vector3& /axis/, float /angle/);
  void setQuaternion(const Vector3& /axis/, const Angle& /angle/);
  Quaternion& getQuaternion();
  const Quaternion& getQuaternion() const;
  Composite* getComposite();
%MethodCode
  sipThisType *sipThis;

  if (sipParseArgs(&sipArgsParsed,sipArgs,""))
  {
    Composite *res;
    CompositeDescriptor *ptr;

    if ((ptr = (CompositeDescriptor *)sipGetCppPtr(sipThis,sipClass_CompositeDescriptor)) == NULL)
      return NULL;

    res = ptr -> CompositeDescriptor::getComposite();
    return pyMapBALLObjectToSip(*res);
  }
	return NULL;
%End
  // const Composite* getComposite() const;
  void update();
  // void registerPrimitiveManager(const GLPrimitiveManager& manager);
  // void registerGLObjectCollector(const GLObjectCollector& collector);
  // void compileEntity(GLEntityDescriptor* entity);
  void drawEntity(bool /dynamic/ = false);
  void drawDirect(bool /dynamic/ = false, bool /with_names/ = false);
  bool isShallowCopy() const;
  bool isDeepCopy() const;
  virtual bool isValid() const throw();
  // virtual void dump(std::ostream& s = std::cout, Size depth = 0) const;
  // virtual void read(std::istream& s);
  // virtual void write(std::ostream& s) const;

  char* __str__() /NonLazy/;
%MethodCode
  CompositeDescriptor* ptr;

    if (sipParseArgs(&sipArgsParsed,sipArgs,"n",&sipSelfObj,sipClass_CompositeDescriptor,&ptr))
    {
			String tmp = String("CompositeDescriptor { ") + ptr->getName() + " }";
		  return PyString_FromString(tmp.c_str());
		}
%End
};
