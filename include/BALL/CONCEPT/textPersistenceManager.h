// $Id: textPersistenceManager.h,v 1.7 2000/12/12 16:18:08 oliver Exp $

#ifndef BALL_CONCEPT_TEXTPERSISTENCEMANAGER_H
#define BALL_CONCEPT_TEXTPERSISTENCEMANAGER_H

#ifndef BALL_CONCEPT_PERSISTENCEMANAGER_H
#	include <BALL/CONCEPT/persistenceManager.h>
#endif

namespace BALL 
{

	/**	Text-format-based persistence manager.
			This class implements a persistnece manager that reads and writes
			its objects in a human-readable text format.
			This is slow and objects get {\em huge}, but the format is clear and
			comprehensible - the ideal format to debug persistence-related programs.
			For production use, the XDRPersistenceManager is preferred.
			@see	XDRPersistenceManager
	*/
	class TextPersistenceManager
		:	public PersistenceManager
	{
		public:

		/**	@name Constructors and Destructors
		*/
		//@{
		
		/**	Default constructor.
				Initializes just an internal variable.
		*/
		TextPersistenceManager();
			
		/**	Detailed constructor with an input stream.
		*/
		TextPersistenceManager(std::istream& is);

		/**	Detailed constructor with an output stream.
		*/
		TextPersistenceManager(std::ostream& os);

		/**	Detailed constructor with an input stream and an output stream.
		*/
		TextPersistenceManager(std::istream& is, std::ostream& os);
		//@}
		
		/**	@name	Layer 0 methods
		*/
		//@{

		/**	Write an object header.
				This method write an obejct header. It starts with the current number of spaces
				needed for a correct indentation.
				If {\tt name} is a null pointer, the object is a base object of the current
				object. This method then writes the string {\tt BASEOBJECT}. If {\tt name}
				is a name or an empty string, {\tt OBJECT} is written.\\
				Then, the object's {\tt type\_name} is written, followed by a blank, the character
				"@" to indicate an address and then the object's {\tt this} pointer in decimal
				format. The last string in the line is either "-" (for a base object or an object without
				name) or the object's {\tt name}.\\
				The indentation level is incremented.
				{\bf Example for a base object:}
				\begin{verbatim}
					BASEOBJECT BALL::Composite @ 1145254238 -
				\end{verbatim}
				{\bf Example for a member object:}
				\begin{verbatim}
					BASEOBJECT BALL::Bond @ 2334208924 bond_
				\end{verbatim}
		*/
		virtual void writeHeader(const char* type_name, const char* name, PointerSizeInt ptr);

		/**	Check for an an object header.
		*/
		virtual bool checkHeader(const char* type_name, const char* name, PointerSizeInt& ptr);

		/**
		*/
		virtual void writeTrailer(const char* name = 0);

		/**
		*/
		virtual bool checkTrailer(const char* name = 0);

		/**	Write a start marker to the output stream.
		*/
		virtual void writeStreamHeader();

		/**	Write a end marker to the output stream.
		*/
		virtual void writeStreamTrailer();

		/**	Check for the start marker in the input stream.
		*/
		virtual bool checkStreamHeader();

		/**	Check for the end marker in the output stream.
		*/
		virtual bool checkStreamTrailer();

		/**	Get an (unknown) object header.
		*/
		virtual bool getObjectHeader(String& type_name, PointerSizeInt& ptr);

		/**	Write a variable/member name.
		*/
		virtual void writeName(const char* name);

		/** Check for variable/member name.
		*/
		virtual bool checkName(const char* name);

		/**	Write storable object header.
		*/
		virtual void writeStorableHeader(const char* type_name, const char* name);
			
		/**	Check for storable object header.
		*/
		virtual bool checkStorableHeader(const char* type_name, const char* name);

		/**	Write type header and name for a primitive type.
		*/
		virtual void writePrimitiveHeader(const char* type_name, const char* name);

		/**	Check for a type header and name for a primitive type.
		*/
		virtual bool checkPrimitiveHeader(const char* type_name, const char* name);

		/**	Write storable object trailer.
		*/
		virtual void writeStorableTrailer();
			
		/**	Check for storable object trailer.
		*/
		virtual bool checkStorableTrailer();

		/**	Write the trailer for a primitive type.
		*/
		virtual void writePrimitiveTrailer();

		/**	Check the trailer of a primitive type.
		*/
		virtual bool checkPrimitiveTrailer();

		/**	Write header for a pointer to a PersistentObject.
		*/
		virtual void writeObjectPointerHeader(const char* type_name, const char* name);

		/**	Check for header for a pointer to a PersistentObject.
		*/
		virtual bool checkObjectPointerHeader(const char* type_name, const char* name);

		/**	Write header for a reference to a PersistentObject.
		*/
		virtual void writeObjectReferenceHeader(const char* type_name, const char* name);

		/**	Check for header for a reference to a PersistentObject.
		*/
		virtual bool checkObjectReferenceHeader(const char* type_name, const char* name);

		/**	Write header for an array of pointers to PersistentObjects.
		*/
		virtual void writeObjectPointerArrayHeader(const char* type_name, const char* name, Size size);

		/**	Check for header for an array of pointers to PersistentObjects.
		*/
		virtual bool checkObjectPointerArrayHeader(const char* type_name, const char* name, Size& size);

		/**	Write trailer for an array of pointers to PersistentObjects.
		*/
		virtual void writeObjectPointerArrayTrailer();

		/**	Check for trailer for an array of pointers to PersistentObjects.
		*/
		virtual bool checkObjectPointerArrayTrailer();
		//@}

		/**	@name	Put methods for primitive data types.
		*/
		//@{
		
		/**	Write a signed char to the output stream.
		*/
		virtual void put(const char c);

		/**	Write an unsigned char to the output stream.
		*/
		virtual void put(const Byte c);

		/**	Write an Index to the output stream.
		*/
		virtual void put(const Index s);

		/**	Write a Size or a Position to the output stream.
		*/
		virtual void put(const Size s);

		/**	Write a boolean value to the output stream.
		*/
		virtual void put(const bool b);

		/**	Write a single precision floating point number to the output stream.
		*/
		virtual void put(const Real f);

		/**	Write a double precision floating point number to the output stream.
		*/
		virtual void put(const DoubleReal d);

		/**	Write a string to the output.
		*/
		virtual void put(const string& s);

		/**	Write a 64-bit pointer to the output.
		*/
		virtual void put(const PointerSizeInt p);
		//@}

		/**	@name	Get methods for primitive data types.
		*/
		//@{

		/**	Read a signed char from the input stream.
		*/
		virtual void get(char& c);

		/**	Read an unsigned char from the input stream.
		*/
		virtual void get(Byte& b);

		/**	Read an Index from the input stream.
		*/
		virtual void get(Index& i);

		/**	Read a Size or a Position from the input stream.
		*/
		virtual void get(Size& s);

		/**	Read a boolean value from the input stream.
		*/
		virtual void get(bool& b);

		/**	Read a single precision floating point number from the input stream.
		*/
		virtual void get(Real& f);

		/**	Read a double precision floating point number from the input stream.
		*/
		virtual void get(DoubleReal& d);

		/**	Read a string from the output stream.
		*/
		virtual void get(string& s);

		/**	Read a pointer from the input stream.
		*/
		virtual void get(PointerSizeInt& p);
		//@}


		protected:
		
		/**	Check whether the streams contains a given string.
				This method reads the next string from the persistence manager's {\tt istream}
				and compares it to the {\tt value} given. If both strings are equal, {\bf true}
				is returned.
				@param	value the expected string
				@return bool, {\bf true} if the expected string was found, {\bf false} otherwise	
		*/
		virtual bool expect(const String& value);

		/**	Return a string for the current indentation level.
		*/
		virtual String indent();
		
		private:

		static const char*	INDENT_STEP;

		Size				indent_depth_;

	};

} // namespace BALL

#endif // BALL_CONCEPT_TEXTPERSISTENCEMANAGER_H
