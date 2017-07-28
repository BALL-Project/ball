class std__openmode
{
%TypeHeaderCode
	#include <BALL/SYSTEM/file.h>
	using std__openmode = File::OpenMode;
%End
	public:
};

class File
{
%TypeHeaderCode
	#include <BALL/SYSTEM/file.h>
%End
	public:

	File();
	File(const String&, OpenMode openmode = std::ios::in);
	~File();
	bool open(const String&, OpenMode openmode = std::ios::in);

	bool reopen();
	void close();
	const String& getName() const;
	Size getSize();
	OpenMode getOpenMode() const;
	bool copyTo(const String&);
	bool moveTo(const String&);
	bool remove();
	bool renameTo(const String&);
	bool truncate(int) const;
	bool operator == (const File&) const;
	bool operator != (const File&) const;
	bool isOpen() const;
	bool isClosed() const;
	bool isAccessible() const;
	bool isCanonized() const;
	bool isReadable() const;
	bool isWritable() const;
	bool isExecutable() const;
	bool isValid() const;

	private:
	File(const File&);
};

class OpenMode
{
%TypeHeaderCode
	#include <BALL/SYSTEM/file.h>
	using OpenMode = File::OpenMode;
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
