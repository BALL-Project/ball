// $Id: directory.h,v 1.7 2000/06/29 14:09:00 amoll Exp $

#ifndef BALL_SYSTEM_DIRECTORY_H
#define BALL_SYSTEM_DIRECTORY_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

#ifndef BALL_CONCEPT_PROCESSOR_H
#	include <BALL/CONCEPT/processor.h>
#endif

#ifndef BALL_DATATYPE_STRING_H
#	include <BALL/DATATYPE/string.h>
#endif

#ifndef BALL_SYSTEM_FILESYSTEM_H
#	include <BALL/SYSTEM/fileSystem.h>
#endif

#ifndef __DIRENT_H
# include <dirent.h>
#endif

namespace BALL 
{
	/**	Directory class 
	*/
	class Directory
	{
		public:

		/**	@name	Constructors and Destructors */
		//@{

		/** Default constructor.
				Constructs new Directory object.
				The directory path is set to the current working directory.
				The directory path does not have a path seperator {\em /} at its end.
				@return    Directory - new constructed Directory object
		*/
		Directory();

		/** Detailed constructor.
				Construct new Directory object from the directory {\em directory_path}.
				The directory path does not have a path seperator {\em /} at its end.
				If the given directory does not exists, the directory path is set to an empty string.
				@param  directory_path the name of the directory to be opend
				@param  set_current true, to set the directory as the current, default = false
				@return Directory - new constructed Directory object
		*/
		Directory(const String& directory_path, bool set_current = false);

		/** Copy constructor.
				Construct new Directory object by copying the Directory {\em directory}.
				@param  directory the Directory object to be copied (cloned)
				@return Directory - new constructed Directory cloned from {\em directory}
		*/
		Directory(const Directory& directory);

		/** Destructor.
				Default destruction of {\em *this} Directory.
		*/
		~Directory();

		/** Explicit default initialization.
				Set the state of {\em *this} Directory to the default values.
		*/
		void clear();

		/** Explicit destructor.*/
		void destroy();
		//@}

		/**	@name	Assignment */
		//@{

		/** Assign the Directory with the path {\em directory_path} to {\em *this}.
				The given directory path can be either absolute or relative. 
				If the path starts with a path seperator it is set as a absolute path.
				@param  directory_path the name of the directory to be cloned
				@param  set_current true to set the directory as the current, default = false
				@return bool true if the path could be set and is valid
		*/
		bool set(const String& directory_path, bool set_current = false);

		/** Assignment with cloning facility.
				Assign the Directory {\em directory} to {\em *this}.
				@param  directory_path the directory to be cloned
				@param  set_current true to set the directory as the current, default = false
		*/
		void set(const Directory& directory);

		/** Assignment operator.
				Assign the Directory {\em directory} to {\em *this} Directory.
		*/
		Directory& operator = (const Directory& directory);

		/** Copying with cloning facility.
				Copy {\em *this} Directory to the Directory {\em directory}.
				@param directory the directory to be assigned to
		*/
		void get(Directory& directory) const;

		/** Swapping of Directorys.
				Swap the states of {\em *this} Directory with the Directory {\em directory}.
				@param	Directory the Directory being swapped with {\em *this} Directory 
		*/
		void swap(Directory& directory);
		//@}
			

		/**	@name	Accessors */
		//@{

		/** Get the path of {\em *this} object.
				The directory path does not have a path seperator {\em /} at its end
				and is absolute; If a unvalid path was set the path is an empty string.
				@return String the name of the directory
		*/
		const String& getPath() const;

		/** Rename a given directory.
				With this method the directory associated with this object can not
				be renamed. Use renameTo instead to do so.
				@param old_path the old oath
				@param new_path the new oath
				@return bool  true if the directory could be renamed
		*/
		bool rename(String old_path, String new_path);

		/** Rename the directory associated with this object.
				@param new_path the new oath
				@return bool  true if the directory could be renamed
		*/
		bool renameTo(String new_path);

		/** Set a directory as the current.
				@param directory_path the name of the directory
				@return bool true if the directory could be set as the current
		*/
		bool setCurrent(String directory_path);

		/** Set this directory as the current.
				@return bool true if the directory could be set as the current
		*/
		bool setCurrent();

		/** Create a new directory.
				The directory is created using an absolute path, if it starts
				with a path seperator, else it is created in this directory.
				@param path the path of the new directory
				@param mode the access mode of the directory
				@return bool true if the directory could be created
		*/
		bool create(String path, const mode_t& mode = 0777);

		/** Remove a directory.
				With this method the directory associated with this object can not
				be removed. Use remove() instead to do so.
				@param old_path the path of the directory
				@return bool true if the directory could be removed
		*/
		bool remove(String old_path);

		/** Remove this directory.
				The directory this object points to is deleted and the object is cleared.
				@return bool true if the directory could be removed
		*/
		bool remove();

		/** Get the name of the first entry in the directory.
				@param entry reference to the name of the first entry
				@return bool true if an entry was found
		*/
		bool getFirstEntry(String& entry);

		/** Get the name of the next entry in the directory.
				@param entry reference to the name of the next entry
				@return bool true if an entry was found
		*/
		bool getNextEntry(String& entry);

		/** Count all items in the directory.
				@return Size the size
		*/
		Size countItems();

		/** Count the files in the directory.
				@return Size the number of files
		*/
		Size countFiles();

		/** Count the subdirectories in the directory.
				@return Size the number of subdirectories
		*/
		Size countDirectories();

		/** Find a file in the directory.
				The search can be recursive.
				@param filename the name of the file to be searched
				@param filepath	the path of the file, if it was found
				@return bool true if the file was found
		*/
		bool find(const String& filename, String& filepath);

		//@}

		/**	@name	Predicates */
		//@{
		
		/** Test if the directory has an item.
				@param filename the name of the item to look for
				@return bool true if the directory has the file
		*/
		bool has(const String& item);

		/**	Test if the directory ist valid.
				The directory is valid if it exists.
				This function uses ::opendir(const char *dirname).
				@return bool true if the directory is valid
		*/
		bool isValid() const;

		/** Test if the directory is the current working directory.
				@return bool
		*/
		bool isCurrent() const;

		/** Test if the directory is empty.
				@return bool
		*/
		bool isEmpty();

		/**	Equality operator.
				@return bool, {\bf true} if the name of both directories are equal
		*/
		bool operator == (const Directory& directory) const;

		/**	Inequality operator.
				@return bool, {\bf true} if the name of both directories are inequal
		*/
		bool operator != (const Directory& directory) const;

		//@}

		private:
			//switch to this dir
			void synchronize_();
			//switch back to the working directory
			bool desynchronize_(bool result = true);

			DIR*						dir_;
			dirent*					dirent_;
			String					directory_path_;
			String					backup_path_;
	};

#	ifndef BALL_NO_INLINE_FUNCTIONS
#		include <BALL/SYSTEM/directory.iC>
#	endif

} // namespace BALL 

#endif // BALL_SYSTEM_DIRECTORY_H 
