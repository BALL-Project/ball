// $Id: lineModel.C,v 1.10 2001/05/13 15:02:39 hekl Exp $

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
			throw()
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
			throw()
		{
			buildBondModels_();

			return true;
		}
				
		Processor::Result AddLineModel::operator() (Composite &composite)
			throw(Exception::OutOfMemory)
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
			Point *pPoint = createPoint_();

			if (pPoint == 0)
			{
					throw Exception::OutOfMemory
						(__FILE__, __LINE__, sizeof(Point));
			}

			// carry on selected flag
			pPoint->Selectable::set(*atom);

			pPoint->PropertyManager::set(*this);
			pPoint->PropertyManager::setProperty(PROPERTY__MODEL_LINES);
			pPoint->setVertexAddress(atom->getPosition());
			
			atom->host(*getColorCalculator());

			pPoint->setColor(getColorCalculator()->getColor());

			// append line in Atom
			composite.appendChild(*pPoint);

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
