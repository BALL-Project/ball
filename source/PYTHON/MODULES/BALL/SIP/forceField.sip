class ForceField
{
%HeaderCode
	#include <BALL/MOLMEC/COMMON/forceField.h>
	using namespace BALL;
%End
	public:
	//typedef vector<pair<Atom*, Atom*> > PairVector;

	ForceField();
	ForceField(System&);
	ForceField(System&, Options&);
	ForceField(const ForceField&, bool);

	// ForceField& operator = (const ForceField& force_field);
	bool isValid();
	bool setup(System& /system/) throw(FileNotFound);
	bool setup(System& /system/, Options& /options/) throw(FileNotFound);
	virtual bool specificSetup();
	void setName(const String&);
	String getName() const;
	int getNumberOfAtoms() const;
	int getNumberOfMovableAtoms() const;
	const AtomVector& getAtoms() const;
	System*  getSystem();
	bool getUseSelection();
	void setUseSelection(bool);
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
	void update();
	Options options;
	PeriodicBoundary periodic_boundary;
};
