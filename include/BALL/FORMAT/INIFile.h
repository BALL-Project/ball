// $Id: INIFile.h,v 1.14 2001/04/08 23:29:35 amoll Exp $

#ifndef BALL_FORMAT_INIFILE_H
#define BALL_FORMAT_INIFILE_H

#ifndef BALL_COMMON_H
# include <BALL/common.h>
#endif

#ifndef BALL_DATATYPE_STRINGHASHMAP_H
# include <BALL/DATATYPE/stringHashMap.h>
#endif

#ifndef BALL_CONCEPT_PROCESSOR_H
#	include <BALL/CONCEPT/processor.h>
#endif

#ifndef BALL_DATATYPE_LIST_H
# include <BALL/DATATYPE/list.h>
#endif

namespace BALL 
{

	/**	INIFile.
			This class provides support to read and evaluate
			the contents of Windows-style INI files.\\
			{\bf Definition:}	\URL{BALL/FORMAT/INIFile.h}
	*/
	class INIFile
	{
		public:

		/**	Enum for constants
		*/
		enum
		{
			/**	Maximum line length for each entry (1024)
			*/
			MAX_LINE_LENGTH = 1024
		};

		class IteratorTraits_;
		
		class Section
		{
			public:

			friend class INIFile;
			friend class INIFile::IteratorTraits_;

			const String& getName() const
			{
				return name_;
			}

			bool operator == (const Section& section) const
			{
				return (name_		 == section.name_		&&
								lines_	 == section.lines_	&&
								key_map_ == section.key_map_);
			}

			protected:

			// name of the section
			String																	name_;

			// all lines of the section
			List<String>														lines_;

			// hashmap with all keys
			StringHashMap<List<String>::Iterator>		key_map_;
		};

		typedef List<Section>::Iterator Section_iterator;
		typedef List<Section>::ConstIterator ConstSection_iterator;
	
		class IteratorTraits_
		{

			friend class INIFile;

			public:

			BALL_CREATE(IteratorTraits_)

			IteratorTraits_()
				:	bound_(0),
					section_(),
					position_()
					
			{
			}
			
			IteratorTraits_(List<Section>& list, 
											Section_iterator section, 
											List<String>::Iterator line)
				:	bound_(&list),
					section_(section),
					position_(line)
			{
			}
			
			IteratorTraits_(const IteratorTraits_& traits)
				:	bound_(traits.bound_),
					section_(traits.section_),
					position_(traits.position_)
			{
			}

			virtual ~IteratorTraits_()
			{
			}
			
			const IteratorTraits_& operator = (const IteratorTraits_ &traits)
			{
				bound_		= traits.bound_;
				section_  = traits.section_;
				position_ = traits.position_;

				return *this;
			}

			List<String>::Iterator getPosition()
			{
				return position_;
			}

			Section_iterator getSection()
			{
				return section_;
			}
		
			const String& operator *() const
			{
				return *position_;
			}

			const String& getLine() const
			{
				return *position_;
			}

			IteratorTraits_& operator ++ ()
			{
				if (bound_ == 0)
				{
					return *this;
				}

				if (!isSectionEnd())
				{
					position_++;

					return *this;
				}

				section_++;

				if (section_ == bound_->end())
				{
					return *this;
				}

				position_ = section_->lines_.begin();

				return *this;
			}

			bool operator == (const IteratorTraits_& traits) const
			{
				return (bound_ == traits.bound_			&&
								section_ == traits.section_ &&
								position_ == traits.position_);
			}

			bool operator != (const IteratorTraits_& traits) const
			{
				return !(*this == traits);
			}
			
			bool operator + () const
			{
				return (bound_ != 0 && 
								section_ != bound_->end() &&
								position_ != section_->lines_.end());
			}

			bool isValid() const
			{
				return (+ (*this));
			}

			void toBegin()
			{
				position_ = section_->lines_.begin();
			}

			bool isSectionBegin() const
			{
				return (position_ == section_->lines_.begin());
			}

			void toSectionEnd()
			{
				position_ = section_->lines_.end();
				position_--;
			}
			
			bool isSectionEnd() const
			{
				List<String>::Iterator last(section_->lines_.end());
				last--;
				return (position_ == last);
			}
			
			//protected:

			String& operator *()
			{
				return *position_;
			}

			private:

			List<Section>*					bound_;
			Section_iterator				section_;
			List<String>::Iterator	position_;
		};

		/** An iterator for the lines in an INIFile.
				With a LineIterator it is easy to iterator over all lines
				in an instance of INIFile.
		*/
		typedef IteratorTraits_ LineIterator;
/*
		friend class IteratorTraits_;

		typedef ForwardIterator<ResourceEntry, ResourceEntry, ResourceEntry*, IteratorTraits_> Iterator;

		Iterator begin()
		{
			return Iterator::begin(*this);
		}

		Iterator end()
		{
			return Iterator::end(*this);
		}


		typedef ConstForwardIterator<ResourceEntry, ResourceEntry, ResourceEntry*, IteratorTraits_> ConstIterator;

		ConstIterator begin() const
		{
			return ConstIterator::begin(*this);
		}

		ConstIterator end() const
		{
			return ConstIterator::end(*this);
		}

*/
		//friend class LineIterator;

		/** Return type: undefined:
				"[UNDEFINED!]"
		*/
		static const String UNDEFINED;
		
		/** Name of the HEADER section:
				"#HEADER!"
		*/
		static const String HEADER;		
		
		/** @name	Constructors and Destructors
		*/
		//@{

		/**	Default constructor.
				The state of valid_ is set to false.
				An instance is valid if it is read or writen succesfully.
		*/
		INIFile();

		/**	Constructor
		*/
		INIFile(const String& filename);

		/**	Destructor.
		*/
		virtual ~INIFile();

		/**	Destroy the contents of the object.
				{\tt Destroy} calls {\tt clear} and also resets
				the filename to the empty string and invalidates the object.
		*/
		void destroy();

		/**	Clear the internal datastructures.
				{\tt Clear} frees all allocated memory but retains the
				filename set for the {\tt INIFile} object.
		*/
		void clear();

		//@}
		/** @name	File I/O and related
		*/
		//@{

		/**	Open a file and read its contents.
				This method destroys all existing data in memory and
				then tries to open the file specified by setFilename.
				If the file could not be opened, the method returns immediately false,
				leaving the INIFile instance in invalid state (as returned by \Ref{isValid}). \\
				If the file could be opened, the whole file is read into an 
				internal buffer and the different sections are interpreted.
				Then, internal datastructures for fast acces to the stored data
				are built (hash tables containing the section names).\\
				Line starting with '!', ';', or '\#' are treated as comment
				lines and are stored, but not interpreted.
				Key-names and values are trimmed.
				If the file could not be read, valid_ is set to false, ow true.
				IF a line starts with "[", but no closing bracket occurs, the line is skipped.
				@return	bool \begin{itemize}
												\item {\bf true} if the file could be opened and read
												\item {\bf false} otherwise
											\end{itemize}
		*/	
		bool read();

		/**	Writes the buffer contents to a file.
				If the file could not be writen, valid_ is set to false, ow true.
				@return	bool - \begin{itemize}
												\item {\bf true} if the file could be succesfully written
												\item {\bf false} otherwise
											\end{itemize}
		*/	
		bool write();

		/**	Returns the current filename.
				@return 	String& -  the filename
		*/	
		const String& getFilename() const;

		/**	Sets a new filename.
				The state of valid_ is set to false.
				@param	filename String - the new filename
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
				The INI file may be also accessed line-wise (ignoring the section
				structure). Each line can be accessed via its index (starting with 0)
				by \Ref{getLine} and modified by \Ref{setLine}. The index has to be less than
				the value returned by \Ref{getNumberOfLines}.
		*/
		//@{	

		/**	Return the contents of the specified line.
				If the {\bf line_number} given is not valid (less than
				0 or greater or equal to the number returned by \Ref{getNumberOfLines})
				a non-valid iterator is returned.
				@param	line_number, first line starts with 0
				@return	LineIterator to the specified line
		*/	
		LineIterator getLine(Size line_number);

		/**	Change the contents of a line.
				Replaces the line given by {\bf line_number} by the text in {\bf line}.
				Section lines cannot be changed with this method.
				If the line contains a key, the old one is deleted and the new one
				(if any) is set.
				If line starts with "[" the method aborts.
				@param	line_number number of the line to change, first line is 0
				@param	line new content of the line
				@return	bool \begin{itemize}
											\item {\bf true} if line_number was in a valid range
											\item {\bf false} otherwise
										\end{itemize}
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
				If the line contains a key and the section contains already this key
				the method aborts and returns false, use setValue() instead.
				@param line_it the iterator to insert after
				@param line the line to be added
				@return true, if line could be added
		*/
		bool insertLine(LineIterator line_it, const String& line);
		
		/** Append a line to a section.
				To add a line to the HEADER use {\bf INIFile::HEADER} as section_name.
				If the given section does not exists, false is returned.
				Lines starting with "[" cannot be added (to prevent problems with
				section headers).
				If the line contains a key and the section contains already this key
				the method aborts and returns false, use setValue() instead.
				@param section_name the section to add the line
				@param line the line to be added
				@return true, if line could be added
		*/
		bool appendLine(const String& section_name, const String& line);

		/**	Return number of lines.
				@return	Size number of lines in the INIFile
		*/	
		Size getNumberOfLines() const;
				
		/**	Queries for a certain section.
				@param	section_name	the name of the section (without square brackets)
				@return bool \begin{itemize}
											\item {\bf true} if the section exists (is hashed!)
											\item {\bf false}	if the section could not be found
										\end{itemize}
		*/	
		bool hasSection(const String& section_name) const;


		/** Return the name of a section at a given position.
				@return String* \begin{itemize}
											   \item the pointer to the section-name, or 
											   \item 0, if pos is too high
										    \end{itemize}
		*/
	  Section_iterator getSection(const String& section_name);

	  Section_iterator getSection(Position pos);

		/**	Count all sections.
				The HEADER is not counted!
		*/	
		Size getNumberOfSections() const;

		/**	Returns the index of the first line of a section.
				The first line of a section is the line immediately following the 
				section name (in square brackets).
 				Remember: Lines that are inserted after a INIFile was read are not counted here.
				@return	Size \begin{itemize}
											\item the index of the first line, or 
											\item INVALID_SIZE if the section could not be found or has no lines
										 \end{itemize}
				@param	section_name	the name of the section to be found
		*/	
		LineIterator getSectionFirstLine(const String& section_name);

		/**	Returns the index of the last line of a section.
				The last line of a section is either the last line 
				before a new section definition (starting with square brackets) 
				or the last line of a file.
				Remember: Lines that are inserted after a INIFile was read are not counted here.
				If the section does not have read lines, INVALID_SIZE is returned.
				@return	Size \begin{itemize}
											\item the index of the last line, or 
											\item INVALID_SIZE if the section could not be found or has no lines
											\end{itemize}
				@param	section_name	the name of the section to be found
		*/	
		LineIterator getSectionLastLine(const String& section_name);	

		/**	Returns the number of lines in a section.
				@return	Size \begin{itemize}
											\item the number of lines, or 
											\item INVALID_SIZE if the section could not be found
										 \end{itemize}
				@param	section_name	the name of the section to be found
		*/	
		Size getSectionLength(const String& section_name) const;

		//@}
		/** @name	Methods to access single entries
		*/
		//@{

		/**	Check whether the given section contains a certain key.
				@return	bool \begin{itemize}
											\item {\bf true} if the key could be found in the section, 
											\item {\bf false} if either key or section didn't exist
										 \end{itemize}
				@param	section	the section to look in for the key
				@param	key the key to look for
		*/	
		bool hasEntry(const String& section, const String& key) const;
		
		/** Query the value corresponding to a given key.
				If no value exists for the given key, or either the section or 
				the key are not defined, an empty string is returned.
				@param	section	the section name to look in for the key
				@param	key a key in the {\bf section}
				@return	String \begin{itemize} 
												\item the value corresponding to the {\bf key} in {\bf section}
												\item or \Ref{UNDEFINED}
											 \end{itemize}
		*/	
		String getValue(const String& section, const String& key) const;

		/**	Change the value corresponding to a given key.
				Replaces the value of an existing key in an existing section.
				If the key or the new value is a empty string nothing is changed.
				@param	section	the section to look in for the key
				@param	key	the key to look for
				@param	value the new value
				@return	bool - \begin{itemize}
									\item {\bf true} if the value was changed
									\item {\bf false} if key or section do not exist
								\end{itemize}
		*/	
		bool setValue(const String& section, const String& key, const String& value);

		/**	
		*/
		bool apply(UnaryProcessor<LineIterator>& processor);

		//@}
		/** @name Predicates
		*/
		//@{

		/** Equality operator 
		*/
		bool operator == (const INIFile& inifile) const;

		//@}

		//protected:	

		bool									valid_;

		String								filename_;	

		// all sections
		// 0. section is "[HEADER]"
		List<Section>		sections_;

		// hashmap with the section names  => index
		StringHashMap<Section_iterator>		section_index_;
	};

} // namespace BALL

#endif // BALL_FORMAT_INIFILE_H
