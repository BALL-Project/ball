#ifndef BALL_FORMAT_MOLFILEFACTORY_H
#define BALL_FORMAT_MOLFILEFACTORY_H

#include <BALL/SYSTEM/file.h>

namespace BALL
{

class String;
class GenericMolFile;

/**
 * This class offers a factory method that given a path to a molecule file
 * determines the file type based on the file extension and returns
 * a pointer to an appropriate subclass of GenericMolFile
 */
class BALL_EXPORT MolFileFactory
{
	public:
		/**
		* This method tries to determine the filetype based on the file extension of name and
		* returns a pointer to an object of the appropriate subclass of GenericMolFile if available.
		* @param name the path to the requested molecule file
		* @return a pointer to a subclass of GenericMolFile, NULL if none is appropriate. Be aware, that
		*         the file handle has not been checked for validity and that you have to delete it yourself
		*/
		static GenericMolFile* open(const String& name, File::OpenMode open_mode = std::ios::in);

		/** When trying to open a file in write-mode, this function can be used to pass along a desired default-format. If the given filename does not have a supported extension, the specified default format will be used. */
		static GenericMolFile* open(const String& name, File::OpenMode open_mode, String default_format);

		/** When trying to open a file in write-mode, this function can be used to pass along a desired default-format. If the given filename does not have a supported extension, the format of 'default_format_file' will be used. */
		static GenericMolFile* open(const String& name, File::OpenMode open_mode, GenericMolFile* default_format_file);

		/** Return a comma-separated string containing the file-extensions that are supported by MolFileFactory */
		static String getSupportedFormats();

		/** Return true if the extension of the specified filename is supported; otherwise return false. */
		static bool isFileExtensionSupported(String filename);

	private:

		/** Try to automatically detect the format of an input-file using keywords that are specific for the respective format. */
		static GenericMolFile* detectFormat(const String& name);
};

}

#endif //BALL_FORMAT_MOLFILEFACTORY_H

