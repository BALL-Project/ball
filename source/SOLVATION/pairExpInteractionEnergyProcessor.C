// $Id: pairExpInteractionEnergyProcessor.C,v 1.4 2000/09/21 13:35:13 anker Exp $

#include <BALL/KERNEL/PTE.h>
#include <BALL/MATHS/surface.h>
#include <BALL/MOLMEC/PARAMETER/forceFieldParameters.h>
#include <BALL/MOLMEC/PARAMETER/lennardJones.h>
#include <BALL/STRUCTURE/numericalSAS.h>

#include <BALL/SOLVATION/pairExpInteractionEnergyProcessor.h>
#include <BALL/SOLVATION/solventParameter.h>
#include <BALL/SOLVATION/claverieParameter.h>

using namespace std;

namespace BALL
{

	const char* PairExpInteractionEnergyProcessor::Option::VERBOSITY 
		= "verbosity";
	const char* PairExpInteractionEnergyProcessor::Option::ALPHA
		= "alpha";
	const char* PairExpInteractionEnergyProcessor::Option::C1
		= "c1";
	const char* PairExpInteractionEnergyProcessor::Option::C2
		= "c2";
	const char* PairExpInteractionEnergyProcessor::Option::CLAVERIE_FILENAME
		= "claverie_filename";
	const char* PairExpInteractionEnergyProcessor::Option::USE_RDF
		= "use_rdf";
	const char* PairExpInteractionEnergyProcessor::Option::RDF_FILENAME
		= "rdf_filename";
	const char* PairExpInteractionEnergyProcessor::Option::SOLVENT_FILENAME
		= "solvent_filename";
	const char* PairExpInteractionEnergyProcessor::Option::SOLVENT_NUMBER_DENSITY 
		= "solvent_number_density";
	const char* PairExpInteractionEnergyProcessor::Option::RADIUS_RULE_FILE
		= "radius_rule_file";
	const char* PairExpInteractionEnergyProcessor::Option::SURFACE_TYPE
		= "surface_type";
	const char* PairExpInteractionEnergyProcessor::Option::SURFACE_FILENAME
		= "surface_filename";

	const int PairExpInteractionEnergyProcessor::Default::VERBOSITY = 1;
	const double PairExpInteractionEnergyProcessor::Default::ALPHA 
		= 12.35;
	const double PairExpInteractionEnergyProcessor::Default::C1
		= 4.7e4;
	const double PairExpInteractionEnergyProcessor::Default::C2
		= 0.214;
	const char* PairExpInteractionEnergyProcessor::Default::CLAVERIE_FILENAME
		= "claverie.ini";
	const bool PairExpInteractionEnergyProcessor::Default::USE_RDF = true;
	const char* PairExpInteractionEnergyProcessor::Default::RDF_FILENAME
		= "rdf.ini";
	const char* PairExpInteractionEnergyProcessor::Default::SOLVENT_FILENAME
		= "solvent.ini";
	const float PairExpInteractionEnergyProcessor::Default::SOLVENT_NUMBER_DENSITY 
		= 3.33253e-2;
	const char* PairExpInteractionEnergyProcessor::Default::RADIUS_RULE_FILE
		= "radius.rul";
	const int PairExpInteractionEnergyProcessor::Default::SURFACE_TYPE
		= SURFACE__SAS;
	const char* PairExpInteractionEnergyProcessor::Default::SURFACE_FILENAME
		= "surface.surf";

	PairExpInteractionEnergyProcessor::PairExpInteractionEnergyProcessor()
		:	EnergyProcessor(),
			options(),
			solvent_(),
			alpha_(0),
			C1_(0),
			C2_(0)
	{
		options.setDefaultInteger(Option::VERBOSITY, Default::VERBOSITY);
		options.setDefaultReal(Option::ALPHA, Default::ALPHA);
		options.setDefaultReal(Option::C1, Default::C1);
		options.setDefaultReal(Option::C2, Default::C2);
		options.setDefault(Option::CLAVERIE_FILENAME, Default::CLAVERIE_FILENAME);
		options.setDefaultBool(Option::USE_RDF, Default::USE_RDF);
		options.setDefault(Option::RDF_FILENAME, Default::RDF_FILENAME);
		options.setDefault(Option::SOLVENT_FILENAME, Default::SOLVENT_FILENAME);
		options.setDefaultReal(Option::SOLVENT_NUMBER_DENSITY, 
				Default::SOLVENT_NUMBER_DENSITY);
		options.setDefault(Option::RADIUS_RULE_FILE,
				Default::RADIUS_RULE_FILE);
		options.setDefault(Option::SURFACE_TYPE, Default::SURFACE_TYPE);
		options.setDefault(Option::SURFACE_FILENAME, Default::SURFACE_FILENAME);
	}

	
	PairExpInteractionEnergyProcessor::PairExpInteractionEnergyProcessor(const
			PairExpInteractionEnergyProcessor& proc)
		:	EnergyProcessor(proc),
			options(proc.options),
			solvent_(proc.solvent_),
			alpha_(proc.alpha_),
			C1_(proc.C1_),
			C2_(proc.C2_)
	{
	}

	
	PairExpInteractionEnergyProcessor::~PairExpInteractionEnergyProcessor()
	{
		destroy();
	}

	
	void PairExpInteractionEnergyProcessor::destroy()
	{
		clear();
	}

	
	void PairExpInteractionEnergyProcessor::clear()
	{
		options.clear();
		solvent_.clear();
		alpha_ = 0.0;
		C1_ = 0.0;
		C2_ = 0.0;
	}


	void PairExpInteractionEnergyProcessor::set(const
			PairExpInteractionEnergyProcessor& proc)
	{
		options = proc.options;
		solvent_ = proc.solvent_;
		alpha_ = proc.alpha_;
		C1_ = proc.C1_;
		C2_ = proc.C2_;
	}


	const PairExpInteractionEnergyProcessor&
		PairExpInteractionEnergyProcessor::operator = 
			(const PairExpInteractionEnergyProcessor& proc)
	{
		set(proc);
		return *this;
	}


	bool PairExpInteractionEnergyProcessor::finish()
	{

		// first check for user settings

		int verbosity = (int) options.getInteger(Option::VERBOSITY);
		// this is the flag stating whether the rdf information should be used
		// BAUSTELLE: Options oder Memebrs aber nicht beides...
		alpha_ = options.getReal(Option::ALPHA);
		C1_ = options.getReal(Option::C1);
		C2_ = options.getReal(Option::C2);
		String claverie_filename = options.get(Option::CLAVERIE_FILENAME);
		bool use_rdf = options.getBool(Option::USE_RDF);
		// BAUSTELLE
		use_rdf = true;
		// the file containing the rdf descriptions
		String rdf_filename = options.get(Option::RDF_FILENAME);
		// the file contacining the solvent description
		String solvent_filename = options.get(Option::SOLVENT_FILENAME);
		// rho is the number density of the solvent (i. e. water) [1/m^3]
		double rho = options.getReal(Option::SOLVENT_NUMBER_DENSITY);
		int surface_type = options.getInteger(Option::SURFACE_TYPE);
		String surface_filename = options.get(Option::SURFACE_FILENAME);

		// vdW-radius of a solvent atom of type i
		double R_s = 0.0; // [ A ]
		// vdW-radius of a solute atom
		double R_j = 0.0; // [ A ]
		// energy contributions of one triangle
		double e_ij = 0.0; // [ ? ]
		// energy contributions of one solvent atom type
		double E_i = 0.0;
		// total dispersive and repulsive energy
		double E = 0.0;
		double E_D = 0.0;
		double E_R = 0.0;
		double E_ij = 0.0;
		double E_ij_D = 0.0;
		double E_ij_R = 0.0;
		double e_ij_D = 0.0;
		double e_ij_R = 0.0;


		// a frequently used term (4*sqrt(R_i*R_j))
		double R_ij_o; // [ m ]
		// the same to the power of 6
		double R_ij_o_6; // [ m^6 ]
		// Claverie calibration constants for dispersion and repulsion, dependant
		// on solvent atom type j and solute atom type i
		double K_ij_D; // [ ? ]
		double K_ij_R; // [ ? ]
		// integral parts of the dispersion and repulsion energies
		double I_disp; // [ ? ]
		double I_rep; // [ ? ]
		// frequently used term (alpha_/R_ij_o)
		double a; // [ ? ]
		double a_r_k;
	
		// Geometry
		Vector3 r_k_vec;
		Vector3 n_k_vec;
		double r_k;
		Vector3 atom_center;
		Vector3 sphere_center;
		Surface* current_surface;
		char tmp[100];

		//
		std::pair<float, float> claverie;

		// define the solvent
		ForceFieldParameters ffparam(solvent_filename);
		SolventParameter solvent_parameter_section;
		solvent_parameter_section.extractSection(ffparam, "SolventDescription");
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
		Atom::Type type_i;
		Atom::Type type_j;

		ForceFieldParameters fffparam(claverie_filename);
		ClaverieParameter claverie_param;
		Log.info() << "claverie_filename = " << claverie_filename << endl;
		claverie_param.extractSection(fffparam, "ClaverieParameters");

		// iterate over all different atom types in the solvent

		for (Size s = 0; s < solvent_descriptor.getNumberOfAtomTypes(); ++s)
		{

			SolventAtomDescriptor solvent_atom 
				= solvent_descriptor.getAtomDescriptor(s);
			type_i = solvent_atom.type;
			R_s = solvent_atom.radius;
			// Log.info() << "Radius of Solvent: " << R_s << endl;

			// now compute the surface for the integration

			vector< pair<Vector3, Surface> > surface_map;
			String filename;
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

			E_ij = 0.0;
			E_ij_D = 0.0;
			E_ij_R = 0.0;

			// iterate over all atoms of the solute
			AtomIterator solute_iterator;
			for (solute_iterator = fragment_->beginAtom(); +solute_iterator;
					++solute_iterator)
			{
				// BAUSTELLE: Warum geht das net?
				// type_j = solute_iterator->getType();
				type_j = ffparam.getAtomTypes().getType(solute_iterator->getTypeName());
				atom_center = solute_iterator->getPosition();
				R_j = solute_iterator->getRadius();
				// Log.info() << "Radius of Solute: " << R_j << endl;

				// compute the necessary pair potential parameters

				if (claverie_param.hasParameters(type_i, type_j))
				{
					claverie = claverie_param.getParameters(type_i, type_j);
				}
				else
				{
					Log.error() << "Cannot assign Claverie parameters."<< endl;
				}

				K_ij_D = claverie.first;
				K_ij_R = claverie.first;
				R_ij_o = claverie.second;

				R_ij_o_6 = pow(R_ij_o, 6);
				a = alpha_ / R_ij_o;

				// iterate over all surface points

				I_disp = I_rep = 0.0;
				e_ij = e_ij_D = e_ij_R = 0.0;

				for (Size sphere_index = 0; sphere_index < surface_map.size(); ++sphere_index)
				{
					sphere_center = surface_map[sphere_index].first;
					Surface &current_surface = surface_map[sphere_index].second;
					for (Size k = 0; k < current_surface.vertex.size(); ++k)
					{

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
							/*
							Log.info() << "sphere_center = " << sphere_center << endl;
							Log.info() << "atom_center = " << atom_center << endl;
							Log.info() << "A = " << A << endl;
							Log.info() << "B = " << B << endl;
							Log.info() << "C = " << C << endl;
							Log.info() << "k1 = " << k1 << ", k2 = " << k2 << endl;
							Log.info() << "rho = " << rho << endl;
							Log.info() << "r_k = " << r_k << endl;
							Log.info() << "r_k_vec * n_k_vec = " << r_k_vec * n_k_vec << endl;
							*/

							// BAUSTELLE: Sollte protected werden. Und nicht geteilt in
							// zwei Beiträge
							PairExpRDFIntegrator integrator(alpha_, C1_, C2_, R_ij_o, 
									k1, k2, rdf_parameter.getRDF(type_i, type_j));
							e_ij += rho * integrator.integrateToInf(r_k)
								* ((r_k_vec * n_k_vec)) / (r_k * r_k * r_k);

							// DEBUG
							/*
							e_ij_D += rho * integrator.integrateToInf(r_k, alpha_, 0.0,
									C2_, R_ij_o, k1, k2) 
								* ((r_k_vec * n_k_vec)) / (r_k * r_k * r_k);
							e_ij_R += rho * integrator.integrateToInf(r_k, alpha_, C1_,
									0.0, R_ij_o, k1, k2) 
								* ((r_k_vec * n_k_vec)) / (r_k * r_k * r_k);

							a_r_k = a * r_k;
							I_disp += (r_k_vec * n_k_vec) / (3.0 * pow(r_k,6));

							I_rep += (r_k_vec * n_k_vec) * (exp(-a * r_k) *
									((1.0 / a_r_k) + (2.0 / (a_r_k * a_r_k)) + 
									 (2.0 / (a_r_k * a_r_k * a_r_k))));

							Log.info() << "e_ij = " << e_ij << endl;
							Log.info() << "Dis: " << e_ij_D << " " 
								<< rho * - C2_ * K_ij_D * R_ij_o_6 * I_disp << endl;
							Log.info() << "Rep: " << e_ij_R << " " 
								<< rho * C1_ * K_ij_R * I_rep << endl;
							*/

						}
						else
						{

							a_r_k = a * r_k;

							I_disp += (r_k_vec * n_k_vec) / (3.0 * pow(r_k,6));

							I_rep += (r_k_vec * n_k_vec) * (exp(-a * r_k) *
									((1.0 / a_r_k) + (2.0 / (a_r_k * a_r_k)) + 
									 (2.0 / (a_r_k * a_r_k * a_r_k))));

							// BAUSTELLE
							e_ij_R = rho * C1_ * K_ij_R * I_rep;
							e_ij_D = rho * - C2_ * K_ij_D * R_ij_o_6 * I_disp;

							e_ij = rho * ( C1_ * K_ij_R * I_rep + C2_ * K_ij_D * R_ij_o_6
									* I_disp);
						}
					} // current surface
				} // current sphere

				// E_ij_x is the contribution of the combination of solvent atom
				// type i and solute atom type j
				E_ij += e_ij;
				E_ij_D += e_ij_D;
				E_ij_R += e_ij_R;

			} // solute


			// E_x is the total energy contribution
			E += solvent_atom.number_of_atoms * E_ij;
			E_D += solvent_atom.number_of_atoms * E_ij_D;
			E_R += solvent_atom.number_of_atoms * E_ij_R;

		} // solvent

		if (verbosity > 0)
		{
			Log.info() << "Dispersion energy: " << E_D << " kcal/mol\t"
				<< "Repulsion energy: " << E_R << " kcal/mol" << endl;
		}

		// energy_ = 4184 * (E_R + E_D);
		energy_ = 4184 * E;
		return true;
	}

	// BAUSTELLE: sollte nicht hier stehen.
	void PairExpInteractionEnergyProcessor::getExternalSurface_(
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
			// DEBUG
			// Log.info() << "tag = " << tag << ", sphere = " << sphere_center << endl;
			if (tag != "Sphere")
			{
				Log.error() << "Sphere expected" << endl;
				// surface_map.clear();
				return;
			}
			ifs >> tag >> number_of_tesserae;
			// DEBUG
			// Log.info() << "tag = " << tag << ", not = " << number_of_tesserae << endl;
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
				// DEBUG
				// Log.info() << "vertex = " << vertex << ", area = " << area << endl;
				surface.vertex[i] = vertex;
				normal = area * (vertex-sphere_center).normalize();
				surface.normal[i] = normal;
			}
			surface_map.push_back(pair<Vector3, Surface>(sphere_center, surface));
		}
		ifs.close();
	}

} // namespace BALL
