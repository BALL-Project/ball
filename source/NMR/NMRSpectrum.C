// $Id: NMRSpectrum.C,v 1.2 2000/08/28 16:08:52 oliver Exp $

#include<BALL/NMR/NMRSpectrum.h>

///////////////////////////////////////////////////////////////////////////

/* shift Module sind alle von Prozessoren abgeleitet
   CBallNMRSpectrum verwaltet eine Liste mit Prozessoren
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

	  NMRSpectrum::NMRSpectrum ()
	{
		create_spectrum_ = new CreateSpectrumProcessor;
		dichte_ = 100;
	}

	NMRSpectrum::~NMRSpectrum ()
	{
	}

	void NMRSpectrum::set_system (System * s)
	{
		system_ = s;
	}

	System *NMRSpectrum::get_system ()
	{
		return system_;
	}

	void NMRSpectrum::insert_shift_module (String module_name)
	{
		processorlist_.push_back (names_->get_module (module_name));
	}

	void NMRSpectrum::calculate_shifts ()
	{
		list < ShiftModule * >::iterator process_iter;

		for (process_iter = processorlist_.begin (); process_iter != processorlist_.end (); ++process_iter)
		{
			//cout <<endl;
			system_->apply (*(*process_iter));
		}
	}

	void NMRSpectrum::create_spectrum ()
	{
		system_->apply (*create_spectrum_);

		spectrum_ = create_spectrum_->get_peaklist ();

		spectrum_->sort ();

	}

	list < peak > *NMRSpectrum::get_spectrum ()
	{
		return spectrum_;
	}

	void NMRSpectrum::set_spectrum (list < peak > *peak_liste)
	{
		spectrum_ = peak_liste;
	}

	float NMRSpectrum::get_spectrum_min ()
	{
		list < peak >::iterator peak_iter;
		peak_iter = spectrum_->begin ();
		return ((*peak_iter).get_ppm ());
	}

	float NMRSpectrum::get_spectrum_max ()
	{
		list < peak >::iterator peak_iter;
		peak_iter = spectrum_->end ();
		return ((*peak_iter).get_ppm ());
	}

	void NMRSpectrum::sort_spectrum ()
	{
		spectrum_->sort ();
	}

	void NMRSpectrum::set_dichte (int d)
	{
		dichte_ = d;
	}

	int NMRSpectrum::get_dichte ()
	{
		return dichte_;
	}

	void NMRSpectrum::plot_peaks (String file_name)
	{
		list < peak >::iterator peak_iter;

		ofstream outfile (file_name.c_str (), ios::out);

		for (peak_iter = spectrum_->begin (); peak_iter != spectrum_->end (); ++peak_iter)
		{
			//cout << (*peak_iter).get_ppm() <<" "<< (*peak_iter).get_height() << endl;
			outfile << (*peak_iter).get_ppm () << " " << (*peak_iter).get_height () << endl;
		}

	}

	void NMRSpectrum::write_peaks (String file_name)
	{
		float shift;
		String atom_name;
		AtomIterator atom_iter;
		FragmentIterator fragment_iter;
		MoleculeIterator molecule_iter;

		ofstream outfile (file_name.c_str (), ios::out);

		for (atom_iter = system_->beginAtom (); atom_iter != system_->endAtom (); ++atom_iter)
		{
			atom_name = (*atom_iter).getFullName ();
			shift = (*atom_iter).getProperty ("chemical_shift").getFloat ();
			if (shift != 0 && shift < 100)
			{
				outfile << atom_name << " " << shift << endl;
			}
		}
		outfile << "END" << " " << 0.0 << endl;
	}

	void NMRSpectrum::plot_spectrum (String file_name)
	{
		// berechnet die peak Daten und schreibt sie in das file : file_name

		float omega, ts, gs, height, breadth, ppm;

		  list < peak >::iterator peak_iter1;
		  list < peak >::iterator peak_iter2;

		ofstream outfile (file_name.c_str (), ios::out);

		// neu jetzt wird eine  Dichteverteilung berechnet :

		float diff, min, max, schrittweite, x, y, weite, wert, wert_old;

		  min = (*(spectrum_->begin ())).get_ppm ();
		  max = (*(--spectrum_->end ())).get_ppm ();
		  diff = max - min;
		  schrittweite = diff / dichte_;
		  y = schrittweite;
		  weite = schrittweite;

		  peak_iter1 = spectrum_->begin ();
		  wert = (*peak_iter1).get_ppm ();
		  wert_old = wert;

		for (x = min; x <= max; x += y)
		{
			if (x < wert)
			{
				//cout << "     x<=";
				omega = x;
			}
			else
			{
				//cout << "    x>=";
				omega = wert;
				++peak_iter1;
				wert_old = wert;
				wert = (*peak_iter1).get_ppm ();
				x -= y;
			}

			//cout <<endl<<"aktuelle Schrittweite:"<<y<<" omega:"<<omega;

			ts = 0;
			gs = 0;
			for (peak_iter2 = spectrum_->begin (); peak_iter2 != spectrum_->end (); ++peak_iter2)
			{
				height = (*peak_iter2).get_height ();
				breadth = (*peak_iter2).get_breadth ();
				ppm = (*peak_iter2).get_ppm ();
				ts =
					height / (1 +
										((ppm * 2 * Constants::PI * 1e6 - omega * 2 * Constants::PI * 1e6) *
										 (ppm * 2 * Constants::PI * 1e6 - omega * 2 * Constants::PI * 1e6)) * 4 / breadth);
				gs += ts;
				//cout <<omega<<endl<<height<<endl<<breadth<<endl<<ppm<<endl<<ts<<endl<<gs<<endl<<endl;

			}
			outfile << omega << " " << gs << endl;	//<<" "<< y << " "<<(x-wert)<<" "<<weite<<endl;
			if ((((x - wert) < weite) && ((x - wert) > -weite)) || (((x - wert_old) < weite) && ((x - wert_old) > -weite)))
			{
				y = schrittweite / 10;
				//cout <<"  geteilt";
			}
			else
			{
				y = schrittweite;
				//cout <<"  normal";
			}
		}

		//cout << endl << min <<" " << max <<" "<< diff <<" "<< anzahl;
		//cout <<" "<< schrittweite <<" "<< weite;

	}

	void make_difference (float diff, String a, String b, String out)
	{


		list < name_shift > liste_b;
		list < name_shift >::iterator iter;

		String atom_name;
		float shift;
		int found = 0;
		name_shift *eintrag;

		ifstream infile_b (b.c_str (), ios::in);

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

		ifstream infile_a (a.c_str (), ios::in);
		ofstream outfile (out.c_str (), ios::out);

		do
		{
			found = 0;
			infile_a >> atom_name;
			infile_a >> shift;
			for (iter = liste_b.begin (); iter != liste_b.end (); ++iter)
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

	void set_difference (NMRSpectrum * a, NMRSpectrum * b, String outpdb, String out)
	{

		PDBFile outfile;

		float shift_a, shift_b, difference, occ;

		System *system_a, *system_b;

		  system_a = a->get_system ();
		  system_b = b->get_system ();

		AtomIterator atom_iter_a, atom_iter_b;

		PDBAtom *patom_a;
		Atom *atom_b;

		  StringHashMap < Atom * >table_b;
		String name;

		ofstream dfile (out.c_str (), ios::out);

		for (atom_iter_b = system_b->beginAtom (); +atom_iter_b; ++atom_iter_b)
		{
			if ((*atom_iter_b).getElement () == PTE[Element::H])
			{
				name = (*atom_iter_b).getFullName ();
				table_b[name] = &(*atom_iter_b);
			}
		}


		for (atom_iter_a = system_a->beginAtom (); +atom_iter_a; ++atom_iter_a)
		{
			patom_a = RTTI::castTo < PDBAtom > ((*atom_iter_a));
			patom_a->setOccupancy (0.5);
			if ((*atom_iter_a).getElement () == PTE[Element::H])
			{
				if (table_b.has ((*atom_iter_a).getFullName ()))
				{
					atom_b = table_b[(*atom_iter_a).getFullName ()];
					shift_a = patom_a->getProperty ("chemical_shift").getFloat ();
					shift_b = atom_b->getProperty ("chemical_shift").getFloat ();
					difference = shift_a - shift_b;
					if (shift_a > 100)
						difference = 0.5;
					occ = patom_a->getOccupancy ();
					occ += difference;
					patom_a->setOccupancy (occ);
					dfile << (*atom_iter_a).getFullName () << " " << shift_a << " " << difference << endl;
				}
			}
		}

		outfile.open (outpdb, ios::out);
		outfile << (*system_a);
		outfile.close ();
	}



}	// namespace Ball
