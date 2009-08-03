// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: XDRPersistenceManager.C,v 1.31 2005/12/23 17:02:33 amoll Exp $
//

#include <BALL/CONCEPT/XDRPersistenceManager.h>

#include <rpc/types.h>
#include <rpc/xdr.h>

//#define BALL_DEBUG_PERSISTENCE

#ifdef BALL_DEBUG_PERSISTENCE
#	define DEBUG(a) Log.info() << a << std::endl;
#else
# define DEBUG(a)
#endif

using namespace std;

namespace BALL 
{

#ifdef BALL_XDRREC_CREATE_VOID
	int (*XDRReadStreamPtr) () = 0;
	int (*XDRWriteStreamPtr) () = 0;
	int (*XDRErrorPtr) () = 0;
#endif

  // XDRReadStream_ reads characters from the stream
  // and hands them back to the xdr buffer.
#ifdef BALL_XDRREC_CREATE_VOID_VOID_INT
		extern "C" int XDRReadStream_(void* stream_ptr, void* buffer, int number)
#else
#ifdef BALL_XDRREC_CREATE_VOID_VOID_UINT
		extern "C" int XDRReadStream_(void* stream_ptr, void* buffer, unsigned int number)
#else
#ifdef BALL_XDRREC_CREATE_VOID_CHAR_INT
		extern "C" int XDRReadStream_(void* stream_ptr, char* buffer, int number)
#else
# ifdef BALL_XDRREC_CREATE_CHAR_CHAR_INT
		extern "C" int XDRReadStream_(char* stream_ptr, char* buffer, int number)
#	else
		extern "C" int XDRReadStream_(void* stream_ptr, void* buffer, unsigned int number)
#	endif
#endif
#endif
#endif
	{
		istream& is = *(istream*)stream_ptr;


		int number_read = 0;
		if (stream_ptr != 0 && !is.eof())
		{
			try 
			{
				for (; number_read < number && !is.eof(); ++number_read)
				{
					is.get(((char*)buffer)[number_read]);
				}
			}
			catch (...)	
			{
			}
			is.clear();
		}

		DEBUG("XDRPersistenceManager: read " << dec << number_read << " bytes.")

		return number_read;
	}

#ifdef BALL_XDRREC_CREATE_VOID_CHAR_INT
	extern "C" int XDRWriteStream_(void* stream_ptr, char* buffer, int number)
#else
#ifdef BALL_XDRREC_CREATE_VOID_VOID_INT
	extern "C" int XDRWriteStream_(void* stream_ptr, void* buffer, int number)
#else
#ifdef BALL_XDRREC_CREATE_VOID_VOID_UINT
	extern "C" int XDRWriteStream_(void* stream_ptr, void* buffer, unsigned int number)
#else
#ifdef BALL_XDRREC_CREATE_CHAR_CHAR_INT
	extern "C" int XDRWriteStream_(char* stream_ptr, char* buffer, int number)
#else
	extern "C" int XDRWriteStream_(void* stream_ptr, void* buffer, unsigned int number)
#endif
#endif
#endif
#endif
	{
		ostream& os = *(ostream*)stream_ptr;

		if (stream_ptr != 0)
		{
			char* buffer_ptr = (char*)buffer;
			os.write(buffer_ptr, number);
		}

		DEBUG("XDRPersistenceManager: wrote " << number << " bytes.")

		return number;
	}


#ifdef BALL_XDRREC_CREATE_VOID_CHAR_INT
	extern "C" int XDRError_(void* , char*, int)
#else
#ifdef BALL_XDRREC_CREATE_VOID_VOID_INT
	extern "C" int XDRError_(void* , void*, int)
#else
#ifdef BALL_XDRREC_CREATE_VOID_VOID_UINT
	extern "C" int XDRError_(void* , void*, unsigned int)
#else
#ifdef BALL_XDRREC_CREATE_CHAR_CHAR_INT
	extern "C" int XDRError_(char* , char*, int)
#else
	extern "C" int XDRError_(void* , void*, unsigned int)
#endif
#endif
#endif
#endif
	{
		Log.error() << "XDRPersistenceManager: error wrong access mode for XDR stream." << endl;

		return 0;
	}

	const Size XDRPersistenceManager::STREAM_HEADER  = 0xDEADBEEF;
	const Size XDRPersistenceManager::STREAM_TRAILER = 0xBEEFDEAD;
	const Size XDRPersistenceManager::OBJECT_HEADER  = 0x0BEC0BEC;
	const Size XDRPersistenceManager::OBJECT_TRAILER = 0xDEAD0BEC;

	XDRPersistenceManager::XDRPersistenceManager()
		:	PersistenceManager(),
			streams_handled_externally_(false)
	{
	}

	XDRPersistenceManager::XDRPersistenceManager(istream& is)
		:	PersistenceManager(is),
			streams_handled_externally_(false)
	{
	}

	XDRPersistenceManager::XDRPersistenceManager(ostream& os)
		:	PersistenceManager(os),
			streams_handled_externally_(false)
	{
	}

	XDRPersistenceManager::XDRPersistenceManager(istream& is, ostream& os)
		:	PersistenceManager(is, os),
			streams_handled_externally_(false)
	{
	}

  void XDRPersistenceManager::writeHeader(const char* type_name, const char* name, LongSize ptr)
  {
		DEBUG("XDRPersistenceManager: writing header for " << name << "/" << type_name)

    if ((name != 0) && (!strcmp(name, "")))
    {
			put(String(type_name));
		}
		put(ptr);
	}

	void XDRPersistenceManager::writeTrailer(const char* /* name */)
	{
		// write a trailer to identify the end of the stream
		put(OBJECT_TRAILER);
		DEBUG("XDRPersistenceManager: wrote object trailer")
	}

	void XDRPersistenceManager::writeStreamHeader()
	{
		if (ostr_ == 0)
		{
			Log.error() << "XDRPersistenceManager::writeStreamHeader: no output stream defined!" << std::endl;
			return;
		}

		put(STREAM_HEADER);

		DEBUG("XDRPersistenceManager: wrote stream header")
	}

	void XDRPersistenceManager::initializeOutputStream()
	{
		if (streams_handled_externally_)
			return;

		if (ostr_ == 0)
		{
			Log.error() << "XDRPersistenceManager::initializeOutputStream: no output stream defined!" << std::endl;
			return;
		}
		const caddr_t ostr_ptr = (caddr_t)ostr_;
		#ifdef BALL_XDRREC_CREATE_VOID
			*((void**)&XDRErrorPtr) = (void*)XDRError_;
			*((void**)&XDRWriteStreamPtr) = (void*)XDRWriteStream_;
			xdrrec_create(&xdr_out_, 0, 0, ostr_ptr, XDRErrorPtr, XDRWriteStreamPtr);
		#else
			xdrrec_create(&xdr_out_, 0, 0, ostr_ptr, XDRError_, XDRWriteStream_);
		#endif
		xdr_out_.x_op = XDR_ENCODE;

		DEBUG("XDRPersistenceManager: initialized output stream.")
	}

	void XDRPersistenceManager::initializeInputStream()
	{
		if (streams_handled_externally_)
			return;

		if (istr_ == 0)
		{
			Log.error() << "XDRPersistenceManager::initializeInputStream: no input stream defined!" << std::endl;
			return;
		}
		const caddr_t istr_ptr = (caddr_t)istr_;
		#ifdef BALL_XDRREC_CREATE_VOID
			*((void**)&XDRErrorPtr) = (void*)XDRError_;
			*((void**)&XDRReadStreamPtr) = (void*)XDRReadStream_;
			xdrrec_create(&xdr_in_, 0, 0, istr_ptr, XDRReadStreamPtr, XDRErrorPtr);
		#else
			xdrrec_create(&xdr_in_, 0, 0, istr_ptr, XDRReadStream_, XDRError_);
		#endif
		xdr_in_.x_op = XDR_DECODE;

		DEBUG("XDRPersistenceManager: initialized input stream.")
	}

	void XDRPersistenceManager::finalizeOutputStream()
	{
		if (!streams_handled_externally_)
			// destroy the XDR stream
			xdr_destroy(&xdr_out_);
		DEBUG("XDRPersistenceManager: finalized output stream.")
	}

	void XDRPersistenceManager::finalizeInputStream()
	{
		if (!streams_handled_externally_)
			// destroy the XDR stream 
			xdr_destroy(&xdr_in_);
		DEBUG("XDRPersistenceManager: finalized input stream.")
	}

	bool XDRPersistenceManager::checkStreamHeader()
	{
		DEBUG("XDRPersistenceManager: entering checkStreamHeader()")

		// skip to the next/first XDR record
		xdrrec_skiprecord(&xdr_in_);

		Size header = 0;
		xdr_u_int(&xdr_in_, &header);

		bool result = (header == STREAM_HEADER);

		DEBUG("XDRPersistenceManager: checkStreamHeader = " << result << " (" << hex << header << "/" << STREAM_HEADER << dec << ")")

		return result;
	}

	void XDRPersistenceManager::writeStreamTrailer()
	{
		// write a trailer to identify the end of the stream
		put(STREAM_TRAILER);
		xdrrec_endofrecord(&xdr_out_, 1);

		DEBUG("XDRPersistenceManager: wrote stream trailer")
	}

	bool XDRPersistenceManager::checkStreamTrailer()
	{
		DEBUG("XDRPersistenceManager: entering checkStreamTrailer()")

		// check for the correct trailer
		Size trailer;
		get(trailer);

		DEBUG("XDRPersistenceManager: checkStreamTrailer = " << (trailer == STREAM_TRAILER))

		return (trailer == STREAM_TRAILER);		
	}

#	ifdef BALL_DEBUG_PERSISTENCE
	bool XDRPersistenceManager::checkTrailer(const char* name) 
#	else
	bool XDRPersistenceManager::checkTrailer(const char* /* name */) 
#endif
	{
		DEBUG("XDRPersistenceManager: entering checkTrailer(" << (name == 0 ? "" : name) << ")")
		// check for the correct trailer
		Size trailer;
		get(trailer);

		DEBUG("XDRPersistenceManager: checkTrailer = " << (trailer == OBJECT_TRAILER))

		return (trailer == OBJECT_TRAILER);		
	}

	bool XDRPersistenceManager::checkHeader(const char* type_name, const char* name, LongSize& ptr)
	{
		DEBUG("XDRPersistenceManager: entering checkHeader(" << type_name << ", " << name << ")")
    if ((name != 0) && (!strcmp(name, "")))
		{
			String s;
			get(s);

			if (s != type_name) 
			{
				DEBUG("Expected object of type " << type_name << " but found definition for " << s << "!")

				return false;
			}
		}

		get(ptr);

		return true;
	}

	bool XDRPersistenceManager::getObjectHeader(String& type_name, LongSize& ptr)
	{
		DEBUG("XDRPersistenceManager: entering getObjectHeader()")

		get(type_name);

		DEBUG("XDRPersistenceManager: read type name: " << type_name)

		get(ptr);
		if (ptr == 0) 
		{
			DEBUG("Could not read a valid object pointer: " << dec << (unsigned int)ptr << "/" << hex << (unsigned int)ptr << "!")
			return false;
		} 
		else 
		{
			DEBUG("XDRPersistenceManager: read pointer: " << dec << (unsigned int)ptr 
								 << "/" << hex << (unsigned int)ptr)
		}

		return true;
	}

	void XDRPersistenceManager::writeName(const char* /* name */)
	{
	}

	bool XDRPersistenceManager::checkName(const char* /* name */)
	{
		DEBUG("XDRPersistenceManager: entering checkName()")
		return true;
	}

	void XDRPersistenceManager::writeStorableHeader(const char* /* type_name */, const char* /* name */)
	{
		DEBUG("XDRPersistenceManager: writeStorableHeader")
	}

	bool XDRPersistenceManager::checkStorableHeader(const char* /* type_name */, const char* /* name */)
	{
		DEBUG("XDRPersistenceManager: entering checkStorableHeader()")
		return true;
	}

	void XDRPersistenceManager::writeStorableTrailer()
	{
		DEBUG("XDRPersistenceManager: writeStorableTrailer")
	}

	bool XDRPersistenceManager::checkStorableTrailer()
	{
		DEBUG("XDRPersistenceManager: entering checkStorableTrailer()")
		return true;
	}

	void XDRPersistenceManager::writePrimitiveHeader(const char* /* type_name */, const char* /* name */)
	{
		DEBUG("XDRPersistenceManager: writePrimitiveHeader")
	}

	bool XDRPersistenceManager::checkPrimitiveHeader(const char* /* type_name */, const char* /* name */)
	{
		DEBUG("XDRPersistenceManager: entering checkPrimitiveHeader()")
		return true;
	}

	void XDRPersistenceManager::writePrimitiveTrailer()
	{
		DEBUG("XDRPersistenceManager: writePrimitiveTrailer")
	}

	bool XDRPersistenceManager::checkPrimitiveTrailer()
	{
		DEBUG("XDRPersistenceManager: entering checkPrimitiveTrailer()")
		return true;
	}

	void XDRPersistenceManager::writeObjectPointerHeader(const char* /* type_name */, const char* /* name */)
	{
		DEBUG("XDRPersistenceManager: writeObjectPointerHeader")
	}

	bool XDRPersistenceManager::checkObjectPointerHeader(const char* /* type_name */, const char* /* name */)
	{
		DEBUG("XDRPersistenceManager: entering checkObjectPointerHeader()")
		return true;
	}

	void XDRPersistenceManager::writeObjectReferenceHeader(const char* /* type_name */, const char* /* name */)
	{
		DEBUG("XDRPersistenceManager: writeObjectReferenceHeader")
	}

	bool XDRPersistenceManager::checkObjectReferenceHeader(const char* /* type_name */, const char* /* name */)
	{
		DEBUG("XDRPersistenceManager: entering checkObjectReferenceHeader()")
		return true;
	}

	void XDRPersistenceManager::writeObjectPointerArrayHeader
		(const char* /* type_name */, const char* /* name */, Size size)
	{
		put(size);
		DEBUG("XDRPersistenceManager: writeObjectPointerArrayHeader (size = " << size << ")")
	}

	bool XDRPersistenceManager::checkObjectPointerArrayHeader
		(const char* /* type_name */, const char* /* name */, Size& size)
	{
		get(size);

		DEBUG("XDRPersistenceManager: checkObjectPointerArrayHeader (size = " << size << ")")
		return true;
	}

	void XDRPersistenceManager::writeObjectPointerArrayTrailer()
	{
		DEBUG("XDRPersistenceManager: entering writeObjectPointerArrayTrailer()")
	}

	bool XDRPersistenceManager::checkObjectPointerArrayTrailer()
	{
		DEBUG("XDRPersistenceManager: entering checkObjectPointerArrayTrailer()")
		return true;
	}

	/// Layer 0: primitive put methods
	void XDRPersistenceManager::put(const char c)
	{
		char* char_ptr = const_cast<char*>(&c);
		xdr_char(&xdr_out_, char_ptr);
		DEBUG("XDRPersistenceManager: put(char = " << (int)c << ")")
	}

	void XDRPersistenceManager::put(const Byte b)
	{
		unsigned char* char_ptr = const_cast<unsigned char*>(&b);
		xdr_u_char(&xdr_out_, char_ptr);
		DEBUG("XDRPersistenceManager: put(Byte = " << b << ")")
	}

	void XDRPersistenceManager::put(const bool b)
	{		
		char c = b ? (char)1 : (char)0;
		xdr_char(&xdr_out_, &c);
		DEBUG("XDRPersistenceManager: put(bool = " << b << ")")
	}

	void XDRPersistenceManager::put(const Index i)
	{
		Index* index_ptr = const_cast<Index*>(&i);
		xdr_int(&xdr_out_, index_ptr);
		DEBUG("XDRPersistenceManager: put(Index = " << i << ")")
	}

	void XDRPersistenceManager::put(const Size s)
	{
		Size* size_ptr = const_cast<Size*>(&s);
		xdr_u_int(&xdr_out_, size_ptr);
		DEBUG("XDRPersistenceManager: put(Size = " << s << ")")
	}

	void XDRPersistenceManager::put(const Real x)
	{
		Real* real_ptr = const_cast<Real*>(&x);
		xdr_float(&xdr_out_, real_ptr);
		DEBUG("XDRPersistenceManager: put(Real = " << x << ")")
	}

	void XDRPersistenceManager::put(const DoubleReal x)
	{
		DoubleReal* double_ptr = const_cast<DoubleReal*>(&x);
		xdr_double(&xdr_out_, double_ptr);
		DEBUG("XDRPersistenceManager: put(DoubleReal = " << x << ")")
	}

	void XDRPersistenceManager::put(const LongSize ptr)
	{
#   ifdef BALL_HAS_XDR_U_HYPER
			BALL_XDR_UINT64_TYPE* p = (BALL_XDR_UINT64_TYPE*)&ptr;
			xdr_u_hyper(&xdr_out_, p);
#   else
			Size t1, t2;
			t1 = (Size)(ptr >> 32);
			t2 = (Size)(ptr);

			XDR_PUTLONG(&xdr_out_, &t1);
			XDR_PUTLONG(&xdr_out_, &t2);
#   endif

		DEBUG("XDRPersistenceManager: put(LongSize = " << ptr << ")")
	}

	void XDRPersistenceManager::put(const string& s)
	{
		char* ptr = const_cast<char*>(s.c_str());
		xdr_string(&xdr_out_, &ptr, ((int)s.size()));

		DEBUG("XDRPersistenceManager: put(string = `" << s << "')")
	}

	void XDRPersistenceManager::get(char& c)
	{
		xdr_char(&xdr_in_, &c);

		DEBUG("XDRPersistenceManager: read char: " << (int)c)
	}

	void XDRPersistenceManager::get(Byte& c)
	{
		xdr_u_char(&xdr_in_, &c);

		DEBUG("XDRPersistenceManager: read Byte: " << c)
	}

	void XDRPersistenceManager::get(bool& b)
	{
		char c;
		xdr_char(&xdr_in_, &c);
		b = (c == (char)1);

		DEBUG("XDRPersistenceManager: read bool: " << (b ? "true" : "false"))
	}

	void XDRPersistenceManager::get(string& s)
	{
		static vector<char> buf(65536);
		char* ptr = &(buf[0]);
		if (xdr_string(&xdr_in_, &ptr, 65535))
		{
			s = ptr;
		}
		else
		{
			DEBUG("XDRPersistenceManager: failed to read string!")
			s.erase();
		}

		DEBUG("XDRPersistenceManager: read string: " << s)
	}

	void XDRPersistenceManager::get(Index& i)
	{
		xdr_int(&xdr_in_, &i);
		DEBUG("XDRPersistenceManager: read int: " << i)
	}

	void XDRPersistenceManager::get(Size& s)
	{
		xdr_u_int(&xdr_in_, &s);
		DEBUG("XDRPersistenceManager: read unsigned int: " << s << "/" << hex << s)
	}

	void XDRPersistenceManager::get(Real& x)
	{
		xdr_float(&xdr_in_, &x);
		DEBUG("XDRPersistenceManager: read Real: " << x)
	}

	void XDRPersistenceManager::get(DoubleReal& x)
	{
		xdr_double(&xdr_in_, &x);
		DEBUG("XDRPersistenceManager: read DoubleReal: " << x)
	}

	void XDRPersistenceManager::get(LongSize& ptr)
	{
#   ifdef BALL_HAS_XDR_U_HYPER
		BALL_XDR_UINT64_TYPE* p = (BALL_XDR_UINT64_TYPE*)&ptr;
		xdr_u_hyper(&xdr_in_, p);
#   else
		Size t1, t2;

		XDR_GETLONG(&xdr_in_, &t1);
		XDR_GETLONG(&xdr_in_, &t2);

		ptr  = ((LongSize)t1) << 32;
		ptr |= (Size) t2; 
#   endif

		DEBUG("XDRPersistenceManager: get ptr: " << hex << ptr)
	}

	void XDRPersistenceManager::setHandleStreamsExternally(bool handle_externally)
	{
		streams_handled_externally_ = handle_externally;
	}

} // namespace BALL
