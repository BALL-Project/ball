// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: XDRPersistenceManager.h,v 1.23 2005/02/06 09:44:53 oliver Exp $
//

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
			
			\ingroup  Persistence
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
		XDRPersistenceManager()
      throw();

		/**	Detailed constructor with an input stream
		*/
		XDRPersistenceManager(std::istream& is)
      throw();

		/**	Detailed constructor with an output stream
		*/
		XDRPersistenceManager(std::ostream& os)
      throw();

		/**	Detailed constructor with an input stream and an output stream.
		*/
		XDRPersistenceManager(std::istream& is, std::ostream& os)
      throw();
		//@}
		
		/**	@name	Layer 0 methods
		*/
		//@{

		/**	Write an object header.
				This method stores  \link OBJECT_HEADER OBJECT_HEADER \endlink  as an int value to mark the
				start of an object (using <tt>xdr_int</tt>).
		*/
		virtual void writeHeader(const char* type_name, const char* name, PointerSizeUInt ptr)
      throw();

		/**	Check for an object header.
				This method reads an int form the input stream (using <tt>xdr_int</tt>) and
				returns <b>true</b> if the value read equals  \link OBJECT_HEADER OBJECT_HEADER \endlink .
		*/
		virtual bool checkHeader(const char* type_name, const char* name, PointerSizeUInt& ptr)
      throw();

		/** Write an object trailer.
				This method stores  \link OBJECT_TRAILER OBJECT_TRAILER \endlink  as an int value to mark the
				start of an object (using <tt>xdr_int</tt>).
		*/
		virtual void writeTrailer(const char* name = 0)
      throw();

		/**	Check for an object trailer.
				This method reads an int form the input stream (using <tt>xdr_int</tt>) and
				returns <b>true</b> if the value read equals  \link OBJECT_TRAILER OBJECT_TRAILER \endlink .
		*/
		virtual bool checkTrailer(const char* name = 0)
      throw();

		/**	Write a start marker to the output stream.
		*/
		virtual void writeStreamHeader()
      throw();

		/**	Write an end marker to the output stream.
		*/
		virtual void writeStreamTrailer()
      throw();

		/**	Check for the start marker in the input stream.
		*/
		virtual bool checkStreamHeader()
      throw();

		/**	Check for the end marker in the output stream.
		*/
		virtual bool checkStreamTrailer()
      throw();

		/**	Get an (unknown) object header.
		*/
		virtual bool getObjectHeader(String& type_name, PointerSizeUInt& ptr)
      throw();

		/**	Write a variable/member name.
		*/
		virtual void writeName(const char* name)
      throw();

		/** Check for variable/member name.
		*/
		virtual bool checkName(const char* name)
      throw();

		/**	Write storable object header.
		*/
		virtual void writeStorableHeader(const char* type_name, const char* name)
      throw();
			
		/**	Check for storable object header.
		*/
		virtual bool checkStorableHeader(const char* type_name, const char* name)
      throw();

		/**	Write type header and name for a primitive type.
		*/
		virtual void writePrimitiveHeader(const char* type_name, const char* name)
      throw();

		/**	Check for a type header and name for a primitive type.
		*/
		virtual bool checkPrimitiveHeader(const char* type_name, const char* name)
      throw();

		/**	Write storable object trailer.
		*/
		virtual void writeStorableTrailer()
      throw();
			
		/**	Check for storable object trailer.
		*/
		virtual bool checkStorableTrailer()
      throw();

		/**	Write the trailer for a primitive type.
		*/
		virtual void writePrimitiveTrailer()
      throw();

		/**	Check the trailer of a primitive type.
		*/
		virtual bool checkPrimitiveTrailer()
      throw();

		/**	Write header for a pointer to a PersistentObject.
		*/
		virtual void writeObjectPointerHeader(const char* type_name, const char* name)
      throw();

		/**	Check for header for a pointer to a PersistentObject.
		*/
		virtual bool checkObjectPointerHeader(const char* type_name, const char* name)
      throw();

		/**	Write header for a reference to a PersistentObject.
		*/
		virtual void writeObjectReferenceHeader(const char* type_name, const char* name)
      throw();

		/**	Check for header for a reference to a PersistentObject.
		*/
		virtual bool checkObjectReferenceHeader(const char* type_name, const char* name)
      throw();

		/**	Write header for an array of pointers to PersistentObjects.
		*/
		virtual void writeObjectPointerArrayHeader(const char* type_name, const char* name, Size size)
      throw();

		/**	Check for header for an array of pointers to PersistentObjects.
		*/
		virtual bool checkObjectPointerArrayHeader(const char* type_name, const char* name, Size& size)
      throw();

		/**	Write trailer for an array of pointers to PersistentObjects.
		*/
		virtual void writeObjectPointerArrayTrailer()
      throw();

		/**	Check for trailer for an array of pointers to PersistentObjects.
		*/
		virtual bool checkObjectPointerArrayTrailer()
      throw();

		/**	Prepare the stream for output.
				This method creates an XDR output stream (using xdrrec_create) and
				prepares it for output.
		*/
		virtual void initializeOutputStream()
      throw();

		/**	Prepare the stream for closing.
				This method destroys the XDR output stream (using xdr_destroy).
		*/
		virtual void finalizeOutputStream()
      throw();

		/**	Prepare the stream for output.
				This method creates an XDR output stream (using xdrrec_create) and
				prepares it for output.
		*/
		virtual void initializeInputStream()
      throw();

		/**	Prepare the stream for closing.
				This method destroys the XDR output stream (using xdr_destroy).
		*/
		virtual void finalizeInputStream()
      throw();

		//@}

		/**	@name	Put methods for primitive data types.
		*/
		//@{
		
		/**	Write a signed char to the output stream.
		*/
		virtual void put(const char c)
      throw();

		/**	Write a single byte the output stream.
		*/
		virtual void put(const Byte b)
      throw();

		/**	Write an Index to the output stream.
		*/
		virtual void put(const Index i)
      throw();

		/**	Write a Size or a Position to the output stream.
		*/
		virtual void put(const Size s)
      throw();

		/**	Write a boolean value to the output stream.
		*/
		virtual void put(const bool b)
      throw();

		/**	Write a single precision floating point number to the output stream.
		*/
		virtual void put(const Real f)
      throw();

		/**	Write a double precision floating point number to the output stream.
		*/
		virtual void put(const DoubleReal d)
      throw();

		/**	Write a string to the output.
		*/
		virtual void put(const string& s)
      throw();

		/**	Write a pointer to the output.
		*/
		virtual void put(const LongSize p)
      throw();
		//@}

		/**	@name	Get methods for primitive data types.
		*/
		//@{

		/**	Read a signed char from the input stream.
		*/
		virtual void get(char& c)
      throw();

		/**	Read a single byte from the input stream.
		*/
		virtual void get(Byte& c)
      throw();

		/**	Read an Index from the input stream.
		*/
		virtual void get(Index& s)
      throw();

		/**	Read a Size or a Position from the input stream.
		*/
		virtual void get(Size& s)
      throw();

		/**	Read a boolean value from the input stream.
		*/
		virtual void get(bool& b)
      throw();

		/**	Read a single precision floating point number from the input stream.
		*/
		virtual void get(Real& f)
      throw();

		/**	Read a double precision floating point number from the input stream.
		*/
		virtual void get(DoubleReal& d)
      throw();

		/**	Read a string from the output stream.
		*/
		virtual void get(string& s)
      throw();

		/**	Read a pointer from the input stream.
		*/
		virtual void get(LongSize& p)
      throw();
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
