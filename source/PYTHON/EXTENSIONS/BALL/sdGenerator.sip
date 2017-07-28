class SDGenerator
{
%TypeHeaderCode
	#include <BALL/STRUCTURE/sdGenerator.h>
	#include <BALL/KERNEL/atom.h>
	#include <BALL/KERNEL/system.h>
%End

	public:

	struct Option
	{
		static const char* SHOW_HYDROGENS;
		static const char* STANDARD_BOND_LENGTH;
	};

	struct Default
	{
		static const bool SHOW_HYDROGENS;
		static const float STANDARD_BOND_LENGTH;
	};

	SDGenerator(bool);
	virtual ~SDGenerator();
	void setDefaultOptions();
	void generateSD(System&);
	void clear();

	Options options;
};
