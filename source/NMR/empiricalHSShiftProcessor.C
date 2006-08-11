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

#define FLOAT_VALUE_NA 3600.
#define STRING_VALUE_NA '?'

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
				property_pairs_[target_name] = empty3;
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
			property_pairs_[target_name].push_back(tuple);

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
//std::cout<< "~~~~" <<(*it).first.first << " " << (*it).first.second  << ":  " <<(*it).second << std::endl;
				ShiftHyperSurface_ shs( (*it).second, atom_type, first_property, second_property); 
																				
				//insert in the map of hypersurfaces
				std::pair<String, String> tuple(first_property, second_property);
				hypersurfaces_[atom_type][tuple]= shs;
			}
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
		
		//printParameters_();	
		//printTargets_();
		
		// if there were no targets, return immediately
		if (targets_.size() == 0)
		{	
			Log.info() << "No targets found in EmpiricalHSShiftProcessor!" << std::endl;
			return true;
		}
		
		//if there are no hypersurfaces, return immediately
		if (hypersurfaces_.size() == 0)
		{
			Log.info() << "No hypersurfaces created in EmpiricalHSShiftProcessor!" << std::endl;
			return true; 
		}
		
		float EHS_shift =  0.;

		// for all targets compute the shift
		for (Position i = 0; i < targets_.size(); i++)
		{
			EHS_shift =  0.;
			
			PropertiesForShift_  target = targets_[i];
			String atom_type = target.atom->getName();
			
			// for all property pairs of the targets atom type
			for (Position j = 0; j < property_pairs_[atom_type].size(); j++) 
			{
				EHS_shift += hypersurfaces_[atom_type][property_pairs_[atom_type][j]](target);
			}
			// set the shift contribution by empiricalHSShiftProcessor
			target.atom->setProperty(PROPERTY__EHS_SHIFT, EHS_shift);
			 
			// add it to the total shift
			float old_shift = target.atom->getProperty(ShiftModule::PROPERTY__SHIFT).getFloat(); 
			target.atom->setProperty(ShiftModule::PROPERTY__SHIFT, (old_shift + EHS_shift)); 
// std::cout << "EHS SHIft: "<< EHS_shift << std::endl;
		}

		printTargets_();
		printParameters_();
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
											const std::vector<float>& sample_values, bool return_average)
	{
		// store the out of bound behaviour flag 
		return_average_ =  return_average;
		
		// do we have reasonable data?
		if (sample_values.size() != sample_positions.size())
		{	
			std::cerr << "CubicSpline1D_::createSpline: number of sample positions != number of sample values" << std::endl;
			return;
		}		
		
		average_ = 0.;
		// in case we have too less values or the access-values is out of bound
		// we want to return the average -> compute the average
		for (Position i=0; i < sample_values.size(); i++)
		{
			average_ += sample_values[i];
		}
		average_ /= sample_values.size();
			
		//
		// now we compute the spline
		// 
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
			sig = (sample_positions[i]-sample_positions[i-1]) / (sample_positions[i+1]-sample_positions[i-1]);
			p = sig*curvature_[i-1] + 2.0;
			curvature_[i] = (sig-1.0)/p;
			u[i] =  (sample_values[i+1]-sample_values[i]) / (sample_positions[i+1]-sample_positions[i]) 
				    - (sample_values[i]-sample_values[i-1]) / (sample_positions[i]-sample_positions[i-1]);
			u[i] =  (6.0*u[i] / (sample_positions[i+1] - sample_positions[i-1]) - sig*u[i-1])/p;
		}

		// for natural splines 
		qn = 0.0;
		un = 0.0; 
		/*else
		 * we have a specified first derivative.
		 qn=0.5;
		 un=(3.0/(sample_positions[n]-sample_positions[n-1]))*(ypn-(sample_values[n]-sample_values[n-1])/(sample_positions[n]-sample_positions[n-1]));
		 */

		curvature_[n-1] = (un-qn*u[n-2])/(qn*curvature_[n-2] + 1.0);

		//This is the backsubstitution loop of the tridiagonal algorithm.
		for (int k = n-2; k >= 0; k--) 
		{	
			curvature_[k] = curvature_[k]*curvature_[k+1] + u[k]; 
		}	
		return;	
	}

	vector<float>& EmpiricalHSShiftProcessor::CubicSpline1D_::getCurvature()
	{
//	std::cout <<"getCurvature" << std::endl;
		return curvature_;
	}				

	void EmpiricalHSShiftProcessor::CubicSpline1D_::setCurvature(std::vector<float> curvature)
	{
//std::cout <<"setCurvature" << std::endl;
		curvature_ = curvature;
	}
	
	void EmpiricalHSShiftProcessor::CubicSpline1D_::setValues(std::vector<float> values)
	{
//std::cout << "setValues"<< std::endl;
		sample_values_ = values;
	}
	
	void EmpiricalHSShiftProcessor::CubicSpline1D_::setPositions(std::vector<float> positions)
	{
//std::cout <<"setPositions" << std::endl;
		sample_positions_= positions;
	}

	float EmpiricalHSShiftProcessor::CubicSpline1D_::operator() (float x)
	{
		unsigned int n=sample_positions_.size();
		// is this x position inside the boundaries?

		if ((sample_positions_.size() > 0) && ((x < sample_positions_[0]) || (x>sample_positions_[n-1])))
		{
			// something _really_ bad happened
			if (!return_average_)
			{
				std::cerr << "invalid x position : " << x << " not between "<< sample_positions_[0] << " and " 
									<< sample_positions_[n-1]<< std::endl;
				return std::numeric_limits<float>::min();
			}
			else
			{
				return average_;
			}
		}

		// Do we have enough points ?
		if (sample_positions_.size() < 3)
		{
			return average_;
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
//std::cout <<"createBICubicSpline" << std::endl;

		sample_positions_x_ = sample_positions_x;
		sample_positions_y_ = sample_positions_y;
		int m = sample_positions_x_.size();

		CubicSpline1D_ cs; 
		
		average_ = 0.;
		for (int j = 0; j < m; j++)
		{
			// compute a 1Dspline
			cs.createSpline(sample_positions_x[j], sample_values[j], true);
			average_ += cs.getAverage()/m;
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
		cs.createSpline(sample_positions_y_, values, true);

		// TEST!
//		for (Position i=0; i<sample_positions_y_.size(); i++)
//			std::cout << "last spline BALL: " << sample_positions_y_[i] << " " << values[i] << std::endl;

		//evaluate the new spline at position y
		return cs(y);
	}



// 
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~:PropertiesForShift_:~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// 



	
	EmpiricalHSShiftProcessor::PropertiesForShift_::PropertiesForShift_()
		throw()
	{
	}
	
	float EmpiricalHSShiftProcessor::PropertiesForShift_::getChiAngle_(Residue* residue) 
		throw()
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
		*/
		String residue_name = residue->getName();
		if (residue_name == "GLY")
		{ 
			Log.info() << "Torsion angle of a Glycine could not be computed!" << std::endl;
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
		throw()
	{	
		Log.info() << "Chi2-angle-computation is not yet implemented!" << std::endl;
		return FLOAT_VALUE_NA;
	}

	
	char EmpiricalHSShiftProcessor::PropertiesForShift_::getSecondaryStructure_(Residue* residue) 
		throw()
	{
		char ret = STRING_VALUE_NA;
		if (residue->getSecondaryStructure() == 0)
		{
			Log.info() << "No secondary structure available. Consider precomputing!" << std::endl;
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
		char ret = STRING_VALUE_NA;
		if (residue->isAminoAcid())
		{
			ret = Peptides::OneLetterCode(residue->getName()); 
		}
		return ret;
	}

	
	float EmpiricalHSShiftProcessor::PropertiesForShift_::getHA_HBondLen_(Residue* residue)
		throw()
	{
		float len = FLOAT_VALUE_NA;
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
		float len = FLOAT_VALUE_NA ;
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
		float len = FLOAT_VALUE_NA;
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
				if(bi->getType() == Bond::TYPE__HYDROGEN);
				{
					len = bi->getLength();
				}
			}
		}	
		return len;
	}	

	
	float 	EmpiricalHSShiftProcessor::PropertiesForShift_::getO_HBondLen_(Residue* residue) 
		throw()
	{
		float len = FLOAT_VALUE_NA;
		
		Atom* O = 0;
		
		AtomIterator r_it = residue->beginAtom();
		for (; r_it != residue->endAtom(); ++r_it)
		{	
			String name = r_it->getName();
			if (name == "O")
			{
				O = &(*r_it);
			}
		}
		
		if (O)
		{
			Atom::BondIterator bi = O->beginBond();
			for (;+bi;++bi)
			{	
				if(bi->getType() == Bond::TYPE__HYDROGEN);
				{
					len = bi->getLength();
					// TODO: Atom * H = bi-getPartner(O);
					// len = (O->getPosition() -H.getPosition()).getLength();
				}
			}
		}	
		
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

	
	bool 	EmpiricalHSShiftProcessor::PropertiesForShift_::hasHN_HBond_(Residue* residue)
		throw()
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
		// NOTE: if a certain property is not available, it gets
		// 	the value FLOAT_VALUE_NA or STRING_VALUE_NA
		// 	thus we can evaluate unknown values 
		// 	by assinging them the average value 
		// 	( which is the way ShiftX works :-) )
		for (std::set<String>::iterator it = properties.begin();
				it != properties.end(); it++)
		{	
			//std::cout << "property" << (*it)<< std::endl;

			if (!prev_residue)
			{
					properties_real_[("PSI_P")]  = FLOAT_VALUE_NA;
					properties_real_[("PHI_P")]  = FLOAT_VALUE_NA;
					properties_real_[("CHI_P")]  = FLOAT_VALUE_NA;
					properties_real_[("CHI2_P")] = FLOAT_VALUE_NA;	
					properties_real_[("HA1L_P")] = FLOAT_VALUE_NA;
					properties_real_[("HA2L_P")] = FLOAT_VALUE_NA;
					properties_real_[("HNL_P")] = FLOAT_VALUE_NA;	
					properties_real_[("OHL_P")] = FLOAT_VALUE_NA;
				
					properties_string_[("AA_P")] = STRING_VALUE_NA;
					properties_string_[("SS_P")] = STRING_VALUE_NA; 
					properties_string_[("HA1_P")] = STRING_VALUE_NA;
					properties_string_[("HA2_P")] = STRING_VALUE_NA;
					properties_string_[("HN_P")] = STRING_VALUE_NA;	
					properties_string_[("OH_P")] = STRING_VALUE_NA;	
					properties_string_[("DISULFIDE_P")] = STRING_VALUE_NA;	
					properties_string_[("CHI_P")] = STRING_VALUE_NA;

					properties_string_[("FR")] = 'Y';
					properties_string_[("FR_P")] = 'N';
					properties_string_[("FR_N")] = 'N';
			}
			else
			{
				if ((*it) == 	"FR_P" )
				{	
					// TODO: maybe is N-terminal?
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
				}else if ((*it) == 	"PHI_P")
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
				}else if ((*it) == 	"CHI_P")
				{
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
					properties_string_[("CHI_N")] = STRING_VALUE_NA;	

					properties_string_[("FR_P")] = STRING_VALUE_NA;
					properties_string_[("FR")]   = STRING_VALUE_NA;
					properties_string_[("FR_N")] = STRING_VALUE_NA;
			}
			else
			{
				if ((*it) == 	"FR_N" )
				{	
					// TODO: maybe is N-terminal?
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
					if (next_residue->hasTorsionPsi())
					{	
						properties_real_[(*it)]= next_residue->getTorsionPsi().toDegree();
						if (properties_real_[(*it)] > 180)
							properties_real_[(*it)] -= 360;
						else if (properties_real_[(*it)] < -180)
							properties_real_[(*it)] += 360;
					}else
					{	
						properties_real_[("PSI_N")]  = FLOAT_VALUE_NA;
					}
				}else if ((*it) == 	"PHI_N")
				{	
					if (next_residue->hasTorsionPhi())
					{	
						properties_real_[(*it)]= next_residue->getTorsionPhi().toDegree();	
						if (properties_real_[(*it)] > 180)
							properties_real_[(*it)] -= 360;
						else if (properties_real_[(*it)] < -180)
							properties_real_[(*it)] += 360;	
					}else
					{
						properties_real_[("PHI_N")]  = FLOAT_VALUE_NA;
					}
				}else if ((*it) == 	"CHI_N")
				{
					properties_real_[(*it)]=  getChiAngle_(next_residue);
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
					// TODO: maybe is N-terminal?
				properties_string_[(*it)]=  (residue->isTerminal() ? "Y": "N");			
			}else if ((*it) == 	"AA" )
			{	
				properties_string_[(*it)]=  getAminoAcid_(residue);
			}else if ((*it) == 	"SS" )
			{
				properties_string_[(*it)]=  getSecondaryStructure_(residue);
			}else if ((*it) == 	"PSI")
			{
				if (residue->hasTorsionPsi())
				{	
					properties_real_[(*it)]= residue->getTorsionPsi().toDegree();
					if (properties_real_[(*it)] > 180)
						properties_real_[(*it)] -= 360;
					else if (properties_real_[(*it)] < -180)
						properties_real_[(*it)] += 360;
				}else
				{	
					properties_real_[(*it)] = FLOAT_VALUE_NA;
				}
			}else if ((*it) == 		"PHI")
			{		
				if (residue->hasTorsionPhi())
				{	
					properties_real_[(*it)]= residue->getTorsionPhi().toDegree();	
					if (properties_real_[(*it)] > 180)
						properties_real_[(*it)] -= 360;
					else if (properties_real_[(*it)] < -180)
						properties_real_[(*it)] += 360;	
				}else
				{
					properties_real_[(*it)] = FLOAT_VALUE_NA;
				}
			}
			else if ((*it) == 	"CHI")
			{
				properties_real_[(*it)]= 	getChiAngle_(residue);	
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
				properties_real_[(*it)]= getChi2Angle_(residue);	
			}
			else if ((*it) == 	"HA1L" )
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
				property.hasSubstring("HNL") || property.hasSubstring("OHL")|| property.hasSubstring("CHI") )
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
		// initialize the return value 
		std::pair<float, String> p(FLOAT_VALUE_NA , STRING_VALUE_NA);
		
		// special case chi: 
		// 		can be a string (ALA, GLY) or have an angle (float)
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


	
	EmpiricalHSShiftProcessor::ShiftHyperSurface_::ShiftHyperSurface_()
		throw()
		: type_(),
			first_property_(),
			second_property_(),
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
			s2d_(),
			s1d_(),
			table_()
	{	
		
		std::cout << filename <<  std::endl;
		// find the data file
		BALL::Path p;
		String file_name = p.find("NMR/"+filename) ;// "NMR/splinedata/hat_PSI_DISULFIDE-1.dat");
		if (file_name == "")
		{
			Log.info() << "File " << filename << " could not be found" << std::endl;
			//ToDO:: Flag for invalid !!!
			return;
		}
		
		//set the x property and the y property
		first_property_  = firstproperty;
		second_property_ = secondproperty;
	
		// set the Hypersurface type
		setType_(firstproperty, secondproperty);
		

		//
		// read and store the data
		//

		BALL::File file(file_name, std::ios::in);
		String line;
		String line2;
		std::vector<BALL::String> fields;
		std::vector<BALL::String> fields2;

		// store the x sample positions
		vector<vector<float> > float_sample_positions_x;
		vector<vector<String> > string_sample_positions_x;
		vector<float>  float_sample_positions_x_1d;
		vector<String>  string_sample_positions_x_1d;

		// store the float y samples positions  
		vector<float>  float_sample_positions_y;
		vector<String> string_sample_positions_y;
	
		// stores the sample values
		vector<std::vector<float> > sample_values_2d;

		
		// if we have no real table 
		// 
		if(type_ == SINGLE__REAL)
		{  
			// we have a single spline 
std::cout <<" SINGLE__REAL ; " << first_property_ << "  " << second_property_ << std::endl;
			// read the points
			line.getline(file);
			line.split(fields, ";");

			for (Position i = 0; i < fields.size(); i++)
			{	
				float_sample_positions_x_1d.push_back(fields[i].toFloat());
			}	

			// read the values
			if (file.good())
			{
				line.getline(file);
				line.split(fields, ";");

				for (Position j = 0; j < fields.size(); j++)
				{	
					float_sample_positions_y.push_back(fields[j].toFloat());
				}	

				// store	
				CubicSpline1D_ s;
				s.createSpline(float_sample_positions_x_1d, float_sample_positions_y, true);
for (Position i=0; i<float_sample_positions_y.size(); i++)
	std::cout << "ANNE: " << float_sample_positions_x_1d[i] << " " << float_sample_positions_y[i] << " " << (s.getCurvature())[i] << std::endl;
				// store him in the map
				s1d_[first_property_] = s;
			}
			else
			{
				std::cerr<< "formaterror in " <<  filename << std::endl;
			}
		}
		else if (type_ == SINGLE__DISCRETE)
		{  // we have a single table line
std::cout <<" SINGLE__DISCRETE ; " << first_property_ << "  " << second_property_ << std::endl;
			// read the points
			line.getline(file);
			line.split(fields, ";");

			for (Position i = 0; i < fields.size(); i++)
			{	
				string_sample_positions_x_1d.push_back(fields[i]);
			}	

			// read the values
			if (file.good())
			{
				line.getline(file);
				line.split(fields, ";");

				if( fields.size()== string_sample_positions_x_1d.size())
				{	
					for (Position i = 0; i < fields.size(); i++)
					{	
						float_sample_positions_y.push_back(fields[i].toFloat());
					}	
					// store
					for (Position i = 0; i < string_sample_positions_x_1d.size(); i++)  
					{
						table_[first_property_][string_sample_positions_x_1d[i]]= float_sample_positions_y[i];
					}
				}
				else
				{	
					std::cerr<< "formaterror in " <<  filename << std::endl;
				}
			}
			else
			{
				std::cerr<< "formaterror in " <<  filename << std::endl;
			}
		}
		else if (type_ == SINGLE__CHI)
		{
			std::cerr<< "CHICHI fuer SS not yet implemented"<< std::endl;
		}
		else    //        if we have a normal "table", 
		{
			//  first we read the y sample positions
			line.getline(file);
			line.split(fields, ";");
			
			/*	if (PropertiesForShift_::isMixed(secondproperty))
			{
				std::cout << "chi second property" << std::endl;

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
			else*/ 
			if (PropertiesForShift_::isDiscrete(secondproperty) 
					|| (PropertiesForShift_::isMixed(secondproperty)))
			{
std::cout << "discrete or chi second property" << std::endl;
				for (Position i = 0; i < fields.size(); i++)
				{
					string_sample_positions_y.push_back(fields[i]);
				}
			}
			else
			{
std::cout << "real second property" << std::endl;
				for (Position i = 0; i < fields.size(); i++)
				{	
					float_sample_positions_y.push_back(fields[i].toFloat());
				}
			}

			//  we read the sample values in line pairs 
			// 		of the  x positions and 
			// 		the corresponding x values
			line.getline(file); 
			if (file.good())
				line2.getline(file);
			else
				return;	

			while (file.good())
			{
				//std::cout << line << std::endl;std::cout << line2 << std::endl;
				line.split(fields, ";"); 		// the x points
				line2.split(fields2, ";"); 	// the x values

				std::vector<float> line_values;	
				std::vector<float> line_values2;	

				// clear the variables
				float_sample_positions_x_1d.clear();
				string_sample_positions_x_1d.clear();

				if (fields.size() != fields2.size())
				{
					std::cerr << "number of points is unequal to number of values in file "<< filename << std::endl;
				}
				else
				{	
					//
					// handle the x positions : can be chi, real or string
					//
					/*if (PropertiesForShift_::isMixed(firstproperty))
					{
	std::cout << "chi first property" << std::endl;
						// we have to store the first 3 entries as floats, 
						// and the second 3 entries as strings
						for (Position i = 0; i < 3; i++)
						{
							if (fabs(fields[i].toFloat()+60) < 1e-5)
								float_sample_positions_x_1d.push_back(300.);
							else
								float_sample_positions_x_1d.push_back(fields[i].toFloat());
						}
						for (Position i = 0; i < 3; i++)
						{
							string_sample_positions_x_1d.push_back(fields[3+i]);
						}
					}
					else 
					*/	
					if (PropertiesForShift_::isDiscrete(firstproperty) || PropertiesForShift_::isMixed(firstproperty))
					{
		std::cout << "discrete  first property" << std::endl;
						for (Position i = 0; i < fields.size(); i++)
						{
							string_sample_positions_x_1d.push_back(fields[i]);
						}
					}
					else
					{
		std::cout << "real first property" << std::endl;
						for (Position i = 0; i < fields.size(); i++)
						{	
							float_sample_positions_x_1d.push_back(fields[i].toFloat());
						}
					}

					// store the new line
					float_sample_positions_x.push_back(float_sample_positions_x_1d);
					string_sample_positions_x.push_back(string_sample_positions_x_1d);
					
					//
					// handle the x values
					// 
		
					for (Position i = 0; i < fields.size(); i++)
					{
						line_values2.push_back(fields2[i].toFloat());
					}
					sample_values_2d.push_back(line_values2);
				}

				// read the next pair of lines
				line.getline(file);
				if (file.good())
					line2.getline(file);
				else
					continue;
			}

			
			// we have read the file now
			// create 	a 2D bicubic spline, 
			// 	  			a map of 1D bicubic splines or 
			//	  			a lookUpTable (map of map)  
 
std::cout << "create HS" << std::endl;
			
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
					s.createSpline(float_sample_positions_x[i], sample_values_2d[i], true);

					// store him in the map
					s1d_[string_sample_positions_y[i]] = s;
				}
				//	std::cout << "value at (-100., H) :" <<  s1d_['H'](float(-100.)) << std::endl;
			}
			else if (type_ == DISCRETE__REAL)
			{ 
std::cout << "DISCRETE__REAL not implemented" << std::endl;
				/*			//
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
				 */	}
			else if ( (type_ == DISCRETE__DISCRETE) || (type_ == CHI__DISCRETE) )
			{
				for (Position i = 0; i < string_sample_positions_y.size(); i++)  // y
				{
					 for (Position j = 0; j < string_sample_positions_x[i].size(); j++) // x
					 { 	
						 table_[string_sample_positions_y[i]][string_sample_positions_x[i][j]]= sample_values_2d[i][j];
					 }
				}
			//	std::cout << "value at (A, ALA) :" <<  table_["ALA"]['A'] << std::endl;
			}
			else if (type_ == DISCRETE__CHI)
				{
std::cout << "DISCRETE__CHI not implemented" << std::endl;
					/*
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

					// we have to split the data
					vector<float > first_float_sample_positions_y;
					for (Position i = 0; i < 3; i++)
					{
					first_float_sample_positions_y.push_back(float_sample_positions_y[i]);
					}			

					turned_sample_values_2d.size();
					for (Position i = 0; i < turned_sample_values_2d.size(); i ++)
					{
					// create a 1D bicubic spline
					CubicSpline1D_ s;
					s.createSpline(first_float_sample_positions_y, turned_sample_values_2d[i]);

					// store him in the map
					s1d_[string_sample_positions_x[i]] = s;
					}
					//std::cout << "value at (C, 60) :" <<  s1d_['C'](float(60.)) << std::endl;

					// second part DISCRETE__DISCRETE

					for (Position i = 0; i < 3; i++) //x
					{
					for (Position j = 0; j < string_sample_positions_x.size(); j++) //y
					{	
					table_[string_sample_positions_x[j]][string_sample_positions_y[i]] = sample_values_2d[i+3][j];	
					}
					}
					//	std::cout << "value at (H, ALA) :" <<  table_['H']["ALA"] << std::endl;
					 */		}	
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

std::cout << "CHI__REAL not implemented" << std::endl;
				/*			
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


				 */		}		
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
				//  Since ShiftX treats it different, we
				//  put both subtables into the map of 1D splines

		/*		// we have to split the data
				vector<vector <float> > first_sample_values_2d(3);
				vector<vector <float> > last_sample_values_2d(3);
				for (Position i = 0 ; i < 3; i++) // y
				{ 
					for (Position j = 0; j < float_sample_positions_x[i].size(); j++)// x 
					{
						first_sample_values_2d[i].push_back(sample_values_2d[i][j]);
						last_sample_values_2d[i].push_back(sample_values_2d[i+3][j]);
					}
				}
				
				// case REAL__REAL : we store a 2d bicubic spline
				s2d_.createBiCubicSpline(float_sample_positions_x, float_sample_positions_y, first_sample_values_2d);
				//		std::cout << "value at ( 180, 300) :" <<  s2d_(float(180),float(300)) << std::endl;
		*/
				// case REAL__DISCRETE

				//   R E M E M B E R : 
				// when acessing these data, one has to switch the parameter
				// since for each discrete value a 1D bicubic spline is stored
				for (Position i = 0; i < string_sample_positions_y.size(); i ++)
				{
					// create a 1D bicubic spline
					CubicSpline1D_ s;
					s.createSpline(float_sample_positions_x[i], sample_values_2d[i]);
					
					// store him in the map
					s1d_[string_sample_positions_y[i]] = s;
				}
				
		//	std::cout << "value at (220., Unkown) :" <<  s1d_["Unknown"](float(220.)) << std::endl;
		//	std::cout << "value at (-500., 360.) :" <<  s1d_["360."](float(-500.)) << std::endl;
		//	std::cout << "value at (400., 120.) :" <<  s1d_["120."](float(400.)) << std::endl;
			}
			else if (type_ == CHI__CHI)
			{
				// Fortunately this case does not occure
				std::cout << "CHI__CHI" << std::endl;
				std::cerr << "The case CHI__CHI is not implemented" <<std::endl; 
			}
		}
	}

	EmpiricalHSShiftProcessor::ShiftHyperSurface_::~ShiftHyperSurface_() 
		throw() 
	{
	}

	void EmpiricalHSShiftProcessor::ShiftHyperSurface_::setType_(String firstproperty, String secondproperty)
		throw()
	{
		if(PropertiesForShift_::isMixed(firstproperty))
		{
			if (PropertiesForShift_::isMixed(secondproperty))
			{	
				if (firstproperty == secondproperty)
					type_ = SINGLE__CHI;
				else
					type_ = CHI__CHI;
			}
			else if (PropertiesForShift_::isDiscrete(secondproperty))
				type_ = CHI__DISCRETE;
			else
			{	
				std::cout << "CHI REAL gesetzt" << std::endl;
				type_ = CHI__REAL;
			}	
		}		
		else if (PropertiesForShift_::isDiscrete(firstproperty)) 
		{
			if (PropertiesForShift_::isDiscrete(secondproperty))
			{	
				if (firstproperty == secondproperty)
					type_ = SINGLE__DISCRETE;
				else
					type_ = DISCRETE__DISCRETE;
			}
			else if (PropertiesForShift_::isMixed(secondproperty))
				type_ = DISCRETE__CHI;
			else 
				type_ = DISCRETE__REAL;
		} 
		else // first property is real
		{ 
			if (PropertiesForShift_::isDiscrete(secondproperty))
			{
				type_ = REAL__DISCRETE;
				std::cout << "REAL gesetzt" << std::endl;
			}
			else if (PropertiesForShift_::isMixed(secondproperty))
			{

				std::cout << "REAL CHI gesetzt" << std::endl;
				type_ = REAL__CHI;
			}
			else
			{
				if (firstproperty == secondproperty)
					type_ = SINGLE__REAL;
				else
					type_ = REAL__REAL;
			}
		}
		return;
	}


	
	float EmpiricalHSShiftProcessor::ShiftHyperSurface_::operator() (EmpiricalHSShiftProcessor::PropertiesForShift_& properties)
		throw()
	{
		float shift = 0.;
		
		if (type_ == SINGLE__REAL)
		{  // we have a single spline 
std::cout <<" operator() SINGLE__REAL ; " << first_property_ << "  " << second_property_ << std::endl;	
			if (s1d_.find(first_property_) != s1d_.end())
					shift = s1d_[first_property_](properties[first_property_].first);
			else
					shift = 0.;
			std::cout << properties.atom->getName() << "  " << first_property_ << ":" << second_property_<< " -- " << properties[first_property_].first<< ":" << properties[second_property_].first << " -- " << shift << std::endl;
		}
		else if (type_ == SINGLE__DISCRETE)
		{	
				// we have a single table line
std::cout <<" single spline - DISCRETE__DISCRETE ; " << first_property_ << "  " << second_property_ << std::endl;
				shift = table_[first_property_][properties[second_property_].second];		
		}
		else if (type_ == SINGLE__CHI)
		{
				std::cerr<< "CHI__CHI also Single Chi not yet implemented"<< std::endl;
		}
		else if (type_ == REAL__REAL)
		{		
			// This simulates SHIFTX behaviour: if only one factor is out of bounds, we return the all-values average
			if ((properties[first_property_].first == FLOAT_VALUE_NA) || (properties[second_property_].first == FLOAT_VALUE_NA))
			{
				shift = s2d_.getAverage();
			}
			else 
				shift = s2d_(properties[first_property_].first, properties[second_property_].first);
std::cout << properties.atom->getName() << "  " << first_property_ << ":" << second_property_<< " -- " << properties[first_property_].first<< ":" << properties[second_property_].first << " -- " << shift << std::endl;
		}
		else if (type_ == REAL__DISCRETE)
		{
			// This simulates SHIFTX behaviour: if only one factor is out of bounds, we return the all-values average
			if (   (properties[first_property_].first == FLOAT_VALUE_NA)
					 ||(properties[second_property_].second == STRING_VALUE_NA) )
			{
				shift = 0;
				std::map<String, CubicSpline1D_>::iterator ci;
				for (ci=s1d_.begin(); ci!=s1d_.end(); ci++)
					shift += ci->second.getAverage();
				shift /= s1d_.size();
			}
			else
				shift = s1d_[properties[second_property_].second](properties[first_property_].first);
std::cout <<properties.atom->getName() << "  " << first_property_ << ":" << second_property_<< " -- " << properties[first_property_].first<< ":" << properties[second_property_].second << " -- " << shift << std::endl;
		}
		else if (type_ == DISCRETE__REAL)
		{
			// This simulates SHIFTX behaviour: if only one factor is out of bounds, we return the all-values average
			if (   (properties[second_property_].first == FLOAT_VALUE_NA)
					 ||(properties[first_property_].second == STRING_VALUE_NA) )
			{
				shift = 0;
				std::map<String, CubicSpline1D_>::iterator ci;
				for (ci=s1d_.begin(); ci!=s1d_.end(); ci++)
					shift += ci->second.getAverage();
				shift /= s1d_.size();
			}
			else
				shift = s1d_[properties[first_property_].second](properties[second_property_].first);
		}
		else if (type_ == DISCRETE__DISCRETE) 
		{
			shift = table_[properties[first_property_].second][properties[second_property_].second];
		}
		else if (type_ == CHI__REAL)
		{	
			
			std::cout << "chi__real not yet implemented SFH operatore () " << std::endl;
			/*
			// we have to decide in which chi--case we are :
			// 			REAL or DISCRETE ? 
			if (properties[first_property_].second == "INVALID")
			{  // case REAL --> REAL__REAL
				shift = s2d_(properties[first_property_].first, properties[second_property_].first);
			}else if (properties[first_property_].first == std::numeric_limits<float>::min())
			{   // case DISCRETE --> DISCRETE__REAL
				shift = s1d_[properties[first_property_].second](properties[second_property_].first);
			}else	{
				std::cerr << first_property_ << " not set!"  << std::endl;
			}*/
		}
		else if (type_ == REAL__CHI)
		{				
			// we have to decide in which chi--case we are :
			// 			REAL or DISCRETE ? 
			
			if (properties[second_property_].first == FLOAT_VALUE_NA) 
			{  // case DISCRETE --> REAL__DISCRETE 
				 //  ALA, GLY or Unknown
				shift = s1d_[properties[second_property_].second](properties[first_property_].first);
			}
			else if (properties[second_property_].second == STRING_VALUE_NA)
			{  // case REAL --> REAL__REAL
				float chi_value = properties[second_property_].first; 
				if (chi_value < 120.)
					shift = s1d_["120."](properties[first_property_].first);
				else if (chi_value < 240.)
					shift = s1d_["240."](properties[first_property_].first);
				else if (chi_value < 360.)
					shift = s1d_["360."](properties[first_property_].first);
				else if (chi_value == FLOAT_VALUE_NA)
					std::cerr<< "something bad happend in PropertiesForShift::operator()" << std::endl;
			}
			else 
			{
				std::cout << "first: "<< (properties[first_property_].first) << " -- second: (" << properties[second_property_].first << "  " <<properties[second_property_].second << ")" << std::endl;
				std::cerr << second_property_ << " not set!"  << std::endl;
			}
		}
		else if (type_ == CHI__DISCRETE)
		{	
			// ToDO: was passiert, wenn die Table die mitgegebenen keys nicht hat? 
			
			if (properties[second_property_].second == STRING_VALUE_NA)
			{
				// ToDo: was passiert, wenn der zweite Wert auch nicht gesetzt ist?  
			}
			if (properties[first_property_].first == FLOAT_VALUE_NA)
			{
				// ALA, GLY or Unknown
				shift = table_[properties[second_property_].second][properties[first_property_].second];
			}
			else
			{
				float chi_value = properties[first_property_].first; 
				if (chi_value < 120.)
					shift = table_["120."][properties[second_property_].second];
				else if (chi_value < 240.)
					shift = table_["240."][properties[second_property_].second];
				else if (chi_value < 360.)
					shift = table_["360."][properties[second_property_].second];
				else if (chi_value == FLOAT_VALUE_NA)
					std::cerr<< "something bad happend in PropertiesForShift::operator()" << std::endl;
			}
		}
		else if (type_ == DISCRETE__CHI)
		{
				std::cout << "discret__chi not yet implemented SFH operatore () " << std::endl;
/*
			// we have to decide in which chi--case we are :
			// 			REAL or DISCRETE ? 
			if (properties[second_property_].second == "INVALID")
			{  // case REAL --> DISCRETE__REAL
				shift = s1d_[properties[first_property_].second](properties[second_property_].first);
			}else if (properties[second_property_].first == std::numeric_limits<float>::min())
			{  // case DISCRETE --> DISCRETE__DISCRETE
				shift = table_[properties[first_property_].second][properties[second_property_].second];		
			}else {
				std::cerr << second_property_ << " not set!"  << std::endl;
			} */
		}
		else if (type_ == CHI__CHI)
		{ 
			std::cerr <<  "CHI CHI not set!"  << std::endl;
		}
		else
		{	
			std::cerr << "Unknown type of properties! " << std::endl;
		}
		return shift;
	} 

	
} // namespace BALL
