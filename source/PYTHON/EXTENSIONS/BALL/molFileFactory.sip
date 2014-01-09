class MolFileFactory
{
%TypeHeaderCode

	#include <BALL/FORMAT/molFileFactory.h>
	using namespace BALL;
%End
	public:
		static String getSupportedFormats();
		static bool isFileExtensionSupported(String filename);
		static GenericMolFile* open(const String&, OpenMode open_mode = std::ios::in);
	    static GenericMolFile* open(const String&, OpenMode, String, bool forced = false);
		static GenericMolFile* open(const String&, OpenMode, GenericMolFile*);

};
