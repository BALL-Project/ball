class AddBallAndStickModel
	: AtomBondModelBaseProcessor
{
%TypeHeaderCode
	#include <BALL/VIEW/MODELS/ballAndStickModel.h>
%End
	public:

	AddBallAndStickModel() throw();
	AddBallAndStickModel(const AddBallAndStickModel&) throw();
	~AddBallAndStickModel() throw();
	virtual void clear() throw();
	void set(const AddBallAndStickModel&) throw();
	void setBallRadius(const float);// throw(Exception::OutOfRange);
	float getBallRadius() const throw();
	void setStickRadius(const float);// throw(Exception::OutOfRange);
	float getStickRadius() const throw();
	void enableBallAndStickModel() throw();
	void enableStickModel() throw();
	bool isBallAndStickModel() const throw();
	bool isStickModel() const throw();
	bool dashedBondsEnabled() const throw();
	void enableDashedBonds(bool) throw();
};
