// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: surfaceModel.C,v 1.6 2003/10/27 16:55:45 amoll Exp $
//

#include <BALL/VIEW/MODELS/surfaceModel.h>
#include <BALL/VIEW/PRIMITIVES/mesh.h>
#include <BALL/VIEW/KERNEL/molecularInformation.h>
#include <BALL/VIEW/KERNEL/representation.h>
#include <BALL/VIEW/KERNEL/common.h>
#include <BALL/KERNEL/atomContainer.h>
#include <BALL/KERNEL/forEach.h>

using namespace std;

namespace BALL
{
	namespace VIEW
	{

		AddSurfaceModel::AddSurfaceModel()
			throw()
			: ModelProcessor(),
				get_composite_(true),
				start_composite_(0),
				type_(SurfaceProcessor::SOLVENT_EXCLUDED_SURFACE)
		{
		}

		AddSurfaceModel::AddSurfaceModel(const AddSurfaceModel& add_surface)
			throw()
			:	ModelProcessor(add_surface),
				get_composite_(true),
				start_composite_(0),
				type_(add_surface.type_)
		{
		}

		AddSurfaceModel::~AddSurfaceModel()
			throw()
		{
			#ifdef BALL_VIEW_DEBUG
				Log.info() << "Destructing object " << (void *)this 
										<< " of class " << RTTI::getName<AddSurfaceModel>() << std::endl;
			#endif 
		}

		void AddSurfaceModel::clear()
			throw()
		{
			ModelProcessor::clear();
			get_composite_ = true;
			start_composite_ = 0;
			type_ = SurfaceProcessor::SOLVENT_EXCLUDED_SURFACE;
		}

		bool AddSurfaceModel::start()
		{
			get_composite_ = true;
			start_composite_ = 0;
			return ModelProcessor::start();
		}
				
		bool AddSurfaceModel::finish()
		{
			// return if no composite found
			if (start_composite_ == 0)  return false;
			
			Mesh* mesh = new Mesh;

			if (mesh == 0) throw Exception::OutOfMemory(__FILE__, __LINE__, sizeof(Mesh));

			// get info from the start composite
			MolecularInformation molecular_information;
			start_composite_->host(molecular_information);

			SurfaceProcessor sp;
			sp.setType(getType());

			if (getSurfaceDrawingPrecision() != -1)
			{
				sp.setDensity(getSurfaceDrawingPrecision());
			}
			else
			{
				switch (getDrawingPrecision())
				{
					case VIEW::DRAWING_PRECISION_LOW:
						sp.setDensity(1.5);
						break;

					case VIEW::DRAWING_PRECISION_MEDIUM:
						sp.setDensity(3.5);
						break;

					case VIEW::DRAWING_PRECISION_HIGH:
						sp.setDensity(6.0);
						break;

					default:
						Log.error() << "Unknown precision in " << __FILE__ << "   " << __LINE__ << std::endl;
				}
			}

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
				Log.error() << "SurfaceModel: caught exception while calculating molecular surface "<<endl;// << e << endl;
			}
			catch (std::exception e)
			{
				Log.error() << "SurfaceModel: caught exception while calculating molecular surface: " << e.what() << endl;
			}
			catch (...)
			{
				Log.error() << "SurfaceModel: caught unknown exception while calculating molecular surface" << endl;
			}
			
			// Compute the surface
			sp.finish();
			*static_cast<Surface*>(mesh) = sp.getSurface();
		
			mesh->setName(String("Surface of ")
										+ molecular_information.getTypeName() 
										+ String(" (")
										+ molecular_information.getName()
										+ String(")"));


			geometric_objects_.push_back(mesh);
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

			ModelProcessor::dump(s, depth + 1);

			BALL_DUMP_STREAM_SUFFIX(s);
		}

	} // namespace VIEW

} // namespace BALL
