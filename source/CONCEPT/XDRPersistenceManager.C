// $Id: XDRPersistenceManager.C,v 1.13 2000/12/12 16:21:32 oliver Exp $

#include <BALL/CONCEPT/XDRPersistenceManager.h>

#define BALL_DEBUG_PERSISTENCE

using namespace std;

namespace BALL 
{

#ifdef BALL_XDRREC_CREATE_VOID_CHAR_INT
	extern "C" int XDRReadStream_(void* stream_ptr, char* buffer, int number)
#else 
#	ifdef BALL_XDRREC_CREATE_CHAR_CHAR_INT
	extern "C" int XDRReadStream_(char* stream_ptr, char* buffer, int number)
#	else
	extern "C" int XDRReadStream_(void* stream_ptr, void* buffer, unsigned int number)
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

		Log.info() << "read " << number_read << " bytes." << endl;

		return (int)number_read;
	}

#ifdef BALL_XDRREC_CREATE_VOID_CHAR_INT
	extern "C" int XDRWriteStream_(void* stream_ptr, char* buffer, int number)
#else
#	ifdef BALL_XDRREC_CREATE_CHAR_CHAR_INT
	extern "C" int XDRWriteStream_(char* stream_ptr, char* buffer, int number)
#	else
	extern "C" int XDRWriteStream_(void* stream_ptr, void* buffer, unsigned int number)
#	endif
#endif
	{
		ostream& os = *(ostream*)stream_ptr;
		
		if (stream_ptr != 0)
		{
			char* buffer_ptr = (char*)buffer;
			os.write(buffer_ptr, number);
		}

		Log.info() << "wrote " << number << " bytes." << endl;
		return number;
	}


#ifdef BALL_XDRREC_CREATE_VOID_CHAR_INT
	extern "C" int XDRError_(void* , char*, int)
#else
#	ifdef BALL_XDRREC_CREATE_CHAR_CHAR_INT
	extern "C" int XDRError_(char* , char*, int)
#	else
	extern "C" int XDRError_(void* , void*, unsigned int)
#	endif
#endif
	{
		Log.error() << "XDRPersistenceManager: error wrong access mode for XDR stream." << endl;

		return 0;
	}

	const Size XDRPersistenceManager::STREAM_HEADER = 0xDEADBEEF;
	const Size XDRPersistenceManager::STREAM_TRAILER = 0x0000FFFF;
	

	XDRPersistenceManager::XDRPersistenceManager()
		:	PersistenceManager()
	{
	}

	XDRPersistenceManager::XDRPersistenceManager(istream& is)
		:	PersistenceManager(is)
	{
	}

	XDRPersistenceManager::XDRPersistenceManager(ostream& os)
		:	PersistenceManager(os)
	{
	}

	XDRPersistenceManager::XDRPersistenceManager(istream& is, ostream& os)
		:	PersistenceManager(is, os)
	{
	}

  void XDRPersistenceManager::writeHeader(const char* type_name, const char* name, PointerSizeInt ptr)
  {
		Log.info() << "writing header for " << name << "/" << type_name << endl;
    if ((name != 0) && (!strcmp(name, "")))
    {
			put(String(type_name));
		}
		put(ptr);
	}
 
	void XDRPersistenceManager::writeTrailer(const char* /* name */)
	{
	}

	void XDRPersistenceManager::writeStreamHeader()
	{
		if (ostr_ == 0)
		{
			Log.error() << "XDRPersistenceManager::writeStreamHeader: no output stream defined!" << std::endl;
			return;
		}
		const caddr_t ostr_ptr = (caddr_t)ostr_;
		xdrrec_create(&xdr_out_, 0, 0, ostr_ptr, XDRError_, XDRWriteStream_);
		xdr_out_.x_op = XDR_ENCODE;
		put(STREAM_HEADER);

		Log.info() << "wrote stream header" << endl;
	}

	void XDRPersistenceManager::initializeOutputStream()
	{
		if (ostr_ == 0)
		{
			Log.error() << "XDRPersistenceManager::initializeOutputStream: no output stream defined!" << std::endl;
			return;
		}
		const caddr_t ostr_ptr = (caddr_t)ostr_;
		xdrrec_create(&xdr_out_, 0, 0, ostr_ptr, XDRError_, XDRWriteStream_);
		xdr_out_.x_op = XDR_ENCODE;
		Log.info() << "initialized output stream." << endl;
	}

	void XDRPersistenceManager::initializeInputStream()
	{
		if (istr_ == 0)
		{
			Log.error() << "XDRPersistenceManager::initializeInputStream: no input stream defined!" << std::endl;
			return;
		}
		const caddr_t istr_ptr = (caddr_t)istr_;
		xdrrec_create(&xdr_in_, 0, 0, istr_ptr, XDRReadStream_, XDRError_);
		xdr_in_.x_op = XDR_DECODE;
		Log.info() << "initialized input stream." << endl;
	}

	void XDRPersistenceManager::finalizeOutputStream()
	{
		// destroy the XDR stream
		xdr_destroy(&xdr_out_);
		Log.info() << "finalized output stream." << endl;
	}

	void XDRPersistenceManager::finalizeInputStream()
	{
		// destroy the XDR stream 
		// xdr_destroy(&xdr_in_); //BAUSTELLE
		Log.info() << "finalized input stream." << endl;
	}

	bool XDRPersistenceManager::checkStreamHeader()
	{
#		ifdef BALL_DEBUG_PERSISTENCE
			Log.info() << "entering checkStreamHeader()" << endl;
#		endif

		// skip to the next/first XDR record
		xdrrec_skiprecord(&xdr_in_);

		Size header = 0;
		xdr_u_int(&xdr_in_, &header);

		bool result = (header == STREAM_HEADER);

		//Log.info() << "checkStreamHeader = " << result << " (" << hex << header << "/" << STREAM_HEADER << ")" << endl;
		Log.info() << "BAEH!" << endl;

		return result;
	}

	void XDRPersistenceManager::writeStreamTrailer()
	{
		// write a trailer to identify the end of the stream
		put(STREAM_TRAILER);
		xdrrec_endofrecord(&xdr_out_, 1);

		Log.info() << "wrote stream trailer" << endl;
	}

	bool XDRPersistenceManager::checkStreamTrailer()
	{
#		ifdef BALL_DEBUG_PERSISTENCE
			Log.info() << "entering checkStreamTrailer()" << endl;
#		endif

		// check for the correct trailer
		Size trailer;
		get(trailer);

		// destroy the XDR stream
		xdr_destroy(&xdr_in_);

		Log.info() << "checkStreamTrailer = " << (trailer == STREAM_TRAILER) << endl;

		return (trailer == STREAM_TRAILER);		
	}

#	ifdef BALL_DEBUG_PERSISTENCE
	bool XDRPersistenceManager::checkTrailer(const char* name) 
#	else
	bool XDRPersistenceManager::checkTrailer(const char* /* name */) 
#endif
	{
#		ifdef BALL_DEBUG_PERSISTENCE
			if (name != 0)
			{
				Log.info() << "entering checkTrailer(" << name << ")" << endl;
			}
			else
			{
				Log.info() << "entering checkTrailer()" << endl;	
			}
#		endif
		return true;
	}

	bool XDRPersistenceManager::checkHeader(const char* type_name, const char* name, PointerSizeInt& ptr)
	{
#		ifdef BALL_DEBUG_PERSISTENCE
			Log.info() << "entering checkHeader(" << type_name << ", " << name << ")" << endl;
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
	{
#		ifdef BALL_DEBUG_PERSISTENCE
			Log.info() << "entering getObjectHeader()" << endl;
#		endif

		get(type_name);

#		ifdef BALL_DEBUG_PERSISTENCE
			Log.info() << "read type name: " << type_name << endl;
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
			Log.info() << "Read pointer: " << ptr << endl;
		}
#		endif

		return true;
	}

	void XDRPersistenceManager::writeName(const char* /* name */)
	{
	}

	bool XDRPersistenceManager::checkName(const char* /* name */)
	{
#		ifdef BALL_DEBUG_PERSISTENCE
			Log.info() << "entering checkName()" << endl;
#		endif

		return true;
	}

	void XDRPersistenceManager::writeStorableHeader(const char* /* type_name */, const char* /* name */)
	{
	}

	bool XDRPersistenceManager::checkStorableHeader
		(const char* /* type_name */, const char* /* name */)
	{
#		ifdef BALL_DEBUG_PERSISTENCE
			Log.info() << "entering checkStorableHeader()" << endl;
#		endif

		return true;
	}

	void XDRPersistenceManager::writeStorableTrailer()
	{
	}

	bool XDRPersistenceManager::checkStorableTrailer()
	{
#		ifdef BALL_DEBUG_PERSISTENCE
			Log.info() << "entering checkStorableTrailer()" << endl;
#		endif

		return true;
	}

	void XDRPersistenceManager::writePrimitiveHeader
		(const char* /* type_name */, const char* /* name */)
	{
	}

	bool XDRPersistenceManager::checkPrimitiveHeader
		(const char* /* type_name */, const char* /* name */)
	{
#		ifdef BALL_DEBUG_PERSISTENCE
			Log.info() << "entering checkPrimitiveHeader()" << endl;
#		endif
		return true;
	}

	void XDRPersistenceManager::writePrimitiveTrailer()
	{
	}

	bool XDRPersistenceManager::checkPrimitiveTrailer()
	{
#		ifdef BALL_DEBUG_PERSISTENCE
			Log.info() << "entering checkPrimitiveTrailer()" << endl;
#		endif

		return true;
	}

	void XDRPersistenceManager::writeObjectPointerHeader
		(const char* /* type_name */, const char* /* name */)
	{
	}
	
	bool XDRPersistenceManager::checkObjectPointerHeader
		(const char* /* type_name */, const char* /* name */)
	{
#		ifdef BALL_DEBUG_PERSISTENCE
			Log.info() << "entering checkObjectPointerHeader()" << endl;
#		endif

		return true;
	}
	
	void XDRPersistenceManager::writeObjectReferenceHeader
		(const char* /* type_name */, const char* /* name */)
	{
	}
	
	bool XDRPersistenceManager::checkObjectReferenceHeader
		(const char* /* type_name */, const char* /* name */)
	{
#		ifdef BALL_DEBUG_PERSISTENCE
			Log.info() << "entering checkObjectReferenceHeader()" << endl;
#		endif

		return true;
	}
	
	void XDRPersistenceManager::writeObjectPointerArrayHeader
		(const char* /* type_name */, const char* /* name */, Size size)
	{
		put(size);
	}

	bool XDRPersistenceManager::checkObjectPointerArrayHeader
		(const char* /* type_name */, const char* /* name */, Size& size)
	{
		get(size);
		return true;
	}
	
	void XDRPersistenceManager::writeObjectPointerArrayTrailer()
	{
	}
			
	bool XDRPersistenceManager::checkObjectPointerArrayTrailer()
	{
		return true;
	}

	/// Layer 0: primitive put methods
	void XDRPersistenceManager::put(const char c)
	{
		char* char_ptr = const_cast<char*>(&c);
		xdr_char(&xdr_out_, char_ptr);
	}

	void XDRPersistenceManager::put(const Byte b)
	{
		unsigned char* char_ptr = const_cast<unsigned char*>(&b);
		xdr_u_char(&xdr_out_, char_ptr);
	}

	void XDRPersistenceManager::put(const bool b)
	{		
		char c = b ? (char)1 : (char)0;
		xdr_char(&xdr_out_, &c);
	}

	void XDRPersistenceManager::put(const Index i)
	{
		Index* index_ptr = const_cast<Index*>(&i);
		xdr_int(&xdr_out_, index_ptr);
	}

	void XDRPersistenceManager::put(const Size s)
	{
		Size* size_ptr = const_cast<Size*>(&s);
		xdr_u_int(&xdr_out_, size_ptr);
	}

	void XDRPersistenceManager::put(const Real x)
	{
		Real* real_ptr = const_cast<Real*>(&x);
		xdr_float(&xdr_out_, real_ptr);
	}

	void XDRPersistenceManager::put(const DoubleReal x)
	{
		DoubleReal* double_ptr = const_cast<DoubleReal*>(&x);
		xdr_double(&xdr_out_, double_ptr);
	}

	void XDRPersistenceManager::put(const PointerSizeInt ptr)
	{
		u_long ptr_ptr = ptr;
		xdr_u_long(&xdr_out_, &ptr_ptr);
	}

	void XDRPersistenceManager::put(const string& s)
	{
		char* ptr = const_cast<char*>(s.c_str());
		xdr_string(&xdr_out_, &ptr, s.size());
	}

	void XDRPersistenceManager::get(char& c)
	{
		xdr_char(&xdr_in_, &c);
		Log.info() << "read char: " << c << endl;
	}

	void XDRPersistenceManager::get(Byte& c)
	{
		xdr_u_char(&xdr_in_, &c);
		Log.info() << "read Byte: " << c << endl;
	}

	void XDRPersistenceManager::get(bool& b)
	{
		char c;
		xdr_char(&xdr_in_, &c);
		b = (c == (char)1);
		Log.info() << "read bool: " << b << endl;
	}

	void XDRPersistenceManager::get(string& s)
	{
		static char buf[65536];
		char* ptr = &(buf[0]);
		xdr_string(&xdr_in_, &ptr, 65535);
		s = ptr;
		Log.info() << "read string: " << s << endl;
	}

	void XDRPersistenceManager::get(Index& i)
	{
		xdr_int(&xdr_in_, &i);
		Log.info() << "read int: " << i << endl;
	}

	void XDRPersistenceManager::get(Size& s)
	{
		xdr_u_int(&xdr_in_, &s);
		Log.info() << "read unsigned int: " << s << "/" << hex << s << endl;
	}

	void XDRPersistenceManager::get(Real& x)
	{
		xdr_float(&xdr_in_, &x);
	}

	void XDRPersistenceManager::get(DoubleReal& x)
	{
		xdr_double(&xdr_in_, &x);
	}

	void XDRPersistenceManager::get(PointerSizeInt& ptr)
	{
		xdr_long(&xdr_in_, &ptr);

		Log.info() << "get ptr: " << hex << ptr << endl;
	}

} // namespace BALL
