// $Id: removeModel.C,v 1.1 1999/12/19 18:14:16 oliver Exp $

#include <BALL/MOLVIEW/FUNCTOR/removeModel.h>

namespace BALL
{

	namespace MOLVIEW
	{

		RemoveModel::RemoveModel
			()
				: 
				BaseModelProcessor(),
				used_atoms_()
		{
		}

		RemoveModel::RemoveModel
			(const RemoveModel &model,
			 bool deep)
				:
				BaseModelProcessor(model, deep),
				used_atoms_()
		{
		}

		RemoveModel::~RemoveModel
			()
		{
			#ifdef BALL_VIEW_DEBUG
				cout << "Destructing object " << (void *)this 
			 << " of class " << getBallClass().getName() << endl;
			#endif 

			destroy();
		}

		void 
		RemoveModel::clear
			()
		{
			BaseModelProcessor::clear();

			used_atoms_.clear();
		}

		void 
		RemoveModel::destroy
			()
		{
			BaseModelProcessor::destroy();

			used_atoms_.destroy();
		}

		void 
		RemoveModel::set
			(const RemoveModel &model,
			 bool deep)
		{
			BaseModelProcessor::set(model, deep);
		}

		RemoveModel &
		RemoveModel::operator =
			(const RemoveModel &model)
		{
			set(model);

			return *this;
		}

		void 
		RemoveModel::get
			(RemoveModel &model,
			 bool deep) const
		{
			model.set(*this, deep);
		}

		void 
		RemoveModel::swap
			(RemoveModel &model)
		{
			BaseModelProcessor::swap(model);
		}

		bool 
		RemoveModel::start
			()
		{
			used_atoms_.clear();
			getSearcher().clear();

			//			getSearcher().setProperty(GeometricObject::PROPERTY__MODELBALL_AND_STICK);

			return BaseModelProcessor::start();
		}
				
		bool 
		RemoveModel::finish
			()
		{
			// generate StickPrimitives
			Atom *first__pAtom = 0;
			Atom *second__pAtom = 0;
			Bond *__pBond = 0;
			AtomBondIterator bond__Iterator;

			List<Atom*>::Iterator list_iterator;

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
						// get bond model
						__pBond->applyChild(getSearcher());

						// if found, delete bond model 
						if (getSearcher().geometricObjectFound() == true)
						{
							// get bond model
							Composite *bond_composite 
								= (Composite *)(getSearcher().getGeometricObject());

							// remove bond model from bond
							__pBond->Composite::removeChild(*bond_composite);
							
							// delete bond model
							delete bond_composite;
						}
					}
				}

				// get the geometric object
				first__pAtom->applyChild(getSearcher());

				Composite *atom_model
					= (Composite *)(getSearcher().getGeometricObject());

				// remove atom model from atom
				first__pAtom->Composite::removeChild(*atom_model);

				// delete atom model
				delete atom_model;
			}
			
			return true;
		}
				
		Processor::Result 
		RemoveModel::operator()
			(Atom &atom)
		{
			// check if there are already models appended
			atom.applyChild(getSearcher());

			// geometric object is not existent => do nothing
			if (getSearcher().geometricObjectFound() == false)
			{
				return Processor::CONTINUE;
			}

			// collect atom with geometric object for deletion
			used_atoms_.push_back(&atom);

			return Processor::CONTINUE;
		}

		void 
		RemoveModel::dump
			(ostream& s,
			 unsigned long depth) const
		{
			BALL_DUMP_STREAM_PREFIX(s);
			
			BALL_DUMP_DEPTH(s, depth);
			BALL_DUMP_HEADER(s, this, this);

			BaseModelProcessor::dump(s, depth + 1);

			BALL_DUMP_STREAM_SUFFIX(s);
		}

		void 
		RemoveModel::read
			(istream & /* s */)
		{
			throw ::BALL::Exception::NotImplemented(__FILE__, __LINE__);
		}

		void 
		RemoveModel::write
			(ostream & /* s */) const
		{
			throw ::BALL::Exception::NotImplemented(__FILE__, __LINE__);
		}


#		ifdef BALL_NO_INLINE_FUNCTIONS
#			include <BALL/MOLVIEW/FUNCTOR/removeModel.iC>
#		endif

	} // namespace MOLVIEW

} // namespace BALL
