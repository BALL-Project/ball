// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: surfaceModel.C,v 1.22 2003/05/08 12:05:08 oliver Exp $

#include <BALL/MOLVIEW/FUNCTOR/surfaceModel.h>
#include <BALL/STRUCTURE/surfaceProcessor.h>
#include <BALL/MOLVIEW/FUNCTOR/molecularInformation.h>
#include <BALL/KERNEL/atomContainer.h>
#include <BALL/KERNEL/forEach.h>

using namespace std;

namespace BALL
{
	namespace MOLVIEW
	{

		AddSurfaceModel::AddSurfaceModel()
			throw()
			: BaseModelProcessor(),
				UnaryProcessor<BALL::Composite*>(),
				get_composite_(true),
				start_composite_(0)
		{
		}

		AddSurfaceModel::AddSurfaceModel(const AddSurfaceModel& add_surface, bool deep)
			throw()
			:	BaseModelProcessor(add_surface, deep),
				UnaryProcessor<BALL::Composite*>(add_surface),
				get_composite_(true),
				start_composite_(0)
		{
		}

		AddSurfaceModel::~AddSurfaceModel()
			throw()
		{
			#ifdef BALL_VIEW_DEBUG
				Log.error() << "Destructing object " << (void *)this 
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
			atoms_.clear();
			return BaseModelProcessor::start();
		}
				
		bool AddSurfaceModel::finish()
		{
			// insert surface only if a composite exist
			if (start_composite_ == 0)
			{
				return false;
			}
			
			Mesh* mesh = createMesh_();

			if (mesh == 0)
			{
				throw Exception::OutOfMemory(__FILE__, __LINE__, sizeof(Mesh));
			}

			// get info from the start composite
			MolecularInformation molecular_information;
			start_composite_->host(molecular_information);

			mesh->PropertyManager::set(*this);

			SurfaceProcessor sp;

			if (hasProperty(GeometricObject::PROPERTY__DRAWING_PRECISION_LOW))
			{
				sp.setDensity(2.0);
			}
			else if (hasProperty(GeometricObject::PROPERTY__DRAWING_PRECISION_MEDIUM))
			{
				sp.setDensity(4.5);
			}
			else if (hasProperty(GeometricObject::PROPERTY__DRAWING_PRECISION_HIGH))
			{
				sp.setDensity(7.0);
			}
			else 
			{
				Log.error() << "Unknown precision in " << __FILE__ << "   " << __LINE__ << std::endl;
			}


			// Initialize the surface processor
			sp.start();
			try 
			{
				// Since we have container of *pointers* only, we have to 
				// do the iteration on our own.
				HashSet<Atom*>::Iterator it(atoms_.begin());
				for (; +it; ++it)
				{
					if (sp.operator () (**it) != Processor::CONTINUE)
					{
						break;
					}
				}
			}
			catch (Exception::GeneralException e)
			{
				Log.error() << "SurfaceModel: caught exception while calculating molecular surface: " << e << endl;
			}
			catch (std::exception e)
			{
				Log.error() << "SurfaceModel: caught exception while calculating molecular surface: " << e.what() << endl;
			}
			catch (...)
			{
				Log.error() << "SurfaceModel: caught unknown exception while calculating molecular surface" << endl;
			}
			Log.info() << "assigning surface (" << sp.getSurface().vertex.size() << " vertices, " 
								 << sp.getSurface().triangle.size() << " triangles)" << endl;
			
			// Compute the surface
			sp.finish();
			*static_cast<Surface*>(mesh) = sp.getSurface();
		
			mesh->setName(String("Surface of ")
										+ molecular_information.getTypeName() 
										+ String(" (")
										+ molecular_information.getName()
										+ String(")"));


			start_composite_->getRoot().appendChild(*mesh);
			return true;
		}
				
		Processor::Result AddSurfaceModel::operator () (Composite& composite)
		{
			// take first composite, surface will be inserted to it later
			if (get_composite_)
			{
				start_composite_ = &composite;
				get_composite_ = false;
			}
			if (RTTI::isKindOf<AtomContainer>(composite)) 
			{
				AtomIterator it;
				BALL_FOREACH_ATOM(*(AtomContainer*) &composite, it)
				{
					atoms_.insert((Atom*) &*it);
				}
				return Processor::CONTINUE;
			}
			
			if (!RTTI::isKindOf<Atom>(composite)) return Processor::CONTINUE;

			atoms_.insert((Atom*) &composite);

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

	} // namespace MOLVIEW

} // namespace BALL
