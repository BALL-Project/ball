//// -*- Mode: C++; tab-width: 2; -*-
//// vi: set ts=2:
////

//#ifndef LIGANDSTRUCTUREBASE_H
//#define LIGANDSTRUCTUREBASE_H


///// BALL: File IO
//#include <BALL/FORMAT/SDFile.h>
//#include <BALL/FORMAT/lineBasedFile.h>
//#include <BALL/SYSTEM/file.h>

///// BALL: Atom and Bond
//#include <BALL/KERNEL/atom.h>
//#include <BALL/KERNEL/atomIterator.h>
//#include <BALL/KERNEL/bond.h>
//#include <BALL/KERNEL/bondIterator.h>
//#include <BALL/KERNEL/PTE.h>

///// BALL: Molecular Conatiners
//#include <BALL/KERNEL/atomContainer.h>
//#include <BALL/KERNEL/fragment.h>
//#include <BALL/KERNEL/molecule.h>

///// BALL: Geometric base container
//#include <BALL/MATHS/angle.h>
//#include <BALL/MATHS/vector3.h>
//#include <BALL/MATHS/matrix44.h>


//#include <vector>

//using namespace OpenBabel;
//using namespace BALL;
//using namespace std;

///**
// * @brief The TemplateCoord class is a simple array wrapper for BALL::Vector3
// * 
// * To efficiently save coordinates without much overhead
// */
//class TemplateCoord
//{
	
//public:
//	TemplateCoord(Size n)
//	{
//		_size = n;
//		positions = new Vector3[n];
//	}
	
//	~TemplateCoord()
//	{
//		delete[] positions;
//	}
	
//	Vector3& operator[](Index idx)
//	{
//		return positions[idx];
//	}
	
//	Vector3& get(Index idx)
//	{
//		return positions[idx];
//	}

//	const Size& size()
//	{
//		return _size;
//	}

//// TODO: optimize the data strcuture, implement initialzier which sets map
////       and vector allocations to reasonable initial values
//struct GroupFragment
//{
//	// intra connections
//	vector< Fragment* > linker_lst; // all bonds within a linker are rotable
//	vector< Fragment* > rigid_lst; // all matchable fragments
//	list< Bond* > rotor_lst;      // all intra fragment bonds of the molecule
//	list< pair< Atom*, Atom*> > frag_con2;
//	// for each fragment, the list of bonds to other fragments
//	boost::unordered_map< Fragment*, list<Bond*> > fragment_connections;
	
////	// alternative connections per fragment mapping (perhaps faster and more memory efficient than map?)
////	vector< Fragment* > fragment_idx;
////	vector< list<Bond*> > fragment_connections;
	
//	// inter connections
//	list< pair< unsigned int, Atom*> > connections;
	
//	// data:
//	Molecule* molecule;
//};






///// Typedefs:

//// Standard Data:
//typedef vector<Atom*> AtmVec;
//typedef vector<Atom*>::iterator AVIter;

//typedef vector<GroupFragment*> GroupFragmentList;
//typedef boost::unordered_map< int, GroupFragmentList* > CombiLib;

//// Lib Data:
//typedef boost::unordered_map <String, TemplateCoord*> CoordinateMap;// String == UCK key
//typedef boost::unordered_map <String, float >         BondLengthMap;
//typedef boost::unordered_map <String, Fragment* >     ConnectionMap;

//#endif // LIGANDSTRUCTUREBASE_H
