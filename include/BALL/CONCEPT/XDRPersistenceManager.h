// $Id: XDRPersistenceManager.h,v 1.13 2000/12/12 16:18:08 oliver Exp $

#ifndef BALL_CONCEPT_XDRPERSISTENCEMANAGER_H
#define BALL_CONCEPT_XDRPERSISTENCEMANAGER_H

#ifndef BALL_CONCEPT_PERSISTENCEMANAGER_H
#	include <BALL/CONCEPT/persistenceManager.h>
#endif

#include <rpc/types.h>
#include <rpc/xdr.h>

namespace BALL 
{

	/**	XDR-format-based persistence manager.
			This class implements a persistence manager that reads and writes
			its objects in a portable binary format.
			@see	TextPersistenceManager
	*/
	class XDRPersistenceManager
		:	public PersistenceManager
	{
		public:

		/**	@name Constants
		*/
		//@{
		/**	A constant value used to mark the beginning of a persistent stream
		*/
		static const Size STREAM_HEADER;

		/**	A constant value used to mark the end of a persistent stream
		*/
		static const Size STREAM_TRAILER;

		/**	A constant value used to mark the beginning of an object
		*/
		static const Size OBJECT_HEADER;

		/**	A constant value used to mark the end of an object
		*/
		static const Size OBJECT_TRAILER;
		//@}

		/**	@name Constructors and Destructors
		*/
		//@{
		
		/**	Default constructor.
		*/
		XDRPersistenceManager();

		/**	Detailed constructor with an input stream
		*/
		XDRPersistenceManager(std::istream& is);

		/**	Detailed constructor with an output stream
		*/
		XDRPersistenceManager(std::ostream& os);

		/**	Detailed constructor with an input stream and an output stream.
		*/
		XDRPersistenceManager(std::istream& is, std::ostream& os);
		//@}
		
		/**	@name	Layer 0 methods
		*/
		//@{

		/**	Write an object header.
				This method stores \Ref{OBJECT_HEADER} as an int value to mark the
				start of an object (using {\tt xdr_int}).
		*/
		virtual void writeHeader(const char* type_name, const char* name, PointerSizeInt ptr);

		/**	Check for an object header.
				This method reads an int form the input stream (using {\tt xdr_int}) and
				returns {\bf true} if the value read equals \Ref{OBJECT_HEADER}.
		*/
		virtual bool checkHeader(const char* type_name, const char* name, PointerSizeInt& ptr);

		/** Write an object trailer.
				This method stores \Ref{OBJECT_TRAILER} as an int value to mark the
				start of an object (using {\tt xdr_int}).
		*/
		virtual void writeTrailer(const char* name = 0);

		/**	Check for an object trailer.
				This method reads an int form the input stream (using {\tt xdr_int}) and
				returns {\bf true} if the value read equals \Ref{OBJECT_TRAILER}.
		*/
		virtual bool checkTrailer(const char* name = 0);

		/**	Write a start marker to the output stream.
		*/
		virtual void writeStreamHeader();

		/**	Write an end marker to the output stream.
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

		/**	Prepare the stream for output.
				This method creates an XDR output stream (using xdrrec_create) and
				prepares it for output.
		*/
		virtual void initializeOutputStream();

		/**	Prepare the stream for closing.
				This method destroys the XDR output stream (using xdr_destroy).
		*/
		virtual void finalizeOutputStream();

		/**	Prepare the stream for output.
				This method creates an XDR output stream (using xdrrec_create) and
				prepares it for output.
		*/
		virtual void initializeInputStream();

		/**	Prepare the stream for closing.
				This method destroys the XDR output stream (using xdr_destroy).
		*/
		virtual void finalizeInputStream();

		//@}

		/**	@name	Put methods for primitive data types.
		*/
		//@{
		
		/**	Write a signed char to the output stream.
		*/
		virtual void put(const char c);

		/**	Write a single byte the output stream.
		*/
		virtual void put(const Byte b);

		/**	Write an Index to the output stream.
		*/
		virtual void put(const Index i);

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

		/**	Write a pointer to the output.
		*/
		virtual void put(const PointerSizeInt p);
		//@}

		/**	@name	Get methods for primitive data types.
		*/
		//@{

		/**	Read a signed char from the input stream.
		*/
		virtual void get(char& c);

		/**	Read a single byte from the input stream.
		*/
		virtual void get(Byte& c);

		/**	Read an Index from the input stream.
		*/
		virtual void get(Index& s);

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

		private:

		/**	The XDR struct used to read from
		*/
		XDR		xdr_in_;
		
		/**	The XDR struct used to write to
		*/
		XDR		xdr_out_;
		
	};

} // namespace BALL

#endif // BALL_CONCEPT_XDRPERSISTENCEMANAGER_H
