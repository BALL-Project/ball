// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: ballAndStickModel.C,v 1.11 2004/06/07 10:16:58 amoll Exp $

#include <BALL/VIEW/MODELS/ballAndStickModel.h>
#include <BALL/KERNEL/atom.h>
#include <BALL/KERNEL/bond.h>
#include <BALL/VIEW/PRIMITIVES/tube.h>
#include <BALL/VIEW/PRIMITIVES/sphere.h>
#include <BALL/VIEW/PRIMITIVES/twoColoredTube.h>

using namespace std;

namespace BALL
{
	namespace VIEW
	{

AddBallAndStickModel::AddBallAndStickModel()
	throw()
	: AtomBondModelBaseProcessor(),
		ball_radius_((float)0.4),
		stick_radius_((float)0.2),
		ball_and_stick_(true)
{
}

AddBallAndStickModel::AddBallAndStickModel(const AddBallAndStickModel &add_ball_and_stick)
	throw()
	: AtomBondModelBaseProcessor(add_ball_and_stick),
		ball_radius_(add_ball_and_stick.ball_radius_),
		stick_radius_(add_ball_and_stick.stick_radius_),
		ball_and_stick_(add_ball_and_stick.ball_and_stick_)
{
}

AddBallAndStickModel::~AddBallAndStickModel()
	throw()
{
	#ifdef BALL_VIEW_DEBUG
		Log.info() << "Destructing object " << (void *)this 
							 << " of class " << RTTI::getName<AddBallAndStickModel>() << std::endl;
	#endif 
}

void AddBallAndStickModel::clear()
	throw()
{
	AtomBondModelBaseProcessor::clear();

	ball_radius_ = (float)0.4;
	stick_radius_ = (float)0.2;
	ball_and_stick_ = true;
}

void AddBallAndStickModel::set(const AddBallAndStickModel &add_ball_and_stick)
	throw()
{
	AtomBondModelBaseProcessor::set(add_ball_and_stick);

	ball_radius_ = add_ball_and_stick.ball_radius_;
	stick_radius_ = add_ball_and_stick.stick_radius_;
	ball_and_stick_ = add_ball_and_stick.ball_and_stick_;
}

const AddBallAndStickModel &AddBallAndStickModel::operator = 
	(const AddBallAndStickModel &add_ball_and_stick)
	throw()
{
	set(add_ball_and_stick);
	return *this;
}

void AddBallAndStickModel::swap(AddBallAndStickModel &add_ball_and_stick)
	throw()
{
	AtomBondModelBaseProcessor::swap(add_ball_and_stick);

	float temp_float = ball_radius_;
	ball_radius_ = add_ball_and_stick.ball_radius_;
	add_ball_and_stick.ball_radius_ = temp_float;

	temp_float = stick_radius_;
	stick_radius_ = add_ball_and_stick.stick_radius_;
	add_ball_and_stick.stick_radius_ = temp_float;

	bool temp_bool = ball_and_stick_;
	ball_and_stick_ = add_ball_and_stick.ball_and_stick_;
	add_ball_and_stick.ball_and_stick_ = temp_bool;
}

void AddBallAndStickModel::setBallRadius(const float radius)
	throw(Exception::OutOfRange)
{
	// a radius never can be lower or equal 0
	if (radius <= (float)0)
	{
		throw Exception::OutOfRange(__FILE__, __LINE__);
	}
	
	ball_radius_ = radius;
}

void AddBallAndStickModel::setStickRadius(const float radius)
	throw(Exception::OutOfRange)
{
	// a radius never can be lower or equal 0
	if (radius <= (float)0)
	{
		throw Exception::OutOfRange(__FILE__, __LINE__);
	}

	stick_radius_ = radius;
}

Processor::Result AddBallAndStickModel::operator() (Composite& composite)
{
	if (!RTTI::isKindOf<Atom>(composite))
	{
		return Processor::CONTINUE;
	}

	Atom *atom = RTTI::castTo<Atom>(composite);

	Sphere* sphere_ptr = new Sphere;

	if (sphere_ptr == 0) throw Exception::OutOfMemory (__FILE__, __LINE__, sizeof(Sphere));

	sphere_ptr->setComposite(atom);

	if (ball_and_stick_)
	{
		sphere_ptr->setRadius(ball_radius_);
	}
	else
	{
		sphere_ptr->setRadius(stick_radius_);
	}

	sphere_ptr->setPositionAddress(atom->getPosition());
	
	// append sphere in Atom
	geometric_objects_.push_back(sphere_ptr);

	// collect used atoms
	insertAtom_(atom);

	return Processor::CONTINUE;
}

void AddBallAndStickModel::dump(std::ostream& s, Size depth) const
	throw()
{
	BALL_DUMP_STREAM_PREFIX(s);
	
	BALL_DUMP_DEPTH(s, depth);
	BALL_DUMP_HEADER(s, this, this);

	AtomBondModelBaseProcessor::dump(s, depth + 1);

	BALL_DUMP_DEPTH(s, depth);
	s << "ball radius: " << ball_radius_ << endl;

	BALL_DUMP_DEPTH(s, depth);
	s << "stick radius: " << stick_radius_ << endl;

	BALL_DUMP_DEPTH(s, depth);
	s << "b&s model: " << ball_and_stick_ << endl;

	BALL_DUMP_DEPTH(s, depth);
	s << "s model: " << ball_and_stick_ << endl;

	BALL_DUMP_STREAM_SUFFIX(s);
}

void AddBallAndStickModel::visualiseBond_(const Bond& bond)
	throw()
{
	// no visualisation for hydrogen bonds
	if (bond.getType() == Bond::TYPE__HYDROGEN) return;
	
	// generate two colored tube
	TwoColoredTube *tube = new TwoColoredTube;
					
	if (tube == 0) throw Exception::OutOfMemory(__FILE__, __LINE__, sizeof(TwoColoredTube));
						
	tube->setRadius(stick_radius_);
	tube->setVertex1Address(bond.getFirstAtom()->getPosition());
	tube->setVertex2Address(bond.getSecondAtom()->getPosition());
	tube->setComposite(&bond);
	geometric_objects_.push_back(tube);
}

#	ifdef BALL_NO_INLINE_FUNCTIONS
#		include <BALL/VIEW/MODELS/ballAndStickModel.iC>
#	endif

} } // namespaces
