// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

class AtomHashGrid3BoxIterator
{
%TypeHeaderCode
	#include <BALL/DATATYPE/hashGrid.h>
	#include <BALL/KERNEL/atom.h>

	typedef HashGridBox3<Atom*> AtomHashGridBox3;
	typedef HashGridBox3<Atom*>::BoxIterator AtomHashGrid3BoxIterator;
%End
	AtomHashGrid3BoxIterator& __iter__();
%MethodCode
	sipRes = sipCpp;
%End

	AtomHashGridBox3& __next__();
%MethodCode
	if(sipCpp->isEnd())
	{
		PyErr_SetNone(PyExc_StopIteration);
		return NULL;
	}

	sipRes = &**sipCpp;

	++*sipCpp;
%End
};

class AtomHashGrid3BoxDataIterator
{
%TypeHeaderCode
	#include <BALL/DATATYPE/hashGrid.h>
	#include <BALL/KERNEL/atom.h>
	typedef HashGridBox3<Atom*> AtomHashGridBox3;
	typedef HashGridBox3<Atom*>::DataIterator AtomHashGrid3BoxDataIterator;
%End
	AtomHashGrid3BoxDataIterator& __iter__();
%MethodCode
	sipRes = sipCpp;
%End

	Atom& __next__();
%MethodCode
	if(sipCpp->isEnd())
	{
		PyErr_SetNone(PyExc_StopIteration);
		return NULL;
	}

	sipRes = **sipCpp;

	++*sipCpp;
%End
};

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

	AtomHashGridBox3(AtomHashGrid3* parent);
	~AtomHashGridBox3();
	void clear();
	void destroy();

	//Item* find(const Item &/item/);
	//const Item* find(const Item&) const;

	Size getSize() const;
	void insert(Atom*);
	bool remove(Atom*);
	bool removeAll(Atom*);
		
	//void host(Visitor<HashGridBox3> &visitor);

	bool operator == (const AtomHashGridBox3) const;
	bool operator != (const AtomHashGridBox3) const;
	bool has(Atom*) const;

	bool isEmpty() const;
	bool isValid() const;

	AtomHashGrid3BoxIterator iterboxes();
%MethodCode
	return BALL_CONVERT_FROM_INSTANCE(new AtomHashGrid3BoxIterator(sipCpp->beginBox()), AtomHashGrid3BoxIterator, Py_None);
%End

	AtomHashGrid3BoxDataIterator iterdata();
%MethodCode
	return BALL_CONVERT_FROM_INSTANCE(new AtomHashGrid3BoxDataIterator(sipCpp->beginData()), AtomHashGrid3BoxDataIterator, Py_None);
%End

//	void dump(ostream, Size) const;

	//bool apply(UnaryProcessor<Item> processor);
	//bool apply(UnaryProcessor< HashGridBox3<Item> > processor);
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
		
	AtomHashGrid3(const Vector3, Size, Size,
			Size, float, float, float);

	AtomHashGrid3(const Vector3, Size, Size, 
			Size, float);

	AtomHashGrid3(const Vector3, const Vector3, float);

	AtomHashGrid3(const AtomHashGrid3, bool);

	virtual void clear();
	void clear(Position, Position, Position);
	void clear(const Vector3);
	void destroy();

	void destroy(Position, Position, Position);
	void destroy(const Vector3);

	void set(const Vector3, const Vector3, 
			Size, Size, Size);

	void set(const Vector3, float, Size);
	void set(const AtomHashGrid3, bool);
	//const AtomHashGrid3& operator = (const AtomHashGrid3);

	void get(Vector3, Vector3, Size, Size, Size) const; 
	void get(AtomHashGrid3, bool) const;

	Size countNonEmptyBoxes() const;
	Size getSize() const;
	Vector3& getOrigin();
	Vector3& getUnit();

	Size getSizeX() const;
	Size getSizeY() const;
	Size getSizeZ() const;

	AtomHashGridBox3* getBox(Position, Position, Position);
	AtomHashGridBox3* getBox(const Vector3);

	bool getIndices(const AtomHashGridBox3, Position& /Out/, Position& /Out/, Position& /Out/) const;

	void insert(Position, Position, Position, Atom*);
	void insert(const Vector3, Atom*);

	bool remove(Position, Position, Position, Atom*);
	bool remove(const Vector3, Atom*);

	//void host(Visitor<HashGrid3>& visitor);

	bool operator == (const AtomHashGrid3) const;
	bool operator != (const AtomHashGrid3) const;

	bool isEmpty() const;

	virtual bool isValid() const;

//	virtual void dump(ostream, Size) const;

	//bool apply(UnaryProcessor<Item> processor);
	//bool apply(UnaryProcessor< HashGridBox3<Item> > processor);

	const Atom& getClosestItem(const Vector3& /In/, Size) const;
%MethodCode
	Atom* const* atom_ptr_ptr = sipCpp->getClosestItem(*a0, a1);
	sipRes = &(**atom_ptr_ptr);
%End

	static float calculateMinSpacing(LongIndex, const Vector3);
};
