// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id$
//

class std__openmode
{
%TypeHeaderCode
	#include <BALL/SYSTEM/file.h>
	using namespace BALL;
	typedef File::OpenMode std__openmode;
%End
	public:
};



class File
{
%TypeHeaderCode
	#include <BALL/SYSTEM/file.h>
	using namespace BALL;

/*
	const File::OpenMode BALL_File__IN = File::MODE_IN;
	const File::OpenMode BALL_File__OUT = File::MODE_OUT;
	const File::OpenMode BALL_File__APP = File::MODE_APP;
	const File::OpenMode BALL_File__BINARY = File::MODE_BINARY;
	const File::OpenMode BALL_File__ATE = File::MODE_ATE;
	const File::OpenMode BALL_File__TRUNC = File::MODE_TRUNC;
*/

%End
  public:

	// nested class File::CannotWrite
	class CannotWrite
	{
		 public:
		 CannotWrite(const char* /file/, int /line/, const String& /filename/) throw();
		 ~CannotWrite() throw();
		 String getFilename() const throw();
	};


	enum {
		MODE_IN,
		MODE_OUT,
		MODE_APP,
		MODE_BINARY,
		MODE_ATE,
		MODE_TRUNC
	};


  File();
	// default open mode: IN
  File(const String&, OpenMode /openmode/ = std::ios::in);
  ~File();
	// default open mode: IN
  bool open(const String&, OpenMode /openmode/ = std::ios::in);

  bool reopen();
  void close();
  const String& getName() const;
  Size getSize();
  OpenMode getOpenMode() const;
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

	private:
  File(const File& /filename/);
};


class OpenMode
{
%TypeHeaderCode
	#include <BALL/SYSTEM/file.h>
	using namespace BALL;
	typedef File::OpenMode OpenMode;
%End
  public:

  SIP_PYOBJECT __str__();
%MethodCode
  OpenMode om = *sipCpp;
	const char* mode_string;
	switch ((int)om)
	{
		case (int)std::ios::app: 		mode_string = "std::ios::app"; break;
		case (int)std::ios::binary:	mode_string = "std::ios::binary"; break;
		case (int)std::ios::ate: 		mode_string = "std::ios::ate"; break;
		case (int)std::ios::trunc: 	mode_string = "std::ios::trunc"; break;
		case (int)std::ios::out: 		mode_string = "std::ios::out"; break;
		case (int)std::ios::in: 		mode_string = "std::ios::in"; break; 
		default:										mode_string = "";
	}
	sipRes = PyString_FromString(mode_string);
%End

%ConvertToTypeCode
  if (sipIsErr == NULL)
    return (PyInt_Check(sipPy) || BALL_IS_SUBCLASS_INSTANCE(sipPy, OpenMode));

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

  *sipCppPtr = BALL_CONVERT_TO_CPP(OpenMode);

  return 0;
%End
}; 
