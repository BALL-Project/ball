// $Id: createSpectrumProcessor.C,v 1.1 2000/07/04 09:33:14 oliver Exp $

#include<BALL/NMR/createSpectrumProcessor.h>

using namespace std;

namespace BALL {

CreateSpectrumProcessor::CreateSpectrumProcessor()
	{
	raster_=0.01e-6;
	}
	
CreateSpectrumProcessor::~CreateSpectrumProcessor()
	{
	}
	
bool CreateSpectrumProcessor::start()
	{
	//cout <<endl<<"CreateSpectrumProcessor::start()";
	return 1;
	}
	
bool CreateSpectrumProcessor::finish()
	{
	//cout <<endl<<"CreateSpectrumProcessor::finish()";	
	return 1;
	}
	
Processor::Result CreateSpectrumProcessor::operator()(Object& object)
	{
	//cout <<endl<<"CreateSpectrumProcessor::operator()";
	
	// Sammelt die Hydrogens in den Peaks
	
	peak *neu;
		
	if (RTTI::isKindOf<PDBAtom>(object))
		{
		patom_=RTTI::castTo<PDBAtom>(object);
		
		if (patom_->getElement()==PTE[Element::H])
			{
			neu=new peak;
			neu->add(patom_);
			peaklist_.push_back(*neu);
			}
		}	
	return Processor::CONTINUE;
	}

list<peak>* CreateSpectrumProcessor::get_peaklist()
	{
	return &peaklist_;
	}

float CreateSpectrumProcessor::get_raster()
	{
	return raster_;
	}
	
void CreateSpectrumProcessor::set_raster(float wert)
	{
	raster_=wert;
	}
} // namespace Ball
			
