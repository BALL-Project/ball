// $Id: NMRSpectrum.C,v 1.6 2000/09/22 13:18:04 amoll Exp $

#include<BALL/NMR/NMRSpectrum.h>
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
		:	density_(100),
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

	void NMRSpectrum::createSpectrum()
	{
		system_->apply(create_spectrum_);
		spectrum_ = create_spectrum_.getPeakList();
		spectrum_.sort();
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
		}

		return min;
	}

	float NMRSpectrum::getSpectrumMax() const
	{
		if (is_sorted_)
		{
			return spectrum_.rbegin()->getValue();
		}

		float max = Limits<float>::min();
		list<Peak1D>::const_iterator it = spectrum_.begin();
		while (it != spectrum_.end())
		{
			if (it->getValue() > max)
			{
				max = it->getValue();
			}
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

		list<Peak1D>::const_iterator peak_iter = spectrum_.begin();
		for (; peak_iter != spectrum_.end(); ++peak_iter)
		{
			outfile << peak_iter->getValue() << " " << peak_iter->getHeight() << endl;
		}
	}

	void NMRSpectrum::writePeaks(const String& filename) const
	{
		float shift;
		ofstream outfile (filename.c_str(), ios::out);

		AtomIterator atom_iter = system_->beginAtom();
		for (; atom_iter != system_->endAtom(); ++atom_iter)
		{
			shift = (*atom_iter).getProperty ("chemical_shift").getFloat();
			if (shift != 0 && shift < 100)
			{
				outfile << (*atom_iter).getFullName() << " " << shift << endl;
			}
		}
		outfile << "END" << " " << 0.0 << endl;
	}

	void NMRSpectrum::plotSpectrum(const String& filename) const
	{
		// berechnet die peak Daten und schreibt sie in das file : filename

	  list<Peak1D>::const_iterator peak_iter1;
	  list<Peak1D>::const_iterator peak_iter2;

		ofstream outfile(filename.c_str(), ios::out);

		// Berechnung der Dichteverteilung:

		float omega, ts, gs;

	  const float& min = getSpectrumMin();
	  const float& max = getSpectrumMax();

	  const float stepSize = max - min / density_;
	  float y = stepSize;
	  float value = peak_iter1->getValue();
	  float value_old = value;
	  peak_iter1 = spectrum_.begin();

		for (float x = min; x <= max; x += y)
		{
			if (x < value)
			{
				omega = x;
			}
			else
			{
				omega = value;
				++peak_iter1;
				value_old = value;
				value = (*peak_iter1).getValue();
				x -= y;
			}

			gs = 0;
			for (peak_iter2 = spectrum_.begin(); peak_iter2 != spectrum_.end(); ++peak_iter2)
			{
				const float number = peak_iter2->getValue() * 2 * Constants::PI * 1e6 - omega * 2 * Constants::PI * 1e6;

				ts =	peak_iter2->getHeight() / (1 +  (number * number * 4 / peak_iter2->getWidth()));
				gs += ts;
			}
			outfile << omega << " " << gs << endl;
			if (((x - value)     < stepSize && (x - value)     > -stepSize) || 
					((x - value_old) < stepSize && (x - value_old) > -stepSize)    )
			{
				y = stepSize / 10;
			}
			else
			{
				y = stepSize;
			}
		}
	}

	void makeDifference (const float& diff, const String &a, const String& b, const String& out)
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

	void setDifference (NMRSpectrum* a, NMRSpectrum* b, const String& outpdb, const String& out)
	{
		const System& system_a = *a->getSystem();
		const System& system_b = *b->getSystem();

	  StringHashMap<Atom*> table_b;

		AtomIterator atom_iter = system_b.beginAtom();
		for (; +atom_iter; ++atom_iter)
		{
			if ((*atom_iter).getElement() == PTE[Element::H])
			{
				table_b[(*atom_iter).getFullName()] = &(*atom_iter);
			}
		}

		PDBAtom*	patom_a;
		Atom*			atom_b;

		ofstream dfile(out.c_str(), ios::out);

		float shift_a, shift_b, difference;

		atom_iter = system_a.beginAtom();
		for (; +atom_iter; ++atom_iter)
		{
			patom_a = RTTI::castTo<PDBAtom>((*atom_iter));
			patom_a->setOccupancy(0.5);
			if ((*atom_iter).getElement() == PTE[Element::H] &&
					 table_b.has((*atom_iter).getFullName()))
			{
				atom_b = table_b[(*atom_iter).getFullName()];
				shift_a = patom_a->getProperty ("chemical_shift").getFloat();
				shift_b = atom_b->getProperty ("chemical_shift").getFloat();

				difference = shift_a - shift_b;
				if (shift_a > 100)
				{
					difference = 0.5;
				}
				patom_a->setOccupancy(patom_a->getOccupancy() + difference);
				dfile << (*atom_iter).getFullName() << " " << shift_a << " " << difference << endl;
			}
		}

		PDBFile outfile(outpdb, ios::out);
		//outfile.open;
		outfile << system_a;
		outfile.close();
	}

}	// namespace Ball
