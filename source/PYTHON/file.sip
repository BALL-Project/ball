class File
{
%HeaderCode
#include <BALL/SYSTEM/file.h>
using namespace BALL;
%End
  public:
	File();
  File(String&, int);

  // static void enableProtocol(Protocol);
  // static void disableProtocol(Protocol);
  bool open(String&, int);
  bool reopen();
  void close();
  const String& getName() const;
  const String& getOriginalName() const;
  int getSize() const;
	int getOpenMode() const;
  // static Type getType(String, bool);
  int getType(bool) const;
  // std::fstream& getFileStream();
  // static bool copy(String, String, int);
  bool copyTo(String&, int);
  // static bool move(String&, String&);
  bool moveTo(String&);
  // static bool remove(String);
  bool remove() const;
  // static bool rename(String, String);
  bool renameTo(const String&) const;
  // static bool truncate(String, int);
  bool truncate(int) const;
  // static void registerAction(String&, String&);
  // static void unregisterAction(String&);
  // static bool createTemporaryFilename(String&);
  // bool operator == (const File& file) const;
  // bool operator != (const File& file) const;
  // static bool isProtocolEnabled(Protocol);
  bool isOpen() const;
  bool isClosed() const;
  // static bool isAccessible(String);
  bool isAccessible() const;
  bool isCanonized() const;
  // static bool isReadable(String);
  bool isReadable() const;
  // static bool isWritable(String);
  bool isWritable() const;
  // static bool isExecutable(String);
  bool isExecutable() const;
  virtual bool hasFormat();
  bool hasFormat() const;
  virtual bool hasFormat(String&) const;
  bool isValid() const;
};
 
