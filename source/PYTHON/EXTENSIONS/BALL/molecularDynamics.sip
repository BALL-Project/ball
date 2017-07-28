class MolecularDynamics
{
%TypeHeaderCode
	#include <BALL/MOLMEC/MDSIMULATION/molecularDynamics.h>
%End
	public:
	MolecularDynamics ();
	MolecularDynamics (ForceField&);
	MolecularDynamics (const MolecularDynamics&);
	~MolecularDynamics ();
	void set(const MolecularDynamics&);
	bool isValid () const;
	virtual bool setup (ForceField &, SnapShotManager*);
	virtual bool setup (ForceField &, SnapShotManager*, const Options &);
	virtual bool specificSetup();
	void setNumberOfIteration (Size);
	void setMaximalNumberOfIterations (Size);
	void setMaximalSimulationTime(double);
	virtual void setTimeStep(double);
	void setReferenceTemperature (double);
	void setCurrentTime (double);
	void setEnergyOutputFrequency (Size);
	void setSnapShotFrequency (Size);
	Size getEnergyOutputFrequency () const;
	Size getNumberOfIterations () const;
	Size getMaximalNumberOfIterations () const;
	double getMaximalSimulationTime () const;
	double getTimeStep () const;
	Size getSnapShotFrequency () const;
	double getTemperature () const;
	double getTime () const;
	double getTotalEnergy () const;
	double getPotentialEnergy () const;
	double getKineticEnergy () const;
	ForceField *getForceField () const;
	virtual bool simulate (bool restart = false);
	virtual bool simulateIterations (Size, bool restart = false);
	virtual bool simulateTime (double, bool restart = false);
	Options options;
};
