// $Id: geometricTransformations.C,v 1.1 1999/08/26 08:02:38 oliver Exp $

#include <BALL/STRUCTURE/geometricTransformations.h>

namespace BALL {

	// TranslationProcessor
	//=============================

	TranslationProcessor::TranslationProcessor(void) 
		:	translation_(0, 0, 0)
	{
	}

	TranslationProcessor::TranslationProcessor(const Vector3& translation) 
		:	translation_(translation)
	{
	}

	void TranslationProcessor::setTranslation(const Vector3& translation) 
	{
		translation_ = translation;
	}

	const Vector3& TranslationProcessor::getTranslation(void) const
	{
		return translation_;
	}

	Processor::Result TranslationProcessor::operator () (Atom& atom) 
	{
		atom.setPosition(atom.getPosition() + translation_);
		return Processor::CONTINUE;
	}



	// TransformationProcessor
	//================================

	TransformationProcessor::TransformationProcessor(void) 
			:
			transformation_(1,0,0,0,
											0,1,0,0,
											0,0,1,0,
											0,0,0,1)
	{
	}

	TransformationProcessor::TransformationProcessor(const Matrix4x4& transformation) 
			:
			transformation_(transformation)
	{
	}

	void TransformationProcessor::setTransformation(const Matrix4x4& transformation) 
	{
		transformation_ = transformation;
	}

	const Matrix4x4& TransformationProcessor::getTransformation(void) const
	{
		return transformation_;
	}

	Processor::Result TransformationProcessor::operator () (Atom& atom) 
	{
		atom.setPosition(transformation_ * atom.getPosition());
		return Processor::CONTINUE;
	}

} // namespace BALL
