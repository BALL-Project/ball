class Fragment
	: AtomContainer
{
%TypeHeaderCode
	#include <BALL/KERNEL/fragment.h>
%End
	public:

	enum Property
	{
		NUMBER_OF_PROPERTIES = AtomContainer::Property::NUMBER_OF_PROPERTIES
	};

	Fragment();
	Fragment(const Fragment&, bool deep = true);
	Fragment(const String&);
	~Fragment();

	SIP_PYOBJECT __str__();
%MethodCode
	sipRes = PyString_FromString(String(String("Fragment ") + sipCpp->getName()
		+ " { " + String(sipCpp->countAtoms()) + " atoms }").c_str());
%End
};
