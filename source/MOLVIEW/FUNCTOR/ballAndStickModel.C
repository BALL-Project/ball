// $Id: ballAndStickModel.C,v 1.13 2001/07/15 18:50:28 oliver Exp $

#include <BALL/MOLVIEW/FUNCTOR/ballAndStickModel.h>

using namespace std;

namespace BALL
{
	namespace MOLVIEW
	{

		AddBallAndStickModel::AddBallAndStickModel()
			throw()
			: AtomBondModelBaseProcessor(),
				ball_radius_((Real)0.4),
				stick_radius_((Real)0.2),
				ball_and_stick_(true)
		{
		}

		AddBallAndStickModel::AddBallAndStickModel
			(const AddBallAndStickModel &add_ball_and_stick, bool deep)
			throw()
			: AtomBondModelBaseProcessor(add_ball_and_stick, deep),
				ball_radius_(add_ball_and_stick.ball_radius_),
				stick_radius_(add_ball_and_stick.stick_radius_),
				ball_and_stick_(add_ball_and_stick.ball_and_stick_)
		{
		}

		AddBallAndStickModel::~AddBallAndStickModel()
			throw()
		{
			#ifdef BALL_VIEW_DEBUG
				cout << "Destructing object " << (void *)this 
			 << " of class " << RTTI::getName<AddBallAndStickModel>() << endl;
			#endif 

			destroy();
		}

		void AddBallAndStickModel::clear()
			throw()
		{
			AtomBondModelBaseProcessor::clear();

			ball_radius_ = (Real)0.4;
			stick_radius_ = (Real)0.2;
			ball_and_stick_ = true;
		}

		void AddBallAndStickModel::destroy()
			throw()
		{
		}

		void AddBallAndStickModel::set
			(const AddBallAndStickModel &add_ball_and_stick, bool deep)
			throw()
		{
			AtomBondModelBaseProcessor::set(add_ball_and_stick, deep);

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

		void AddBallAndStickModel::get(AddBallAndStickModel &add_ball_and_stick, bool deep) const
			throw()
		{
			add_ball_and_stick.set(*this, deep);
		}

		void AddBallAndStickModel::swap(AddBallAndStickModel &add_ball_and_stick)
			throw()
		{
			AtomBondModelBaseProcessor::swap(add_ball_and_stick);

			Real temp_Real = ball_radius_;
			ball_radius_ = add_ball_and_stick.ball_radius_;
			add_ball_and_stick.ball_radius_ = temp_Real;

			temp_Real = stick_radius_;
			stick_radius_ = add_ball_and_stick.stick_radius_;
			add_ball_and_stick.stick_radius_ = temp_Real;

			bool temp_bool = ball_and_stick_;
			ball_and_stick_ = add_ball_and_stick.ball_and_stick_;
			add_ball_and_stick.ball_and_stick_ = temp_bool;
		}

		void AddBallAndStickModel::setBallRadius(const Real radius)
			throw(Exception::OutOfRange)
		{
			// a radius never can be lower or equal 0
			if (radius <= (Real)0)
			{
					throw Exception::OutOfRange(__FILE__, __LINE__);
			}
			
			ball_radius_ = radius;
		}

		void AddBallAndStickModel::setStickRadius(const Real radius)
			throw(Exception::OutOfRange)
		{
			// a radius never can be lower or equal 0
			if (radius <= (Real)0)
			{
				throw Exception::OutOfRange(__FILE__, __LINE__);
			}

			stick_radius_ = radius;
		}

		bool AddBallAndStickModel::start()
		{
			// init model connector
			getModelConnector()->setProperties(*this);
			getModelConnector()->setProperty(String("STICK_RADIUS"), (float)stick_radius_);
			getModelConnector()->setProperty(String("BALL_RADIUS"), (float)ball_radius_);

			return AtomBondModelBaseProcessor::start();
		}
				
		bool AddBallAndStickModel::finish()
		{
			buildBondModels_();
			
			return true;
		}
				
		Processor::Result AddBallAndStickModel::operator() (Composite& composite)
		{
			// composite is an atom ?
			if (!RTTI::isKindOf<Atom>(composite))
			{
				return Processor::CONTINUE;
			}

			Atom *atom = RTTI::castTo<Atom>(composite);

			// remove only models appended to atom
			removeGeometricObjects_(*atom, true);

			// generate BallPrimitive
			Sphere* sphere_ptr = createSphere_();

			if (sphere_ptr == 0)
			{
					throw Exception::OutOfMemory
						(__FILE__, __LINE__, sizeof(Sphere));
			}

			// carry on selected flag
			sphere_ptr->Selectable::set(*atom);

			sphere_ptr->PropertyManager::set(*this);
			sphere_ptr->PropertyManager::setProperty(PROPERTY__MODEL_BALL_AND_STICK);

			if (ball_and_stick_ == true)
			{
				sphere_ptr->setRadius(ball_radius_);
			}
			else
			{
				sphere_ptr->setRadius(stick_radius_);
			}

			sphere_ptr->setVertexAddress(atom->getPosition());
			
			atom->host(*getColorCalculator());

			sphere_ptr->setColor(getColorCalculator()->getColor());
			
			// append sphere in Atom
			composite.appendChild(*sphere_ptr);

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

		Sphere* AddBallAndStickModel::createSphere_()
		{
			return (Sphere *)(new Sphere());
		}

		Tube* AddBallAndStickModel::createTube_()
		{
			return (Tube *)(new Tube());
		}

		TwoColoredTube*AddBallAndStickModel::createTwoColoredTube_()
		{
			return (TwoColoredTube *)(new TwoColoredTube());
		}


#		ifdef BALL_NO_INLINE_FUNCTIONS
#			include <BALL/MOLVIEW/FUNCTOR/ballAndStickModel.iC>
#		endif

	} // namespace MOLVIEW

} // namespace BALL
