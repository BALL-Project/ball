// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/STRUCTURE/numericalSAS.h>
#include <BALL/STRUCTURE/triangulatedSurface.h>
#include <BALL/STRUCTURE/geometricProperties.h>
#include <BALL/KERNEL/atom.h>
#include <BALL/DATATYPE/hashMap.h>
#include <BALL/DATATYPE/hashGrid.h>
#include <BALL/KERNEL/atomContainer.h>
#include <BALL/MATHS/surface.h>

namespace BALL
{
	const String NumericalSAS::Option::COMPUTE_AREA      					= "compute_area";
	const String NumericalSAS::Option::COMPUTE_VOLUME    					= "compute_volume";
	const String NumericalSAS::Option::COMPUTE_SURFACE   					= "compute_surface";
	const String NumericalSAS::Option::COMPUTE_SURFACE_PER_ATOM   = "compute_surface_per_atom";
	const String NumericalSAS::Option::COMPUTE_SURFACE_MAP				= "compute_surface_map";
	const String NumericalSAS::Option::NUMBER_OF_POINTS  					= "number_of_points";
	const String NumericalSAS::Option::PROBE_RADIUS      					= "probe_radius";

	const bool   NumericalSAS::Default::COMPUTE_AREA     					= true;
	const bool   NumericalSAS::Default::COMPUTE_VOLUME   					= true;
	const bool   NumericalSAS::Default::COMPUTE_SURFACE  					= false;
	const bool   NumericalSAS::Default::COMPUTE_SURFACE_PER_ATOM  = false;
	const bool   NumericalSAS::Default::COMPUTE_SURFACE_MAP			  = false;
	const Size   NumericalSAS::Default::NUMBER_OF_POINTS 					= 400;
	const float  NumericalSAS::Default::PROBE_RADIUS     					= 1.5;

	NumericalSAS::NumericalSAS()
		: total_area_(0.)
	{
		setDefaultOptions_();
	}

	NumericalSAS::NumericalSAS(const Options& options)
		:	options(options),
			total_area_(0.)
	{
		setDefaultOptions_();
	}

	NumericalSAS::~NumericalSAS()
	{
	}

	void NumericalSAS::operator() (const AtomContainer& fragment)
	{
		fragment_ = &fragment;

		atom_areas_.clear();
		total_area_ = 0.;

		atom_volumes_.clear();
		total_volume_ = 0.;

		surface_.clear();
		atom_surfaces_.clear();

		atom_surface_map_.clear();
		
		bool compute_area    					= options.getBool(Option::COMPUTE_AREA   					);
		bool compute_volume  					= options.getBool(Option::COMPUTE_VOLUME 					);
		bool compute_surface			 		= options.getBool(Option::COMPUTE_SURFACE					);
		bool compute_surface_per_atom = options.getBool(Option::COMPUTE_SURFACE_PER_ATOM);
		bool compute_surface_map			= options.getBool(Option::COMPUTE_SURFACE_MAP     );

		Size num_points_requested = options.getInteger(Option::NUMBER_OF_POINTS);
		float probe_radius = options.getReal(Option::PROBE_RADIUS);

		// precompute a triangulated sphere
		TriangulatedSphere sphere_template_t;
		Size num_points = computeSphereTesselation_(sphere_template_t, num_points_requested);		

		float unit_area_per_point = 4.*M_PI/num_points;
		float unit_volume         = 4.*M_PI/(3.*num_points);

		// it's simpler to work with surfaces later
		Surface sphere_template;
		sphere_template_t.exportSurface(sphere_template);

		// a safety threshold
		float epsilon = 0.5;

		// determine the maximum SAS radius
		float max_radius = 0;
		for (AtomConstIterator at_it = fragment.beginAtom(); +at_it; ++at_it)
		{
			max_radius = std::max(max_radius, at_it->getRadius());
		}
		max_radius += probe_radius;

		// find the center of gravity
		GeometricCenterProcessor gcp;

		// ugly, but necessary; and actually not a problem, since
		// the geometric center processor does *really* change nothing
		const_cast<AtomContainer&>(fragment).apply(gcp);

		Vector3& center_of_gravity = gcp.getCenter();

		// build the containing box
		BoundingBoxProcessor bpp;

		// ugly, but necessary; and actually not a problem, since
		// the bounding box processor does *really* change nothing
		const_cast<AtomContainer&>(fragment).apply(bpp);

		// and a hash grid containing all atoms
		Vector3 grid_origin = bpp.getLower() - Vector3(max_radius + epsilon);
		HashGrid3<Atom const*> atom_grid(grid_origin, bpp.getUpper() - grid_origin + Vector3(max_radius + epsilon), 2 * max_radius + epsilon);

		for (AtomConstIterator at_it = fragment.beginAtom(); +at_it; ++at_it)
		{
			if (at_it->getRadius() > 0.001)
			{
				atom_grid.insert(at_it->getPosition(), &(*at_it));
			}
		}

		// now iterate over all atoms and determine their possibly occluding neighbours
		for (AtomConstIterator at_it = fragment.beginAtom(); +at_it; ++at_it)
		{
			if (at_it->getRadius() <= 0.001)
			{
				continue;
			}

			Vector3 current_center = at_it->getPosition();
			float   current_radius = at_it->getRadius()+probe_radius;

			if (current_radius == probe_radius)
				continue;

			if (compute_surface_map)
				atom_surface_map_.push_back(std::pair<Vector3, Surface>(at_it->getPosition(), Surface()));

			std::vector<Atom const*> neighbours;

			// find the atom's box
			HashGridBox3<Atom const*>* box = atom_grid.getBox(at_it->getPosition());
			if(!box)
			{
				throw BALL::Exception::GeneralException(__FILE__, __LINE__, "NumericalSAS error", "Cannot find atom in hashgrid!");
			}

			// and iterate over all boxes in the neighbourhood, including the box itself
			HashGridBox3<Atom const*>::BoxIterator neighbour_box = box->beginBox();

			for (; +neighbour_box; ++neighbour_box)
			{
				// iterate over all atoms of the current neighbouring box
				HashGridBox3<Atom const*>::DataIterator data_it;
				for (data_it = neighbour_box->beginData(); +data_it; ++data_it)
				{
					if (*data_it == &*at_it)
						continue;

					// do the atoms overlap at all?
					float radius_sum = current_radius + (*data_it)->getRadius() + probe_radius;
					if ((current_center-(*data_it)->getPosition()).getSquareLength() <= radius_sum*radius_sum)
						neighbours.push_back(*data_it);
				}
			} // end loop over neighbour boxes

			Size num_occluded=0;
			Vector3 dr(0.); // volume element

			// now we know all the potentially occluding atoms => test each point for overlap
			for (Size current_point_index=0; current_point_index<num_points; ++current_point_index)
			{
				bool is_occluded = false;
				Vector3 current_point = sphere_template.vertex[current_point_index]*current_radius + current_center;

				for (Size current_neighbour=0; current_neighbour<neighbours.size(); ++current_neighbour)
				{
					float partner_radius = neighbours[current_neighbour]->getRadius() + probe_radius;
					if ((current_point-neighbours[current_neighbour]->getPosition()).getSquareLength() <= partner_radius*partner_radius)
					{
						++num_occluded;
						is_occluded = true;
						break;
					}
				}

				if (!is_occluded)
				{
					if (compute_volume)
					{
						dr += sphere_template.vertex[current_point_index];
					}

					if (compute_surface)
					{
						surface_.vertex.push_back(current_point);
						surface_.normal.push_back(sphere_template.vertex[current_point_index]*current_radius*current_radius*unit_area_per_point);
					}

					if (compute_surface_per_atom)
					{
						Surface& current_surface = atom_surfaces_[&*at_it];
						current_surface.vertex.push_back(current_point);
						current_surface.normal.push_back(sphere_template.vertex[current_point_index]);
					}

					if (compute_surface_map)
					{
						Surface& current_surface = (--atom_surface_map_.end())->second;
						current_surface.vertex.push_back(current_point);
						current_surface.normal.push_back(sphere_template.vertex[current_point_index]);
					}
				}
			}

			if (compute_area)
			{
				float atom_area = current_radius*current_radius * unit_area_per_point * (num_points - num_occluded);
				total_area_ += atom_area;

				atom_areas_[&*at_it] = atom_area;
			}

			if (compute_volume)
			{
				float atom_volume = current_radius * current_radius * unit_volume
																					 * (  (current_center-center_of_gravity)*dr 
																							 + current_radius * (num_points - num_occluded));
				total_volume_ += atom_volume;
			}

			if (compute_surface_per_atom)
			{
				float length = current_radius*current_radius * unit_area_per_point;
				Surface& current_surface = atom_surfaces_[&*at_it];
				for (Position i=0; i<current_surface.normal.size(); ++i)
					current_surface.normal[i] *= length;
			}

			if (compute_surface_map)
			{
				float length = current_radius*current_radius * unit_area_per_point;
				Surface& current_surface = (--atom_surface_map_.end())->second;
				for (Position i=0; i<current_surface.normal.size(); ++i)
					current_surface.normal[i] *= length;
			}
		}
	}

	void NumericalSAS::setDefaultOptions_()
	{
		options.setDefault(Option::COMPUTE_AREA,     					Default::COMPUTE_AREA);
		options.setDefault(Option::COMPUTE_VOLUME,  					Default::COMPUTE_VOLUME);
		options.setDefault(Option::COMPUTE_SURFACE,  					Default::COMPUTE_SURFACE);
		options.setDefault(Option::COMPUTE_SURFACE_PER_ATOM,  Default::COMPUTE_SURFACE_PER_ATOM);
		options.setDefault(Option::COMPUTE_SURFACE_MAP,  			Default::COMPUTE_SURFACE_MAP);

		options.setDefault(Option::NUMBER_OF_POINTS, 					Default::NUMBER_OF_POINTS);
		options.setDefault(Option::PROBE_RADIUS,     					Default::PROBE_RADIUS);
	}

	Size NumericalSAS::computeSphereTesselation_(TriangulatedSphere& result, int num_points)
	{
		// first, we decide whether to use an icosahedron or a pentakis dodecahedron
		// for the tesselation; this only depends on which comes closer to the number
		// of requested points
		Size levels_icosahedron  					= (Size)ceil(log((float)(num_points - 2)/10.)/log(4.f));
		Size levels_pentakis_dodecahedron = (Size)ceil(log((float)(num_points - 2)/30.)/log(4.f));

		Size num_points_icosahedron 					= (Size)(10*pow(4.f, (int)levels_icosahedron          )+2);
		Size num_points_pentakis_dodecahedron = (Size)(30*pow(4.f, (int)levels_pentakis_dodecahedron)+2);

		// both numbers are >= num_points -> take the smaller one
		bool use_icosahedron = num_points_icosahedron < num_points_pentakis_dodecahedron;

		Size result_size;
		if (use_icosahedron)
		{
			result.icosaeder();
			if (levels_icosahedron > 0)
				result.refine(levels_icosahedron);

			result_size = num_points_icosahedron;
		}
		else
		{
			result.pentakisDodecaeder();
			if (levels_pentakis_dodecahedron > 0)
				result.refine(levels_pentakis_dodecahedron);

			result_size = num_points_pentakis_dodecahedron;
		}

		return result_size;
	}	

} // namespace BALL
