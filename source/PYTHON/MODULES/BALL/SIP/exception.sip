// $Id: exception.sip,v 1.2 2000/08/07 11:53:44 oliver Exp $

class GeneralException 
{
%HeaderCode
	#include <BALL/COMMON/exception.h>
	using namespace BALL;
	using namespace BALL::Exception;
%End
	
   public:
   GeneralException();
   GeneralException
    (const char* /file/, int /line/, 
     const string& name = "GeneralException",
     const string& message = "unspecified error");
   GeneralException(const GeneralException& /exception/);
   // virtual ~GeneralException();
   string getName() const;
   string getMessage() const;
   int getLine() const;
   string getFile() const;
};  


class IndexUnderflow 
 	: GeneralException
{
%HeaderCode
	#include <BALL/COMMON/exception.h>
	using namespace BALL;
	using namespace BALL::Exception;
%End
  public:
  IndexUnderflow(const char* /file/, int /line/, Index /index/ = 0, Size /size/ = 0);
   protected:
   Size size_;
   Index index_;
  };
  class SizeUnderflow 
   : public GeneralException
  {
   public:
   SizeUnderflow(const char* file, int line, Size size = 0);
   protected:
   Size size_;
  };
  class IndexOverflow 
   : public GeneralException
  {
   public:
   IndexOverflow(const char* file, int line, Index index = 0, Size size = 0);
   protected:
   Size size_;
   Index index_;
  };
  class InvalidRange 
   : public GeneralException
  {
   public:
   InvalidRange(const char* file, int line);
  };
  class InvalidFormat 
   : public GeneralException
  {
   public:
   InvalidFormat(const char* file, int line);
  };
  class IllegalSelfOperation
   : public GeneralException
  {
   public:
   IllegalSelfOperation(const char* file, int line);
  };
  class NullPointer 
   : public GeneralException
  {
   public:
   NullPointer(const char* file, int line);
  };
  class InvalidIterator
   : public GeneralException
  {
   public:
   InvalidIterator(const char* file, int line);
  };
  class IncompatibleIterators
   : public GeneralException
  {
   public:
   IncompatibleIterators(const char* file, int line);
  };
  class NotImplemented
   : public GeneralException
  {
   public:
   NotImplemented(const char* file, int line);
  };
  class OutOfMemory
   : public GeneralException, public std::bad_alloc
  {
   public:
   OutOfMemory(const char* file, int line, Size size = 0);
   virtual ~OutOfMemory() throw();
   protected:
   Size size_;
  };
  class BufferOverflow 
   : public GeneralException
  {
   public:
   BufferOverflow(const char* file, int line);
  };
  class DivisionByZero 
   : public GeneralException
  {
   public:
   DivisionByZero(const char* file, int line);
  };
  class FileNotFound 
   : public GeneralException
  {
   public:
   FileNotFound(const char* file, int line, const string& filename);
   string getFilename() const;
   protected:
   string filename_;
  };
  class GlobalExceptionHandler
  {
   public:
   GlobalExceptionHandler();
   static void setName(const string& name);
   static void setMessage(const string& message);
   static void setLine(int line);
   static void setFile(const string& file);
   static void set
    (const string& file, int line, 
     const string& name, const string& message);
   protected:
   static void terminate();
   static void newHandler();
   static string file_;
   static int  line_;
   static string name_;
   static string message_;
  };
  extern GlobalExceptionHandler globalHandler;
 }
}
