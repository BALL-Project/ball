class AtomHashGrid3BoxIterator
{
%TypeHeaderCode
	#include <BALL/DATATYPE/hashGrid.h>
	#include <BALL/KERNEL/atom.h>

	using AtomHashGridBox3 = HashGridBox3<Atom*>;
	using AtomHashGrid3BoxIterator = HashGridBox3<Atom*>::BoxIterator;
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
	using AtomHashGridBox3 = HashGridBox3<Atom*>;
	using AtomHashGrid3BoxDataIterator = HashGridBox3<Atom*>::DataIterator;
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
	using AtomHashGridBox3 = HashGridBox3<Atom*>;
%End
	public:

	AtomHashGridBox3(AtomHashGrid3* parent);
	~AtomHashGridBox3();
	void clear();
	void destroy();

	Size getSize() const;
	void insert(Atom*);
	bool remove(Atom*);
	bool removeAll(Atom*);

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
};

class AtomHashGrid3
{
%TypeHeaderCode
	#include <BALL/DATATYPE/hashGrid.h>
	#include <BALL/KERNEL/atom.h>

	using AtomHashGrid3 = HashGrid3<Atom*>;
%End
	public:

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

	bool operator == (const AtomHashGrid3) const;
	bool operator != (const AtomHashGrid3) const;

	bool isEmpty() const;

	virtual bool isValid() const;

	const Atom& getClosestItem(const Vector3& /In/, Size) const;
%MethodCode
	Atom* const* atom_ptr_ptr = sipCpp->getClosestItem(*a0, a1);
	sipRes = &(**atom_ptr_ptr);
%End

	static float calculateMinSpacing(LongIndex, const Vector3);
};
