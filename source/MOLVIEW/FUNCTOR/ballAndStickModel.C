// $Id: ballAndStickModel.C,v 1.2 1999/12/19 17:14:25 oliver Exp $

#include <BALL/MOLVIEW/FUNCTOR/ballAndStickModel.h>

namespace BALL
{

	namespace MOLVIEW
	{

		AddBallAndStickModel::AddBallAndStickModel
			()
				: 
				BaseModelProcessor(),
				ball_radius_((Real)0.4),
				stick_radius_((Real)0.2),
				ball_and_stick_(true),
				used_atoms_()
		{
			setProperty(GeometricObject::PROPERTY__MODELBALL_AND_STICK);
		}

		AddBallAndStickModel::AddBallAndStickModel
			(const AddBallAndStickModel &add_ball_and_stick,
			 bool deep)
				:
				BaseModelProcessor(add_ball_and_stick, deep),
				ball_radius_(add_ball_and_stick.ball_radius_),
				stick_radius_(add_ball_and_stick.stick_radius_),
				ball_and_stick_(add_ball_and_stick.ball_and_stick_),
				used_atoms_()
		{
		}

		AddBallAndStickModel::~AddBallAndStickModel
			()
		{
			#ifdef BALL_VIEW_DEBUG
				cout << "Destructing object " << (void *)this 
			 << " of class " << getBallClass().getName() << endl;
			#endif 

			destroy();
		}

		void 
		AddBallAndStickModel::clear
			()
		{
			BaseModelProcessor::clear();

			setProperty(GeometricObject::PROPERTY__MODELBALL_AND_STICK);

			ball_radius_ = (Real)0.4;
			stick_radius_ = (Real)0.2;
			ball_and_stick_ = true;

			used_atoms_.clear();
		}

		void 
		AddBallAndStickModel::destroy
			()
		{
			BaseModelProcessor::destroy();
			used_atoms_.destroy();
		}

		void 
		AddBallAndStickModel::set
			(const AddBallAndStickModel &add_ball_and_stick,
			 bool deep)
		{
			BaseModelProcessor::set(add_ball_and_stick, deep);

			ball_radius_ = add_ball_and_stick.ball_radius_;
			stick_radius_ = add_ball_and_stick.stick_radius_;
			ball_and_stick_ = add_ball_and_stick.ball_and_stick_;
		}

		AddBallAndStickModel &
		AddBallAndStickModel::operator =
			(const AddBallAndStickModel &add_ball_and_stick)
		{
			set(add_ball_and_stick);

			return *this;
		}

		void 
		AddBallAndStickModel::get
			(AddBallAndStickModel &add_ball_and_stick,
			 bool deep) const
		{
			add_ball_and_stick.set(*this, deep);
		}

		void 
		AddBallAndStickModel::swap
			(AddBallAndStickModel &add_ball_and_stick)
		{
			BaseModelProcessor::swap(add_ball_and_stick);

			Real temp__Real = ball_radius_;
			ball_radius_ = add_ball_and_stick.ball_radius_;
			add_ball_and_stick.ball_radius_ = temp__Real;

			temp__Real = stick_radius_;
			stick_radius_ = add_ball_and_stick.stick_radius_;
			add_ball_and_stick.stick_radius_ = temp__Real;

			bool temp__bool = ball_and_stick_;
			ball_and_stick_ = add_ball_and_stick.ball_and_stick_;
			add_ball_and_stick.ball_and_stick_ = temp__bool;
		}

		void 
		AddBallAndStickModel::setBallRadius
			(const Real radius)
		{
			BALL_PRECONDITION
				(radius > (Real)0,
				 BALL_MOLVIEW_BALLANDSTICKMODEL_ERROR_HANDLER
				 (AddBallAndStickModel::ERROR_BALL_RADIUS_LOWER_OR_EQUAL_ZERO));
			
			ball_radius_ = radius;
		}

		void 
		AddBallAndStickModel::setStickRadius
			(const Real radius)
		{
			BALL_PRECONDITION
				(radius > (Real)0,
				 BALL_MOLVIEW_BALLANDSTICKMODEL_ERROR_HANDLER
				 (AddBallAndStickModel::ERROR__STICK_RADIUS_LOWER_OR_EQUAL_ZERO));
			
			stick_radius_ = radius;
		}

		bool 
		AddBallAndStickModel::start
			()
		{
			used_atoms_.clear();

			return BaseModelProcessor::start();
		}
				
		bool 
		AddBallAndStickModel::finish
			()
		{
			// generate StickPrimitives
			Atom *first__pAtom = 0;
			Atom *second__pAtom = 0;
			Bond *__pBond = 0;
			AtomBondIterator bond__Iterator;
			ColorRGBA first__ColorRGBA;
			ColorRGBA second__ColorRGBA;

			List<Atom*>::Iterator list_iterator;

			// search for BallAndStick primitives
			getSearcher().setProperty(GeometricObject::PROPERTY__MODELBALL_AND_STICK);

			// for all used atoms
			for (list_iterator = used_atoms_.begin();
					 list_iterator != used_atoms_.end(); ++list_iterator)
			{
				first__pAtom = *list_iterator;

				// for all bonds connected from first- to second atom
				BALL_FOREACH_ATOM_BOND(*first__pAtom, bond__Iterator)
				{
					__pBond = &(*bond__Iterator);
					second__pAtom = __pBond->getSecondAtom();

					// use only atoms with greater handles than first atom
					if (*first__pAtom < *second__pAtom)
					{
			// search for BallAndStick representants
			second__pAtom->applyChild(getSearcher());

			// if found, build a Tube between them
			if (getSearcher().geometricObjectFound() == true)
			{
				// get colors from both atoms
				first__pAtom->host(*getColorCalculator());
				first__ColorRGBA = getColorCalculator()->getColor();

				second__pAtom->host(*getColorCalculator());
				second__ColorRGBA = getColorCalculator()->getColor();

				// if both colors are identical
				if (first__ColorRGBA == second__ColorRGBA)
				{
					// generate single colored tube
					Tube *__pTube = createTube_();

					BALL_PRECONDITION
						(__pTube != 0,
						 BALL_MOLVIEW_BALLANDSTICKMODEL_ERROR_HANDLER
						 (AddBallAndStickModel::ERROR__CANNOT_CREATE_TUBE));

					__pTube->PropertyManager::set(*this);
					__pTube->setRadius(stick_radius_);
					__pTube->setVertex1Address(first__pAtom->getPosition());
					__pTube->setVertex2Address(second__pAtom->getPosition());
					__pTube->setColor(first__ColorRGBA);

					__pBond->Composite::appendChild(*__pTube);
				}
				else
				{
					// generate two colored tube
					TwoColoredTube *__pTwoColoredTube = createTwoColoredTube_();

					BALL_PRECONDITION
						(__pTwoColoredTube != 0,
						 BALL_MOLVIEW_BALLANDSTICKMODEL_ERROR_HANDLER
						 (AddBallAndStickModel::ERROR__CANNOT_CREATE_2CTUBE));

					__pTwoColoredTube->PropertyManager::set(*this);
					__pTwoColoredTube->setRadius(stick_radius_);
					__pTwoColoredTube->setVertex1Address(first__pAtom->getPosition());
					__pTwoColoredTube->setVertex2Address(second__pAtom->getPosition());
					__pTwoColoredTube->setColor1(first__ColorRGBA);
					__pTwoColoredTube->setColor2(second__ColorRGBA);

					__pBond->Composite::appendChild(*__pTwoColoredTube);
				}
			}
					}
				}
			}
			
			// clear search model
			getSearcher().clearProperty(GeometricObject::PROPERTY__MODELBALL_AND_STICK);

			return true;
		}
				
		Processor::Result 
		AddBallAndStickModel::operator()
			(Atom &atom)
		{
			// check if there are already BallAndStick models appended
			atom.applyChild(getSearcher());

			// geometric object is already existent => do nothing
			if (getSearcher().geometricObjectFound() == true)
			{
				return Processor::CONTINUE;
			}

			// generate BallPrimitive
			Sphere *__pSphere = createSphere_();

			BALL_PRECONDITION
				(__pSphere != 0,
				 BALL_MOLVIEW_BALLANDSTICKMODEL_ERROR_HANDLER
				 (AddBallAndStickModel::ERROR__CANNOT_CREATE_SPHERE));

			__pSphere->PropertyManager::set(*this);

			if (ball_and_stick_ == true)
			{
				__pSphere->setRadius(ball_radius_);
			}
			else
			{
				__pSphere->setRadius(stick_radius_);
			}

			__pSphere->setVertexAddress(atom.getPosition());
			
			atom.host(*getColorCalculator());

			__pSphere->setColor(getColorCalculator()->getColor());
			
			// append sphere in Atom
			atom.Composite::appendChild(*__pSphere);

			// collect used atoms
			used_atoms_.push_back(&atom);

			return Processor::CONTINUE;
		}

		void 
		AddBallAndStickModel::dump
			(ostream& s,
			 unsigned long depth) const
		{
			BALL_DUMP_STREAM_PREFIX(s);
			
			BALL_DUMP_DEPTH(s, depth);
			BALL_DUMP_HEADER(s, this, this);

			BaseModelProcessor::dump(s, depth + 1);

			BALL_DUMP_DEPTH(s, depth);
			s << "ball radius: " << ball_radius_ << endl;

			BALL_DUMP_DEPTH(s, depth);
			s << "stick radius: " << stick_radius_ << endl;

			BALL_DUMP_DEPTH(s, depth);
			s << "b&s model: " << ((bool)(ball_and_stick_ == true)) << endl;

			BALL_DUMP_DEPTH(s, depth);
			s << "s model: " << ((bool)(ball_and_stick_ == false)) << endl;

			BALL_DUMP_STREAM_SUFFIX(s);
		}

		void 
		AddBallAndStickModel::read
			(istream & /* s */)
		{
			throw ::BALL::Exception::NotImplemented(__FILE__, __LINE__);
		}

		void 
		AddBallAndStickModel::write
			(ostream & /* s */) const
		{
			throw ::BALL::Exception::NotImplemented(__FILE__, __LINE__);
		}

		Sphere *
		AddBallAndStickModel::createSphere_
			()
		{
			return (Sphere *)(new Sphere());
		}

		Tube *
		AddBallAndStickModel::createTube_
			()
		{
			return (Tube *)(new Tube());
		}

		TwoColoredTube *
		AddBallAndStickModel::createTwoColoredTube_
			()
		{
			return (TwoColoredTube *)(new TwoColoredTube());
		}


#		ifdef BALL_NO_INLINE_FUNCTIONS
#			include <BALL/MOLVIEW/FUNCTOR/ballAndStickModel.iC>
#		endif

	} // namespace MOLVIEW

} // namespace BALL
