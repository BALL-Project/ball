// $Id: haighMallionShiftProcessor.C,v 1.6 2000/09/24 13:24:32 oliver Exp $

#include <BALL/NMR/haighMallionShiftProcessor.h>
#include <BALL/KERNEL/atomIterator.h>
#include <BALL/KERNEL/PTE.h>

using namespace std;

namespace BALL 
{

  const char* HaighMallionShiftProcessor::PROPERTY__RING_CURRENT_SHIFT = "RingCurrentShift[HM]";

	const float HaighMallionShiftProcessor::big_loop_radius_		= 1.39e-10;
	const float HaighMallionShiftProcessor::little_loop_radius_ = 1.182e-10;
	const float HaighMallionShiftProcessor::B_ = 5.455e-6;					//Konstante in IBG(r)
	const float cut_off2 = 225.0; // cut off: nuclei that are further than 15A away from the center
																// of a ring are no affected by this ring (cut of is squared for efficiency)

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
		:	ShiftModule()
	{
	}

	void HaighMallionShiftProcessor::init()
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

		valid_ = true;
	}

		
	HaighMallionShiftProcessor::~HaighMallionShiftProcessor()
		throw()
	{
	}		

	bool HaighMallionShiftProcessor::finish()
		throw()
	{
		if (!isValid())
		{
			return false;
		}
		
		// Berechnung des shifts fuer jedes Proton der liste _proton_list
				
		// hshift bezeichnet den chemicalshift der durch den gerade abgearbeiteten Ring verursacht wird.
			
		// iteriere ueber alle Protonen
		// fuer jedes Proton iteriere ueber alle Ringe und berechne chemical_shift
		
		Vector3* ring_positions = new Vector3[6];

		Size zaehler, number_of_rings;
		float radius;
		for (list<PDBAtom*>::iterator proton_iter = proton_list_.begin();
				 proton_iter != proton_list_.end(); ++proton_iter)
		{
			float shift = 0;
			for (list<Residue*>::iterator arom_iter = aromat_list_.begin();
					 arom_iter != aromat_list_.end(); ++arom_iter)
			{
				const Residue& residue = *(*arom_iter);
				if (((*proton_iter)->getResidue() == &residue)
						&& ((*proton_iter)->getName() != "HA") 
						&& ((*proton_iter)->getName() != "HA"))
				{
					continue;
				}

				if (residue.getName()=="TRP")
				{
					zaehler = 0;
					number_of_rings = 2;
					radius = big_loop_radius_;
				}
				if (residue.getName()=="PHE")
				{
					zaehler = 1;
					number_of_rings = 1;
					radius = big_loop_radius_;
				}
				if (residue.getName()=="TYR")
				{
					zaehler = 2; 
					number_of_rings = 1; 
					radius = little_loop_radius_;
				}
				if (residue.getName()=="HIS")
				{
					zaehler = 3; 
					number_of_rings = 1; 
					radius = little_loop_radius_;
				}

				Position number_of_ring_atoms = 0;
				Position hilf = 0;
				while(number_of_rings) // for all rings in the amino acid
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
							if	(asrings_[zaehler][1 + pos] == (*atom_iter).getName())
							{
								ring_positions[number_of_ring_atoms] = (*atom_iter).getPosition();
								number_of_ring_atoms++ ;
								Log.info() << "ring atom #" << number_of_ring_atoms << ":  " << atom_iter->getName() << endl;
								break;	// gefunden
							}	
						}
					}
				
					// _vector_feld ist bestimmt und number_of_ring_atoms zeigt hinter letzten gueltigen vector
				
					// bestimme den Mittelpunkt
				
					Vector3 center;
					
					for (Position pos = 0; pos < number_of_ring_atoms; pos++)
					{
						center += ring_positions[pos];
					}
					center /= number_of_ring_atoms;
					Log.info() << "center: " << center << endl;
				
					// if the center of the ring is within the cut off,
					// perform the shift calculation

					float sum = 0;
					Vector3 nucleus_pos = (*proton_iter)->getPosition();					
					if (nucleus_pos.getSquareDistance(center) <= cut_off2)
					{
						Log.info() << "  proton: " << nucleus_pos << endl;
						Log.info() << "  distance: " << nucleus_pos.getDistance(center) << endl;

						// determine the normal vector of the ring plane
						Vector3 normal;
						for (Position pos = 0; pos < number_of_ring_atoms; pos++)
						{
							const Vector3& left  = ring_positions[(pos + 0) % (number_of_ring_atoms)];
							const Vector3& middle  = ring_positions[(pos + 1) % (number_of_ring_atoms)];
							const Vector3& right = ring_positions[(pos + 2) % (number_of_ring_atoms)];
							normal += (middle - left) % (middle - right);  
						}
						// normalize the normal vector to unit length
						normal.normalize(); 
						Log.info() << "normal: " << normal << endl;
					
						// determine the ring area
						const Vector3 ein_vector(ring_positions[5] - ring_positions[1]);
						Vector3 v(ring_positions[1] - ring_positions[2]);
						float ring_area = (ring_positions[0] - ring_positions[1]) * (ring_positions[5] - ring_positions[1]);
						ring_area += ein_vector.getLength() * v.getLength();
					
						
						// berechne Projektion des Protons auf die Ringebene
						float a = normal * nucleus_pos - normal * center;
						Vector3 v_ring;

						if (a < 0) 
						{
							v_ring = nucleus_pos - a * normal;
						}
						else 
						{
							v_ring= nucleus_pos + a * normal;
						}
					
						// Schleife ueber alle Bindungen des Rings
						for (Position pos = 0; pos < number_of_ring_atoms; pos++ )
						{
							const Vector3& eins = ring_positions[(pos + 0) % (number_of_ring_atoms)];
							const Vector3& zwei = ring_positions[(pos + 1) % (number_of_ring_atoms)];
						
							// berechne Abstand von Proton zu eins und zwei
							Vector3 ein_vector = nucleus_pos - eins;
							float a_eins = (ein_vector.getLength() / radius) * 1e-10;
							ein_vector = nucleus_pos - zwei;
							float a_zwei = (ein_vector.getLength() / radius) * 1e-10;
							Log.info() << "hm: " << eins - nucleus_pos << zwei - nucleus_pos << normal << endl;
						
							// berechne Flaecheninhalt des 3Ecks aus eins,zwei,v_ring
							Vector3 eins_zwei(eins - zwei);
						
							Vector3 n0 = normal % eins_zwei;
							n0.normalize();
						
							float h = (n0 * v_ring  -  n0 * zwei);
							if (h < 0)
							{
								h = -h;
							}
						
							float f = 0.5 * h * eins_zwei.getLength() / ring_area;
						
							// bestimme das Vorzeichen der Dreiecksflaeche
							if (getLambda((v_ring - eins) % (v_ring - zwei), normal) > 0)
							{
								f *= -1; 
								Log.info() << "  inverting sign" << endl;
							}
						
							// addiere gesamtsumme	
							sum += f *( (1 / (a_eins * a_eins * a_eins)) + (1 / (a_zwei * a_zwei * a_zwei)) );

							Log.info() << "HM contribution: " << f*( (1 / (a_eins * a_eins * a_eins)) + (1 / (a_zwei * a_zwei * a_zwei)) ) << endl;
		
						}
					}

					Log.info() << "  ring contribution: " << sum *B_ << " (sum = " << sum << ")" << endl;
					shift += sum * B_; // zaehle aktuellen chemicalshift zum Gesamtwert dazu
					
					number_of_ring_atoms = 0;
					number_of_rings--;
					hilf = 6; // fuer den naechsten schleifendurchlauf
				}
			}
			
			// Setze Property chemicalshift des gerade bearbeiteten Protons auf den entsprechenden Wert.
			float hshift = shift * 1e6;
			shift = (*proton_iter)->getProperty(ShiftModule::PROPERTY__SHIFT).getFloat();
			shift += hshift;
			(*proton_iter)->setProperty(ShiftModule::PROPERTY__SHIFT, shift);
			(*proton_iter)->setProperty(PROPERTY__RING_CURRENT_SHIFT, hshift);			
			Log.info() << "  SHIFT: " << hshift << endl;
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
