// $Id: compositeDescriptor.sip,v 1.1 2000/07/18 17:02:19 oliver Exp $

class CompositeDescriptor
{
%HeaderCode
	#include <BALL/VIEW/OPENGL/KERNEL/compositeDescriptor.h>
  #include <BALL/PYTHON/pyBALLSipHelper.h> 
	using namespace BALL;
	using namespace VIEW;
%End
  public:
  // BALL_CREATE(CompositeDescriptor)
  CompositeDescriptor();
  CompositeDescriptor
    (const CompositeDescriptor& /composite_descriptor/, bool /deep/ = true);
  ~CompositeDescriptor();
  virtual void clear();
  virtual void destroy();
  void set
    (CompositeDescriptor& /composite_descriptor/, bool /deep/ = true);
  // CompositeDescriptor& operator = (CompositeDescriptor& composite_descriptor);
  // void get(CompositeDescriptor& /composite_descriptor/, bool /deep/ = true);
  void swap(CompositeDescriptor& /composite_descriptor/);
  void setName(const String& /name/);
  String& getName();
  const String& getName() const;
  void setCenter(const Vector3& /center/);
  void setCenter(float /x/, float /y/, float /z/);
  Vector3& getCenter();
  const Vector3& getCenter() const;
  void setBoundingBox(const Box3& /bounding_box/);
  void setBoundingBox
    (const Vector3& /upper/, const Vector3& /lower/);
  void setBoundingBox
    (float /upper_x/, float /upper_y/, float /upper_z/,
     float /lower_x/, float /lower_y/, float /lower_z/);
  Box3& getBoundingBox();
  const Box3& getBoundingBox() const;
  void setQuaternion(const Quaternion& /quaternion/);
  void setQuaternion(const Vector3& /axis/, float /angle/);
  void setQuaternion(const Vector3& /axis/, const Angle& /angle/);
  Quaternion& getQuaternion();
  const Quaternion& getQuaternion() const;
  void setComposite(const Composite& /composite/);
  void setComposite(const Composite* /composite/, bool /allow_deletion/ = false);
  Composite* getComposite();
%MemberCode
  sipThisType *sipThis;

  if ((sipThis = sipGetThis(sipThisObj,&sipArgs,sipClass_CompositeDescriptor)) == NULL)
    return NULL;

  if (sipParseArgs(sipArgs,""))
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
  virtual bool isValid() const;
  // virtual void dump(std::ostream& s = std::cout, Size depth = 0) const;
  // virtual void read(std::istream& s);
  // virtual void write(std::ostream& s) const;

	// __repr__ code:
	operator const char* ();
%MemberCode
  CompositeDescriptor* ptr;
  if ((ptr = (CompositeDescriptor*)sipGetCppPtr(sipThis,sipClass_CompositeDescriptor)) == NULL)
    return NULL;

	String tmp = String("CompositeDescriptor { ") + ptr->getName() + " }";
  return PyString_FromString(tmp.c_str());
%End
};
