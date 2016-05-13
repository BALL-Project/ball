// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/FORMAT/PDBFile.h>
#include <BALL/NMR/empiricalHSShiftProcessor.h>
#include <BALL/KERNEL/bond.h>
#include <BALL/FORMAT/parameterSection.h>
#include <BALL/STRUCTURE/peptides.h>
#include <BALL/KERNEL/PTE.h>
#include <BALL/MATHS/analyticalGeometry.h>
#include <map>
#include <set>
#include <BALL/SYSTEM/path.h>
#include <BALL/MATHS/cubicSpline1D.h>
#include <BALL/MATHS/cubicSpline2D.h>


#define FLOAT_VALUE_NA 3600.
// This value signifies that the evaluation should return 0. This is a very ugly hack,
// but the best way I can think of to prevent spline evaluation for non-existing h-bonds
#define FLOAT_VALUE_IGNORE 3700.
#define STRING_VALUE_NA "BADVAL"
#define CHAR_VALUE_NA  '?'

//#define DEBUG
#undef DEBUG 

using namespace std;

namespace BALL 
{
	const int EmpiricalHSShiftProcessor::VERBOSITY_LEVEL_CRITICAL = 5;
	const int EmpiricalHSShiftProcessor::VERBOSITY_LEVEL_DEBUG = 10;

	const char* EmpiricalHSShiftProcessor::Option::VERBOSITY = "empirical_HS_shift_processor";
	const int EmpiricalHSShiftProcessor::Default::VERBOSITY = EmpiricalHSShiftProcessor::VERBOSITY_LEVEL_CRITICAL;
	
	const char* EmpiricalHSShiftProcessor::PROPERTY__EHS_SHIFT= "EmpiricalHSShift";

	EmpiricalHSShiftProcessor::EmpiricalHSShiftProcessor()
		:	ShiftModule()
	{
		setDefaultOptions();
	}

	EmpiricalHSShiftProcessor::~EmpiricalHSShiftProcessor()
	{
	}
	
	void EmpiricalHSShiftProcessor::init()
	{	
		verbosity_ = options.getInteger(Option::VERBOSITY);
	
		//for TEST ing
		//verbosity_ = 10;

		
		// By default, we assume the worst...
		valid_ = false;

		// If no parameters are assigned, abort immediately.
		if (parameters_ == 0)
		{
			return;
		}

		// Do we have correction factors for SSBonds?
		ParameterSection parameter_section_ssbond;
		parameter_section_ssbond.extractSection(*parameters_, "SSBondCorrection");
		if (   !parameter_section_ssbond.hasVariable("atomtype")
				 ||!parameter_section_ssbond.hasVariable("correction") )
		{
			return;
		}
		
		// Extract the column indices.
		Position atomtype_column =  parameter_section_ssbond.getColumnIndex("atomtype");
		Position correction_column =  parameter_section_ssbond.getColumnIndex("correction");

		int index = -1;
		for (Position counter = 0; counter < parameter_section_ssbond.getNumberOfKeys(); counter++)
		{
			String atomtype   = parameter_section_ssbond.getValue(counter, atomtype_column);
			float  correction = parameter_section_ssbond.getValue(counter, correction_column).toFloat();

			ssbond_correction_[atomtype] = correction;
		}
	
		// Check that the parameter file contains the correct section...
		ParameterSection parameter_section;
		parameter_section.extractSection(*parameters_, "EmpiricalShiftHyperSurfaces");

		// ...and that this section contains the correct column names
		if (   !parameter_section.hasVariable("name") || !parameter_section.hasVariable("property")
				|| !parameter_section.hasVariable("file")  )
		{
			return;
		}
	
		// Check for the options.
		int verbosity = options.getInteger(Option::VERBOSITY);
		if (parameter_section.options.has("verbosity"))
		{
			verbosity = parameter_section.options.getInteger("verbosity");
		}
		options.setInteger(Option::VERBOSITY, verbosity);
		verbosity_ = verbosity;

		exclude_prolins_ = false;
		if (parameter_section.options.has("exclude_prolins"))
		{
			exclude_prolins_ = parameter_section.options.getBool("exclude_prolins");
		}

		// Clear the arrays of the targets, the targets names and the target_properties.
		targets_.clear();
		target_names_.clear();
		target_property_names_.clear();

		
		// Extract the column indices.
		Position name_column =  parameter_section.getColumnIndex("name");
		Position property_column =  parameter_section.getColumnIndex("property");
		Position file_column =  parameter_section.getColumnIndex("file");
		
		
		// Insert per (!) target atom type the necessay properties (in a set)
		// and in a map the property pairs and the corresponding 
		// hypersurface/spline files.
		// 	E.g. an entry in ShiftX.ini could look like this
		// 1						CA          PHI/PSI          	file1
		// so we need to store:
		// - CA as a target name
		// - PHI and PSI as target_property_name__s__ for CA
		// - file1 in a map with the key pair (PHI,PSI) (for CA)
		// - and the pair (PHI, PSI)   (for CA) .
		
		index = -1;
		String old_target_name = "";
		for (Position counter = 0; counter < parameter_section.getNumberOfKeys(); counter++)
		{
			String target_name = parameter_section.getValue(counter, name_column);
			
			// Create a new vector entry for each new target atom.
			if (target_name != old_target_name)
			{
				index +=1;
				old_target_name = target_name;
				// Store the atom type we need to look fro in this processor.
				target_names_.push_back(target_name);

				// Create some dummy entries for 
				// 	- target_property_names_
				// 	- property_files_
				// 	- property_pairs_ .
				std::set< String> empty;
				target_property_names_.push_back(empty);
				std::map< std::pair<String, String>, String> empty2;
				property_files_.push_back(empty2);
				vector<std::pair<String, String> > empty3;
				property_pairs_[target_name] = empty3;
			}
			
			// Split the string at "/" and store the property_names.
			String string = parameter_section.getValue(counter, property_column);
			std::vector<String> props;
			string.split(props, "/");
		
			if (props.size()!=2)
			{
				continue;
			}
			// Store the property names.
			target_property_names_[index].insert(props[0]);
			target_property_names_[index].insert(props[1]);
				
			// Create a key tuple and insert the filename.
			std::pair<String, String> tuple(props[0], props[1]);
			// Store the file name.
			property_files_[index][tuple] =  parameter_section.getValue(counter, file_column);
			// Store the property pair.
			property_pairs_[target_name].push_back(tuple);

		}

		// For each property pair  
		// create a hypersurface in  hypersurfaces_.

		// For all atom types... 
		for (Position i = 0; i < property_files_.size(); i++)
		{		
			std::map< std::pair<String, String>, String >::iterator it = property_files_[i].begin(); 
			for (; it != property_files_[i].end(); it++)
			{
				String atom_type = target_names_[i];
				String first_property = (*it).first.first ;
				String second_property = (*it).first.second;
				ShiftHyperSurface_ shs( (*it).second, atom_type, first_property, second_property, verbosity_); 
										
				if (shs.isvalid())
				{
					// ... insert in the map of hypersurfaces.
					std::pair<String, String> tuple(first_property, second_property);
					hypersurfaces_[atom_type][tuple]= shs;
				}
			}
		}
		
		// Mark the module as initialized.
		valid_ = true;
	}

	bool EmpiricalHSShiftProcessor::start()
	{	
		// If the module is invalid, abort!
		if (!isValid())
		{
			return false;
		}

		// Clear the target list.
		targets_.clear();

		return true;
	}


	bool EmpiricalHSShiftProcessor::finish()
	{
		// If the module is in an invalid state, abort!
		if (!isValid())
		{
			return false;
		}
		
		//printParameters_();	
		//printTargets_();
		
		// If there were no targets, return immediately.
		if (targets_.empty())
		{	
			if (verbosity_ >= VERBOSITY_LEVEL_DEBUG)
			{
				Log.info() << "EmpiricalHSShiftProcessor: no targets found!" << std::endl;
			}
			return true;
		}
		
		// If there are no hypersurfaces, return immediately.
		if (hypersurfaces_.empty())
		{	
			if (verbosity_ >= VERBOSITY_LEVEL_DEBUG)
			{
				Log.info() << "EmpiricalHSShiftProcessor: no hypersurfaces created!" << std::endl;
			}
			return true; 
		}
		
		float EHS_shift =  0.;

		// For all targets compute the shift.
		for (Position i = 0; i < targets_.size(); i++)
		{
			EHS_shift =  0.;
			
			PropertiesForShift_  target = targets_[i];
			String atom_type = target.current_atom->getName();
		
			// Do we have to exclude prolins?
			if (target.current_atom->getResidue()->getName() == "PRO" && target.current_atom->getName() == "N")
			{
				target.current_atom->setProperty(PROPERTY__EHS_SHIFT, EHS_shift);
				continue;
			}
			
			// For all property pairs of the targets atom type.
			for (Position j = 0; j < property_pairs_[atom_type].size(); j++) 
			{
				if (hypersurfaces_[atom_type][property_pairs_[atom_type][j]].isvalid())
				EHS_shift += hypersurfaces_[atom_type][property_pairs_[atom_type][j]](target);
			}
	
			// Correction for ssbonds.
			if (target.current_atom->getResidue()->hasProperty(Residue::PROPERTY__HAS_SSBOND))
			{
				if (ssbond_correction_.find(atom_type) != ssbond_correction_.end())
					EHS_shift += ssbond_correction_[atom_type];
			}
			
			// Set the shift contribution as a property
			target.current_atom->setProperty(PROPERTY__EHS_SHIFT, EHS_shift);
			 
			// Add the empirical hypersurface shift to the total shift.
			float old_shift = target.current_atom->getProperty(ShiftModule::PROPERTY__SHIFT).getFloat(); 
			target.current_atom->setProperty(ShiftModule::PROPERTY__SHIFT, (old_shift + EHS_shift)); 
		}

		//printTargets_();
		//printParameters_();
		
		return true;
	}
	
		
	Processor::Result  EmpiricalHSShiftProcessor::operator () (Composite& composite)
	{	
		// Here, we collect all target atoms, specified in the section 
		// "EmpiricalShiftHyperSurfaces" of the {\tt ShiftX.ini} - file,
		// and computes the corresponding atoms properties which are likewise specified 
		// in the ShiftX.ini-file. Since there is no object in BALL to 
		// store the type of a property in init() and then use it directly here to
		// compute and store these properties, we store the _type_ of the 
		// property as a string in init() and hardcode here for each
		// string the corresponding property computation.
		
		// If the composite is an atom ...
        if (RTTI::isKindOf<Atom>(&composite))
		{
			Atom* atom = dynamic_cast<Atom*>(&composite);
			
			// ...clear the property ... 
			atom->clearProperty(PROPERTY__EHS_SHIFT);

			// ... and compute all demanded properties for this atom type
			for (Position i = 0; i < target_names_.size(); i++) 
			{
				if (atom->getName() == target_names_[i])
				{	
					PropertiesForShift_  property(verbosity_);
					if( property.computeProperties_(atom, target_property_names_[i]))
						targets_.push_back(property);
					
				}
			}
		}	
		
		return Processor::CONTINUE;
	}
	
	void EmpiricalHSShiftProcessor::printParameters_()
	{
			Log.info() << "********* \n EHS: list of parameters" << std::endl;
			Log.info() << "exclude prolins: " << exclude_prolins_ << std::endl;
			Log.info() << "\n EHS: target names \t--\ttarget properties \t--\tfiles " << std::endl;
	
		for (Position i = 0; i < target_names_.size(); i++)
		{
			Log.info() << "\t\t" << target_names_[i] << "\t--\t";
		
			for (std::set<String>::iterator it = target_property_names_[i].begin();
					 it != target_property_names_[i].end(); it++)
			{
				Log.info() << (*it) << ", ";
			}
			Log.info() << "\t--\t";

			for (std::map< std::pair<String, String>, String >::iterator it = property_files_[i].begin();
						it != property_files_[i].end(); it++ )
			{
					Log.info() << "(" << (*it).first.first << "," << (*it).first.second << ")  " ;
			}
				Log.info() << "------------------------------\n" << std::endl;
		}
	}
	
	void EmpiricalHSShiftProcessor::printTargets_()
	{
		Log.info() << "********* \n EHS: list of targets and their values" << std::endl;
		Log.info() << "   atom\tFR\tAA\tSS\tPSI\tPHI\tCHI\tCHI2\tHAL\tHA\tHA2L\tHA2\tHNL\tHN\tOHL\tOH\tDisulfid" << std::endl;
		for (Position i = 0; i < targets_.size(); i++)
		{
				PropertiesForShift_  p = targets_[i];

				Log.info()<< "  " << p.current_atom->getName() << "\t" << p["FR"].second << "\t" << p["AA"].second << "\t" 
									<< p["SS"].second << "\t" << p["PSI"].first 	 << "\t"    << p["PHI"].first << "\t" 
									<< p["CHI"].first << "\t" << p["CHI2"].second  << "\t"  << p["HA1L"].first << "\t"
									<< p["HA1"].first << "\t" << p["HA2L"].first  << "\t"   << p["HA2"].second << "\t" 
									<< p["HNL"].first << "\t" << p["HN"].second   <<  "\t"
									<< p["OHL"].first << "\t" << p["OH"].second   << "\t" 
									<< p["DISULFIDE"].second << std::endl;

		}
		Log.info() << "------------------------------\n" << std::endl;
	}
	

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~:PropertiesForShift_:~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// 
	
	EmpiricalHSShiftProcessor::PropertiesForShift_::PropertiesForShift_(int verbosity)
		: verbosity_(verbosity)
	{
	}
	
	float EmpiricalHSShiftProcessor::PropertiesForShift_::getChiAngle_(Residue* residue) 
	{
		Atom* N = 0;
		Atom* CA = 0;
		Atom* CB = 0;
		Atom* X = 0;
		
		int num_of_atoms = 0;
	
		float angle =  FLOAT_VALUE_NA;


		// SHIFTX requires that ALA has no typical CHI - angle
		if ( 		(residue->getName() == "ALA") 
				 || (residue->getName() == "GLY" ) )
			return angle;
		
		AtomIterator r_it = residue->beginAtom();
		for (; r_it != residue->endAtom(); ++r_it)
		{	
			String name = r_it->getName();
			if (name == "N")
			{
				N = &(*r_it);
				num_of_atoms += 1;
			}
			else if (name == "CA")
			{
				CA = &(*r_it);
				num_of_atoms += 1;
			}
			else if (name == "CB")
			{ 
				CB = &(*r_it);
				num_of_atoms += 1;
			}
		}

		if (num_of_atoms != 3)
		{
			if (verbosity_ >= VERBOSITY_LEVEL_DEBUG)
			{
				Log.info() << "EmpiricalHSShiftProcessor: torsion angle of " << residue->getName() << " could not be computed!" << std::endl;
			}
			return FLOAT_VALUE_NA;
		}
		
		// determine the missing atom X
		/* Sidechain dihedral angle chi1 is defined
			 (for non-Gly residues) as follows:

			Chi1:	N(i)-CA(i)-CB(i)-X(i)

			where X is the following atom for the following
			residue types:

				X				Residue type
				----		____________
				HB1			Ala (for models with protons,labelled 1HB in PDB files).
				SG			Cys
				CG			Asp, Glu, Phe, His, Lys, Leu, Met,
								Asn, Pro, Gln, Arg, Trp, Tyr
				CG1			Ile, Val
				OG			Ser
				OG1			Thr
		
		In order to be conform with ShiftX we leave out GLY and ALA
		*/
		String residue_name = residue->getName();
		if (residue_name == "GLY")
		{ 
			if (verbosity_ >= VERBOSITY_LEVEL_DEBUG)
			{
				Log.info() << "EmpiricalHSShiftProcessor: torsion angle of a glycine could not be computed!" << std::endl;
			}
			return FLOAT_VALUE_NA;
		}
		else if (residue_name == "ALA")
		{
			r_it = residue->beginAtom();
			for (;r_it != residue->endAtom(); ++r_it)
			{	
				if (r_it->getName() == "HB1" || r_it->getName() == "1HB")
				{
					X = &(*r_it);
					num_of_atoms += 1;
					break;
				}
			}
		}
		else if (residue_name == "CYS")
		{
			r_it = residue->beginAtom();
			for (;r_it != residue->endAtom(); ++r_it)
			{	
				if (r_it->getName() == "SG")
				{
				 	X = &(*r_it);	
					num_of_atoms += 1;
					break;
				}
			}
		}
		else if (residue_name == "ASP" || residue_name ==  "GLU" || 
							residue_name == "PHE" || residue_name ==  "HIS" || 
							residue_name == "LYS" || residue_name ==  "LEU" ||
							residue_name == "MET" || residue_name ==  "ASN" ||
							residue_name == "PRO" || residue_name ==  "GLN" ||
							residue_name == "ARG" || residue_name ==  "TRP" ||  residue_name == "TYR")
		{
			r_it = residue->beginAtom();
			for (;r_it != residue->endAtom(); ++r_it)
			{	
				if (r_it->getName() == "CG")
				{
				 	X = &(*r_it);	
					num_of_atoms += 1;
					break;
				}
			}
		}
		else if (residue_name == "VAL" || residue_name ==  "ILE")
		{									
			r_it = residue->beginAtom();
			for (;r_it != residue->endAtom(); ++r_it)
			{	
				if (r_it->getName() == "CG1")
				{
					X = &(*r_it); 	
					num_of_atoms += 1;
					break;
				}
			}
		}
		else if (residue_name == "SER") 
		{									
			r_it = residue->beginAtom();
			for (;r_it != residue->endAtom(); ++r_it)
			{	
				if (r_it->getName() == "OG")
				{
					X = &(*r_it);	
					num_of_atoms += 1;
					break;
				}
			}
		}
		else if (residue_name == "THR") 
		{									
			r_it = residue->beginAtom();
			for (;r_it != residue->endAtom(); ++r_it)
			{	
				if (r_it->getName() == "OG1")
				{
					X = &(*r_it);
					num_of_atoms += 1;
					break;
				}
			}
		}

		if (num_of_atoms != 4)
		{
			if (verbosity_ >= VERBOSITY_LEVEL_DEBUG)
			{
				Log.info() << "EmpiricalHSShiftProcessor: chi torsion angle of " << residue->getID() << "-"<<  residue->getName() << " could not be computed!" << std::endl;
			}

			return FLOAT_VALUE_NA;
		}

		Vector3 a = N->getPosition();
		Vector3 b = CA->getPosition();
		Vector3 c = CB->getPosition();
		Vector3 d = X->getPosition();
		
		angle = getTorsionAngle(a.x, a.y, a.z, b.x, b.y, b.z, 
													 c.x, c.y, c.z, d.x, d.y, d.z)*180./M_PI;
		while (angle < 0.)
		{
			angle = angle + 360.;
		}
		return angle; 
	}

	
	float EmpiricalHSShiftProcessor::PropertiesForShift_::getChi2Angle_(Residue* residue) 
	{
		// NOTE: for compatibility with ShiftX, 
		// in case of C, S, G and A we have to take the fourth column of the input table, which is symbolized by ALA

		float angle =  FLOAT_VALUE_NA;
		int num_of_atoms = 0;
		Atom* CA = 0;
		Atom* CB = 0;
		Atom* CG = 0;
		Atom* XG = 0;
		
		String residue_name = residue->getName();
		
		// GLY, ALA, SER and CYS have no typical CHI2 - angle
		if ( 		(residue->getName() == "ALA") 
				 || (residue->getName() == "GLY") 
				 || (residue->getName() == "SER") 
				 || (residue->getName() == "CYS"))
			return angle;

		//  Sidechain dihedral angle chi2 is defined as follows:
		//  
		// 	Chi2:	CA(i)-CB(i)-CG(i)-XG(i)
		//  where XG is the following atom for the following
		//	residue types:
		//
		//	CG     XG     residue 
		//	-------------------------------------
		// 	CG     CD     PRO, GLN,  GLU, LYS, ARG
		// 		     CD1    LEU,TRP,PHE,TYR, 
	  //  		   OD1    ASN, ASP
		//  		   ND1		HIS	 
		//         SD     MET 
		//  CG1    CD1    ILE
		//         1HG1   VAL
		//	CG2	   (1HG2   VAL)
		//				 1HG2		THR
		//	
		//  TO DO: (is that true? implement : ok :-))  
		//  Note: in some amino acids the atom names can be switched, i.e.
    // 				for chi2 in amino acids PHE:  CD1 <-> CD2
    //          				              TYR:  CD1 <-> CD2
    //        	        			 	      ASP:  OD1 <-> OD2
				
		
		AtomIterator r_it = residue->beginAtom();
		for (; r_it != residue->endAtom(); ++r_it)
		{	
			String name = r_it->getName();
			if (name == "CA")
			{
				CA = &(*r_it);
				num_of_atoms += 1;
			}
			else if (name == "CB")
			{ 
				CB = &(*r_it);
				num_of_atoms += 1;	
			}
			if (name == "CG")
			{
				CG = &(*r_it);
				num_of_atoms += 1;
			}
		}
		
		//look for XG
		if (   (residue_name == "ARG") || (residue_name == "GLN") 
				|| (residue_name == "GLU") || (residue_name == "LYS") 
				|| (residue_name == "PRO"))
		{
			r_it = residue->beginAtom();
			for (;r_it != residue->endAtom(); ++r_it)
			{	
				if (r_it->getName() == "CD")
				{
					XG = &(*r_it);
					num_of_atoms += 1;
					break;
				}
			}
		}
		else if ( (residue_name == "ASN") || (residue_name == "ASP") )
		{
			r_it = residue->beginAtom();
			for (;r_it != residue->endAtom(); ++r_it)
			{	
				if (r_it->getName() == "OD1")
				{
					XG = &(*r_it);
					num_of_atoms += 1;
					break;
				}
			}
		}
		else if ( (residue_name == "LEU") || (residue_name == "TRP") 
				    ||(residue_name == "PHE") || (residue_name == "TYR") )
		{
			r_it = residue->beginAtom();
			for (;r_it != residue->endAtom(); ++r_it)
			{	
				if (r_it->getName() == "CD1")
				{
					XG = &(*r_it);
					num_of_atoms += 1;
					break;
				}
			}
		}	
		else if (residue_name == "HIS") 
		{
			r_it = residue->beginAtom();
			for (;r_it != residue->endAtom(); ++r_it)
			{	
				if (r_it->getName() == "ND1")
				{
					XG = &(*r_it);
					num_of_atoms += 1;
					break;
				}
			}
		}
		else if(residue_name == "MET") 	
		{
			r_it = residue->beginAtom();
			for (;r_it != residue->endAtom(); ++r_it)
			{	
				if (r_it->getName() == "SD")
				{
					XG = &(*r_it);
					num_of_atoms += 1;
					break;
				}
			}
		}
		// we have to take special care about ILE, VAL and THR
		else if(residue_name == "ILE") 	
		{
			r_it = residue->beginAtom();
			for (;r_it != residue->endAtom(); ++r_it)
			{	
				if (r_it->getName() == "CG1")
				{
					CG = &(*r_it);
					num_of_atoms += 1;
				}
				else if (r_it->getName() == "CD1")
				{
					XG = &(*r_it);
					num_of_atoms += 1;
				}
			}
		}
		else if(residue_name == "VAL") 	
		{
			r_it = residue->beginAtom();
			for (;r_it != residue->endAtom(); ++r_it)
			{	
				if (r_it->getName() == "1HG1")
				{
					XG = &(*r_it);
					num_of_atoms += 1;
				}
				else if (r_it->getName() == "CG1")
				{
					CG = &(*r_it);
					num_of_atoms += 1;
				}
			}
		}
		else if(residue_name == "THR") 	
		{
			r_it = residue->beginAtom();
			for (;r_it != residue->endAtom(); ++r_it)
			{	
				if (r_it->getName() == "1HG2")
				{
					XG = &(*r_it);
					num_of_atoms += 1;
				}
				else if (r_it->getName() == "CG2")
				{
					CG = &(*r_it);
					num_of_atoms += 1;
				}
			}
		} 

		if (num_of_atoms != 4)
		{
			if (verbosity_ >= VERBOSITY_LEVEL_DEBUG)
			{
				Log.info() << "EmpiricalHSShiftProcessor: chi2 torsion angle of " << residue->getID() << "-"  << residue->getName() << " could not be computed!" << std::endl;
			}
			return FLOAT_VALUE_NA;
		}
			
		Vector3 a = CA->getPosition();
		Vector3 b = CB->getPosition();
		Vector3 c = CG->getPosition();
		Vector3 d = XG->getPosition();
		
		angle = getTorsionAngle(a.x, a.y, a.z, b.x, b.y, b.z, 
													 c.x, c.y, c.z, d.x, d.y, d.z)*180./M_PI;
		while (angle < 0.)
		{
			angle = angle + 360.;
		}

		return angle;
	}

	
	char EmpiricalHSShiftProcessor::PropertiesForShift_::getSecondaryStructure_(Residue* residue) 
	{
		char ret = CHAR_VALUE_NA;
		if (!(residue->getSecondaryStructure())) 
		{
			if (verbosity_ >= VERBOSITY_LEVEL_DEBUG)
			{
				Log.info() << "EmpiricalHSShiftProcessor: no secondary structure available. Consider precomputing!" << std::endl;
			}
		}
		else if (residue->getSecondaryStructure()->getType() == SecondaryStructure::COIL)
		{
			ret = 'C';
		}
		else if (residue->getSecondaryStructure()->getType() == SecondaryStructure::HELIX)
		{
			ret = 'H';
		}
		else if (residue->getSecondaryStructure()->getType() == SecondaryStructure::STRAND)
		{
			ret = 'B';
		}
		return ret;
	}

	char EmpiricalHSShiftProcessor::PropertiesForShift_::getAminoAcid_(Residue* residue)
	{	
		char ret = CHAR_VALUE_NA;
		if (residue->isAminoAcid())
		{
			ret = Peptides::OneLetterCode(residue->getName()); 
		}
		return ret;
	}

	
	float EmpiricalHSShiftProcessor::PropertiesForShift_::getHA_HBondLen_(Residue* residue)
	{
		//float len = FLOAT_VALUE_NA;	
		float len = FLOAT_VALUE_IGNORE;

		Atom* HA = 0;
		
		AtomIterator r_it = residue->beginAtom();
		for (; r_it != residue->endAtom(); ++r_it)
		{	
			String name = r_it->getName();
			if (name == "HA")
			{
				HA = &(*r_it);
			}
		}
		
		if (HA)
		{	
			Atom::BondIterator bi = HA->beginBond();
			for (;+bi;++bi)
			{	
				if(bi->getType() == Bond::TYPE__HYDROGEN)
				{
					len = bi->getLength();
				}
			}
		}	
		return len;
	}	

	
	float EmpiricalHSShiftProcessor::PropertiesForShift_::getHA2_HBondLen_(Residue* residue)
	{
		//float len = FLOAT_VALUE_NA ;
		float len = FLOAT_VALUE_IGNORE;

		Atom* HA2 = 0;
		
		AtomIterator r_it = residue->beginAtom();
		for (; r_it != residue->endAtom(); ++r_it)
		{	
			String name = r_it->getName();
			if (name == "HA2" || name == "2HA")
			{
				HA2 = &(*r_it);
			}
		}
		
		if (HA2)
		{	
			Atom::BondIterator bi = HA2->beginBond();
			for (;+bi;++bi)
			{	
				if(bi->getType() == Bond::TYPE__HYDROGEN)
				{
					len = bi->getLength();
				}
			}
		}	
		return len;
	}	

	float EmpiricalHSShiftProcessor::PropertiesForShift_::getHN_HBondLen_(Residue* residue)
	{
		//float len = FLOAT_VALUE_NA;	
		float len = FLOAT_VALUE_IGNORE;

		Atom* HN = 0;
		
		AtomIterator r_it = residue->beginAtom();
		for (; r_it != residue->endAtom(); ++r_it)
		{	
			String name = r_it->getName();
			if (name == "H")
			{
				HN = &(*r_it);
			}
		}
		
		if (HN)
		{
			Atom::BondIterator bi = HN->beginBond();
			for (;+bi;++bi)
			{	
				if(bi->getType() == Bond::TYPE__HYDROGEN)
				{
					len = bi->getLength();
				}
			}
		}	
		return len;
	}	

	
	float 	EmpiricalHSShiftProcessor::PropertiesForShift_::getO_HBondLen_(Residue* residue) 
	{
//		float len = FLOAT_VALUE_NA;
		float len = FLOAT_VALUE_IGNORE;
		
		Atom* O = 0;
		
		// this works since FLOAT_VALUE_IGNORE is much larger than any possible bond length
		// find the oxygen with smallest bond length
		AtomIterator r_it = residue->beginAtom();
		for (; r_it != residue->endAtom(); ++r_it)
		{	
			if (r_it->getElement() == PTE[Element::O])
			//if (r_it->getName() == "O")
			{
				O = &(*r_it);
				Atom::BondIterator bi = O->beginBond();
				for (;+bi;++bi)
				{	
					if(bi->getType() == Bond::TYPE__HYDROGEN)
					{
						// for backbone "N", shiftx ignores partners that are not backbone "O"
						if ((bi->getPartner(*O)->getName() == "H") && (O->getName() != "O"))
							continue;

						len = std::min(len,bi->getLength());
					}	
				}
			}
		}
		
		return len;
	}
		
	bool 		EmpiricalHSShiftProcessor::PropertiesForShift_::hasDisulfidBond_(Residue* residue)
	{
		return residue->hasProperty(Residue::PROPERTY__HAS_SSBOND);
	}
	
	bool EmpiricalHSShiftProcessor::PropertiesForShift_::hasHA_HBond_(Residue* residue) 
	{		
		bool ret= false;
		Atom* HA = 0;
		
		AtomIterator r_it = residue->beginAtom();
		for (; r_it != residue->endAtom(); ++r_it)
		{	
			String name = r_it->getName();
			if ( (name == "HA") || (name == "1HA") || (name == "HA1") )
			{
				HA = &(*r_it);
			}
		}
		
		if (HA)
		{	
			Atom::BondIterator bi = HA->beginBond();
			for (;+bi;++bi)
			{	
				if(bi->getType() == Bond::TYPE__HYDROGEN)
				{
					ret = true;
				}
			}
		}	

		return ret; 
	}
	

	bool 		EmpiricalHSShiftProcessor::PropertiesForShift_::hasHA2_HBond_(Residue* residue) 
	{
		bool ret = false; 
		Atom* HA2 = 0;
		
		AtomIterator r_it = residue->beginAtom();
		for (; r_it != residue->endAtom(); ++r_it)
		{	
			String name = r_it->getName();
			if (name == "HA2" || name == "2HA")
			{
				HA2 = &(*r_it);
			}
		}
		
		if (HA2)
		{	
			Atom::BondIterator bi = HA2->beginBond();
			for (;+bi;++bi)
			{	
				if(bi->getType() == Bond::TYPE__HYDROGEN)
				{
					ret = true;
				}
			}
		}	
		return ret;
	}

	
	bool 	EmpiricalHSShiftProcessor::PropertiesForShift_::hasHN_HBond_(Residue* residue)
	{
		bool ret = false;
		Atom* HN = 0;
		
		AtomIterator r_it = residue->beginAtom();
		for (; r_it != residue->endAtom(); ++r_it)
		{	
			String name = r_it->getName();
		// the H atom iuapc named HN is called H in BALL!!! 
			if (name == "H")
			{
				HN = &(*r_it);
			}
		}
		
		if (HN)
		{
			Atom::BondIterator bi = HN->beginBond();
			for (;+bi;++bi)
			{	
				if (bi->getType() == Bond::TYPE__HYDROGEN)
				{
					ret = true;	
				}
			}
		}	
		return ret;
	}

	bool EmpiricalHSShiftProcessor::PropertiesForShift_::computeProperties_(Atom* a, std::set<String> properties) 
	{				
		// Set the atom pointer.
		current_atom = a;
		Residue* residue = current_atom->getResidue();
		// If the atom is not part on a residue, return false.
		if (!residue)
		{
			return false;
		}

		// Determine the predecessor and successor. 
		Residue* prev_residue = current_atom->getAncestor(*residue)->getPrevious(*residue);
		Residue* next_residue = current_atom->getAncestor(*residue)->getNext(*residue);

		//  All properties as specified in "properties" are computed. 
		// NOTE: if a certain property value is not available, the property gets
		// 		the value FLOAT_VALUE_NA or STRING_VALUE_NA.
		// 		Thus we can evaluate unknown values 
		// 		by assinging them the average value 
		// 	  (which is the way ShiftX works :-) )
		for (std::set<String>::iterator it = properties.begin();
				it != properties.end(); it++)
		{	
			// If there is no predecessor, set the "Unknown values"
			// FLOAT_VALUE_NA and STRING_VALUE_NA.
			if (!prev_residue)
			{
					properties_real_[("PSI_P")]  = FLOAT_VALUE_NA;
					properties_real_[("PHI_P")]  = FLOAT_VALUE_NA;
					properties_real_[("CHI_P")]  = FLOAT_VALUE_NA;
					properties_real_[("CHI2_P")] = FLOAT_VALUE_NA;
					properties_real_[("HA1L_P")] = FLOAT_VALUE_NA;
					properties_real_[("HA2L_P")] = FLOAT_VALUE_NA;
					properties_real_[("HNL_P")]  = FLOAT_VALUE_NA;	
					properties_real_[("OHL_P")]  = FLOAT_VALUE_NA;
				
					properties_string_[("AA_P")] = STRING_VALUE_NA;
					properties_string_[("SS_P")] = STRING_VALUE_NA; 
					properties_string_[("HA1_P")] = STRING_VALUE_NA;
					properties_string_[("HA2_P")] = STRING_VALUE_NA;
					properties_string_[("HN_P")] = STRING_VALUE_NA;	
					properties_string_[("OH_P")] = STRING_VALUE_NA;	
					properties_string_[("DISULFIDE_P")] = STRING_VALUE_NA;	
					properties_string_[("CHI_P")] = "Unknown"; 
					properties_string_[("CHI2_P")] = "Unknown";

					properties_string_[("FR_P")] = STRING_VALUE_NA;
			}
			else // Otherwise compute!
			{
				if ((*it) == 	"FR_P" )
				{	
					properties_string_[(*it)]= (prev_residue->isNTerminal() ? "Y": "N");
				}
				else if ((*it) == 	"AA_P" )
				{
					properties_string_[(*it)]= getAminoAcid_(prev_residue);

				}
				else if ((*it) == 	"SS_P" )
				{
					properties_string_[(*it)]= getSecondaryStructure_(prev_residue);

				}
				else if ((*it) == 	"PSI_P")
				{
					if (prev_residue->hasTorsionPsi())
					{	
						properties_real_[(*it)]= prev_residue->getTorsionPsi().toDegree();	
						if (properties_real_[(*it)] > 180)
							properties_real_[(*it)] -= 360;
						else if (properties_real_[(*it)] < -180)
							properties_real_[(*it)] += 360;
					}else
					{	
						properties_real_[(*it)] = FLOAT_VALUE_NA;
					}
				}
				else if ((*it) == 	"PHI_P")
				{	
					if (prev_residue->hasTorsionPhi())
					{		
						properties_real_[(*it)]= prev_residue->getTorsionPhi().toDegree();	
						if (properties_real_[(*it)] > 180)
							properties_real_[(*it)] -= 360;
						else if (properties_real_[(*it)] < -180)
							properties_real_[(*it)] += 360;
					}else
					{
						properties_real_[(*it)] = FLOAT_VALUE_NA;
					}
				}
				else if ((*it) == 	"CHI_P")
				{  // This simulates the ShiftX behaviour! 	
					properties_string_[(*it)] =  STRING_VALUE_NA;
					properties_real_[(*it)] = getChiAngle_(prev_residue);	
					if (properties_real_[(*it)] == FLOAT_VALUE_NA)
					{
						if (   (prev_residue->getName() == "ALA") 
								|| (prev_residue->getName() == "GLY") )
						{
							properties_string_[(*it)] = (prev_residue->getName());
						}
						else
						{
							properties_string_[(*it)] = "Unknown"; 
						}
					}						
				}
				else if ((*it) == 	"CHI2_P")
				{
					properties_string_[(*it)] =  STRING_VALUE_NA;
					properties_real_[(*it)] = getChi2Angle_(prev_residue);	
					if (properties_real_[(*it)] == FLOAT_VALUE_NA)
					{
						if (   (prev_residue->getName() == "ALA") 
								|| (prev_residue->getName() == "GLY")	
								|| (prev_residue->getName() == "SER") 	
								|| (prev_residue->getName() == "CYS")  )
						{
							properties_string_[(*it)] = "ALA";
						}
						else
						{
							properties_string_[(*it)] = "Unknown"; 
						}
					}
				}
				else if ((*it) == 	"HA1L_P" )
				{
					properties_real_[(*it)]= getHA_HBondLen_(prev_residue);	
				}
				else if ((*it) == 	"HA1_P" )
				{
					properties_string_[(*it)]= (hasHA_HBond_(prev_residue)? "Y": "N");	
				}
				else if ((*it) == 	"HA2L_P" )
				{
					properties_real_[(*it)]= getHA2_HBondLen_(prev_residue);	
				}
				else if ((*it) == 	"HA2_P" )
				{
					properties_string_[(*it)]= (hasHA2_HBond_(prev_residue) ? "Y": "N");	
				}
				else if ((*it) == 	"HNL_P" )
				{
					properties_real_[(*it)]= getHN_HBondLen_(prev_residue);	
				}
				else if ((*it) == 	"HN_P" )
				{
					properties_string_[(*it)]  = (hasHN_HBond_(prev_residue)? "Y": "N");
				}
				else if ((*it) == 	"OHL_P" )
				{
					properties_real_[(*it)] = getO_HBondLen_(prev_residue);	
				}
				else if ((*it) == 	"OH_P" )
				{
					properties_string_[(*it)] = (getO_HBondLen_(prev_residue) != FLOAT_VALUE_IGNORE) ? "Y": "N";	
				}
				else if ((*it) == 	"DISULFIDE_P" )
				{
					properties_string_[(*it)]= (hasDisulfidBond_(prev_residue)  ? "Y": "N");	
				}
			}
			
			// // If there is no predecessor, set the "Unknown values"
			// FLOAT_VALUE_NA and STRING_VALUE_NA.
			if (!next_residue)
			{
					properties_real_[("PSI_N")]  = FLOAT_VALUE_NA;
					properties_real_[("PHI_N")]  = FLOAT_VALUE_NA;
					properties_real_[("CHI_N")]  = FLOAT_VALUE_NA;
					properties_real_[("CHI2_N")] = FLOAT_VALUE_NA;
					properties_real_[("HA1L_N")] = FLOAT_VALUE_NA;
					properties_real_[("HA2L_N")] = FLOAT_VALUE_NA;
					properties_real_[("HNL_N")] = FLOAT_VALUE_NA;	
					properties_real_[("OHL_N")] = FLOAT_VALUE_NA;
					
					properties_string_[("AA_N")] = STRING_VALUE_NA;
					properties_string_[("SS_N")] = STRING_VALUE_NA; 
					properties_string_[("HA1_N")] = STRING_VALUE_NA;
					properties_string_[("HA2_N")] = STRING_VALUE_NA;
					properties_string_[("HN_N")] = STRING_VALUE_NA;	
					properties_string_[("OH_N")] = STRING_VALUE_NA;	
					properties_string_[("DISULFIDE_N")] = STRING_VALUE_NA;
					properties_string_[("CHI_N")] = "Unknown";// STRING_VALUE_NA;	
					properties_string_[("CHI2_N")] = "Unknown";//STRING_VALUE_NA;	

					//properties_string_[("FR_N")] = STRING_VALUE_NA;'N';
					properties_string_[("FR_N")] = 'N';

			}
			else // Otherwise: compute! 
			{
				if ((*it) == 				"FR_N" )
				{	
					properties_string_[(*it)]=  (next_residue->isNTerminal()? "Y": "N");	
				}
				else if ((*it) == 	"AA_N" )
				{
					properties_string_[(*it)]= getAminoAcid_(next_residue);
				} 
				else if ((*it) == 	"SS_N" )
				{
					properties_string_[(*it)]= getSecondaryStructure_(next_residue);
				}
				else if ((*it) == 	"PSI_N")
				{
					if (next_residue->hasTorsionPsi())
					{	
						properties_real_[(*it)]= next_residue->getTorsionPsi().toDegree();
						if (properties_real_[(*it)] > 180)
							properties_real_[(*it)] -= 360;
						else if (properties_real_[(*it)] < -180)
							properties_real_[(*it)] += 360;
					}
					else
					{	
						properties_real_[("PSI_N")]  = FLOAT_VALUE_NA;
					}
				}
				else if ((*it) == 	"PHI_N")
				{	
					if (next_residue->hasTorsionPhi())
					{	
						properties_real_[(*it)]= next_residue->getTorsionPhi().toDegree();	
						if (properties_real_[(*it)] > 180)
							properties_real_[(*it)] -= 360;
						else if (properties_real_[(*it)] < -180)
							properties_real_[(*it)] += 360;	
					}
					else
					{
						properties_real_[("PHI_N")]  = FLOAT_VALUE_NA;
					}
				}
				else if ((*it) == 	"CHI_N")
				{   // Simulate ShiftX behaviour! 
					properties_string_[(*it)] = STRING_VALUE_NA;	
					properties_real_[(*it)]   =  getChiAngle_(next_residue);
					if (properties_real_[(*it)] == FLOAT_VALUE_NA) 
					{
						if (   (next_residue->getName() == "ALA") 
								|| (next_residue->getName() == "GLY") )
						{
							properties_string_[(*it)] = (next_residue->getName());
						}
						else
						{
							properties_string_[(*it)] = "Unknown"; 
						}
					}				
				}
				else if ((*it) == 	"CHI2_N")
				{	
					properties_string_[(*it)] = STRING_VALUE_NA;	
					properties_real_[(*it)]   = getChi2Angle_(next_residue);
					if (properties_real_[(*it)] == FLOAT_VALUE_NA)
					{
						if ( (next_residue->getName() == "ALA") 
							|| (next_residue->getName() == "GLY")	
							|| (next_residue->getName() == "SER") 	
							|| (next_residue->getName() == "CYS")  )
						{
							properties_string_[(*it)] = ("ALA");
						}
						else
						{
							properties_string_[(*it)] = "Unknown"; 
						}
					}
				}
				else if ((*it) == 	"HA1L_N" )
				{
					properties_real_[(*it)]= getHA_HBondLen_(next_residue) ;		
				}
				else if((*it) == 		"HA1_N")
				{
					properties_string_[(*it)]= (hasHA_HBond_(next_residue)? "Y": "N");	
				}
				else if ((*it) == 	"HA2L_N" )
				{
					properties_real_[(*it)]= getHA2_HBondLen_(next_residue);	
				}
				else if ((*it) == 	"HA2_N" )
				{
					properties_string_[(*it)]= (hasHA2_HBond_(next_residue)? "Y": "N");	
				}
				else if ((*it) == 	"HNL_N" )
				{
					properties_real_[(*it)]= getHN_HBondLen_(next_residue);
				}
				else if ((*it) == 	"HN_N" )
				{
					properties_string_[(*it)]= (hasHN_HBond_(next_residue)? "Y": "N");	
				}
				else if ((*it) == 	"OHL_N" )
				{
					properties_real_[(*it)]= getO_HBondLen_(next_residue);	
				}
				else if ((*it) == 	"OH_N" )
				{
					properties_string_[(*it)] = (getO_HBondLen_(next_residue) != FLOAT_VALUE_IGNORE) ? "Y": "N";	
				}
				else if ((*it) == 	"DISULFIDE_N" )
				{
					properties_string_[(*it)]= 	(hasDisulfidBond_(next_residue)? "Y": "N");		
				}
			}
				
   		// Now compute the properties of the actual residue! 
			if ((*it) == "FR" )
			{
				properties_string_[(*it)]=  (residue->isNTerminal() ? "Y": "N");			
			}
			else if ((*it) == 	"AA" )
			{	
				properties_string_[(*it)]=  getAminoAcid_(residue);
			}
			else if ((*it) == 	"SS" )
			{
				properties_string_[(*it)]=  getSecondaryStructure_(residue);
			}
			else if ((*it) == 	"PSI")
			{
				if (residue->hasTorsionPsi())
				{	
					properties_real_[(*it)]= residue->getTorsionPsi().toDegree();
					if (properties_real_[(*it)] > 180)
						properties_real_[(*it)] -= 360;
					else if (properties_real_[(*it)] < -180)
						properties_real_[(*it)] += 360;
				}
				else
				{	
					properties_real_[(*it)] = FLOAT_VALUE_NA;
				}
			}
			else if ((*it) == 		"PHI")
			{		
				if (residue->hasTorsionPhi())
				{	
					properties_real_[(*it)]= residue->getTorsionPhi().toDegree();	
					if (properties_real_[(*it)] > 180)
						properties_real_[(*it)] -= 360;
					else if (properties_real_[(*it)] < -180)
						properties_real_[(*it)] += 360;	
				}
				else
				{
					properties_real_[(*it)] = FLOAT_VALUE_NA;
				}
			}
			else if ((*it) == 	"CHI")
			{	 // Simlulate ShiftX behaviour! 
				properties_string_[(*it)] = STRING_VALUE_NA;	
				properties_real_[(*it)]   = getChiAngle_(residue);	
				if (properties_real_[(*it)] ==  FLOAT_VALUE_NA)
				{
					if (   (residue->getName() == "ALA") 
							|| (residue->getName() == "GLY") )
					{
						properties_string_[(*it)] = (residue->getName());
					}
					else
					{
						properties_string_[(*it)] = "Unknown"; 
					}
				}				
			}
			else if ((*it) == 	"CHI2")
			{	
				properties_string_[(*it)] = STRING_VALUE_NA;	
				properties_real_[(*it)]   = getChi2Angle_(residue);	
				if (properties_real_[(*it)] == FLOAT_VALUE_NA)
				{
					if (   (residue->getName() == "ALA") 
							|| (residue->getName() == "GLY")	
							|| (residue->getName() == "SER") 	
							|| (residue->getName() == "CYS")  )
					{	
						properties_string_[(*it)] = ("ALA");
					}
					else
					{
						properties_string_[(*it)] = "Unknown"; 
					}
				}
			}
			else if ((*it) == 	"HA1L" )
			{
				properties_real_[(*it)]= getHA_HBondLen_(residue); 	
			}
			else if ((*it) == 	"HA1" )
			{ 	
				properties_string_[(*it)]=(hasHA_HBond_(residue)? "Y": "N");
			}
			else if ((*it) == 	"HA2L" )
			{
				properties_real_[(*it)]= getHA2_HBondLen_(residue) ;	
			}
			else if ((*it) == 	"HA2" )
			{ 	
				properties_string_[(*it)]= (hasHA2_HBond_(residue)? "Y": "N");		
			}
			else if ((*it) == 	"HNL" )
			{
				properties_real_[(*it)]= getHN_HBondLen_(residue) ;	
			}
			else if ((*it) == 	"HN" )
			{
				properties_string_[(*it)]=  (hasHN_HBond_(residue)? "Y": "N");		
			}
			else if ((*it) == 	"OHL" )
			{
				properties_real_[(*it)]= getO_HBondLen_(residue);	
			}
			else if ((*it) == 	"OH" )
			{
				properties_string_[(*it)] = (getO_HBondLen_(residue) != FLOAT_VALUE_IGNORE) ? "Y": "N";	
			}
			else if ((*it) == 	"DISULFIDE" )
			{
				properties_string_[(*it)]= (hasDisulfidBond_(residue)? "Y": "N");			
			}
			// The following both cases are due to SHIFTX:
			else if ((*it) == "ROW")
			{
				properties_string_[(*it)] = "N";	
			}
			else if ((*it) == "HBONDSTAT")
			{
				// 	The first position reflects existence of HA1 HBond (Length > 0.)
				// 	The second position reflects existence of HA2 HBond     "
				// 	The third position reflects HN HBond Length             "
				// 	The fourth position reflects O_Bond length
				//
				properties_string_[(*it)] = "YYYY";	
				properties_string_[(*it)][0] = (hasHA_HBond_(residue)? 'Y': 'N');
				properties_string_[(*it)][1] = (hasHA2_HBond_(residue)? 'Y': 'N');
				properties_string_[(*it)][2] = (hasHN_HBond_(residue)? 'Y': 'N');
				properties_string_[(*it)][3] = (getO_HBondLen_(residue) != FLOAT_VALUE_IGNORE) ? 'Y': 'N';
			}
		} 
		return true;
	}


	// Returns true, if the property type is alphanumeric/discrete.
	bool EmpiricalHSShiftProcessor::PropertiesForShift_::isDiscrete(String property)
	{
		return !(property.hasSubstring("PSI")  || property.hasSubstring("PHI")
		      || property.hasSubstring("HA2L") || property.hasSubstring("HA1L")
		      || property.hasSubstring("HNL")  || property.hasSubstring("OHL")
		      || property.hasSubstring("CHI"));
	}
	
	bool EmpiricalHSShiftProcessor::PropertiesForShift_::isMixed(String property)
	{
		return property.hasSubstring("CHI") || property.hasSubstring("CHI2");
	}

	

	
	std::pair<float, String>  EmpiricalHSShiftProcessor::PropertiesForShift_::operator [] (const String& property_name)
	{
		// Initialize the return value pair.
		std::pair<float, String> p(FLOAT_VALUE_NA , STRING_VALUE_NA);
	
		// Take care of the special case chi: 
		// 		can be a string (ALA, GLY, Unkown) or have an angle (float) e (-180, 180)
		if (property_name.hasSubstring("CHI"))
		{
			p.first = properties_real_[property_name];	
			p.second = properties_string_[property_name];
		}
		else if (properties_real_.find(property_name) != properties_real_.end())
		{
			p.first = properties_real_[property_name];
		}
		else if (properties_string_.find(property_name) != properties_string_.end())
		{
			p.second = properties_string_[property_name];
		}
		return p;
	}


// 
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~:ShiftHyperSurface_:~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// 

	EmpiricalHSShiftProcessor::ShiftHyperSurface_::ShiftHyperSurface_(int verbosity)
		: type_(),
			first_property_(),
			second_property_(),
			s2d_(),
			s1d_(),
			table_(),
			row_averages_(),
			col_averages_(),
			row_spacing_(),
			col_spacing_(),
			invalid_(),
			average_(), 
			verbosity_(verbosity),
			y_axis_values_(),
			x_axis_values_(),
			sample_values_()
	{
	}
	
	EmpiricalHSShiftProcessor::ShiftHyperSurface_::ShiftHyperSurface_(String filename, String atomtype, 
																																		String firstproperty, String secondproperty, int verbosity)
		throw(Exception::FileNotFound)
		:	type_(),
			first_property_(),
			second_property_(),
			s2d_(),
			s1d_(),
			table_(),
			row_averages_(),
			col_averages_(),
			row_spacing_(),
			col_spacing_(),
			invalid_(),
			average_(), 
			verbosity_(verbosity),
			y_axis_values_(),
			x_axis_values_(),
			sample_values_()
	{			
		// Find the data file.
		BALL::Path p;
		String file_name = p.find("NMR/"+filename) ;// "NMR/splinedata/hat_PSI_DISULFIDE-1.dat");
		if (file_name == "")
		{
			invalid_ = true;
			throw(Exception::FileNotFound(__FILE__, __LINE__, "NMR/"+filename ));
			return;
		}
		
		// The hypersurface is valid.
		invalid_ = false;
		
		// Set the x and y property type.
		first_property_  = firstproperty;
		second_property_ = secondproperty;
		
		// Set the hypersurface type.
		setType_(firstproperty, secondproperty);
		

		//
		// Read and Store the data.
		//
		
		BALL::File file(file_name, std::ios::in);

		// Parse the data file.
		parseDataFile_(file, filename);

		// Case single 1D spline.
		if (type_ == SINGLE__REAL) 
		{  
			// Create a 1D spline.	
			vector<float> x_axis;
			convertToReal_(x_axis_values_[0], x_axis);

			CubicSpline1D s(x_axis, sample_values_[0], average_);

			s.setVerbosity(verbosity_);

			// Set the lower and upper bounds of the 1D spline.
			// TODO: in den Construktor
      if (row_spacing_ != FLOAT_VALUE_NA)
      {
        s.setLowerBound(x_axis[0] - row_spacing_);
        s.setUpperBound(x_axis[x_axis.size()-1] + row_spacing_);
      }
			
			// Store the 1D spline in the 1D spline map.
			s1d_[first_property_] = s;
		}
		// Case  single look-up table line.
		else if (type_ == SINGLE__CHI || type_ == SINGLE__DISCRETE)  
		{ 
			// In order to save storage, the data is compressed 
			// into a single look-up line!
			// NOTE: in the datafile the values are stored diagonally! 
			for (Position i = 0; i < x_axis_values_[0].size(); i++)  
			{
				table_[first_property_][x_axis_values_[0][i]] = sample_values_[i][i];
			}
		}
		// Otherwise we have a 2D hypersurface.
		else   
		{
			// Depending on the property types, create 	
			// 					a 2D bicubic spline, 
			// 	  			a map of 1D bicubic splines, or 
			//	  			a lookUpTable (map of map)  
			//
			// Case 2D bicubic spline.
			if (type_ == REAL__REAL)
			{
				// We have a 2D bicubic spline.
				// Convert the values.
				vector<float> y_axis;
				convertToReal_(y_axis_values_, y_axis);

				vector<vector<float> > x_axis;
				for (Position i=0; i < y_axis.size(); i++)
				{
					vector<float> v;
					convertToReal_(x_axis_values_[i], v);
					x_axis.push_back(v);
				}

				// Create and store a bicubic spline.
				s2d_ = CubicSpline2D(x_axis, y_axis, sample_values_);
				s2d_.setVerbosity(verbosity_);

				// Build the lower and upper bounds like SHIFTX.
				for (Position i=0; i < s2d_.getNumberOfSplines(); i++)
				{
					// For each 1D spline...
					CubicSpline1D& cs = s2d_.getSpline(i);

					// ...set the bounds...
          if (row_spacing_ != FLOAT_VALUE_NA)
          {
            cs.setLowerBound(x_axis[i][0] - row_spacing_);
            cs.setUpperBound(x_axis[i][x_axis[i].size()-1] + row_spacing_);
          }

					// ...and set the average as default-value. 
					cs.setDefaultValue(row_averages_[y_axis_values_[i]]);
				}
				
				// For all 1D splines in the second direction...
        if (col_spacing_ != FLOAT_VALUE_NA)
        {
					//...set the bounds.
          s2d_.setYLowerBound(y_axis[0] - col_spacing_);
          s2d_.setYUpperBound(y_axis[y_axis.size()-1] + col_spacing_);
        }
			}    // Case 1D splines.
			else if (type_ == REAL__DISCRETE) 
			{
				// 		!!!!  C A U T I O N !!!! 
				// When accessing the data, one has to switch X and Y, 
				// since for each discrete value a 1D bicubic spline is stored
				
				// For all 1D splines...
				for (Position i = 0; i < y_axis_values_.size(); i++)
				{
					// ..convert the x-axis data...
					vector<float> x_axis;
					convertToReal_(x_axis_values_[i], x_axis);
					
					// ...create the 1D cubic spline...
					CubicSpline1D s(x_axis, sample_values_[i], row_averages_[y_axis_values_[i]]);	 
					s.setVerbosity(verbosity_);

					//...and the set bounds as read from the datafile.
          if (row_spacing_ != FLOAT_VALUE_NA)
          {
            s.setLowerBound(x_axis[0] - row_spacing_);
            s.setUpperBound(x_axis[x_axis.size()-1] + row_spacing_);
          }
					
					// Finally store the 1D spline in the 1D spline map.
					s1d_[y_axis_values_[i]] = s;
				}
			}  // Case 1D CHI spline.
			else if (type_ == REAL__CHI)
			{
				// 		!!!!  C A U T I O N !!!! 
				// When accessing the data, one has to switch X and Y, 
				// since for each discrete value a 1D bicubic spline is stored
				
				// For all 1D splines...
				for (Position i = 0; i < y_axis_values_.size(); i++)
				{
					// ...convert the data,...
					vector<float> x_axis;
					convertToReal_(x_axis_values_[i], x_axis);

					// ...create a 1D bicubic spline...
					CubicSpline1D s(x_axis, sample_values_[i], true);  //TODo: wieso hat der keine defaultwerte?? 
					s.setVerbosity(verbosity_);

					//...and set the bounds as read from the datafile.
          if (row_spacing_ != FLOAT_VALUE_NA)
          {
            s.setLowerBound(x_axis[0] - row_spacing_);
            s.setUpperBound(x_axis[x_axis.size()-1] + row_spacing_);
          }

					// Finally store the 1D spline in the 1D spline map.
					s1d_[y_axis_values_[i]] = s;
				}
			}  
			else if (   (type_ == DISCRETE__REAL ) 
					     && (verbosity_ >= VERBOSITY_LEVEL_CRITICAL))
			{ 
				Log.error() << "EmpiricalHSShiftProcessor: case DISCRETE__REAL is not implemented ("	
										<< __FILE__ << " " << __LINE__ << ")" <<  std::endl;
			} // Case look-up table
			else if ( (type_ == DISCRETE__DISCRETE) || (type_ == CHI__DISCRETE)|| (type_ == DISCRETE__CHI) || (type_ == CHI__CHI) )
			{
				if (    (x_axis_values_.size() != y_axis_values_.size()) 
						&&  (verbosity_ >= VERBOSITY_LEVEL_CRITICAL))
				{
					Log.error() << "EmpiricalHSShiftProcessor: invalid hypersuface table in file " 
											<< filename << "! (" 
											<< __FILE__ << " " << __LINE__ << ")" <<  std::endl;
				}
				else  // If the file is valid...
				{
					// ...no data has to be converted and
					// a look-up table is created.
					// NOTE: access of the look--up table: first the discrete key x, second the numerical key y.
					for (Position i = 0; i < y_axis_values_.size(); i++)  // y
					{
						for (Position j = 0; j < x_axis_values_[i].size(); j++) // x
					 	{ 	
							 table_[x_axis_values_[i][j]][y_axis_values_[i]] = sample_values_[i][j];
					 	}
					}
				}
			}	
			else if (   (type_ == CHI__REAL) 
							 && (verbosity_ >= VERBOSITY_LEVEL_CRITICAL))
			{	
				// Fortunately this case does not occur -> this method should not be send.
				Log.error() << "EmpiricalHSShiftProcessor: case CHI__REAL is not implemented (" 
				<< __FILE__ << " " << __LINE__ << ")" <<  std::endl;
			}		
			else if (   (type_ == CHI__CHI) 
					     && (verbosity_ >= VERBOSITY_LEVEL_CRITICAL))
			{
				// Fortunately this case does not occur -> this method should not be send.
				Log.error() << "EmpiricalHSShiftProcessor: case CHI__CHI is not implemented (" 
				<< __FILE__ << " " << __LINE__ << ")" <<  std::endl;
			}
		}
	}

	EmpiricalHSShiftProcessor::ShiftHyperSurface_::~ShiftHyperSurface_() 
	{
	}

	void EmpiricalHSShiftProcessor::ShiftHyperSurface_::setType_(String firstproperty, String secondproperty)
	{
		// Is the first property of type "CHI"?
		if(PropertiesForShift_::isMixed(firstproperty))
		{
			// Is the second property of type "CHI"? 
			if (PropertiesForShift_::isMixed(secondproperty))
			{	
				if (firstproperty == secondproperty)
					type_ = SINGLE__CHI;
				else
					type_ = CHI__CHI;
			}
			// Is the second property discrete?
			else if (PropertiesForShift_::isDiscrete(secondproperty))
			{
				type_ = CHI__DISCRETE;
			}
			else  // Or is it real?
			{	
				type_ = CHI__REAL;
			}	
		}		// Is the first proptery discrete? 
		else if (PropertiesForShift_::isDiscrete(firstproperty)) 
		{
			// Is the second property discrete too?
			if (PropertiesForShift_::isDiscrete(secondproperty))
			{	
				if (firstproperty == secondproperty)
					type_ = SINGLE__DISCRETE;
				else
					type_ = DISCRETE__DISCRETE;
			} // or is the second property of type "CHI"? 
			else if (PropertiesForShift_::isMixed(secondproperty))
				type_ = DISCRETE__CHI;
			else 
				type_ = DISCRETE__REAL;
		} 
		else // Otherwise the first property is real!
		{ // The second property is of discrete type.
			if (PropertiesForShift_::isDiscrete(secondproperty))
			{
				type_ = REAL__DISCRETE;
			} // The second property has type "CHI".
			else if (PropertiesForShift_::isMixed(secondproperty))
			{
				type_ = REAL__CHI;
			}
			else  // Or both are real.
			{
				if (firstproperty == secondproperty)
					type_ = SINGLE__REAL;
				else
					type_ = REAL__REAL;
			}
		}
		return;
	}

	void EmpiricalHSShiftProcessor::ShiftHyperSurface_::convertToReal_(const vector<String>& input, vector<float>& output)
	{
		// Clear the output vector.
		output.clear();
		
		// Convert all string entries into floats and store them in output.
		for (Position i=0; i < input.size(); i++)
			output.push_back(input[i].toFloat());
	}
	
	// Structure of all input files:
	//
	// atomtype factorx factory
	// total_average
	// (row_average_1;row_average_2;...;row_average_n|N/A)
	// (col_averages_1;col_averages_2;...;col_averages_n|N/A)
  // (row_spacing|N/A)
  // (col_spacing|N/A)
	// (y_axis_1;...;y_axis_n|N/A)
	// x_axis_11;...;x_axis_1m
	// value_11;...;value_1m
	// ...
	// x_axis_n1;...;x_axis_nm
	// value_n1;...;value_nm
	void EmpiricalHSShiftProcessor::ShiftHyperSurface_::parseDataFile_(BALL::File& file, String filename) 
	{
		String line;
		std::vector<BALL::String> fields;
		
		row_averages_.clear();
		col_averages_.clear();

		try {
			// Read over the first line. We don't need the information currently.
			line.getline(file);

			// Now read the total average. This is contained in _all_ datafiles, so we can depend on it being there.
			line.getline(file);
			average_ = line.toFloat();
			
			// Test for row averages.
			line.getline(file);
			String testline = line;
			testline.toUpper();
			
			// Are we given row averages?
			std::vector<float> row_average_values;
			if (!testline.hasSubstring("N/A"))
			{
				// Yes -> parse the row averages.
				line.split(fields, ";");
				for (Position i=0; i<fields.size(); i++)
					row_average_values.push_back(fields[i].toFloat());
			}

			// Test for col averages
			line.getline(file);
			testline = line;
			testline.toUpper();

			// Are we given column averages?
			std::vector<float> col_average_values;
			if (!testline.hasSubstring("N/A"))
			{
				// Yes -> parse the row averages.
				line.split(fields, ";");
				for (Position i=0; i<fields.size(); i++)
					col_average_values.push_back(fields[i].toFloat());
			}

			// Test for row spacing.
			line.getline(file);
			testline = line;
			testline.toUpper();

			// Is the row spacing set? 
			if (!testline.hasSubstring("N/A"))
        row_spacing_ = testline.toFloat();
      else
        row_spacing_ = FLOAT_VALUE_NA;
      
      // Test for col spacing.
			line.getline(file);
			testline = line;
			testline.toUpper();
	
			// Is the column spacing set? 
			if (!testline.hasSubstring("N/A"))
        col_spacing_ = testline.toFloat();
      else
        col_spacing_ = FLOAT_VALUE_NA;

			// Test for y_axis.
			line.getline(file);
			testline = line;
			testline.toUpper();

			if (!testline.hasSubstring("N/A"))
			{
				// Parse the row averages.
				testline.split(fields, ";");
				for (Position i=0; i<fields.size(); i++)
					y_axis_values_.push_back(fields[i]);
			}	
			
			// Finally, read the consecutive x_axis / value lines.
			Size number_of_lines = std::max((size_t)1, y_axis_values_.size());
			x_axis_values_.clear();
			x_axis_values_.resize(number_of_lines);
			sample_values_.clear();
			sample_values_.resize(number_of_lines);

			// Read the values...
			for (Position i=0; i<number_of_lines; i++)
			{
				line.getline(file);
				line.toUpper();
				line.split(fields, ";"); 
				for (Position j = 0; j < fields.size(); j++)
					x_axis_values_[i].push_back(fields[j]);

				line.getline(file);
				line.split(fields, ";"); 	

				for (Position j = 0; j < fields.size(); j++)
					sample_values_[i].push_back(fields[j].toFloat());
			}

			// ...and build the row_ and col_averages if applicable.
			for (Position i=0; i<row_average_values.size(); i++)
					row_averages_[y_axis_values_[i]] = row_average_values[i];
			for (Position i=0; i<col_average_values.size(); i++)
				col_averages_[x_axis_values_[0][i]] = col_average_values[i];

    } catch (...)
		{
			std::cerr<< "format error in " <<  filename << std::endl;
		}

	}


	
	float EmpiricalHSShiftProcessor::ShiftHyperSurface_::operator() (EmpiricalHSShiftProcessor::PropertiesForShift_& properties)
	{
		// This method evaluates the empirical hypersurface given 
		// the properties of an atom.
		// From all properties of that atom, we extract the two 
		// property values defining the hypersurface and evaluate 
		// the hypersurface at that value(s) called accessor(s).
		// Since hypersurfaces can be spliny or look-up-tably , we
		// have to take special care about the two accessors.
		// In particular there are two special cases: CHI-type 
		// and SINGLE-type properties.
		
		float shift = 0.;
		
		// First, for the cases 
		// 							 DISCRETE__DISCRETE
		// 							 CHI__DISCRETE
		// 							 DISCRETE__CHI
		// 							 SINGLE__DISCRETE
		// 							 SINGLE__CHI
		// we precompute the access string.
		// 
		// NOTE: properties of type "CHI" have a "mixed" nature.
		// The property can have a numeric or alphanumeric (= discrete) value.
		// Since the alphanumeric values are treated as 
		// a bin, we convert the alphanumeric value into a string 
		// representing the bin :-). 
		// NOTE: for the single properties (SINGLE__CHI, SINGLE__DISCRETE) 
		// we take the property type string itself (like "PHI")  as first access string.
		
		String string1 = properties[first_property_].second;
		String string2 = properties[second_property_].second;

		// Our first special case: CHI - types  as first property.
		if (PropertiesForShift_::isMixed(first_property_))
		{	
			string1 = properties[first_property_].second;
			string1.toUpper();
			
			// Is the property value numeric?  
			if (properties[first_property_].first != FLOAT_VALUE_NA)
			{
				float chi_value = properties[first_property_].first; 
				if (chi_value < 120.)	
					string1 = "60.000000";
				else if (chi_value < 240.) 
					string1 = "180.000000";
				else if (chi_value < 360.)
					string1 = "300.000000";
				else
				{	
					if (verbosity_ >= VERBOSITY_LEVEL_CRITICAL)
					{ 
						Log.error() << "EmpiricalHSShiftProcessor: " << properties.current_atom->getName() << ": " 
						<< second_property_ << "-value is not valid! ("
				 		<< __FILE__ << " " << __LINE__ << ")" <<  std::endl;
					}
				}
			}
		}
		
		// CHI - types  as second property.
		if (PropertiesForShift_::isMixed(second_property_))
		{
			string2 = properties[second_property_].second; 
			string2.toUpper();
			
			// Is the property value numeric? 
			if (properties[second_property_].first != FLOAT_VALUE_NA)
			{
				float chi_value = properties[second_property_].first; 
				if (chi_value < 120.)	
					string2 = "60.000000";
				else if (chi_value < 240.) 
					string2 = "180.000000";
				else if (chi_value < 360.)
					string2 = "300.000000";
				else 	
				{	
					if (verbosity_ >= VERBOSITY_LEVEL_CRITICAL)
					{
						Log.error() << "EmpiricalHSShiftProcessor: " << properties.current_atom->getName() << ": " 
						<< second_property_ << "-value is not valid! ("
				 		<< __FILE__ << " " << __LINE__ << ")" <<  std::endl;
					}
				}
			}
		}
		
		// Now we can assume that for CHI-types the correct alphanumeric string is set! 
		
		// Special case 2: SINGLE__??
		// The property type string itself is the first accessor.
		if (type_ == SINGLE__DISCRETE || type_ == SINGLE__CHI)
		{ 
			string1 = first_property_;
		}
		
		//
		// Now we access the hypersurface according to its type.
		// 
		
		// In case of look-up-tably types...
		if (   type_ == SINGLE__DISCRETE || type_ == SINGLE__CHI    || type_ == CHI__CHI 
				|| type_ == CHI__DISCRETE    || type_ == DISCRETE__CHI  || type_ == DISCRETE__DISCRETE)
		{
			// ...find out, if the first property is contained in the table.
			tabletype::iterator first_it = table_.find(string1);
			if (first_it != table_.end())
			{  
				// Yes it is :-)
				// Check if the second property is contained in the table.
				std::map<String, float>::iterator second_it = first_it->second.find(string2);
				if (second_it != first_it->second.end())
				{	// So both accessors are valid: we can just return the value.
					shift = second_it->second;   // = table_[string1][string2];	
				}
				else
						shift = average_;
			}
			else // If the first accessor is not valid... 
			{	
				// ...does the second property occur at all?
				if (tableHasColumn_(string2))
				{	// Yes -> return the column average.
						shift = average_;
				}
				else  // Both accessores are invalid. 
				{
					// We don't have the value at all... average over the whole table.
					shift = average_; 
				}
			}
		}  // In case of two identical properties of type real.
		else if (type_ == SINGLE__REAL)
		{ 
			// Remember: the property type string is first 
			// accessor and returns a 1D spline. 
			
			// Check, if the first property was set?
			if (s1d_.find(first_property_) != s1d_.end())
			{
				// No -> take average.
				if (properties[first_property_].first == FLOAT_VALUE_NA)
					shift = average_;
				else  // Yes -> evaluate the 1D spline.
					shift = s1d_[first_property_](properties[first_property_].first);
			}
			else 
				shift = 0.;
		}// Case of a 2D bicubic spline.
		else if (type_ == REAL__REAL)
		{		
			// Both properties valid?
			// No -> return something by default. 
			// This simulates SHIFTX behaviour: 
			// if only one factor is out of bounds, we 
			// return the all-values average.
			if ((properties[first_property_].first == FLOAT_VALUE_NA) || (properties[second_property_].first == FLOAT_VALUE_NA))
			{
				shift = average_;
			}
			else // Yes -> evaluate.
				shift = s2d_(properties[first_property_].first, properties[second_property_].first);
		}// Case of 1D splines -- part 1:
		else if (type_ == REAL__DISCRETE)
		{
			// Both properties valid?
			// No -> return something by default. 
			// This simulates SHIFTX behaviour: if only one 
			// factor is out of bounds, we return the all-values
			// average.
			// TODO: For some reason, SHIFTX does not seem to use the row_averages here! Find out why!!!
			if (   (properties[first_property_].first == FLOAT_VALUE_NA)
					 ||(properties[second_property_].second == STRING_VALUE_NA) )
			{
				// Return the total average over the whole map.
				shift = average_;
			}
			else // Yes -> evaluate.
			{
				shift = s1d_[properties[second_property_].second](properties[first_property_].first);
			}
		}// Case of 1D splines -- part 2:
		else if (type_ == DISCRETE__REAL)
		{	
			// Both properties valid?	
			// No -> return something by default. 
			// This simulates SHIFTX behaviour: if only one factor is 
			// out of bounds, we return the all-values average.
			if (   (properties[second_property_].first == FLOAT_VALUE_NA)
					 ||(properties[first_property_].second == STRING_VALUE_NA) )
			{
				// We have to compute the average of 
				// all 1D spline averages! 
				shift = 0;
				std::map<String, CubicSpline1D>::iterator ci;
				for (ci = s1d_.begin(); ci != s1d_.end(); ci++)
					shift += ci->second.getDefaultValue();
				shift /= s1d_.size();
			}
			else // Yes -> evaluate.
				shift = s1d_[properties[first_property_].second](properties[second_property_].first);
		}
		else if (   (type_ == CHI__REAL) 
				     && (verbosity_ >= VERBOSITY_LEVEL_CRITICAL))
		{	
			Log.error() << "EmpiricalHSShiftProcessor: CHI REAL should NEVER be called (at " 
				<< __FILE__ << " " << __LINE__ << ")" <<  std::endl;
		}
		else if (type_ == REAL__CHI)
		{	
			// This simulates SHIFTX behaviour: 
			// if only one factor is out of bounds, we return the all-values average.
			if (properties[first_property_].first == FLOAT_VALUE_NA)
				shift = average_;
			else
			{  // Everything ok? 
				if (s1d_.find(string2) != s1d_.end())
				{  // Yes -> evaluate!  
					shift = s1d_[string2](properties[first_property_].first);	
				}
				else // Otherwise return average!  
				{
					shift = average_; // TODO: is this the way SHIFTX does? Seems so! 
				}
			}
		}
		else
		{	
			if (verbosity_ >= VERBOSITY_LEVEL_CRITICAL)
			{
				Log.error() << "EmpiricalHSShiftProcessor: Unknown type of properties! (at " 
				<< __FILE__ << " " << __LINE__ << ")" <<  std::endl;
			}
		}
		
		// if one of the values is FLOAT_VALUE_IGNORE, we will _in all cases_ return zero
		if ( 		 (properties[first_property_].first == FLOAT_VALUE_IGNORE) 
					|| (properties[second_property_].first == FLOAT_VALUE_IGNORE) )
		{
			shift = 0.;
		}

#ifdef DEBUG
		std::cout << "_operator (): ";
		std::cout << properties.current_atom->getName() << " " << properties.current_atom->getResidue()->getID()<<  "  " 
			 				<< properties.current_atom->getResidue()->getName()<< "  " << first_property_ << ":" << second_property_<< " -- " 
							<< properties[first_property_].first << "/" << properties[first_property_].second<< ":" 
							<< properties[second_property_].first << "/" << properties[second_property_].second <<  " -- " << shift << std::endl;
#endif

		return shift;
	} 


	float EmpiricalHSShiftProcessor::ShiftHyperSurface_::getTableAverage_()
	{
		float average = 0.;
		int count = 0;

		// Run over all rows...
		tabletype::iterator row = table_.begin();
		for (; row != table_.end(); ++row)
		{
			// ...and all columns...
			std::map<String, float>::iterator column = row->second.begin();
			for (; column != row->second.end(); ++column)
			{
				// ...to compute the average.
				average += column->second;
				count++;
			}	 
		}

		if (count > 0)
			average /= count;

		return average;
	}
	
	float EmpiricalHSShiftProcessor::ShiftHyperSurface_::getTableRowAverage_(const std::map<String, float>& row)
	{
		float average = 0.;
		int   count = 0;
		
		// Run over all entries of the given row ...
		std::map<String, float>::const_iterator column = row.begin();
		for (; column != row.end(); ++column)
		{
			// ...to compute the average.
			average += column->second;
			count++;
		}

		if (count > 0)
			average /= count;

		return average;
	}

	float EmpiricalHSShiftProcessor::ShiftHyperSurface_::getTableColumnAverage_(const String& name)
	{
		float average = 0.;
		int   count = 0;

		// Run over all rows...
		tabletype::iterator row = table_.begin();
		for (; row!=table_.end(); ++row)
		{
			// ...look for the column named "name"... 
			std::map<String, float>::iterator column = row->second.find(name);
			if (column != row->second.end())
			{
				// ...and compute the average.
				average += column->second;
				count++;
			}
		}

		if (count > 0)
			average /= count;

		return average;
	}

	bool EmpiricalHSShiftProcessor::ShiftHyperSurface_::tableHasColumn_(const String& name)
	{
		// Method to check if the hypersurface's look--up table
		// has a column named "name".
		tabletype::iterator row = table_.begin();
		for (; row!=table_.end(); ++row)
			if (row->second.find(name) != row->second.end())
				return true;

		return false;
	}
	
	void  EmpiricalHSShiftProcessor::postprocessing_()
	{
		// Get the system.
		System* system = NULL;
		
		for (Position i = 0; i < targets_.size(); i++)
		{
            if  (RTTI::isKindOf<System>(&targets_[i].current_atom->getRoot()))
			{	
				system = dynamic_cast<System*>(&(targets_[i].current_atom->getRoot()));
			}
		}

		// If we found a system...
		if (system) 
		{
			// ...add for all CA 0.2 times the values of HA.
			for (BALL::ResidueIterator r_it = system->beginResidue(); r_it != system->endResidue(); ++r_it)
			{
				Atom* CA = 0;
				Atom* HA = 0;

				for (BALL::AtomIterator at_it = r_it->beginAtom(); +at_it; ++at_it)
				{
					if (at_it->getName() == "CA")
						CA = &(*at_it);
					if (at_it->getName() == "HA")
						HA = &(*at_it);
				}

				if (CA && HA)
				{	
					float total = CA->getProperty(ShiftModule::PROPERTY__SHIFT).getFloat();
					float ca_shift = CA->getProperty(BALL::EmpiricalHSShiftProcessor::PROPERTY__EHS_SHIFT).getFloat();
					float ha_shift = HA->getProperty(BALL::EmpiricalHSShiftProcessor::PROPERTY__EHS_SHIFT).getFloat();
					
					CA->setProperty(BALL::EmpiricalHSShiftProcessor::PROPERTY__EHS_SHIFT, ca_shift + 0.2*ha_shift);
					CA->setProperty(ShiftModule::PROPERTY__SHIFT, total+ 0.2*ha_shift );
				}
			}
		}
		else
		{
			if (verbosity_ >= VERBOSITY_LEVEL_DEBUG)
			{
				Log.error() << "Error in EmpiricalHSShiftProcessor: no system found for postprocessing. (" 
									<< __FILE__ << " " << __LINE__ << ")" <<  std::endl;
			}
		}
	}
	
	
	void EmpiricalHSShiftProcessor::setDefaultOptions()
	{			
		options.setDefaultInteger(EmpiricalHSShiftProcessor::Option::VERBOSITY,
													    EmpiricalHSShiftProcessor::Default::VERBOSITY);
	}

} // namespace BALL
