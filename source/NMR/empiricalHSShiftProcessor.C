
#include <BALL/NMR/empiricalHSShiftProcessor.h>
#include <BALL/COMMON/limits.h>
#include <BALL/KERNEL/bond.h>
#include <BALL/FORMAT/parameterSection.h>
#include <BALL/STRUCTURE/peptides.h>
#include <BALL/MATHS/analyticalGeometry.h>
#include <map>
#include <set>

using namespace std;

namespace BALL 
{
	
	const char* EmpiricalHSShiftProcessor::PROPERTY__EHS_SHIFT= "EmpiricalHSShift";

	EmpiricalHSShiftProcessor::EmpiricalHSShiftProcessor()
		throw()
		:	ShiftModule()
	{
	}

	EmpiricalHSShiftProcessor::EmpiricalHSShiftProcessor(const EmpiricalHSShiftProcessor& processor)
		throw()
		:	ShiftModule(processor)
		{
		}

	
	EmpiricalHSShiftProcessor::~EmpiricalHSShiftProcessor()
		throw()
	{
	}
	
	void EmpiricalHSShiftProcessor::init()
		throw()
	{	
		std::cout << "******************* EHS-Shift ******************* " << std::endl;
		// by default, we assume the worst...
		valid_ = false;

		// if no parameters are assigned, abort immediately
		if (parameters_ == 0)
		{
		std::cout << "return" << std::endl;
			return;
		}

		// check that the parameter file contains the correct section...
		ParameterSection parameter_section;
		parameter_section.extractSection(*parameters_, "EmpiricalShiftHyperSurfaces");

		// ..and that this section contains the correct column names
		if (   !parameter_section.hasVariable("name") || !parameter_section.hasVariable("property")
				|| !parameter_section.hasVariable("file")  )
		{
			return;
		}
	
		// clear the arrays of the targets, the targets names and the target_properties
		targets_.clear();
		target_names_.clear();
		target_property_names_.clear();

		
		// extract the column indices
		Position name_column =  parameter_section.getColumnIndex("name");
		Position property_column =  parameter_section.getColumnIndex("property");
		Position file_column =  parameter_section.getColumnIndex("file");
		

		
		// insert per (!) target atom the necessay properties (in a set)
		// and in a map the property pairs and the corresponding hypersurface/spline
		// e.g. an entry could look like this
		// 1						CA          PHI/PSI          	file1
		// so we need to store 
		// - CA as a target name
		// - PHI and PSI as target_property_names
		// - and file1 in a map with the key PHI/PSI
		
		int index = -1;
		String old_target_name = "";
		for (Position counter = 0; counter < parameter_section.getNumberOfKeys(); counter++)
		{
			String target_name = parameter_section.getValue(counter, name_column);
			//create a new vector entry for each new targetatom

	//std::cout << "index" << index << " target: " << target_name << std::endl;
			if (target_name != old_target_name)
			{
				index +=1;
				old_target_name = target_name;
				target_names_.push_back(target_name);

				std::set< String> empty;
				target_property_names_.push_back(empty);
				std::map< std::pair<String, String>, String> empty2;
				property_files_.push_back(empty2);
			}
			
			// split the string at "/" and store the property_names
			String string = parameter_section.getValue(counter, property_column);
			std::vector<String> props;
			string.split(props, "/");
		
			if (props.size()!=2)
			{
				continue;
			}
			target_property_names_[index].insert(props[0]);
			target_property_names_[index].insert(props[1]);
		
			// create a key tuple and insert the filename
			std::pair<String, String> tuple(props[0], props[1]);
			property_files_[index][tuple] =  parameter_section.getValue(counter, file_column);
	
			//property_files_[index].   push_back(parameter_section.getValue(counter, file_column));
		}
		
		// mark the module as initialized
		valid_ = true;
	}

	bool EmpiricalHSShiftProcessor::start()
		throw()
	{	
		std::cout << "******************* EHS-Shift start ******************* " << std::endl;
		// if the module is invalid, abort
		if (!isValid())
		{
			return false;
		}

		// clear the target list
		targets_.clear();

printParameters_();		
		return true;
	}


	bool EmpiricalHSShiftProcessor::finish()
		throw()
	{
		std::cout << "******************* EHS-Shift finish******************* " << std::endl;
		// if the module is in an invalid state, abort
		if (!isValid())
		{
			return false;
		}
		
		printParameters_();	
		printTargets_();
		
		// if there were no targets, return immediately
		if ((targets_.size() == 0))
		{
			return true;
		}
		
		
		return true;
	}
	
		
	Processor::Result  EmpiricalHSShiftProcessor::operator () (Composite& composite)
		throw()
	{
		// Here, we collect all target atoms, specified in the section 
		// "EmpiricalShiftHyperSurfaces" of the ShiftX.ini file,
		// and the corresponding properties which are likewise specified 
		// in the ShiftX.ini-file. Since there is no object in BALL to 
		// store the kind of property in init() and then use it directly here to
		// compute and store these properties, we store the _kind_ of the 
		// property as a string in init() and hardcode here for each
		// string the corresponding property computation.
		if (RTTI::isKindOf<Atom>(composite))
		{
			Atom* atom = dynamic_cast<Atom*>(&composite);
			//std::cout << atom->getName()<< "!!!!" << atom->getFullName()  << std::endl ;
			
			for (Position i = 0; i < target_names_.size(); i++) 
			{
				if (atom->getName() == target_names_[i])
				{	
					// ToDo: We need a constructor
					PropertiesForShift_  property;
					
					property.atom = atom;
					Residue* residue = atom->getResidue();
					if (!residue)
					{
						continue;
					}
					
					Residue* prev_residue = atom->getAncestor(*residue)->getPrevious(*residue);
					Residue* next_residue = atom->getAncestor(*residue)->getNext(*residue);
					
					for (std::set<String>::iterator it = target_property_names_[i].begin();
					 it != target_property_names_[i].end(); it++)
					{	
		//std::cout << "property" << (*it)<< std::endl;

						if (!prev_residue)
						{
							continue;
						}
						else
						{
							if ((*it) == 	"FirstResidue-1" )
							{	
								property.prev_is_first_residue = prev_residue->isTerminal();
							}
							else if ((*it) == 	"AA-1" )
							{
								setAminoAcid_(prev_residue, property.prev_amino_acid_type);
							}else if ((*it) == 	"SS-1" )
							{
								setSecondaryStructure_(prev_residue, property.prev_secondary_structure);
							}else if ((*it) == 	"PSI-1")
							{
								property.prev_psi_angle = prev_residue->getTorsionPsi();
							}else if ((*it) == 	"PHI-1")
							{	
								property.prev_phi_angle = prev_residue->getTorsionPhi();
							}else if ((*it) == 	"CHI-1")
							{
								property.prev_chi_angle = getChiAngle_(prev_residue);
							}else if ((*it) == 	"CHI2-1")
							{
								property.prev_chi2_angle = getChi2Angle_(prev_residue);
							}else if ((*it) == 	"HA1-1" )
							{
								property.prev_HA_bond_len	= getHA_HBondLen_(prev_residue) ;
								//property.prev_has_HA_Hbond;
							}else if ((*it) == 	"HA2-1" )
							{
								property.prev_HA2_bond_len = getHA2_HBondLen_(prev_residue);
								//property.prev_has_HA2_Hbond = ;
							}else if ((*it) == 	"HN-1" )
							{
								//property.prev_HN_bond_len = ;
								//property.prev_has_HN_Hbond;
							}else if ((*it) == 	"OH-1" )
							{
								//property.prev_OH_bond_len = ;
							}else if ((*it) == 	"Disulfid-1" )
							{
								//property.prev_has_disulfid_bond = ;
							}

						}

						if (!next_residue)
						{
							continue;
						}
						else
						{
							if ((*it) == 	"FirstResidue+1" )
							{	
								//std::cout << "FirstResidue +1" << (*it)<< std::endl;
								property.next_is_first_residue = next_residue->isTerminal();
							}
							else if ((*it) == 	"AA+1" )
							{
								setAminoAcid_(next_residue, property.next_amino_acid_type);
							}else if ((*it) == 	"SS+1" )
							{
								setSecondaryStructure_(next_residue, property.next_secondary_structure);
							}else if ((*it) == 	"PSI+1")
							{
								property.next_psi_angle = next_residue->getTorsionPsi();
							}else if ((*it) == 	"PHI+1")
							{	
								property.next_phi_angle = next_residue->getTorsionPhi();
							}else if ((*it) == 	"CHI+1")
							{
								property.next_chi_angle = getChiAngle_(next_residue);
							}else if ((*it) == 	"CHI2+1")
							{
								property.next_chi2_angle = getChi2Angle_(next_residue);
							}else if ((*it) == 	"HA1+1" )
							{
								property.next_HA_bond_len= getHA_HBondLen_(next_residue) ;	
								//property.next_has_HA_Hbond;
							}else if ((*it) == 	"HA2+1" )
							{
								property.next_HA2_bond_len = getHA2_HBondLen_(next_residue);
								//property.next_has_HA2_Hbond;
							}else if ((*it) == 	"HN+1" )
							{
								//property.next_HN_bond_len = ;
								//property.next_has_HN_Hbond;
							}else if ((*it) == 	"OH+1" )
							{
								//property.next_has_disulfid_bond = ;
							}else if ((*it) == 	"Disulfid+1" )
							{
								//property.next_has_disulfid_bond = ;
							}
						}

						
						if ((*it) == "FirstResidue" )
						{
		//std::cout << "FirstResidue" << (*it)<< std::endl;
							property.is_first_residue = residue->isTerminal();
							//std::cout << "FR" << property.is_first_residue << std::endl;
						}else if ((*it) == 	"AA" )
						{	
							setAminoAcid_(residue, property.amino_acid_type);
						}else if ((*it) == 	"SS" )
						{
							setSecondaryStructure_(residue, property.secondary_structure);
						}else if ((*it) == 	"PSI")
						{
							property.psi_angle = residue->getTorsionPsi();
						}else if ((*it) == 		"PHI")
						{		
							property.phi_angle = residue->getTorsionPhi();
						}else if ((*it) == 	"CHI")
						{
							property.chi_angle = 	getChiAngle_(residue);
							//std::cout << "CHI" << property.chi_angle << std::endl;
						}else if ((*it) == 	"CHI2")
						{
							property.chi2_angle = getChi2Angle_(residue);
						}else if ((*it) == 	"HA1" )
						{
							property.HA_bond_len = getHA_HBondLen_(residue); 
							//property.has_HA_Hbond;
						}else if ((*it) == 	"HA2" )
						{
							property.HA2_bond_len =	getHA2_HBondLen_(residue) ;
							//property.has_HA2_Hbond;
						}else if ((*it) == 	"HN" )
						{
							//property.HN_bond_len = ;
							//property.has_HN_Hbond;
						}else if ((*it) == 	"OH" )
						{
							//property.OH_bond_len = ;
							//property.has_O_Hbond = ;
						}else if ((*it) == 	"Disulfid" )
						{
							//property.has_disulfid_bond = ;
						}						
						targets_.push_back(property);
					} 
				}
			}
		}	
		
		return Processor::CONTINUE;
	}
	
	void EmpiricalHSShiftProcessor::printParameters_()
			throw()
	{
		std::cout << "\n EHS:Liste der Parameter " << std::endl;
		std::cout << "\t\n EHS:targetnames \t\ttarget properties \t\t files " << std::endl;
	
		for (Position i = 0; i < target_names_.size(); i++)
		{
			std::cout << "\t\t" << target_names_[i] << "--- ";
		
			for (std::set<String>::iterator it = target_property_names_[i].begin();
					 it != target_property_names_[i].end(); it++)
			{
				std::cout << (*it) << ", ";
			}
			std::cout << "\n  \t\t---";

			for (std::map< std::pair<String, String>, String >::iterator it = property_files_[i].begin();
						it != property_files_[i].end(); it++ )
			{
				std::cout << "(" << (*it).first.first << "," << (*it).first.second << ")  " ;
			}
			std::cout << " \n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << std::endl;
		}
	}
	
	void EmpiricalHSShiftProcessor::printTargets_()
			throw()
	{
		std::cout << "\n EHS:Liste der Targets " << std::endl;
		std::cout << " atom \t FR \t AA \t SS \t PSI \t PHI \t CHI \t CHI2 \t HA \t HA2 \t HN \t OH \t Disulfid " << std::endl;
		for (Position i = 0; i < targets_.size(); i++)
		{
				PropertiesForShift_  p = targets_[i];
				//std::cout << "Blubb: " << p.is_first_residue << std::endl;

				std::cout << p.atom->getName() << "\t" << p.is_first_residue << "\t" << p.amino_acid_type << "\t" 
									<< p.secondary_structure << "\t" << p.psi_angle << "\t" << p.phi_angle << "\t" << p.chi_angle << "\t" 
									<< p.chi2_angle << "\t" << p.HA_bond_len << "\t" << 	p.HA2_bond_len<< "\t" << 	p.HN_bond_len << "\t" 
									<< p.OH_bond_len << "\t" << p.has_disulfid_bond << std::endl;
		}
	}
	
	float EmpiricalHSShiftProcessor::getChiAngle_(Residue* residue) 
		throw()
	{

	//std::cout << "getChiAngle " << residue << std::endl;						
		Atom* N = 0;
		Atom* CA = 0;
		Atom* CB = 0;
		Atom* X = 0;
		
		int num_of_atoms = 0;
		
		AtomIterator r_it = residue->beginAtom();
		for (; r_it != residue->endAtom(); ++r_it)
		{	
			String name = r_it->getName();
			if (name == "N")
			{
				N = &(*r_it);
				num_of_atoms += 1;
			}else if (name == "CA")
			{
				CA = &(*r_it);
				num_of_atoms += 1;
			}else if (name == "CB")
			{ 
				CB = &(*r_it);
				num_of_atoms += 1;
			}
		}

		if (num_of_atoms != 3)
		{
			Log.info() << "Torsion angle of " << residue->getName() << " could not be computed!" << std::endl;
			return 0.;
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
		*/
		String residue_name = residue->getName();
		if (residue_name == "GLY")
		{ 
			Log.info() << "Torsion angle of a Glycine could not be computed!" << std::endl;
			return 0.;
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
		}else if (residue_name == "CYS")
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
		}else if (residue_name == "ASP" || residue_name ==  "GLU" || 
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
		}else if (residue_name == "VAL" || residue_name ==  "ILE")
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
		}else if (residue_name == "SER") 
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
		}else if (residue_name == "THR") 
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
			Log.info() << "Torsion angle of " << residue->getName() << " could not be computed!" << std::endl;
			return 0.;

		}
		Vector3 a = N->getPosition();
		Vector3 b = CA->getPosition();
		Vector3 c = CB->getPosition();
		Vector3 d = X->getPosition();
		
		return getTorsionAngle(a.x, a.y, a.z, b.x, b.y, b.z, 
													 c.x, c.y, c.z, d.x, d.y, d.z);
	}

	
	float EmpiricalHSShiftProcessor::getChi2Angle_(Residue* residue) 
		throw()
	{
		return 0.;
	}

	
	void EmpiricalHSShiftProcessor::setSecondaryStructure_(Residue* residue, char& property) 
		throw()
	{
		if (residue->getSecondaryStructure()->getType() == SecondaryStructure::COIL)
		{
			property = 'C';
		}else if (residue->getSecondaryStructure()->getType() == SecondaryStructure::HELIX)
		{
			property = 'H';
		}else if (residue->getSecondaryStructure()->getType() == SecondaryStructure::STRAND)
		{
			property = 'B';
		}
		return;
	}

	void EmpiricalHSShiftProcessor::setAminoAcid_(Residue* residue, char& property) 
		throw()
	{	
		if (residue->isAminoAcid())
		{
			//	std::cout << residue->getFullName() << " " << residue->getName() << " " << Peptides::OneLetterCode(residue->getName()) << std::endl;
			property = Peptides::OneLetterCode(residue->getName()); 
		}else
		{
			property = ' ';
		}
		return;
	}

	
	float EmpiricalHSShiftProcessor::getHA_HBondLen_(Residue* residue)
		throw()
	{
		float len = 0.;
		Atom* HA = 0;
		Atom* H = 0;
		
		AtomIterator r_it = residue->beginAtom();
		for (; r_it != residue->endAtom(); ++r_it)
		{	
			String name = r_it->getName();
			if (name == "H")
			{
				H = &(*r_it);
			}
			else if (name == "HA")
			{
				HA = &(*r_it);
			}
		}
		
		if (HA && H)
			len = (HA->getPosition() - H->getPosition()).getLength();
			
		return len;
	}	

	
	float EmpiricalHSShiftProcessor::getHA2_HBondLen_(Residue* residue)
		throw()
	{
		float len = 0.;
		Atom* HA2 = 0;
		Atom* H = 0;
		
		AtomIterator r_it = residue->beginAtom();
		for (; r_it != residue->endAtom(); ++r_it)
		{	
			String name = r_it->getName();
			if (name == "H")
			{
				H = &(*r_it);
			}
			else if (name == "HA2")
			{
				HA2 = &(*r_it);
			}
		}
		
		if (HA2 && H)
			len = (HA2->getPosition() - H->getPosition()).getLength();
			
		return len;
	}	

	float EmpiricalHSShiftProcessor::getHN_HBondLen_(Residue* residue)
		throw()
	{
		float len = 0.;
		Atom* HN = 0;
		Atom* H = 0;
		
		AtomIterator r_it = residue->beginAtom();
		for (; r_it != residue->endAtom(); ++r_it)
		{	
			String name = r_it->getName();
			if (name == "H")
			{
				H = &(*r_it);
			}
			else if (name == "HN")
			{
				HN = &(*r_it);
			}
		}
		
		if (HN && H)
			len = (HN->getPosition() - H->getPosition()).getLength();
			
		return len;
	}	

	EmpiricalHSShiftProcessor::PropertiesForShift_::PropertiesForShift_()
		throw()
	{
		is_first_residue = false;
		amino_acid_type = ' ';
		secondary_structure = ' ';
		psi_angle = 0.;
		phi_angle = 0.;
		chi_angle = 0.;
		chi2_angle = 0.;
		HA_bond_len	= 0.;	
		has_HA_Hbond = false; // unknown
		HA2_bond_len = 0.;
		has_HA_Hbond = false; // unknown
		HN_bond_len = 0.;
		has_HN_Hbond = false;
		OH_bond_len = 0.;
		has_disulfid_bond = false;
		prev_is_first_residue = false;
		prev_amino_acid_type = ' ';
		prev_secondary_structure = ' ';
		prev_psi_angle = 0.;
		prev_phi_angle = 0.;
		prev_chi_angle = 0.;
		prev_chi2_angle = 0.;
		prev_HA_bond_len	= 0.;	
		prev_has_HA_Hbond = false; // unknown
		prev_HA2_bond_len = 0.;
		prev_has_HA_Hbond = false; // unknown
		prev_HN_bond_len = 0.;
		prev_has_HN_Hbond = false;
		prev_OH_bond_len = 0.;
		prev_has_disulfid_bond = false;
		next_is_first_residue = false;
		next_amino_acid_type = ' ';
		next_secondary_structure = ' ';
		next_psi_angle = 0.;
		next_phi_angle = 0.;
		next_chi_angle = 0.;
		next_chi2_angle = 0.;
		next_HA_bond_len	= 0.;	
		next_has_HA_Hbond = false; // unknown
		next_HA2_bond_len = 0.;
		next_has_HA_Hbond = false; // unknown
		next_HN_bond_len = 0.;
		next_has_HN_Hbond = false;
		next_OH_bond_len = 0.;
		next_has_disulfid_bond = false;
	}
} // namespace BALL
