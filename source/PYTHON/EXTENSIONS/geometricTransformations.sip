// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: geometricTransformations.sip,v 1.4 2003/10/29 22:54:40 oliver Exp $
//

class TranslationProcessor
//  : public UnaryProcessor<Atom> 
	: AtomProcessor
{
%TypeHeaderCode
	#include <BALL/STRUCTURE/geometricTransformations.h>
	using namespace BALL;
	typedef UnaryProcessor<Atom> AtomProcessor;
%End
  public:
  TranslationProcessor();
  TranslationProcessor(const Vector3& /translation/);
  void setTranslation(const Vector3& /translation/);
  const Vector3& getTranslation() const;
  Processor::Result operator()(Atom& /atom/);
};

class TransformationProcessor
//  :public UnaryProcessor<Atom> 
	: AtomProcessor
{
%TypeHeaderCode
	#include <BALL/STRUCTURE/geometricTransformations.h>
	using namespace BALL;
	typedef UnaryProcessor<Atom> AtomProcessor;
%End
  public:
  TransformationProcessor();
  TransformationProcessor(const Matrix4x4& /transformation/);
  void setTransformation(const Matrix4x4& /transformation/);
  const Matrix4x4& getTransformation() const;
  Processor::Result operator()(Atom& /atom/);
};
