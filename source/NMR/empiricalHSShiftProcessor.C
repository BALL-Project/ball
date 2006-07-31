#include <BALL/FORMAT/PDBFile.h>
#include <BALL/NMR/empiricalHSShiftProcessor.h>
#include <BALL/COMMON/limits.h>
#include <BALL/KERNEL/bond.h>
#include <BALL/FORMAT/parameterSection.h>
#include <BALL/STRUCTURE/peptides.h>
#include <BALL/MATHS/analyticalGeometry.h>
#include <map>
#include <set>
#include <BALL/SYSTEM/path.h>


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
std::cout<< "Anfang "<< std::endl;

		// if no parameters are assigned, abort immediately
		if (parameters_ == 0)
		{
		std::cout << "return" << std::endl;
			return;
		}
std::cout<< "asldkfjasdf "<< std::endl;

		// check that the parameter file contains the correct section...
		ParameterSection parameter_section;
		parameter_section.extractSection(*parameters_, "EmpiricalShiftHyperSurfaces");
std::cout<< "huhu "<< std::endl;

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
		
std::cout<< "HAllo "<< std::endl;

		
		// insert per (!) target atom type the necessay properties (in a set)
		// and in a map the property pairs and the corresponding 
		// hypersurface/spline files
		// 	e.g. an entry in ShiftX.ini could look like this
		// 1						CA          PHI/PSI          	file1
		// so we need to store 
		// - CA as a target name
		// - PHI and PSI as target_property_name__s__ for CA
		// - file1 in a map with the key pair (PHI,PSI) (for CA)
		// - and the pair (PHI, PSI)   (for CA)
		
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
				// store the atom type we need to look fro in this processor
				target_names_.push_back(target_name);

				// create some dummy entries for 
				// 	- target_property_names_
				// 	- property_files_
				// 	- property_pairs_
				std::set< String> empty;
				target_property_names_.push_back(empty);
				std::map< std::pair<String, String>, String> empty2;
				property_files_.push_back(empty2);
				vector<std::pair<String, String> > empty3;
				property_pairs_.push_back(empty3);
			}
			
			// split the string at "/" and store the property_names
			String string = parameter_section.getValue(counter, property_column);
			std::vector<String> props;
			string.split(props, "/");
		
			if (props.size()!=2)
			{
				continue;
			}
			// store the property names
			target_property_names_[index].insert(props[0]);
			target_property_names_[index].insert(props[1]);
				
			// create a key tuple and insert the filename
			std::pair<String, String> tuple(props[0], props[1]);
			//store the file name
			property_files_[index][tuple] =  parameter_section.getValue(counter, file_column);
			//store the property pair
			property_pairs_[index].push_back(tuple);

		}

		
		// for each property pair  
		// create a hypersurface in  hypersurfaces_;

		// for all atom types	
		for (Position i = 0; i < property_files_.size(); i++)
		{		
			std::map< std::pair<String, String>, String >::iterator it = property_files_[i].begin(); 
			for (; it != property_files_[i].end(); it++)
			{
				String atom_type = target_names_[i];
				String first_property = (*it).first.first ;
				String second_property = (*it).first.second;
			std::cout<< "~~~~" <<(*it).first.first << " " << (*it).first.second  << ":  " <<(*it).second << std::endl;
				ShiftHyperSurface_ shs( (*it).second, atom_type, 
															  first_property, second_property); 
																				
				//insert in the list of
				//tuple .. 
				//hypersurfaces_[atom_type]= s
			}
		}
		/*		// find the data file
				Path p;
				String filename = p.find( (*it).second) ;// "NMR/splinedata/hat_PSI_DISULFIDE-1.dat");
				if (filename == "")
				{
					Log.info () << "File " <<(*it).second << " could not be found" << std::endl;
				}

				// read the datafile
				BALL::File file(filename, std::ios::in);
				String line;
				std::vector<BALL::String> fields;

				// read the first line and decide which spline has to be build
				line.getline(file);
				line.split(fields, ";");
				for ( Position i = 0; i < fields.size(); i++)
				{
					;
				}
				while (file.good())
				{
					line.getline(file);
					line.split(fields, ";");
					std::cout << fields.size() << std::endl;	
				}
			}
		}
		//std::vector< std::map< std::pair<String, String>, String >	>		property_files_;

		CubicSpline2D s2d;
		*/

		
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
			//std::cout << atom->getName()<< "!!!!" << atom->getFullName()  << std::endl ;
			
			for (Position i = 0; i < target_names_.size(); i++) 
			{
				if (atom->getName() == target_names_[i])
				{	
					PropertiesForShift_  property;
					if( property.computeProperties_(atom, target_property_names_[i]))
						targets_.push_back(property);
					
				/*		
					property.atom = atom;
					Residue* residue = atom->getResidue();
					if (!residue)
					{
						continue;
					}
					
					Residue* prev_residue = atom->getAncestor(*residue)->getPrevious(*residue);
					Residue* next_residue = atom->getAncestor(*residue)->getNext(*residue);
					
					// compute all properties needed for this atom type 
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
							if ((*it) == 	"FR_P" )
							{	
								//property.prev_is_first_residue = prev_residue->isTerminal();
								property.properties_string_[(*it)]= (prev_residue->isTerminal() ? "Y": "N");
							}
							else if ((*it) == 	"AA_P" )
							{
							//	setAminoAcid_(prev_residue, property.prev_amino_acid_type);	
								property.properties_string_[(*it)]= getAminoAcid_(prev_residue);

							}else if ((*it) == 	"SS_P" )
							{
							//	setSecondaryStructure_(prev_residue, property.prev_secondary_structure);	
								property.properties_string_[(*it)]= getSecondaryStructure_(prev_residue);

							}else if ((*it) == 	"PSI_P")
							{
							//	property.prev_psi_angle = prev_residue->getTorsionPsi();	
								property.properties_real_[(*it)]=prev_residue->getTorsionPsi();	
							}else if ((*it) == 	"PHI_P")
							{	
							//	property.prev_phi_angle = prev_residue->getTorsionPhi();	
								property.properties_real_[(*it)]= prev_residue->getTorsionPhi();	
							}else if ((*it) == 	"CHI_P")
							{
						//		property.prev_chi_angle = getChiAngle_(prev_residue);	
								property.properties_real_[(*it)]=getChiAngle_(prev_residue);	
							}else if ((*it) == 	"CHI2_P")
							{
						//		property.prev_chi2_angle = getChi2Angle_(prev_residue);	
								property.properties_real_[(*it)]=getChi2Angle_(prev_residue);	
							}else if ((*it) == 	"HA1L_P" )
							{
						//		property.prev_HA_HBond_len	= getHA_HBondLen_(prev_residue);	
								property.properties_real_[(*it)]= getHA_HBondLen_(prev_residue);	
							}else if ((*it) == 	"HA1_P" )
							{
					//			property.prev_has_HA_HBond = hasHA_HBond_(prev_residue);	
								property.properties_string_[(*it)]= (hasHA_HBond_(prev_residue)? "Y": "N");	
							}else if ((*it) == 	"HA2L_P" )
							{
					//			property.prev_HA2_HBond_len = getHA2_HBondLen_(prev_residue);	
								property.properties_real_[(*it)]= getHA2_HBondLen_(prev_residue);	
							}else if ((*it) == 	"HA2_P" )
							{
				//				property.prev_has_HA2_HBond = hasHA2_HBond_(prev_residue);
								property.properties_string_[(*it)]= (hasHA2_HBond_(prev_residue) ? "Y": "N");	
							}else if ((*it) == 	"HNL_P" )
							{
					//			property.prev_HN_HBond_len =  getHN_HBondLen_(prev_residue);	
								property.properties_real_[(*it)]= getHN_HBondLen_(prev_residue);	
							}else if ((*it) == "HN_P" )
							{
						//		property.prev_has_HN_HBond = hasHN_HBond_(prev_residue);
								property.properties_string_[(*it)]  = (hasHN_HBond_(prev_residue)? "Y": "N");
							}else if ((*it) == 	"OHL_P" )
							{
					//			property.prev_O_HBond_len = getO_HBondLen_(prev_residue);	
								property.properties_real_[(*it)] = getO_HBondLen_(prev_residue);	
							}else if ((*it) == 	"OH_P" )
							{
					//			property.prev_has_O_HBond = hasO_HBond_(prev_residue);
								property.properties_string_[(*it)] = (hasO_HBond_(prev_residue) ? "Y": "N");	
							}else if ((*it) == 	"DISULFIDE_P" )
							{
					//			property.prev_has_disulfid_bond = hasDisulfidBond_(prev_residue);	
								property.properties_string_[(*it)]= (hasDisulfidBond_(prev_residue)  ? "Y": "N");	
							}
						}

						if (!next_residue)
						{
							continue;
						}
						else
						{
							if ((*it) == 	"FR_N" )
							{	
								//std::cout << "FirstResidue _N" << (*it)<< std::endl;
					//			property.next_is_first_residue = next_residue->isTerminal();	
								property.properties_string_[(*it)]=  (next_residue->isTerminal()? "Y": "N");	
							}
							else if ((*it) == 	"AA_N" )
							{
							//	setAminoAcid_(next_residue, property.next_amino_acid_type);	
								property.properties_string_[(*it)]= getAminoAcid_(next_residue);
							}else if ((*it) == 	"SS_N" )
							{
								//setSecondaryStructure_(next_residue, property.next_secondary_structure);	
								property.properties_string_[(*it)]= getSecondaryStructure_(next_residue);

							}else if ((*it) == 	"PSI_N")
							{
						//		property.next_psi_angle = next_residue->getTorsionPsi();	
								property.properties_real_[(*it)]= next_residue->getTorsionPsi();
							}else if ((*it) == 	"PHI_N")
							{	
						//		property.next_phi_angle = next_residue->getTorsionPhi();	
								property.properties_real_[(*it)]= next_residue->getTorsionPhi();	
							}else if ((*it) == 	"CHI_N")
							{
					//			property.next_chi_angle = getChiAngle_(next_residue);	
								property.properties_real_[(*it)]=  getChiAngle_(next_residue);
							}else if ((*it) == 	"CHI2_N")
							{
					//			property.next_chi2_angle = getChi2Angle_(next_residue);	
								property.properties_real_[(*it)]=  getChi2Angle_(next_residue);	
							}else if ((*it) == 	"HA1L_N" )
							{
				//				property.next_HA_HBond_len = getHA_HBondLen_(next_residue) ;		
								property.properties_real_[(*it)]= getHA_HBondLen_(next_residue) ;		
							}else if((*it) == 	"HA1_N")
							{
					//			property.next_has_HA_HBond = hasHA_HBond_(next_residue)  ;	
								property.properties_string_[(*it)]= (hasHA_HBond_(next_residue)? "Y": "N");	
							}else if ((*it) == 	"HA2L_N" )
							{
				//				property.next_HA2_HBond_len = getHA2_HBondLen_(next_residue);	
								property.properties_real_[(*it)]= getHA2_HBondLen_(next_residue);	
							}else if ((*it) == 	"HA2_N" )
							{
				//				property.next_has_HA2_HBond = hasHA2_HBond_(next_residue);
								property.properties_string_[(*it)]= (hasHA2_HBond_(next_residue)? "Y": "N");	
							}else if ((*it) == 	"HNL_N" )
							{
				//				property.next_HN_HBond_len = getHN_HBondLen_(next_residue);	
								property.properties_real_[(*it)]= getHN_HBondLen_(next_residue);
							}else if ((*it) == 	"HN_N" )
							{
				//				property.next_has_HN_HBond = hasHN_HBond_(next_residue);	
								property.properties_string_[(*it)]= (hasHN_HBond_(next_residue)? "Y": "N");	
							}else if ((*it) == 	"OHL_N" )
							{
				//				property.next_O_HBond_len = getO_HBondLen_(next_residue);	
								property.properties_real_[(*it)]= getO_HBondLen_(next_residue);	
							}else if ((*it) == 	"OH_N" )
							{
				//				property.next_has_O_HBond = hasO_HBond_(next_residue);	
								property.properties_string_[(*it)]= (hasO_HBond_(next_residue)? "Y": "N");	
	
							}else if ((*it) == 	"DISULFIDE_N" )
							{
				//				property.next_has_disulfid_bond =	hasDisulfidBond_(next_residue) ;	
								property.properties_string_[(*it)]= 	(hasDisulfidBond_(next_residue)? "Y": "N");		
							}
						}

						
						if ((*it) == "FR" )
						{
		//std::cout << "FirstResidue" << (*it)<< std::endl;
				//			property.is_first_residue = residue->isTerminal();	
							property.properties_string_[(*it)]=  (residue->isTerminal() ? "Y": "N");			
							//std::cout << "FR" << property.is_first_residue << std::endl;
						}else if ((*it) == 	"AA" )
						{	
							//setAminoAcid_(residue, property.amino_acid_type);	
							property.properties_string_[(*it)]=  getAminoAcid_(residue);
						}else if ((*it) == 	"SS" )
						{
							//setSecondaryStructure_(residue, property.secondary_structure);	
							property.properties_string_[(*it)]=  getSecondaryStructure_(residue);
						}else if ((*it) == 	"PSI")
						{
				//			property.psi_angle = residue->getTorsionPsi(); 
							property.properties_real_[(*it)]= residue->getTorsionPsi();
						}else if ((*it) == 		"PHI")
						{		
				//			property.phi_angle = residue->getTorsionPhi();	
							property.properties_real_[(*it)]= residue->getTorsionPhi();	
						}else if ((*it) == 	"CHI")
						{
				//			property.chi_angle = 	getChiAngle_(residue);	
							property.properties_real_[(*it)]= 	getChiAngle_(residue);	
							//std::cout << "CHI" << property.chi_angle << std::endl;
						}else if ((*it) == 	"CHI2")
						{
				//			property.chi2_angle = getChi2Angle_(residue);	
							property.properties_real_[(*it)]= getChi2Angle_(residue);	
						}else if ((*it) == 	"HA1L" )
						{
			//				property.HA_HBond_len = getHA_HBondLen_(residue); 	
							property.properties_real_[(*it)]= getHA_HBondLen_(residue); 	
						}else if ((*it) == 	"HA1" )
						{ 	
			//				property.has_HA_HBond = hasHA_HBond_(residue);	
							property.properties_string_[(*it)]=( hasHA_HBond_(residue)? "Y": "N");
						}else if ((*it) == 	"HA2L" )
						{
			//				property.HA2_HBond_len =	getHA2_HBondLen_(residue) ;	
							property.properties_real_[(*it)]= getHA2_HBondLen_(residue) ;	
						}else if ((*it) == 	"HA2" )
						{ 	
			//				property.has_HA2_HBond = hasHA2_HBond_(residue);	
							property.properties_string_[(*it)]= (hasHA2_HBond_(residue)? "Y": "N");		
						}else if ((*it) == 	"HNL" )
						{
			//				property.HN_HBond_len = getHN_HBondLen_(residue) ;	
							property.properties_real_[(*it)]= getHN_HBondLen_(residue) ;	
						}else if ((*it) == 	"HN" )
						{
			//				property.has_HN_HBond = hasHN_HBond_(residue);	
							property.properties_string_[(*it)]=  (hasHN_HBond_(residue)? "Y": "N");		
						}else if ((*it) == 	"OHL" )
						{
			//				property.O_HBond_len = getO_HBondLen_(residue);	
							property.properties_real_[(*it)]= getO_HBondLen_(residue);	
						}else if ((*it) == 	"OH" )
						{
			//				property.has_O_HBond = hasO_HBond_(residue);	
							property.properties_string_[(*it)]= (hasO_HBond_(residue)? "Y": "N");		
						}else if ((*it) == 	"DISULFIDE" )
						{
			//				property.has_disulfid_bond =	hasDisulfidBond_(residue) ;	
							property.properties_string_[(*it)]= (	hasDisulfidBond_(residue)? "Y": "N");			
						}						
					}*/ 
					//targets_.push_back(property);
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
		std::cout << " atom\tFR\tAA\tSS\tPSI\tPHI\tCHI\tCHI2\tHAL\tHA\tHA2L\tHA2\tHNL\tHN\tOHL\tOH\tDisulfid" << std::endl;
		for (Position i = 0; i < targets_.size(); i++)
		{
				PropertiesForShift_  p = targets_[i];
				//std::cout << "Blubb: " << p.is_first_residue << std::endl;

		//		std::cout << p.atom->getName() << "\t" << p.is_first_residue << "\t" << p.amino_acid_type << "\t" 
			//						<< p.secondary_structure << "\t" << p.psi_angle << "\t" << p.phi_angle << "\t" << p.chi_angle << "\t" 
		//							<< p.chi2_angle << "\t" << p.HA_HBond_len << "\t"<< p.has_HA_HBond << "\t"
	//								<< p.HA2_HBond_len<< "\t"<< p.has_HA2_HBond<<"\t" << 	p.HN_HBond_len << "\t" << p.has_HN_HBond<<"\t"
	//								<< p.O_HBond_len << "\t" << p.has_O_HBond << "\t" << p.has_disulfid_bond << std::endl;	
		//		std::cout << p.atom->getName() << "\t" << p.properties_string_["FR"] << "\t" << p.properties_string_["AA"] << "\t" 
			//						<< p.properties_string_["SS"]<< "\t" << p.properties_real_["PSI"]<< "\t" << p.properties_real_["PHI"]<< "\t" 
			//						<< p.properties_real_["CHI"]<< "\t" << p.properties_real_["CHI2"]<< "\t"<< p.properties_real_["HA1L"] << "\t"
			//						<< p.properties_string_["HA1"] << "\t" << p.properties_real_["HA2L"]<< "\t"<< p.properties_string_["HA2"]<<"\t" 
			//						<< p.properties_real_["HNL"] << "\t" << p.properties_string_["HN"]<<"\t"
			//						<< p.properties_real_["OHL"] << "\t" << p.properties_string_["OH"] << "\t" 
			//						<< p.properties_string_["DISULFIDE"]<< std::endl;
				std::cout << p.atom->getName() << "\t" << p["FR"].second << "\t" << p["AA"].second << "\t" 
									<< p["SS"].second << "\t" << p["PSI"].first 	 << "\t"    << p["PHI"].first << "\t" 
									<< p["CHI"].first << "\t" << p["CHI2"].second  << "\t"  << p["HA1L"].first << "\t"
									<< p["HA1"].first << "\t" << p["HA2L"].first  << "\t"   << p["HA2"].second << "\t" 
									<< p["HNL"].first << "\t" << p["HN"].second   <<  "\t"
									<< p["OHL"].first << "\t" << p["OH"].second   << "\t" 
									<< p["DISULFIDE"].second << std::endl;

		}
	}
	

// 
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~:CubicSpline:~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// 

	void EmpiricalHSShiftProcessor::CubicSpline1D_::createSpline(const std::vector<float>& sample_positions, 
											const std::vector<float>& sample_values)
	{
		float p, qn, sig,un;
		std::vector<float> u;

		sample_positions_ = sample_positions;
		sample_values_ = sample_values;
		int n = sample_positions.size();

		//initialize the vectors
		curvature_.resize(n,0.);
		u.resize(n,0.); 

		//natural spline
		curvature_[0] = 0.;        //-0.5;
		u[0] = 0.0;                // (3.0/(sample_positions[1]-sample_positions[0]))*((sample_values[1]-sample_values[0])/(sample_positions[1]-sample_positions[0])-yp1);

		for (int i=1;i < n-1;i++) 
		{

//			std::cout << "i" <<  i << " " << sample_positions_.size() << " " << sample_values.size() << std::endl;
			//This is the decomposition loop of the tridiagonal algorithm.
			//curvature_ and u are used for temporary
			//storage of the decomposed factors.
			sig=(sample_positions[i]-sample_positions[i-1])/(sample_positions[i+1]-sample_positions[i-1]);
			p=sig*curvature_[i-1]+2.0;
			curvature_[i]=(sig-1.0)/p;
			u[i]=(sample_values[i+1]-sample_values[i])/(sample_positions[i+1]-sample_positions[i]) - (sample_values[i]-sample_values[i-1])/(sample_positions[i]-sample_positions[i-1]);
			u[i]=(6.0*u[i]/(sample_positions[i+1]-sample_positions[i-1])-sig*u[i-1])/p;
		}

		// for natural splines 
		qn = 0.0;
		un = 0.0; 
		/*else
		 * we have a specified first derivative.
		 qn=0.5;
		 un=(3.0/(sample_positions[n]-sample_positions[n-1]))*(ypn-(sample_values[n]-sample_values[n-1])/(sample_positions[n]-sample_positions[n-1]));
		 */

		curvature_[n-1]=(un-qn*u[n-2])/(qn*curvature_[n-2]+1.0);

		//This is the backsubstitution loop of the tridiagonal algorithm.
		for (int k=n-2;k>=0;k--) 
		{	
			curvature_[k]=curvature_[k]*curvature_[k+1]+u[k]; 
		}	
		return;	
	}

	vector<float>& EmpiricalHSShiftProcessor::CubicSpline1D_::getCurvature()
	{
		std::cout <<"getCurvature" << std::endl;
		return curvature_;
	}				

	void EmpiricalHSShiftProcessor::CubicSpline1D_::setCurvature(std::vector<float> curvature)
	{
		std::cout <<"setCurvature" << std::endl;

		curvature_ = curvature;
	}
	
	void EmpiricalHSShiftProcessor::CubicSpline1D_::setValues(std::vector<float> values)
	{
		std::cout << "setValues"<< std::endl;

		sample_values_ = values;
	}
	
	void EmpiricalHSShiftProcessor::CubicSpline1D_::setPositions(std::vector<float> positions)
	{
		std::cout <<"setPositions" << std::endl;

		sample_positions_= positions;
	}

	float EmpiricalHSShiftProcessor::CubicSpline1D_::operator() (float x)
	{
		unsigned int n=sample_positions_.size();
		// is this x position allowed?
		if ((x < sample_positions_[0]) || (x>sample_positions_[n-1]))
		{
			// something _really_ bad happened
			std::cerr << "invalid x position " << x << std::endl;
			std::cerr << "(not between "<< sample_positions_[0] << " and "<< sample_positions_[n-1] <<")" << std::endl;
			return std::numeric_limits<float>::min();
		}

		// first, we find the indices bracketing the value x. we use bisection here
		int lower_bound=0, upper_bound=n-1;
		int index;
		while (upper_bound - lower_bound > 1) 
		{
			index=(upper_bound + lower_bound)/2;
			if (sample_positions_[index] > x) upper_bound=index;
			else lower_bound=index;
		} 
		float spacing=sample_positions_[upper_bound]-sample_positions_[lower_bound];
		if (spacing == 0.0)
		{
			std::cerr << "Zero length interval" << std::endl;
			return std::numeric_limits<float>::min();
		}

		float a = (sample_positions_[upper_bound]-x)/spacing; 
		float b = (x-sample_positions_[lower_bound])/spacing;

		float result = a*sample_values_[lower_bound]+b*sample_values_[upper_bound]
			+((a*a*a-a)*curvature_[lower_bound]+(b*b*b-b)*curvature_[upper_bound])*(spacing*spacing)/6.0;

		return result;
	}


	void EmpiricalHSShiftProcessor::CubicSpline2D_::createBiCubicSpline(const std::vector<float>& sample_positions_x, 
																				const std::vector<float>& sample_positions_y, 
																			  const std::vector<std::vector<float> >& sample_values) 
	{
		std::vector<std::vector<float> > complete_x_positions(sample_positions_y.size());
		for (Position i=0; i<complete_x_positions.size(); i++)
		{
			complete_x_positions[i] = sample_positions_x;
		}
		createBiCubicSpline(complete_x_positions, sample_positions_y, sample_values);
	}

	void EmpiricalHSShiftProcessor::CubicSpline2D_::createBiCubicSpline(const std::vector<std::vector<float> >& sample_positions_x, 
																				const std::vector<float>& sample_positions_y, 
																			  const std::vector<std::vector<float> >& sample_values) 
	{
		std::cout <<"createBICubicSpline" << std::endl;

		sample_positions_x_=sample_positions_x;
		sample_positions_y_=sample_positions_y;
		int m = sample_positions_x_.size();

		CubicSpline1D_ cs;

		for (int j = 0; j < m; j++)
		{
			// compute a 1Dspline
			cs.createSpline(sample_positions_x[j], sample_values[j]);

			// store the spline
			splines_.push_back(cs);	
		}

	}

	float EmpiricalHSShiftProcessor::CubicSpline2D_::operator () (float x, float y)
	{

		CubicSpline1D_ cs;

		std::vector<float> positions;
		std::vector<float> values; 

		// number of rows
		int n = sample_positions_y_.size();  

		//evaluate the stored help table at position x
		for (int i = 0; i < n ;i++)
		{
			values.push_back(splines_[i](x));
		}

		// construct a new spline at these positions
		cs.createSpline(sample_positions_y_, values);

		//evaluate the new spline at position y
		return cs(y);
	}



// 
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~:PropertiesForShift_:~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// 



	
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
		HA_HBond_len	= 0.;	
		has_HA_HBond = false; // unknown
		HA2_HBond_len = 0.;
		has_HA2_HBond = false; // unknown
		HN_HBond_len = 0.;
		has_HN_HBond = false;
		O_HBond_len = 0.;	
		has_O_HBond = false;
		has_disulfid_bond = false;
		prev_is_first_residue = false;
		prev_amino_acid_type = ' ';
		prev_secondary_structure = ' ';
		prev_psi_angle = 0.;
		prev_phi_angle = 0.;
		prev_chi_angle = 0.;
		prev_chi2_angle = 0.;
		prev_HA_HBond_len	= 0.;	
		prev_has_HA_HBond = false; // unknown
		prev_HA2_HBond_len = 0.;
		prev_has_HA_HBond = false; // unknown
		prev_HN_HBond_len = 0.;
		prev_has_HN_HBond = false;
		prev_O_HBond_len = 0.;	
		prev_has_O_HBond = false;
		prev_has_disulfid_bond = false;
		next_is_first_residue = false;
		next_amino_acid_type = ' ';
		next_secondary_structure = ' ';
		next_psi_angle = 0.;
		next_phi_angle = 0.;
		next_chi_angle = 0.;
		next_chi2_angle = 0.;
		next_HA_HBond_len	= 0.;	
		next_has_HA_HBond = false; // unknown
		next_HA2_HBond_len = 0.;
		next_has_HA_HBond = false; // unknown
		next_HN_HBond_len = 0.;
		next_has_HN_HBond = false;
		next_O_HBond_len = 0.;
		next_has_O_HBond = false;
		next_has_disulfid_bond = false;
	}
	
	float EmpiricalHSShiftProcessor::PropertiesForShift_::getChiAngle_(Residue* residue) 
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

	
	float EmpiricalHSShiftProcessor::PropertiesForShift_::getChi2Angle_(Residue* residue) 
		throw()
	{
		return 0.;
	}

	
	char EmpiricalHSShiftProcessor::PropertiesForShift_::getSecondaryStructure_(Residue* residue) 
		throw()
	{
		char ret = ' ';
		if (residue->getSecondaryStructure() == 0)
		{
			ret = '?';
			Log.info() << "No secondary structure available. Consider precomputing !" << std::endl;
		}
		else if (residue->getSecondaryStructure()->getType() == SecondaryStructure::COIL)
		{
			ret = 'C';
		}else if (residue->getSecondaryStructure()->getType() == SecondaryStructure::HELIX)
		{
			ret = 'H';
		}else if (residue->getSecondaryStructure()->getType() == SecondaryStructure::STRAND)
		{
			ret = 'B';
		}
		return ret;
	}

	char EmpiricalHSShiftProcessor::PropertiesForShift_::getAminoAcid_(Residue* residue)
		throw()
	{	
		char ret = ' ';
		if (residue->isAminoAcid())
		{
			//	std::cout << residue->getFullName() << " " << residue->getName() << " " << Peptides::OneLetterCode(residue->getName()) << std::endl;
			ret = Peptides::OneLetterCode(residue->getName()); 
		}
		return ret;
	}

	
	float EmpiricalHSShiftProcessor::PropertiesForShift_::getHA_HBondLen_(Residue* residue)
		throw()
	{
		float len = 0.;
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
				if(bi->getType() == Bond::TYPE__HYDROGEN);
				{
					len = bi->getLength();
				}
			}
		}	
		return len;
	}	

	
	float EmpiricalHSShiftProcessor::PropertiesForShift_::getHA2_HBondLen_(Residue* residue)
		throw()
	{
		float len = 0.;
		Atom* HA2 = 0;
		
		AtomIterator r_it = residue->beginAtom();
		for (; r_it != residue->endAtom(); ++r_it)
		{	
			String name = r_it->getName();
			if (name == "HA2")
			{
				HA2 = &(*r_it);
			}
		}
		
		if (HA2)
		{	
			Atom::BondIterator bi = HA2->beginBond();
			for (;+bi;++bi)
			{	
				if(bi->getType() == Bond::TYPE__HYDROGEN);
				{
					len = bi->getLength();
				}
			}
		}	
		return len;
	}	

	float EmpiricalHSShiftProcessor::PropertiesForShift_::getHN_HBondLen_(Residue* residue)
		throw()
	{
		float len = 0.;
		Atom* HN = 0;
		
		AtomIterator r_it = residue->beginAtom();
		for (; r_it != residue->endAtom(); ++r_it)
		{	
			String name = r_it->getName();
			if (name == "HN")
			{
				HN = &(*r_it);
			}
		}
		
		if (HN)
		{
			Atom::BondIterator bi = HN->beginBond();
			for (;+bi;++bi)
			{	
				if(bi->getType() == Bond::TYPE__HYDROGEN);
				{
					len = bi->getLength();
				}
			}
		}	
		return len;
	}	

//TODO!!!!!!!!!!!!!!!!!!!!!!!1
	float 	EmpiricalHSShiftProcessor::PropertiesForShift_::getO_HBondLen_(Residue* residue) 
		throw()
	{
		float len = 0.;

		return len;
	}
		
	bool 		EmpiricalHSShiftProcessor::PropertiesForShift_::hasDisulfidBond_(Residue* residue)
		throw()
	{
		bool ret = false;
		Atom* SG = 0;
		
		AtomIterator r_it = residue->beginAtom();
		for (; r_it != residue->endAtom(); ++r_it)
		{	
			String name = r_it->getName();
			if (name == "SG")
			{
				SG = &(*r_it);
			}
		}
		
		if (SG)
		{
			Atom::BondIterator bi = SG->beginBond();
			for (;+bi;++bi)
			{	
				if(bi->getType() == Bond::TYPE__DISULPHIDE_BRIDGE);
				{
					ret = true;
				}
			}
		}	
		return ret; 
	}
	
	bool 		EmpiricalHSShiftProcessor::PropertiesForShift_::hasHA_HBond_(Residue* residue) 
		throw()
	{		
		bool ret= false;
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
				if(bi->getType() == Bond::TYPE__HYDROGEN);
				{
					ret = true;
				}
			}
		}	

		return ret; 
	}
	

	bool 		EmpiricalHSShiftProcessor::PropertiesForShift_::hasHA2_HBond_(Residue* residue) 
		throw()
	{
		bool ret = false; 
		Atom* HA2 = 0;
		
		AtomIterator r_it = residue->beginAtom();
		for (; r_it != residue->endAtom(); ++r_it)
		{	
			String name = r_it->getName();
			if (name == "HA2")
			{
				HA2 = &(*r_it);
			}
		}
		
		if (HA2)
		{	
			Atom::BondIterator bi = HA2->beginBond();
			for (;+bi;++bi)
			{	
				if(bi->getType() == Bond::TYPE__HYDROGEN);
				{
					ret = true;
				}
			}
		}	
		return ret;
	}

	bool 		EmpiricalHSShiftProcessor::PropertiesForShift_::hasHN_HBond_(Residue* residue)
		throw()
	{
		bool ret = false;
		Atom* HN = 0;
		
		AtomIterator r_it = residue->beginAtom();
		for (; r_it != residue->endAtom(); ++r_it)
		{	
			String name = r_it->getName();
			if (name == "HN")
			{
				HN = &(*r_it);
			}
		}
		
		if (HN)
		{
			Atom::BondIterator bi = HN->beginBond();
			for (;+bi;++bi)
			{	
				if(bi->getType() == Bond::TYPE__HYDROGEN);
				{
					ret = true;	
				}
			}
		}	
		return ret;
	}

	bool 		EmpiricalHSShiftProcessor::PropertiesForShift_::hasO_HBond_(Residue* residue) 
		throw()
	{
		return false; 
	}


	bool EmpiricalHSShiftProcessor::PropertiesForShift_::computeProperties_(Atom* a, std::set<String> properties) 
		throw()
	{				
		atom = a;
		Residue* residue = atom->getResidue();
		if (!residue)
		{
			return false;
		}

		Residue* prev_residue = atom->getAncestor(*residue)->getPrevious(*residue);
		Residue* next_residue = atom->getAncestor(*residue)->getNext(*residue);

		// compute all properties needed for this atom type 
		//for (std::set<String>::iterator it = target_property_names_[i].begin();
		for (std::set<String>::iterator it = properties.begin();
				it != properties.end(); it++)
		{	
			//std::cout << "property" << (*it)<< std::endl;

			if (!prev_residue)
			{
				return false;
			}
			else
			{
				if ((*it) == 	"FR_P" )
				{	
					properties_string_[(*it)]= (prev_residue->isTerminal() ? "Y": "N");
				}
				else if ((*it) == 	"AA_P" )
				{
					properties_string_[(*it)]= getAminoAcid_(prev_residue);

				}else if ((*it) == 	"SS_P" )
				{
					properties_string_[(*it)]= getSecondaryStructure_(prev_residue);

				}else if ((*it) == 	"PSI_P")
				{
					properties_real_[(*it)]=prev_residue->getTorsionPsi();	
				}else if ((*it) == 	"PHI_P")
				{	
					properties_real_[(*it)]= prev_residue->getTorsionPhi();	
				}else if ((*it) == 	"CHI_P")
				{
					properties_real_[(*it)]=getChiAngle_(prev_residue);	
				}else if ((*it) == 	"CHI2_P")
				{
					properties_real_[(*it)]=getChi2Angle_(prev_residue);	
				}else if ((*it) == 	"HA1L_P" )
				{
					properties_real_[(*it)]= getHA_HBondLen_(prev_residue);	
				}else if ((*it) == 	"HA1_P" )
				{
					properties_string_[(*it)]= (hasHA_HBond_(prev_residue)? "Y": "N");	
				}else if ((*it) == 	"HA2L_P" )
				{
					properties_real_[(*it)]= getHA2_HBondLen_(prev_residue);	
				}else if ((*it) == 	"HA2_P" )
				{
					properties_string_[(*it)]= (hasHA2_HBond_(prev_residue) ? "Y": "N");	
				}else if ((*it) == 	"HNL_P" )
				{
					properties_real_[(*it)]= getHN_HBondLen_(prev_residue);	
				}else if ((*it) == "HN_P" )
				{
					properties_string_[(*it)]  = (hasHN_HBond_(prev_residue)? "Y": "N");
				}else if ((*it) == 	"OHL_P" )
				{
					properties_real_[(*it)] = getO_HBondLen_(prev_residue);	
				}else if ((*it) == 	"OH_P" )
				{
					properties_string_[(*it)] = (hasO_HBond_(prev_residue) ? "Y": "N");	
				}else if ((*it) == 	"DISULFIDE_P" )
				{
					properties_string_[(*it)]= (hasDisulfidBond_(prev_residue)  ? "Y": "N");	
				}
			}

			if (!next_residue)
			{
				return false;
			}
			else
			{
				if ((*it) == 	"FR_N" )
				{	
					properties_string_[(*it)]=  (next_residue->isTerminal()? "Y": "N");	
				}
				else if ((*it) == 	"AA_N" )
				{
					properties_string_[(*it)]= getAminoAcid_(next_residue);
				}else if ((*it) == 	"SS_N" )
				{
					properties_string_[(*it)]= getSecondaryStructure_(next_residue);

				}else if ((*it) == 	"PSI_N")
				{
					properties_real_[(*it)]= next_residue->getTorsionPsi();
				}else if ((*it) == 	"PHI_N")
				{	
					properties_real_[(*it)]= next_residue->getTorsionPhi();	
				}else if ((*it) == 	"CHI_N")
				{
					properties_real_[(*it)]=  getChiAngle_(next_residue);
				}else if ((*it) == 	"CHI2_N")
				{
					properties_real_[(*it)]=  getChi2Angle_(next_residue);	
				}else if ((*it) == 	"HA1L_N" )
				{
					properties_real_[(*it)]= getHA_HBondLen_(next_residue) ;		
				}else if((*it) == 	"HA1_N")
				{
					properties_string_[(*it)]= (hasHA_HBond_(next_residue)? "Y": "N");	
				}else if ((*it) == 	"HA2L_N" )
				{
					properties_real_[(*it)]= getHA2_HBondLen_(next_residue);	
				}else if ((*it) == 	"HA2_N" )
				{
					properties_string_[(*it)]= (hasHA2_HBond_(next_residue)? "Y": "N");	
				}else if ((*it) == 	"HNL_N" )
				{
					properties_real_[(*it)]= getHN_HBondLen_(next_residue);
				}else if ((*it) == 	"HN_N" )
				{
					properties_string_[(*it)]= (hasHN_HBond_(next_residue)? "Y": "N");	
				}else if ((*it) == 	"OHL_N" )
				{
					properties_real_[(*it)]= getO_HBondLen_(next_residue);	
				}else if ((*it) == 	"OH_N" )
				{
					properties_string_[(*it)]= (hasO_HBond_(next_residue)? "Y": "N");	

				}else if ((*it) == 	"DISULFIDE_N" )
				{
					properties_string_[(*it)]= 	(hasDisulfidBond_(next_residue)? "Y": "N");		
				}
			}


			if ((*it) == "FR" )
			{
				properties_string_[(*it)]=  (residue->isTerminal() ? "Y": "N");			
			}else if ((*it) == 	"AA" )
			{	
				properties_string_[(*it)]=  getAminoAcid_(residue);
			}else if ((*it) == 	"SS" )
			{
				properties_string_[(*it)]=  getSecondaryStructure_(residue);
			}else if ((*it) == 	"PSI")
			{
				properties_real_[(*it)]= residue->getTorsionPsi();
			}else if ((*it) == 		"PHI")
			{		
				properties_real_[(*it)]= residue->getTorsionPhi();	
			}else if ((*it) == 	"CHI")
			{
				properties_real_[(*it)]= 	getChiAngle_(residue);	
			}else if ((*it) == 	"CHI2")
			{
				properties_real_[(*it)]= getChi2Angle_(residue);	
			}else if ((*it) == 	"HA1L" )
			{
				properties_real_[(*it)]= getHA_HBondLen_(residue); 	
			}else if ((*it) == 	"HA1" )
			{ 	
				properties_string_[(*it)]=( hasHA_HBond_(residue)? "Y": "N");
			}else if ((*it) == 	"HA2L" )
			{
				properties_real_[(*it)]= getHA2_HBondLen_(residue) ;	
			}else if ((*it) == 	"HA2" )
			{ 	
				properties_string_[(*it)]= (hasHA2_HBond_(residue)? "Y": "N");		
			}else if ((*it) == 	"HNL" )
			{
				properties_real_[(*it)]= getHN_HBondLen_(residue) ;	
			}else if ((*it) == 	"HN" )
			{
				properties_string_[(*it)]=  (hasHN_HBond_(residue)? "Y": "N");		
			}else if ((*it) == 	"OHL" )
			{
				properties_real_[(*it)]= getO_HBondLen_(residue);	
			}else if ((*it) == 	"OH" )
			{
				properties_string_[(*it)]= (hasO_HBond_(residue)? "Y": "N");		
			}else if ((*it) == 	"DISULFIDE" )
			{
				properties_string_[(*it)]= (hasDisulfidBond_(residue)? "Y": "N");			
			}						
		} 
		return true;
	}


	bool EmpiricalHSShiftProcessor::PropertiesForShift_::isDiscrete(String property)
		throw()
	{
		if( property.hasSubstring("PSI") || property.hasSubstring("PHI") || 
				property.hasSubstring("HA2L") || property.hasSubstring("HA1L") || 
				property.hasSubstring("HNL") || property.hasSubstring("OHL") )
			return false;
		else
			return true;
	}
	
	bool EmpiricalHSShiftProcessor::PropertiesForShift_::isMixed(String property)
		throw()
	{
		if(property.hasSubstring("CHI") || property.hasSubstring("CHI2"))
			return true;
		else
			return false;
	}

	

	
	std::pair<float, String>  EmpiricalHSShiftProcessor::PropertiesForShift_::operator [] (const String& property_name)
		throw()
	{
		std::pair<float, String> p(std::numeric_limits<float>::min(),"INVALID");
		if (properties_real_.find(property_name) != properties_real_.end())
		{
			p.first = properties_real_[property_name];
		}else if (properties_string_.find(property_name) != properties_string_.end())
		{
			p.second = properties_string_[property_name];
		}
			
		return p;
	}


// 
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~:ShiftHyperSurface_:~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// 


	
	EmpiricalHSShiftProcessor::ShiftHyperSurface_::ShiftHyperSurface_()
		throw()
		: type_(),
			first_property_(),
			second_property_(),
 			float_property_(),
			string_property_(),
			s2d_(),
			s1d_(),
			table_()
	{
	}
	
	EmpiricalHSShiftProcessor::ShiftHyperSurface_::ShiftHyperSurface_(String filename, String atomtype, 
																																		String firstproperty, String secondproperty)
		throw()
		:	type_(),
			first_property_(),
			second_property_(),
 			float_property_(),
			string_property_(),
			s2d_(),
			s1d_(),
			table_()
	{		
		// find the data file
		BALL::Path p;
		String file_name = p.find("NMR/"+filename) ;// "NMR/splinedata/hat_PSI_DISULFIDE-1.dat");
		if (file_name == "")
		{
			Log.info () << "File " << filename << " could not be found" << std::endl;
			//ToDO:: Flag for invalid !!!
			return;
		}
		
		//set the x property and the y property
		first_property_  = firstproperty;
		second_property_ = secondproperty;
		
		//set the type
		if(PropertiesForShift_::isMixed(firstproperty))
		{
			if (PropertiesForShift_::isMixed(secondproperty))
				type_ = CHI__CHI;
			else if (PropertiesForShift_::isDiscrete(secondproperty))
				type_ = CHI__DISCRETE;
			else 
				type_ = CHI__REAL;
		}		
		else if (PropertiesForShift_::isDiscrete(firstproperty)) 
		{
			if (PropertiesForShift_::isMixed(secondproperty))
				type_ = DISCRETE__CHI;
			else if (PropertiesForShift_::isDiscrete(secondproperty))
				type_ = DISCRETE__DISCRETE;
			else 
				type_ = DISCRETE__REAL;
		} 
		else 
		{
			if (PropertiesForShift_::isMixed(secondproperty))
				type_ = REAL__CHI;
			else if (PropertiesForShift_::isDiscrete(secondproperty))
				type_ = REAL__DISCRETE;
			else 
				type_ = REAL__REAL;
		}
		
		// read the datafile
		BALL::File file(file_name, std::ios::in);
		String line;
		std::vector<BALL::String> fields;

		// store the data
		//createBiCubicSpline(const std::vector<std::vector<float> >& sample_positions_x,
		//				const std::vector<float>& sample_positions_y,
	//					const std::vector<std::vector<float> >& sample_values);	
	//void createSpline(const std::vector<float>& sample_positions, 
		//				const std::vector<float>& sample_values); // spline

		// stores in the R_* case the sample positions
		vector<float> float_sample_positions_x;
		// stores in the D_* case the string-keys, also used for the string part of the chi sample positions
		vector<String> string_sample_positions_x;
		// stores the float y samples positions in case _R 
		vector<float>  float_sample_positions_y;
		vector<String> string_sample_positions_y;
		
		vector<std::vector<float> > sample_values_2d;
		vector<float> sample_values_1d;
		vector<float> sample_positions_1d; //??
		
		// the x sample positions
		line.getline(file);
		line.split(fields, ";");
std::cout << "------------------------   ------------------"<< std::endl;
std::cout << line << std::endl;		
		if (PropertiesForShift_::isMixed(firstproperty))
		{std::cout << "chi first property" << std::endl;

			// we have to store the first 3 entries as floats, 
			// and the second 3 entries as strings
			for (Position i = 0; i < 3; i++)
			{
				if (fabs(fields[i].toFloat()+60) < 1e-5)
					float_sample_positions_x.push_back(300.);
				else
					float_sample_positions_x.push_back(fields[i].toFloat());
			}
			for (Position i = 0; i < 3; i++)
			{
				string_sample_positions_x.push_back(fields[3+i]);
			}
		}
		else if (PropertiesForShift_::isDiscrete(firstproperty))
		{std::cout << "discrete  first property" << std::endl;

			for (Position i = 0; i < fields.size(); i++)
			{
				string_sample_positions_x.push_back(fields[i]);
			}
		}
		else
		{std::cout << "real first property" << std::endl;

			for (Position i = 0; i < fields.size(); i++)
			{	
				float_sample_positions_x.push_back(fields[i].toFloat());
			}
		}
		
		// the y sample positions
		line.getline(file);
		line.split(fields, ";");
std::cout << line << std::endl;
		if (PropertiesForShift_::isMixed(secondproperty))
		{std::cout << "chi second property" << std::endl;

			// we have to store the first 3 entries as floats, 
			// and the second 3 entries as strings
			for (Position i = 0; i < 3; i++)
			{	
				if (fabs(fields[i].toFloat()+60) < 1e-5)
					float_sample_positions_y.push_back(300.);
				else
					float_sample_positions_y.push_back(fields[i].toFloat());
			}

			for (Position i = 0; i < 3; i++)
			{
				
				string_sample_positions_y.push_back(fields[3+i]);
			}
		}
		else if (PropertiesForShift_::isDiscrete(secondproperty))
		{
			std::cout << "discrete second property" << std::endl;
			for (Position i = 0; i < fields.size(); i++)
			{
				string_sample_positions_y.push_back(fields[i]);
			}
		}
		else
		{std::cout << "real second property" << std::endl;

			for (Position i = 0; i < fields.size(); i++)
			{	
				float_sample_positions_y.push_back(fields[i].toFloat());
			}
		}

		// the sample values
			
		line.getline(file);
		while (file.good())
		{
std::cout << line<< std::endl;
			line.split(fields, ";");
			
			std::vector<float> line_values;	
			
			for ( Position i = 0; i < fields.size(); i++)
			{
				line_values.push_back(fields[i].toFloat());
			}
			sample_values_2d.push_back(line_values);
			line.getline(file);
		}
	
		//create 	a 2D bicubic spline, 
		//				a map of 1D bicubic splines or 
		//				a lookUpTable (map of map)  

		if (type_ == REAL__REAL)
		{
			std::cout << "REAL__REAL" << std::endl;
			// a bicubic spline is stored
			s2d_.createBiCubicSpline(float_sample_positions_x, float_sample_positions_y, sample_values_2d);
		//	std::cout << "value at (-170, -160) :" <<  s2d_(float(-170.),float(-160.)) << std::endl;
		}
		else if (type_ == REAL__DISCRETE)
		{
			// 		!!!!  C A U T I O N !!!! 
			// When accessing the data, one has to switch X and Y, 
			// since for each discrete value a 1D bicubic spline is stored
			
			std::cout << "REAL__DISCRETE" << std::endl;

			for (Position i = 0; i < sample_values_2d.size(); i ++)
			{
				// create a 1D bicubic spline
				CubicSpline1D_ s;
				s.createSpline(float_sample_positions_x, sample_values_2d[i]);
						
				// store him in the map
				s1d_[string_sample_positions_y[i]] = s;
			}
		//	std::cout << "value at (-100., H) :" <<  s1d_['H'](float(-100.)) << std::endl;
		}
		else if (type_ == DISCRETE__REAL)
		{
			std::cout << "DISCRETE__REAL" << std::endl;
			//
			//   !!!!!  C A U T I O N  !!!!! 
			//
			// we store for each discrete value a 1D bicubic spline
			// which means we store the "vertical" splines as "horizontal" splines
			// such that when acessing one has to switch x and y 
	
			// and therefor we have to transpose the sample_values
			vector <vector <float> > turned_sample_values_2d(string_sample_positions_x.size());
			for (Position i = 0; i < float_sample_positions_y.size(); i++)
			{
				for (Position j = 0; j < string_sample_positions_x.size(); j++)
				{
					turned_sample_values_2d[j].push_back(sample_values_2d[i][j]);
				}
			}		
			
			for (Position i = 0; i < turned_sample_values_2d.size(); i ++)
			{
				// create a 1D bicubic spline
				CubicSpline1D_ s;
				s.createSpline(float_sample_positions_y, turned_sample_values_2d[i]);
						
				// store him in the map
				s1d_[string_sample_positions_x[i]] = s;
			}
		//	std::cout << "value at (C, -170) :" <<  s1d_['C'](float(-170.)) << std::endl;
			// TO DO: diesen Fehler und ähnliche abfangen!!! 
			//std::cout << "value at (R, -170) :" <<  s1d_['R'](float(-170.)) << std::endl;
		}
		else if (type_ == DISCRETE__DISCRETE)
		{
			std::cout << "DISCRETE__DISCRETE" << std::endl;

			for (Position i = 0; i < sample_values_2d.size(); i++)  // y
			{
				for (Position j = 0; j < sample_values_2d[i].size(); j++) // x
				{ 	
					table_[string_sample_positions_x[j]][string_sample_positions_y[i]]= sample_values_2d[i][j];
				}
			}
		//	std::cout << "value at (A, N) :" <<  table_['A']['N'] << std::endl;
		}
		else if (type_ == CHI__DISCRETE)
		{
			std::cout << "CHI__DISCRETE" << std::endl;
			// we have a table like this
			// 
			//	     60   180 -60        Unknown  ALA  GLY
			//	C   								|
			//									 		|
			//  H   REAL__DISCRETE  | 	DISCRETE__DISCRETE
			//   								 		| 			
			//  B										|
			//  so we have to store 
			//     - a map of 1D Bicubic splines (case REAL__DISCRETE)
			//  	 - and a table (case DISCRETE DISCRETE) 
			
			// 		!!!!  Remember: 
			// When accessing the data, one has to switch X and Y, 
			// since for each discrete value a 1D bicubic spline is stored
			
		/*	// we have to split the data sample_positions
			vector <float> first_float_sample_positions_x;
			for (Position i  = 0; i< 3 ; i++)
			{
				first_float_sample_positions_x.push_back(float_sample_positions_x[i]);
			}
			*/
			// 		we have to split the data
			vector<vector <float> > first_sample_values_2d(string_sample_positions_y.size());
			vector<vector <float> > last_sample_values_2d(string_sample_positions_y.size());
			for (Position i = 0 ; i < string_sample_positions_y.size() ; i++)
			{
				for (Position j = 0; j < 3; j++)
				{
					first_sample_values_2d[i].push_back(sample_values_2d[i][j]);
				}
				for (Position j = 0; j < 3; j++)
				{
					last_sample_values_2d[i].push_back(sample_values_2d[i][j+3]);
				}
			}
			
			// first case REAL__DISCRETE
			
			
			for (Position i = 0; i < string_sample_positions_y.size(); i ++)
			{
				// create a 1D bicubic spline
				CubicSpline1D_ s;
				s.createSpline(float_sample_positions_x, first_sample_values_2d[i]);
						
				// store him in the map
				s1d_[string_sample_positions_y[i]] = s;
			}
			
			//std::cout << "value at (300., C) :" <<  s1d_['C'](float(300.)) << std::endl;
			
			// second case DISCRETE__DISCRETE
			
			for (Position i = 0; i < last_sample_values_2d.size(); i++)  // y
			{
				for (Position j = 0; j < last_sample_values_2d[i].size()  ; j++) // x
				{
					table_[string_sample_positions_x[j]][string_sample_positions_y[i]] = last_sample_values_2d[i][j];
				}
			}
		//	std::cout << "value at (GLY, B) :" <<  table_["GLY"]["B"] << std::endl;
		}else if (type_ == DISCRETE__CHI)
		{
			std::cout << "DISCRETE__CHI" << std::endl;

			// we are given a table like this
			// 	 			C   H    B	
			// 60
			// 180
			//-60
			// --------------------------
			// UNK
			// ALA
			// GLY

			// so for the first three lines, we have 
			// the case DISCRETE__REAL, and for the
			// last three lines we have the case
			// DISCRETE__DISCRETE
			
			
			// First part  DISCRETE__REAL
			
			// and therefor we have to transpose the sample_values for the first 3 lines
			vector <vector <float> > turned_sample_values_2d(string_sample_positions_x.size());
			for (Position i = 0; i < 3; i++)// float_sample_positions_y.size(); i++)
			{
				for (Position j = 0; j < string_sample_positions_x.size(); j++)
				{
					turned_sample_values_2d[j].push_back(sample_values_2d[i][j]);
				}
			}	

			std::cout << "hallo" << std::endl;
			std::cout << "x len" <<string_sample_positions_x.size() << std::endl;
			// we have to split the data
			vector<float > first_float_sample_positions_y;
			for (Position i = 0; i < 3; i++)
			{
				first_float_sample_positions_y.push_back(float_sample_positions_y[i]);
			}			
		
	std::cout << "hallo2" << std::endl;
			turned_sample_values_2d.size();
std::cout << "turned_sample_values_2d.size()"<<turned_sample_values_2d.size() << std::endl;
			for (Position i = 0; i < turned_sample_values_2d.size(); i ++)
			{
				// create a 1D bicubic spline
				CubicSpline1D_ s;
				s.createSpline(first_float_sample_positions_y, turned_sample_values_2d[i]);

std::cout << string_sample_positions_x[i] << std::endl;	
				// store him in the map
				s1d_[string_sample_positions_x[i]] = s;
			}
			//std::cout << "value at (C, 60) :" <<  s1d_['C'](float(60.)) << std::endl;

			// second part DISCRETE__DISCRETE

			std::cout << "hallo3" << std::endl;
			for (Position i = 0; i < 3; i++) //x
			{
				for (Position j = 0; j < string_sample_positions_x.size(); j++) //y
				{	
					table_[string_sample_positions_x[j]][string_sample_positions_y[i]] = sample_values_2d[i+3][j];	
				}
			}
		//	std::cout << "value at (H, ALA) :" <<  table_['H']["ALA"] << std::endl;
		}	
		else if (type_ == CHI__REAL)
		{
			// we are given a table like this
			//
			//        60  180  300  |  Unknown ALA GLY
			// - 180								|
			//  .										|
			//  0										|
			//  .										|
			//  180									|
			// so again we split into two subtables
			//    - case REAL__REAL
			//    - and case DISCRETE__REAL 
			
			std::cout << "CHI__REAL" << std::endl;
			
			// we have to split the data
			vector<vector <float> > first_sample_values_2d(float_sample_positions_y.size());
			vector<vector <float> > last_sample_values_2d(float_sample_positions_y.size());
			for (Position i = 0 ; i < float_sample_positions_y.size() ; i++)
			{
				for (Position j = 0; j < 3; j++)
				{
					first_sample_values_2d[i].push_back(sample_values_2d[i][j]);
				}
				for (Position j = 0; j < 3; j++)
				{
					last_sample_values_2d[i].push_back(sample_values_2d[i][j+3]);
				}
			}

			// case REAL__REAL : we store a 2d bicubic spline
			s2d_.createBiCubicSpline(float_sample_positions_x, float_sample_positions_y, first_sample_values_2d);
		//	std::cout << "value at ( 300, 180) :" <<  s2d_(float(300),float(180)) << std::endl;


			// case DISCRETE__REAL 
			// !! R E M E M B E R !! 
			// we have to transpose the data in order to store 
			// for each discrete value a 1D bicubic spline
			vector <vector <float> > turned_sample_values_2d(string_sample_positions_x.size());
			for (Position i = 0; i < float_sample_positions_y.size(); i++)
			{
				for (Position j = 0; j < string_sample_positions_x.size(); j++)
				{
					turned_sample_values_2d[j].push_back(last_sample_values_2d[i][j]);
				}
			}
			
			for (Position i = 0; i < turned_sample_values_2d.size(); i ++)
			{
				// create a 1D bicubic spline
				CubicSpline1D_ s;
				s.createSpline(float_sample_positions_y, turned_sample_values_2d[i]);
						
				// store him in the map
				s1d_[string_sample_positions_x[i]] = s;
			}
		//	std::cout << "value at (Unknown, -180) :" <<  s1d_["Unknown"](float(-180.)) << std::endl;

			
		}		
		else if (type_ == REAL__CHI)
		{
			std::cout << "REAL__CHI" << std::endl;
			// We are given a table looking like this
			//
			//      -180  . . 0 . . 180
			// 60
			// 180
			// 300
			// ---------------------------
			// Unknown
			// ALA
			// GLY
			//
			// so we have two subtables:
			//  - REAL__REAL
			//  - REAL__DISCRETE
				
			// we have to split the data
			vector<vector <float> > first_sample_values_2d(3);
			vector<vector <float> > last_sample_values_2d(3);
			for (Position i = 0 ; i < 3; i++) // y
			{
				for (Position j = 0; j < float_sample_positions_x.size(); j++)// x 
				{
					first_sample_values_2d[i].push_back(sample_values_2d[i][j]);
					last_sample_values_2d[i].push_back(sample_values_2d[i+3][j]);
				}
			}
			
			// case REAL__REAL : we store a 2d bicubic spline
			s2d_.createBiCubicSpline(float_sample_positions_x, float_sample_positions_y, first_sample_values_2d);
	//		std::cout << "value at ( 180, 300) :" <<  s2d_(float(180),float(300)) << std::endl;

			// case REAL__DISCRETE
			
			//   R E M E M B E R : 
			// when acessing these data, one has to switch the parameter
			// since for each discrete value a 1D bicubic spline is stored
			for (Position i = 0; i < last_sample_values_2d.size(); i ++)
			{
				// create a 1D bicubic spline
				CubicSpline1D_ s;
				s.createSpline(float_sample_positions_x, last_sample_values_2d[i]);
						
				// store him in the map
				s1d_[string_sample_positions_y[i]] = s;
			}
		//	std::cout << "value at (0., ALA) :" <<  s1d_["ALA"](float(0.)) << std::endl;
		}
		else if (type_ == CHI__CHI)
		{
			// Fortunately this case does not occure
			std::cout << "CHI__CHI" << std::endl;
			std::cerr << "The case CHI__CHI is not implemented" <<std::endl; 
		}


		
		
	}	

	EmpiricalHSShiftProcessor::ShiftHyperSurface_::~ShiftHyperSurface_() 
		throw() 
	{
	}


	float EmpiricalHSShiftProcessor::ShiftHyperSurface_::operator() (EmpiricalHSShiftProcessor::PropertiesForShift_& properties)
		throw()
	{
		float shift = 0.;
		
		if (type_ == REAL__REAL)
		{
			shift = s2d_( properties[first_property_].first, properties[second_property_].first );
		}else if (type_ == REAL__DISCRETE)
		{
			shift = s1d_[properties[second_property_].second](properties[first_property_].first);
									
		}else {
		Log.info() << "andere Tabele! " << std::endl;
		}
		// TO DO:
		//  - Ala -A
		//  -REAL__DISCRETE xy-> yx
		

		return shift;
	} 

	
} // namespace BALL
