// $Id: file.sip,v 1.10 2003/04/28 15:41:12 oliver Exp $

class std__openmode
{
%HeaderCode
	#include <BALL/SYSTEM/file.h>
	using namespace BALL;
	typedef std::ios::openmode std__openmode;
%End
	public:
};

class File
{
%HeaderCode
	#include <BALL/SYSTEM/file.h>
	using namespace BALL;
%End
  public:
	typedef std__openmode OpenMode;
	static const OpenMode IN;
	static const OpenMode OUT;
	static const OpenMode APP;
	static const OpenMode BINARY;
	static const OpenMode ATE;
	static const OpenMode TRUNC;

  File();
	// default open mode: IN
  File(const String&, File::OpenMode /openmode/ = File::IN);
  File(const File& /filename/);
  ~File();
	// default open mode: IN
  bool open(const String&, File::OpenMode /openmode/ = File::IN);

  bool reopen();
  void close();
  const String& getName() const;
  Size getSize();
  File::OpenMode getOpenMode() const;
  // int getType() const;
  // std::fstream& getFileStream();
  bool copyTo(const String&);
  bool moveTo(const String&);
  bool remove() const;
  bool renameTo(const String&);
  bool truncate(int) const;
  bool operator == (const File& /file/) const;
  bool operator != (const File& /file/) const;
  bool isOpen() const;
  bool isClosed() const;
  bool isAccessible() const;
  bool isCanonized() const;
  bool isReadable() const;
  bool isWritable() const;
  bool isExecutable() const;
  bool isValid() const;
};


class OpenMode
{
%HeaderCode
	#include <BALL/SYSTEM/file.h>
	using namespace BALL;
	typedef File::OpenMode OpenMode;
%End
  public:

  char* __str__() /NonLazy/;
%MemberCode
  OpenMode* ptr;
  if (sipParseArgs(&sipArgsParsed,sipArgs,"n",&sipSelfObj,sipClass_OpenMode,&ptr))
  {
		String mode_string;
		switch ((int)*ptr)
		{
			case (int)std::ios::app: 		mode_string = "std::ios::app"; break;
			case (int)std::ios::binary:	mode_string = "std::ios::binary"; break;
			case (int)std::ios::ate: 		mode_string = "std::ios::ate"; break;
			case (int)std::ios::trunc: 	mode_string = "std::ios::trunc"; break;
			case (int)std::ios::out: 		mode_string = "std::ios::out"; break;
			case (int)std::ios::in: 			mode_string = "std::ios::in"; break; 
			default:						mode_string = *ptr;
		}

		return PyString_FromString(mode_string.c_str());
	}
%End

%ConvertToTypeCode
  if (PyInt_Check(sipPy))
  {
		int mode = PyInt_AS_LONG(sipPy);
		File::OpenMode result;
		switch (mode)
		{
			case std::ios::out:		result = std::ios::out; break;
			case std::ios::binary:	result = std::ios::binary; break;
			case std::ios::ate:		result = std::ios::ate; break;
			case std::ios::trunc:	result = std::ios::trunc; break;
			case std::ios::app:		result = std::ios::app; break;

			case std::ios::in:
			default: result = std::ios::in;
		};
    *sipCppPtr = new OpenMode(result);

    return 1;
  }

  *sipCppPtr = (OpenMode*)sipConvertToCpp(sipPy, sipClass_OpenMode, sipIsErr);

  return 0;
%End
}; 
