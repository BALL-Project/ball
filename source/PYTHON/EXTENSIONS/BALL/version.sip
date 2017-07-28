class VersionInfo
{
%TypeHeaderCode
	#include <BALL/COMMON/version.h>
%End
	public:

	enum Type
	{
		STABLE,
		ALPHA,
		BETA,
		NONPUBLIC,
		UNKNOWN
	};

	static const char* getVersion();
	static int getMajorRevision();
	static int getMinorRevision();
	static Type getType();

	SIP_PYOBJECT __str__();
%MethodCode
	return PyString_FromString(VersionInfo::getVersion());
%End
};
