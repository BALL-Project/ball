// $Id: geometricTransformations.C,v 1.2 1999/12/04 18:34:33 oliver Exp $

#include <BALL/STRUCTURE/geometricTransformations.h>

namespace BALL {

	// TranslationProcessor
	//=============================

	TranslationProcessor::TranslationProcessor() 
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

	const Vector3& TranslationProcessor::getTranslation() const
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

	TransformationProcessor::TransformationProcessor() 
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

	const Matrix4x4& TransformationProcessor::getTransformation() const
	{
		return transformation_;
	}

	Processor::Result TransformationProcessor::operator () (Atom& atom) 
	{
		atom.setPosition(transformation_ * atom.getPosition());
		return Processor::CONTINUE;
	}

} // namespace BALL
