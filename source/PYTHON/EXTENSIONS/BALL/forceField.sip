// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id$
//

class ForceField
{
%TypeHeaderCode
	#include <BALL/MOLMEC/COMMON/forceField.h>
	#include <BALL/COMMON/exception.h>
	using namespace BALL;
	using namespace BALL::Exception;
%End
	public:
	//typedef vector<pair<Atom*, Atom*> > PairVector;

	ForceField();
	ForceField(System&);
	ForceField(System&, Options&);
	ForceField(const ForceField&);

	// ForceField& operator = (const ForceField& force_field);
	bool isValid();
	bool setup(System&) throw(FileNotFound);
	bool setup(System&, Options&) throw(FileNotFound);
	virtual bool specificSetup() throw(TooManyErrors);
	void setName(const String&);
	String getName() const;
	int getNumberOfAtoms() const;
	int getNumberOfMovableAtoms() const;
	const AtomVector& getAtoms() const;
	System*  getSystem();
	bool getUseSelection();
	void disableSelection() throw();
	void enableSelection() throw();
	// ForceFieldParameters& getParameters();
	int countComponents() const;
	void insertComponent(ForceFieldComponent*);
	void removeComponent(const ForceFieldComponent*);
	void removeComponent(const String&);
	ForceFieldComponent* getComponent(int) const;
	ForceFieldComponent* getComponent(String&) const;
	double getEnergy() const;
	double updateEnergy();
	void  updateForces();
	double getRMSGradient() const;
	int getUpdateFrequency() const;
	void update() throw (TooManyErrors);
	Options options;
	PeriodicBoundary periodic_boundary;
};
