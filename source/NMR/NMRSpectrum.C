// $Id: NMRSpectrum.C,v 1.5 2000/09/21 07:43:42 oliver Exp $

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
   

   !!! korrigieren : Fehler wenn nur ein peak vorhanden : Schrittweite = 0 und 
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
		:	density_(100)
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
	}

	float NMRSpectrum::getSpectrumMin() const
	{
		// BAUSTELLE: additional member should store information
		// whether the spectrum is sorted. If it is sorted,
		// this can be done in O(1)!

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
		// BAUSTELLE: additional member should store information
		// whether the spectrum is sorted. If it is sorted,
		// this can be done in O(1)!

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

	void NMRSpectrum::sortSpectrum ()
	{
		spectrum_.sort();
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
		list<Peak1D>::const_iterator peak_iter;

		ofstream outfile(filename.c_str (), ios::out);

		for (peak_iter = spectrum_.begin(); peak_iter != spectrum_.end(); ++peak_iter)
		{
			outfile << peak_iter->getValue() << " " << peak_iter->getHeight() << endl;
		}
	}

	void NMRSpectrum::writePeaks(const String& filename) const
	{
		float shift;
		String atom_name;
		AtomIterator atom_iter;
		FragmentIterator fragment_iter;
		MoleculeIterator molecule_iter;

		ofstream outfile (filename.c_str(), ios::out);

		for (atom_iter = system_->beginAtom(); atom_iter != system_->endAtom(); ++atom_iter)
		{
			atom_name = (*atom_iter).getFullName();
			shift = (*atom_iter).getProperty ("chemical_shift").getFloat();
			if (shift != 0 && shift < 100)
			{
				outfile << atom_name << " " << shift << endl;
			}
		}
		outfile << "END" << " " << 0.0 << endl;
	}

	void NMRSpectrum::plotSpectrum(const String& filename) const
	{
		// berechnet die peak Daten und schreibt sie in das file : filename

		float omega, ts, gs, height, width, ppm;

	  list<Peak1D>::const_iterator peak_iter1;
	  list<Peak1D>::const_iterator peak_iter2;

		ofstream outfile(filename.c_str(), ios::out);

		// neu jetzt wird eine  Dichteverteilung berechnet :

		float diff, min, max, schrittweite, x, y, weite, value, value_old;

	  min = getSpectrumMin();
	  max = getSpectrumMax();
	  diff = max - min;
	  schrittweite = diff / density_;
	  y = schrittweite;
	  weite = schrittweite;

	  peak_iter1 = spectrum_.begin();
	  value = peak_iter1->getValue();
	  value_old = value;

		for (x = min; x <= max; x += y)
		{
			if (x < value)
			{
				//cout << "     x<=";
				omega = x;
			}
			else
			{
				//cout << "    x>=";
				omega = value;
				++peak_iter1;
				value_old = value;
				value = (*peak_iter1).getValue();
				x -= y;
			}

			ts = 0;
			gs = 0;
			for (peak_iter2 = spectrum_.begin(); peak_iter2 != spectrum_.end(); ++peak_iter2)
			{
				height = peak_iter2->getHeight();
				width = peak_iter2->getWidth();
				ppm = peak_iter2->getValue();
				ts =
					height / (1 +
										((ppm * 2 * Constants::PI * 1e6 - omega * 2 * Constants::PI * 1e6) *
										 (ppm * 2 * Constants::PI * 1e6 - omega * 2 * Constants::PI * 1e6)) * 4 / width);
				gs += ts;
			}
			outfile << omega << " " << gs << endl;	//<<" "<< y << " "<<(x-value)<<" "<<weite<<endl;
			if ((((x - value) < weite) && ((x - value) > -weite)) || (((x - value_old) < weite) && ((x - value_old) > -weite)))
			{
				y = schrittweite / 10;
			}
			else
			{
				y = schrittweite;
			}
		}
	}

	void makeDifference (float diff, String a, String b, String out)
	{


		list<name_shift> liste_b;
		list<name_shift>::iterator iter;

		String atom_name;
		float shift;
		int found = 0;
		name_shift *eintrag;

		ifstream infile_b (b.c_str(), ios::in);

		do
		{
			infile_b >> atom_name;
			infile_b >> shift;
			eintrag = new name_shift;
			eintrag->name = atom_name;
			eintrag->shift = shift;
			liste_b.push_back (*eintrag);
		}
		while (atom_name != "END");

		ifstream infile_a (a.c_str(), ios::in);
		ofstream outfile (out.c_str(), ios::out);

		do
		{
			found = 0;
			infile_a >> atom_name;
			infile_a >> shift;
			for (iter = liste_b.begin(); iter != liste_b.end(); ++iter)
			{
				if ((atom_name == (*iter).name) && ((shift - (*iter).shift < diff) && (shift - (*iter).shift > -diff)))
					found = 1;
			}
			if (!found)
			{
				outfile << atom_name << " " << shift << endl;
			}
		}
		while (atom_name != "END");

		outfile << "END" << " " << 0.0 << endl;

	}

	void setDifference (NMRSpectrum * a, NMRSpectrum * b, String outpdb, String out)
	{

		PDBFile outfile;

		float shift_a, shift_b, difference, occ;

		const System* system_a;
		const System* system_b;

	  system_a = a->getSystem();
	  system_b = b->getSystem();

		AtomIterator atom_iter_a, atom_iter_b;

		PDBAtom *patom_a;
		Atom *atom_b;

	  StringHashMap<Atom*> table_b;
		String name;

		ofstream dfile(out.c_str(), ios::out);

		for (atom_iter_b = system_b->beginAtom(); +atom_iter_b; ++atom_iter_b)
		{
			if ((*atom_iter_b).getElement() == PTE[Element::H])
			{
				name = (*atom_iter_b).getFullName();
				table_b[name] = &(*atom_iter_b);
			}
		}


		for (atom_iter_a = system_a->beginAtom(); +atom_iter_a; ++atom_iter_a)
		{
			patom_a = RTTI::castTo < PDBAtom > ((*atom_iter_a));
			patom_a->setOccupancy (0.5);
			if ((*atom_iter_a).getElement() == PTE[Element::H])
			{
				if (table_b.has ((*atom_iter_a).getFullName()))
				{
					atom_b = table_b[(*atom_iter_a).getFullName()];
					shift_a = patom_a->getProperty ("chemical_shift").getFloat();
					shift_b = atom_b->getProperty ("chemical_shift").getFloat();
					difference = shift_a - shift_b;
					if (shift_a > 100)
						difference = 0.5;
					occ = patom_a->getOccupancy();
					occ += difference;
					patom_a->setOccupancy (occ);
					dfile << (*atom_iter_a).getFullName() << " " << shift_a << " " << difference << endl;
				}
			}
		}

		outfile.open (outpdb, ios::out);
		outfile << (*system_a);
		outfile.close();
	}



}	// namespace Ball
