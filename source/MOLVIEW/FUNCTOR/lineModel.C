// $Id: lineModel.C,v 1.7 2000/06/25 19:06:36 hekl Exp $

#include <BALL/MOLVIEW/FUNCTOR/lineModel.h>

using namespace std;

namespace BALL
{

	namespace MOLVIEW
	{


		AddLineModel::AddLineModel
			()
				: 
				AtomBondModelBaseProcessor()
		{
		}

		AddLineModel::AddLineModel
			(const AddLineModel &__rAddLineModel,
			 bool deep)
				:
				AtomBondModelBaseProcessor(__rAddLineModel, deep)
		{
		}

		AddLineModel::~AddLineModel
			()
		{
			#ifdef BALL_VIEW_DEBUG
				cout << "Destructing object " << (void *)this 
			 << " of class " << getBallClass().getName() << endl;
			#endif 

			destroy();
		}

		void 
		AddLineModel::clear
			()
		{
			AtomBondModelBaseProcessor::clear();
		}

		void 
		AddLineModel::destroy
			()
		{
			AtomBondModelBaseProcessor::destroy();
		}

		void 
		AddLineModel::set
			(const AddLineModel &__rAddLineModel,
			 bool deep)
		{
			AtomBondModelBaseProcessor::set(__rAddLineModel, deep);
		}

		AddLineModel &
		AddLineModel::operator =
			(const AddLineModel &__rAddLineModel)
		{
			set(__rAddLineModel);

			return *this;
		}

		void 
		AddLineModel::get
			(AddLineModel &__rAddLineModel,
			 bool deep) const
		{
			__rAddLineModel.set(*this, deep);
		}

		void 
		AddLineModel::swap
			(AddLineModel &__rAddLineModel)
		{
			AtomBondModelBaseProcessor::swap(__rAddLineModel);
		}

		bool AddLineModel::start()
		{
			if (hasProperty(GeometricObject::PROPERTY__DRAWING_MODE_SOLID) == true)
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
				
		Processor::Result 
		AddLineModel::operator()
			(Composite &composite)
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
			Point *__pPoint = createPoint_();

			BALL_PRECONDITION
				(__pPoint != 0,
				 BALL_MOLVIEW_LINEMODEL_ERROR_HANDLER
				 (AddLineModel::ERROR__CANNOT_CREATE_POINT));

			// carry on selected flag
			__pPoint->Selectable::set(*atom);

			__pPoint->PropertyManager::set(*this);
			__pPoint->PropertyManager::setProperty(GeometricObject::PROPERTY__MODEL_LINES);
			__pPoint->setVertexAddress(atom->getPosition());
			
			atom->host(*getColorCalculator());

			__pPoint->setColor(getColorCalculator()->getColor());

			// append line in Atom
			composite.appendChild(*__pPoint);

			// collect used atoms
			insertAtom_(atom);

			return Processor::CONTINUE;
		}

		void 
		AddLineModel::dump
			(ostream& s, Size depth) const
		{
			BALL_DUMP_STREAM_PREFIX(s);
			
			BALL_DUMP_DEPTH(s, depth);
			BALL_DUMP_HEADER(s, this, this);

			AtomBondModelBaseProcessor::dump(s, depth + 1);

			BALL_DUMP_STREAM_SUFFIX(s);
		}

		void 
		AddLineModel::read
			(istream & /* s */)
		{
			throw ::BALL::Exception::NotImplemented(__FILE__, __LINE__);
		}

		void 
		AddLineModel::write
			(ostream & /* s */) const
		{
			throw ::BALL::Exception::NotImplemented(__FILE__, __LINE__);
		}

		Point *
		AddLineModel::createPoint_
			()
		{
			return (Point *)(new Point());
		}

		Line *
		AddLineModel::createLine_
			()
		{
			return (Line *)(new Line());
		}

		TwoColoredLine *
		AddLineModel::createTwoColoredLine_
			()
		{
			return (TwoColoredLine *)(new TwoColoredLine());
		}


#		ifdef BALL_NO_INLINE_FUNCTIONS
#			include <BALL/MOLVIEW/FUNCTOR/lineModel.iC>
#		endif

	} // namespace MOLVIEW

} // namespace BALL
