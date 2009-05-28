class MolFileFactory
{
%TypeHeaderCode

	#include <BALL/FORMAT/molFileFactory.h>
	using namespace BALL;
%End
	public:
		static GenericMolFile* open(const String& /name/, std__openmode open_mode = std::ios::in);
};
