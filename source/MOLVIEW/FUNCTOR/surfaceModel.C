// $Id: surfaceModel.C,v 1.7 2001/05/13 15:02:40 hekl Exp $

#include <BALL/MOLVIEW/FUNCTOR/surfaceModel.h>
#include <BALL/STRUCTURE/surfaceProcessor.h>

using namespace std;

namespace BALL
{

	namespace MOLVIEW
	{

		AddSurfaceModel::AddSurfaceModel()
			throw()
			: BaseModelProcessor(),
				get_composite_(true),
				start_composite_(0)
		{
		}

		AddSurfaceModel::AddSurfaceModel
			(const AddSurfaceModel& add_surface,
			 bool deep)
			throw()
			:	BaseModelProcessor(add_surface, deep),
				get_composite_(true),
				start_composite_(0)
		{
		}

		AddSurfaceModel::~AddSurfaceModel()
			throw()
		{
			#ifdef BALL_VIEW_DEBUG
				cout << "Destructing object " << (void *)this 
						 << " of class " << RTTI::getName<AddSurfaceModel>() << endl;
			#endif 

			destroy();
		}

		void AddSurfaceModel::clear()
			throw()
		{
			BaseModelProcessor::clear();
			get_composite_ = true;
			start_composite_ = 0;
		}

		void AddSurfaceModel::destroy()
			throw()
		{
		}

		bool AddSurfaceModel::start()
			throw()
		{
			get_composite_ = true;
			start_composite_ = 0;
			return BaseModelProcessor::start();
		}
				
		bool AddSurfaceModel::finish()
			throw(Exception::OutOfMemory)
		{
			// insert surface only if a composite exist
			if (start_composite_ != 0)
			{
				Mesh* mesh = createMesh_();

				if (mesh == 0)
				{
					throw Exception::OutOfMemory
						(__FILE__, __LINE__, sizeof(Mesh));
				}
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
					SurfaceProcessor sp;
					cerr << "applying SurfaceProcessor..." << endl;
					system->apply(sp);
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

			}

			return true;
		}
				
		Processor::Result 
		AddSurfaceModel::operator()
			(Composite &composite)
			throw()
		{
			// take first composite, surface will be inserted to it later
			if (get_composite_)
			{
				start_composite_ = &composite;
				get_composite_ = false;
			}

			return Processor::CONTINUE;
		}

		void AddSurfaceModel::dump
			(std::ostream& s, Size depth) const
			throw()
		{
			BALL_DUMP_STREAM_PREFIX(s);
			
			BALL_DUMP_DEPTH(s, depth);
			BALL_DUMP_HEADER(s, this, this);

			BaseModelProcessor::dump(s, depth + 1);

			BALL_DUMP_STREAM_SUFFIX(s);
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
