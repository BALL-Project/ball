// $Id: lineModel.C,v 1.6 2000/06/18 16:33:38 hekl Exp $

#include <BALL/MOLVIEW/FUNCTOR/lineModel.h>

using namespace std;

namespace BALL
{

	namespace MOLVIEW
	{


		AddLineModel::AddLineModel
			()
				: 
				BaseModelProcessor(),
				used_atoms_()
		{
			setProperty(GeometricObject::PROPERTY__MODEL_LINES);
		}

		AddLineModel::AddLineModel
			(const AddLineModel &__rAddLineModel,
			 bool deep)
				:
				BaseModelProcessor(__rAddLineModel, deep),
				used_atoms_()
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
			BaseModelProcessor::clear();

			setProperty(GeometricObject::PROPERTY__MODEL_LINES);

			used_atoms_.clear();
		}

		void 
		AddLineModel::destroy
			()
		{
			BaseModelProcessor::destroy();
			used_atoms_.destroy();
		}

		void 
		AddLineModel::set
			(const AddLineModel &__rAddLineModel,
			 bool deep)
		{
			BaseModelProcessor::set(__rAddLineModel, deep);
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
			BaseModelProcessor::swap(__rAddLineModel);
		}

		bool 
		AddLineModel::start
			()
		{
			used_atoms_.clear();

			if (hasProperty(GeometricObject::PROPERTY__DRAWING_MODE_SOLID) == true)
			{
				setProperty(GeometricObject::PROPERTY__DRAWING_MODE_WIREFRAME);
			}

			return BaseModelProcessor::start();
		}
				
		bool 
		AddLineModel::finish
			()
		{
			// generate Lines
			Atom *first__pAtom = 0;
			Atom *second__pAtom = 0;
			Bond *__pBond = 0;
			AtomBondIterator bond__Iterator;
			ColorRGBA first__ColorRGBA;
			ColorRGBA second__ColorRGBA;

			List<Atom *>::Iterator list__Iterator;

			// for all used atoms
			for (list__Iterator = used_atoms_.begin();
					 list__Iterator != used_atoms_.end();
					 ++list__Iterator)
			{
				first__pAtom = *list__Iterator;

				// for all bonds connected from first- to second atom
				BALL_FOREACH_ATOM_BOND(*first__pAtom, bond__Iterator)
				{
					__pBond = &(*bond__Iterator);

					second__pAtom = __pBond->getSecondAtom();

					// use only atoms with greater handles than first atom
					if (*first__pAtom < *second__pAtom)
					{
						// remove all models appended to bond
						getSearcher_().clearProperty(GeometricObject::PROPERTY__MODEL_LINES);
						removeGeometricObjects_(*__pBond, true);

						// search for Line representants
						getSearcher_().setProperty(GeometricObject::PROPERTY__MODEL_LINES);
						second__pAtom->applyChild(getSearcher_());
						
						// if found, build a Line between them
						if (getSearcher_().geometricObjectsFound() == true)
						{
							// get colors from both atoms
							first__pAtom->host(*getColorCalculator());
							first__ColorRGBA = getColorCalculator()->getColor();
							
							second__pAtom->host(*getColorCalculator());
							second__ColorRGBA = getColorCalculator()->getColor();
							
							// if both colors are identical
							if (first__ColorRGBA == second__ColorRGBA)
							{
								// generate single colored line
								Line *__pLine = createLine_();
								
								BALL_PRECONDITION
									(__pLine != 0,
									 BALL_MOLVIEW_LINEMODEL_ERROR_HANDLER
									 (AddLineModel::ERROR__CANNOT_CREATE_LINE));
								
								__pLine->PropertyManager::set(*this);
								__pLine->setVertex1Address(first__pAtom->getPosition());
								__pLine->setVertex2Address(second__pAtom->getPosition());
								__pLine->setColor(first__ColorRGBA);
								
								__pBond->Composite::appendChild(*__pLine);
							}
							else
							{
								// generate two colored line
								TwoColoredLine *__pTwoColoredLine = createTwoColoredLine_();
								
								BALL_PRECONDITION
									(__pTwoColoredLine != 0,
									 BALL_MOLVIEW_LINEMODEL_ERROR_HANDLER
									 (AddLineModel::ERROR__CANNOT_CREATE_2CLINE));
								
								__pTwoColoredLine->PropertyManager::set(*this);
								__pTwoColoredLine->setVertex1Address(first__pAtom->getPosition());
								__pTwoColoredLine->setVertex2Address(second__pAtom->getPosition());
								__pTwoColoredLine->setColor1(first__ColorRGBA);
								__pTwoColoredLine->setColor2(second__ColorRGBA);
								
								__pBond->Composite::appendChild(*__pTwoColoredLine);
							}
						}
					}
				}
			}

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

			__pPoint->PropertyManager::set(*this);
			__pPoint->setVertexAddress(atom->getPosition());
			
			atom->host(*getColorCalculator());

			__pPoint->setColor(getColorCalculator()->getColor());

			// append line in Atom
			composite.appendChild(*__pPoint);

			// collect used atoms
			used_atoms_.push_back(atom);

			return Processor::CONTINUE;
		}

		void 
		AddLineModel::dump
			(ostream& s, Size depth) const
		{
			BALL_DUMP_STREAM_PREFIX(s);
			
			BALL_DUMP_DEPTH(s, depth);
			BALL_DUMP_HEADER(s, this, this);

			BaseModelProcessor::dump(s, depth + 1);

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
