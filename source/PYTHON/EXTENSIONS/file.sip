// $Id: file.sip,v 1.1 2001/09/03 16:53:08 oliver Exp $

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

    const String *a0;
    PyObject *a0obj;
    File::OpenMode a1 = File::IN;

    if (sipParseArgs(&sipArgsParsed,sipArgs,"I|i",sipCanConvertTo_String,&a0obj,&a1))
    {
      bool res;
      File *ptr;

      if ((ptr = (File *)sipGetCppPtr((sipThisType*)a0,sipClass_File)) == NULL)
        return NULL;

      int iserr = 0;

      int istemp0 = sipConvertTo_String(a0obj,(String **)&a0,1,&iserr);

      if (iserr)
        return NULL;

      res = ptr -> File::open(* a0, a1);

			if (res == false)
			{
				Log.error() << "Cannot open file " << *a0 << std::endl;
			}

      if (istemp0)
        delete a0;

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
  // bool operator == (const File&) const;
  // bool operator != (const File&) const;
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
	PyMethods:
	__str__
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
%CanConvertToTypeCode
  // automatic conversion of Py integers to Position
  return (PyInt_Check(sipPy) || sipIsSubClassInstance(sipPy, sipClass_OpenMode));
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
