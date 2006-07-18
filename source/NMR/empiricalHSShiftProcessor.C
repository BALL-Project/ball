
#include <BALL/NMR/empiricalHSShiftProcessor.h>
#include <BALL/COMMON/limits.h>
#include <BALL/KERNEL/bond.h>
#include <BALL/FORMAT/parameterSection.h>
#include <BALL/STRUCTURE/peptides.h>
#include <BALL/MATHS/analyticalGeometry.h>

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
			std::cout<< "hjkhjkhkjh";
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
			
		int index = -1;
		String old_target_name = "";
		for (Position counter = 0; counter < parameter_section.getNumberOfKeys(); counter++)
		{
			String target_name = parameter_section.getValue(counter, name_column);
			if (target_name != old_target_name)
			{
				index +=1;
				old_target_name = target_name;
				target_names_.push_back(target_name);

				std::vector< String> empty;
				target_property_names_.push_back(empty);
				std::vector< String> empty2;
				property_files_.push_back(empty2);
			}
			target_property_names_[index].push_back(parameter_section.getValue(counter, property_column));
			property_files_[index].push_back(parameter_section.getValue(counter, file_column));
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
			//std::cout << atom->getName()<< "  " << atom->getFullName() << std::endl;
			
			for (Position i = 0; i < target_names_.size(); i++) 
			{
				if (atom->getName() == target_names_[i])
				{
					PropertiesForShift_  property;
					
					property.atom = atom;
					Residue* residue = atom->getResidue();
					Residue* prev_residue = atom->getAncestor(*residue)->getPrevious(*residue);
					Residue* next_residue = atom->getAncestor(*residue)->getNext(*residue);

					for (Position j = 0; j < target_property_names_[i].size(); j++)
					{	
						if (target_property_names_[i][j] == "FirstResidue" )
						{
							property.is_first_residue = residue->isTerminal();
							std::cout << "FR" << property.is_first_residue << std::endl;
						}else if (target_property_names_[i][j] == 	"FirstResidue-1" )
						{	
						//	property.prev_is_first_residue = prev_residue->isTerminal();
						}else if (target_property_names_[i][j] == 	"FirstResidue+1" )
						{	
						//	property.next_is_first_residue = next_residue->getTerminal();
						}else if (target_property_names_[i][j] == 	"AA" )
						{	
							if (residue->isAminoAcid())
							{
							//	std::cout << residue->getFullName() << " " << residue->getName() << " " << Peptides::OneLetterCode(residue->getName()) << std::endl;
								property.amino_acid_type = Peptides::OneLetterCode(residue->getName()); 
							}
						}else if (target_property_names_[i][j] == 	"AA-1" )
						{
							//property.prev_amino_acid_type = ;
						}else if (target_property_names_[i][j] == 	"AA+1" )
						{
							//property.next_amino_acid_type = ;
						}else if (target_property_names_[i][j] == 	"SS" )
						{
							if (residue->getSecondaryStructure()->getType() == SecondaryStructure::COIL)
							{
								property.secondary_structure = 'C';
							}else if (residue->getSecondaryStructure()->getType() == SecondaryStructure::HELIX)
							{
								property.secondary_structure = 'H';
							}else if (residue->getSecondaryStructure()->getType() == SecondaryStructure::STRAND)
							{
								property.secondary_structure = 'B';
							}
						}else if (target_property_names_[i][j] == 	"SS-1" )
						{
							//property.prev_secondary_structure = ;
						}else if (target_property_names_[i][j] == 	"SS+1" )
						{
							//property.next_secondary_structure = ;
						}else if (target_property_names_[i][j] == 	"PSI")
						{
							property.psi_angle = residue->getTorsionPsi();
						}else if (target_property_names_[i][j] == 	"PSI-1")
						{
							//property.prev_psi_angle =
						}else if (target_property_names_[i][j] == 	"PSI+1")
						{
							//property.next_psi_angle =            ->getTorsionPsi() << std::endl;
						}else if (target_property_names_[i][j] == 		"PHI")
						{		
							property.phi_angle = residue->getTorsionPhi();

						}else if (target_property_names_[i][j] == 	"PHI-1")
						{	
							//property.prev_phi_angle = 0.;
						}else if (target_property_names_[i][j] == 	"PHI+1")
						{	
							//property.next_phi_angle = 0.;
						}else if (target_property_names_[i][j] == 	"CHI")
						{
							property.chi_angle = 	getChiAngle_(residue);
							std::cout << "CHI" << property.chi_angle << std::endl;
							//ResidueTorsions::getTorsionAngle();
						}else if (target_property_names_[i][j] == 	"CHI-1")
						{
							//property.prev_chi_angle = 0.;
						}else if (target_property_names_[i][j] == 	"CHI+1")
						{
							//property.next_chi_angle = 0.;
						}else if (target_property_names_[i][j] == 	"CHI2")
						{
							//property.chi2_angle = ;
						}else if (target_property_names_[i][j] == 	"CHI2-1")
						{
							//property.prev_chi2_angle = 0.;
						}else if (target_property_names_[i][j] == 	"CHI2+1")
						{
							//property.next_chi2_angle = 0.;
						}else if (target_property_names_[i][j] == 	"HA1" )
						{
							//property.HA_bond_len = ; 
							//property.has_HA_Hbond;
						}else if (target_property_names_[i][j] == 	"HA1-1" )
						{
							//property.prev_HA_bond_len	= ;
							//property.prev_has_HA_Hbond;
						}else if (target_property_names_[i][j] == 	"HA1+1" )
						{
							//property.next_HA_bond_len;	
							//property.next_has_HA_Hbond;
						}else if (target_property_names_[i][j] == 	"HA2" )
						{
							//property.HA2_bond_len = ;
							//property.has_HA2_Hbond;
						}else if (target_property_names_[i][j] == 	"HA2-1" )
						{
							//property.prev_HA2_bond_len;
							//property.prev_has_HA2_Hbond = ;
						}else if (target_property_names_[i][j] == 	"HA2+1" )
						{
							//property.next_HA2_bond_len;
							//property.next_has_HA2_Hbond;
						}else if (target_property_names_[i][j] == 	"HN" )
						{
							//property.HN_bond_len = ;
							//property.has_HN_Hbond;
						}else if (target_property_names_[i][j] == 	"HN-1" )
						{
							//property.prev_HN_bond_len = ;
							//property.prev_has_HN_Hbond;
						}else if (target_property_names_[i][j] == 	"HN+1" )
						{
							//property.next_HN_bond_len = ;
							//property.next_has_HN_Hbond;
						}else if (target_property_names_[i][j] == 	"OH" )
						{
							//property.OH_bond_len = ;
							//property.has_O_Hbond = ;
						}else if (target_property_names_[i][j] == 	"OH-1" )
						{
							//property.prev_OH_bond_len = ;
						}else if (target_property_names_[i][j] == 	"OH+1" )
						{
							//property.next_has_disulfid_bond = ;
						}else if (target_property_names_[i][j] == 	"Disulfid" )
						{
							//property.has_disulfid_bond = ;
						}else if (target_property_names_[i][j] == 	"Disulfid-1" )
						{
							//property.prev_has_disulfid_bond = ;
						}else if (target_property_names_[i][j] == 	"Disulfid+1" )
						{
							//property.next_has_disulfid_bond = ;
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
			std::cout << "\t\t" << target_names_[i] << "---";
			for (Position j = 0; j < target_property_names_[i].size(); j++)
			{
					std::cout << "(" << target_property_names_[i][j] << "," << property_files_[i][j] << ")  " ;
			}
			std::cout << " " << std::endl;
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
				std::cout << "Blubb: " << p.is_first_residue << std::endl;

				std::cout << p.atom->getName() << "\t" << p.is_first_residue << "\t" << p.amino_acid_type << "\t" 
									<< p.secondary_structure << "\t" << p.psi_angle << "\t" << p.phi_angle << "\t" << p.chi_angle << "\t" 
									<< p.chi2_angle << "\t" << p.HA_bond_len << "\t" << 	p.HA2_bond_len<< "\t" << 	p.HN_bond_len << "\t" 
									<< p.OH_bond_len << "\t" << p.has_disulfid_bond << std::endl;
		}
	}
	
	float EmpiricalHSShiftProcessor::getChiAngle_(Residue* residue) 
		throw()
	{
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
		if (num_of_atoms !=3)
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
				OG1		Thr
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
		}else 
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

} // namespace BALL
