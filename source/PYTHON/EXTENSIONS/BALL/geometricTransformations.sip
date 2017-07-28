class TranslationProcessor
	: AtomProcessor
{
%TypeHeaderCode
	#include <BALL/PYTHON/pyUnaryProcessors.h>
	#include <BALL/STRUCTURE/geometricTransformations.h>
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
	: AtomProcessor
{
%TypeHeaderCode
	#include <BALL/PYTHON/pyUnaryProcessors.h>
	#include <BALL/STRUCTURE/geometricTransformations.h>
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
