#ifndef MOLFILEFACTORY_H
#define MOLFILEFACTORY_H

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
};

}

#endif //MOLFILEFACTORY_H

