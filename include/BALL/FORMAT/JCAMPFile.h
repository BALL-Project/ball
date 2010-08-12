// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#ifndef BALL_FORMAT_JCAMPFILE_H
#define BALL_FORMAT_JCAMPFILE_H

#ifndef BALL_FORMAT_LINEBASEDFILE_H
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
			drawback currently is the lack of support for writing JCAMP files.
			
    	\ingroup  NMRFileFormats
	*/
	class BALL_EXPORT JCAMPFile 
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
		class BALL_EXPORT JCAMPValue
		{
		  public:
			///
			String string_value;
			///
			std::vector<double> numeric_value;
			///
			ContentType type;
			
			JCAMPValue() : string_value(""), numeric_value(), type(STRING) {}
			
			bool operator == (const JCAMPValue& value) const;

			bool operator != (const JCAMPValue& value) const;
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
		JCAMPFile()  {}

		///
		JCAMPFile(const String& name, OpenMode open_mode = std::ios::in)
			throw(Exception::FileNotFound);

		///	Destructor.
		virtual ~JCAMPFile()  {}

		//@}
		/** @name Accessors
		 */
		//@{

		/// Read the file.
		void read() 
			throw(Exception::ParseError);

		/// Write the file.
		bool write()
			throw(File::CannotWrite);

		///
		HeaderMap& getHeader()  { return header_; }

		///
		const HeaderMap& getHeader() const  { return header_; }

		///
		EntryMap& getEntries()  { return entries_; }

		///
		const EntryMap& getEntries() const  { return entries_; }

		///
		const JCAMPValue& operator [] (const String& name) const { return entries_[name]; }

		///
		double getDoubleValue(const String& name) const 
			throw(Exception::InvalidFormat);
		
		///
		Index getIntValue(const String& name) const 
			throw(Exception::InvalidFormat);

		/// 
		bool hasEntry(const String& name) const  { return entries_.has(name); }

		/// 
		bool hasHeader(const String& name) const  { return header_.has(name); }

		/// 
		const JCAMPFile& operator = (const JCAMPFile& file) ;

		//@}
		/**	@name Equality operators
		*/
		//@{

		/** Equality operator
		*/
		bool operator == (const JCAMPFile& f)  const;

		/** Inequality operator
		*/
		bool operator != (const JCAMPFile& f)  const;
		//@}

		
		protected:

		/// Entries from the header section 
		HeaderMap header_;

		/// Entries from the key-value section
		EntryMap entries_;
	};
}

#endif // BALL_FORMAT_JCAMPFILE_H
