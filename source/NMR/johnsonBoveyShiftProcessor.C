// $Id: johnsonBoveyShiftProcessor.C,v 1.6 2000/09/27 07:20:24 oliver Exp $

#include <BALL/NMR/johnsonBoveyShiftProcessor.h>
#include <BALL/KERNEL/atomIterator.h>
#include <BALL/KERNEL/PTE.h>
#include <BALL/SYSTEM/path.h>

#include <list>

using namespace std;

namespace BALL 
{

	const char* JohnsonBoveyShiftProcessor::PROPERTY__RING_CURRENT_SHIFT = "RingCurrentShift[JB]";

	const int RING_MAX_ATOMS = 6;

	BALL_INLINE 
	float SQR(float x)
	{
		return (x*x);
	}
		
	float JohnsonBoveyShiftProcessor::carlsonEllipticalIntegral1_(float x,float y,float z)
	{
		// Lokale Konstanten Definitionen :
		
		const float ERRTOL=0.08;
		const float TINY=1.5e-38;
		const float BIG=3.0e37;
		const float THIRD=(1.0/3.0);
		const float C1=(1.0/24.0);
		const float C2=0.1;
		const float C3=(3.0/44.0);
		const float C4=(1.0/14.0);
		
		/*
		Computes Carlson's elliptic integral of the first kind, Rf(x,y,z). x,y,z must be nonnegative, and at most
		one can be zero.Tiny must be at least 5 times the machine underflow limit, Big must be at most on fit with the 
		machine overflow limit.
		*/
		
		float alamb,ave,delx,dely,delz,e2,e3,sqrtx,sqrty,sqrtz,xt,yt,zt;
		
		if(BALL_MIN3(x, y, z)  < 0.0 || BALL_MIN3(x + y, x + z ,y + z) < TINY || BALL_MAX3(x, y, z) > BIG)
		{
			Log.level(LogStream::ERROR) << endl << "Funktion rf : Fehler bei den Argumenten";
			return 0;
		}
		else 	
		{
			xt=x;
			yt=y;
			zt=z;
			do
			{
				sqrtx=sqrt(xt);
				sqrty=sqrt(yt);
				sqrtz=sqrt(zt);
				alamb=sqrtx*(sqrty + sqrtz) + sqrty*sqrtz;
				xt=0.25*(xt + alamb);
				yt=0.25*(yt + alamb);
				zt=0.25*(zt + alamb);
				ave=THIRD*(xt + yt + zt);
				delx=(ave-xt)/ave;
				dely=(ave-yt)/ave;
				delz=(ave-zt)/ave;
			}
			while (BALL_MAX3(fabs(delx), fabs(dely), fabs(delz)) > ERRTOL);
			e2=delx*dely-delz*delz;
			e3=delx*dely*delz;
			return (1.0 + (C1*e2-C2-C3*e3)*e2 + C4*e3)/sqrt(ave);
		}
	}
		
	float JohnsonBoveyShiftProcessor::carlsonEllipticalIntegral2_(float x,float y,float z)
	{
		//Lokale Konstanten Definitionen :
		
		const float ERRTOL=0.05;
		const float TINY=1.0e-25;
		const float BIG=4.5e21;
		const float C1=(3.0/14.0);
		const float C2=(1.0/6.0);
		const float C3=(9.0/22.0);
		const float C4=(3.0/26.0);
		const float C5=(0.25*C3);
		const float C6=(1.5*C4);
		
		/*
		Computes Carlson's ellipic integral of the second kind, Rd(x,y,z). x,y,z must be nonnegative, and at most
		one can be zero. z must be positive. TINY must be at least twice the negative 2/3 power of the machine
		overflow limit. BIG must be at mos 0.1xERRTOL time the negative 2/3 power of the machin underflow limit
		*/
		
		float alamb,ave,delx,dely,delz,ea,eb,ec,ed,ee,fac,sqrtx,sqrty,sqrtz,sum,xt,yt,zt;
		
		if(BALL_MIN(x, y) < 0.0 || BALL_MIN(x  +  y, z) < TINY || BALL_MAX3(x, y, z) > BIG)
		{
			//cout << endl << "Funktion rd : Fehler bei den Argumenten";
			Log.level(LogStream::ERROR) << endl << "Funktion rd : Fehler bei den Argumenten";
			return 0;
		}
		else
		{
				xt=x;
				yt=y;
				zt=z;
				sum=0.0;
				fac=1.0;
				do
					{
					sqrtx=sqrt(xt);
					sqrty=sqrt(yt);
					sqrtz=sqrt(zt);
					alamb=sqrtx*(sqrty + sqrtz) + sqrty*sqrtz;
					sum +=fac/(sqrtz*(zt + alamb));
					fac=0.25*fac;
					xt=0.25*(xt + alamb);
					yt=0.25*(yt + alamb);
					zt=0.25*(zt + alamb);
					ave=0.2*(xt + yt + 3.0*zt);
					delx=(ave-xt)/ave;
					dely=(ave-yt)/ave;
					delz=(ave-zt)/ave;
					}
				while (BALL_MAX3(fabs(delx), fabs(dely), fabs(delz)) > ERRTOL);
				ea=delx*dely;
				eb=delz*delz;
				ec=ea-eb;
				ed=ea-6.0*eb;
				ee=ed + ec + ec;
				return 3.0*sum + fac*(1.0 + ed*(-C1 + C5*ed-C6*delz*ee) + delz*(C2*ee + delz*(-C3*ec + delz*C4*ea)))/(ave*sqrt(ave));
			}
	}
		
	float JohnsonBoveyShiftProcessor::legendreEllipticalIntegral1_(float phi,float ak)
	{
		/*
		Legendre elliptic integral of the 1st kind f(phi,k) , evaluated using Carlson's function rf.
		The argument ranges are 0 <=phi <=PI/2 , 0 <=k*sin(phi) <=1.
		*/
		
		float s;
		
		s=sin(phi);
		return s*carlsonEllipticalIntegral1_(SQR(cos(phi)),(1.0-s*ak)*(1.0 + s*ak),1.0);
	}
		
	float JohnsonBoveyShiftProcessor::legendreEllipticalIntegral2_(float phi,float ak)
	{
		/*
		Legendre elliptic integral of the 2nd kind E(phi,k). evaluated usin Carlson's functions Rd and Rf.
		The argument ranges are 0 <=phi <=PI/2, o <=ksin(phi) <=1.
		*/
		
		float cc,q,s;
		
		s=sin(phi);
		cc=SQR(cos(phi));
		q=(1.0-s*ak)*(1.0 + s*ak);
		return s*(carlsonEllipticalIntegral1_(cc,q,1.0)-(SQR(s*ak))*carlsonEllipticalIntegral2_(cc,q,1.0)/3.0);
	}
		
	JohnsonBoveyShiftProcessor::JohnsonBoveyShiftProcessor()
		throw()
	{
	}

	JohnsonBoveyShiftProcessor::~JohnsonBoveyShiftProcessor()
		throw()
	{
	}

	void JohnsonBoveyShiftProcessor::init()
		throw()
	{
		// if anything fails, valid_ will be false
		valid_ = false;

		// if we have no parameters, abort
		if (parameters_ == 0)
		{
			return;
		}

		// einlesen der Ringe und Aufbau der Hashtabellen
		ParameterSection parameter_section;
		parameter_section.extractSection(*parameters_, "JB-Rings");
		
		Size number_of_keys = parameter_section.getNumberOfKeys();
		
		String ring_entry;
		String residue_name;
		String name_list;
		
		Position residue_name_column = parameter_section.getColumnIndex("residue_name");
		Position radius_column = parameter_section.getColumnIndex("radius");
		Position electrons_column = parameter_section.getColumnIndex("electrons");
		Position name_list_column = parameter_section.getColumnIndex("name_list");
		
		for (Position key = 0; key < number_of_keys; key++)
		{
			Ring new_ring;
			
			residue_name = parameter_section.getValue(key, residue_name_column);
			
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
			
			new_ring.radius = parameter_section.getValue(key, radius_column).toFloat();
			
			new_ring.electrons = parameter_section.getValue(key, electrons_column).toUnsignedInt();
			
			name_list = parameter_section.getValue(key, name_list_column);

			vector<String> names;
			name_list.split(names, ",");
			Log.info() << "names = " << name_list << " (vector size: " << names.size() << ")" << endl;
			
			new_ring.atom_names = names;
			
			rings_[ring_entry] = new_ring;
		}
		
		// einlesen der shift Atome und liste von expressions aufbauen
		
		parameter_section.extractSection(*parameters_,"JB-ShiftAtoms");
		
		number_of_keys = parameter_section.getNumberOfKeys();
		expressions_.clear();
		
		Position description_column = parameter_section.getColumnIndex("description");
		
		for (Position pos = 0; pos < number_of_keys; pos++)
		{
			expressions_.push_back(parameter_section.getValue(pos, description_column));
		}

		// initialization successful: the module is valid
		valid_ = true;
	}
		
	bool JohnsonBoveyShiftProcessor::start()
		throw()
	{
		if (!isValid())
		{
			return false;
		}

		// clear all temporary data structures
		proton_list_.clear();
		atom_list_.clear();
		aromat_list_.clear();

		return true;
	}
	
	bool JohnsonBoveyShiftProcessor::finish()
		throw()
	{		
		// check for validity of the object
		if (!isValid())
		{
			return false;
		}
		
		std::vector<String> ring_atoms;
		
		Position vzaehler;
		float  p, z, lambda, k, e, hshift;
		Vector3 left, center, right;
		Vector3* vector_field = new Vector3[RING_MAX_ATOMS];


		Log.info() << "# nuclei: " << atom_list_.size() << endl;
		Log.info() << "# rings: " << aromat_list_.size() << endl;
		// iterate over all nuclei
		for (list<Atom*>::iterator atom_iter = atom_list_.begin();
				 atom_iter != atom_list_.end(); ++atom_iter)  
		{
			// iterate over all aromatic rings and add their contributions
			// to the curent shift of the nucleus

			float shift = 0;
			for (std::list<Residue*>::iterator arom_iter = aromat_list_.begin();
					 arom_iter != aromat_list_.end(); ++arom_iter)
			{
				Residue* residue = *arom_iter;

				// ignore the ring contribution in the aromatic residue itself
				// (except for H and HA atoms)
				// BAUSTELLE: this should be parameterizable as well!
				if (((*atom_iter)->getResidue() == residue)
						&& ((*atom_iter)->getName() != "H")
						&& ((*atom_iter)->getName() != "HA"))
				{
					continue;
				}

				
				String residue_name = residue->getName();
				Size number_of_rings = residues_with_rings_[residue_name];				
				Log.info() << "number of rings for " << residue_name << ": " << number_of_rings << endl;
				for	(Position pos = 1; pos <= number_of_rings; pos++)
				{
					vzaehler = 0;
					
					String ring_name = residue_name;
					ring_name.append(String(pos));
					Ring& ring = rings_[ring_name];
					
					float radius = ring.radius;
					ring_atoms = ring.atom_names;

					for (Position counter2 = 0; counter2 < ring_atoms.size(); counter2++)
					{
						for (AtomIterator atomiterator = residue->beginAtom();
								+atomiterator; ++atomiterator)
						{
							if ((*atomiterator).getName() == ring_atoms[counter2])
							{
								vector_field[vzaehler] = (*atomiterator).getPosition();
								vzaehler++;
								break;  // found
							}
						}	
					}
					if (vzaehler != ring_atoms.size())
					{
						Log.warn() << "JohnsonBoveyShiftProcessor::finish: problem: could not identify all ring atoms for " 
											 << residue->getName() << residue->getID() << endl;
					}
					
					// das VektorFeld ist bestimmt und vzaehler zeigt hinter den letzten gueltigen vector
		
					
					// determine the center of the ring
					Vector3 center;
					for (Position counter = 0; counter < vzaehler; counter++)
					{
						center += vector_field[counter];
					}
					center /= vzaehler;
				
					// determine the vector perpendicular to the 
					// ring plane
					Vector3 normal;
					for (Position counter = 0; counter < vzaehler; counter++)
					{
						left  = vector_field[(counter + 0) % (vzaehler)];
						center  = vector_field[(counter + 1) % (vzaehler)];
						right = vector_field[(counter + 2) % (vzaehler)];
						normal += (center - left) % (center - right);
					}

					// normalize the normal vector (if possible)
					if (Maths::isZero(normal.getSquareLength()))
					{
						Log.warn() << "Problem: cannot normalize vector: "<< normal << endl;
					}
					else
					{
						normal.normalize(); 
				
						// determine the secondary shift contribution of this ring
						const Vector3& atom_position = (*atom_iter)->getPosition();
					
						// calculate p und z;
						z = normal * atom_position  -  normal * center;
					
						lambda= normal * (atom_position - center) / (normal * normal);
					
						p = ((center +  lambda * normal)  -  atom_position).getLength();
					
						p *= 1e-10;
						z *= 1e-10;

						p /= radius;
						z /= radius;
						
						// calculate the geometry factor: two elliptical integrals
						using namespace Constants;
						float value = sqrt(4 * p / (SQR(1 + p)  +  SQR(z)));
						k = legendreEllipticalIntegral1_(PI / 2, value);
						e = legendreEllipticalIntegral2_(PI / 2, value);
					
						// p und z sind berechnet, berechne nun die Integrale		
						hshift = VACUUM_PERMEABILITY * (double)ring.electrons * ELEMENTARY_CHARGE * ELEMENTARY_CHARGE;
						hshift /= 4 * PI * 6 * PI * ELECTRON_MASS * radius;
						hshift*= (1 / sqrt( ((1 + p) * (1 + p)) + (z * z)));				
						hshift*= (k + ((1 - p * p - z * z) / ((1 - p) * (1 - p) + z * z)) *e );

						Log.info() << "shift of " << residue_name << " on " << (*atom_iter)->getFullName() << " = " << hshift << endl;

						shift += hshift;								
						vzaehler=0;
					}
				}// Schleife ueber die Anzahl der Ringe des Residues					
			
			}// Schleife ueber alle Ringe

			hshift = shift * 1e6;
			shift = ((*atom_iter)->getProperty(ShiftModule::PROPERTY__SHIFT)).getFloat();
			shift += hshift;
			(*atom_iter)->setProperty(ShiftModule::PROPERTY__SHIFT, shift);
			(*atom_iter)->setProperty(PROPERTY__RING_CURRENT_SHIFT, hshift);
			
		}// Schleife ueber alle Atome
				
		return true;
	}
		
	Processor::Result JohnsonBoveyShiftProcessor::operator () (Composite& composite)
		throw()
	{
		// ueberpruefe fuer jedes Residue ob es in residues_with_rings ist und fuege es in die Liste aromat_list_ ein.
		// ueberpruefe fuer jedes Atom die Liste der Expressions und falls eine wahr ist fuege das Atom in die Liste ein.
		if (RTTI::isKindOf<Residue>(composite))  // erganze aromat_list_ um aromatische Residues 			
		{
			Residue* residue = RTTI::castTo<Residue>(composite);
			if (residues_with_rings_.has(residue->getName())) 
			{	
				aromat_list_.push_back(residue);
			}
		}
		// Liste um Aromaten erweitert
		
		if (RTTI::isKindOf<Atom>(composite))
		{			
			Atom* atom_ptr = RTTI::castTo<Atom>(composite);
			
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
