// $Id: backboneModel.C,v 1.1 2001/01/07 15:38:58 hekl Exp $

#include <BALL/MOLVIEW/FUNCTOR/backboneModel.h>

using namespace std;

namespace BALL
{

	namespace MOLVIEW
	{

		AddBackboneModel::AddBackboneModel()
			: BaseModelProcessor(),
				get_composite_(true),
				start_composite_(0)
		{
		}

		AddBackboneModel::AddBackboneModel
			(const AddBackboneModel& add_Backbone,
			 bool deep)
			:	BaseModelProcessor(add_Backbone, deep),
				get_composite_(true),
				start_composite_(0)
		{
		}

		AddBackboneModel::~AddBackboneModel()
		{
			#ifdef BALL_VIEW_DEBUG
				cout << "Destructing object " << (void *)this 
						 << " of class " << getBallClass().getName() << endl;
			#endif 

			destroy();
		}

		void AddBackboneModel::clear()
		{
			BaseModelProcessor::clear();
			get_composite_ = true;
			start_composite_ = 0;

			atoms_.clear();
			atoms_color_.clear();
		}

		void AddBackboneModel::destroy()
		{
			BaseModelProcessor::destroy();
			get_composite_ = true;
			start_composite_ = 0;

			atoms_.clear();
			atoms_color_.clear();
		}

		void AddBackboneModel::set
			(const AddBackboneModel& add_Backbone,
			 bool deep)
		{
			BaseModelProcessor::set(add_Backbone, deep);
			get_composite_ = true;
			start_composite_ = 0;
			atoms_.clear();
			atoms_color_.clear();
		}

		AddBackboneModel& AddBackboneModel::operator =
			(const AddBackboneModel& add_Backbone)
		{
			set(add_Backbone);

			return *this;
		}

		void AddBackboneModel::get
			(AddBackboneModel& add_Backbone,
			 bool deep) const
		{
			add_Backbone.set(*this, deep);
		}

		void AddBackboneModel::swap
			(AddBackboneModel& add_Backbone)
		{
			BaseModelProcessor::swap(add_Backbone);
			get_composite_ = true;
			start_composite_ = 0;
			atoms_.clear();
			atoms_color_.clear();
		}

		bool AddBackboneModel::start()
		{
			get_composite_ = true;
			start_composite_ = 0;
			atoms_.clear();
			atoms_color_.clear();

			return BaseModelProcessor::start();
		}
				
		bool AddBackboneModel::finish()
		{
			// insert Backbone only if a composite exist
			if (start_composite_ != 0)
			{
				Backbone* backbone = createBackbone_();

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
		{
			BALL_DUMP_STREAM_PREFIX(s);
			
			BALL_DUMP_DEPTH(s, depth);
			BALL_DUMP_HEADER(s, this, this);

			BaseModelProcessor::dump(s, depth + 1);

			BALL_DUMP_STREAM_SUFFIX(s);
		}

		void 
		AddBackboneModel::read
			(std::istream & /* s */)
		{
			throw ::BALL::Exception::NotImplemented(__FILE__, __LINE__);
		}

		void 
		AddBackboneModel::write
			(std::ostream & /* s */) const
		{
			throw ::BALL::Exception::NotImplemented(__FILE__, __LINE__);
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
