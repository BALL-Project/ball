class ForceFieldParameters
	: Parameters
{
%TypeHeaderCode
	#include <BALL/MOLMEC/PARAMETER/forceFieldParameters.h>
	using namespace BALL;
%End
	public:

	ForceFieldParameters();
	ForceFieldParameters(const String& filename);
	ForceFieldParameters(const ForceFieldParameters& force_field_parameter);
	virtual ~ForceFieldParameters();
	virtual void clear();
	AtomTypes&	getAtomTypes();

	virtual bool init();

	virtual bool isValid() const;
	bool operator == (const ForceFieldParameters& param) const;
};
