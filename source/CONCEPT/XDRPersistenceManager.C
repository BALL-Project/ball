// $Id: XDRPersistenceManager.C,v 1.7 2000/03/14 22:46:23 oliver Exp $

#include <BALL/CONCEPT/XDRPersistenceManager.h>

//#define BALL_DEBUG_PERSISTENCE

using namespace std;

namespace BALL 
{

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
		ostr_->put('S');
	}

	bool XDRPersistenceManager::checkStreamHeader()
	{
#		ifdef BALL_DEBUG_PERSISTENCE
			Log.info() << "entering checkStreamHeader()" << endl;
#		endif

		char c;
		istr_->get(c);

		return (c == 'S');
	}

	void XDRPersistenceManager::writeStreamTrailer()
	{
		ostr_->put('X');
	}

	bool XDRPersistenceManager::checkStreamTrailer()
	{
#		ifdef BALL_DEBUG_PERSISTENCE
			Log.info() << "entering checkStreamTrailer()" << endl;
#		endif

		char c;
		istr_->get(c);

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
			Log.level(LogStream::ERROR) << "Could not read a valid object pointer: " << dec << (unsigned int)ptr << "!" << endl;
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
		ostr_->put((unsigned char)c);
	}

	void XDRPersistenceManager::put(const unsigned char c)
	{
		ostr_->put((unsigned char)c);
	}

	void XDRPersistenceManager::put(const bool b)
	{		
		ostr_->put(b ? (unsigned char)1 : (unsigned char)0);
	}

#define BALL_DEFINE_NUMBER_PUT(type)\
	void XDRPersistenceManager::put(const type i)\
	{\
		const unsigned char*	ptr = (const unsigned char*)&i;\
		for (unsigned short j = 0; j < sizeof(i); ++j)\
		{\
			ostr_->put((unsigned char)*ptr++);\
		}\
	}\

	BALL_DEFINE_NUMBER_PUT(short)
	BALL_DEFINE_NUMBER_PUT(unsigned short)
	BALL_DEFINE_NUMBER_PUT(int)
	BALL_DEFINE_NUMBER_PUT(unsigned int)
	BALL_DEFINE_NUMBER_PUT(long)
	BALL_DEFINE_NUMBER_PUT(unsigned long)
#ifndef BALL_64BIT_ARCHITECTURE
	BALL_DEFINE_NUMBER_PUT(long long)
	BALL_DEFINE_NUMBER_PUT(unsigned long long)
#endif
	BALL_DEFINE_NUMBER_PUT(float)
	BALL_DEFINE_NUMBER_PUT(double)
	BALL_DEFINE_NUMBER_PUT(void*)

	void XDRPersistenceManager::put(const string& s)
	{
		const unsigned char* ptr = (unsigned char*)s.c_str();

		do
		{
			ostr_->put(*ptr);
		}	while (*ptr++ != (unsigned char)0);
	}

	void XDRPersistenceManager::get(char& c)
	{
		char tmp;
		istr_->get(tmp);
		c = tmp;
	}

	void XDRPersistenceManager::get(unsigned char& c)
	{
		char& c_ref = (char&)c;
		istr_->get(c_ref);
	}

	void XDRPersistenceManager::get(bool& b)
	{
		char c;
		istr_->get(c);
		b = (c == (char)1);
	}

	void XDRPersistenceManager::get(string& s)
	{
		static char buf[1024];
		char* ptr = &(buf[0]);
		
		do
		{
			istr_->get(*ptr);
		} while (*ptr++ != (char)0);

		s.assign((char*)&buf[0]);
	}

#define BALL_DEFINE_NUMBER_GET(type)\
	void XDRPersistenceManager::get(type& i)\
	{\
		char* ptr = (char*)&i;\
		for (unsigned short j = 0; j < sizeof(i); ++j)\
		{\
			istr_->get(*ptr++);\
		}\
	}\


	BALL_DEFINE_NUMBER_GET(short)
	BALL_DEFINE_NUMBER_GET(unsigned short)
	BALL_DEFINE_NUMBER_GET(int)
	BALL_DEFINE_NUMBER_GET(unsigned int)
	BALL_DEFINE_NUMBER_GET(long)
	BALL_DEFINE_NUMBER_GET(unsigned long)
#ifndef BALL_64BIT_ARCHITECTURE
	BALL_DEFINE_NUMBER_GET(long long)
	BALL_DEFINE_NUMBER_GET(unsigned long long)
#endif
	BALL_DEFINE_NUMBER_GET(float)
	BALL_DEFINE_NUMBER_GET(double)	
	BALL_DEFINE_NUMBER_GET(void*)

} // namespace BALL
