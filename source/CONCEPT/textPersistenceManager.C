// $Id: textPersistenceManager.C,v 1.12 2000/12/12 16:21:33 oliver Exp $

#include <BALL/CONCEPT/textPersistenceManager.h>

// #define BALL_DEBUG_PERSISTENCE

using namespace std;

namespace BALL 
{

	const char* TextPersistenceManager::INDENT_STEP = "  ";

	TextPersistenceManager::TextPersistenceManager()
		:	PersistenceManager(),
			indent_depth_(0)
	{
	}

	TextPersistenceManager::TextPersistenceManager(istream& is)
		:	PersistenceManager(is),
			indent_depth_(0)
	{
	}

	TextPersistenceManager::TextPersistenceManager(ostream& os)
		:	PersistenceManager(os),
			indent_depth_(0)
	{
	}

	TextPersistenceManager::TextPersistenceManager(istream& is, ostream& os)
		:	PersistenceManager(is, os),
			indent_depth_(0)
	{
	}

	bool TextPersistenceManager::expect(const String& value)
	{
		static String s;
		*istr_ >> s;
		if (s != value)
		{
			Log.error() << "textPersistenceManager::expect: " 
				<< " at position " << istr_->tellg() << ": read " << s << ", expected " << value << endl;
		}
		
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
			
  void TextPersistenceManager::writeHeader
		(const char* type_name, const char* name, PointerSizeInt ptr)
  {
    *ostr_ << indent();

    if (name == 0)
    {
      *ostr_ << "BASEOBJECT ";
		} 
		else 
		{
      *ostr_ << "OBJECT ";
		}

    *ostr_ << type_name << " @ " << dec << ptr;
    if ((name != 0) && (*name != (char)0))
    {
      *ostr_ << " " << name;
		} 
		else 
		{
      *ostr_ << " -";
		}

    *ostr_ << endl;

    indent_depth_++;
	}
 
	void TextPersistenceManager::writeTrailer(const char* name)
	{
		// decrease indentation by one column
		indent_depth_--;
		*ostr_ << indent();
		
		// and write the object/base object trailer
		if (name == 0) 
		{
			*ostr_ << "END_BASEOBJECT" << endl;
		} 
		else 
		{
			*ostr_ << "END_OBJECT" << endl;
		}
	}

	void TextPersistenceManager::writeStreamHeader()
	{
		// write a start tag
		*ostr_ << "START" << endl;
	}

	bool TextPersistenceManager::checkStreamHeader()
	{
#		ifdef BALL_DEBUG_PERSISTENCE
			Log.info() << "entering checkStreamHeader()" << endl;
#		endif

		// read a string from the input stream
		String s;
		*istr_ >> s;

		// and check whether it is the start tag we expected
		return (s == "START");
	}

	void TextPersistenceManager::writeStreamTrailer()
	{
		// write a END trailer to the stream
		*ostr_ << "END" << endl;
	}

	bool TextPersistenceManager::checkStreamTrailer()
	{
#		ifdef BALL_DEBUG_PERSISTENCE
			Log.info() << "entering checkStreamTrailer()" << endl;
#		endif

		// read the next string from the stream
		String s;
		*istr_ >> s;

		// and check for the END tag
		return (s == "END");
	}

	bool TextPersistenceManager::checkTrailer(const char* name) 
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

		// check for the object or base object trailer
		if (name == 0) 
		{
			return expect("END_BASEOBJECT");
		} 
		else 
		{
			return expect("END_OBJECT");
		}
	}

	bool TextPersistenceManager::checkHeader(const char* type_name, const char* name, PointerSizeInt& ptr)
	{
#		ifdef BALL_DEBUG_PERSISTENCE
			if (name == 0)
			{
				Log.info() << "entering checkHeader(" << type_name << ", 0)" << endl;
			} 
			else 
			{
				Log.info() << "entering checkHeader(" << type_name << ", " << name << ")" << endl;
			}
#		endif
		// check for the header keyword OBJECT/BASEOBJECT
		if (name == 0) 
		{
			if (!expect("BASEOBJECT"))
			{
				// abort immediately if not found
				return false;	
			}
		} 
		else 
		{
			if (!expect("OBJECT"))
			{
				// abort immediately if not found
				return false;
			}
		}

		// read the next string. it should contain the
		// type name ofthe object
		String s;
		*istr_ >> s;

		// compare it with the type we expected
		if (s != type_name) 
		{
#			ifdef BALL_DEBUG_PERSISTENCE
				Log.error() << "Expected object of type " << type_name 
										<< " but found definition for " << s << "!" << endl;
#			endif

			// we didn't find the right object - abort
			return false;
		}

		// abort if we cannot read an @ sign 
		// (indicating the pointer that follows)
		if (!expect("@"))
		{
			return false;
		}
		
		// read the object pointer
		get(ptr);
		if (ptr == 0) 
		{
#			ifdef BALL_DEBUG_PERSISTENCE
				Log.error() << "Could not read a valid object pointer: " 
										<< dec << (PointerSizeInt)ptr << "!" << endl;
#			endif

			// abort if it was not a valid pointer
			return false;
		}

		// if we check for a baseclass, name should be 0
		if (name == 0)
		{
			if (!expect("-"))
			{
				return false;
			}
		} 
		else 
		{
			// since an empty object name cannot be read
			// as a string, we convert it to a "-"
			if (!strcmp(name, ""))
			{
				if (!expect("-"))
				{
					return false;
				}
			} 
			else if (!expect(name))
			{
				// it the object name is incorrect - abort
				return false;
			}
		}

		// everything went well - we read a complete object header
		return true;
	}

	bool TextPersistenceManager::getObjectHeader(String& type_name, PointerSizeInt& ptr)
	{
#		ifdef BALL_DEBUG_PERSISTENCE
			Log.info() << "entering getObjectHeader()" << endl;
#		endif

		if (!expect("OBJECT"))
		{
			return false;
		}

		*istr_ >> type_name;
#		ifdef BALL_DEBUG_PERSISTENCE
			Log.info() << "read type name: " << type_name << endl;
#		endif

		// retrieve the 
		if (!expect("@"))
		{
			return false;
		}
		
		get(ptr);
		if (ptr == 0) 
		{
#			ifdef BALL_DEBUG_PERSISTENCE
				Log.error() << "Could not read a valid object pointer: " 
										<< dec << (PointerInt)ptr << "!" << endl;
#			endif

			return false;
		} 
#		ifdef BALL_DEBUG_PERSISTENCE
		else 
		{
			Log.info() << "Read pointer: " << ptr << endl;
		}
#		endif

		return expect("-");
	}

	void TextPersistenceManager::writeName(const char* name)
	{
		if ((name == 0) || !strcmp(name, ""))
		{
			*ostr_ << " - ";
		} 
		else 
		{
			*ostr_ << ' ' << name << ' ';
		}
	}

	bool TextPersistenceManager::checkName(const char* name)
	{
#		ifdef BALL_DEBUG_PERSISTENCE
			Log.info() << "entering checkName()" << endl;
#endif

		String s;
		*istr_ >> s;
			
		if ((name == 0) || !strcmp(name, ""))
		{
			return (s == "-");
		} 
		
		return (s == name);
	}

	void TextPersistenceManager::writeStorableHeader(const char* type_name, const char* name)
	{
		*ostr_ << indent() << "STORABLE";
		writeName(type_name);
		writeName(name);
		*ostr_ << endl;
		++indent_depth_;
	}

	bool TextPersistenceManager::checkStorableHeader(const char* type_name, const char* name)
	{
#		ifdef BALL_DEBUG_PERSISTENCE
			Log.info() << "entering checkStorableHeader()" << endl;
#		endif

		return (expect("STORABLE") && checkName(type_name) && checkName(name));
	}

	void TextPersistenceManager::writeStorableTrailer()
	{
		// decrease indentation by one column and write a trailer string
		--indent_depth_;
		*ostr_ << indent() << "END_STORABLE" << endl;
	}

	bool TextPersistenceManager::checkStorableTrailer()
	{
#		ifdef BALL_DEBUG_PERSISTENCE
			Log.info() << "entering checkStorableTrailer()" << endl;
#		endif

		// check for the trailer "END_STORABLE"
		return expect("END_STORABLE");
	}

	void TextPersistenceManager::writePrimitiveHeader(const char* type_name, const char* name)
	{
		*ostr_ << indent() << type_name;
		writeName(name);
		*ostr_ << "=";
	}

	bool TextPersistenceManager::checkPrimitiveHeader(const char* type_name, const char* name)
	{
#		ifdef BALL_DEBUG_PERSISTENCE
			Log.info() << "entering checkPrimitiveHeader(" << name << ")" << endl;
#		endif

		if (!strcmp(name, ""))
		{
			return (expect(type_name) && expect("-") && expect("="));
		} 
		else 
		{
			return (expect(type_name) && expect(name) && expect("="));
		}
	}

	void TextPersistenceManager::writePrimitiveTrailer()
	{
		*ostr_ << endl;
	}

	bool TextPersistenceManager::checkPrimitiveTrailer()
	{
#		ifdef BALL_DEBUG_PERSISTENCE
			Log.info() << "entering checkPrimitiveTrailer()" << endl;
#		endif

		return true;
	}

	void TextPersistenceManager::writeObjectPointerHeader(const char* type_name, const char* name)
	{
		*ostr_ << indent() << type_name << " *";
		writeName(name);
	}
	
	bool TextPersistenceManager::checkObjectPointerHeader(const char* type_name, const char* name)
	{
#		ifdef BALL_DEBUG_PERSISTENCE
			Log.info() << "entering checkObjectPointerHeader()" << endl;
#		endif

		return (checkName(type_name) && expect("*") && checkName(name));
	}
	
	void TextPersistenceManager::writeObjectReferenceHeader(const char* type_name, const char* name)
	{
		*ostr_ << indent() << type_name << " &";
		writeName(name);
	}
	
	bool TextPersistenceManager::checkObjectReferenceHeader(const char* type_name, const char* name)
	{
#		ifdef BALL_DEBUG_PERSISTENCE
			Log.info() << "entering checkObjectReferenceHeader()" << endl;
#		endif

		return (checkName(type_name) && expect("&") && checkName(name));
	}
	
	void TextPersistenceManager::writeObjectPointerArrayHeader(const char* type_name, const char* name, Size size)
	{
		*ostr_ << indent() << type_name;
		writeName(name);
		*ostr_ << " [";
		put(size);
		*ostr_ << "] = {";
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
		*ostr_ << " }" << endl;
	}
			
	bool TextPersistenceManager::checkObjectPointerArrayTrailer()
	{
		return expect("}");
	}

	/// Layer 0: primitive put methods
	void TextPersistenceManager::put(const char c)
	{
		*ostr_ << ' ' << (int)c << ' ';
	}

	void TextPersistenceManager::put(const Byte c)
	{
		*ostr_ << ' ' << (int)c << ' ';
	}

	void TextPersistenceManager::put(const bool b)
	{	
		// write a boolean value as TRUE or FALSE
		if (b)
		{
			*ostr_ << " TRUE ";
		} 
		else 
		{
			*ostr_ << " FALSE ";
		}
	}
	
// define a macro for all numeric types
// just write it to a stream...
#define BALL_DEFINE_NUMBER_PUT(type)\
	void TextPersistenceManager::put(const type i)\
	{\
		*ostr_ << ' ' << i << ' ';\
	}\

	BALL_DEFINE_NUMBER_PUT(Size)
	BALL_DEFINE_NUMBER_PUT(Index)
	BALL_DEFINE_NUMBER_PUT(PointerSizeInt)
	BALL_DEFINE_NUMBER_PUT(Real)
	BALL_DEFINE_NUMBER_PUT(DoubleReal)

	void TextPersistenceManager::put(const string& s)
	{
		// strings are prefixed by their size
		*ostr_ << ' ' << s.size() << ' ';

		// then we write each characters sequentially
		for (Size i = 0; i < s.size(); i++)
		{
			ostr_->put(s[i]);
		}

		// and terminate everything with a blank
		*ostr_ << ' ';
	}

	void TextPersistenceManager::get(char& c)
	{
		int i;
		*istr_ >> i;
		c = (char)i;
	}

	void TextPersistenceManager::get(Byte& c)
	{
		int i;
		*istr_ >> i;
		c = (char)i;
	}

	void TextPersistenceManager::get(bool& b)
	{
		String s;
		*istr_ >> s;
		b = (s == "TRUE");
	}

	void TextPersistenceManager::get(string& s)
	{
		// read the number of characters
		int i;
		*istr_ >> i;

		// read the leading blank
		char c;	
		istr_->get(c);

		// clear the string
		s.erase();
		
		// read i characters into the string
		if (i > 0)
		{
			for (; i > 0; i--)
			{
				istr_->get(c);
				s += c;
			}
		}
	}

#define BALL_DEFINE_NUMBER_GET(type)\
	void TextPersistenceManager::get(type& i)\
	{\
		*istr_ >> i;\
	}\


	BALL_DEFINE_NUMBER_GET(Index)
	BALL_DEFINE_NUMBER_GET(Size)
	BALL_DEFINE_NUMBER_GET(PointerSizeInt)
	BALL_DEFINE_NUMBER_GET(Real)
	BALL_DEFINE_NUMBER_GET(DoubleReal)

} // namespace BALL
