// $Id: XDRPersistenceManager.C,v 1.19 2001/07/15 21:07:03 oliver Exp $

#include <BALL/CONCEPT/XDRPersistenceManager.h>

#include <rpc/types.h>
#include <rpc/xdr.h>

// #define BALL_DEBUG_PERSISTENCE


using namespace std;

namespace BALL 
{

#ifdef BALL_XDRREC_CREATE_VOID
	extern "C" int (*XDRReadStreamPtr) () = 0;
	extern "C" int (*XDRWriteStreamPtr) () = 0;
	extern "C" int (*XDRErrorPtr) () = 0;
#endif

	

#ifdef BALL_XDRREC_CREATE_VOID_CHAR_INT
		extern "C" int XDRReadStream_(void* stream_ptr, char* buffer, int number)
			throw()
#else
# ifdef BALL_XDRREC_CREATE_CHAR_CHAR_INT
		extern "C" int XDRReadStream_(char* stream_ptr, char* buffer, int number)
			throw()
#	else
		extern "C" int XDRReadStream_(void* stream_ptr, void* buffer, unsigned int number)
			throw()
#	endif
#endif
	{
		istream& is = *(istream*)stream_ptr;

		streampos number_read = is.gcount();
		if (stream_ptr != 0)
		{
			is.get((char*)buffer, number);
		}
		number_read = is.gcount() - number_read;

#ifdef BALL_DEBUG_PERSISTENCE
		Log.info() << "XDRPersistenceManager: read " << number_read << " bytes." << endl;
#endif

		return (int)(long)number_read;
	}

#ifdef BALL_XDRREC_CREATE_VOID_CHAR_INT
	extern "C" int XDRWriteStream_(void* stream_ptr, char* buffer, int number)
		throw()
#else
#	ifdef BALL_XDRREC_CREATE_CHAR_CHAR_INT
	extern "C" int XDRWriteStream_(char* stream_ptr, char* buffer, int number)
		throw()
#	else
	extern "C" int XDRWriteStream_(void* stream_ptr, void* buffer, unsigned int number)
		throw()
#	endif
#endif
	{
		ostream& os = *(ostream*)stream_ptr;
		
		if (stream_ptr != 0)
		{
			char* buffer_ptr = (char*)buffer;
			os.write(buffer_ptr, number);
		}

#ifdef BALL_DEBUG_PERSISTENCE
		Log.info() << "XDRPersistenceManager: wrote " << number << " bytes." << endl;
#endif

		return number;
	}


#ifdef BALL_XDRREC_CREATE_VOID_CHAR_INT
	extern "C" int XDRError_(void* , char*, int)
		throw()
#else
#	ifdef BALL_XDRREC_CREATE_CHAR_CHAR_INT
	extern "C" int XDRError_(char* , char*, int)
		throw()
#	else
	extern "C" int XDRError_(void* , void*, unsigned int)
		throw()
#	endif
#endif
	{
		Log.error() << "XDRPersistenceManager: error wrong access mode for XDR stream." << endl;

		return 0;
	}

	const Size XDRPersistenceManager::STREAM_HEADER = 0xDEADBEEF;
	const Size XDRPersistenceManager::STREAM_TRAILER = 0x0000FFFF;
	

	XDRPersistenceManager::XDRPersistenceManager()
		throw()
		:	PersistenceManager()
	{
	}

	XDRPersistenceManager::XDRPersistenceManager(istream& is)
		throw()
		:	PersistenceManager(is)
	{
	}

	XDRPersistenceManager::XDRPersistenceManager(ostream& os)
		throw()
		:	PersistenceManager(os)
	{
	}

	XDRPersistenceManager::XDRPersistenceManager(istream& is, ostream& os)
		throw()
		:	PersistenceManager(is, os)
	{
	}

  void XDRPersistenceManager::writeHeader(const char* type_name, const char* name, PointerSizeInt ptr)
		throw()
  {
#ifdef BALL_DEBUG_PERSISTENCE
		Log.info() << "XDRPersistenceManager: writing header for " << name << "/" << type_name << endl;
#endif

    if ((name != 0) && (!strcmp(name, "")))
    {
			put(String(type_name));
		}
		put(ptr);
	}
 
	void XDRPersistenceManager::writeTrailer(const char* /* name */)
		throw()
	{
	}

	void XDRPersistenceManager::writeStreamHeader()
		throw()
	{
		if (ostr_ == 0)
		{
			Log.error() << "XDRPersistenceManager::writeStreamHeader: no output stream defined!" << std::endl;
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
		put(STREAM_HEADER);

#ifdef BALL_DEBUG_PERSISTENCE
		Log.info() << "XDRPersistenceManager: wrote stream header" << endl;
#endif
	}

	void XDRPersistenceManager::initializeOutputStream()
		throw()
	{
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

#ifdef BALL_DEBUG_PERSISTENCE
		Log.info() << "XDRPersistenceManager: initialized output stream." << endl;
#endif
	}

	void XDRPersistenceManager::initializeInputStream()
		throw()
	{
		if (istr_ == 0)
		{
			Log.error() << "XDRPersistenceManager::initializeInputStream: no input stream defined!" << std::endl;
			return;
		}
		const caddr_t istr_ptr = (caddr_t)istr_;
		#ifdef BALL_XDRREC_CREATE_VOID
			*((void**)&XDRErrorPtr) = (void*)XDRError_;
			*((void**)&XDRReadStreamPtr) = (void*)XDRReadStream_;
			xdrrec_create(&xdr_out_, 0, 0, istr_ptr, XDRReadStreamPtr, XDRErrorPtr);
		#else
			xdrrec_create(&xdr_in_, 0, 0, istr_ptr, XDRReadStream_, XDRError_);
		#endif
		xdr_in_.x_op = XDR_DECODE;

#ifdef BALL_DEBUG_PERSISTENCE
		Log.info() << "XDRPersistenceManager: initialized input stream." << endl;
#endif
	}

	void XDRPersistenceManager::finalizeOutputStream()
		throw()
	{
		// destroy the XDR stream
		xdr_destroy(&xdr_out_);
#ifdef BALL_DEBUG_PERSISTENCE
		Log.info() << "XDRPersistenceManager: finalized output stream." << endl;
#endif
	}

	void XDRPersistenceManager::finalizeInputStream()
		throw()
	{
		// destroy the XDR stream 
		// xdr_destroy(&xdr_in_); //BAUSTELLE
#ifdef BALL_DEBUG_PERSISTENCE
		Log.info() << "XDRPersistenceManager: finalized input stream." << endl;
#endif
	}

	bool XDRPersistenceManager::checkStreamHeader()
		throw()
	{
#		ifdef BALL_DEBUG_PERSISTENCE
			Log.info() << "XDRPersistenceManager: entering checkStreamHeader()" << endl;
#		endif

		// skip to the next/first XDR record
		xdrrec_skiprecord(&xdr_in_);

		Size header = 0;
		xdr_u_int(&xdr_in_, &header);

		bool result = (header == STREAM_HEADER);

#ifdef BALL_DEBUG_PERSISTENCE
		Log.info() << "XDRPersistenceManager: checkStreamHeader = " << result << " (" << hex << header << "/" << STREAM_HEADER << ")" << endl;
#endif

		return result;
	}

	void XDRPersistenceManager::writeStreamTrailer()
		throw()
	{
		// write a trailer to identify the end of the stream
		put(STREAM_TRAILER);
		xdrrec_endofrecord(&xdr_out_, 1);

#ifdef BALL_DEBUG_PERSISTENCE
		Log.info() << "XDRPersistenceManager: wrote stream trailer" << endl;
#endif
	}

	bool XDRPersistenceManager::checkStreamTrailer()
		throw()
	{
#		ifdef BALL_DEBUG_PERSISTENCE
			Log.info() << "XDRPersistenceManager: entering checkStreamTrailer()" << endl;
#		endif

		// check for the correct trailer
		Size trailer;
		get(trailer);

		// destroy the XDR stream
		xdr_destroy(&xdr_in_);

#ifdef BALL_DEBUG_PERSISTENCE
		Log.info() << "XDRPersistenceManager: checkStreamTrailer = " << (trailer == STREAM_TRAILER) << endl;
#endif

		return (trailer == STREAM_TRAILER);		
	}

#	ifdef BALL_DEBUG_PERSISTENCE
	bool XDRPersistenceManager::checkTrailer(const char* name) 
		throw()
#	else
	bool XDRPersistenceManager::checkTrailer(const char* /* name */) 
		throw()
#endif
	{
#		ifdef BALL_DEBUG_PERSISTENCE
			if (name != 0)
			{
				Log.info() << "XDRPersistenceManager: entering checkTrailer(" << name << ")" << endl;
			}
			else
			{
				Log.info() << "XDRPersistenceManager: entering checkTrailer()" << endl;	
			}
#		endif
		return true;
	}

	bool XDRPersistenceManager::checkHeader(const char* type_name, const char* name, PointerSizeInt& ptr)
		throw()
	{
#		ifdef BALL_DEBUG_PERSISTENCE
			Log.info() << "XDRPersistenceManager: entering checkHeader(" << type_name << ", " << name << ")" << endl;
#		endif
    if ((name != 0) && (!strcmp(name, "")))
		{
			String s;
			get(s);

			if (s != type_name) 
			{
#				ifdef BALL_DEBUG_PERSISTENCE
					Log.error() << "Expected object of type " << type_name 
											<< " but found definition for " << s << "!" << endl;
#				endif

				return false;
			}
		}
		
		get(ptr);

		return true;
	}

	bool XDRPersistenceManager::getObjectHeader(String& type_name, PointerSizeInt& ptr)
		throw()
	{
#		ifdef BALL_DEBUG_PERSISTENCE
			Log.info() << "XDRPersistenceManager: entering getObjectHeader()" << endl;
#		endif

		get(type_name);

#		ifdef BALL_DEBUG_PERSISTENCE
			Log.info() << "XDRPersistenceManager: read type name: " << type_name << endl;
#		endif

		get(ptr);
		if (ptr == 0) 
		{
#		ifdef BALL_DEBUG_PERSISTENCE
			Log.level(LogStream::ERROR) << "Could not read a valid object pointer: " 
																	<< dec << (unsigned int)ptr << "!" << endl;
#		endif

			return false;
		} 
#		ifdef BALL_DEBUG_PERSISTENCE
		else 
		{
			Log.info() << "XDRPersistenceManager: read pointer: " << ptr << endl;
		}
#		endif

		return true;
	}

	void XDRPersistenceManager::writeName(const char* /* name */)
		throw()
	{
	}

	bool XDRPersistenceManager::checkName(const char* /* name */)
		throw()
	{
#		ifdef BALL_DEBUG_PERSISTENCE
			Log.info() << "XDRPersistenceManager: entering checkName()" << endl;
#		endif

		return true;
	}

	void XDRPersistenceManager::writeStorableHeader(const char* /* type_name */, const char* /* name */)
		throw()
	{
#ifdef BALL_DEBUG_PERSISTENCE
		Log.info() << "XDRPersistenceManager: writeStorableHeader" << endl;
#endif
	}

	bool XDRPersistenceManager::checkStorableHeader(const char* /* type_name */, const char* /* name */)
		throw()
	{
#		ifdef BALL_DEBUG_PERSISTENCE
			Log.info() << "XDRPersistenceManager: entering checkStorableHeader()" << endl;
#		endif

		return true;
	}

	void XDRPersistenceManager::writeStorableTrailer()
		throw()
	{
#ifdef BALL_DEBUG_PERSISTENCE
		Log.info() << "XDRPersistenceManager: writeStorableTrailer" << endl;
#endif
	}

	bool XDRPersistenceManager::checkStorableTrailer()
		throw()
	{
#		ifdef BALL_DEBUG_PERSISTENCE
			Log.info() << "XDRPersistenceManager: entering checkStorableTrailer()" << endl;
#		endif

		return true;
	}

	void XDRPersistenceManager::writePrimitiveHeader(const char* /* type_name */, const char* /* name */)
		throw()
	{
#ifdef BALL_DEBUG_PERSISTENCE
		Log.info() << "XDRPersistenceManager: writePrimitiveHeader" << endl;
#endif
	}

	bool XDRPersistenceManager::checkPrimitiveHeader(const char* /* type_name */, const char* /* name */)
		throw()
	{
#		ifdef BALL_DEBUG_PERSISTENCE
			Log.info() << "XDRPersistenceManager: entering checkPrimitiveHeader()" << endl;
#		endif

		return true;
	}

	void XDRPersistenceManager::writePrimitiveTrailer()
		throw()
	{
#ifdef BALL_DEBUG_PERSISTENCE
		Log.info() << "XDRPersistenceManager: writePrimitiveTrailer" << endl;
#endif
	}

	bool XDRPersistenceManager::checkPrimitiveTrailer()
		throw()
	{
#		ifdef BALL_DEBUG_PERSISTENCE
			Log.info() << "XDRPersistenceManager: entering checkPrimitiveTrailer()" << endl;
#		endif

		return true;
	}

	void XDRPersistenceManager::writeObjectPointerHeader(const char* /* type_name */, const char* /* name */)
		throw()
	{
#ifdef BALL_DEBUG_PERSISTENCE
		Log.info() << "XDRPersistenceManager: writeObjectPointerHeader" << endl;
#endif
	}
	
	bool XDRPersistenceManager::checkObjectPointerHeader(const char* /* type_name */, const char* /* name */)
		throw()
	{
#		ifdef BALL_DEBUG_PERSISTENCE
			Log.info() << "XDRPersistenceManager: entering checkObjectPointerHeader()" << endl;
#		endif

		return true;
	}
	
	void XDRPersistenceManager::writeObjectReferenceHeader(const char* /* type_name */, const char* /* name */)
		throw()
	{
#ifdef BALL_DEBUG_PERSISTENCE
		Log.info() << "XDRPersistenceManager: writeObjectReferenceHeader" << endl;
#endif
	}
	
	bool XDRPersistenceManager::checkObjectReferenceHeader(const char* /* type_name */, const char* /* name */)
		throw()
	{
#		ifdef BALL_DEBUG_PERSISTENCE
			Log.info() << "XDRPersistenceManager: entering checkObjectReferenceHeader()" << endl;
#		endif

		return true;
	}
	
	void XDRPersistenceManager::writeObjectPointerArrayHeader
		(const char* /* type_name */, const char* /* name */, Size size)
		throw()
	{
		put(size);
#ifdef BALL_DEBUG_PERSISTENCE
		Log.info() << "XDRPersistenceManager: writeObjectPointerArrayHeader (size = " << size << ")" << endl;
#endif
	}

	bool XDRPersistenceManager::checkObjectPointerArrayHeader
		(const char* /* type_name */, const char* /* name */, Size& size)
		throw()
	{
		get(size);

#ifdef BALL_DEBUG_PERSISTENCE
		Log.info() << "XDRPersistenceManager: checkObjectPointerArrayHeader (size = " << size << ")" << endl;
#endif

		return true;
	}
	
	void XDRPersistenceManager::writeObjectPointerArrayTrailer()
		throw()
	{
#		ifdef BALL_DEBUG_PERSISTENCE
			Log.info() << "XDRPersistenceManager: entering writeObjectPointerArrayTrailer()" << endl;
#		endif
	}
			
	bool XDRPersistenceManager::checkObjectPointerArrayTrailer()
		throw()
	{
#		ifdef BALL_DEBUG_PERSISTENCE
			Log.info() << "XDRPersistenceManager: entering checkObjectPointerArrayTrailer()" << endl;
#		endif

		return true;
	}

	/// Layer 0: primitive put methods
	void XDRPersistenceManager::put(const char c)
		throw()
	{
		char* char_ptr = const_cast<char*>(&c);
		xdr_char(&xdr_out_, char_ptr);

#		ifdef BALL_DEBUG_PERSISTENCE
			Log.info() << "XDRPersistenceManager: put(char = " << (int)c << ")" << endl;
#		endif		
	}

	void XDRPersistenceManager::put(const Byte b)
		throw()
	{
		unsigned char* char_ptr = const_cast<unsigned char*>(&b);
		xdr_u_char(&xdr_out_, char_ptr);

#		ifdef BALL_DEBUG_PERSISTENCE
			Log.info() << "XDRPersistenceManager: put(Byte = " << b << ")" << endl;
#		endif		
	}

	void XDRPersistenceManager::put(const bool b)
		throw()
	{		
		char c = b ? (char)1 : (char)0;
		xdr_char(&xdr_out_, &c);

#		ifdef BALL_DEBUG_PERSISTENCE
			Log.info() << "XDRPersistenceManager: put(bool = " << b << ")" << endl;
#		endif		
	}

	void XDRPersistenceManager::put(const Index i)
		throw()
	{
		Index* index_ptr = const_cast<Index*>(&i);
		xdr_int(&xdr_out_, index_ptr);

#		ifdef BALL_DEBUG_PERSISTENCE
			Log.info() << "XDRPersistenceManager: put(Index = " << i << ")" << endl;
#		endif		
	}

	void XDRPersistenceManager::put(const Size s)
		throw()
	{
		Size* size_ptr = const_cast<Size*>(&s);
		xdr_u_int(&xdr_out_, size_ptr);

#		ifdef BALL_DEBUG_PERSISTENCE
			Log.info() << "XDRPersistenceManager: put(Size = " << s << ")" << endl;
#		endif		
	}

	void XDRPersistenceManager::put(const Real x)
		throw()
	{
		Real* real_ptr = const_cast<Real*>(&x);
		xdr_float(&xdr_out_, real_ptr);

#		ifdef BALL_DEBUG_PERSISTENCE
			Log.info() << "XDRPersistenceManager: put(Real = " << x << ")" << endl;
#		endif		
	}

	void XDRPersistenceManager::put(const DoubleReal x)
		throw()
	{
		DoubleReal* double_ptr = const_cast<DoubleReal*>(&x);
		xdr_double(&xdr_out_, double_ptr);

#		ifdef BALL_DEBUG_PERSISTENCE
			Log.info() << "XDRPersistenceManager: put(DoubleReal = " << x << ")" << endl;
#		endif		
	}

	void XDRPersistenceManager::put(const PointerSizeInt ptr)
		throw()
	{
#   ifdef BALL_HAS_XDR_U_HYPER
			BALL_XDR_UINT64_TYPE* p = (BALL_XDR_UINT64_TYPE*)&ptr;
			xdr_u_hyper(&xdr_out_, p);
#   else
			Size* p = (Size*)&ptr;
			xdr_u_int(&xdr_out_, p);
			p++;
			xdr_u_int(&xdr_out_, p);
#   endif

#		ifdef BALL_DEBUG_PERSISTENCE
			Log.info() << "XDRPersistenceManager: put(PointerSizeInt = " << ptr << ")" << endl;
#		endif		
	}

	void XDRPersistenceManager::put(const string& s)
		throw()
	{
		char* ptr = const_cast<char*>(s.c_str());
		xdr_string(&xdr_out_, &ptr, ((int)s.size()));

#		ifdef BALL_DEBUG_PERSISTENCE
			Log.info() << "XDRPersistenceManager: put(string = `" << s << "')" << endl;
#		endif		
	}

	void XDRPersistenceManager::get(char& c)
		throw()
	{
		xdr_char(&xdr_in_, &c);

#		ifdef BALL_DEBUG_PERSISTENCE
		Log.info() << "XDRPersistenceManager: read char: " << c << endl;
#		endif		
	}

	void XDRPersistenceManager::get(Byte& c)
		throw()
	{
		xdr_u_char(&xdr_in_, &c);

#		ifdef BALL_DEBUG_PERSISTENCE
		Log.info() << "XDRPersistenceManager: read Byte: " << c << endl;
#		endif		
	}

	void XDRPersistenceManager::get(bool& b)
		throw()
	{
		char c;
		xdr_char(&xdr_in_, &c);
		b = (c == (char)1);

#		ifdef BALL_DEBUG_PERSISTENCE
		Log.info() << "XDRPersistenceManager: read bool: " << b << endl;
#		endif		
	}

	void XDRPersistenceManager::get(string& s)
		throw()
	{
		static char buf[65536];
		char* ptr = &(buf[0]);
		xdr_string(&xdr_in_, &ptr, 65535);
		s = ptr;

#		ifdef BALL_DEBUG_PERSISTENCE
		Log.info() << "XDRPersistenceManager: read string: " << s << endl;
#		endif		
	}

	void XDRPersistenceManager::get(Index& i)
		throw()
	{
		xdr_int(&xdr_in_, &i);

#		ifdef BALL_DEBUG_PERSISTENCE
		Log.info() << "XDRPersistenceManager: read int: " << i << endl;
#		endif		
	}

	void XDRPersistenceManager::get(Size& s)
		throw()
	{
		xdr_u_int(&xdr_in_, &s);

#		ifdef BALL_DEBUG_PERSISTENCE
		Log.info() << "XDRPersistenceManager: read unsigned int: " << s << "/" << hex << s << endl;
#		endif		
	}

	void XDRPersistenceManager::get(Real& x)
		throw()
	{
		xdr_float(&xdr_in_, &x);

#		ifdef BALL_DEBUG_PERSISTENCE
		Log.info() << "XDRPersistenceManager: read Real: " << x << endl;
#		endif		
	}

	void XDRPersistenceManager::get(DoubleReal& x)
		throw()
	{
		xdr_double(&xdr_in_, &x);

#		ifdef BALL_DEBUG_PERSISTENCE
		Log.info() << "XDRPersistenceManager: read DoubleReal: " << x << endl;
#		endif		
	}

	void XDRPersistenceManager::get(PointerSizeInt& ptr)
		throw()
	{
#   ifdef BALL_HAS_XDR_U_HYPER
		BALL_XDR_UINT64_TYPE* p = (BALL_XDR_UINT64_TYPE*)&ptr;
		xdr_u_hyper(&xdr_in_, p);
#   else
		Size* p = (Size*)&ptr;
		xdr_u_int(&xdr_in_, p);
		p++;
		xdr_u_int(&xdr_in_, p);
#   endif

#		ifdef BALL_DEBUG_PERSISTENCE
		Log.info() << "XDRPersistenceManager: get ptr: " << hex << ptr << endl;
#		endif		
	}

} // namespace BALL
