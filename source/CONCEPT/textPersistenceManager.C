// $Id: textPersistenceManager.C,v 1.4 2000/01/10 15:51:10 oliver Exp $

#include <BALL/CONCEPT/textPersistenceManager.h>

// #define BALL_DEBUG_PERSISTENCE

using namespace std;

namespace BALL 
{

	const char* TextPersistenceManager::INDENT_STEP = "  ";

	TextPersistenceManager::TextPersistenceManager()
		:	indent_depth_(0)
	{
	}

	bool TextPersistenceManager::expect(const String& value)
	{
		static String s;
		*is >> s;
		if (s != value)
			Log.level(LogStream::ERROR) << "textPersistenceManager::expect: read " << s << ", expected " << value << endl;
		
		return (s == value);
	}

	String TextPersistenceManager::indent() 
	{
		string result("");

		// assemble a string consisting of indent_depth_ copies
		// of INDENT_STEP
		for (Size i = 0; i < indent_depth_; i++)
		{
			result.append(INDENT_STEP);
		}

		return result;
	}
			
  void TextPersistenceManager::writeHeader(const char* type_name, const char* name, void* ptr)
  {
    *os << indent();

    if (name == 0)
    {
      *os << "BASEOBJECT ";
		} else {
      *os << "OBJECT ";
		}

    *os << type_name << " @ " << dec << (PointerInt)ptr;
    if ((name != 0) && (*name != (char)0))
    {
      *os << " " << name;
		} else {
      *os << " -";
		}

    *os << endl;

    indent_depth_++;
	}
 
	void TextPersistenceManager::writeTrailer(const char* name)
	{
		indent_depth_--;
		*os << indent();
			
		if (name == 0) {
			*os << "END_BASEOBJECT" << endl;
		} else {
			*os << "END_OBJECT" << endl;
		}
	}

	void TextPersistenceManager::writeStreamHeader()
	{
		*os << "START" << endl;
	}

	bool TextPersistenceManager::checkStreamHeader()
	{
#		ifdef BALL_DEBUG_PERSISTENCE
			Log.level(LogStream::INFORMATION) << "entering checkStreamHeader()" << endl;
#		endif

		String s;
		*is >> s;

		return (s == "START");
	}

	void TextPersistenceManager::writeStreamTrailer()
	{
		*os << "END" << endl;
	}

	bool TextPersistenceManager::checkStreamTrailer()
	{
#		ifdef BALL_DEBUG_PERSISTENCE
			Log.level(LogStream::INFORMATION) << "entering checkStreamTrailer()" << endl;
#		endif

		String s;
		*is >> s;

		return (s == "END");
	}

	bool TextPersistenceManager::checkTrailer(const char* name) 
	{
#		ifdef BALL_DEBUG_PERSISTENCE
			if (name != 0)
				Log.level(LogStream::INFORMATION) << "entering checkTrailer(" << name << ")" << endl;
			else
				Log.level(LogStream::INFORMATION) << "entering checkTrailer()" << endl;
#		endif

		if (name == 0) 
		{
			return expect("END_BASEOBJECT");
		} else {
			return expect("END_OBJECT");
		}
	}

	bool TextPersistenceManager::checkHeader(const char* type_name, const char* name, void*& ptr)
	{
#		ifdef BALL_DEBUG_PERSISTENCE
			if (name == 0)
			{
				Log.level(LogStream::INFORMATION) << "entering checkHeader(" << type_name << ", 0)" << endl;
			} else {
				Log.level(LogStream::INFORMATION) << "entering checkHeader(" << type_name << ", " << name << ")" << endl;
			}
#		endif
		if (name == 0) 
		{
			if (!expect("BASEOBJECT"))
				return false;
		} else {
			if (!expect("OBJECT"))
				return false;
		}

		String s;
		*is >> s;
		if (s != type_name) 
		{
#			ifdef BALL_DEBUG_PERSISTENCE
				Log.level(LogStream::ERROR) << "Expected object of type " << type_name << " but found definition for " << s << "!" << endl;
#			endif

			return false;
		}

		if (!expect("@"))
			return false;
		
		get(ptr);
		if (ptr == 0) 
		{
#			ifdef BALL_DEBUG_PERSISTENCE
				Log.level(LogStream::ERROR) << "Could not read a valid object pointer: " << dec << (PointerInt)ptr << "!" << endl;
#			endif

			return false;
		}

		// if we check for a baseclass, name should be 0
		if (name == 0)
		{
			if (!expect("-"))
				return false;
		} else {
			if (!strcmp(name, ""))
			{
				if (!expect("-"))
				{
					return false;
				}
			} 
			else if (!expect(name))
			{
				return false;
			}
		}

		return true;
	}

	bool TextPersistenceManager::getObjectHeader(String& type_name, void*& ptr)
	{
#		ifdef BALL_DEBUG_PERSISTENCE
			Log.level(LogStream::INFORMATION) << "entering getObjectHeader()" << endl;
#		endif

		if (!expect("OBJECT"))
			return false;

		*is >> type_name;
#		ifdef BALL_DEBUG_PERSISTENCE
			Log.level(LogStream::INFORMATION) << "read type name: " << type_name << endl;
#		endif

		// retrieve the 
		if (!expect("@"))
			return false;
		
		get(ptr);
		if (ptr == 0) 
		{
#		ifdef BALL_DEBUG_PERSISTENCE
			Log.level(LogStream::ERROR) << "Could not read a valid object pointer: " << dec << (PointerInt)ptr << "!" << endl;
#		endif

			return false;
		} 
#		ifdef BALL_DEBUG_PERSISTENCE
		else 
		{
			Log.level(LogStream::INFORMATION) << "Read pointer: " << ptr << endl;
		}
#		endif

		return expect("-");
	}

	void TextPersistenceManager::writeName(const char* name)
	{
		if ((name == 0) || !strcmp(name, ""))
		{
			*os << " - ";
		} else {
			*os << ' ' << name << ' ';
		}
	}

	bool TextPersistenceManager::checkName(const char* name)
	{
#		ifdef BALL_DEBUG_PERSISTENCE
			Log.level(LogStream::INFORMATION) << "entering checkName()" << endl;
#endif

		String s;
		*is >> s;
			
		if ((name == 0) || !strcmp(name, ""))
		{
			return (s == "-");
		} 
		
		return (s == name);
	}

	void TextPersistenceManager::writeStorableHeader(const char* type_name, const char* name)
	{
		*os << indent() << "STORABLE";
		writeName(type_name);
		writeName(name);
		*os << endl;
		++indent_depth_;
	}

	bool TextPersistenceManager::checkStorableHeader(const char* type_name, const char* name)
	{
#		ifdef BALL_DEBUG_PERSISTENCE
			Log.level(LogStream::INFORMATION) << "entering checkStorableHeader()" << endl;
#		endif

		return (expect("STORABLE") && checkName(type_name) && checkName(name));
	}

	void TextPersistenceManager::writeStorableTrailer()
	{
		--indent_depth_;
		*os << indent() << "END_STORABLE" << endl;
	}

	bool TextPersistenceManager::checkStorableTrailer()
	{
#		ifdef BALL_DEBUG_PERSISTENCE
			Log.level(LogStream::INFORMATION) << "entering checkStorableTrailer()" << endl;
#		endif

		return expect("END_STORABLE");
	}

	void TextPersistenceManager::writePrimitiveHeader(const char* type_name, const char* name)
	{
		
		*os << indent() << type_name;
		writeName(name);
		*os << "=";
	}

	bool TextPersistenceManager::checkPrimitiveHeader(const char* type_name, const char* name)
	{
#		ifdef BALL_DEBUG_PERSISTENCE
			Log.level(LogStream::INFORMATION) << "entering checkPrimitiveHeader(" << name << ")" << endl;
#		endif

		if (!strcmp(name, ""))
		{
			return (expect(type_name) && expect("-") && expect("="));
		} else {
			return (expect(type_name) && expect(name) && expect("="));
		}
	}

	void TextPersistenceManager::writePrimitiveTrailer()
	{
		*os << endl;
	}

	bool TextPersistenceManager::checkPrimitiveTrailer()
	{
#		ifdef BALL_DEBUG_PERSISTENCE
			Log.level(LogStream::INFORMATION) << "entering checkPrimitiveTrailer()" << endl;
#		endif

		return true;
	}

	void TextPersistenceManager::writeObjectPointerHeader(const char* type_name, const char* name)
	{
		*os << indent() << type_name << " *";
		writeName(name);
	}
	
	bool TextPersistenceManager::checkObjectPointerHeader(const char* type_name, const char* name)
	{
#		ifdef BALL_DEBUG_PERSISTENCE
			Log.level(LogStream::INFORMATION) << "entering checkObjectPointerHeader()" << endl;
#		endif

		return (checkName(type_name) && expect("*") && checkName(name));
	}
	
	void TextPersistenceManager::writeObjectReferenceHeader(const char* type_name, const char* name)
	{
		*os << indent() << type_name << " &";
		writeName(name);
	}
	
	bool TextPersistenceManager::checkObjectReferenceHeader(const char* type_name, const char* name)
	{
#		ifdef BALL_DEBUG_PERSISTENCE
			Log.level(LogStream::INFORMATION) << "entering checkObjectReferenceHeader()" << endl;
#		endif

		return (checkName(type_name) && expect("&") && checkName(name));
	}
	
	void TextPersistenceManager::writeObjectPointerArrayHeader(const char* type_name, const char* name, Size size)
	{
		*os << indent() << type_name;
		writeName(name);
		*os << " [";
		put(size);
		*os << "] = {";
	}

	bool TextPersistenceManager::checkObjectPointerArrayHeader(const char* type_name, const char* name, Size& size)
	{
		size = 0;
		// check for the first part of the header
		if (!(checkName(type_name) && checkName(name) && expect("[")))
		{
			return false;
		}
		
		// read the number of array elements
		get(size);
		
		// read the rest of the header
		return (expect("]") && expect("=") && expect("{"));
	}
		
	void TextPersistenceManager::writeObjectPointerArrayTrailer()
	{
		*os << " }" << endl;
	}
			
	bool TextPersistenceManager::checkObjectPointerArrayTrailer()
	{
		return expect("}");
	}

	/// Layer 0: primitive put methods
	void TextPersistenceManager::put(const char c)
	{
		*os << ' ' << (int)c << ' ';
	}

	void TextPersistenceManager::put(const unsigned char c)
	{
		*os << ' ' << (int)c << ' ';
	}

	void TextPersistenceManager::put(const bool b)
	{		
		if (b)
		{
			*os << " TRUE ";
		} else {
			*os << " FALSE ";
		}
	}
	
#define BALL_DEFINE_NUMBER_PUT(type)\
	void TextPersistenceManager::put(const type i)\
	{\
		*os << ' ' << i << ' ';\
	}\

	BALL_DEFINE_NUMBER_PUT(short)
	BALL_DEFINE_NUMBER_PUT(unsigned short)
	BALL_DEFINE_NUMBER_PUT(int)
	BALL_DEFINE_NUMBER_PUT(unsigned int)
	BALL_DEFINE_NUMBER_PUT(long)
	BALL_DEFINE_NUMBER_PUT(unsigned long)
	BALL_DEFINE_NUMBER_PUT(float)
	BALL_DEFINE_NUMBER_PUT(double)

	void TextPersistenceManager::put(const string& s)
	{
		*os << ' ' << s.size() << ' ';

		for (Size i = 0; i < s.size(); i++)
		{
			os->put(s[i]);
		}

		*os << ' ';
	}

	void TextPersistenceManager::put(const void* p)
	{
		*os << ' ' << dec << (PointerInt)p << ' ';
	}

	
	void TextPersistenceManager::get(char& c)
	{
		int i;
		*is >> i;
		c = (char)i;
	}

	void TextPersistenceManager::get(unsigned char& c)
	{
		int i;
		*is >> i;
		c = (char)i;
	}

	void TextPersistenceManager::get(bool& b)
	{
		String s;
		*is >> s;
		b = (s == "TRUE");
	}

	void TextPersistenceManager::get(string& s)
	{
		// read the number of characters
		int i;
		*is >> i;

		// read the leading blank
		char c;	
		is->get(c);

		// clear the string
		s.erase();
		
		// read i characters into the string
		if (i > 0)
		{
			for (; i > 0; i--)
			{
				is->get(c);
				s += c;
			}
		}
	}

	void TextPersistenceManager::get(void*& p)
	{
		PointerInt i;
		*is >> i;
		p = (void*)i;
	}

#define BALL_DEFINE_NUMBER_GET(type)\
	void TextPersistenceManager::get(type& i)\
	{\
		*is >> i;\
	}\


	BALL_DEFINE_NUMBER_GET(short)
	BALL_DEFINE_NUMBER_GET(unsigned short)
	BALL_DEFINE_NUMBER_GET(int)
	BALL_DEFINE_NUMBER_GET(unsigned int)
	BALL_DEFINE_NUMBER_GET(long)
	BALL_DEFINE_NUMBER_GET(unsigned long)
	BALL_DEFINE_NUMBER_GET(float)
	BALL_DEFINE_NUMBER_GET(double)

} // namespace BALL
