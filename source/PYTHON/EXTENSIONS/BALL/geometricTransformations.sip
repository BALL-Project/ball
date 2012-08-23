// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id$
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
  TranslationProcessor(const Vector3&);
  void setTranslation(const Vector3&);
  const Vector3& getTranslation() const;
  Processor::Result operator()(Atom&);
%MethodCode
  sipRes = sipCpp->operator () (*a0);
%End
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
  TransformationProcessor(const Matrix4x4&);
  void setTransformation(const Matrix4x4&);
  const Matrix4x4& getTransformation() const;
  Processor::Result operator()(Atom&);
%MethodCode
  sipRes = sipCpp->operator () (*a0);
%End
};
