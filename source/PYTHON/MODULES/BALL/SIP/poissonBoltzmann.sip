// $Id: poissonBoltzmann.sip,v 1.1 2001/08/16 00:30:48 oliver Exp $


class FDPB
{
%HeaderCode
	#include <BALL/SOLVATION/poissonBoltzmann.h>
	using namespace BALL;
%End
	public:
	enum ErrorCode
	{
		ERROR__NONE = -1,
		ERROR__UNKNOWN = 0,
		ERROR__NOT_IMPLEMENTED = 1,
		ERROR__CANNOT_CREATE_ATOM_ARRAY,
		ERROR__CANNOT_CREATE_SAS_GRID,

		ERROR__CANNOT_CREATE_EPSILON_GRID,

		ERROR__CANNOT_CREATE_KAPPA_GRID,
		ERROR__CANNOT_CREATE_CHARGE_GRID,
		ERROR__CANNOT_CREATE_PHI_GRID,
		ERROR__SAS_GRID_REQUIRED,

		ERROR__EPSILON_GRID_REQUIRED,
		ERROR__ATOM_ARRAY_REQUIRED,

		ERROR__PHI_GRID_REQUIRED,
		ERROR__OUT_OF_MEMORY,

		ERROR__UNKNOWN_DIELECTRIC_SMOOTHING_METHOD,

		ERROR__UNKNOWN_CHARGE_DISTRIBUTION_METHOD,

		ERROR__UNKNOWN_BOUNDARY_CONDITION_TYPE,

		ERROR__NOT_A_VECTOR_IN_UPPER_LOWER,

		ERROR__ILLEGAL_VALUE_FOR_LOWER_UPPER,
		ERROR__SETUP_REQUIRED,
		NUMBER_OF_ERRORS
	};
	class Option
	{
%HeaderCode
	#include <BALL/SOLVATION/poissonBoltzmann.h>
	using namespace BALL;
%End
		public:
		static const String& VERBOSITY;

		static const String& PRINT_TIMING;
		static const String& SPACING;
		static const String& BORDER;
		static const String& IONIC_STRENGTH;
		static const String& SOLUTE_DC;
		static const String& SOLVENT_DC;
		static const String& PROBE_RADIUS;
		static const String& ION_RADIUS;
		static const String& TEMPERATURE;
		static const String& BOUNDARY;
		static const String& CHARGE_DISTRIBUTION;
		static const String& DIELECTRIC_SMOOTHING;
		static const String& OFFSET;
		static const String& RMS_CRITERION;
		static const String& MAX_CRITERION;
		static const String& CHECK_AFTER_ITERATIONS;
		static const String& MAX_ITERATIONS;
		static const String& LOWER;
		static const String& UPPER;
		static const String& BOUNDING_BOX_LOWER;
		static const String& BOUNDING_BOX_UPPER;
	};
	class Boundary
	{
%HeaderCode
	#include <BALL/SOLVATION/poissonBoltzmann.h>
	using namespace BALL;
%End
		public:
		static const String& ZERO;

		static const String& DEBYE;
		static const String& COULOMB;
		static const String& DIPOLE;
		static const String& FOCUSING;
	};
	class ChargeDistribution
	{
%HeaderCode
	#include <BALL/SOLVATION/poissonBoltzmann.h>
	using namespace BALL;
%End
		public:
		static const String& TRILINEAR;
		static const String& UNIFORM;
	};
	class DielectricSmoothing
	{
%HeaderCode
	#include <BALL/SOLVATION/poissonBoltzmann.h>
	using namespace BALL;
%End
		public:
		static const String& NONE;

		static const String& UNIFORM;

		static const String& HARMONIC;
	};
	class Default
	{
%HeaderCode
	#include <BALL/SOLVATION/poissonBoltzmann.h>
	using namespace BALL;
%End
		public:
		static const int VERBOSITY;
		static const bool PRINT_TIMING ;
		static const float SPACING;
		static const float BORDER;
		static const float IONIC_STRENGTH;
		static const float TEMPERATURE;
		static const float PROBE_RADIUS;
		static const float ION_RADIUS;
		static const String& BOUNDARY;
		static const String& CHARGE_DISTRIBUTION;
		static const String& DIELECTRIC_SMOOTHING;
		static const float SOLVENT_DC;
		static const float SOLUTE_DC;
		static const float RMS_CRITERION;
		static const float MAX_CRITERION;
		static const Index MAX_ITERATIONS;
		static const Index CHECK_AFTER_ITERATIONS;
	};

	class FastAtomStruct
	{
%HeaderCode
	#include <BALL/SOLVATION/poissonBoltzmann.h>
	using namespace BALL;
%End
		public:
		float q;
		float r;
		float x;
		float y;
		float z;
		Index index;
	};
	typedef struct FastAtomStruct FastAtom;
	public:
	FDPB();
	FDPB(const FDPB& /fdpb/);
	FDPB(System& /system/);
	FDPB(Options& /new_options/);

	FDPB(System& /system/, Options& /new_options/);
	~FDPB();
	void destroy();

	void destroyGrids();

	bool setup(System& /system/);

	bool setup(System& /system/, Options& /options/);

	bool setupEpsGrid(System& /system/);
	bool setupSASGrid(System& /system/);

	bool setupAtomArray(System& /system/);
	bool setupKappaGrid();
	bool setupQGrid();
	bool setupPhiGrid();
	bool setupBoundary();

	bool solve();

	double getEnergy() const;
	double getReactionFieldEnergy() const;
	double calculateReactionFieldEnergy() const;

	Size getNumberOfIterations() const;

	Index getErrorCode() const;
	static String getErrorMessage(Index /error_code/);

	Options options;
	Options results;

	//RegularData3D<Vector3>* eps_grid;
	RegularData3D* kappa_grid;

	RegularData3D* q_grid;

	RegularData3D* phi_grid;
	//TRegularData3D<char>* SAS_grid;
	//vector<FDPB::FastAtom>* atom_array;
};
