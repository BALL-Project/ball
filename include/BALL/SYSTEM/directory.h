// $Id: directory.h,v 1.1 1999/08/26 07:53:20 oliver Exp $

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

namespace BALL {

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

			///
			Directory();

			///
			Directory(const String& directory_path, bool set_current = false);

			///
			Directory(const Directory& directory);

			///
			~Directory();

			///
			void clear();

			///
			void destroy();
			//@}

			/**	@name	Assignment */
			//@{

			///
			void set(const String& directory_path, bool set_current = false);

			///
			void set(const Directory& directory);

			///
			Directory& operator = (const Directory& directory);

			///
			void get(Directory& directory) const;

			///
			void swap(Directory& directory);
			//@}
				

			/**	@name	Accessors */
			//@{

			///
			const String& getPath() const;

			///
			static bool rename(String old_path, String new_path);

			///
			bool renameTo(const String& new_path);

			///
			static bool setCurrent(String& directory_path);

			///
			bool setCurrent();

			///
			static bool create(String& path, mode_t mode = 0777);

			///
			static bool remove(String old_path);

			bool getFirstEntry(String& entry);

			const String& getFirstEntry(String& entry) const;

			bool getNextEntry(String& entry);

			const String& getNextEntry(String& entry) const;

			BALL::Size getSize() const;

			BALL::Size countFiles() const;

			BALL::Size countDirectories() const;

			bool find(const String& filename, String& filepath, bool recursive = false);

			bool find(const String& filename, String& filepath, bool recursive = false) const;
			//@}

			/**	@name	Predicates */
			//@{
			
			///
			bool has(const String& filename, bool recursive = false) const;

			///
			bool isCurrent() const;

			///
			bool isEmpty() const;

			///
			bool operator == (const Directory& directory) const;

			///
			bool operator != (const Directory& directory) const;
			//@}

			/**	@name	Internal Iterators */
			//@{
			
			///
			bool apply(DirectoryProcessor& processor);

			///
			bool apply(DirectoryRecursiveProcessor& processor);
			//@}

		private:

			class FileFinder_
				: public DirectoryProcessor
			{
				public:

					FileFinder_(const String& filename)
						:	filename_(filename)
					{
					}

					virtual BALL::Processor::Result operator() (const String& path)
					{
						const char* filename = strrchr(path.getData(), FileSystem::PATH_SEPARATOR);

						if ((filename != 0 && strcmp(filename + 1, filename_) == 0)
								|| strcmp(path, filename_) == 0)
						{
							return Processor::ABORT;
						} else {
							return Processor::CONTINUE;
						}
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
