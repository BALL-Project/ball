// $Id: johnsonBoveyShiftProcessor.C,v 1.3 2000/09/21 13:52:51 amoll Exp $

#include <BALL/NMR/johnsonBoveyShiftProcessor.h>
#include <BALL/KERNEL/atomIterator.h>
#include <BALL/KERNEL/PTE.h>
#include <BALL/SYSTEM/path.h>

#include <list>

using namespace std;

namespace BALL 
{

	const int RING_MAX_ATOMS = 6;

	BALL_INLINE 
	float SQR(float x)
	{
		return (x*x);
	}
		
	float JohnsonBoveyShiftProcessor::carlsonEllipticalIntegral1_(float x, float y, float z)
	{
		// Lokale Konstanten Definitionen :
		
		const float ERRTOL = 0.08;
		const float TINY = 1.5e-38;
		const float BIG = 3.0e37;
		const float C1 = (1.0 / 24.0);
		const float C2 = 0.1;
		const float C3 = (3.0 / 44.0);
		const float C4 = (1.0 / 14.0);
		
		/*
		Computes Carlson's elliptic integral of the first kind, Rf(x,y,z). x,y,z must be nonnegative, and at most
		one can be zero.Tiny must be at least 5 times the machine underflow limit, Big must be at most on fit with the 
		machine overflow limit.
		*/
		
		if(BALL_MIN3(x, y, z)  < 0.0 || BALL_MIN3(x + y, x + z ,y + z) < TINY || BALL_MAX3(x, y, z) > BIG)
		{
			Log.error() << "JohnsonBoveyShiftProcessorProcessor::carlsonEllipticalIntegral1_ : Fehler bei den Argumenten" << endl;
			return 0;
		}

		float alamb, ave, e2, e3;

		do
		{
			alamb = sqrt(x) * (sqrt(y) + sqrt(z))  + sqrt(y) * sqrt(z);
			x = 0.25 * (x + alamb);
			y = 0.25 * (y + alamb);
			z = 0.25 * (z + alamb);
			ave = (x + y + z) / 3;
			x = (ave - x) / ave;
			y = (ave - y) / ave;
			z = (ave - z) / ave;
		}
		while (BALL_MAX3(fabs(x), fabs(y), fabs(z)) > ERRTOL);
		e2 = x * y - z * z;
		e3 = x * y * z;
		return (1.0 + (C1 * e2 - C2 - C3 * e3) * e2 + C4 * e3) / sqrt(ave);
	}
		
	float JohnsonBoveyShiftProcessor::carlsonEllipticalIntegral2_(float x,float y,float z)
	{
		//Lokale Konstanten Definitionen :
		
		const float ERRTOL =	0.05;
		const float TINY =		1.0e-25;
		const float BIG =			4.5e21;
		const float C1 =			(3.0 / 14.0);
		const float C2 =			(1.0 / 6.0);
		const float C3 =			(9.0 / 22.0);
		const float C4 =			(3.0 / 26.0);
		const float C5 =			(0.25 * C3);
		const float C6 =			(1.5 * C4);
		
		/*
		Computes Carlson's ellipic integral of the second kind, Rd(x,y,z). x,y,z must be nonnegative, and at most
		one can be zero. z must be positive. TINY must be at least twice the negative 2/3 power of the machine
		overflow limit. BIG must be at mos 0.1xERRTOL time the negative 2/3 power of the machin underflow limit
		*/
		
		if(BALL_MIN(x, y) < 0.0 || BALL_MIN(x  +  y, z) < TINY || BALL_MAX3(x, y, z) > BIG)
		{
			Log.error() << "JohnsonBoveyShiftProcessorProcessor::carlsonEllipticalIntegral2_ : Fehler bei den Argumenten" << endl;
			return 0;
			}

			float alamb, ave, ea, eb, ec, ed, ee;
			float	sum = 0.0;
			float	fac = 1.0;
			do
			{
				alamb = sqrt(x) * (sqrt(y) + sqrt(z)) + sqrt(y) * sqrt(z);
				sum += fac / (sqrt(z) * (z + alamb));
				fac = 0.25 * fac;
				x = 0.25 * (x + alamb);
				y = 0.25 * (y + alamb);
				z = 0.25 * (z + alamb);
				ave = 0.2 * (x + y + 3.0 * z);
				x = (ave - x) / ave;
				y = (ave - y) / ave;
				z = (ave - z) / ave;
			}
			while (BALL_MAX3(fabs(x), fabs(y), fabs(z)) > ERRTOL);
			ea = x * y;
			eb = z * z;
			ec = ea - eb;
			ed = ea - 6.0 * eb;
			ee = ed + ec + ec;
			return 3.0 * sum + fac * (1.0 + ed * (-C1 + C5 * ed - C6 * z * ee) 
							+ z * (C2 * ee + z * (-C3 * ec + z * C4 * ea))) / (ave * sqrt(ave));
		}
		
	float JohnsonBoveyShiftProcessor::legendreEllipticalIntegral1_(float phi, float ak)
	{
		/*
		Legendre elliptic integral of the 1st kind f(phi,k) , evaluated using Carlson's function rf.
		The argument ranges are 0 <=phi <=PI/2 , 0 <=k*sin(phi) <=1.
		*/
		
		const float s = sin(phi);
		return s * carlsonEllipticalIntegral1_(SQR(cos(phi)), (1.0 - s * ak) * (1.0 + s * ak), 1.0);
	}
		
	float JohnsonBoveyShiftProcessor::legendreEllipticalIntegral2_(float phi,float ak)
	{
		/*
		Legendre elliptic integral of the 2nd kind E(phi,k). evaluated using Carlson's functions Rd and Rf.
		The argument ranges are 0 <=phi <=PI/2, o <=ksin(phi) <=1.
		*/
		
		const float s = sin(phi);
		const float cc = SQR(cos(phi));
		const float q = (1.0 - s * ak) * (1.0 + s * ak);
		return s * (carlsonEllipticalIntegral1_(cc,q,1.0) - (SQR(s * ak)) 
						 * carlsonEllipticalIntegral2_(cc, q, 1.0) / 3.0);
	}
		
	JohnsonBoveyShiftProcessor::JohnsonBoveyShiftProcessor()
		throw()
	{
		Path path;
		ini_filename_ = path.find("NMR/dat/nmr.ini");
	}

	JohnsonBoveyShiftProcessor::~JohnsonBoveyShiftProcessor()
		throw()
	{
	}

	void JohnsonBoveyShiftProcessor::setFilename(const String& filename)
		throw()
	{
		ini_filename_ = filename;
	}

	const String& JohnsonBoveyShiftProcessor::getFilename() const
		throw()
	{
		return ini_filename_;
	}

	bool JohnsonBoveyShiftProcessor::start()
		throw()
	{
		// hier werden die Parameter eingelesen und entsprechende Datenstrukturen aufgebaut
		parameters_.setFilename(ini_filename_);
		
		// einlesen der Ringe und Aufbau der Hashtabellen
		parameter_section_.extractSection(parameters_, "JB-Rings");
		
		Size number_of_keys = parameter_section_.getNumberOfKeys();
		
		String ring_entry;
		String residue_name;
		String name_list;
		
		Position residue_name_column = parameter_section_.getColumnIndex("residue_name");
		Position radius_column = parameter_section_.getColumnIndex("radius");
		Position electrons_column = parameter_section_.getColumnIndex("electrons");
		Position name_list_column = parameter_section_.getColumnIndex("name_list");
		
		for (Position key = 0; key < number_of_keys; key++)
		{
			Ring new_ring;
			
			residue_name = parameter_section_.getValue(key, residue_name_column);
			
			Size number;

			if (residues_with_rings_.has(residue_name)) 
			{
				number = residues_with_rings_[residue_name] + 1;
			}
			else
			{
				number = 1;
			}
				
			residues_with_rings_[residue_name] = number;
			ring_entry = residue_name;
			ring_entry.append(String(number));
			
			new_ring.radius = parameter_section_.getValue(key, radius_column).toFloat();
			
			new_ring.electrons = parameter_section_.getValue(key, electrons_column).toUnsignedInt();
			
			name_list = parameter_section_.getValue(key, name_list_column);

			vector<String> names;
			name_list.split(names, ",");
			
			new_ring.atom_names = names;
			
			new_ring.number_of_atoms = number;
			
			rings_[ring_entry] = new_ring;
		}
		
		// einlesen der shift Atome und liste von expressions aufbauen
		
		parameter_section_.extractSection(parameters_,"JB-ShiftAtoms");
		
		number_of_keys = parameter_section_.getNumberOfKeys();
		expressions_.clear();
		
		Position description_column = parameter_section_.getColumnIndex("description");
		
		for (Position pos = 0; pos < number_of_keys; pos++)
		{
			expressions_.push_back(parameter_section_.getValue(pos, description_column));
		}
					
		return true;
	}
		
	bool JohnsonBoveyShiftProcessor::finish()
		throw()
	{		
		String ring_name, residue_name;
		std::vector<String> ring_atoms;
		
		Position vzaehler;
		float wert, p, z, lambda, k, e, hshift;
		Vector3 links, mitte, rechts;
		Vector3* vector_field = new Vector3[RING_MAX_ATOMS];

		// iteriere ueber alle Protonen
		// fuer jedes Proton iteriere ueber alle Ringe und berechne chemical_shift
		for (list<PDBAtom*>::iterator atom_iter = atom_list_.begin();
				 atom_iter != atom_list_.end(); ++atom_iter)  
		{
			float shift;
			for (std::list<Residue*>::iterator arom_iter = aromat_list_.begin();
					 arom_iter != aromat_list_.end(); ++arom_iter)
			{
				Residue* residue = *arom_iter;
				if ((*atom_iter)->getResidue() == residue)
				{
					continue;
				}
				residue_name = residue->getName();
				Size numberOfRings = residues_with_rings_[residue_name];
				
				for(Position pos = 1; pos <= numberOfRings; pos++)
				{
					vzaehler = 0;
					
					ring_name = residue_name;
					ring_name.append(String(pos));
					Ring& ring = rings_[ring_name];
					
					float radius = ring.radius;
					ring_atoms = ring.atom_names;
					Size number = ring.number_of_atoms;
					
					for(Position counter2 = 0; counter2 <number; counter2++)
					{
						for(AtomIterator atomiterator = residue->beginAtom();
								+atomiterator; ++atomiterator)
						{
							if ((*atomiterator).getName() == ring_atoms[counter2])
							{
								vector_field[vzaehler]=(*atomiterator).getPosition();
								vzaehler++;
								break;  // found
							}
						}	
					}
					// das VektorFeld ist bestimmt und vzaehler zeigt hinter den letzten gueltigen vector
				
					Vector3 center;
					
					for (Position counter = 0; counter < vzaehler; counter++)
					{
						center += vector_field[counter];
					}
					center /= vzaehler;
				
					// bestimme den NormalenVektor der Ringebene				
					Vector3 normal;
					
					for (Position counter = 0; counter < vzaehler; counter++)
					{
						links  = vector_field[(counter + 0) % (vzaehler)];
						mitte  = vector_field[(counter + 1) % (vzaehler)];
						rechts = vector_field[(counter + 2) % (vzaehler)];
						normal += (mitte - links) % (mitte - rechts);
					}
				
					normal /= vzaehler; // Normalenvektor wurde gemittelt
					normal.normalize(); //Normalenvektor ist jetzt NormalenEinheitsvektor
				
					// bestimme den chemicalshift des Protons durch diesen Ring
					const Vector3& atomPosition = (*atom_iter)->getPosition();
				
					// berechne p und z;
					z = normal * atomPosition  -  normal * center;
				
					lambda= normal * (atomPosition - center) / (normal * normal);
				
					p = ((center +  lambda * normal)  -  atomPosition).getLength();
				
					p *= 1e-10;
					z *= 1e-10;
				
					p /= radius;
					z /= radius;
					
					// Funktionsaufruf zur Integralberechnung							
					using namespace Constants;

					wert = sqrt(4 * p / (SQR(1 + p)  +  SQR(z)));
					k = legendreEllipticalIntegral1_(PI / 2,wert);
					e = legendreEllipticalIntegral2_(PI / 2,wert);
				
					// p und z sind berechnet, berechne nun die Integrale		
					hshift = VACUUM_PERMEABILITY * (double)ring.electrons * ELEMENTARY_CHARGE * ELEMENTARY_CHARGE;
					hshift /= 4 * PI * 6 * PI * ELECTRON_MASS * radius;
					hshift*= (1 / sqrt( ((1 + p) * (1 + p)) + (z * z)));				
					hshift*= (k + ((1 - p * p - z * z) / ((1 - p) * (1 - p) + z * z)) *e );

					shift += hshift;								
					vzaehler=0;
				}// Schleife ueber die Anzahl der Ringe des Residues					
			
			}// Schleife ueber alle Ringe

			hshift = shift * 1e6;
			shift = ((*atom_iter)->getProperty("chemical_shift")).getFloat();
			shift += hshift;
			(*atom_iter)->setProperty("chemical_shift", shift);
			(*atom_iter)->setProperty("JB", hshift);
			
		}// Schleife ueber alle Atome
				
		return true;
	}
		
	Processor::Result JohnsonBoveyShiftProcessor::operator()(Composite&  object)
		throw()
	{
		// ueberpruefe fuer jedes Residue ob es in residues_with_rings ist und fuege es in die Liste aromat_list_ ein.
		// ueberpruefe fuer jedes Atom die Liste der Expressions und falls eine wahr ist fuege das Atom in die Liste ein.
		if (RTTI::isKindOf<Residue>(object))  // erganze aromat_list_ um aromatische Residues 			
		{
			Residue* residue = RTTI::castTo<Residue>(object);
			if (residues_with_rings_.has(residue->getName())) 
			{	
				aromat_list_.push_back(residue);
			}
		}
		// Liste um Aromaten erweitert
		
		if (RTTI::isKindOf<PDBAtom>(object))
		{			
			PDBAtom* atom_ptr = RTTI::castTo<PDBAtom>(object);
			
			for (Size counter = 0; counter < expressions_.size(); counter++)
			{
				if (expressions_[counter](*atom_ptr))
				{
					atom_list_.push_back(atom_ptr);
					break;
				}
			}
		}
		
		return Processor::CONTINUE;
	}

} // namespace BALL
