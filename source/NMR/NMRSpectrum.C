// $Id: NMRSpectrum.C,v 1.14 2001/05/07 00:56:40 oliver Exp $

#include<BALL/NMR/NMRSpectrum.h>
#include<BALL/NMR/randomCoilShiftProcessor.h>
#include<BALL/NMR/johnsonBoveyShiftProcessor.h>
#include<BALL/NMR/haighMallionShiftProcessor.h>
#include<BALL/NMR/EFShiftProcessor.h>
#include<BALL/NMR/anisotropyShiftProcessor.h>
#include<BALL/NMR/HBondShiftProcessor.h>
#include<BALL/FORMAT/PDBFile.h>
#include<BALL/KERNEL/PTE.h>
#include<BALL/COMMON/limits.h>

///////////////////////////////////////////////////////////////////////////

/* shift Module sind alle von Prozessoren abgeleitet
   NMRSpectrum verwaltet eine Liste mit Prozessoren
   verbesserung : eine von Prozessor abgeleitete gemeinsame Basisklasse 
   		  der shift Module entwerfen und die Liste darauf definieren
   		  stellt sicher das nur shift module in der Liste abgelegt 
   		  werden koennen.

   Shift Module koennen ueber Strings definiert werden.
   neue Module erforden eine neue Zeile in insert_shift_module(CBallString)
   und dementsprechung eine neu compilierung. besser waere es die Neucompilierung
   auf das neue Modulzu beschraenken.
   
   !!! korrigieren : Fehler wenn nur ein peak vorhanden : stepSize = 0 und 
	Schleife terminiert nicht  bei Ausgabe in file
*/

///////////////////////////////////////////////////////////////////////////

using namespace std;

namespace BALL
{

	typedef struct
	{
		String name;
		float shift;
	}
	name_shift;

	NMRSpectrum::NMRSpectrum()
		:	system_(0),
			density_(100),
			is_sorted_(false)
	{
	}

	NMRSpectrum::~NMRSpectrum()
	{
	}

	void NMRSpectrum::setSystem(System* s)
	{
		system_ = s;
	}

	const System* NMRSpectrum::getSystem() const
	{
		return system_;
	}

	void NMRSpectrum::calculateShifts()
	{
		system_->apply(shift_model_);
	}

	CreateSpectrumProcessor& NMRSpectrum::getCreateSpectrumProcessor()
		throw()
	{
		return create_spectrum_;
	}

	void NMRSpectrum::createSpectrum()
	{
		create_spectrum_.init();
		system_->apply(create_spectrum_);
		spectrum_ = create_spectrum_.getPeakList();
		spectrum_.sort();
	}

	const ShiftModel& NMRSpectrum::getShiftModel() const
	{
		return shift_model_;
	}

	void NMRSpectrum::setShiftModel(const ShiftModel& model)
	{
		shift_model_ = model;
	}

	const list<Peak1D>& NMRSpectrum::getPeakList() const
	{
		return spectrum_;
	}

	void NMRSpectrum::setPeakList(const list<Peak1D>& peak_list)
	{
		spectrum_ = peak_list;
		is_sorted_ = false;
	}

	float NMRSpectrum::getSpectrumMin() const
	{
		if (is_sorted_)
		{
			return spectrum_.begin()->getValue();
		}

		float min = Limits<float>::max();
		list<Peak1D>::const_iterator it = spectrum_.begin();
		while (it != spectrum_.end())
		{
			if (it->getValue() < min)
			{
				min = it->getValue();
			}
			it++;
		}
		if (min == Limits<float>::max())
		{ 
			min = 0.0;
		}

		return min;
	}

	float NMRSpectrum::getSpectrumMax() const
	{
		float max = Limits<float>::min();
		list<Peak1D>::const_iterator it = spectrum_.begin();
		while (it != spectrum_.end())
		{
			if (it->getValue() > max)
			{
				max = it->getValue();
			}
			it++;
		}
		if (max == Limits<float>::min())
		{ 
			max = 0.0;
		}

		return max;
	}

	void NMRSpectrum::sortSpectrum()
	{
		spectrum_.sort();
		is_sorted_ = true;
	}

	void NMRSpectrum::setDensity(Size density)
	{
		density_ = density;
	}

	Size NMRSpectrum::getDensity() const
	{
		return density_;
	}

	void NMRSpectrum::plotPeaks(const String& filename) const
	{
		ofstream outfile(filename.c_str (), ios::out);

		list<Peak1D>::const_iterator peak_it = spectrum_.begin();
		for (; peak_it != spectrum_.end(); ++peak_it)
		{
			outfile << peak_it->getValue() << " " << peak_it->getHeight() << " " << peak_it->getAtom()->getFullName()<< endl;
		}
	}

	void NMRSpectrum::writePeaks(const String& filename) const
	{
		float shift;
		ofstream outfile (filename.c_str(), ios::out);

		list<Peak1D>::const_iterator list_it(spectrum_.begin());
		for (; list_it != spectrum_.end(); ++list_it)
		{
			const Atom* atom_ptr = list_it->getAtom();
			if (atom_ptr != 0)
			{
				shift = atom_ptr->getProperty(ShiftModule::PROPERTY__SHIFT).getFloat();
				outfile << atom_ptr->getResidue()->getName() << atom_ptr->getResidue()->getID()
								<< ":"  << atom_ptr->getName() << " " << shift << " ";
				outfile << atom_ptr->getProperty(RandomCoilShiftProcessor::PROPERTY__RANDOM_COIL_SHIFT).getFloat() << " ";
				outfile << atom_ptr->getProperty(AnisotropyShiftProcessor::PROPERTY__ANISOTROPY_SHIFT).getFloat() << " ";
				outfile << atom_ptr->getProperty(EFShiftProcessor::PROPERTY__EF_SHIFT).getFloat() << " ";
				outfile << atom_ptr->getProperty(JohnsonBoveyShiftProcessor::PROPERTY__RING_CURRENT_SHIFT).getFloat() << " ";
				outfile << atom_ptr->getProperty(HaighMallionShiftProcessor::PROPERTY__RING_CURRENT_SHIFT).getFloat() << " ";
				outfile << atom_ptr->getProperty(HBondShiftProcessor::PROPERTY__HBOND_SHIFT).getFloat() << " " << endl;
			}
		}
	}

	void NMRSpectrum::plotSpectrum(const String& filename) const
	{
		// berechnet die peak Daten und schreibt sie in das file : filename

		ofstream outfile(filename.c_str(), ios::out);

		// Berechnung der Dichteverteilung:

	  float min = -2.0;
	  float max = 12.0;
	  float step_size = (max - min) / 5000;

		
		if (step_size <= 0.0)
		{
			Log.error() << "NMRSpectrum:plotSpectrum: spectrum has empty range. Aborted." << endl;
		}
		else 
		{
			List<Peak1D>::const_iterator peak_it;
			for (float x = min; x <= max; x += step_size)
			{
				float y = 0;
				for (peak_it = spectrum_.begin(); peak_it != spectrum_.end(); ++peak_it)
				{
					float number = peak_it->getValue() * 2 * Constants::PI  - x * 2 * Constants::PI;

					y +=	peak_it->getHeight() / (1 +  (number * number * 4 / (peak_it->getWidth() / 10.0)));
				}
				outfile << x << " " << y << endl;
			}
		}
		outfile.close();
	}

	void makeDifference(const float& diff, const String &a, const String& b, const String& out)
	{
		std::list<name_shift>						liste_b;
		std::list<name_shift>::iterator iter;

		String atom_name;
		float shift;
		name_shift *eintrag;

		ifstream infile_b (b.c_str(), ios::in);

		while (atom_name != "END");
		{
			infile_b >> atom_name;
			infile_b >> shift;
			eintrag = new name_shift;
			eintrag->name = atom_name;
			eintrag->shift = shift;
			liste_b.push_back (*eintrag);
		}

		ifstream infile_a (a.c_str(), ios::in);
		ofstream outfile (out.c_str(), ios::out);

		bool found;

		do
		{
			found = false;
			infile_a >> atom_name;
			infile_a >> shift;
			for (iter = liste_b.begin(); iter != liste_b.end(); ++iter)
			{
				if ((atom_name == (*iter).name) && ((shift - (*iter).shift < diff) && (shift - (*iter).shift > -diff)))
				{
					found = true;
					break;
				}
			}
			if (!found)
			{
				outfile << atom_name << " " << shift << endl;
			}
		}
		while (atom_name != "END");

		outfile << "END" << " " << 0.0 << endl;
	}


}	// namespace Ball
