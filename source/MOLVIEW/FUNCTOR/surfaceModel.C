// $Id: surfaceModel.C,v 1.4 2000/10/11 10:19:59 oliver Exp $

#include <BALL/MOLVIEW/FUNCTOR/surfaceModel.h>
#include <BALL/STRUCTURE/surfaceProcessor.h>

using namespace std;

namespace BALL
{

	namespace MOLVIEW
	{

		AddSurfaceModel::AddSurfaceModel()
			: BaseModelProcessor(),
				get_composite_(true),
				start_composite_(0)
		{
		}

		AddSurfaceModel::AddSurfaceModel
			(const AddSurfaceModel& add_surface,
			 bool deep)
			:	BaseModelProcessor(add_surface, deep),
				get_composite_(true),
				start_composite_(0)
		{
		}

		AddSurfaceModel::~AddSurfaceModel()
		{
			#ifdef BALL_VIEW_DEBUG
				cout << "Destructing object " << (void *)this 
			 << " of class " << getBallClass().getName() << endl;
			#endif 

			destroy();
		}

		void AddSurfaceModel::clear()
		{
			BaseModelProcessor::clear();
			get_composite_ = true;
			start_composite_ = 0;
		}

		void AddSurfaceModel::destroy()
		{
			BaseModelProcessor::destroy();
			get_composite_ = true;
			start_composite_ = 0;
		}

		void AddSurfaceModel::set
			(const AddSurfaceModel& add_surface,
			 bool deep)
		{
			BaseModelProcessor::set(add_surface, deep);
			get_composite_ = true;
			start_composite_ = 0;
		}

		AddSurfaceModel& AddSurfaceModel::operator =
			(const AddSurfaceModel& add_surface)
		{
			set(add_surface);

			return *this;
		}

		void AddSurfaceModel::get
			(AddSurfaceModel& add_surface,
			 bool deep) const
		{
			add_surface.set(*this, deep);
		}

		void AddSurfaceModel::swap
			(AddSurfaceModel& add_surface)
		{
			BaseModelProcessor::swap(add_surface);
			get_composite_ = true;
			start_composite_ = 0;
		}

		bool AddSurfaceModel::start()
		{
			return BaseModelProcessor::start();
		}
				
		bool AddSurfaceModel::finish()
		{
			// insert surface only if a composite exist
			if (start_composite_ != 0)
			{
				Mesh* mesh = createMesh_();

				// create mesh
				// ...

				// get info from the start composite
				MolecularInformation molecular_information;
				start_composite_->host(molecular_information);

				mesh->PropertyManager::set(*this);
				// mesh->readMSMSFile("/KM/usr/oliver/test.surf.vert.large", "/KM/usr/oliver/test.surf.face.large");

				System* system = dynamic_cast<System*>(start_composite_);
				if (system != 0)
				{
					cerr << "number of atoms in system:" << system->countAtoms() << endl;
				}

				SurfaceProcessor sp;
				cerr << "applying SurfaceProcessor..." << endl;
				start_composite_->apply(sp);
				cerr << "assigning surface..." << endl;
				sp.getSurface(*mesh);
				cerr << "setting mesh name..." << endl;

				mesh->setName(String("Surface of ")
											+ molecular_information.getTypeName() 
											+ String(" (")
											+ molecular_information.getName()
											+ String(")"));


				start_composite_->getRoot().appendChild(*mesh);
			}

			return true;
		}
				
		Processor::Result 
		AddSurfaceModel::operator()
			(Composite &composite)
		{
			// take first composite, surface will be inserted to it later
			if (get_composite_)
			{
				start_composite_ = &composite;
				get_composite_ = false;
			}

			return Processor::CONTINUE;
		}

		void 
		AddSurfaceModel::dump
			(std::ostream& s, Size depth) const
		{
			BALL_DUMP_STREAM_PREFIX(s);
			
			BALL_DUMP_DEPTH(s, depth);
			BALL_DUMP_HEADER(s, this, this);

			BaseModelProcessor::dump(s, depth + 1);

			BALL_DUMP_STREAM_SUFFIX(s);
		}

		void 
		AddSurfaceModel::read
			(std::istream & /* s */)
		{
			throw ::BALL::Exception::NotImplemented(__FILE__, __LINE__);
		}

		void 
		AddSurfaceModel::write
			(std::ostream & /* s */) const
		{
			throw ::BALL::Exception::NotImplemented(__FILE__, __LINE__);
		}

		Mesh *
		AddSurfaceModel::createMesh_
			()
		{
			return (Mesh *)(new Mesh());
		}


#		ifdef BALL_NO_INLINE_FUNCTIONS
#			include <BALL/MOLVIEW/FUNCTOR/surfaceModel.iC>
#		endif

	} // namespace MOLVIEW

} // namespace BALL
