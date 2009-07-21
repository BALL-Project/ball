// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: INIFile.h,v 1.41.20.1 2007/08/09 17:11:53 amoll Exp $
//

#ifndef BALL_FORMAT_INIFILE_H
#define BALL_FORMAT_INIFILE_H

#ifndef BALL_DATATYPE_STRINGHASHMAP_H
# include <BALL/DATATYPE/stringHashMap.h>
#endif

#ifndef BALL_CONCEPT_PROCESSOR_H
#	include <BALL/CONCEPT/processor.h>
#endif

namespace BALL 
{
	class INIFile;

	/**	INIFile.
			This class provides support to read and evaluate
			the contents of Windows-style INI files. \par
    	
			\ingroup  General
	*/
	class BALL_EXPORT INIFile
	{
		public:

		/**	Enum for constants
		*/
		enum
		{
			/**	Maximum line length for each entry (1048576)
			*/
			MAX_LINE_LENGTH = 1048576
		};

		class IteratorTraits_;
	
		/** A section within an INI file */
		class BALL_EXPORT Section
		{
			public:

			friend class INIFile;
			friend class IteratorTraits_;

			///
			const String& getName() const
			{
				return name_;
			}

			///
			bool operator == (const Section& section) const
			{
				return (name_		 == section.name_		&&
								lines_	 == section.lines_);
			}

			/// Needed for MSVC
			bool operator < (const Section& section) const
				;

			/// Needed for MSVC
			bool operator > (const Section& section) const
				;

			protected:

			// name of the section
			String																	name_;

			// all lines of the section
			std::list<String>														lines_;

			// hashmap with all keys
			StringHashMap<std::list<String>::iterator>		key_map_;
		};

		///
		typedef std::list<Section>::iterator SectionIterator;

		/** An iterator for the lines in an INIFile.
				With a LineIterator it is easy to iterate over all lines
				in an instance of INIFile.
		*/
		typedef IteratorTraits_ LineIterator;

		/** Return type for undefined keys
				"[UNDEFINED!]"
		*/
		static const String UNDEFINED;
		
		/** Name of the HEADER section:
				$	"\#HEADER!" $
		*/
		static const String HEADER;		
		
		/** @name	Constructors and Destructors
		*/
		//@{

		/**	Default constructor.
				The state of valid_ is set to false.
				An instance is valid if it is read or written successfully.
		*/
		INIFile();

		/**	Constructor with filename
		 		The file is not opend.
		*/
		INIFile(const String& filename);

		/**	Destructor.
		*/
		virtual ~INIFile();

		/**	Clear the internal datastructures.
				<tt>Clear</tt> frees all allocated memory but retains the
				filename set for the <tt>INIFile</tt> object.
		*/
		void clear();

		//@}
		/** @name	File I/O and related
		*/
		//@{

		/**	Open a file and read its contents.
				This method destroys all existing data in memory and
				then tries to open the file specified by setFilename.
				If the file could not be opened, the method returns false,
				leaving the INIFile instance in invalid state 
				(as returned by  \link isValid isValid \endlink ).  \par
				If the file could be opened, the whole file is read into an 
				internal buffer and the different sections are interpreted.
				Then, internal datastructures for fast access to the stored data
				are build (Hashtable containing the sections). \par
				Lines starting with '!', ';', or '\#' are treated as comment
				lines and stored, but not interpreted.
				Key-names and values are trimmed.
				If a line starts with "[", but no closing bracket occurs, false is returned.
				@return	bool
					- <b>true</b> if the file could be opened and read
					- <b>false</b> otherwise
											
		*/	
		bool read();

		/**	Writes the buffer contents to a file.
				If the file could not be written, valid_ is set to false, ow true.
				@return	bool
					- <b>true</b> if the file could be succesfully written
					- <b>false</b> otherwise
		*/	
		bool write();

		/**	Returns the current filename.
		*/	
		const String& getFilename() const;

		/**	Sets a new filename.
				The state of valid_ is set to false.
		*/	
		void setFilename(const String& filename);

		//@}
		/** @name	Debugging and Diagnostics
		*/
		//@{

		/**	Returns the current state of the instance of INIFile.
				An instance is valid if it is read or writen succesfully.
		*/	
		bool isValid() const;

		//@}
		/** @name	Methods for line-wise access.	
				The INIFile can be accessed line-wise (ignoring the section structure). 
				Each line can be accessed via its index (starting with 0)	by  \link getLine getLine \endlink  
				and modified by  \link setLine setLine \endlink . 
				The index has to be less than	the value returned by  \link getNumberOfLines getNumberOfLines \endlink .
		*/
		//@{	

		/**	Return the contents of the specified line.
				If the <b>line_number</b> given is not valid (less than
				0 or greater/equal to the number returned by  \link getNumberOfLines getNumberOfLines \endlink )
				a non-valid iterator is returned.
				Use of this method is not recommended, because in the worst case, it could be O(n).
				@param	line_number, first line starts with 0
				@return	LineIterator to the specified line
		*/	
		LineIterator getLine(Size line_number);

		//@}
		/** @name	Methods for access with an iterator.
				The INIFile may be also accessed with an LineIterator.			
		*/
		//@{		
		
		/**	Change the contents of a line.
				Replaces the line given by <b>line_it</b> by the text in <b>line</b>.
				Section lines cannot be changed with this method.
				If the line contains a key, the old key is deleted and the new 
				(if any) is set.
				If the line starts with "[" the method aborts.
				@param	line_it iterator to the line to change
				@param	line new content of the line
		*/	
		bool setLine(LineIterator line_it, const String& line);
		
		/** Delete a line.
				If the line does not exists, false is returned.
				Section headers can not be removed with this method.
				If the line contains a key, it will be removed.
				@param line_number the line to delete
		*/
		bool deleteLine(LineIterator line_it);
		
		/** Add a line after a given iterator.
				Lines starting with "[" cannot be added (to prevent problems with
				section headers).
				If the line contains a key and the section already contains this key
				the method aborts and returns false, use setValue() instead.
				@param line_it the iterator to insert after
				@param line the line to be added
				@return true, if line could be added
		*/
		bool insertLine(LineIterator line_it, const String& line);
		
		//@}
		/** @name	Methods for access per section.
		*/
		//@{		

		/** Append a line to a section.
				To add a line to the HEADER use <b>INIFile::HEADER</b> as section_name.
				If the given section does not exists, false is returned.
				Lines starting with "[" cannot be added (to prevent problems with
				section headers).
				If checking for duplicate keys is enabled and
				line contains a key, the section already contains, 
				the method aborts and returns false, use setValue() instead.
				If an empty string is given as value for section_name, the last section
				is used.
				@param section_name the section to add the line
				@param line the line to be added
				@see setDuplicateKeyCheck
				@return true, if line could be added
		*/
		bool appendLine(const String& section_name, const String& line);

		/// Append a line to the last section
		bool appendLine(const String& line);

		/**	Return number of lines.
		*/	
		Size getNumberOfLines() const;
				
		/**	Queries for a certain section.
				@param	section_name	the name of the section (without square brackets)
				@return bool
					- <b>true</b>  if the section exists (is hashed!)
					- <b>false</b> if the section could not be found
		*/	
		bool hasSection(const String& section_name) const;

		/** Return an iterator to a section with a given name.
				@return SectionIterator
					- iterator to the section
										    
		*/
	  SectionIterator getSection(const String& section_name);

		/** Return an iterator to a section at a given position.
				@return SectionIterator
					- iterator to the section
		*/
	  SectionIterator getSection(Position pos);

		/**	Count all sections.
				The HEADER is not counted!
		*/	
		Size getNumberOfSections() const;

		/**	Returns an iterator to the first line of a section.
				The first line of a section is the line with the section name (in square brackets).
				@return	LineIterator
					- iterator to the first line of the section
					- unvalid iterator, if section does not exist
										 
				@param	section_name	the name of the section to be found
		*/	
		LineIterator getSectionFirstLine(const String& section_name);

		/**	Returns an iterator to the last line of a section.
				@return	LineIterator
					- iterator to the last line of the section
					- unvalid iterator, if section does not exist
				@param	section_name	the name of the section to be found
		*/	
		LineIterator getSectionLastLine(const String& section_name);	

		/**	Returns the number of lines in a section.
				@return	Size
					- the number of lines, or 
					- INVALID_SIZE if the section could not be found
				@param	section_name	the name of the section to be found
		*/	
		Size getSectionLength(const String& section_name) const;

		/** Delete a section.
				If the given section does not exist, false is returned.
				If you remove the header, all lines of the header are removed,
				but the header itself still remains as section in the instance.
		*/
		bool deleteSection(const String& section);

		/** Append a section.
				If the given section does already exists, false is returned.
		*/
		bool appendSection(const String& section);

		//@}
		/** @name	Methods to access single entries
		*/
		//@{

		/**	Check whether the given section contains a certain key.
				@return	bool
					- <b>true</b> if the key could be found in the section, 
					- <b>false</b> if either key or section didn't exist
				@param	section	the section to look in for the key
				@param	key the key to look for
		*/	
		bool hasEntry(const String& section, const String& key) const;
		
		/** Query the value corresponding to a given key.
				If no value exists for the given key, or either the section or 
				the key are not defined, UNDEFINED is returned.
				@param	section	the section name to look in for the key
				@param	key a key in the <b>section</b>
				@return	String
					- the value corresponding to the <b>key</b> in <b>section</b>
					- or  \link UNDEFINED UNDEFINED \endlink 
		*/	
		String getValue(const String& section, const String& key) const;

		/**	Change the value corresponding to a given key.
				Replaces the value of an existing key in an existing section.
				If the key or the new value is a empty string nothing is changed.
				@param	section	the section to look in for the key
				@param	key	the key to look for
				@param	value the new value
				@return	bool
					- <b>true</b> if the value was changed
					- <b>false</b> if key or section do not exist
		*/	
		bool setValue(const String& section, const String& key, const String& value);

		/** Insert a new value in a given section
		 		If the key exists already or the sections doesnt exist, nothing happens.
				@param	section	the section to insert the key
				@param	key	the key to insert
				@param	value the new value
				@return	bool
					- <b>true</b> if the value was inserted
					- <b>false</b> if key or section do not exist
		*/
		bool insertValue(const String& section, const String& key, const String& value);

		///
		const INIFile& operator = (const INIFile& file)
			;

		//@}
		/** @name Predicates
		*/
		//@{

		/** Equality operator.
				Two instances are equal if they have the same sections
				with the same lines.
		*/
		bool operator == (const INIFile& inifile) const;

		/** Test if the given LineIterator is valid for this instance.
		*/
		bool isValid(const LineIterator& it) const;

    /** Test if the given SectionIterator is valid for this instance.
    */
    bool isValid(const SectionIterator& it) const;

		//@}

		/**	Apply a processor to all lines of the file.
		*/
		bool apply(UnaryProcessor<LineIterator>& processor);

		/** Set checking for duplicate keys mode
		*/
		void setDuplicateKeyCheck(bool mode);

		/** Get checking mode for duplicate keys
		*/
		bool duplicateKeyCheckEnabled() const;

		///
		std::list<String> getContent() const
			;

		///
		bool setContent(const std::list<String>& lines)
			;

		protected:

		bool															check_duplicate_keys_;

		bool															valid_;

		String														filename_;	

		// all sections, 0. section is HEADER
		std::list<Section>											sections_;

		// hashmap with the section names  => index
		StringHashMap<SectionIterator>		section_index_;

		public:

		/// Interface for the LineIterator
		class BALL_EXPORT IteratorTraits_
		{
			friend class INIFile;

			public:

			BALL_CREATE(IteratorTraits_)

			///
			IteratorTraits_();

			///
			IteratorTraits_(const IteratorTraits_& traits);

			///
			virtual ~IteratorTraits_();
			
			///
			const IteratorTraits_& operator = (const IteratorTraits_ &traits);

			///
			std::list<String>::iterator getPosition();

			///
			SectionIterator getSection();
		
			///
			const String& operator * () const;

			///
			IteratorTraits_& operator ++ ();

			///
			IteratorTraits_& operator -- ();

			///
			IteratorTraits_& getSectionNextLine();

			///
			bool operator == (const IteratorTraits_& traits) const;

			///
			bool operator != (const IteratorTraits_& traits) const;
			
			///
			bool operator + () const;

			///
			bool isValid() const;

			///
			void toSectionFirstLine();

			///
			void toSectionLastLine();

			///
			void toSectionEnd();

			///
			bool isSectionFirstLine() const;

			///
			bool isSectionLastLine() const;

			///
			bool isSectionEnd() const;

			///
			void toFirstLine();

			///
			void toLastLine();

			///
			void toEnd();

			protected:

			//_
			IteratorTraits_(std::list<Section>& list,
											SectionIterator section, 
											std::list<String>::iterator line);
			
			//_
			const std::list<Section>* getBound_() const;

			//_
			void setLine_(const String& line);

			private:

			std::list<Section>*					bound_;
			SectionIterator					section_;
			std::list<String>::iterator	position_;
		};
	};
} // namespace BALL

#endif // BALL_FORMAT_INIFILE_H
