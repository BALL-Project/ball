// $Id: geometricTransformations.sip,v 1.1 2000/07/06 14:08:48 oliver Exp $

class TranslationProcessor
//  : public UnaryProcessor<Atom> 
	: AtomProcessor
{
%HeaderCode
	#include <BALL/STRUCTURE/geometricTransformations.h>
	using namespace BALL;
	typedef UnaryProcessor<Atom> AtomProcessor;
%End
  public:
  TranslationProcessor();
  TranslationProcessor(const Vector3& /translation/);
  void setTranslation(const Vector3& /translation/);
  const Vector3& getTranslation() const;
  virtual Processor::Result operator()(Atom& /atom/);
};

class TransformationProcessor
//  :public UnaryProcessor<Atom> 
	: AtomProcessor
{
%HeaderCode
	#include <BALL/STRUCTURE/geometricTransformations.h>
	using namespace BALL;
	typedef UnaryProcessor<Atom> AtomProcessor;
%End
  public:
  TransformationProcessor();
  TransformationProcessor(const Matrix4x4& /transformation/);
  void setTransformation(const Matrix4x4& /transformation/);
  const Matrix4x4& getTransformation() const;
  virtual Processor::Result operator()(Atom& /atom/);
};
