// $Id: surfaceModel.C,v 1.13.4.2 2002/08/26 15:55:32 oliver Exp $

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
		{
			get_composite_ = true;
			start_composite_ = 0;
			return BaseModelProcessor::start();
		}
				
		bool AddSurfaceModel::finish()
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

				System* system = dynamic_cast<System*>(start_composite_);
				if (system != 0)
				{
					SurfaceProcessor sp;
					try 
					{
						system->apply(sp);
					}
					catch (Exception::GeneralException e)
					{
						Log.error() << "SurfaceModel: caught exception while calculating molecular surface: " << e << endl;
					}
					catch (...)
					{
						Log.error() << "SurfaceModel: caught exception while calculating molecular surface" << endl;
					}
					Log.info() << "assigning surface (" << sp.getSurface().vertex.size() << " vertices, " 
										 << sp.getSurface().triangle.size() << " triangles)" << endl;
					*static_cast<Surface*>(mesh) = sp.getSurface();

					// TEST!!! TEST!!! TEST!!!
					mesh->colorList.resize(mesh->vertex.size());
					ColorRGBA list[3];
					list[0] = ColorRGBA(0.,0.,1.,1.);
					list[1] = ColorRGBA(0.,1.,1.,1.);
					list[2] = ColorRGBA(1.,1.,0.,1.);
					
					ColorTable table(list, 3);
					table.setNumberOfColors(53);
					table.createTable();
					table.setRange(0.,20.);

					// calculate the center of mass of the mesh
					Vector3 com = Vector3(0,0,0);
					
					for (int i=0; i<mesh->colorList.size(); i++)
					{
						com += mesh->vertex[i];
					}

					com /= (float)mesh->colorList.size();
					
					for (int i=0; i<mesh->colorList.size();i++)
					{
						float len = (mesh->vertex[i]-com).getLength();

						mesh->colorList[i] = table.map(len);
					}
					
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
		AddSurfaceModel::operator () (Composite& composite)
		{
			// take first composite, surface will be inserted to it later
			if (get_composite_)
			{
				start_composite_ = &composite;
				get_composite_ = false;
			}

			return Processor::CONTINUE;
		}

		void AddSurfaceModel::dump(std::ostream& s, Size depth) const
			throw()
		{
			BALL_DUMP_STREAM_PREFIX(s);
			
			BALL_DUMP_DEPTH(s, depth);
			BALL_DUMP_HEADER(s, this, this);

			BaseModelProcessor::dump(s, depth + 1);

			BALL_DUMP_STREAM_SUFFIX(s);
		}

		Mesh*	AddSurfaceModel::createMesh_()
		{
			return (Mesh *)(new Mesh());
		}


#		ifdef BALL_NO_INLINE_FUNCTIONS
#			include <BALL/MOLVIEW/FUNCTOR/surfaceModel.iC>
#		endif

	} // namespace MOLVIEW

} // namespace BALL
