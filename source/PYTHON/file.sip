// $Id: file.sip,v 1.2 2000/06/20 22:16:11 oliver Exp $
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
  // virtual ~File();
  // static void enableProtocol(Protocol protocol);
  // static void disableProtocol(Protocol protocol);
	// default open mode: READ
  bool open(const String&, int = 1);
  bool reopen();
  void close();
  const String& getName() const;
  const String& getOriginalName() const;
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
  virtual bool hasFormat();
  bool hasFormat() const;
  virtual bool hasFormat(const String&) const;
  bool isValid() const;
};
