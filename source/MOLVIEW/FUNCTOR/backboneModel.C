// $Id: backboneModel.C,v 1.3 2001/05/13 15:02:38 hekl Exp $

#ifndef BALL_MOLVIEW_FUNCTOR_BACKBONEMODEL_H
#include <BALL/MOLVIEW/FUNCTOR/backboneModel.h>
#endif

using namespace std;

namespace BALL
{

	namespace MOLVIEW
	{

		AddBackboneModel::AddBackboneModel()
			throw()
			: BaseModelProcessor(),
				get_composite_(true),
				start_composite_(0)
		{
		}

		AddBackboneModel::AddBackboneModel
			(const AddBackboneModel& add_Backbone,
			 bool deep)
			throw()
			:	BaseModelProcessor(add_Backbone, deep),
				get_composite_(true),
				start_composite_(0)
		{
		}

		AddBackboneModel::~AddBackboneModel()
			throw()
		{
			#ifdef BALL_VIEW_DEBUG
				cout << "Destructing object " << (void *)this 
						 << " of class " << RTTI::getName<AddBackboneModel>() << endl;
			#endif 

			destroy();
		}

		void AddBackboneModel::clear()
			throw()
		{
			BaseModelProcessor::clear();
			get_composite_ = true;
			start_composite_ = 0;

			atoms_.clear();
			atoms_color_.clear();
		}

		void AddBackboneModel::destroy()
			throw()
		{
		}

		bool AddBackboneModel::start()
			throw()
		{
			get_composite_ = true;
			start_composite_ = 0;
			atoms_.clear();
			atoms_color_.clear();

			return BaseModelProcessor::start();
		}
				
		bool AddBackboneModel::finish()
			throw(Exception::OutOfMemory)
		{
			// insert Backbone only if a composite exist
			if (start_composite_ != 0)
			{
				Backbone* backbone = createBackbone_();

				if (backbone == 0)
				{
					throw Exception::OutOfMemory
						(__FILE__, __LINE__, sizeof(Backbone));
				}

				// get info from the start composite
				MolecularInformation molecular_information;
				start_composite_->host(molecular_information);

				backbone->PropertyManager::set(*this);
				backbone->setAtomList(atoms_);
				backbone->setAtomColorList(atoms_color_);
				backbone->createBackbone();

				System* system = dynamic_cast<System*>(&(start_composite_->getRoot()));

				if (system != 0)
				{
					backbone->setName(String("Backbone of ")
												+ molecular_information.getTypeName() 
												+ String(" (")
												+ molecular_information.getName()
												+ String(")"));

					start_composite_->getRoot().appendChild(*backbone);
				}
			}

			return true;
		}
				
		Processor::Result 
		AddBackboneModel::operator()
			(Composite &composite)
			throw()
		{
			// take first composite, Backbone will be inserted to it later
			if (get_composite_)
			{
				start_composite_ = &composite;
				get_composite_ = false;
			}

			// composite is an atom ?
			if (!RTTI::isKindOf<Atom>(composite))
			{
				return Processor::CONTINUE;
			}

			Atom *atom = RTTI::castTo<Atom>(composite);

			// collect only CA-Atoms
			if (atom->getName().hasSubstring(String("CA")))
			{
				atom->host(*getColorCalculator());
				atoms_.push_back(atom);
				atoms_color_.push_back(getColorCalculator()->getColor());
			}

			return Processor::CONTINUE;
		}

		void 
		AddBackboneModel::dump
			(std::ostream& s, Size depth) const
			throw()
		{
			BALL_DUMP_STREAM_PREFIX(s);
			
			BALL_DUMP_DEPTH(s, depth);
			BALL_DUMP_HEADER(s, this, this);

			BaseModelProcessor::dump(s, depth + 1);

			BALL_DUMP_STREAM_SUFFIX(s);
		}

		Backbone *
		AddBackboneModel::createBackbone_
			()
		{
			return (Backbone *)(new Backbone());
		}


#		ifdef BALL_NO_INLINE_FUNCTIONS
#			include <BALL/MOLVIEW/FUNCTOR/backboneModel.iC>
#		endif

	} // namespace MOLVIEW

} // namespace BALL
