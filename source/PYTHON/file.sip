// $Id: file.sip,v 1.5 2000/07/06 14:08:47 oliver Exp $
class File
{
%HeaderCode
	#include <BALL/SYSTEM/file.h>
	using namespace BALL;
%End
  public:
  File();
	// default open mode: READ
  File(const String&, int = 1);
  File(const File&);
  ~File();
  // static void enableProtocol(Protocol protocol);
  // static void disableProtocol(Protocol protocol);
	// default open mode: READ
  bool open(const String&, int = 1);
%MemberCode

    const String *a0;
    PyObject *a0obj;
    int a1 = 1;

    if (sipParseArgs(sipArgs,"I|i",sipCanConvertTo_String,&a0obj,&a1))
    {
      bool res;
      File *ptr;

      if ((ptr = (File *)sipGetCppPtr(sipThis,sipClass_File)) == NULL)
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
  int getSize();
  int getOpenMode() const;
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
