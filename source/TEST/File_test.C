// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: File_test.C,v 1.34.2.2 2003/02/05 15:33:58 anker Exp $

#include <BALL/CONCEPT/classTest.h>

///////////////////////////
#include <BALL/SYSTEM/file.h>
#include <sys/types.h>
#include <sys/stat.h>
///////////////////////////

using namespace BALL;
using namespace std;

#include "networkTest.h"

#ifdef BALL_COMPILER_MSVC
#	define sleep(a) _sleep(1000 * a)
#endif

START_TEST(File, "$Id: File_test.C,v 1.34.2.2 2003/02/05 15:33:58 anker Exp $")


/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

File* f1;

CHECK(File())
	f1 = new File();
	TEST_NOT_EQUAL(f1, 0)
RESULT

CHECK(~File())
	delete f1;
RESULT

CHECK(File(const String& name, OpenMode open_mode = std::ios::in))
	File f("data/File_test.txt");
	TEST_EQUAL(f.getSize(), 100)

	File* f2;
	TEST_EXCEPTION(Exception::FileNotFound, f2 = new File("x"))
	TEST_EXCEPTION(Exception::FileNotFound, f2 = new File("sdffsdf"))
RESULT

CHECK(File(const File& file))
	File  file("data/File_test.txt");
	const File& f  = file;
	File f1(f);
	TEST_EQUAL(f1 == f, true)

	File f2;
	TEST_EXCEPTION(Exception::FileNotFound, File f3(f2))
RESULT

CHECK(close())
	File  file("data/File_test.txt");
	const File& f  = file;
	TEST_EQUAL(file.getSize(), 100)
	file.close();
	TEST_EQUAL(f.isClosed(), true)
	TEST_EQUAL(file.getSize(), 100)
RESULT

CHECK(open(const String& name, OpenMode open_mode = std::ios::in))
	File  file("data/File_test.txt");
	const File& f  = file;
	file.open("data/File_test.txt");
	TEST_EQUAL(f.isOpen(), true)
	TEST_EQUAL(file.getSize(), 100)

	File f1(f);
	TEST_EXCEPTION(Exception::FileNotFound, f1.open(""))
RESULT

CHECK(reopen())
	File  file("data/File_test.txt");
	const File& f  = file;
	file.close();
	file.reopen();
	file.reopen();
	TEST_EQUAL(f.isOpen(), true)
	TEST_EQUAL(file.getSize(), 100)
RESULT

CHECK(getName())
	File  file("data/File_test.txt");
	const File& f  = file;
	TEST_EQUAL(f.getName(), "data/File_test.txt")
	TEST_EQUAL(file.getSize(), 100)
RESULT

CHECK(getSize())
	File  file("data/File_test.txt");
	TEST_EQUAL(file.getSize(), 100)
RESULT

CHECK(static getSize(String filename))
	File  file("data/File_test.txt");
	TEST_EQUAL(file.getSize("data/File_test.txt"), 100)
	TEST_EXCEPTION(Exception::FileNotFound, file.getSize("XXX"))
RESULT

CHECK(int getOpenMode() const)
	File  file("data/File_test.txt");
	const File& f  = file;
	TEST_EQUAL(f.getOpenMode(), std::ios::in)
	TEST_EQUAL(file.getSize(), 100)
RESULT

CHECK(static Type getType(String name, bool trace_link))
	File  file("data/File_test.txt");
	TEST_EQUAL(file.getType("data/File_test.txt", false), 4)
	TEST_EQUAL(file.getSize(), 100)
RESULT

CHECK(Type getType(bool trace_link) const;)
	File  file("data/File_test.txt");
	const File& f  = file;
	TEST_EQUAL(f.getType(false), 4)
	TEST_EQUAL(f.getType(true), 4)
	TEST_EQUAL(file.getSize(), 100)
RESULT

CHECK(copy(String source_name, String destination_name, Size buffer_size = 4096))
	File  file("data/File_test.txt");
	const File& f  = file;
	TEST_EQUAL(f.copy("data/File_test.txt", "data/File_test.txt"), false)
	TEST_EQUAL(f.copy("", "data/File_test.txt"), false)
	TEST_EQUAL(f.copy("data/File_test.txt", ""), false)
	TEST_EQUAL(f.copy("", ""), false)
	TEST_EXCEPTION(Exception::FileNotFound, f.copy("ZZZZZZZZZZ", "XXX"))
	TEST_EQUAL(f.copy("data/File_test.txt", "XXX"), true)
	TEST_EQUAL(f.copy("data/File_test.txt", "XXX"), true)
	TEST_EQUAL(file.getSize(), 100)
	TEST_EQUAL(file.getSize("XXX"), 100)
	f.remove("XXX");

	TEST_EQUAL(f.copy("", "X"), false)
	TEST_EQUAL(f.copy("data/File_test.txt", ""), false)
RESULT

CHECK(copyTo(const String& destination_name, Size buffer_size = 4096))
	File  file("data/File_test.txt");
	const File& f  = file;
	TEST_EQUAL(file.copyTo("data/File_test.txt"), false)
	TEST_EQUAL(file.copyTo(""), false)
	TEST_EQUAL(file.copyTo("XXX"), true)
	TEST_EQUAL(file.copyTo("XXX"), true)
	TEST_EQUAL(file.getSize(), 100)
	TEST_EQUAL(file.getSize("XXX"), 100)
	f.remove("XXX");

	TEST_EQUAL(file.copyTo(""), false)
RESULT

CHECK(move(const String& source_name, const String& destination_name))
	File  file("data/File_test.txt");
	const File& f  = file;
	TEST_EQUAL(file.copyTo("XXX"), true)
	TEST_EQUAL(file.copyTo("YYY"), true)
	TEST_EQUAL(f.move("XXX", "XXX"), false)
	TEST_EQUAL(f.move("", "XXX"), false)
	TEST_EQUAL(f.move("XXX", ""), false)

	TEST_EQUAL(f.move("XXX", "YYY") && f.move("YYY", "XXX"), true)
	TEST_EQUAL(f.isAccessible("YYY"), false)
	TEST_EQUAL(file.getSize("XXX"), 100)

	file.copyTo("XXX");
	TEST_EQUAL(f.move("XXX", "YYY") && f.move("YYY", "XXX"), true)
	TEST_EQUAL(file.getSize(), 100)

	TEST_EQUAL(f.move("YYY", ""), false)
	TEST_EQUAL(f.move("", "XXX"), false)

	f.remove("XXX");
	f.remove("YYY");

	TEST_EXCEPTION(Exception::FileNotFound, f.move("ZZZZZZZZZZ", "XXX"))
RESULT

CHECK(moveTo(const String& destination_name))
	File  file("data/File_test.txt");
	const File& f  = file;
	file.copyTo("XXX");
	File f1("XXX");
	TEST_EQUAL(f1.moveTo("XXX"), false)
	TEST_EQUAL(f1.moveTo(""), false)
	TEST_EQUAL(f1.moveTo("YYY"), true)
	TEST_EQUAL(f1.isAccessible(), true)
	TEST_EQUAL(f1.moveTo("YYY"), false)
	TEST_EQUAL(f1.isAccessible(), true)
	TEST_EQUAL(file.getSize("YYY"), 100)

	TEST_EQUAL(f1.moveTo(""), false)
	TEST_EQUAL(f.remove("YYY"), true)
	TEST_EXCEPTION(Exception::FileNotFound, f1.moveTo("XXX"))
RESULT

CHECK(remove(String name))
	File  file("data/File_test.txt");
	const File& f  = file;
	file.copyTo("XXX");
	TEST_EQUAL(f.remove("XXX"), true)
	TEST_EQUAL(file.getSize(), 100)
	TEST_EQUAL(f.remove("XXX"), false)
RESULT

CHECK(remove())
	File  file("data/File_test.txt");
	file.copyTo("XXX");
	File f1 = File("XXX");
	TEST_EQUAL(f1.remove(), true)
	TEST_EQUAL(f1.remove(), false)
	TEST_EQUAL(f1.isAccessible(), false)
RESULT

CHECK(rename(String old_path, String new_path) - Part 1)
	File  file("data/File_test.txt");
	file.copyTo("XXX");
RESULT

CHECK(rename(String old_path, String new_path) - Part 2)
	File  file("data/File_test.txt");
	const File& f  = file;
	File f1("XXX");
	TEST_EQUAL(f1.rename("XXX", "XXX"), true)
	f1.close();
	TEST_EQUAL(f1.rename("XXX", "YYY") && f1.rename("YYY", "XXX"), true)

	TEST_EQUAL(f1.isAccessible("YYY"), false)
	TEST_EQUAL(f1.isAccessible("XXX"), true)
	f1.remove();

	TEST_EQUAL(file.getSize(), 100)
	f.remove("YYY");

	TEST_EXCEPTION(Exception::FileNotFound, f1.rename("", "XXX"))
	TEST_EXCEPTION(Exception::FileNotFound, f1.rename("XXX", ""))
RESULT

CHECK(renameTo(const String& new_path) - Part 1)
	File  file("data/File_test.txt");
	file.copyTo("XXX");
RESULT

CHECK(renameTo(const String& new_path) - Part 2)
	File f1("XXX");
	TEST_EQUAL(f1.renameTo("XXX"), true)
	TEST_EQUAL(f1.isAccessible("XXX"), true)
	TEST_EQUAL(f1.renameTo("YYY"), true)
	TEST_EQUAL(f1.isAccessible("XXX"), false)
	TEST_EQUAL(f1.isAccessible("YYY"), true)
	f1.remove();
RESULT

CHECK(truncate(String path, Size size = 0))
	File  file("data/File_test.txt");
	file.copyTo("XXX");
	File f1("XXX");
	TEST_EQUAL(f1.truncate("XXX", 50), true)
	TEST_EQUAL(f1.getSize(), 50)
	TEST_EQUAL(f1.truncate("XXX", 0), true)
	TEST_EQUAL(f1.getSize(), 0)
	f1.remove();

	TEST_EXCEPTION(Exception::FileNotFound, f1.truncate("", 50))
RESULT

CHECK(truncate(Size size = 0))
	File  file("data/File_test.txt");
	file.copyTo("XXX");
	File f1("XXX");
	TEST_EQUAL(f1.truncate(50), true)
	TEST_EQUAL(f1.getSize(), 50)
	TEST_EQUAL(f1.truncate(0), true)
	TEST_EQUAL(f1.getSize(), 0)
	f1.remove();
RESULT

CHECK(createTemporaryFilename(String& temporary))
	File  file("data/File_test.txt");
	const File& f  = file;
	String s;
	TEST_EQUAL(f.createTemporaryFilename(s), true)
	TEST_NOT_EQUAL(s, "")
RESULT

CHECK(operator == (const File& file))
	File  file("data/File_test.txt");
	const File& f  = file;
	File f1(f);
	TEST_EQUAL(f1 == f, true)	
	file.copyTo("XXX");
	File f2("XXX");
	TEST_EQUAL(f2 == f, false)	
	TEST_EQUAL(file.getSize(), 100)
RESULT

CHECK(operator != (const File& file))
	File  file("data/File_test.txt");
	const File& f  = file;
	File f1(f);
	TEST_EQUAL(f1 != f, false)	
	file.copyTo("XXX");
	File f2("XXX");
	TEST_EQUAL(f2 != f, true)	
	TEST_EQUAL(file.getSize(), 100)
RESULT

CHECK(isAccessible(String name))
	File  file("data/File_test.txt");
	const File& f  = file;
	TEST_EQUAL(f.isAccessible("data/File_test.txt"), true)
	f.remove("XXX");
	TEST_EQUAL(f.isAccessible("XXX"), false)
RESULT

CHECK(isAccessible() - part 1)
	
	File  file("data/File_test.txt");
	const File& f  = file;
	TEST_EQUAL(f.isAccessible(), true)
	file.copyTo("XXZ");
RESULT

CHECK(isAccessible() - part 2)
	File f1("XXZ");
	
	f1.remove();
	TEST_EQUAL(f1.isAccessible(), false)
RESULT

CHECK(isCanonized())
	File f0("../TEST/data/File_test.txt");
	TEST_EQUAL(f0.isValid(), true)
	TEST_EQUAL(f0.isCanonized(), true)

	File f2("data//File_test.txt");
	TEST_EQUAL(f2.isValid(), true)
	TEST_EQUAL(f2.isCanonized(), true)

	File f4("data/../data/File_test.txt");
	TEST_EQUAL(f4.isValid(), true)
	TEST_EQUAL(f4.isCanonized(), true)

	File f5("./data/File_test.txt");
	TEST_EQUAL(f5.isValid(), true)
	TEST_EQUAL(f5.isCanonized(), true)

	File f6("data/File_test.txt");
	TEST_EQUAL(f6.isValid(), true)
	TEST_EQUAL(f6.isCanonized(), true)
RESULT

CHECK(isReadable(String name))
	File  file("data/File_test.txt");
	const File& f  = file;
	TEST_EQUAL(f.isReadable("File_test.C"), true)	
RESULT

CHECK(isReadable())
	File f2("File_test.C");
	TEST_EQUAL(f2.isReadable(), true)
RESULT

CHECK(isWritable(String name))
	File  file("data/File_test.txt");
	const File& f  = file;
	TEST_EQUAL(f.isWritable("File_test.C"), true)	
RESULT

CHECK(isWritable())
	File f2("File_test.C");
	TEST_EQUAL(f2.isWritable(), true)	
RESULT

CHECK(isExecutable(String name))
	File  file("data/File_test.txt");
	const File& f  = file;
#ifndef BALL_COMPILER_MSVC
	TEST_EQUAL(f.isExecutable(BALL_PATH "/source/configure"), true)	
#endif
	TEST_EQUAL(f.isExecutable("File_test.C"), false)	
RESULT

CHECK(isExecutable())
#ifndef BALL_COMPILER_MSVC
	File f1(BALL_PATH "/source/configure");
	TEST_EQUAL(f1.isExecutable(), true)	
#endif
	File f2("File_test.C");
	TEST_EQUAL(f2.isExecutable(), false)	
RESULT

CHECK(isValid())
	File f;
	TEST_EQUAL(f.isValid(), false)	

	File f1;
	TEST_EXCEPTION(Exception::FileNotFound, f1 = File("XXY"))
	TEST_EQUAL(f1.isValid(), false)	

	File f2("File_test.C");
	TEST_EQUAL(f2.isValid(), true)	
	f2.close();
	TEST_EQUAL(f2.isValid(), true)	
RESULT

CHECK(isOpen())
	File  file("data/File_test.txt");
	const File& f  = file;
	TEST_EQUAL(f.isOpen(), true)	
	File f2;
	TEST_EQUAL(f2.isOpen(), false)	
	f2.open("File_test.C");
	TEST_EQUAL(f2.isOpen(), true)	
	f2.close();
	TEST_EQUAL(f2.isOpen(), false)	
RESULT

CHECK(isClosed())
	File  file("data/File_test.txt");
	const File& f  = file;
	TEST_EQUAL(f.isClosed(), false)	
	File f2;
	TEST_EQUAL(f2.isClosed(), true)	
	f2.open("File_test.C");
	TEST_EQUAL(f2.isClosed(), false)	
	f2.close();
	TEST_EQUAL(f2.isClosed(), true)	
RESULT

CHECK(TCPTransfer/1)
	bool network = NetworkTest::test("www.mpi-sb.mpg.de", NetworkTest::HTTP);
	STATUS("network status of www.mpi-sb.mpg.de: " << (network ? "up" : "down"))
	ABORT_IF(!network)
	File f("http://www.mpi-sb.mpg.de/BALL/test/http_test.txt");
	String filename;
	NEW_TMP_FILE(filename)
	f.copyTo(filename);
	TEST_FILE(filename.c_str(), "data/http_test.txt")
RESULT

sleep(2);

CHECK(TCPTransfer/2)
	// just repeat test to make sure we didn't mess up ports or stuff...
	bool network = NetworkTest::test("www.mpi-sb.mpg.de", NetworkTest::HTTP);
	STATUS("network status of www.mpi-sb.mpg.de: " << (network ? "up" : "down"))
	ABORT_IF(!network)
	File f("http://www.mpi-sb.mpg.de/BALL/test/http_test.txt");
	String filename;
	NEW_TMP_FILE(filename)
	f.copyTo(filename);
	TEST_FILE(filename.c_str(), "data/http_test.txt")
RESULT

sleep(2);

CHECK(TCPTransfer/3)
	bool network = NetworkTest::test("ftp.mpi-sb.mpg.de", NetworkTest::FTP);
	STATUS("network status of ftp.mpi-sb.mpg.de: " << (network ? "up" : "down"))
	ABORT_IF(!network)
	File f("ftp://ftp.mpi-sb.mpg.de/pub/outgoing/BALL/ftp_test.txt");
	String filename;
	NEW_TMP_FILE(filename)
	f.copyTo(filename);
	TEST_FILE(filename.c_str(), "data/ftp_test.txt")
RESULT	

sleep(2);

CHECK(TCPTransfer/4)
	// just repeat test to make sure that FTP transfers don't upset HTTP transfers
	bool network = NetworkTest::test("ftp.mpi-sb.mpg.de", NetworkTest::FTP);
	STATUS("network status of ftp.mpi-sb.mpg.de: " << (network ? "up" : "down"))
	ABORT_IF(!network)
	File f("ftp://ftp.mpi-sb.mpg.de/pub/outgoing/BALL/ftp_test.txt");
	String filename;
	NEW_TMP_FILE(filename)
	f.copyTo(filename);
	TEST_FILE(filename.c_str(), "data/ftp_test.txt")
RESULT	

sleep(2);

CHECK(TCPTransfer/5)
	// ... and the other way round
	bool network = NetworkTest::test("ftp.mpi-sb.mpg.de", NetworkTest::FTP);
	STATUS("network status of ftp.mpi-sb.mpg.de: " << (network ? "up" : "down"))
	ABORT_IF(!network)
	File f("ftp://ftp.mpi-sb.mpg.de/pub/outgoing/BALL/ftp_test.txt");
	String filename;
	NEW_TMP_FILE(filename)
	f.copyTo(filename);
	TEST_FILE(filename.c_str(), "data/ftp_test.txt")
RESULT	

sleep(2);

CHECK(TCPTransfer/6)
	bool network = NetworkTest::test("www.mpi-sb.mpg.de", NetworkTest::HTTP);
	STATUS("network status of www.mpi-sb.mpg.de: " << (network ? "up" : "down"))
	ABORT_IF(!network)
	File::registerTransformation("BALLhttp://", "http://www.mpi-sb.mpg.de/BALL/test/%b.txt");
	File f("BALLhttp://http_test");
	String filename;
	NEW_TMP_FILE(filename)
	f.copyTo(filename);
	TEST_FILE(filename.c_str(), "data/http_test.txt")
RESULT

sleep(2);

CHECK(TCPTransfer/7)
	bool network = NetworkTest::test("ftp.mpi-sb.mpg.de", NetworkTest::FTP);
	STATUS("network status of ftp.mpi-sb.mpg.de: " << (network ? "up" : "down"))
	ABORT_IF(!network)
	File::registerTransformation("BALLftp://", "ftp://ftp.mpi-sb.mpg.de/pub/outgoing/BALL/%b.txt");
	File f("BALLftp://ftp_test");
	String filename;
	NEW_TMP_FILE(filename)
	f.copyTo(filename);
	TEST_FILE(filename.c_str(), "data/ftp_test.txt")
RESULT

CHECK(TCPTransfer/failedTransfer)
	TEST_EXCEPTION(Exception::FileNotFound, File f("ftp://ftp.mpi-sb.mpg.de/pub/outgoing/BALL/notthere.txt"))
RESULT

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
