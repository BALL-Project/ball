// $Id: haighMallionShiftProcessor.C,v 1.4 2000/09/21 09:46:03 oliver Exp $

#include <BALL/NMR/haighMallionShiftProcessor.h>
#include <BALL/KERNEL/atomIterator.h>
#include <BALL/KERNEL/PTE.h>

namespace BALL 
{

	const float HaighMallionShiftProcessor::big_loop_radius_		= 1.39e-10;
	const float HaighMallionShiftProcessor::little_loop_radius_ = 1.182e-10;
	const float HaighMallionShiftProcessor::B_ = 5.455e-6;					//Konstante in IBG(r)

	float getLambda(const Vector3& v1, const Vector3& v2)
	{
		if (v2.x==0)
		{
			if (v2.y==0)
			{
				if (v2.z==0)
				{
					return 0;
				}
				else
				{
					return (v1.z/v2.z);
				}
			}
			else
			{
				return (v1.y/v2.y);
			}
		}
		else
		{
			return (v1.x/v2.x);
		}
	}
		
	HaighMallionShiftProcessor::HaighMallionShiftProcessor()
		throw()
	{
		asrings_ = new String*[4];
		
		asrings_[0]=new String[13];
		
		asrings_[0][0]="TRP";
		asrings_[0][1]="CG";
		asrings_[0][2]="CD2";
		asrings_[0][3]="CE2";
		asrings_[0][4]="NE1";
		asrings_[0][5]="CD1";
		asrings_[0][6]="NULL";
		asrings_[0][7]="CD2";
		asrings_[0][8]="CZ2";
		asrings_[0][9]="CZ3";
		asrings_[0][10]="CE3";
		asrings_[0][11]="CH2";
		asrings_[0][12]="CE2";
		
		asrings_[1]=new String[7];
		
		asrings_[1][0]="PHE";
		asrings_[1][1]="CG";
		asrings_[1][2]="CD2";
		asrings_[1][3]="CE2";
		asrings_[1][4]="CZ";
		asrings_[1][5]="CE1";
		asrings_[1][6]="CD1";
		
		asrings_[2]=new String[7];
		
		asrings_[2][0]="TYR";
		asrings_[2][1]="CG";
		asrings_[2][2]="CD2";
		asrings_[2][3]="CE2";
		asrings_[2][4]="CZ";
		asrings_[2][5]="CE1";
		asrings_[2][6]="CD1";
		
		asrings_[3]=new String[7];
		
		asrings_[3][0]="HIS";
		asrings_[3][1]="CB";
		asrings_[3][2]="CD2";
		asrings_[3][3]="NE2";
		asrings_[3][4]="CE1";
		asrings_[3][5]="ND1";
		asrings_[3][6]="NULL";
	}

		
	HaighMallionShiftProcessor::~HaighMallionShiftProcessor()
		throw()
	{
	}		

	bool HaighMallionShiftProcessor::finish()
		throw()
	{
		// Berechnung des shifts fuer jedes Proton der liste _proton_list
				
		// hshift bezeichnet den chemicalshift der durch den gerade abgearbeiteten Ring verursacht wird.
			
		// iteriere ueber alle Protonen
		// fuer jedes Proton iteriere ueber alle Ringe und berechne chemical_shift
		
		Vector3* vectorField = new Vector3[6];

		Size zaehler, numberOfRings;
		float radius, shift;

		for (list<PDBAtom*>::iterator proton_iter = proton_list_.begin();
				 proton_iter != proton_list_.end(); ++proton_iter)
		{
			for(list<Residue*>::iterator arom_iter = aromat_list_.begin();
					arom_iter != aromat_list_.end(); ++arom_iter)
			{
				const Residue& residue = *(*arom_iter);
				if ((*proton_iter)->getResidue() == &residue)
				{
					continue;
				}

				if (residue.getName()=="TRP")
				{
					zaehler = 0;
					numberOfRings = 2;
					radius = big_loop_radius_;
				}
				if (residue.getName()=="PHE")
				{
					zaehler = 1;
					numberOfRings = 1;
					radius = big_loop_radius_;
				}
				if (residue.getName()=="TYR")
				{
					zaehler = 2; 
					numberOfRings = 1; 
					radius = little_loop_radius_;
				}
				if (residue.getName()=="HIS")
				{
					zaehler = 3; 
					numberOfRings = 1; 
					radius = little_loop_radius_;
				}

				Position vzaehler = 0;
				Position hilf = 0;
				while(numberOfRings) // for all rings in the amino acid
				{
					//Aufbau von _vector_feld
					for(Position pos = hilf; pos < hilf + 6; pos++ )
					{
						if (asrings_[zaehler][1 + pos] == "NULL")
						{
							break;
						}

						for(AtomIterator atom_iter = residue.beginAtom();
								+ atom_iter;
								++ atom_iter)
						{
							if(asrings_[zaehler][1 + pos] == (*atom_iter).getName())
							{
								vectorField[vzaehler] = (*atom_iter).getPosition();
								vzaehler++ ;
								break;	// gefunden
							}	
						}
					}
				
					// _vector_feld ist bestimmt und vzaehler zeigt hinter letzten gueltigen vector
				
					// bestimme den Mittelpunkt
				
					Vector3 center;
					
					for (Position pos = 0; pos < vzaehler; pos++)
					{
						center += vectorField[pos];
					}
					center /= vzaehler;
				
					// bestimme den NormalenVektor der Ringebene
				
					Vector3 normal;
					
					for (Position pos = 0; pos < vzaehler; pos++)
					{
						const Vector3& links  = vectorField[(pos + 0) % (vzaehler)];
						const Vector3& mitte  = vectorField[(pos + 1) % (vzaehler)];
						const Vector3& rechts = vectorField[(pos + 2) % (vzaehler)];
						normal += (mitte - links) % (mitte - rechts);  
					}
				
					normal /= vzaehler; // Normalenvektor wurde gemittelt
					normal.normalize(); //Normalenvektor ist jetzt NormalenEinheitsvektor
				
					// Die Ringebene wurde ermittelt

					// berechne den Flaecheninhalt des Rings
					const Vector3 ein_vector(vectorField[5] - vectorField[1]);
					Vector3 v(vectorField[1] - vectorField[2]);
					float ringSurface = (vectorField[0] - vectorField[1]) * (vectorField[5] - vectorField[1]);
					ringSurface += ein_vector.getLength() * v.getLength();
				
					// bestimme den chemicalshift des Protons durch diesen Ring
					v = (*proton_iter)->getPosition();
				
					// berechne Projektion des Protons auf die Ringebene
					float a = normal * v-normal * center;
					Vector3 v_ring;

					if (a < 0) 
					{
						v_ring = v - a * normal;
					}
					else 
					{
						v_ring= v + a * normal;
					}
				
					// Schleife ueber alle Bindungen des Rings
					float sum = 0;
					for (Position pos = 0; pos < vzaehler; pos++ )
					{
						const Vector3& eins = vectorField[(pos + 0) % (vzaehler)];
						const Vector3& zwei = vectorField[(pos + 1) % (vzaehler)];
					
						// berechne Abstand von Proton zu eins und zwei
						Vector3 ein_vector = v - eins;
						float a_eins = (ein_vector.getLength() / radius) * 1e-10;
						ein_vector = v - zwei;
						float a_zwei = (ein_vector.getLength() / radius) * 1e-10;
					
						// berechne Flaecheninhalt des 3Ecks aus eins,zwei,v_ring
						Vector3 eins_zwei(eins - zwei);
					
						Vector3 n0 = normal % eins_zwei;
						n0.normalize();
					
						float h = (n0 * v_ring  -  n0 * zwei);
						if (h < 0)
						{
							h = -h;
						}
					
						float f = 0.5 * h * eins_zwei.getLength() / ringSurface;
					
						// bestimme das Vorzeichen der Dreiecksflaeche
						if (getLambda((v_ring - eins) % (v_ring - zwei), normal) > 0)
						{
							f *= -1; 
						}
					
						// addiere gesamtsumme	
						sum += f *( (1 / (a_eins * a_eins * a_eins)) + (1 / (a_zwei * a_zwei * a_zwei)) );
					}

					shift += sum * B_; // zaehle aktuellen chemicalshift zum Gesamtwert dazu
					
					vzaehler = 0;
					numberOfRings--;
					hilf = 6; // fuer den naechsten schleifendurchlauf
				}
			}
			
			// Setze Property chemicalshift des gerade bearbeiteten Protons auf den entsprechenden Wert.
			float hshift = shift * 1e6;
			shift = (*proton_iter)->getProperty("chemical_shift").getFloat();
			shift += hshift;
			(*proton_iter)->setProperty("chemical_shift", shift);
			(*proton_iter)->setProperty("HM", hshift);			
		}
				
		return true;
	}
		
	Processor::Result HaighMallionShiftProcessor::operator () (Composite& object)
		throw()
	{
		// Arbeitet als Kollektor :
		// - Alle Ringe werden in _aromat_list gespeichert
		// - Alle Protonen werden in _protonlist gespeichert
		// - die Berechnung erfolgt dann in der Finish Prozedur
		
		// erganze _aromat_list um aromatische Residues
		if ( RTTI::isKindOf<Residue>(object) ) 
		{
			Residue* residue = RTTI::castTo<Residue>(object);
			for(Position pos = 0; pos < 4; pos++ )
			{
				if (asrings_[pos][0] == residue->getName())
				{
					aromat_list_.push_back(residue);
					break; // gefunden und schleife hinterher beenden
				}
			}		
			return Processor::CONTINUE;
		} 

		// erganze proton_list_	um H
		if (RTTI::isKindOf<PDBAtom>(object))
		{
			PDBAtom* atom_ptr = RTTI::castTo<PDBAtom>(object);
			if (atom_ptr->getElement() == PTE[Element::H])
			{
				proton_list_.push_back(atom_ptr);
			}
		}
		
		return Processor::CONTINUE;
	}

} // namespace BALL
