// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

class AtomHashGridBox3
{
%TypeHeaderCode
	#include <BALL/DATATYPE/hashGrid.h>
	#include <BALL/KERNEL/atom.h>
	#include <BALL/COMMON/exception.h>

	#include <iostream>

	using namespace BALL;
	using namespace BALL::Exception;
	using namespace std;
	typedef HashGridBox3<Atom*> AtomHashGridBox3;
%End
	public:

	AtomHashGridBox3();
	AtomHashGridBox3(const AtomHashGridBox3 /grid_box/, bool /deep/);
	~AtomHashGridBox3();
	void clear();
	void destroy();

	void set(const AtomHashGridBox3 /box/, bool / deep /)
		throw(NotImplemented);

//	const AtomHashGridBox3& operator = (const AtomHashGridBox3& /box/)
//		throw(NotImplemented);

	//Item* find(const Item &/item/);
	//const Item* find(const Item& /item/) const;

	Size getSize() const;
	void insert(Atom* /item/);
	bool remove(Atom* /item/);
	bool removeAll(Atom* /item/);
		
	//void host(Visitor<HashGridBox3> &visitor);

	bool operator == (const AtomHashGridBox3 /box/) const;
	bool operator != (const AtomHashGridBox3 /box/) const;
	bool has(Atom* /item/) const;

	bool isEmpty() const;
	bool isValid() const;
//	void dump(ostream /s/, Size /depth/) const;

	//bool apply(UnaryProcessor<Item> processor);
	//bool apply(UnaryProcessor< HashGridBox3<Item> > processor);

	deque<Atom*> data;
};

class AtomHashGrid3
{
%TypeHeaderCode
	#include <BALL/DATATYPE/hashGrid.h>
	#include <BALL/KERNEL/atom.h>

	#include <iostream>

	using namespace BALL;
	typedef HashGrid3<Atom*> AtomHashGrid3;
%End
	public:

//	BALL_CREATE(HashGrid3)
	AtomHashGrid3();
		
	AtomHashGrid3(const Vector3 /origin/, Size /dimension_x/, Size /dimension_y/,
			Size /dimension_z/, float /spacing_x/, float /spacing_y/, float /spacing_z/);

	AtomHashGrid3(const Vector3 /origin/, Size /dimension_x/, Size /dimension_y/, 
			Size /dimension_z/, float /spacing/);

	AtomHashGrid3(const Vector3 /origin/, const Vector3 /size/, float /spacing/);

	AtomHashGrid3(const AtomHashGrid3 /grid/, bool /deep/);

	virtual void clear();
	void clear(Position /x/, Position /y/, Position /z/);
	void clear(const Vector3 /vector/);
	void destroy();

	void destroy(Position /x/, Position /y/, Position /z/);
	void destroy(const Vector3 /vector/);

	void set(const Vector3 /origin/, const Vector3 /unit/, 
			Size /dimension_x/, Size	/dimension_y/, Size	/dimension_z/);

	void set(const Vector3 /origin/, float /unit/, Size /size/);
	void set(const AtomHashGrid3 /grid/, bool /deep/);
	//const AtomHashGrid3& operator = (const AtomHashGrid3 /grid/);

	void get(Vector3 /origin/, Vector3 /unit/, Size	/dimension_x/, Size	/dimension_y/, Size	/dimension_z/) const; 
	void get(AtomHashGrid3 /grid/, bool /deep/) const;

	Size countNonEmptyBoxes() const;
	Size getSize() const;
	Vector3& getOrigin();
	Vector3& getUnit();

	Size getSizeX() const;
	Size getSizeY() const;
	Size getSizeZ() const;

	AtomHashGridBox3* getBox(Position /x/, Position /y/, Position /z/);
	AtomHashGridBox3* getBox(const Vector3 /vector/);

	bool getIndices(const AtomHashGridBox3, Position& /Out/, Position& /Out/, Position& /Out/) const;

	void insert(Position /x/, Position /y/, Position /z/, Atom* /item/);
	void insert(const Vector3 /vector/, Atom* /item/);

	bool remove(Position /x/, Position /y/, Position /z/, Atom* /item/);
	bool remove(const Vector3 /vector/, Atom* /item/);

	//void host(Visitor<HashGrid3>& visitor);

	bool operator == (const AtomHashGrid3 /grid/) const;
	bool operator != (const AtomHashGrid3 /grid/) const;

	bool isEmpty() const;

	virtual bool isValid() const;

//	virtual void dump(ostream /s/, Size /depth/) const;

	//bool apply(UnaryProcessor<Item> processor);
	//bool apply(UnaryProcessor< HashGridBox3<Item> > processor);

	const Atom& getClosestItem(const Vector3& /In/, Size) const;
%MethodCode
	Atom* const* atom_ptr_ptr = sipCpp->getClosestItem(*a0, a1);
	sipRes = &(**atom_ptr_ptr);
%End

	static float calculateMinSpacing(LongIndex /memory/, const Vector3 /size/);
};
