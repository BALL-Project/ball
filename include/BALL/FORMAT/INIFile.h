// $Id: INIFile.h,v 1.7 2000/10/05 17:28:09 anker Exp $

#ifndef BALL_FORMAT_INIFILE_H
#define BALL_FORMAT_INIFILE_H

#ifndef BALL_COMMON_H
# include <BALL/common.h>
#endif

#ifndef BALL_DATATYPE_STRINGHASHMAP_H
# include <BALL/DATATYPE/stringHashMap.h>
#endif

#include <vector>

namespace BALL 
{

	/**	INIFile.
			This class provides support to read and evaluate
			the contents of Windows-style INI files.\\

			{\bf Definition:}	\URL{BALL/FORMAT/INIFile.h}
			\\
			{\bf Implementation:}	\URL[source/FORMAT/INIFile.C]{../../source/FORMAT/INIFile.C}
			\\
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
		/**@name	Constructors and Destructors
		*/
		//@{

		/**	Default constructor.
		*/
		INIFile();

		/**	Constructor
		*/
		INIFile(const String& filename);

		/**	Destructor.
		*/
		virtual ~INIFile();

		/**	Destroy the contents of the object.
				{\tt destroy} calls {\tt clear} and also resets
				the filename to the empty string and invalidates the object.
		*/
		void destroy();

		/**	Clear the internal datastructures.
				{\tt clear} frees all allocated memory but retains the
				filename set for the {\tt INIFile} object.
		*/
		void clear();
		//@}
		
		/**@name	File I/O and related
		*/
		//@{
		/**	Open a file and read its contents.
				This method destroys all existing data in memory and
				then tries to open the file specified by setFilename.
				If the file could not be opened, the method returns immediately,
				leaving the \Ref{INIFile} instance in invalid state (as returned
				by \Ref{isValid}).\\
				If the file could be opened, the whole file is read into an 
				internal buffer and the different sections are interpreted.
				Then, internal datastructures for fast acces to the stored data
				are built (hash tables containing the section names).\\
				Line starting with '!', ';', or '\#' are treated as comment
				lines and are stored, but not interpreted.

				@return	bool \begin{itemize}
												\item {\bf true} if the file could be opened and read
												\item {\bf false} otherwise
											\end{itemize}
		*/	
		bool read();

		/**	Writes the buffer contents to a file.
				Not yet implemented!

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
				@param	filename String - the new filename
		*/	
		void setFilename(const String& filename);

		//@}



		/**@name	Debugging and Diagnostics
		*/
		//@{

		/**	Returns the current state of the instance of INIFile.
		*/	
		bool isValid() const;

		//@}


		/**@name	Methods for line-wise access.
				
				The INI file may be also accessed line-wise (ignoring the section
				structure). Each line can be accessed via its index (starting with 0)
				by \Ref{getLine} and modified by \Ref{setLine}. The index has to me less than
				the value returned by \Ref{getNumberOfLines}.
		*/
		//@{	

		/**	Return the contents of the specified line.
				If the {\bf line_number} given is not valid (less than
				0 or greater or equal to the number returned by \Ref{getNumberOfLines})
				0 is returned
				@param	line_number	first line starts with 0
				@return	a pointer to the specified line (as a \Ref{String}) or 0
		*/	
		String* getLine(Size line_number);

		/**	Change the contents of a line.
				Replaces the line given by {\bf line_number} by the text 
				in {\bf line}.\\
				Not yet implemented!
				@param	line_number number of the line to change, first line is 0
				@param	line new content of the line
				@return	bool \begin{itemize}
											\item {\bf true} if line_number was in a valid range
											\item {\bf false} otherwise
										\end{itemize}
		*/	
		bool setLine(Size line_number, const String& line);
		
		/**	Returns the number of lines in the buffer.
				@return 	Size total number of lines in the buffer
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

		/**	Returns the index of the first line of a section.
				The first line of a section is the line immediately following the
				section name (in square brackets).
				@return	Size - the index of the first line, or -1 if the section
						could not be found
				@param	section_name	the name of the section to be found
		*/	
		Size getSectionFirstLine(const String& section_name) const;

		/**	Returns the index of the last line of a section.
				The last line of a section is either the last line
				before a new section definition (starting with square brackets)
				or the last line of a file.
				@return	Size - the index of the last line, or -1 if the section
						could not be found
				@param	section_name	the name of the section to be found
		*/	
		Size getSectionLastLine(const String& section_name) const;	

		/**	Returns the number of line in a section.
				The header line is not counted. The last section is terminated
				by the last line of the file.
				@return	Size - the number of lines, or -1 if the section
						could not be found
				@param	section_name	the name of the section to be found
		*/	
		Size getSectionLength(const String& section_name) const;

		//@}

		/**@name	Methods to access single entries
		*/
		//@{

		/**	Check whether the given section contains a certain key.
				
				@return	bool - {\bf true} if the key could be found in the section,
											{\bf false} if either key or section didn't exist
				@param	section	the section to lokk in for the key
				@param	key the key to look for
		*/	
		bool hasEntry(const String& section, const String& key) const;
		
		/** Query the value corresponding to a given key.
				If no value exists for the given key, or either the section or
				the key are not defined, an empty string is returned.
				@param	section	the section name to look in for the key
				@param	key a key in the {\bf section}
				@return	String - the value corresponding to the {\bf key} in {\bf section}
								or an empty string
		*/	
		String getValue(const String& section, const String& key) const;

		/**	Change the value corresponding to a given key.
				Replaces the value of an existing key in an existing section.
				@param	section	the section to look in for the key
				@param	key	the key to look for
				@param	value the new value
				@return	bool - \begin{itemize}
									\item {\bf true} if the value was changed
									\item {\bf false} if key or section do not exist
								\end{itemize}
		*/	
		bool setValue(const String& section, const String& key, const String& value);

		//@}

		/** @name Predicates */
		//@{

		/** Equality operator */
		bool operator == (const INIFile& inifile) const;

		//@}


		private:	

		bool									valid_;

		String								filename_;
		
		std::vector<String>		section_names_;

		std::vector<String>		lines_;

		std::vector<Index>		line_section_index_;

		std::vector<Size>			section_start_;

		std::vector<Size>			section_end_;

		StringHashMap<Size>		section_index_;

		StringHashMap<Size>		section_key_map_;

		Size									original_number_of_lines_;

	};

} // namespace BALL

#endif // BALL_FORMAT_INIFILE_H
