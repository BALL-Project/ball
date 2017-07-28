class Path
{
%TypeHeaderCode
	#include <BALL/SYSTEM/path.h>
%End

	public:
	Path();
	String getDataPath();
	void setDataPath(const String&);
	void addDataPath(const String&);
	String getDefaultDataPath();
	String find(const String&);
	String findStrict(const String&);
};
