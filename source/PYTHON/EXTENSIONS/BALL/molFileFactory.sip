class MolFileFactory
{
%TypeHeaderCode

	#include <BALL/FORMAT/molFileFactory.h>
	using namespace BALL;
%End
	public:
		static GenericMolFile* open(const String& /name/, OpenMode open_mode = std::ios::in);
};
