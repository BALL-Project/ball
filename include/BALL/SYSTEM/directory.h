// $Id: directory.h,v 1.3 2000/06/13 14:12:47 amoll Exp $

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

#include <string.h>
#include <dirent.h>

namespace BALL 
{

	class DirectoryProcessor
		: public UnaryProcessor<String>
	{
		public:

		DirectoryProcessor()
		{
		}

		virtual ~DirectoryProcessor()
		{
		}

		virtual Processor::Result operator () (const String& path) = 0;
	};

	class DirectoryRecursiveProcessor
		: public BinaryProcessor<String, const unsigned long>
	{
		public:

		DirectoryRecursiveProcessor()
		{
		}

		virtual ~DirectoryRecursiveProcessor()
		{
		}

		virtual Processor::Result operator () 
			(const String& path, const unsigned long& directory_depth) = 0;
	};

	/**	Directory class 
	*/
	class Directory
	{
		public:

		/**	@name	Constructors and Destructors */
		//@{

		/** Default constructor.
				Construct new Directory object.
				@return    Directory - new constructed Directory object
		*/
		Directory();

		/** Detailed constructor.
				Construct new Directory object from the directory {\em directory_path}.
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
		void set(const String& directory_path, bool set_current = false);

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
		static bool rename(String old_path, String new_path);

		/** Rename the directory associated with this object.
				@param new_path the new oath
				@return bool  true if the directory could be renamed
		*/
		bool renameTo(String new_path);

		/** Set a directory as the current.
				@param directory_path the name of the directory
				@return bool true if the directory could be set as the current
		*/
		static bool setCurrent(String directory_path);

		/** Set the directory as the current.
				@return bool true if the directory could be set as the current
		*/
		bool setCurrent();

		/** Create a new directory.
				@param path the path of the new directory
				@param mode the access mode of the directory
				@return bool true if the directory could be created
		*/
		static bool create(String path, const mode_t& mode = 0777);

		/** Remove a directory.
				@param old_path the path of the directory
				@return bool true if the directory could be removed
		*/
		static bool remove(String old_path);

		bool getFirstEntry(String& entry);

		bool getFirstEntry(String& entry) const;

		bool getNextEntry(String& entry);

		bool getNextEntry(String& entry) const;

		/** Count all items in the directory.
				@return Size the size
		*/
		Size countItems() const;

		/** Count the files in the directory.
				@return Size the number of files
		*/
		Size countFiles() const;

		/** Count the subdirectories in the directory.
				@return Size the number of subdirectories
		*/
		Size countDirectories() const;

		/** Find a file in the directory.
				The search can be recursive.
				@param filename the name of the file to be searched
				@param filepath	the path of the file, if it was found
				@param recursive true to search recursive, default = false
				@return bool true if the file was found
		*/
		bool find(const String& filename, String& filepath, bool recursive = false);

		/** Find a file in the directory.
				The search can be recursive.
				This function returns a constant reference.
				@param filename the name of the file to be searched
				@param filepath contains the path the file was found in
				@param recursive true to search recursive, default = false
				@return bool true if the file was found
		*/
		bool find(const String& filename, String& filepath, bool recursive = false) const;
		//@}

		/**	@name	Predicates */
		//@{
		
		/** Test if the directory has a file.
				The search can be recursive.
				@param filename the name of the file to look for
				@param recursive true to search recursive
				@return bool true if the directory has the file
		*/
		bool has(const String& filename, bool recursive = false) const;

		/** Test if the directory is the current.
				@return bool
		*/
		bool isCurrent() const;

		/** Test if the directory is empty.
				@return bool
		*/
		bool isEmpty() const;

		/**	Equality operator.
				@return bool, {\bf true} if the name of the directories are equal
		*/
		bool operator == (const Directory& directory) const;

		/**	Inequality operator.
				@return bool, {\bf true} if the name of the directories are inequal
		*/
		bool operator != (const Directory& directory) const;
		//@}

		/**	@name	Internal Iterators */
		//@{
		
		/** Apply a directory processor.
				@param processor the DirectoryProcessor
				@return true if successful
		*/
		bool apply(DirectoryProcessor& processor);

		/** Apply a recursive directory processor.
				@param processor the DirectoryRecursiveProcessor
				@return true if successful
		*/
		bool apply(DirectoryRecursiveProcessor& processor);
		//@}

		private:

		class FileFinder_
			: public DirectoryProcessor
		{
			public:

				FileFinder_(const String& filename)
					:filename_(filename)
				{
				}

				virtual BALL::Processor::Result operator() (const String& path)
				{
					const char* filename = strrchr(path.data(), FileSystem::PATH_SEPARATOR);

					if ((filename != 0 && strcmp(filename + 1, filename_.data()) == 0)
							|| path == filename_)
					{
						return Processor::ABORT;
					} else return Processor::CONTINUE;
				}

				private:

				const String& filename_;
			};

			Processor::Result apply_(DirectoryRecursiveProcessor& processor);

			DIR*						dir_;
			dirent*					dirent_;
			unsigned long		directory_depth_;
			String					directory_path_;
	};

#	ifndef BALL_NO_INLINE_FUNCTIONS
#		include <BALL/SYSTEM/directory.iC>
#	endif

} // namespace BALL 

#endif // BALL_SYSTEM_DIRECTORY_H 
