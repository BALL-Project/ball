// $Id: PersistenceManager_test.C,v 1.4 2001/07/15 17:32:40 amoll Exp $
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
				PointerSizeInt /* ptr */)
		{
		}

		bool checkHeader(const char* /* type_name */, const char* /* name */,
				PointerSizeInt& /* ptr */)
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

		bool getObjectHeader(String& /* type_name */, PointerSizeInt& /* ptr */)
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

		void put(const PointerSizeInt /* p */)
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

		void get(PointerSizeInt& /* p */)
		{
		}


	};

} // namespace BALL


///////////////////////////

START_TEST(PersistanceManager, "$Id: PersistenceManager_test.C,v 1.4 2001/07/15 17:32:40 amoll Exp $")

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

// the test class

using namespace BALL;

// tests for class PersistenceManager::

PersistenceTest* ptr;

CHECK(PersistenceManager::PersistenceManager() throw())
	ptr = new PersistenceTest;
	TEST_NOT_EQUAL(ptr, 0)
RESULT


CHECK(PersistenceManager::PersistenceManager(::std::istream& is) throw())
	// BAUSTELLE:
	// We need additional accessors to be able to test this method
	// needed: access to istr_
	// no, we don't We just have to create an intermediate derived class
	// for testing!
RESULT


CHECK(PersistenceManager::PersistenceManager(::std::ostream& os) throw())
  // BAUSTELLE:
	// We need additional accessors to be able to test this method
	// needed: access to ostr_
RESULT


CHECK(PersistenceManager::PersistenceManager(::std::istream& is, ::std::ostream& os) throw())
  // BAUSTELLE:
	// We need additional accessors to be able to test this method
	// needed: access to istr_, ostr_
RESULT


CHECK(PersistenceManager::~PersistenceManager() throw())
  delete ptr;
RESULT


CHECK(PersistenceManager::registerClass(String signature, const CreateMethod m) throw())
  //BAUSTELLE
	// We need additional accessors to be able to test this method
RESULT


CHECK(PersistenceManager::createObject(String signature) const  throw())
	// some RTTI check here 
	PersistenceTest pm;
	// BAUSTELLE
	// String sig = RTTI::getStreamName<Atom>();
	// Atom* atom = (Atom*) pm.createObject(sig);
	// bool test = RTTI::isKindOf<Atom>(atom);
	// TEST_EQUAL(test, true)
RESULT


CHECK(PersistenceManager::getNumberOfClasses() const  throw())
	PersistenceTest tpm;
	Size noc = tpm.getNumberOfClasses();
	TEST_EQUAL(noc, 16);
RESULT


CHECK(PersistenceManager::setOstream(::std::ostream& s) throw())
  // BAUSTELLE:
	// We need additional accessors to be able to test this method
	// needed: access to ostr_
RESULT


CHECK(PersistenceManager::setIstream(::std::istream& s) throw())
  // BAUSTELLE:
	// We need additional accessors to be able to test this method
	// needed: access to istr_
RESULT


CHECK(PersistenceManager::startOutput() throw())
  // BAUSTELLE:
	// We need additional accessors to be able to test this method
	// needed: access to object_out_ and object_out_needed_
RESULT


CHECK(PersistenceManager::endOutput() throw())
  // BAUSTELLE:
	// We need additional accessors to be able to test this method
	// needed: access to object_out_ and object_out_needed_
RESULT


CHECK(PersistenceManager::readObject() throw())
  //BAUSTELLE
RESULT


CHECK(PersistenceManager::PersistenceManager& operator << (const PersistentObject& object) throw())
  //BAUSTELLE
RESULT


CHECK(PersistenceManager::PersistenceManager& operator >> (PersistentObject*& object_ptr) throw())
  //BAUSTELLE
RESULT


CHECK(PersistenceManager::checkObjectHeader(const T& /* object */, const char* name = 0) throw())
  //BAUSTELLE
RESULT


CHECK(PersistenceManager::checkObjectHeader(const char* type_name) throw())
  //BAUSTELLE
RESULT


CHECK(PersistenceManager::writeObjectTrailer(const char* name = 0) throw())
  //BAUSTELLE
RESULT


CHECK(PersistenceManager::writePrimitive(const T& t, const char* name) throw())
  //BAUSTELLE
RESULT


CHECK(PersistenceManager::readPrimitive(T& t, const char* name) throw())
  //BAUSTELLE
RESULT


CHECK(PersistenceManager::readStorableObject(T& t, const char* name) throw())
  //BAUSTELLE
RESULT


CHECK(PersistenceManager::readObjectPointer(T*& object, const char* name) throw())
  //BAUSTELLE
RESULT


CHECK(PersistenceManager::readObjectReference(T& object, const char* name) throw())
  //BAUSTELLE
RESULT


CHECK(PersistenceManager::writeObjectArray(const T* array, const char* name, Size size) throw())
  //BAUSTELLE
RESULT


CHECK(PersistenceManager::readObjectArray(const T* array, const char* name, Size& size) throw())
  //BAUSTELLE
RESULT


CHECK(PersistenceManager::writeObjectPointerArray(T** arr, const char* name, const Size size) throw())
  //BAUSTELLE
RESULT


CHECK(PersistenceManager::readObjectPointerArray(T** array, const char* name, Size& size) throw())
  //BAUSTELLE
RESULT


/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
