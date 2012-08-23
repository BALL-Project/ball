// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id$
//

class MolecularDynamics
{
%TypeHeaderCode
  #include <BALL/MOLMEC/MDSIMULATION/molecularDynamics.h>
  using namespace BALL;
%End                                                                                                                                                            
	public:
  //struct Option
  //{
  //  static const char *MAXIMAL_NUMBER_OF_ITERATIONS;
  //  static const char *MAXIMAL_SIMULATION_TIME; 
  //  static const char *NUMBER_OF_ITERATION;
  //  static const char *ENERGY_OUTPUT_FREQUENCY;
  //  static const char *SNAPSHOT_FREQUENCY;
  //  static const char *TIME_STEP;
  //  static const char *REFERENCE_TEMPERATURE;
  //  static const char *BATH_RELAXATION_TIME;
 //  static const char *CURRENT_TIME;
  // };
  // struct Default
  // {
  //  static const int MAXIMAL_NUMBER_OF_ITERATIONS;
  //  static const double  MAXIMAL_SIMULATION_TIME; 
  //  static const int NUMBER_OF_ITERATION;
  //  static const int ENERGY_OUTPUT_FREQUENCY;
  //  static const int SNAPSHOT_FREQUENCY;
  //  static const double TIME_STEP;
  //  static const double REFERENCE_TEMPERATURE;
  //  static const double BATH_RELAXATION_TIME;
  // static const double CURRENT_TIME;
  // };
  // BALL_CREATE (MolecularDynamics)
  MolecularDynamics ();
  MolecularDynamics (ForceField&);
  MolecularDynamics (const MolecularDynamics&);
  ~MolecularDynamics ();
  // const MolecularDynamics& operator = (const MolecularDynamics&);
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
  
