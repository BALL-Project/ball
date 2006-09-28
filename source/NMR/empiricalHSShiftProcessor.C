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
#define STRING_VALUE_NA "BADVAL"
#define CHAR_VALUE_NA  '?'

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
										
				if (shs.isvalid())
				{
					//insert in the map of hypersurfaces
					std::pair<String, String> tuple(first_property, second_property);
					hypersurfaces_[atom_type][tuple]= shs;
				}
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
std::cout << "******************* EHS-Shift start-end" << std::endl; 
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
				if (atom_type == "CA")
				{
				//	std::cout << "hs: " << property_pairs_[atom_type][j].first << " " << property_pairs_[atom_type][j].second << " " 
					//	        << hypersurfaces_[atom_type][property_pairs_[atom_type][j]](target) << std::endl;
				}		
				if (hypersurfaces_[atom_type][property_pairs_[atom_type][j]].isvalid())
				EHS_shift += hypersurfaces_[atom_type][property_pairs_[atom_type][j]](target);
			}
			// set the shift contribution by empiricalHSShiftProcessor
			target.atom->setProperty(PROPERTY__EHS_SHIFT, EHS_shift);
			 
			// add it to the total shift
			float old_shift = target.atom->getProperty(ShiftModule::PROPERTY__SHIFT).getFloat(); 
			target.atom->setProperty(ShiftModule::PROPERTY__SHIFT, (old_shift + EHS_shift)); 
// std::cout << "EHS SHIft: "<< EHS_shift << std::endl;
		}

		//printTargets_();
		//printParameters_();
		
		// do the ShiftX correction
		postprocessing_();
		return true;
	}
	
		
	Processor::Result  EmpiricalHSShiftProcessor::operator () (Composite& composite)
		throw()
	{
		// Here, we collect all target atoms, specified in the section 
		// "EmpiricalShiftHyperSurfaces" of the ShiftX.ini file,
		// and the corresponding properties which are likewise specified 
		// in the ShiftX.ini-file. Since there is no object in BALL to 
		// store the type of property in init() and then use it directly here to
		// compute and store these properties, we store the _type_ of the 
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
		
		In order to be conform with ShiftX we leave out GLY and ALA
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
			Log.info() << "Chi torsion angle of " << residue->getID() << "-"<<  residue->getName() << " could not be computed!" << std::endl;
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
			Log.info() << "Chi2 torsion angle of " << residue->getID() << "-"  << residue->getName() << " could not be computed!" << std::endl;
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
		throw()
	{
		char ret = CHAR_VALUE_NA;
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
		char ret = CHAR_VALUE_NA;
		if (residue->isAminoAcid())
		{
			ret = Peptides::OneLetterCode(residue->getName()); 
		}
		return ret;
	}

	
	float EmpiricalHSShiftProcessor::PropertiesForShift_::getHA_HBondLen_(Residue* residue)
		throw()
	{
		//float len = FLOAT_VALUE_NA;	
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
				if(bi->getType() == Bond::TYPE__HYDROGEN)
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
		//float len = FLOAT_VALUE_NA ;
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
				if(bi->getType() == Bond::TYPE__HYDROGEN)
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
		//float len = FLOAT_VALUE_NA;	
		float len = 0.;

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
		throw()
	{
//		float len = FLOAT_VALUE_NA;
		float len = 0.;
		
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
				if(bi->getType() == Bond::TYPE__HYDROGEN)
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
				if(bi->getType() == Bond::TYPE__DISULPHIDE_BRIDGE)
				{
					ret = true;
				}
			}
		}	
		return ret; 
	}
	
	bool EmpiricalHSShiftProcessor::PropertiesForShift_::hasHA_HBond_(Residue* residue) 
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
				if(bi->getType() == Bond::TYPE__HYDROGEN)
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
				if(bi->getType() == Bond::TYPE__HYDROGEN)
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
				if(bi->getType() == Bond::TYPE__HYDROGEN)
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

		// we compute all properties needed for this atom type 
		// NOTE: if a certain property is not available, it gets
		// 		the value FLOAT_VALUE_NA or STRING_VALUE_NA
		// 		thus we can evaluate unknown values 
		// 		by assinging them the average value 
		// 	  (which is the way ShiftX works :-) )
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

					properties_string_[("FR_P")] = 'N';
			}
			else
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
				{	
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
				else if ((*it) == "HN_P" )
				{
					properties_string_[(*it)]  = (hasHN_HBond_(prev_residue)? "Y": "N");
				}
				else if ((*it) == 	"OHL_P" )
				{
					properties_real_[(*it)] = getO_HBondLen_(prev_residue);	
				}
				else if ((*it) == 	"OH_P" )
				{
					properties_string_[(*it)] = (hasO_HBond_(prev_residue) ? "Y": "N");	
				}
				else if ((*it) == 	"DISULFIDE_P" )
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
					properties_string_[("CHI_N")] = "Unknown";// STRING_VALUE_NA;	
					properties_string_[("CHI2_N")] = "Unknown";//STRING_VALUE_NA;	

					//properties_string_[("FR_N")] = STRING_VALUE_NA;'N';
					properties_string_[("FR_N")] = 'N';

			}
			else
			{
				if ((*it) == 	"FR_N" )
				{	
					properties_string_[(*it)]=  (next_residue->isNTerminal()? "Y": "N");	
				}
				else if ((*it) == 	"AA_N" )
				{
					properties_string_[(*it)]= getAminoAcid_(next_residue);
				}else if ((*it) == 	"SS_N" )
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
					}else
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
					}else
					{
						properties_real_[("PHI_N")]  = FLOAT_VALUE_NA;
					}
				}
				else if ((*it) == 	"CHI_N")
				{
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
						if (   (prev_residue->getName() == "ALA") 
							|| (prev_residue->getName() == "GLY")	
							|| (prev_residue->getName() == "SER") 	
							|| (prev_residue->getName() == "CYS")  )
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
				else if((*it) == 	"HA1_N")
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
					properties_string_[(*it)]= (hasO_HBond_(next_residue)? "Y": "N");	

				}
				else if ((*it) == 	"DISULFIDE_N" )
				{
					properties_string_[(*it)]= 	(hasDisulfidBond_(next_residue)? "Y": "N");		
				}
			}


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
			{	
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
					if (   (prev_residue->getName() == "ALA") 
							|| (prev_residue->getName() == "GLY")	
							|| (prev_residue->getName() == "SER") 	
							|| (prev_residue->getName() == "CYS")  )
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
				properties_string_[(*it)]=( hasHA_HBond_(residue)? "Y": "N");
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
				properties_string_[(*it)]= (hasO_HBond_(residue)? "Y": "N");		
			}
			else if ((*it) == 	"DISULFIDE" )
			{
				properties_string_[(*it)]= (hasDisulfidBond_(residue)? "Y": "N");			
			}
			// the following both cases are due to SHIFTX
			else if ((*it) == "ROW")
			{
				properties_string_[(*it)] = "N";	
			}
			else if ((*it) == "HBONDSTAT")
			{
				// TODO: first position reflects existence of HA1 HBond (Length > 0.)
				// 			 second position reflects existence of HA2 HBond     "
				// 			 third position reflects HN HBond Length             "
				// 			 fourth position reflects ?? HBond length
				//
				// 			 0.0 ? 'N' : 'Y';
				properties_string_[(*it)] = "YYYY";	
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
//std::cout << filename <<  std::endl;
		// find the data file
		BALL::Path p;
		String file_name = p.find("NMR/"+filename) ;// "NMR/splinedata/hat_PSI_DISULFIDE-1.dat");
		if (file_name == "")
		{
			Log.info() << "File " << filename << " could not be found" << std::endl;
			invalid_ = true;
			return;
		}
		
		invalid_ = false;
		
		//set the x property and the y property
		first_property_  = firstproperty;
		second_property_ = secondproperty;

		
		
		// set the Hypersurface type
		setType_(firstproperty, secondproperty);
		

		//
		// read and store the data
		//

		BALL::File file(file_name, std::ios::in);

		// parse the data file
		parseDataFile_(file, filename);

		if (type_ == SINGLE__REAL) // we have a single spline
		{  
			// create a spline	
			CubicSpline1D_ s;
			vector<float> x_axis;
			convertToReal_(x_axis_values_[0], x_axis);
			s.createSpline(x_axis, sample_values_[0], true);
			s.setAverage(average_);
			
			// store him in the map
			s1d_[first_property_] = s;
		}
		else if (type_ == SINGLE__CHI || type_ == SINGLE__DISCRETE)  // we have a single table line
		{ 
			// NOTE: in the datafile the values are stored diagonally! 
			for (Position i = 0; i < x_axis_values_[0].size(); i++)  
			{
				// in order to save storage, the data is compressed into a single line
				table_[first_property_][x_axis_values_[0][i]] = sample_values_[i][i];
			}
			//std::cout << String(atoi("60.000000"))<< "single CHI: bei 60 " << table_[first_property_]["60.000000"]<< "soll:" << sample_values_[0][0] <<  std::endl;

		}
		else   // if we have a normal "table", 
		{
			// depending on the types, create 	
			// 					a 2D bicubic spline, 
			// 	  			a map of 1D bicubic splines or 
			//	  			a lookUpTable (map of map)  
			if (type_ == REAL__REAL)
			{
				vector<float> y_axis;
				convertToReal_(y_axis_values_, y_axis);

				vector<vector<float> > x_axis;
				for (Position i=0; i<y_axis.size(); i++)
				{
					vector<float> v;
					convertToReal_(x_axis_values_[i], v);
					x_axis.push_back(v);
				}

				// a bicubic spline is stored
				s2d_.createBiCubicSpline(x_axis, y_axis, sample_values_);
			}
			else if (type_ == REAL__DISCRETE) 
			{
				// 		!!!!  C A U T I O N !!!! 
				// When accessing the data, one has to switch X and Y, 
				// since for each discrete value a 1D bicubic spline is stored
				for (Position i = 0; i < y_axis_values_.size(); i++)
				{
					// create a 1D bicubic spline
					CubicSpline1D_ s;
					vector<float> x_axis;
					convertToReal_(x_axis_values_[i], x_axis);

					s.createSpline(x_axis, sample_values_[i], true);
					s.setAverage(row_averages_[i]);
					// store him in the map
					s1d_[y_axis_values_[i]] = s;
				}
			}
			else if (type_ == REAL__CHI)
			{
				// 		!!!!  C A U T I O N !!!! 
				// When accessing the data, one has to switch X and Y, 
				// since for each discrete value a 1D bicubic spline is stored
				for (Position i = 0; i < y_axis_values_.size(); i++)
				{
					// create a 1D bicubic spline
					CubicSpline1D_ s;
					vector<float> x_axis;
					convertToReal_(x_axis_values_[i], x_axis);

					s.createSpline(x_axis, sample_values_[i], true);

					// store him in the map
					s1d_[y_axis_values_[i]] = s;
				}
			}
			else if (type_ == DISCRETE__REAL)
			{ 
std::cout << "DISCRETE__REAL not implemented" << std::endl;
			}
			else if ( (type_ == DISCRETE__DISCRETE) || (type_ == CHI__DISCRETE)|| (type_ == DISCRETE__CHI) || (type_ == CHI__CHI) )
			{
				if (x_axis_values_.size() != y_axis_values_.size())
				{
					std::cerr << "Tried to read an invalid table in file"<< filename <<  std::endl;
				}
				else
				{
					for (Position i = 0; i < y_axis_values_.size(); i++)  // y
					{
						 for (Position j = 0; j < x_axis_values_[i].size(); j++) // x
					 	{ 	
							 table_[x_axis_values_[i][j]][y_axis_values_[i]] = sample_values_[i][j];
					 	}
					}
				}
			}	
			else if (type_ == CHI__REAL)
			{
std::cout << "CHI__REAL not implemented" << std::endl;
			}		
			else if (type_ == CHI__CHI)
			{
				// Fortunately this case does not occur
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
		// is the first CHI?
		if(PropertiesForShift_::isMixed(firstproperty))
		{
			// is the second CHI? 
			if (PropertiesForShift_::isMixed(secondproperty))
			{	
				if (firstproperty == secondproperty)
					type_ = SINGLE__CHI;
				else
					type_ = CHI__CHI;
			}
			// is the second discrete
			else if (PropertiesForShift_::isDiscrete(secondproperty))
			{
				type_ = CHI__DISCRETE;
//std::cout << "first pro: " << firstproperty << " :second pro" << secondproperty << std::endl;
			}
			else  // second is real
			{	
				type_ = CHI__REAL;
			}	
		}		// is the first discrete? 
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
			}
			else if (PropertiesForShift_::isMixed(secondproperty))
			{
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

	void EmpiricalHSShiftProcessor::ShiftHyperSurface_::convertToReal_(const vector<String>& input, vector<float>& output)
		throw()
	{
		output.clear();
		for (Position i=0; i<input.size(); i++)
			output.push_back(input[i].toFloat());
	}
	
	// Structure of all input files:
	//
	// atomtype factorx factory
	// total_average
	// (row_average_1;row_average_2;...;row_average_n|N/A)
	// (col_averages_1;col_averages_2;...;col_averages_n|N/A)
	// (y_axis_1;...;y_axis_n|N/A)
	// x_axis_11;...;x_axis_1m
	// value_11;...;value_1m
	// ...
	// x_axis_n1;...;x_axis_nm
	// value_n1;...;value_nm
	void EmpiricalHSShiftProcessor::ShiftHyperSurface_::parseDataFile_(BALL::File& file, String filename) 
		throw()
	{
		String line;
		std::vector<BALL::String> fields;
		
		row_averages_.clear();
		col_averages_.clear();

		try {
			// read over the first line. we don't need the information currently
			line.getline(file);

			// now read the total average. this is contained in _all_ datafiles, so we can depend on it being there
			line.getline(file);
			average_ = line.toFloat();
			
			// test for row averages
			line.getline(file);
			String testline = line;
			testline.toUpper();
			
			std::vector<float> row_average_values;
			if (!testline.hasSubstring("N/A"))
			{
				// parse the row averages
				line.split(fields, ";");
				for (Position i=0; i<fields.size(); i++)
					row_average_values.push_back(fields[i].toFloat());
			}

			// test for col averages
			line.getline(file);
			testline = line;
			testline.toUpper();

			std::vector<float> col_average_values;
			if (!testline.hasSubstring("N/A"))
			{
				// parse the row averages
				line.split(fields, ";");
				for (Position i=0; i<fields.size(); i++)
					col_average_values.push_back(fields[i].toFloat());
			}

			// test for y_axis
			line.getline(file);
			testline = line;
			testline.toUpper();

			if (!testline.hasSubstring("N/A"))
			{
				// parse the row averages
				line.split(fields, ";");
				for (Position i=0; i<fields.size(); i++)
					y_axis_values_.push_back(fields[i]);
			}	
			
			// finally, read the consecutive x_axis / value lines
			Size number_of_lines = std::max((size_t)1, y_axis_values_.size());
			x_axis_values_.clear();
			x_axis_values_.resize(number_of_lines);
			sample_values_.clear();
			sample_values_.resize(number_of_lines);

			// read the values
			for (Position i=0; i<number_of_lines; i++)
			{
				line.getline(file);
				line.split(fields, ";"); 

				for (Position j = 0; j < fields.size(); j++)
					x_axis_values_[i].push_back(fields[j]);

				line.getline(file);
				line.split(fields, ";"); 	

				for (Position j = 0; j < fields.size(); j++)
					sample_values_[i].push_back(fields[j].toFloat());
			}

			// and build the row_ and col_averages if applicable
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
		throw()
	{
		float shift = 0.;
		// for the cases DISCRETE__DISCRETE
		// 							 CHI__DISCRETE
		// 							 DISCRETE__CHI
		// 							 SINGLE__DISCRETE
		// 							 SINGLE__CHI
		// we precompute the access string
		// 
		// NOTE: chi properties have a "mixed" nature 
		// the property can have a numeric or alphanumeric value
		// since the alphanumeric values are treated as 
		// a bin we convert the alphanumeric value into string 
		// representing the bin :-) 
		// NOTE: for the singles we take the property itself as first access string
		
		String string1 = properties[first_property_].second;
		String string2 = properties[second_property_].second;

		// special case1 : CHI
		if (PropertiesForShift_::isMixed(first_property_))
		{	
//std::cout << "chi1: " << properties[first_property_].first << "|" << properties[first_property_].second<< std::endl;
			
			string1 = properties[first_property_].second;
			
			// is the property numeric or alphanumeric? 
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
					std::cerr<< "There is a problem for atom " << properties.atom->getName() << ": " 
						<< second_property_ << "-value is not valid!" << std::endl;
			}
		}
		
		if (PropertiesForShift_::isMixed(second_property_))
		{
//std::cout << "chi2: " << properties[first_property_].first << "|" << properties[first_property_].second<< std::endl;

			string2 = properties[second_property_].second; 
			// is the property numeric or alphanumeric? 
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
					std::cerr<< "There is a problem for atom " << properties.atom->getName() << ": " 
						<< second_property_ << "-value is not valid!" << std::endl;
			}
		}
		// now we can assume that for chi the correct alphanumeric string is set! 
		
		// Special case 2: SINGLE__??
		if (type_ == SINGLE__DISCRETE || type_ == SINGLE__CHI)
		{ 
			//Remember: we did this to save space
			string1 = first_property_;
		}
		
		//
		// now access the hypersurface according to its type
		// 
		if (   type_ == SINGLE__DISCRETE || type_ == SINGLE__CHI    || type_ == CHI__CHI 
				|| type_ == CHI__DISCRETE    || type_ == DISCRETE__CHI  || type_ == DISCRETE__DISCRETE)
		{
			// find out if the first property is contained in the table
			tabletype::iterator first_it = table_.find(string1);
			if (first_it != table_.end())
			{  
				// yes it is :-)
				// check if the second property is contained in the table
				std::map<String, float>::iterator second_it = first_it->second.find(string2);
				if (second_it != first_it->second.end())
					// so both accessors are valid: we can just return the value
					shift = second_it->second; //  table_[string1][string2];	
				else
//					shift = row_averages_[string1];
						shift = average_;
					// average over the row we already found
					//shift = getTableXAverage(properties[first_property_].second);// getTableRowAverage_(first_it->second);
			}
			else // the first accessor is not valid! 
			{	
				// does the second property occur at all?
				if (tableHasColumn_(string2))
				{	// return the column average
//					shift = getTableYAverage(properties[second_property_].second);//getTableColumnAverage_(string2);
						shift = average_;
						//shift = col_averages_[string2];
				}
				else
				{
					// we don't have the value at all... average over the whole table
					shift = average_; //getTableAverage_(); 
					Log.info() << "Took the average because of invalid properties of atom " << properties.atom->getResidue()->getID() 
					<< properties.atom->getResidue()->getName()
					<< "-" << properties.atom->getName() << "   : " << first_property_ <<"/" << second_property_ 
					<< " =  " << string1 << "/"  << string2 << std::endl;
				}
			}
		}
		else if (type_ == SINGLE__REAL)
		{ 
			// was the property set?
			if (s1d_.find(first_property_) != s1d_.end())
			{
				if (properties[first_property_].first == FLOAT_VALUE_NA)
					shift = average_;
				else 
					shift = s1d_[first_property_](properties[first_property_].first);
			}
			else 
				shift = 0.;
			//std::cout << "single spline for " << first_property_ << " " << shift << " " << properties.atom->getFullName() << std::endl;
		}
		else if (type_ == REAL__REAL)
		{		
			// This simulates SHIFTX behaviour: if only one factor is out of bounds, we return the all-values average
			if ((properties[first_property_].first == FLOAT_VALUE_NA) || (properties[second_property_].first == FLOAT_VALUE_NA))
			{
				shift = average_;
			}
			else 
				shift = s2d_(properties[first_property_].first, properties[second_property_].first);
			
//std::cout << properties.atom->getName() << "  " << first_property_ << ":" << second_property_<< " -- " << properties[first_property_].first<< "::" << properties[second_property_].first << " -- " << shift << std::endl;
		
		}
		else if (type_ == REAL__DISCRETE)
		{
			// This simulates SHIFTX behaviour: if only one factor is out of bounds, we return the all-values average
			// TODO: For some reason, SHIFTX does not seem to use the row_averages here! Find out why!!!
			if (   (properties[first_property_].first == FLOAT_VALUE_NA)
					 ||(properties[second_property_].second == STRING_VALUE_NA) )
			{
				// return the total average over the whole map
				shift = average_;
			}
			else
				shift = s1d_[properties[second_property_].second](properties[first_property_].first);
		}
		else if (type_ == DISCRETE__REAL)
		{
//std::cerr<< "Discrete Real should NEVER be called!! "<< std::endl;

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
	/*	else if (type_ == DISCRETE__DISCRETE) 
		{
			// find out if the second property is contained in the table
			tabletype::iterator second_it = table_.find(properties[second_property_].second);

			if (second_it != table_.end())
			{
				// find out if the first property is contained in the table
				std::map<String, float>::iterator first_it = second_it->second.find(properties[first_property_].second);

				if (first_it != second_it->second.end())
					// we can just return the value
					shift = first_it->second;
				else
					// average over the row we already found
					shift = getTableRowAverage_(second_it->second);
			}
			else
			{	
				// does the column property occur at all?
				if (tableHasColumn_(properties[first_property_].second))
					// return the column average
					shift = getTableColumnAverage_(properties[first_property_].second);
				else
					// we don't have the value at all... average over the whole table
					shift = getTableAverage_();
			}
		
		//	std::cout << "shift: " << shift<< std::endl;
		}*/
		else if (type_ == CHI__REAL)
		{	
			std::cout << "CHI REAL should NEVER be called " << std::endl;
		}
		else if (type_ == REAL__CHI)
		{	
			if (s1d_.find(string2) != s1d_.end())
				shift = s1d_[string2](properties[first_property_].first);	
			else
			{
				std::cerr << "Tried to access the hypersurface for atom " << properties.atom->getResidue()->getID() 	
					<< properties.atom->getResidue()->getName()
					<< "-" << properties.atom->getName() << "'s properties " << first_property_ <<"/" << second_property_ 
					<< " with " << properties[first_property_].first << "/"  << string2<< std::endl;
				shift = average_; // is this the correct thing to do?
			}
		}
		else
		{	
			std::cerr << "Unknown type of properties! " << std::endl;
		}

		//std::cout << "_operator (): ";
		//std::cout <<properties.atom->getName() << "  " << first_property_ << ":" << second_property_<< " -- " << properties[first_property_].first << "/" << properties[first_property_].second<< ":" << properties[second_property_].first << "/" << properties[second_property_].second <<  " -- " << shift << std::endl;
		
		return shift;
	} 

	float EmpiricalHSShiftProcessor::ShiftHyperSurface_::getTableAverage_()
		throw()
	{
		float average = 0.;
		int count = 0;

		tabletype::iterator row = table_.begin();
		for (; row != table_.end(); ++row)
		{
			std::map<String, float>::iterator column = row->second.begin();
			for (; column != row->second.end(); ++column)
			{
				average += column->second;
				count++;
			}	 
		}

		if (count > 0)
			average /= count;

		return average;
	}
	
	float EmpiricalHSShiftProcessor::ShiftHyperSurface_::getTableRowAverage_(const std::map<String, float>& row)
		throw()
	{
		float average = 0.;
		int   count = 0;

		std::map<String, float>::const_iterator column = row.begin();
		for (; column != row.end(); ++column)
		{
			average += column->second;
			count++;
		}

		if (count > 0)
			average /= count;

		return average;
	}

	float EmpiricalHSShiftProcessor::ShiftHyperSurface_::getTableColumnAverage_(const String& name)
		throw()
	{
		float average = 0.;
		int   count = 0;

		tabletype::iterator row = table_.begin();
		for (; row!=table_.end(); ++row)
		{
			std::map<String, float>::iterator column = row->second.find(name);
			if (column != row->second.end())
			{
				average += column->second;
				count++;
			}
		}

		if (count > 0)
			average /= count;

		return average;
	}

	bool EmpiricalHSShiftProcessor::ShiftHyperSurface_::tableHasColumn_(const String& name)
		throw()
	{
		tabletype::iterator row = table_.begin();
		for (; row!=table_.end(); ++row)
			if (row->second.find(name) != row->second.end())
				return true;

		return false;
	}
	
/*	
	float EmpiricalHSShiftProcessor::ShiftHyperSurface_::getTableXAverage(const String& name) 
		throw()
	{
		tabletype::iterator it = table_.find(name);
		if (it != table_.end())
		{
			Log.info() << "Not yet implemented!" << std::endl;
		}
		else
		{
			Log.info() << "Invalid value for X average determination" << std::endl;
			return 0.0;
		}
	}
	
	float EmpiricalHSShiftProcessor::ShiftHyperSurface_::getTableYAverage(const String& name) 
		throw()
	{
		Log.info() << "Not yet implemented!" << std::endl;
	}
*/

				
				
				
	void  EmpiricalHSShiftProcessor::postprocessing_()
		throw()
	{
		// get the System
		System* system = NULL;
		
		for (Position i = 0; i < targets_.size(); i++)
		{
			if  (RTTI::isKindOf<System>(targets_[i].atom->getRoot()))
			{	
				system = dynamic_cast<System*>(&(targets_[i].atom->getRoot()));
			}
		}

		if (system) 
		{
			// add for all CA 0.2 times the values of HA
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
			std::cerr << "found no system -> could not perform a postprocessing for EFShiftProcessor" << std::endl;
		}
	}



	
} // namespace BALL
