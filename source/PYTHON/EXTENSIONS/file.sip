// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: file.sip,v 1.13 2003/10/29 22:54:39 oliver Exp $
//

class std__openmode
{
%TypeHeaderCode
	#include <BALL/SYSTEM/file.h>
	using namespace BALL;
	typedef std::ios::openmode std__openmode;
%End
	public:
};

class File
{
%TypeHeaderCode
	#include <BALL/SYSTEM/file.h>
	using namespace BALL;
	const File::OpenMode BALL_File__IN = File::IN;
	const File::OpenMode BALL_File__OUT = File::OUT;
	const File::OpenMode BALL_File__APP = File::APP;
	const File::OpenMode BALL_File__BINARY = File::BINARY;
	const File::OpenMode BALL_File__ATE = File::ATE;
	const File::OpenMode BALL_File__TRUNC = File::TRUNC;
%End
  public:
	typedef std__openmode OpenMode;
	enum {
		IN = BALL_File__IN,
		OUT = BALL_File__OUT,
		APP = BALL_File__APP,
		BINARY = BALL_File__BINARY,
		ATE = BALL_File__ATE,
		TRUNC = BALL_File__TRUNC
	};

  File();
	// default open mode: IN
  File(const String&, File::OpenMode /openmode/ = std::ios::in);
  File(const File& /filename/);
  ~File();
	// default open mode: IN
  bool open(const String&, File::OpenMode /openmode/ = std::ios::in);

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
%TypeHeaderCode
	#include <BALL/SYSTEM/file.h>
	using namespace BALL;
	typedef File::OpenMode OpenMode;
%End
  public:

  char* __str__() /NonLazy/;
%MethodCode
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
