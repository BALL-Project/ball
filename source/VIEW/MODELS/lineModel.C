// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: lineModel.C,v 1.2 2003/08/26 18:35:37 amoll Exp $

#include <BALL/VIEW/MODELS/lineModel.h>
#include <BALL/KERNEL/atom.h>
#include <BALL/VIEW/MODELS/colorProcessor.h>
#include <BALL/VIEW/PRIMITIVES/point.h>

using namespace std;

namespace BALL
{
	using VIEW::Point;

	namespace VIEW
	{


AddLineModel::AddLineModel()
	throw()
	: AtomBondModelBaseProcessor()
{
}

AddLineModel::AddLineModel(const AddLineModel& rAddLineModel)
	throw()
	: AtomBondModelBaseProcessor(rAddLineModel)
{
}

AddLineModel::~AddLineModel()
	throw()
{
	#ifdef BALL_VIEW_DEBUG
		Log.error() << "Destructing object " << (void *)this 
							  << " of class " << RTTI::getName<AddLineModel>() << std::endl;
	#endif 
}

bool AddLineModel::start()
{
	// init model connector
	//getModelConnector()->setProperties(*this);

	return AtomBondModelBaseProcessor::start();
}
		
bool AddLineModel::finish()
{
	buildBondModels_();
	return true;
}
		
Processor::Result AddLineModel::operator() (Composite &composite)
{
	// composite is an atom ?
	if (!RTTI::isKindOf<Atom>(composite))
	{
		return Processor::CONTINUE;
	}

	Atom *atom = RTTI::castTo<Atom>(composite);

	// generate help BallPrimitive
	Point *point_ptr = new Point;

	if (point_ptr == 0) throw Exception::OutOfMemory(__FILE__, __LINE__, sizeof(Point));

	point_ptr->setVertex(atom->getPosition());

	point_ptr->setComposite(atom);
	
	getColorProcessor()->operator() (atom);
	point_ptr->setColor(getColorProcessor()->getColor());

	// append line in Atom
	geometric_objects_.push_back(point_ptr);

	// collect used atoms
	insertAtom_(atom);

	return Processor::CONTINUE;
}

void AddLineModel::dump(ostream& s, Size depth) const
	throw()
{
	BALL_DUMP_STREAM_PREFIX(s);
	
	BALL_DUMP_DEPTH(s, depth);
	BALL_DUMP_HEADER(s, this, this);

	AtomBondModelBaseProcessor::dump(s, depth + 1);

	BALL_DUMP_STREAM_SUFFIX(s);
}

	} // namespace VIEW
} // namespace BALL
