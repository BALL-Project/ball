// $Id: file.sip,v 1.4 2001/12/27 00:25:43 oliver Exp $

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
  File(const File&);
  ~File();
	// default open mode: IN
  bool open(const String&, File::OpenMode /openmode/ = File::IN);
%MemberCode

    const String* a0;
		int a0IsTemp = 0;
    File::OpenMode a1 = File::IN;

    File* ptr = 0;
    if (sipParseArgs(&sipArgsParsed,sipArgs,"mM1i",sipThisObj, sipClass_File,&ptr,
										sipConvertTo_String,&a0,&a0IsTemp,
										&a1))
    {
      bool res;

      res = ptr -> File::open(*a0, a1);

			if (res == false)
			{
				Log.error() << "Cannot open file " << *a0 << std::endl;
			}

      if (a0IsTemp)
			{
				delete a0;
			}

      return sipConvertFromBool((int)res);
    }
%End

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
  // bool operator != (const File& /file/) const;
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
	PyMethod __str__
%MemberCode
  OpenMode* ptr;
  if ((ptr = (OpenMode*)sipGetCppPtr((sipThisType*)a0,sipClass_OpenMode)) == NULL)
    return NULL;
	
	String mode_string;
	switch ((int)*ptr)
	{
		case (int)File::APP: 		mode_string = "File::APP"; break;
		case (int)File::BINARY:	mode_string = "File::BINARY"; break;
		case (int)File::ATE: 		mode_string = "File::ATE"; break;
		case (int)File::TRUNC: 	mode_string = "File::TRUNC"; break;
		case (int)File::OUT: 		mode_string = "File::OUT"; break;
		case (int)File::IN: 			mode_string = "File::IN"; break;
		default:						mode_string = *ptr;
	}

  return PyString_FromString(mode_string.c_str());
%End
%ConvertToTypeCode
  if (PyInt_Check(sipPy))
  {
		int mode = PyInt_AS_LONG(sipPy);
		File::OpenMode result;
		switch (mode)
		{
			case File::OUT:		result = File::OUT; break;
			case File::BINARY:	result = File::BINARY; break;
			case File::ATE:		result = File::ATE; break;
			case File::TRUNC:	result = File::TRUNC; break;
			case File::APP:		result = File::APP; break;

			case File::IN:
			default: result = File::IN;
		};
    *sipCppPtr = new OpenMode(result);

    return 1;
  }

  *sipCppPtr = (OpenMode*)sipConvertToCpp(sipPy, sipClass_OpenMode, sipIsErr);

  return 0;
%End
}; 
