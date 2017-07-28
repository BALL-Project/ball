class SnapShot
{
%TypeHeaderCode
	#include <BALL/MOLMEC/COMMON/snapShot.h>
%End
	public:
	SnapShot() throw();
	SnapShot(const SnapShot&) throw();
	~SnapShot() throw();

	virtual void clear() throw();
	bool operator == (const SnapShot&) const throw();
	bool isValid() const throw();
	void setIndex(Size) throw();
	Size getIndex() const throw();
	void setNumberOfAtoms(Size) throw();
	Size getNumberOfAtoms() const throw();
	void setPotentialEnergy(DoubleReal)  throw();
	DoubleReal getPotentialEnergy() const throw();
	void setKineticEnergy(DoubleReal) throw();
	DoubleReal getKineticEnergy() const throw();
	void takeSnapShot(const System&) throw(OutOfMemory);
	void applySnapShot(System&) const throw();
	void getAtomPositions(const System&) throw(OutOfMemory);
	void setAtomPositions(System&) const throw();
	void getAtomVelocities(const System&) throw(OutOfMemory);
	void setAtomVelocitites(System&) const throw();
	void getAtomForces(const System&) throw(OutOfMemory);
	void setAtomForces(System&) const throw();
};
