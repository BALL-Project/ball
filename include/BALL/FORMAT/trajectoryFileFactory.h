// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#ifndef BALL_FORMAT_TRAJECTORYFILEFACTORY_H
#define BALL_FORMAT_TRAJECTORYFILEFACTORY_H

#ifndef BALL_SYSTEM_FILE_H
# include <BALL/SYSTEM/file.h>
#endif

namespace BALL
{
	class String;
	class TrajectoryFile;

	/**
	 * This class offers a factory method that given a path to a trajectory file
	 * determines the file type based on the file extension and returns
	 * a pointer to an appropriate subclass of TrajectoryFile
	 */
	class BALL_EXPORT TrajectoryFileFactory
	{
		public:
			/** Open a file.
			* This method tries to determine the filetype based on the file extension of name and
			* returns a pointer to an object of the appropriate subclass of TrajectoryFile if available.
			* If the file should be opened in read-mode and the extension is unknown or the file does not
			* have an extension, we try to automatically detect the format. 
			* If the detection fails, NULL is returned.
			*
			* @param name the path to the requested molecule file
			* @return a pointer to a subclass of TrajectoryFile, NULL if none is appropriate. Be aware that
			*         the file handle has not been checked for validity and that you have to delete it yourself
			*/
			static TrajectoryFile* open(const String& name, File::OpenMode open_mode = std::ios::in);

			/** Open a file.
			 *  When trying to open a file in write-mode, this function can be used to pass along a desired default-format. 
			 *  If the given filename does not have a supported extension, the specified default format will be used.
			 */
			static TrajectoryFile* open(const String& name, File::OpenMode open_mode, String default_format);

			/** Open a file.
			 *  When trying to open a file in write-mode, this function can be used to pass along a desired default-format. 
			 *  If the given filename does not have a supported extension, the format of 'default_format_file' will be used.
			 */
			static TrajectoryFile* open(const String& name, File::OpenMode open_mode, TrajectoryFile* default_format_file);

			/** Return a comma-separated string containing the file-extensions that are supported by TrajectoryFileFactory */
			static String getSupportedFormats();

			/** Return true if the extension of the specified filename is supported; otherwise return false. */
			static bool isFileExtensionSupported(String filename);

		private:

			/** Try to automatically detect the format of an input-file using keywords that are specific for the respective format. */
			static TrajectoryFile* detectFormat(const String& name);
	};

}

#endif // BALL_FORMAT_TRAJECTORYFILEFACTORY_H
