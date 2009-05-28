// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id$
//

class AddBallAndStickModel  
	: AtomBondModelBaseProcessor
{
%TypeHeaderCode
	#include <BALL/VIEW/MODELS/ballAndStickModel.h>
	using namespace BALL;
	using namespace BALL::VIEW;
%End
  public:

  AddBallAndStickModel() throw();
  AddBallAndStickModel(const AddBallAndStickModel&) throw();
  ~AddBallAndStickModel() throw();
  virtual void clear() throw();
  void set(const AddBallAndStickModel&) throw();
	//const AddBallAndStickModel& operator = (const AddBallAndStickModel& add_ball_and_stick_model) throw();
  void setBallRadius(const float);// throw(Exception::OutOfRange);
  float getBallRadius() const throw();
  void setStickRadius(const float);// throw(Exception::OutOfRange);
  float getStickRadius() const throw();
  void enableBallAndStickModel() throw();
  void enableStickModel() throw();
  //Processor::Result operator() (Composite&);
  bool isBallAndStickModel() const throw();
  bool isStickModel() const throw();
	bool dashedBondsEnabled() const throw();
	void enableDashedBonds(bool) throw();
	//void dump(std::ostream& s = std::cout, Size depth = 0) const throw();
};
