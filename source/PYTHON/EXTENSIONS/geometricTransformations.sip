// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: geometricTransformations.sip,v 1.2 2003/08/26 09:18:17 oliver Exp $
//

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
