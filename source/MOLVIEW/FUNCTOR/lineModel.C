// $Id: lineModel.C,v 1.9 2001/01/26 00:43:32 amoll Exp $

#include <BALL/MOLVIEW/FUNCTOR/lineModel.h>

using namespace std;

namespace BALL
{
	namespace MOLVIEW
	{

		AddLineModel::AddLineModel()
			: AtomBondModelBaseProcessor()
		{
		}

		AddLineModel::AddLineModel(const AddLineModel& rAddLineModel, bool deep)
			: AtomBondModelBaseProcessor(rAddLineModel, deep)
		{
		}

		AddLineModel::~AddLineModel()
			throw()
		{
			#ifdef BALL_VIEW_DEBUG
				cout << "Destructing object " << (void *)this << " of class " << getBallClass().getName() << endl;
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
			AtomBondModelBaseProcessor::destroy();
		}

		void AddLineModel::set(const AddLineModel &rAddLineModel, bool deep)
		{
			AtomBondModelBaseProcessor::set(rAddLineModel, deep);
		}

		AddLineModel& AddLineModel::operator = (const AddLineModel &rAddLineModel)
		{
			set(rAddLineModel);

			return *this;
		}

		void AddLineModel::get(AddLineModel &rAddLineModel, bool deep) const
		{
			rAddLineModel.set(*this, deep);
		}

		void AddLineModel::swap(AddLineModel &rAddLineModel)
		{
			AtomBondModelBaseProcessor::swap(rAddLineModel);
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
			Point *pPoint = createPoint_();

			BALL_PRECONDITION
				(pPoint != 0,
				 BALL_MOLVIEW_LINEMODEL_ERROR_HANDLER
				 (AddLineModel::ERROR__CANNOT_CREATE_POINT));

			// carry on selected flag
			pPoint->Selectable::set(*atom);

			pPoint->PropertyManager::set(*this);
			pPoint->PropertyManager::setProperty(GeometricObject::PROPERTY__MODEL_LINES);
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

		void AddLineModel::read(istream & /* s */)
		{
			throw ::BALL::Exception::NotImplemented(__FILE__, __LINE__);
		}

		void AddLineModel::write(ostream & /* s */) const
		{
			throw ::BALL::Exception::NotImplemented(__FILE__, __LINE__);
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
