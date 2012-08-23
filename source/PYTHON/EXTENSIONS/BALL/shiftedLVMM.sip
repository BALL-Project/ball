// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id$
//

class ShiftedLVMMMinimizer 
	: EnergyMinimizer
{
%TypeHeaderCode
	#include <BALL/MOLMEC/MINIMIZATION/shiftedLVMM.h>
	using namespace BALL;
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
	//	struct Option 
	//	{
	//		static const char* UPDATE_METHOD;
	//		static const char* CORRECTION_PARAMETER;
	//		static const char* NUM_OF_COLUMNS;
	//	};
	// 	struct Default
	//	{
	//		static const bool UPDATE_METHOD;
	//		static const Size CORRECTION_PARAMETER;
	//		static const Size NUM_OF_COLUMNS;
	//	};
	//    BALL_CREATE(ShiftedLVMMMinimizer)
	ShiftedLVMMMinimizer();	
	ShiftedLVMMMinimizer(ForceField&);
	ShiftedLVMMMinimizer(ForceField&, SnapShotManager*);
	ShiftedLVMMMinimizer(ForceField&, SnapShotManager*, const Options&);
	ShiftedLVMMMinimizer(ForceField&, const Options&);
	~ShiftedLVMMMinimizer();
	// ShiftedLVMMMinimizer& operator = (const ShiftedLVMMMinimizer&);
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
