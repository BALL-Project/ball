// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: lineModel.C,v 1.12 2005/12/23 17:03:34 amoll Exp $

#include <BALL/VIEW/MODELS/lineModel.h>
#include <BALL/KERNEL/atom.h>
#include <BALL/KERNEL/bond.h>
#include <BALL/VIEW/PRIMITIVES/point.h>
#include <BALL/VIEW/PRIMITIVES/twoColoredLine.h>

using namespace std;

namespace BALL
{
	namespace VIEW
	{

		AddLineModel::AddLineModel()
			: AtomBondModelBaseProcessor()
		{
		}

		AddLineModel::AddLineModel(const AddLineModel& model)
			: AtomBondModelBaseProcessor(model)
		{
		}

		AddLineModel::~AddLineModel()
		{
			#ifdef BALL_VIEW_DEBUG
				Log.error() << "Destructing object " << (void *)this 
										<< " of class " << RTTI::getName<AddLineModel>() << std::endl;
			#endif 
		}

		Processor::Result AddLineModel::operator() (Composite &composite)
		{
			if (!RTTI::isKindOf<Atom>(composite))
			{
				return Processor::CONTINUE;
			}

			Atom *atom = RTTI::castTo<Atom>(composite);

			Point *point_ptr = new Point;

			if (point_ptr == 0) throw Exception::OutOfMemory(__FILE__, __LINE__, sizeof(Point));

			point_ptr->setVertexAddress(atom->getPosition());
			point_ptr->setComposite(atom);

			// append line in Atom
			geometric_objects_.push_back(point_ptr);

			// collect used atoms
			insertAtom_(atom);

			return Processor::CONTINUE;
		}

		void AddLineModel::dump(ostream& s, Size depth) const
		{
			BALL_DUMP_STREAM_PREFIX(s);
			
			BALL_DUMP_DEPTH(s, depth);
			BALL_DUMP_HEADER(s, this, this);

			AtomBondModelBaseProcessor::dump(s, depth + 1);

			BALL_DUMP_STREAM_SUFFIX(s);
		}

		void AddLineModel::visualiseBond_(const Bond& bond)
		{
			if (bond.getType() == Bond::TYPE__HYDROGEN ||
			    bond.getFirstAtom() == 0 ||
					bond.getSecondAtom() == 0)
			{
				return;
			}

			// generate two colored tube
			TwoColoredLine *line = new TwoColoredLine;

			if (line == 0) return;
							
			line->setVertex1Address(bond.getFirstAtom()->getPosition());
			line->setVertex2Address(bond.getSecondAtom()->getPosition());
			line->setComposite(&bond);
			geometric_objects_.push_back(line);
		}

	} // namespace VIEW
} // namespace BALL
