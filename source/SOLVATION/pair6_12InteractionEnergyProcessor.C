// $Id: pair6_12InteractionEnergyProcessor.C,v 1.2 2000/10/05 17:24:40 anker Exp $

#include <BALL/KERNEL/PTE.h>
#include <BALL/MATHS/surface.h>
#include <BALL/MOLMEC/PARAMETER/forceFieldParameters.h>
#include <BALL/MOLMEC/PARAMETER/lennardJones.h>
#include <BALL/STRUCTURE/numericalSAS.h>

#include <BALL/SOLVATION/pair6_12InteractionEnergyProcessor.h>
#include <BALL/SOLVATION/solventParameter.h>


namespace BALL
{

	const char* Pair6_12InteractionEnergyProcessor::Option::VERBOSITY 
		= "verbosity";
	const char* Pair6_12InteractionEnergyProcessor::Option::USE_RDF
		= "use_rdf";
	const char* Pair6_12InteractionEnergyProcessor::Option::RDF_FILENAME
		= "rdf_filename";
	const char* Pair6_12InteractionEnergyProcessor::Option::SOLVENT_FILENAME
		= "solvent_filename";
	const char* Pair6_12InteractionEnergyProcessor::Option::SOLVENT_NUMBER_DENSITY 
		= "solvent_number_density";
	const char* Pair6_12InteractionEnergyProcessor::Option::RADIUS_RULE_FILE
		= "radius_rule_file";
	const char* Pair6_12InteractionEnergyProcessor::Option::SURFACE_TYPE
		= "surface_type";
	const char* Pair6_12InteractionEnergyProcessor::Option::SURFACE_FILENAME
		= "surface_filename";
	

	const int Pair6_12InteractionEnergyProcessor::Default::VERBOSITY = 1;
	const bool Pair6_12InteractionEnergyProcessor::Default::USE_RDF = false;
	const char* Pair6_12InteractionEnergyProcessor::Default::RDF_FILENAME
		= "rdf.ini";
	const char* Pair6_12InteractionEnergyProcessor::Default::SOLVENT_FILENAME
		= "solvent.ini";
	const float Pair6_12InteractionEnergyProcessor::Default::SOLVENT_NUMBER_DENSITY 
		= 3.33253e-2;
	const char* Pair6_12InteractionEnergyProcessor::Default::RADIUS_RULE_FILE
		= "radius.rul";
	const int Pair6_12InteractionEnergyProcessor::Default::SURFACE_TYPE
		= SURFACE__SAS;
	const char* Pair6_12InteractionEnergyProcessor::Default::SURFACE_FILENAME
		= "surface.surf";


	Pair6_12InteractionEnergyProcessor::Pair6_12InteractionEnergyProcessor()
		: EnergyProcessor(),
			solvent_()
	{
		options.setDefaultInteger(Option::VERBOSITY, Default::VERBOSITY);
		options.setDefaultInteger(Option::USE_RDF, Default::USE_RDF);
		options.setDefault(Option::RDF_FILENAME, Default::RDF_FILENAME);
		options.setDefault(Option::SOLVENT_FILENAME, Default::SOLVENT_FILENAME);
		options.setDefaultReal(Option::SOLVENT_NUMBER_DENSITY, 
				Default::SOLVENT_NUMBER_DENSITY);
		options.setDefault(Option::RADIUS_RULE_FILE,
				Default::RADIUS_RULE_FILE);
		options.setDefault(Option::SURFACE_TYPE, Default::SURFACE_TYPE);
		options.setDefault(Option::SURFACE_FILENAME, Default::SURFACE_FILENAME);
	}


	Pair6_12InteractionEnergyProcessor::Pair6_12InteractionEnergyProcessor
		(const Pair6_12InteractionEnergyProcessor& proc)
		: EnergyProcessor(proc),
			solvent_(proc.solvent_)
	{
	}


	Pair6_12InteractionEnergyProcessor::~Pair6_12InteractionEnergyProcessor()
	{
		clear();
		valid_ = false;
	}


	void Pair6_12InteractionEnergyProcessor::clear()
	{
		EnergyProcessor::clear();
		solvent_.clear();
	}


	const Pair6_12InteractionEnergyProcessor&
		Pair6_12InteractionEnergyProcessor::operator =
		(const Pair6_12InteractionEnergyProcessor& proc)
	{
		EnergyProcessor::operator = (proc);
		solvent_ = proc.solvent_;

		return *this;
	}


	bool Pair6_12InteractionEnergyProcessor::finish()
	{

		// first check for user settings

		// how loud will we cry?
		int verbosity = (int) options.getInteger(Option::VERBOSITY);
		// this is the flag stating whether the rdf information should be used
		bool use_rdf = options.getBool(Option::USE_RDF);
		// the file containing the rdf descriptions
		String rdf_filename = options.get(Option::RDF_FILENAME);
		// the file contacining the solvent description
		String solvent_filename = options.get(Option::SOLVENT_FILENAME);
		// rho is the number density of the solvent (i. e. water) [1/m^3]
		double rho = options.getReal(Option::SOLVENT_NUMBER_DENSITY) * 1e30;
		int surface_type = options.getInteger(Option::SURFACE_TYPE);
		String surface_filename = options.get(Option::SURFACE_FILENAME);

		// define the solvent
		ForceFieldParameters ffparam(solvent_filename);
		SolventParameter solvent_parameter_section;
		if (!solvent_parameter_section.extractSection(ffparam,
			"SolventDescription"))
		{
			Log.error() << "Pair6_12InteractionEnergyProcessor::finish(); "
				<< "Cannot read solvent description." << endl;
			return 0.0;
		}
		SolventDescriptor solvent_descriptor 
			= solvent_parameter_section.getSolventDescriptor();
		rho = solvent_descriptor.getNumberDensity();

		// define the rdf, if desired
		ForceFieldParameters rdf_ff_param(rdf_filename);
		RDFParameter rdf_parameter;
		if (use_rdf)
		{
			// BAUSTELLE
			if (!rdf_parameter.extractSection(rdf_ff_param, "RDF"))
			{
				Log.error() << "PairExpInteractionEnergyProcessor::finish(); "
					<< "Cannot read RDF descriptions." << endl;
				return 0.0;
			}
		}

		LennardJones lennard_jones;
		lennard_jones.extractSection(ffparam, "LennardJones");

		// iterate over all different atom types in the solvent

		// vdW-radius of a solvent atom of type s
		double R_s = 0.0; // [ A ]
		// vdW-radius of a solute atom
		double R_m = 0.0; // [ A ]
		// the types of the atoms 
		Atom::Type type_i;
		Atom::Type type_j;
		// the surface description containing surface elements for each atom
		// center
		vector< pair<Vector3, Surface> > surface_map;
		// the filename of the external surface 
		String filename;

		// different energy contributions
		double E;
		double E_D;
		double E_R;
		double E_ij;
		double E_ij_D;
		double E_ij_R;
		double e_ij;
		double e_ij_D;
		double e_ij_R;
		
		// force field parameters
		double A_ij = 0.0;
		double B_ij = 0.0;

		double I_rep = 0.0;
		double I_disp = 0.0;
		double r_k_6;

		// Geometry
		Vector3 r_k_vec;
		Vector3 n_k_vec;
		double r_k;
		Vector3 atom_center;
		Vector3 sphere_center;

		SolventAtomDescriptor solvent_atom;
		AtomIterator solute_iterator;
		LennardJones::Values values;
		Pair6_12RDFIntegrator integrator;

		for (Size s = 0; s < solvent_descriptor.getNumberOfAtomTypes(); ++s)
		{

			solvent_atom = solvent_descriptor.getAtomDescriptor(s);
			type_i = solvent_atom.type;
			R_s = solvent_atom.radius;
			if (verbosity > 2)
			{
				Log.info() << "Radius of Solvent: " << R_s << endl;
			}

			// now compute the surface for the integration

			switch (surface_type)
			{
				case SURFACE__SAS:

					if (verbosity > 0)
					{
						Log.info() << "Using SAS surface" << endl;
					}
					calculateSASAtomPoints(*fragment_, surface_map, R_s);
					break;

				case SURFACE__SES:

					if (verbosity > 0)
					{
						Log.info() << "Using SES surface" << endl;
					}
					// calculateSESAtomPoints(*fragment_, surface_map, R_s);
					break;

				case SURFACE__EXTERNAL:
					
					if (verbosity > 0)
					{
						Log.info() << "Using external surface, filename = ";
					}
					filename = surface_filename + solvent_descriptor.getName()
						+ "-" + solvent_atom.element_symbol + ".surf";
					if (verbosity > 0)
					{
						Log.info() << filename << endl;
					}
					getExternalSurface_(surface_map, filename.c_str());
					break;

				case SURFACE__UNKNOWN:
				default:

					Log.error() << "Pair6_12InteractionEnergyProcessor::finish(): "
						<< "Unknown or unspecified surface type." << endl;
					return false;
			}

			// initialize vars before iteration

			E_ij = 0.0;
			E_ij_D = 0.0;
			E_ij_R = 0.0;

			// iterate over all atoms of the solute

			for (solute_iterator = fragment_->beginAtom(); +solute_iterator;
					++solute_iterator)
			{
				// BAUSTELLE: Warum geht das net?
				// type_j = solute_iterator->getType();
				type_j = ffparam.getAtomTypes().getType(solute_iterator->getTypeName());
				atom_center = solute_iterator->getPosition();
				R_m = solute_iterator->getRadius();
				if (verbosity > 2)
				{
					Log.info() << "Radius of Solute: " << R_m << endl;
				}
				
				// compute the necessary pair potential parameters

				if (verbosity > 0)
				{
					Log.info() << "type i/j " << type_i << "/" << type_j << endl;
				}
			  if (lennard_jones.hasParameters(type_i, type_j))
				{
					values = lennard_jones.getParameters(type_i, type_j);
				}
				else
				{
					Log.error() << "Pair6_12InteractionEnergyProcessor::finish(): "
						<< "Cannot assign force field parameters." << endl;
					return false;
				}

				A_ij = values.A;
				B_ij = values.B;

				// DEBUG
				Log.info() << "A_ij (" << solute_iterator->getElement().getSymbol() <<
					"," << solvent_atom.element_symbol << "): " << A_ij << endl;
				Log.info() << "B_ij (" << solute_iterator->getElement().getSymbol() <<
					"," << solvent_atom.element_symbol << "): " << B_ij << endl;

				// iterate over all surface points

				e_ij = e_ij_D = e_ij_R = 0.0;

				// if the parameters are zero, we dont have to compute the whole
				// term
				// VORSICHT FALLE
				if ((fabs(A_ij) > 1e-6) && (fabs(B_ij) > 1e-6))
				{
					for (Size sphere_index = 0; sphere_index < surface_map.size(); ++sphere_index)
					{
						sphere_center = surface_map[sphere_index].first;
						Surface &current_surface = surface_map[sphere_index].second;
						for (Size k = 0; k < current_surface.vertex.size(); ++k)
						{

							// DEBUG
							Log.info() << "vertex.size() = " << current_surface.vertex.size() << endl;
							// r_k_vec is the vector from the center of the considered atom to
							// the center of the current surface area
							r_k_vec = (current_surface.vertex[k] - atom_center);
							r_k = r_k_vec.getLength();
							// n_k_vec is the normal of the current surface triangle 
							n_k_vec = current_surface.normal[k];

							if (use_rdf)
							{
								float A = (atom_center - sphere_center).getSquareLength();
								float B = (current_surface.vertex[k] - atom_center).getLength();
								float C = 
									(sphere_center - current_surface.vertex[k]).getSquareLength();

								float k1 = (B*B + A - C) / B;
								float k2 = A;

								// DEBUG
								// A_ij = B_ij = rho = 1.0;
								// k1 = k2 = 0.0;

								// DEBUG
								Log.info() << "sphere_center = " << sphere_center << endl;
								Log.info() << "atom_center = " << atom_center << endl;
								Log.info() << "A = " << A << endl;
								Log.info() << "B = " << B << endl;
								Log.info() << "C = " << C << endl;
								Log.info() << "k1 = " << k1 << ", k2 = " << k2 << endl;
								Log.info() << "rho = " << rho << endl;
								Log.info() << "r_k = " << r_k << endl;
								Log.info() << "r_k_vec * n_k_vec = " << r_k_vec * n_k_vec << endl;

								// BAUSTELLE: Sollte protected werden...
								integrator.setConstants(A_ij, B_ij, k1, k2);
								integrator.setRDF(rdf_parameter.getRDF(type_i, type_j));

								e_ij += rho * integrator(r_k)
									* (-(r_k_vec * n_k_vec)) / (r_k * r_k * r_k);

								// DEBUG
								integrator.setConstants(A_ij, 0.0, k1, k2);
								e_ij_R += rho * integrator(r_k)
									* (-(r_k_vec * n_k_vec)) / (r_k * r_k * r_k);
								integrator.setConstants(0.0, B_ij, k1, k2);
								e_ij_D += rho * integrator(r_k)
									* (-(r_k_vec * n_k_vec)) / (r_k * r_k * r_k);
							}
							else
							{
								// integral part of the dispersion energy 

								r_k_6 = pow(r_k, 6);

								I_rep = (r_k_vec * n_k_vec) / (9.0 * r_k_6 * r_k_6);

								// integral part of the repulsion energy 

								I_disp = (r_k_vec * n_k_vec) / (3.0 * r_k_6);

								// BAUSTELLE
								e_ij_R += rho * A_ij * I_rep;
								e_ij_D += - rho * B_ij * I_disp;
								e_ij += rho * (A_ij * I_rep - B_ij * I_disp);

								// DEBUG
								Log.info() << "r_k_vec = " << r_k_vec << endl;
								Log.info() << "r_k = " << r_k << endl;
								Log.info() << "n_k_vec = " << n_k_vec << endl;
								Log.info() << "I_rep = " << I_rep << endl;
								Log.info() << "I_disp = " << I_disp << endl;
								Log.info() << "e_ij = " << e_ij << endl;
								Log.info() << "e_ij_R = " << e_ij_R << endl;
								Log.info() << "e_ij_D = " << e_ij_D << endl;
								


							} 
						} // surface
					} // sphere
				
					// E_ij_x is the contribution of the combination of solvent atom
					// type i and solute atom type j
					E_ij += e_ij;
					E_ij_D += e_ij_D;
					E_ij_R += e_ij_R;

				} // if (A != 0 && B != 0)
				
			} // solute

			// E_x is the total energy contribution
			E += solvent_atom.number_of_atoms * E_ij;

			if (verbosity > 0)
			{
				E_D += solvent_atom.number_of_atoms * E_ij_D;
				E_R += solvent_atom.number_of_atoms * E_ij_R;
			}


		} // solvent

		if (verbosity > 0)
		{
			Log.info() << "Dispersion energy: " << E_D << " kJ/mol\t"
				<< "Repulsion energy: " << E_R << " kJ/mol" << endl;
		}

		energy_ = E;
		return true;
	}


	// BAUSTELLE: sollte nicht hier stehen.
	void Pair6_12InteractionEnergyProcessor::getExternalSurface_(
			vector< pair<Vector3, Surface> >& surface_map, const char* surface_file)
	{
		// HIER WIRD NICHTS, ABER AUCH GAR NICHTS GEPRÜFT!!!
		surface_map.clear();

		String tag;
		Vector3 sphere_center;
		Size number_of_tesserae;
		Vector3 vertex;
		float area;
		Vector3 normal;

		ifstream ifs(surface_file);
		while (ifs.good()) 
		{
			ifs >> tag >> sphere_center;
			if (ifs.bad())
			{
				return;
			}
			Log.info() << "tag = " << tag << ", sphere = " << sphere_center << endl;
			if (tag != "Sphere")
			{
				Log.error() << "Sphere expected" << endl;
				// surface_map.clear();
				return;
			}
			ifs >> tag >> number_of_tesserae;
			Log.info() << "tag = " << tag << ", not = " << number_of_tesserae << endl;
			if (tag != "Tesserae")
			{
				Log.error() << "Tesserae expected" << endl;
				// surface_map.clear();
				return;
			}
			Surface surface;
			surface.vertex.resize(number_of_tesserae);
			surface.normal.resize(number_of_tesserae);
			for (Size i = 0; i < number_of_tesserae; ++i)
			{
				ifs >> vertex >> area;
				Log.info() << "vertex = " << vertex << ", area = " << area << endl;
				surface.vertex[i] = vertex;
				normal = area * (vertex-sphere_center).normalize();
				surface.normal[i] = normal;
			}
			surface_map.push_back(pair<Vector3, Surface>(sphere_center, surface));
		}
		ifs.close();
	}
} // namespace BALL
