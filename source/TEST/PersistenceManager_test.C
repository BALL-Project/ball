// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/CONCEPT/classTest.h>

///////////////////////////

// insert includes here
#include <BALL/CONCEPT/persistenceManager.h>
#include <BALL/KERNEL/atom.h>

///////////////////////////


namespace BALL
{

	class PersistenceTest
		: public PersistenceManager
	{
		public: 

		PersistenceTest()
			:	PersistenceManager()
		{
		}

		PersistenceTest(const PersistenceTest& pt)
			:	PersistenceManager(pt)
		{
		}

		virtual ~PersistenceTest()
			throw()
		{
		}

		// define the abstract classes of PersistenceManager

		// Layer 0

		void writeHeader(const char* /* type_name */, const char* /* name */,
										 LongSize /* ptr */)
		{
		}

		bool checkHeader(const char* /* type_name */, const char* /* name */,
										 LongSize& /* ptr */)
		{
			return true;
		}

		void writeTrailer(const char* /* name */ = 0)
		{
		}

		bool checkTrailer(const char* /* name */ = 0)
		{
			return true;
		}

		void writeStreamHeader()
		{
		}

		void writeStreamTrailer()
		{
		}

		bool checkStreamHeader()
		{
			return true;
		}

		bool checkStreamTrailer()
		{
			return true;
		}

		bool getObjectHeader(String& /* type_name */, LongSize& /* ptr */)
		{
			return true;
		}

		void writeName(const char* /* name */)
		{
		}

		bool checkName(const char* /* name */)
		{
			return true;
		}

		void writeStorableHeader(const char* /* type_name */, const char* /* name */)
		{
		}

		bool checkStorableHeader(const char* /* type_name */, const char* /* name */)
		{
			return true;
		}

		void writeStorableTrailer()
		{
		}

		bool checkStorableTrailer()
		{
			return true;
		}

		void writePrimitiveHeader(const char* /* type_name */, const char* /* name */)
		{
		}

		bool checkPrimitiveHeader(const char* /* type_name */, const char* /* name */)
		{
			return true;
		}

		void writePrimitiveTrailer()
		{
		}

		bool checkPrimitiveTrailer()
		{
			return true;
		}

		void writeObjectPointerHeader(const char* /* type_name */, const char* /* name */)
		{
		}

		bool checkObjectPointerHeader(const char* /* type_name */, const char* /* name */)
		{
			return true;
		}

		void writeObjectReferenceHeader(const char* /* type_name */, const char* /* name */)
		{
		}

		bool checkObjectReferenceHeader(const char* /* type_name */, const char* /* name */)
		{
			return true;
		}

		void writeObjectPointerArrayHeader(const char* /* type_name */,
				const char* /* name */, Size /* size */)
		{
		}

		bool checkObjectPointerArrayHeader(const char* /* type_name */,
				const char* /* name */, Size& /* size */)
		{
			return true;
		}

		void writeObjectPointerArrayTrailer()
		{
		}

		bool checkObjectPointerArrayTrailer()
		{
			return true;
		}

		void put(const char /* c */)
		{
		}

		void put(const Byte /* c */)
		{
		}

		void put(const Index /* i */)
		{
		}

		void put(const Size /* p */)
		{
		}

		void put(const bool /* b */)
		{
		}

		void put(const Real /* f */)
		{
		}

		void put(const DoubleReal /* d */)
		{
		}

		void put(const string& /* s */)
		{
		}

		void put(const LongSize /* p */)
		{
		}

		void get(char& /* c */)
		{
		}

		void get(Byte& /* b */)
		{
		}

		void get(Index& /* s */)
		{
		}

		void get(Size& /* s */)
		{
		}

		void get(bool& /* b */)
		{
		}

		void get(Real& /* f */)
		{
		}

		void get(DoubleReal& /* d */)
		{
		}

		void get(string& /* s */)
		{
		}

		void get(LongSize& /* p */)
		{
		}


	};

} // namespace BALL


///////////////////////////

START_TEST(PersistanceManager)

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

// the test class

using namespace BALL;

// tests for class PersistenceManager::

PersistenceTest* ptr;

CHECK(PersistenceManager() throw())
	ptr = new PersistenceTest;
	TEST_NOT_EQUAL(ptr, 0)
RESULT


CHECK(PersistenceManager(const PersistenceManager& pm) throw())
  // ???
RESULT


CHECK(PersistenceManager(::std::istream& is) throw())
	// ?????:
	// We need additional accessors to be able to test this method
	// needed: access to istr_
	// no, we don't We just have to create an intermediate derived class
	// for testing!
RESULT


CHECK(PersistenceManager(::std::ostream& os) throw())
  // ?????:
	// We need additional accessors to be able to test this method
	// needed: access to ostr_
RESULT


CHECK(PersistenceManager(::std::istream& is, ::std::ostream& os) throw())
  // ?????:
	// We need additional accessors to be able to test this method
	// needed: access to istr_, ostr_
RESULT


CHECK(~PersistenceManager() throw())
  delete ptr;
RESULT


CHECK(void registerClass(String signature, const CreateMethod m) throw())
  //?????
	// We need additional accessors to be able to test this method
RESULT


CHECK(void* createObject(String signature) const throw())
	// some RTTI check here 
	PersistenceTest pm;
	// ?????
	// String sig = RTTI::getStreamName<Atom>();
	// Atom* atom = (Atom*) pm.createObject(sig);
    // bool test = RTTI::isKindOf<Atom>(&atom);
	// TEST_EQUAL(test, true)
RESULT


CHECK(Size getNumberOfClasses() const throw())
	PersistenceTest tpm;
	Size noc = tpm.getNumberOfClasses();
	TEST_EQUAL(noc, 18);
RESULT


CHECK(void setOstream(::std::ostream& s) throw())
  // ?????:
	// We need additional accessors to be able to test this method
	// needed: access to ostr_
RESULT


CHECK(void setIstream(::std::istream& s) throw())
  // ?????:
	// We need additional accessors to be able to test this method
	// needed: access to istr_
RESULT


CHECK(void startOutput() throw())
  // ?????:
	// We need additional accessors to be able to test this method
	// needed: access to object_out_ and object_out_needed_
RESULT


CHECK(void endOutput() throw())
  // ?????:
	// We need additional accessors to be able to test this method
	// needed: access to object_out_ and object_out_needed_
RESULT


CHECK(PersistentObject* readObject() throw(Exception::GeneralException))
  //?????
RESULT


CHECK(PersistenceManager& operator << (const PersistentObject& object) throw())
  //?????
RESULT


CHECK(PersistenceManager& operator >> (PersistentObject*& object_ptr) throw())
  //?????
RESULT


CHECK(template<typename T> bool checkObjectHeader(const T& /* object */, const char* name = 0) throw())
  //?????
RESULT


CHECK(bool checkObjectHeader(const char* type_name) throw())
  //?????
RESULT


CHECK(void writeObjectTrailer(const char* name = 0) throw())
  //?????
RESULT


CHECK(template<typename T> void writePrimitive(const T& t, const char* name) throw())
  //?????
RESULT


CHECK(template<typename T> bool readPrimitive(T& t, const char* name) throw())
  //?????
RESULT


CHECK(template<typename T> bool readStorableObject(T& t, const char* name) throw())
  //?????
RESULT


CHECK(template<typename T> bool readObjectPointer(T*& object, const char* name) throw())
  //?????
RESULT


CHECK(template<typename T> bool readObjectReference(T& object, const char* name) throw())
  //?????
RESULT


CHECK(template<typename T> void writeObjectArray(const T* array, const char* name, Size size) throw())
  //?????
RESULT


CHECK(template<typename T> bool readObjectArray(const T* array, const char* name, Size& size) throw())
  //?????
RESULT


CHECK(template<typename T> void writeObjectPointerArray(T** arr, const char* name, const Size size) throw())
  //?????
RESULT


CHECK(template<typename T> bool readObjectPointerArray(T** array, const char* name, Size& size) throw())
  //?????
RESULT

CHECK(bool checkHeader(const char* type_name, const char* name, LongSize& ptr))
  // ???
RESULT

CHECK(bool checkName(const char* name))
  // ???
RESULT

CHECK(bool checkObjectPointerArrayHeader(const char* type_name, const char* name, Size& size))
  // ???
RESULT

CHECK(bool checkObjectPointerArrayTrailer())
  // ???
RESULT

CHECK(bool checkObjectPointerHeader(const char* type_name, const char* name))
  // ???
RESULT

CHECK(bool checkObjectReferenceHeader(const char* type_name, const char* name))
  // ???
RESULT

CHECK(bool checkObjectTrailer(const char* name = 0) throw())
  // ???
RESULT

CHECK(bool checkPrimitiveHeader(const char* type_name, const char* name))
  // ???
RESULT

CHECK(bool checkPrimitiveTrailer())
  // ???
RESULT

CHECK(bool checkStorableHeader(const char* type_name, const char* name))
  // ???
RESULT

CHECK(bool checkStorableTrailer())
  // ???
RESULT

CHECK(bool checkStreamHeader())
  // ???
RESULT

CHECK(bool checkStreamTrailer())
  // ???
RESULT

CHECK(bool checkTrailer(const char* name = 0))
  // ???
RESULT

CHECK(bool getObjectHeader(String& type_name, LongSize& ptr))
  // ???
RESULT

CHECK(template<typename T> void writeObjectHeader(const T* object, const char* name = 0) throw())
  // ???
RESULT

CHECK(template<typename T> void writeObjectPointer(const T* object, const char* name) throw())
  // ???
RESULT

CHECK(template<typename T> void writeObjectReference(const T& object, const char* name) throw())
  // ???
RESULT

CHECK(template<typename T> void writeStorableObject(const T& t, const char* name) throw())
  // ???
RESULT

CHECK(typedefvoid* (*CreateMethod)())
  // ???
RESULT

CHECK(void finalizeInputStream() throw())
  // ???
RESULT

CHECK(void finalizeOutputStream() throw())
  // ???
RESULT

CHECK(void get(Byte& b))
  // ???
RESULT

CHECK(void get(DoubleReal& d))
  // ???
RESULT

CHECK(void get(Index& s))
  // ???
RESULT

CHECK(void get(LongSize& p))
  // ???
RESULT

CHECK(void get(Real& f))
  // ???
RESULT

CHECK(void get(Size& s))
  // ???
RESULT

CHECK(void get(bool& b))
  // ???
RESULT

CHECK(void get(char& c))
  // ???
RESULT

CHECK(void get(string& s))
  // ???
RESULT

CHECK(void initializeInputStream() throw())
  // ???
RESULT

CHECK(void initializeOutputStream() throw())
  // ???
RESULT

CHECK(void put(const Byte c))
  // ???
RESULT

CHECK(void put(const DoubleReal d))
  // ???
RESULT

CHECK(void put(const Index i))
  // ???
RESULT

CHECK(void put(const LongSize p))
  // ???
RESULT

CHECK(void put(const Real f))
  // ???
RESULT

CHECK(void put(const Size p))
  // ???
RESULT

CHECK(void put(const bool b))
  // ???
RESULT

CHECK(void put(const char c))
  // ???
RESULT

CHECK(void put(const string& s))
  // ???
RESULT

CHECK(void writeHeader(const char* type_name, const char* name, LongSize ptr))
  // ???
RESULT

CHECK(void writeName(const char* name))
  // ???
RESULT

CHECK(void writeObjectPointerArrayHeader(const char* type_name, const char* name, Size size))
  // ???
RESULT

CHECK(void writeObjectPointerArrayTrailer())
  // ???
RESULT

CHECK(void writeObjectPointerHeader(const char* type_name, const char* name))
  // ???
RESULT

CHECK(void writeObjectReferenceHeader(const char* type_name, const char* name))
  // ???
RESULT

CHECK(void writePrimitiveHeader(const char* type_name, const char* name))
  // ???
RESULT

CHECK(void writePrimitiveTrailer())
  // ???
RESULT

CHECK(void writeStorableHeader(const char* type_name, const char* name))
  // ???
RESULT

CHECK(void writeStorableTrailer())
  // ???
RESULT

CHECK(void writeStreamHeader())
  // ???
RESULT

CHECK(void writeStreamTrailer())
  // ???
RESULT

CHECK(void writeTrailer(const char* name = 0))
  // ???
RESULT

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
