// $Id: directory.h,v 1.5 2000/06/19 00:09:56 amoll Exp $

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

#include <dirent.h>


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
				If the given directory does not exists, the directory path is set to an empty string.
				The directory path does not have a path seperator {\em /} at its end.
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

		/** Assignment with cloning facility.
				Assign the Directory with the path {\em directory_path} to {\em *this}.
				@param  directory_path the name of the directory to be cloned
				@param  set_current true to set the directory as the current, default = false
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
				@return String the name of the directory
		*/
		const String& getPath() const;

		/** Rename a given directory.
				@param old_path the old oath
				@param new_path the new oath
				@return bool  true if the directory could be renamed
		*/
		/*static*/ bool rename(String old_path, String new_path);

		/** Rename the directory associated with this object.
				@param new_path the new oath
				@return bool  true if the directory could be renamed
		*/
		bool renameTo(String new_path);

		/** Set a directory as the current.
				@param directory_path the name of the directory
				@return bool true if the directory could be set as the current
		*/
		/*static*/ bool setCurrent(String directory_path);

		/** Set the directory as the current.
				@return bool true if the directory could be set as the current
		*/
		bool setCurrent();

		/** Create a new directory.
				@param path the path of the new directory
				@param mode the access mode of the directory
				@return bool true if the directory could be created
		*/
		/*static*/ bool create(String path, const mode_t& mode = 0777);

		/** Remove a directory.
				@param old_path the path of the directory
				@return bool true if the directory could be removed
		*/
		/*static*/ bool remove(String old_path);

		/** Get the name of the first entry in the directory.
				@param entry reference to the first name of the entry
				@return bool true if an entry was found
		*/
		bool getFirstEntry(String& entry);

		/** Get the name of the first entry in the directory.
				@param entry constant reference to first the name of the entry
				@return bool true if an entry was found
		*/
		//bool getFirstEntry(String& entry) const;

		/** Get the name of the next entry in the directory.
				@param entry reference to the next name of the entry
				@return bool true if an entry was found
		*/
		bool getNextEntry(String& entry);

		/** Get the name of the next entry in the directory.
				@param entry constant reference to the next name of the entry
				@return bool true if an entry was found
		*/
		//bool getNextEntry(String& entry) const;

		/** Count all items in the directory.
				@return Size the size
		*/
		Size countItems(); //const;

		/** Count the files in the directory.
				@return Size the number of files
		*/
		Size countFiles(); //const;

		/** Count the subdirectories in the directory.
				@return Size the number of subdirectories
		*/
		Size countDirectories(); //const;

		/** Find a file in the directory.
				The search can be recursive.
				@param filename the name of the file to be searched
				@param filepath	the path of the file, if it was found
				@return bool true if the file was found
		*/
		bool find(const String& filename, String& filepath);

		/** Find a file in the directory.
				The search can be recursive.
				This function returns a constant reference.
				@param filename the name of the file to be searched
				@param filepath contains the path the file was found in
				@return bool true if the file was found
		*/
		//bool find(const String& filename, String& filepath) //const;
		//@}

		/**	@name	Predicates */
		//@{
		
		/** Test if the directory has a item.
				The search can be recursive.
				@param filename the name of the item to look for
				@return bool true if the directory has the file
		*/
		bool has(const String& item); //const;

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
		bool isEmpty(); // const;

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

			void synchronize_();
			bool desynchronize_();

			DIR*						dir_;
			dirent*					dirent_;
			String					directory_path_;
			String					backup_path_;
			bool						result_;
	};

#	ifndef BALL_NO_INLINE_FUNCTIONS
#		include <BALL/SYSTEM/directory.iC>
#	endif

} // namespace BALL 

#endif // BALL_SYSTEM_DIRECTORY_H 
