// $Id: XDRPersistenceManager.C,v 1.9 2000/10/29 22:31:59 oliver Exp $

#include <BALL/CONCEPT/XDRPersistenceManager.h>

//#define BALL_DEBUG_PERSISTENCE

using namespace std;

namespace BALL 
{

	extern "C" int XDRReadStream_(void*, char*, int)
	{
		// BAUSTELLE: need to read something from the istream
		return 0;
	}

	extern "C" int XDRWriteStream_(void*, char*, int)
	{
		// BAUSTELLE: need to write something to the ostream
		return 0;
	}

	extern "C" int XDRError_(void*, char*, int)
	{
		Log.error() << "XDRPersistenceManager: error wrong access mode fro XDR stream." << endl;

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

  void XDRPersistenceManager::writeHeader(const char* type_name, const char* name, LongPointerType ptr)
  {
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
		xdrrec_create(&xdr_in_, 0, 0, 0, XDRReadStream_, XDRError_);
		xdrrec_create(&xdr_out_, 0, 0, 0, XDRError_, XDRWriteStream_);
		xdr_in_.x_op = XDR_DECODE;
		xdr_out_.x_op = XDR_ENCODE;
		put(STREAM_HEADER);
	}

	bool XDRPersistenceManager::checkStreamHeader()
	{
#		ifdef BALL_DEBUG_PERSISTENCE
			Log.info() << "entering checkStreamHeader()" << endl;
#		endif

		// skip to the next/first XDR record
		xdrrec_skiprecord(&xdr_in_);

		Size header;
		get(header);

		bool result = ((xdr_u_int(&xdr_in_, &header) == 1) 
									 && (header == STREAM_HEADER));

		// destroy the XDR streams
		xdr_destroy(&xdr_in_);
		xdr_destroy(&xdr_out_);

		return result;
	}

	void XDRPersistenceManager::writeStreamTrailer()
	{
		put(STREAM_TRAILER);
		xdrrec_endofrecord(&xdr_out_, 1);
	}

	bool XDRPersistenceManager::checkStreamTrailer()
	{
#		ifdef BALL_DEBUG_PERSISTENCE
			Log.info() << "entering checkStreamTrailer()" << endl;
#		endif

		Size trailer;
		get(trailer);

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

	bool XDRPersistenceManager::checkHeader(const char* type_name, const char* name, LongPointerType& ptr)
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

	bool XDRPersistenceManager::getObjectHeader(String& type_name, LongPointerType& ptr)
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

	void XDRPersistenceManager::put(const unsigned char c)
	{
		unsigned char* char_ptr = const_cast<unsigned char*>(&c);
		xdr_u_char(&xdr_out_, char_ptr);
	}

	void XDRPersistenceManager::put(const bool b)
	{		
		char c = b ? (char)1 : (char)0;
		xdr_char(&xdr_out_, &c);
	}

	void XDRPersistenceManager::put(const short i)
	{
		short* short_ptr = const_cast<short*>(&i);
		xdr_short(&xdr_out_, short_ptr);
	}

	void XDRPersistenceManager::put(const unsigned short i)
	{
		unsigned short* short_ptr = const_cast<unsigned short*>(&i);
		xdr_u_short(&xdr_out_, short_ptr);
	}

	void XDRPersistenceManager::put(const int i)
	{
		int* int_ptr = const_cast<int*>(&i);
		xdr_int(&xdr_out_, int_ptr);
	}

	void XDRPersistenceManager::put(const unsigned int i)
	{
		unsigned int* int_ptr = const_cast<unsigned int*>(&i);
		xdr_u_int(&xdr_out_, int_ptr);
	}

	void XDRPersistenceManager::put(const long i)
	{
		long* long_ptr = const_cast<long*>(&i);
		xdr_long(&xdr_out_, long_ptr);
	}

	void XDRPersistenceManager::put(const unsigned long i)
	{
		unsigned long* long_ptr = const_cast<unsigned long*>(&i);
		xdr_u_long(&xdr_out_, long_ptr);
	}

#ifndef BALL_64BIT_ARCHITECTURE
	void XDRPersistenceManager::put(const long long i)
	{
		long long* long_ptr = const_cast<long long*>(&i);
		xdr_longlong_t(&xdr_out_, long_ptr);
	}

	void XDRPersistenceManager::put(const unsigned long long i)
	{
		unsigned long long* long_ptr = const_cast<unsigned long long*>(&i);
		xdr_u_longlong_t(&xdr_out_, long_ptr);
	}
#endif

	
	void XDRPersistenceManager::put(const float x)
	{
		float* float_ptr = const_cast<float*>(&x);
		xdr_float(&xdr_out_, float_ptr);
	}

	void XDRPersistenceManager::put(const double x)
	{
		double* double_ptr = const_cast<double*>(&x);
		xdr_double(&xdr_out_, double_ptr);
	}

	void XDRPersistenceManager::put(const void* ptr)
	{
		long ptr_long = (long)ptr;
		xdr_long(&xdr_out_, &ptr_long);
	}

	void XDRPersistenceManager::put(const string& s)
	{
		char* ptr = const_cast<char*>(s.c_str());
		xdr_string(&xdr_out_, &ptr, s.size());
	}

	void XDRPersistenceManager::get(char& c)
	{
		xdr_char(&xdr_in_, &c);
	}

	void XDRPersistenceManager::get(unsigned char& c)
	{
		xdr_u_char(&xdr_in_, &c);
	}

	void XDRPersistenceManager::get(bool& b)
	{
		char c;
		xdr_char(&xdr_in_, &c);
		b = (c == (char)1);
	}

	void XDRPersistenceManager::get(string& s)
	{
		static char buf[65536];
		char* ptr = &(buf[0]);
		xdr_string(&xdr_in_, &ptr, 65535);
		s = ptr;
	}

	void XDRPersistenceManager::get(short& s)
	{
		xdr_short(&xdr_in_, &s);
	}

	void XDRPersistenceManager::get(unsigned short& s)
	{
		xdr_u_short(&xdr_in_, &s);
	}

	void XDRPersistenceManager::get(int& s)
	{
		xdr_int(&xdr_in_, &s);
	}

	void XDRPersistenceManager::get(unsigned int& s)
	{
		xdr_u_int(&xdr_in_, &s);
	}

	void XDRPersistenceManager::get(long& s)
	{
		xdr_long(&xdr_in_, &s);
	}

	void XDRPersistenceManager::get(unsigned long& s)
	{
		xdr_u_long(&xdr_in_, &s);
	}

#ifndef BALL_64BIT_ARCHITECTURE
	void XDRPersistenceManager::get(long long& s)
	{
		xdr_longlong_t(&xdr_in_, &s);
	}

	void XDRPersistenceManager::get(unsigned long long& s)
	{
		xdr_u_longlong_t(&xdr_in_, &s);
	}
#endif

	void XDRPersistenceManager::get(float& x)
	{
		xdr_float(&xdr_in_, &x);
	}

	void XDRPersistenceManager::get(double& x)
	{
		xdr_double(&xdr_in_, &x);
	}

	void XDRPersistenceManager::get(void*& ptr)
	{
		long ptr_long;
		xdr_long(&xdr_in_, &ptr_long);
		ptr = (void*)ptr_long;
	}

} // namespace BALL
