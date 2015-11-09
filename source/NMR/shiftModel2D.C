// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/NMR/shiftModel2D.h>
#include <BALL/NMR/shiftModel.h>
#include <BALL/KERNEL/PTE.h>
#include <BALL/KERNEL/bond.h>

using namespace std;

namespace BALL
{	
	ShiftModel2D::ShiftModel2D() 
		:	ShiftModule(),
			peaks_(),
			origin_(),
			dimension_(),
			spacing_(),	
			type_(),
			parameters_(),
			system_(NULL),
			valid_(false),
			compute_shifts_(true)
	{// ?? should we do this? 
			//	registerStandardModules_();
	}

	ShiftModel2D::ShiftModel2D(const String& filename, SPECTRUM_TYPE st, bool compute_shifts) 
		:	ShiftModule(),
			peaks_(),
			origin_(),
			dimension_(),
			spacing_(),
			type_(st),
			parameters_(filename),
			system_(NULL),
			valid_(false),
			compute_shifts_(compute_shifts)
	{
		//?? should we do this? 
		//registerStandardModules_();
		init_();
	}

	ShiftModel2D::ShiftModel2D(const String& filename,SPECTRUM_TYPE st, Vector2 origin, Vector2 dimension, Vector2 spacing, bool compute_shifts) 
		:	ShiftModule(),
			peaks_(),
			origin_(origin),
			dimension_(dimension),
			spacing_(spacing),
			type_(st),
			parameters_(filename),
			system_(NULL),
			valid_(false),
			compute_shifts_(compute_shifts)
	{
		//?? should we do this? 
		//registerStandardModules_();
		init_();
	}

	ShiftModel2D::ShiftModel2D(const ShiftModel2D& model)
		:	ShiftModule(),
			peaks_(model.peaks_),
			origin_(model.origin_),
			dimension_(model.dimension_),
			spacing_(model.spacing_),
			type_(model.type_),
			parameters_(model.parameters_),
			system_(NULL),
			valid_(false),
			compute_shifts_(model.compute_shifts_)
	{
		init_();
	}
	
	ShiftModel2D::~ShiftModel2D()
	{
		clear();
	}
	
			
	void ShiftModel2D::clear()
	{
		// model is invalid
		valid_ = false;

		// clear parameters
		parameters_.clear();
		
		peaks_.clear();
		system_ = NULL;
		
	}
	
	bool  ShiftModel2D::init_()
			throw(Exception::FileNotFound)
	{
		valid_ = true;

		// return the current state
		return valid_;

	}

	void ShiftModel2D::setFilename(const String& filename)
			throw(Exception::FileNotFound)
	{
		// set the parameter filename 
		parameters_.setFilename(filename);

		// ...and initialize!
		init_();
	}
	
	bool ShiftModel2D::isValid() const
	{
		return valid_;
	}


	bool ShiftModel2D::start()
	{	
		peaks_.clear();
		return true;
	}
	
	bool ShiftModel2D::finish()
	{
		if (!isValid())
		{
			return false;
		}
	
		if (!system_)
		{
			Log.info() << "No valid system found!" << std::endl;
			return false;
		}
		
		// compute the shift model if necessary
		if (compute_shifts_)
		{
			BALL::ShiftModel sm(parameters_.getFilename());
			system_->apply(sm);
		}
		
		if (type_== HSQC_NH  ||  type_ ==  HSQC_CH)
		{
			Element element_type1 = Element::UNKNOWN;
			Element element_type2 = Element::UNKNOWN;

			// Peter Bayer proposed as peak width  
			// for N--H 10hz/15Hz and
			// for C--H  5Hz/15Hz

			// peakwidth is meassured in ppm, since
			// experiments were done in Hz, we convert the values 
			// according to the formular
			// 
			// 		offset [Hz] = offset[ppm] * basic frequency
			//
			// for our prediction we assume a basic frequency of 700 MHz
			float peakwidth_proton = 0.0;
			float	peakwidth_atom = 0.0;

			if (type_ == HSQC_NH)
			{
				element_type1 = PTE[Element::H];
				 peakwidth_proton = 0.02142; // AKD 11.7.07
				//peakwidth_proton = 0.;
				element_type2 = PTE[Element::N];
				peakwidth_atom = 0.01428;  //AKD 11.07.07
				//peakwidth_atom = 0.;
			}
			else if (type_ == HSQC_CH)
			{
				element_type1 = PTE[Element::H];
				peakwidth_proton = 0.02142; //AKD 11.7.07
				//peakwidth_proton = 0.0;
				element_type2 = PTE[Element::C];
				peakwidth_atom = 0.00714; //AKD 11.7.07
				//peakwidth_atom = 0.0;
			}

			if (element_type1 == Element::UNKNOWN  || element_type2 == Element::UNKNOWN)
				return true;

			// for debugging
			int counter_element1 = 0;
			int counter_element2 = 0;
			int counter_peaks = 0;

// test ausgabe			
std::ofstream outfile("/home/HPL/anne/DEVELOP/NMR/2DSPECTRA/1z0r/1z0r.peaks");

			//look for valid atom pairs
			for (BALL::ResidueIterator r_it = system_->beginResidue(); +r_it; ++r_it)
			{	
				Atom* proton = NULL;
				Atom* atom = NULL;
			
				// do we have atoms in this residue? 
				for (BALL::AtomIterator at_it = r_it->beginAtom(); +at_it; ++at_it)
				{
					// for debugging
					if (at_it->getElement() == PTE[Element::H])
					{
						// is it bound to a backbone C or N??
						Atom::BondIterator b_it = at_it->beginBond();
						for (; +b_it; ++b_it)
						{	
							if (    b_it->getType()!=(Bond::TYPE__HYDROGEN) 
									&& (b_it->getPartner(*at_it)->getName()=="N" || b_it->getPartner(*at_it)->getElement() == PTE[Element::C]))
							{
								counter_element1++;
								proton = &(*at_it);
								atom = b_it->getPartner(*at_it);
							}
						}
					}
					// end debugging
					
					// at the moment we just allow backbone's N
					if ( (type_ == HSQC_NH) && (at_it->getName() == "N" ))
					{	
						atom = &(*at_it);
						// is it bound to a proton? 
						Atom::BondIterator b_it = atom->beginBond();
						for (; +b_it; ++b_it)
						{
							if (  b_it->getType()!=(Bond::TYPE__HYDROGEN)  && b_it->getPartner(*atom)->getElement() == element_type1)
							{
								proton = b_it->getPartner(*atom);
								createPeak_(proton, atom, peakwidth_proton, peakwidth_atom);
			// test ausgabe
			outfile << atom->getResidue()->getID() << " " << atom->getFullName() << " "  << atom->getTypeName() << " "
							<< proton->getFullName() << " " <<  proton->getTypeName() << " " 
							<< proton->getProperty(BALL::ShiftModule::PROPERTY__SHIFT).getFloat() << "  "
							<< atom->getProperty(BALL::ShiftModule::PROPERTY__SHIFT).getFloat() << std::endl;

								counter_peaks++;
							}
						}
						counter_element2++;
					}
					else if ( (type_ == HSQC_CH) && ( at_it->getElement() == PTE[Element::C]))
					{
						atom = &(*at_it);
						// is it bound to a proton? 
						Atom::BondIterator b_it = atom->beginBond();
						for (; +b_it; ++b_it)
						{
							if (  b_it->getType()!=(Bond::TYPE__HYDROGEN)  && b_it->getPartner(*atom)->getElement() == element_type1)
							{
								proton = b_it->getPartner(*atom);
								createPeak_(proton, atom, peakwidth_proton, peakwidth_atom);
	// test ausgabe
	outfile << atom->getResidue()->getID() << " " << atom->getFullName() << " "  << atom->getTypeName() << " "
					<< proton->getFullName() << " " <<  proton->getTypeName() << " " 
					<< proton->getProperty(BALL::ShiftModule::PROPERTY__SHIFT).getFloat() << "  "
					<< atom->getProperty(BALL::ShiftModule::PROPERTY__SHIFT).getFloat() << std::endl;

								counter_peaks++;
							}
	
							counter_element2++;
						}
					}										
				}
			}
	//testausgabe
	outfile.close();
			
			//debugging
			std::cout << "Number of peaks : " << counter_peaks << "  Number of N's: " << counter_element2 << "  Number of H's: " << counter_element1 << std::endl;

		} /*
		else 
		{
			Element element_type1 = Element::UNKNOWN;
			Element element_type2 = Element::UNKNOWN;

			// Peter Bayer proposed as peak width  
			// for N--H 10hz/15Hz and
			// for C--H  5Hz/15Hz

			// peakwidth is meassured in ppm, since
			// experiments were done in Hz, we convert the values 
			// according to the formular
			// 
			// 		offset [Hz] = offset[ppm] * basic frequency
			//
			// for our prediction we assume a basic frequency of 700 MHz
			float peakwidth_atom1 = 0.0;
			float	peakwidth_atom2 = 0.0;
			
			if (type_ == HSQC_NH)
			{
				element_type1 = PTE[Element::H];
				peakwidth_atom1 = 0.02142;
				element_type2 = PTE[Element::N];
				peakwidth_atom2 = 0.01428;
			}
			else if (type_ == HSQC_CH)
			{
				element_type1 = PTE[Element::H];
				peakwidth_atom1 = 0.02142;
				element_type2 = PTE[Element::C];
				peakwidth_atom2 = 0.00714;
			}
		
			if (element_type1 == Element::UNKNOWN  || element_type2 == Element::UNKNOWN)
				return true;

	
			//look for valid atom pairs
			for (BALL::ResidueIterator r_it = system_->beginResidue(); +r_it; ++r_it)
			{	
				Atom* atom1 = NULL;
				Atom* atom2 = NULL;

				// do we have atom1 and atom2 in this residue? 
				for (BALL::AtomIterator at_it = r_it->beginAtom(); +at_it; ++at_it)
				{
					if (at_it->getElement() == element_type1)
					{
						atom1 = &(*at_it);
					}

					if (at_it->getElement() == element_type2)
					{
						atom2 = &(*at_it);
					}

					if (atom1 && atom2 )  
					{
						createPeak_(atom1, atom2,	peakwidth_atom1, 	peakwidth_atom2 );
					}
				}
			}
		}*/
		return true;
	}

	void ShiftModel2D::createPeak_(Atom* atom1, Atom* atom2, float peakwidth_atom1, float peakwidth_atom2)
	{
//std::cout << "Atoms: atom1:" << atom1->getFullName()<< " -- atom2: " << atom2->getFullName() << std::endl; 
		// we have, get the shift
		float shift1 = atom1->getProperty(BALL::ShiftModule::PROPERTY__SHIFT).getFloat();
		float shift2 = atom2->getProperty(BALL::ShiftModule::PROPERTY__SHIFT).getFloat();
		
/*		//do we have to exlcude Prolines?
		if (type_ == HSQC_NH && atom2->getResidue()->getName() == "PRO" && atom2->getName() == "N")
		{ 
 			shift2 = 0.;
		} */
		
		//store the shift in the peak list
		Peak2D peak;
		Vector2 pos(shift1, shift2); 
		peak.setPosition(pos);

	//std::cout << " halo " << atom2->getName() << " " << atom2->getResidue()->getName() << ": " <<  shift2  << "     "<< atom1->getName() << " " << atom1->getResidue()->getName() << ": " <<  shift1  << std::endl;
		peak.setWidth(Vector2(peakwidth_atom1,peakwidth_atom2));
		peak.setIntensity(1.);
		//setAtom();
		peaks_.push_back(peak);
	}

	
	Processor::Result ShiftModel2D::operator () (Composite& composite)
	{
		Processor::Result result = Processor::CONTINUE;

                if (!system_ && RTTI::isKindOf<Atom>(&composite))
		{
			Atom* atom = dynamic_cast<Atom*>(&composite);
                        if  (RTTI::isKindOf<System>(&atom->getRoot()))
			{	
				system_ = dynamic_cast<System*>(&(atom->getRoot()));
			}
		}
		return result;
	}

	void ShiftModel2D::operator >> (Spectrum2D& spectrum)
  {
     // this overwrites the parameter
     spectrum = Spectrum2D(peaks_, origin_, dimension_, spacing_);
  }
			

	std::ostream& operator << (std::ostream& os, const BALL::ShiftModel2D& shiftmodel)
	{
		// output the data in gnuplottable format :-)
		// e.g. ShiftModel sm; ...; File f("plot.dat", std::ios::out); f << sm;
		// gnuplot -persist plot.dat
		const std::vector<Peak2D>& peaks = shiftmodel.getPeaks();

		Size length = peaks.size();
		for (Size i = 0; i < length; i++)
		{
				RegularData2D::CoordinateType pos = peaks[i].getPosition();

				os << pos.x << " " << pos.y << " " << peaks[i].getIntensity() << std::endl;	
		}

		return os;
	}
}

