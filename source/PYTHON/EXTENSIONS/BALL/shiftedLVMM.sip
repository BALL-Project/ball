class ShiftedLVMMMinimizer
	: EnergyMinimizer
{
%TypeHeaderCode
	#include <BALL/MOLMEC/MINIMIZATION/shiftedLVMM.h>
%End
	public:

	enum UpdateMethod
	{
		RANK_1,
		RANK_2
	};

	enum CorrectionParameter
	{
		UNIT_VALUE,
		BALANCING_VALUE,
		SQUARE_ROOT,
		GEOMETRIC_MEAN,
		RATIO_OF_SHIFT_PARAMS
	};

	ShiftedLVMMMinimizer();
	ShiftedLVMMMinimizer(ForceField&);
	ShiftedLVMMMinimizer(ForceField&, SnapShotManager*);
	ShiftedLVMMMinimizer(ForceField&, SnapShotManager*, const Options&);
	ShiftedLVMMMinimizer(ForceField&, const Options&);
	~ShiftedLVMMMinimizer();

	virtual bool specificSetup();
	void setUpdateMethod(UpdateMethod);
	UpdateMethod getUpdateMethod() const;
	void setCorrectionParameter(CorrectionParameter);
	CorrectionParameter getCorrectionParameter() const;
	void setMaxNumOfColumns(Size);
	Size getMaxNumOfColumns() const;
	virtual double findStep();
	virtual void updateDirection();
	virtual bool minimize(Size iterations = 0, bool restart = false);
};
