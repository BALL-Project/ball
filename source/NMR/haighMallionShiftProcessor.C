// $Id: haighMallionShiftProcessor.C,v 1.8 2000/09/25 21:23:48 oliver Exp $

#include <BALL/NMR/haighMallionShiftProcessor.h>
#include <BALL/KERNEL/atomIterator.h>
#include <BALL/FORMAT/parameterSection.h>
#include <BALL/KERNEL/PTE.h>

using namespace std;

namespace BALL 
{

  const char* HaighMallionShiftProcessor::PROPERTY__RING_CURRENT_SHIFT = "RingCurrentShift[HM]";

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
		valid_ = false;
		if (parameters_ == 0)
		{
			return;
		}

		ParameterSection parameter_section;
		parameter_section.extractSection(*parameters_, "HaighMallionRingCurrent");
		// BAUSTELLE: ALLE Parameter muessen eingelesen werden!


		// BAUSTELLE: diese C-Arrays verschwinden! 
		//		Und END-Marker wird es auch nicht mehr geben!
		asrings_ = new String*[4];
		
		asrings_[0] = new String[13];
		
		asrings_[0][0]  = "TRP";
		asrings_[0][1]  = "CG";
		asrings_[0][2]  = "CD1";
		asrings_[0][3]  = "NE1";
		asrings_[0][4]  = "CE2";
		asrings_[0][5]  = "CD2";
		asrings_[0][6]  = "NULL";
		asrings_[0][7]  = "CE2";
		asrings_[0][8]  = "CD2";
		asrings_[0][9]  = "CE3";
		asrings_[0][10] = "CZ3";
		asrings_[0][11] = "CH2";
		asrings_[0][12] = "CZ2";
		
		asrings_[1] = new String[7];
		
		asrings_[1][0]="PHE";
		asrings_[1][1]="CG";
		asrings_[1][2]="CD1";
		asrings_[1][3]="CE1";
		asrings_[1][4]="CZ";
		asrings_[1][5]="CE2";
		asrings_[1][6]="CD2";
		
		asrings_[2] = new String[7];
		
		asrings_[2][0]="TYR";
		asrings_[2][1]="CG";
		asrings_[2][2]="CD1";
		asrings_[2][3]="CE1";
		asrings_[2][4]="CZ";
		asrings_[2][5]="CE2";
		asrings_[2][6]="CD2";
		
		asrings_[3] = new String[7];
		
		asrings_[3][0]="HIS";
		asrings_[3][1]="CG";
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
		
		// Berechnung des shifts fuer jedes Proton der liste proton_list
				
		// hshift bezeichnet den chemicalshift der durch den gerade abgearbeiteten Ring verursacht wird.
			
		// iteriere ueber alle Protonen
		// fuer jedes Proton iteriere ueber alle Ringe und berechne chemical_shift
		
		Vector3* ring_positions = new Vector3[6];

		Size zaehler, number_of_rings;
		for (list<PDBAtom*>::iterator proton_iter = proton_list_.begin();
				 proton_iter != proton_list_.end(); ++proton_iter)
		{
			float shift = 0;
			for (list<Residue*>::iterator arom_iter = aromat_list_.begin();
					 arom_iter != aromat_list_.end(); ++arom_iter)
			{
				// consider the HN and Halpha protons in the same residue only				
				const Residue& residue = *(*arom_iter);
				if (((*proton_iter)->getResidue() == &residue)
						&& ((*proton_iter)->getName() != "HA") 
						&& ((*proton_iter)->getName() != "H"))
				{
					continue;
				}

				// the intensity factor i in  \delta = i B G(r)
				// (different for each aromatic residue and relative to benzene = 1.0)
				float intensity_factor = 1.0;

				if (residue.getName()=="TRP")
				{
					zaehler = 0;
					number_of_rings = 2;
					intensity_factor = 1.05;
				}
				if (residue.getName()=="PHE")
				{
					zaehler = 1;
					number_of_rings = 1;
					intensity_factor = 1.05;
				}
				if (residue.getName()=="TYR")
				{
					zaehler = 2; 
					number_of_rings = 1; 
					intensity_factor = 0.92;
				}
				if (residue.getName()=="HIS")
				{
					zaehler = 3; 
					number_of_rings = 1; 
					intensity_factor = 0.43;
				}

				Position number_of_ring_atoms = 0;
				Position hilf = 0;
				while (number_of_rings) // for all rings in the amino acid
				{
					if ((number_of_rings == 1) && (residue.getName() == "TRP"))	
					{
						intensity_factor = 1.04;
					}

					//Aufbau von vector_feld
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
								break;
							}	
						}
					}
				
					// BAUSTELLE: check for missing ring atoms!
				
					// determine the ring center				
					Vector3 center;
					for (Position pos = 0; pos < number_of_ring_atoms; pos++)
					{
						center += ring_positions[pos];
					}
					center /= number_of_ring_atoms;
				
					// if the center of the ring is within the cut off,
					// perform the shift calculation
					Vector3 nucleus_pos = (*proton_iter)->getPosition();					
					if (nucleus_pos.getSquareDistance(center) <= cut_off2)
					{
						// determine the normal vector of the ring plane
						Vector3 normal;
						for (Position pos = 0; pos < number_of_ring_atoms; pos++)
						{
							const Vector3& left  = ring_positions[(pos + 0) % (number_of_ring_atoms)];
							const Vector3& middle  = ring_positions[(pos + 1) % (number_of_ring_atoms)];
							const Vector3& right = ring_positions[(pos + 2) % (number_of_ring_atoms)];
							normal += (middle - right) % (middle - left);  
						}
						// normalize the normal vector to unit length
						normal.normalize(); 

						// determine the sign of the normal
						float normal_sign = 1.0;
						
						Vector3 d1 = ring_positions[0] - center;
						Vector3 d2 = ring_positions[1] - center;
						Vector3 vp = d1 % d2;
						if ((vp * normal) > 0.0)
						{
							normal_sign = -1.0;
						}
						
						// loop over all ring bonds
						float sum = 0;
						for (Position pos = 0; pos < number_of_ring_atoms; pos++ )
						{
							// determine the contributions of one ring bond
							Vector3 r_1 = ring_positions[(pos + 0) % (number_of_ring_atoms)] - nucleus_pos;
							Vector3 r_2 = ring_positions[(pos + 1) % (number_of_ring_atoms)] - nucleus_pos;

							//          <r_1, (r_2 x n)>   /    1              1    \
							// value =  ---------------- * |---------   +  ---------|
							//	               2           \ |r_1|^3        |r_2|^3 /
							float value  = r_1 * (r_2 % normal) 
										* 0.5 * (1.0 / (r_1.getSquareLength() * r_1.getLength()) 
														 +  1.0 / (r_2.getSquareLength() * r_2.getLength()));
							
							// add the contributions of this ring
							sum += value;
						}

						// add up all contributions
						shift +=  intensity_factor * B_ * normal_sign * sum;
					}

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
		}
				
		return true;
	}
		
	Processor::Result HaighMallionShiftProcessor::operator () (Composite& object)
		throw()
	{
		// Arbeitet als Kollektor :
		// - Alle Ringe werden in aromat_list gespeichert
		// - Alle Protonen werden in protonlist gespeichert
		// - die Berechnung erfolgt dann in der Finish Prozedur
		
		// erganze aromat_list um aromatische Residues
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
