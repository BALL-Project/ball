// $Id: lineModel.C,v 1.11 2001/07/15 18:50:28 oliver Exp $

#include <BALL/MOLVIEW/FUNCTOR/lineModel.h>

using namespace std;

namespace BALL
{
	namespace MOLVIEW
	{

		AddLineModel::AddLineModel()
			throw()
			: AtomBondModelBaseProcessor()
		{
		}

		AddLineModel::AddLineModel(const AddLineModel& rAddLineModel, bool deep)
			throw()
			: AtomBondModelBaseProcessor(rAddLineModel, deep)
		{
		}

		AddLineModel::~AddLineModel()
			throw()
		{
			#ifdef BALL_VIEW_DEBUG
				cout << "Destructing object " << (void *)this 
						 << " of class " << RTTI::getName<AddLineModel>() << endl;
			#endif 

			destroy();
		}

		void AddLineModel::clear()
			throw()
		{
			AtomBondModelBaseProcessor::clear();
		}

		void AddLineModel::destroy()
			throw()
		{
		}

		bool AddLineModel::start()
		{
			if (hasProperty(GeometricObject::PROPERTY__DRAWING_MODE_SOLID))
			{
				setProperty(GeometricObject::PROPERTY__DRAWING_MODE_WIREFRAME);
			}

			// init model connector
			getModelConnector()->setProperties(*this);

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

			// remove all models appended to atom
			removeGeometricObjects_(*atom, true);

			// generate help BallPrimitive
			Point *point_ptr = createPoint_();

			if (point_ptr == 0)
			{
					throw Exception::OutOfMemory
						(__FILE__, __LINE__, sizeof(Point));
			}

			// carry on selected flag
			point_ptr->Selectable::set(*atom);

			point_ptr->PropertyManager::set(*this);
			point_ptr->PropertyManager::setProperty(PROPERTY__MODEL_LINES);
			point_ptr->setVertexAddress(atom->getPosition());
			
			atom->host(*getColorCalculator());

			point_ptr->setColor(getColorCalculator()->getColor());

			// append line in Atom
			composite.appendChild(*point_ptr);

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

		Point* AddLineModel::createPoint_()
		{
			return (Point *)(new Point());
		}

		Line* AddLineModel::createLine_()
		{
			return (Line *)(new Line());
		}

		TwoColoredLine* AddLineModel::createTwoColoredLine_()
		{
			return (TwoColoredLine *)(new TwoColoredLine());
		}


#		ifdef BALL_NO_INLINE_FUNCTIONS
#			include <BALL/MOLVIEW/FUNCTOR/lineModel.iC>
#		endif

	} // namespace MOLVIEW

} // namespace BALL
