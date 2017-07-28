class AtomVector
{
%TypeHeaderCode
	#include <BALL/MOLMEC/COMMON/atomVector.h>
%End

	public:
	AtomVector();
	AtomVector(Composite&, bool);
	AtomVector(AtomVector&, bool);

	void clear();
	void set(const AtomVector&);
	void set(const Composite&, bool);
	int size() const;
	void savePositions();
	void resetPositions();
	void moveTo(const Gradient&, float);
	void push_back(Atom*);
	void resize(int);
};
