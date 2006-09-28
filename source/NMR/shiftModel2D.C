#include <BALL/NMR/shiftModel2D.h>
#include <BALL/NMR/shiftModel.h>


using namespace std;

namespace BALL
{	
	ShiftModel2D::ShiftModel2D() 
		throw()
		:	ShiftModule(),
			peaks_(),
			origin_(),
			dimension_(),
			spacing_(),	
			type_(),
			parameters_(),
			system_(NULL),
			valid_(false)		
	{// ?? should we do this? 
			//	registerStandardModules_();
	}

	ShiftModel2D::ShiftModel2D(const String& filename, SPECTRUM_TYPE st) 
		throw()
		:	ShiftModule(),
			peaks_(),
			origin_(),
			dimension_(),
			spacing_(),
			type_(st),
			parameters_(filename),
			system_(NULL),
			valid_(false)		
	{
		//?? should we do this? 
		//registerStandardModules_();
		init_();
	}

	ShiftModel2D::ShiftModel2D(const String& filename,SPECTRUM_TYPE st, Vector2 origin, Vector2 dimension, Vector2 spacing) 
		throw()
		:	ShiftModule(),
			peaks_(),
			origin_(origin),
			dimension_(dimension),
			spacing_(spacing),
			type_(st),
			parameters_(filename),
			system_(NULL),
			valid_(false)		
	{
		//?? should we do this? 
		//registerStandardModules_();
		init_();
	}

	ShiftModel2D::ShiftModel2D(const ShiftModel2D& model)
		throw()
		:	ShiftModule(),
			peaks_(model.peaks_),
			origin_(model.origin_),
			dimension_(model.dimension_),
			spacing_(model.spacing_),
			type_(model.type_),
			parameters_(model.parameters_),
			system_(NULL),
			valid_(false)		
	{
		init_();
	}
	
	ShiftModel2D::~ShiftModel2D()
		throw()
	{
		clear();
	}
	
			
	void ShiftModel2D::clear()
		throw()
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
		throw()
	{
		return valid_;
	}


	bool ShiftModel2D::start()
		throw()
	{	
		peaks_.clear();
		return true;
	}
	
	bool ShiftModel2D::finish()
		throw()
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
		
		// compute the shift model
		BALL::ShiftModel sm(parameters_.getFilename());
		system_->apply(sm);

		String atomname1 = "";
		String atomname2 = "";
		
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
		float peakwidth1 = 0.0;
		float	peakwidth2 = 0.0;
		
		if (type_ == NH)
		{
			atomname1 = "H";
			peakwidth1 = 0.02142;
			atomname2 = "N";
			peakwidth2 = 0.01428;
		}
		else if (type_ == CH)
		{
			atomname1 = "HA";
			peakwidth1 = 0.02142;
			atomname2 = "C";
			peakwidth2 = 0.00714;
		}
		
		if (atomname1== ""  || atomname2== "")
			return true;
		
		for (BALL::ResidueIterator r_it = system_->beginResidue(); +r_it; ++r_it)
		{	
			Atom* atom1 = NULL;
			Atom* atom2 = NULL;

			// do we have atom1 and atom2 in this residue? 
			for (BALL::AtomIterator at_it = r_it->beginAtom(); +at_it; ++at_it)
			{
				if (at_it->getName() == atomname1)
				{
					atom1 = &(*at_it);
				}
			
				if (at_it->getName() == atomname2)
				{
					atom2 = &(*at_it);
				}
			}
			
			if (!atom1 || !atom2)
				continue;

			// we have, get the shift
			float shift1 = atom1->getProperty(BALL::ShiftModule::PROPERTY__SHIFT).getFloat();
			float shift2 = atom2->getProperty(BALL::ShiftModule::PROPERTY__SHIFT).getFloat();
			
			//store the shift in the peak list
			Peak2D<Vector2> peak;
			Vector2<float> pos(shift1, shift2); 
			peak.setPosition(pos);

			// Peter Bayer proposed as peak width  
			// for N--H 10hz/15Hz and
			// for C--H  5Hz/15Hz
			// NOTE: peakwidth is meassured in ppm, since
			// experiments were done in Hz, we convert the values 
			// according to the formular
			// 
			// 		offset [Hz] = offset[ppm] * basic frequency
			//
			// for our prediction we assume a basic frequency of 700 MHz
			peak.setWidth(Vector2(peakwidth1,peakwidth2));
			peak.setIntensity(peak.getIntensity()+1);
			//setAtom();
			peaks_.push_back(peak);
			
		}
		return true;
	}
	
	Processor::Result ShiftModel2D::operator () (Composite& composite)
			throw()
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

	void ShiftModel2D::operator >> (Spectrum2D& spectrum)
		throw()
  {
     // this overwrites the parameter
     spectrum = Spectrum2D(peaks_, origin_, dimension_, spacing_);
  }
	
		
}
