// $Id: XDRPersistenceManager.C,v 1.3 2000/01/16 17:26:51 oliver Exp $

#include <BALL/CONCEPT/XDRPersistenceManager.h>

//#define BALL_DEBUG_PERSISTENCE

namespace BALL 
{

	XDRPersistenceManager::XDRPersistenceManager()
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
		os->put('S');
	}

	bool XDRPersistenceManager::checkStreamHeader()
	{
#		ifdef BALL_DEBUG_PERSISTENCE
			Log.level(LogStream::INFORMATION) << "entering checkStreamHeader()" << endl;
#		endif

		char c;
		is->get(c);

		return (c == 'S');
	}

	void XDRPersistenceManager::writeStreamTrailer()
	{
		os->put('X');
	}

	bool XDRPersistenceManager::checkStreamTrailer()
	{
#		ifdef BALL_DEBUG_PERSISTENCE
			Log.level(LogStream::INFORMATION) << "entering checkStreamTrailer()" << endl;
#		endif

		char c;
		is->get(c);

		return (c == 'X');
	}

#	ifdef BALL_DEBUG_PERSISTENCE
	bool XDRPersistenceManager::checkTrailer(const char* name) 
#	else
	bool XDRPersistenceManager::checkTrailer(const char* /* name */) 
#endif
	{
#		ifdef BALL_DEBUG_PERSISTENCE
			if (name != 0)
				Log.level(LogStream::INFORMATION) << "entering checkTrailer(" << name << ")" << endl;
			else
				Log.level(LogStream::INFORMATION) << "entering checkTrailer()" << endl;
#		endif
		return true;
	}

	bool XDRPersistenceManager::checkHeader(const char* type_name, const char* name, LongPointerType& ptr)
	{
#		ifdef BALL_DEBUG_PERSISTENCE
			Log.level(LogStream::INFORMATION) << "entering checkHeader(" << type_name << ", " << name << ")" << endl;
#		endif
    if ((name != 0) && (!strcmp(name, "")))
		{
			String s;
			get(s);

			if (s != type_name) 
			{
#				ifdef BALL_DEBUG_PERSISTENCE
					Log.error() << "Expected object of type " << type_name << " but found definition for " << s << "!" << endl;
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
			Log.level(LogStream::INFORMATION) << "entering getObjectHeader()" << endl;
#		endif

		get(type_name);

#		ifdef BALL_DEBUG_PERSISTENCE
			Log.level(LogStream::INFORMATION) << "read type name: " << type_name << endl;
#		endif

		get(ptr);
		if (ptr == 0) 
		{
#		ifdef BALL_DEBUG_PERSISTENCE
			Log.level(LogStream::ERROR) << "Could not read a valid object pointer: " << dec << (unsigned int)ptr << "!" << endl;
#		endif

			return false;
		} 
#		ifdef BALL_DEBUG_PERSISTENCE
		else 
		{
			Log.level(LogStream::INFORMATION) << "Read pointer: " << ptr << endl;
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
			Log.level(LogStream::INFORMATION) << "entering checkName()" << endl;
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
			Log.level(LogStream::INFORMATION) << "entering checkStorableHeader()" << endl;
#		endif

		return true;
	}

	void XDRPersistenceManager::writeStorableTrailer()
	{
	}

	bool XDRPersistenceManager::checkStorableTrailer()
	{
#		ifdef BALL_DEBUG_PERSISTENCE
			Log.level(LogStream::INFORMATION) << "entering checkStorableTrailer()" << endl;
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
			Log.level(LogStream::INFORMATION) << "entering checkPrimitiveHeader()" << endl;
#		endif
		return true;
	}

	void XDRPersistenceManager::writePrimitiveTrailer()
	{
	}

	bool XDRPersistenceManager::checkPrimitiveTrailer()
	{
#		ifdef BALL_DEBUG_PERSISTENCE
			Log.level(LogStream::INFORMATION) << "entering checkPrimitiveTrailer()" << endl;
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
			Log.level(LogStream::INFORMATION) << "entering checkObjectPointerHeader()" << endl;
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
			Log.level(LogStream::INFORMATION) << "entering checkObjectReferenceHeader()" << endl;
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
		os->put((unsigned char)c);
	}

	void XDRPersistenceManager::put(const unsigned char c)
	{
		os->put((unsigned char)c);
	}

	void XDRPersistenceManager::put(const bool b)
	{		
		os->put(b ? (unsigned char)1 : (unsigned char)0);
	}

	void XDRPersistenceManager::put(const LongPointerType& p)
	{
		const unsigned char*	ptr = (const unsigned char*)&p;
		for (unsigned short j = 0; j < sizeof(p); ++j)
		{
			os->put((unsigned char)*ptr++);
		}
	}

#define BALL_DEFINE_NUMBER_PUT(type)\
	void XDRPersistenceManager::put(const type i)\
	{\
		const unsigned char*	ptr = (const unsigned char*)&i;\
		for (unsigned short j = 0; j < sizeof(i); ++j)\
		{\
			os->put((unsigned char)*ptr++);\
		}\
	}\

	BALL_DEFINE_NUMBER_PUT(short)
	BALL_DEFINE_NUMBER_PUT(unsigned short)
	BALL_DEFINE_NUMBER_PUT(int)
	BALL_DEFINE_NUMBER_PUT(unsigned int)
	BALL_DEFINE_NUMBER_PUT(long)
	BALL_DEFINE_NUMBER_PUT(unsigned long)
	BALL_DEFINE_NUMBER_PUT(float)
	BALL_DEFINE_NUMBER_PUT(double)
	BALL_DEFINE_NUMBER_PUT(void*)

	void XDRPersistenceManager::put(const string& s)
	{
		const unsigned char* ptr = (unsigned char*)s.c_str();

		do
		{
			os->put(*ptr);
		}	while (*ptr++ != (unsigned char)0);
	}

	void XDRPersistenceManager::get(char& c)
	{
		char tmp;
		is->get(tmp);
		c = tmp;
	}

	void XDRPersistenceManager::get(unsigned char& c)
	{
		char& c_ref = (char&)c;
		is->get(c_ref);
	}

	void XDRPersistenceManager::get(bool& b)
	{
		char c;
		is->get(c);
		b = (c == (char)1);
	}

	void XDRPersistenceManager::get(string& s)
	{
		static char buf[1024];
		char* ptr = &(buf[0]);
		
		do
		{
			is->get(*ptr);
		} while (*ptr++ != (char)0);

		s.assign((char*)&buf[0]);
	}

#define BALL_DEFINE_NUMBER_GET(type)\
	void XDRPersistenceManager::get(type& i)\
	{\
		char* ptr = (char*)&i;\
		for (unsigned short j = 0; j < sizeof(i); ++j)\
		{\
			is->get(*ptr++);\
		}\
	}\


	BALL_DEFINE_NUMBER_GET(short)
	BALL_DEFINE_NUMBER_GET(unsigned short)
	BALL_DEFINE_NUMBER_GET(int)
	BALL_DEFINE_NUMBER_GET(unsigned int)
	BALL_DEFINE_NUMBER_GET(long)
	BALL_DEFINE_NUMBER_GET(unsigned long)
	BALL_DEFINE_NUMBER_GET(float)
	BALL_DEFINE_NUMBER_GET(double)	
	BALL_DEFINE_NUMBER_GET(LongPointerType)
	BALL_DEFINE_NUMBER_GET(void*)

} // namespace BALL
