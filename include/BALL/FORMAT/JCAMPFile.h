// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: JCAMPFile.h,v 1.17 2003/07/09 12:56:42 amoll Exp $
//

#ifndef BALL_FORMAT_JCAMPFILE_H
#define BALL_FORMAT_JCAMPFILE_H

#ifndef BALL_FORMAT_LINEBASEDFILE__H
#	include <BALL/FORMAT/lineBasedFile.h>
#endif

#ifndef BALL_DATATYPE_STRINGHASHMAP_H
#	include <BALL/DATATYPE/stringHashMap.h>
#endif

namespace BALL
{
	/**	JCAMP file class.
			This class parses JCAMP files, which are often
			used to store parameter files in spectroscopy (NMR, IR, MS), e.g. 
			in Bruker instruments.
			\par
			This class has rudimentary support for the format only. The most severe
			drawback currently is the lcak of support for writing JCAMP files.
			
    	\ingroup  NMRFileFormats
	*/
	class JCAMPFile 
		: public LineBasedFile
	{
		public:

		/** Type definitions 
		*/
		//@{
		// The value types supported by JCAMP
		enum ContentType
		{
			///
			STRING,
			///
			NUMERIC,
			///
			ARRAY
		};

		///
		class JCAMPValue
		{
		  public:
			///
			String string_value;
			///
			std::vector<double> numeric_value;
			///
			ContentType type;
			
			JCAMPValue() : string_value(""), numeric_value(), type(STRING) {}
		};

		/// a key-value pair
		typedef std::pair<String, JCAMPValue> KeyValuePair;

		/// A hash map containing the JCAMP entries
		typedef StringHashMap<JCAMPValue> EntryMap;

		/// A hash map containing the header entries
		typedef StringHashMap<String> HeaderMap;

		//@}
		/**	@name	Constructors and Destructors
		*/
		//@{

		///
		JCAMPFile() throw() {}

		///
		JCAMPFile(const String& name, OpenMode open_mode = std::ios::in)
			throw(Exception::FileNotFound);

		/// Copy constructor.
		JCAMPFile(const JCAMPFile& file)
			throw(Exception::FileNotFound);

		///	Destructor.
		virtual ~JCAMPFile() throw() {}

		//@}
		/** @name Accessors
		 */
		//@{

		/// Read the file.
		void read() 
			throw(Exception::ParseError);

		/// Write the file.
		void write();

		///
		HeaderMap& getHeader() throw() { return header_; }

		///
		const HeaderMap& getHeader() const throw() { return header_; }

		///
		EntryMap& getEntries() throw() { return entries_; }

		///
		const EntryMap& getEntries() const throw() { return entries_; }

		///
		const JCAMPValue& operator [] (const String& name) const { return entries_[name]; }

		///
		double getDoubleValue(const String& name) const throw();
		
		///
		Index getIntValue(const String& name) const throw();

		/// 
		bool hasEntry(const String& name) const throw() { return entries_.has(name); }

		/// 
		bool hasHeader(const String& name) const throw() { return header_.has(name); }
		//@}
		
		protected:

		/// Entries from the header section 
		HeaderMap header_;

		/// Entries from the key-value section
		EntryMap entries_;
	};
}

#endif // BALL_FORMAT_JCAMPFILE_H
