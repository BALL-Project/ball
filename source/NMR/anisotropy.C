// $Id: anisotropy.C,v 1.4 2000/08/28 16:08:52 oliver Exp $

#include<BALL/NMR/anisotropy.h>

///////////////////////////////////////////////////////////////////////////////
// noch zu erledigen :
// 
///////////////////////////////////////////////////////////////////////////////
using namespace std;

namespace BALL
{

//Konstruktor

	AnIsoShift::AnIsoShift ()
	{
		//cout << endl << "AnIsoShift::AnIsoShift()";
	}


//Destruktor

	AnIsoShift::~AnIsoShift ()
	{
		//cout << endl << "AnIsoShift::~AnIsoShift()";
	}


//StartFunktion

	bool AnIsoShift::start ()
	{
		// hier passiert nichts
		//cout << endl << "AnIsoShift::start()";
		return 1;
	}


//FinishFunktion

	bool AnIsoShift::finish ()
	{
		//cout << endl << "Aniso Modul";

		int anzahl, zaehler;
		float stheta, sgamma, abstand, ts, gs, calc1, calc2;
		Vector3 vz, vx, vy, hilf, c_pos, o_pos, x_pos, n_pos, cen, v1, v2, v3;
		const float dX1 = -13.0;
		const float dX2 = -4.0;
		const float dXN1 = -11.0;
		const float dXN2 = -5.0;
		const float ndX1 = -11;
		const float ndX2 = 1.4;
		const float ndXN1 = -7.0;
		const float ndXN2 = 1.0;

		String name;
		Atom *c_atom, *o_atom, *x_atom, *n_atom;
		  list < PDBAtom * >::iterator proton_iter;
		  list < Bond * >::iterator eff_iter;
		Bond *hbond;

		if (proton_list_.size () > 0)
		{
			// Iteriere über alle Protonen in proton_list_

			for (proton_iter = proton_list_.begin (); proton_iter != proton_list_.end (); ++proton_iter)
			{
				gs = 0;

				for (eff_iter = eff_list_.begin (); eff_iter != eff_list_.end (); ++eff_iter)
				{
					// Für jedes Proton iteriere über alle Effektorbindungen in eff_list_
					//cout << endl << " Eff-Bindung wird bearbeitet : ";        

					patom_ = *proton_iter;
					bond_ = *eff_iter;
					c_atom = (bond_->getFirstAtom ());
					o_atom = (bond_->getSecondAtom ());

					if ((*proton_iter)->getResidue () != c_atom->getFragment ())
					{
						name = c_atom->getName ();
						if (name == "C")
							name = "CA";
						else if (name == "CG")
							name = "CB";
						else if (name == "CD")
							name = "CG";

						anzahl = c_atom->countBonds ();
						for (zaehler = 0; zaehler < anzahl; zaehler++)
						{
							hbond = c_atom->getBond (zaehler);
							if ((hbond->getBoundAtom (*c_atom)->getName ()) == name)
								x_atom = hbond->getBoundAtom (*c_atom);
						}

						c_pos = c_atom->getPosition ();
						o_pos = o_atom->getPosition ();
						x_pos = x_atom->getPosition ();
						// baue rechtwinkliges Koordinatensystem auf :

						vz = o_pos - c_pos;
						vz /= vz.getLength ();
						hilf = x_pos - c_pos;
						vy = vz % hilf;
						vy /= vy.getLength ();
						vx = vz % vy;
						vx /= vx.getLength ();
						cen = c_pos + (vz * 1.1);
						v1 = patom_->getPosition () - cen;
						v2 = v1 % vy;
						v3 = v2 % vx;

						abstand = v1.getLength ();
						stheta = v2.getLength () / (v1.getLength () * vy.getLength ());
						sgamma = v3.getLength () / (v2.getLength () * vx.getLength ());

						ts = 0;
						if ((*proton_iter)->getName () == "H")
						{
							calc1 = dXN1 * ((3.0 * stheta * stheta) - 2.0);
							calc2 = dXN2 * (1.0 - (3.0 * stheta * stheta * sgamma * sgamma));
						}
						else
						{
							calc1 = dX1 * ((3.0 * stheta * stheta) - 2.0);
							calc2 = dX2 * (1.0 - (3.0 * stheta * stheta * sgamma * sgamma));
						}
						ts = (calc1 + calc2) / (3.0 * abstand * abstand * abstand);
						gs += ts;
					}
				}

				for (eff_iter = eff_list_2_.begin (); eff_iter != eff_list_2_.end (); ++eff_iter)
				{
					patom_ = *proton_iter;
					bond_ = *eff_iter;
					c_atom = bond_->getFirstAtom ();
					n_atom = bond_->getSecondAtom ();

					if (!(((*proton_iter)->getName () == "H") && ((*proton_iter)->getResidue () == n_atom->getFragment ())))
					{
						anzahl = c_atom->countBonds ();
						for (zaehler = 0; zaehler < anzahl; zaehler++)
						{
							hbond = c_atom->getBond (zaehler);
							if ((hbond->getBoundAtom (*c_atom)->getName ()) == "O")
								o_atom = hbond->getBoundAtom (*c_atom);
						}
						c_pos = c_atom->getPosition ();
						o_pos = o_atom->getPosition ();
						n_pos = n_atom->getPosition ();

						// baue rechtwinkliges Koordinatensystem auf
						vz = n_pos - c_pos;
						abstand = vz.getLength ();
						vz /= abstand;
						hilf = o_pos - c_pos;
						vy = vz % hilf;
						vy /= vy.getLength ();
						vx = vz % vy;
						vx /= vx.getLength ();
						cen = c_pos + (vz * (0.85 * abstand));
						v1 = patom_->getPosition () - cen;
						v2 = v1 % vy;
						v3 = v2 % vx;

						abstand = v1.getLength ();
						stheta = v2.getLength () / (v1.getLength () * vy.getLength ());
						sgamma = v3.getLength () / (v2.getLength () * vx.getLength ());

						ts = 0;
						if ((*proton_iter)->getName () == "H")
						{
							calc1 = ndXN1 * ((3.0 * stheta * stheta) - 2.0);
							calc2 = ndXN2 * (1.0 - (3.0 * stheta * stheta * sgamma * sgamma));
						}
						else
						{
							calc1 = ndX1 * ((3.0 * stheta * stheta) - 2.0);
							calc2 = ndX2 * (1.0 - (3.0 * stheta * stheta * sgamma * sgamma));
						}

						ts = (calc1 + calc2) / (3.0 * abstand * abstand * abstand);
						gs += ts;
					}
				}
				shift_ = (*proton_iter)->getProperty ("chemical_shift").getFloat ();
				shift_ += gs;
				(*proton_iter)->setProperty ("chemical_shift", shift_);
				//Log.level(LogStream::INFORMATION)  << "chemical_shift :" << shift_ << endl;
				//cout << endl <<"AnISO:Residue :"<<(*proton_iter)->getResidue()->getName()<<" Atom :"<<(*proton_iter)->getName()<< " chemical_shift :" << shift_;
			}
		}
		return 1;
	}


//apply Funktion

	Processor::Result AnIsoShift::operator ()(Object & object)
	{
		// hier werden alle Effektorbindungen gesammelt( C=O ) und in eff_list_ gespeichert.
		// hier werden alle Wasserstoffe in proton_list_ gespeichert.

		int anzahl, zaehler, n, o, n_zahl;
		String residue_name;

		if (RTTI::isKindOf < PDBAtom > (object))
		{
			//cout  << endl << "Object is ProteinAtom";

			patom_ = RTTI::castTo < PDBAtom > (object);
			//cout  << endl << "    atom name : " << patom_->getName();
			//cout  << endl << "    Element : " << (patom_->getElement()).getName();

			// suche im Backbone

			if (patom_->getName () == "C")
			{
				if (patom_->isBound ())
				{
					n = 0;
					o = 0;
					anzahl = patom_->countBonds ();
					//cout << endl << "anzahl :" << anzahl;

					// laufe alle Bindungen des Atoms durch und suche nach Sauerstoff-
					// Doppelbindung

					for (zaehler = 0; zaehler < anzahl; zaehler++)
					{
						bond_ = patom_->getBond (zaehler);
						if ((bond_->getBoundAtom (*patom_)->getName ()) == "N")
						{
							n = 1;
							n_zahl = zaehler;
						}
						if ((bond_->getBoundAtom (*patom_)->getName ()) == "O")
						{
							o = 1;
							eff_list_.push_back (bond_);
							//cout  << "... Bindung in list 1 eingefügt.";
						}
					}
					if (n * o)
					{
						bond_ = patom_->getBond (n_zahl);
						eff_list_2_.push_back (bond_);
						//cout << " ... Bindung auch in list 2 eingefügt.";
					}
				}
			}


			// suche in der Seitenkette , nur ASP ASN GLU GLN

			if (patom_->getElement () == PTE[Element::C])
			{
				residue_name = patom_->getResidue ()->getName ();
				if ((residue_name == "ASP") || (residue_name == "ASN"))
				{
					if (patom_->getName () == "CG")
					{
						if (patom_->isBound ())
						{
							anzahl = patom_->countBonds ();
							//cout << endl << "anzahl :" << anzahl;

							// laufe alle Bindungen des Atoms durch und suche nach Sauerstoff-
							// Doppelbindung

							for (zaehler = 0; zaehler < anzahl; zaehler++)
							{
								bond_ = patom_->getBond (zaehler);
								if ((bond_->getBoundAtom (*patom_)->getElement ()) == PTE[Element::O])
								{
									if ((bond_->getBoundAtom (*patom_)->getName ()) == "OD1")
										eff_list_.push_back (bond_);
									//cout  << "... Bindung eingefügt.";
								}
							}

						}
					}
				}

				if ((residue_name == "GLU") || (residue_name == "GLN"))
				{
					if (patom_->getName () == "CD")
					{
						if (patom_->isBound ())
						{
							anzahl = patom_->countBonds ();
							//cout << endl << "anzahl :" << anzahl;

							// laufe alle Bindungen des Atoms durch und suche nach Sauerstoff-
							// Doppelbindung

							for (zaehler = 0; zaehler < anzahl; zaehler++)
							{
								bond_ = patom_->getBond (zaehler);
								if ((bond_->getBoundAtom (*patom_)->getElement ()) == PTE[Element::O])
								{
									if ((bond_->getBoundAtom (*patom_)->getName ()) == "OE1")
										eff_list_.push_back (bond_);
									//cout  << "... Bindung eingefügt.";
								}
							}

						}
					}
				}
			}



			if (patom_->getElement () == PTE[Element::H])
			{
				// Element ist Hydrogen
				proton_list_.push_back (patom_);
				//cout << "...eingefügt.";
			}
		}	// Ende is kind of ProteinAtom : die proton_list_ wurde um protonen erweitert.
		//               eff_list_ wurde um Effektorbindungen erweitert.  

		return Processor::CONTINUE;
	}

}	// namespace Ball
