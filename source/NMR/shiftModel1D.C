
// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/NMR/shiftModel1D.h>
#include <BALL/NMR/shiftModel.h>
#include <BALL/KERNEL/PTE.h>
#include <BALL/KERNEL/bond.h>

using namespace std;

namespace BALL
{
	ShiftModel1D::ShiftModel1D() 
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
	{
		// TODO: should we do this? 
			//	registerStandardModules_();
	}

	ShiftModel1D::ShiftModel1D(const String& filename, SPECTRUM_TYPE st, bool compute_shifts) 
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
		// TODO: should we do this? 
		//registerStandardModules_();
		init_();
	}

	ShiftModel1D::ShiftModel1D(const String& filename,SPECTRUM_TYPE st, double origin, double dimension, double spacing, bool compute_shifts) 
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

	ShiftModel1D::ShiftModel1D(const ShiftModel1D& model)
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
	
	ShiftModel1D::~ShiftModel1D()
	{
		clear();
	}
	
			
	void ShiftModel1D::clear()
	{
		// model is invalid
		valid_ = false;

		// clear parameters
		parameters_.clear();
		
		peaks_.clear();
		system_ = NULL;
		
	}

	bool  ShiftModel1D::init_()
			throw(Exception::FileNotFound)
	{
		valid_ = true;

		// return the current state
		return valid_;

	}

	void ShiftModel1D::setFilename(const String& filename)
			throw(Exception::FileNotFound)
	{
		// set the parameter filename 
		parameters_.setFilename(filename);

		// ...and initialize!
		init_();
	}
	
	bool ShiftModel1D::isValid() const
	{
		return valid_;
	}	
	
	bool ShiftModel1D::start()
	{	
		peaks_.clear();
		return true;
	}
	
	bool ShiftModel1D::finish()
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

		String element = "";
		
		// Peter Bayer proposed as peak width  
		// for H      15Hz
		// for N			10hz
		// for C			5Hz

		// peakwidth is meassured in ppm, since
		// experiments were done in Hz, we convert the values 
		// according to the formular
		// 
		// 		offset [Hz] = offset[ppm] * basic frequency
		//
		// for our prediction we assume a basic frequency of 700 MHz
		float peakwidth = 0.0;
		
		switch(type_)
		{
			case H:
			case H_ON_BACKBONE:
				element = "H";
				//peakwidth = 0.02142; // Peter Bayers estimation
				peakwidth = 0.0032; // this is the former BALL estimation
				break;
			case N:
			case N_BACKBONE:
				element = "N";
				peakwidth = 0.01428;
				break;
			case C:
			case C_BACKBONE:  
				element = "C";
				peakwidth = 0.00714;
				break;
		}
		int counter = 0;

		if (element == "" )
			return true;
		
		for (BALL::ResidueIterator r_it = system_->beginResidue(); +r_it; ++r_it)
		{	
			Atom* atom = NULL;

			for (BALL::AtomIterator at_it = r_it->beginAtom(); +at_it; ++at_it)
			{
				if (hasType_(&(*at_it), type_))
				{
					counter++;
					atom = &(*at_it);
					// we have, get the shift
					float shift = atom->getProperty(BALL::ShiftModule::PROPERTY__SHIFT).getFloat();
					Peak1D peak;
					
					float pos = shift; 
					peak.setPosition(pos);
				
					peak.setWidth(peakwidth);
					peak.setIntensity(peak.getIntensity()+1);
					//setAtom();
					peaks_.push_back(peak);
				}
			}
		}

		std::cout << "Number of considered atoms: "<< counter << std::endl;
		return true;
	}

	Processor::Result ShiftModel1D::operator () (Composite& composite)
	{

		Processor::Result result = Processor::CONTINUE;

		if (!system_ && RTTI::isKindOf<Atom>(composite))
		{
			Atom* atom = dynamic_cast<Atom*>(&composite);
			if  (RTTI::isKindOf<System>(atom->getRoot()))
			{	
				system_ = dynamic_cast<System*>(&(atom->getRoot()));
			}
		}
		return result;
	}


	void ShiftModel1D::operator >> (Spectrum1D& spectrum)
  {
     // this overwrites the parameter
  	spectrum = Spectrum1D(peaks_, origin_, dimension_, spacing_);

  }

	
	bool ShiftModel1D::hasType_(Atom* a, SPECTRUM_TYPE type)
	{
		bool ret = false;
		switch (type)
		{
			case H:
				if (a->getElement() == PTE[Element::H])
					ret = true;
				 //if (   (a->getName().hasSubstring("HG")) || (a->getName().hasSubstring("HD")) 
					//	|| (a->getName().hasSubstring("HB")) || (a->getName().hasSubstring("HA"))
					//	|| (a->getName().hasSubstring("HE")) || (a->getName().hasSubstring("HH")) 
					//	|| (a->getName().hasSubstring("HZ")))
				 //	ret = true;
				break;
			case C:
				if  (a->getElement() ==  PTE[Element::C])
					ret = true;
			//	if (   (a->getName().hasSubstring("CA")) || (a->getName().hasSubstring("CB")) 
			//			|| (a->getName().hasSubstring("CD")) || (a->getName().hasSubstring("CE")) 
			//			|| (a->getName().hasSubstring("CH")) || (a->getName().hasSubstring("CG")) || (a->getName().hasSubstring("CZ")) )
			////		ret = true;
				break;
			case N:	
				if  (a->getElement() ==  PTE[Element::N])
					ret = true;
 				//if (   (a->getName() == "N") || (a->getName().hasSubstring("ND")) || (a->getName().hasSubstring("NE")) 
				//	 	|| (a->getName().hasSubstring("NH")) || (a->getName().hasSubstring("NZ"))  )
				//	ret = true;
				break;
			case H_ON_BACKBONE: //check, whether the given atom is of type hydrogen and whether it is bound to a backbone C or N
				if (a->getElement() == PTE[Element::H])
				{
					// is it bound to a backbone C or N??
					Atom::BondIterator b_it = a->beginBond();
					for (; !ret && +b_it; ++b_it)
					{	
						if (    b_it->getType()!=(Bond::TYPE__HYDROGEN) 
								&& (b_it->getPartner(*a)->getName()=="N" || b_it->getPartner(*a)->getName()=="CA" || b_it->getPartner(*a)->getName()=="C"))
						{
							ret = true;
						}
					}
				}
				break;
			case C_BACKBONE:
				if  (a->getName() == "CA" || a->getName() == "C" )
					ret = true;
				break;
			case N_BACKBONE:
				if (a->getName() == "N")
						ret = true;
				break;
		}
		
		return ret;
	}


	
}
