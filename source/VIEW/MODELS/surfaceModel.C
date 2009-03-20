// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: surfaceModel.C,v 1.13.18.2 2007/04/12 12:04:06 amoll Exp $
//

#include <BALL/VIEW/MODELS/surfaceModel.h>
#include <BALL/VIEW/PRIMITIVES/mesh.h>
#include <BALL/VIEW/KERNEL/common.h>
#include <BALL/KERNEL/atomContainer.h>
#include <BALL/KERNEL/forEach.h>

using namespace std;

namespace BALL
{
	namespace VIEW
	{

		AddSurfaceModel::AddSurfaceModel()
			: ModelProcessor(),
				type_(SurfaceProcessor::SOLVENT_EXCLUDED_SURFACE),
				probe_radius_(1.5)
		{
		}

		AddSurfaceModel::AddSurfaceModel(const AddSurfaceModel& add_surface)
			:	ModelProcessor(add_surface),
				type_(add_surface.type_),
				probe_radius_(add_surface.probe_radius_)
		{
		}

		AddSurfaceModel::~AddSurfaceModel()
		{
			#ifdef BALL_VIEW_DEBUG
				Log.info() << "Destructing object " << (void *)this 
										<< " of class " << RTTI::getName<AddSurfaceModel>() << std::endl;
			#endif 
		}

		void AddSurfaceModel::clear()
		{
			ModelProcessor::clear();
			type_ = SurfaceProcessor::SOLVENT_EXCLUDED_SURFACE;
		}

				
		Processor::Result AddSurfaceModel::operator () (Composite& composite)
		{
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
		{
			BALL_DUMP_STREAM_PREFIX(s);
			
			BALL_DUMP_DEPTH(s, depth);
			BALL_DUMP_HEADER(s, this, this);

			ModelProcessor::dump(s, depth + 1);

			BALL_DUMP_STREAM_SUFFIX(s);
		}

		void AddSurfaceModel::clearComposites()
		{
			atoms_.clear();
		}

		bool AddSurfaceModel::createGeometricObjects()
		{
			ModelProcessor::createGeometricObjects();
			Mesh* mesh = new Mesh;

			if (mesh == 0) return false;

			SurfaceProcessor sp;
			sp.setType(getType());
			sp.setProbeRadius(probe_radius_);
			sp.start();

			if (getSurfaceDrawingPrecision() != -1)
			{
				sp.setDensity(getSurfaceDrawingPrecision());
			}
			else
			{
				if (getDrawingPrecision() >= DRAWING_PRECISION_LOW &&
						getDrawingPrecision() <= DRAWING_PRECISION_ULTRA)
				{
					sp.setDensity(SurfaceDrawingPrecisions[getDrawingPrecision()]);
				}
				else
				{
					Log.error() << "Invalid Drawing Precision " << getDrawingPrecision() 
											<< " in " << __FILE__ << " " << __LINE__ << std::endl;
					sp.setDensity(1.5);
				}
			}

			bool ok = true;

			try 
			{
				// Since we have container of *pointers* only, we have to 
				// do the iteration on our own.
				HashSet<Atom*>::Iterator it(atoms_.begin());
				for (; +it; ++it)
				{
					if (sp.operator () (**it) != Processor::CONTINUE)
					{
						ok = false;
						break;
					}
				}
			}
			catch (Exception::GeneralException& e)
			{
				Log.error() << "SurfaceModel: caught exception while calculating molecular surface "<<endl;// << e << endl;
			}
			catch (std::exception& e)
			{
				Log.error() << "SurfaceModel: caught exception while calculating molecular surface: " << e.what() << endl;
			}
			catch (...)
			{
				Log.error() << "SurfaceModel: caught unknown exception while calculating molecular surface" << endl;
			}
			
			// Compute the surface
			ok &= sp.finish();
			if (!ok)
			{
				Log.error() << "Unknown error occured while calculating molecular surface" << std::endl;
			}

			*static_cast<Surface*>(mesh) = sp.getSurface();

			// normalize normal length:
			vector<Vector3>& normals = mesh->normal;
			for (Position p = 0; p < normals.size(); p++)
			{
				float square_length = normals[p].getSquareLength();
				if (square_length == 0) continue;
				normals[p] /= sqrt(square_length);
			}

			geometric_objects_.push_back(mesh);
			return true;
		}

	} // namespace VIEW
} // namespace BALL
