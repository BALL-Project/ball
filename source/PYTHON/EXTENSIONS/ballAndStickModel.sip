// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: ballAndStickModel.sip,v 1.3 2003/12/19 12:31:07 amoll Exp $
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
//  void dump(std::ostream& s = std::cout, Size depth = 0) const throw();
//  const AddBallAndStickModel& operator = (const AddBallAndStickModel& add_ball_and_stick_model) throw();
  AddBallAndStickModel() throw();
  AddBallAndStickModel(const AddBallAndStickModel&) throw();
  //Processor::Result operator() (Composite&);
  bool isBallAndStickModel() const throw();
  bool isStickModel() const throw();
  float getBallRadius() const throw();
  float getStickRadius() const throw();
  void clear() throw();
  void enableBallAndStickModel() throw();
  void enableStickModel() throw();
  void set(const AddBallAndStickModel&) throw();
  void setBallRadius(const float);// throw(Exception::OutOfRange);
  void setStickRadius(const float);// throw(Exception::OutOfRange);
  void swap(AddBallAndStickModel&) throw();
  ~AddBallAndStickModel() throw();
};
