// $Id: johnsonBovey.C,v 1.10 2000/09/16 07:44:13 oliver Exp $


#include <BALL/NMR/johnsonBovey.h>
#include <BALL/KERNEL/atomIterator.h>
#include <BALL/KERNEL/PTE.h>

#include <list>
using std::list;

using namespace std;

namespace BALL 
{

	const int RING_MAX_ATOMS = 6;
	const int MAX_EXPR = 5;

	BALL_INLINE float SQR(float x)
	{
		return (x*x);
	}
		
	Vector3 kreuz(const Vector3 &v1,const Vector3 &v2)
		{
		Vector3* erg=new Vector3;
		erg->x=v1.y*v2.z-v1.z*v2.y;
		erg->y=v1.z*v2.x-v1.x*v2.z;
		erg->z=v1.x*v2.y-v1.y*v2.x;
		return *erg;
		}

	float betrag(const Vector3 &v)	// berechnet Betrag eines Vektors
					{
					float e=sqrt( (v*v) );
					return e;
					}	

	//// begin der echten Klassen Funktionen

	// die Integral Funktionen

	float JohnsonBoveyShift::carlsonEllipticalIntegral1_(float x,float y,float z)
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
			Log.error() << "JohnsonBoveyShiftProcessor::carlsonEllipticalIntegral1_ : Fehler bei den Argumenten" << endl;
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
		
	float JohnsonBoveyShift::carlsonEllipticalIntegral2_(float x,float y,float z)
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
			Log.error() << "JohnsonBoveyShiftProcessor::carlsonEllipticalIntegral2_ : Fehler bei den Argumenten" << endl;
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
		
	float JohnsonBoveyShift::legendreEllipticalIntegral1_(float phi,float ak)
		{
		/*
		Legendre elliptic integral of the 1st kind f(phi,k) , evaluated using Carlson's function rf.
		The argument ranges are 0 <=phi <=PI/2 , 0 <=k*sin(phi) <=1.
		*/
		
		float s;
		
		s=sin(phi);
		return s*carlsonEllipticalIntegral1_(SQR(cos(phi)),(1.0-s*ak)*(1.0 + s*ak),1.0);
		}
		
	float JohnsonBoveyShift::legendreEllipticalIntegral2_(float phi,float ak)
		{
		/*
		Legendre elliptic integral of the 2nd kind E(phi,k). evaluated using Carlson's functions Rd and Rf.
		The argument ranges are 0 <=phi <=PI/2, o <=ksin(phi) <=1.
		*/
		
		float cc,q,s;
		
		s=sin(phi);
		cc=SQR(cos(phi));
		q=(1.0-s*ak)*(1.0 + s*ak);
		return s*(carlsonEllipticalIntegral1_(cc,q,1.0)-(SQR(s*ak))*carlsonEllipticalIntegral2_(cc,q,1.0)/3.0);
		}
		
					 
			

	//Konstruktor

	JohnsonBoveyShift::JohnsonBoveyShift()
		{
		ini_filename_ = "/KM/fopra/compbio/burch/BALL/source/NMR/dat/nmr.ini";
		}

		
	//Destruktor

	JohnsonBoveyShift::~JohnsonBoveyShift()
		{
		}

	void JohnsonBoveyShift::setFilename(const String& filename)
	{
		ini_filename_ = filename;
	}

	const String& JohnsonBoveyShift::getFilename() const
	{
		return ini_filename_;
	}


	//StartFunktion

	bool JohnsonBoveyShift::start()
	{
		//cout << "JohnsonBoveyShift::start()" << endl;
		
		// hier werden die Parameter eingelesen und entsprechende Datenstrukturen aufgebaut
		
		parameters_.setFilename(ini_filename_);
		
		int number_of_keys;
		int counter;
		int number;
		
		// einlesen der Ringe und Aufbau der Hashtabellen
		
		parameter_section_.extractSection(parameters_,"JB-Rings");
		
		number_of_keys = parameter_section_.getNumberOfKeys();
		
		String ring_entry;
		String residue_name;
		String name_list;
		
		Position residue_name_column = parameter_section_.getColumnIndex("residue_name");
		Position radius_column = parameter_section_.getColumnIndex("radius");
		Position electrons_column = parameter_section_.getColumnIndex("electrons");
		Position name_list_column = parameter_section_.getColumnIndex("name_list");
		
		for (counter=0;counter<number_of_keys;counter++)
		{
			Ring new_ring;
			
			residue_name = parameter_section_.getValue(counter,residue_name_column);
			
			if (residues_with_rings_.has(residue_name)) number = residues_with_rings_[residue_name];
				else number = 0;
				
			number++;
			residues_with_rings_[residue_name]=number;
			ring_entry=residue_name;
			ring_entry.append(String(number));
			
			new_ring.radius = parameter_section_.getValue(counter,radius_column).toFloat();
			
			new_ring.electrons = parameter_section_.getValue(counter,electrons_column).toUnsignedInt();
			
			name_list = parameter_section_.getValue(counter,name_list_column);

			vector<String> names;
			name_list.split(names, ",");
			
			new_ring.atom_names=names;
			
			new_ring.number_of_atoms=number;
			
			rings_[ring_entry] = new_ring;
			
			}
		
		// einlesen der shift Atome und liste von expressions aufbauen
		
		parameter_section_.extractSection(parameters_,"JB-ShiftAtoms");
		
		number_of_keys = parameter_section_.getNumberOfKeys();
		expressions_.clear();
		
		Position description_column = parameter_section_.getColumnIndex("description");
		
		for (counter = 0; counter < number_of_keys; counter++)
		{
			expressions_.push_back(parameter_section_.getValue(counter, description_column));
		}
					
		return true;
	}
		

	//FinishFunktion

	bool JohnsonBoveyShift::finish()
	{
		// cout <<"JohnsonBoveyShift::finish()" <<endl;
		
		//Definition der lokalen Variablen:
		
		list < Residue*>::iterator	arom_iter;
		list < PDBAtom*>::iterator	atom_iter;
		AtomIterator atomiterator;
		
		int number_of_rings,counter,counter2;
		String ring_name,residue_name;
		vector<String> ring_atoms;
		int number;
		
		int vzaehler;
		int index, found;
		float wert, p, z, lambda, k, e, hshift;
		Vector3 links, mitte, rechts, v;
		Vector3* vector_field;
		
			
		// iteriere ueber alle Protonen
		// fuer jedes Proton iteriere ueber alle Ringe und berechne
		// chemical_shift
		
		vector_field=new Vector3[RING_MAX_ATOMS];
		
		for (atom_iter = atom_list_.begin(); atom_iter != atom_list_.end(); ++atom_iter)  
		{
			float shift;
			for(arom_iter = aromat_list_.begin(); arom_iter != aromat_list_.end(); ++arom_iter)
			{
				Residue* residue =*arom_iter;
				if ( (*atom_iter)->getResidue()!=residue)
					{
					residue_name = residue->getName();
					number_of_rings = residues_with_rings_[residue_name];
					
					for(counter=1;counter<=number_of_rings;counter++)
						{
						vzaehler = 0;
						
						ring_name = residue_name;
						ring_name.append(String(counter));
						Ring& ring = rings_[ring_name];
						
						Size n = ring.electrons;
						float radius = ring.radius;
						ring_atoms = ring.atom_names;
						number = ring.number_of_atoms;
						
						for(counter2=0;counter2<number;counter2++)
						{
							found = 0;
							for(atomiterator = residue->beginAtom(); +atomiterator; ++atomiterator)
							{
								//cout << counter2 <<endl;
								if ((*atomiterator).getName()==ring_atoms[counter2])
									{
									//cout << ring_atoms[counter2] <<" "<< counter2 <<" "<< vzaehler <<endl;
									vector_field[vzaehler]=(*atomiterator).getPosition();
									vzaehler++;
									found = 1;
									}
								if (found) break;
								}
							}
						// das VektorFeld ist bestimmt und vzaehler zeigt hinter den letzten gueltigen vector
					
						Vector3 center;
						
						for (counter2=0;counter2 < vzaehler;counter2++ )
						{
							center +=vector_field[counter2];
						}
						center/=vzaehler;
					
						// bestimme den NormalenVektor der Ringebene
					
						Vector3 normal;
						
						for (counter2=0;counter2 < vzaehler;counter2++ )
							{
							index=(counter2 + 0)%(vzaehler);
							links=vector_field[index];
							index=(counter2 + 1)%(vzaehler);
							mitte=vector_field[index];
							index=(counter2 + 2)%(vzaehler);
							rechts=vector_field[index];
							normal +=kreuz((mitte-links),(mitte-rechts));  // kreuz bezeichnet das Kreuzprodukt
							}
					
						normal /=vzaehler; // Normalenvektor wurde gemittelt
						normal /=betrag(normal); //Normalenvektor ist jetzt NormalenEinheitsvektor
					
						// bestimme den chemicalshift des Protons durch diesen Ring
						v=(*atom_iter)->getPosition();
					
						// berechne p und z;
					
						z= normal * v  -  normal * center;
					
						lambda= normal *(v-center)/(normal*normal);
					
						p= betrag ( (center   +   lambda * normal)  -  v);
					
						p*=1e-10;
						z*=1e-10;
					
						p/=radius;
						z/=radius;
					
						
						// Funktionsaufruf zur Integralberechnung
						
						using namespace Constants;

						wert=sqrt(4*p/(SQR(1 + p)  +  SQR(z)));
						k=legendreEllipticalIntegral1_(PI / 2,wert);
						e=legendreEllipticalIntegral2_(PI / 2,wert);
					
					// p und z sind berechnet, berechne nun die Integrale
						//
						//
						//cout << " atom : " << (*atom_iter)->getName() <<endl;
						//cout << " res  : " << (*atom_iter)->getResidue()->getName() << endl;
						
						//cout  << "p    :" << p <<endl;
						//cout  << "z    :" << z <<endl;
						//cout  << "wert :" << wert <<endl;
						//cout  << "k    :" << k <<endl;
						//cout  << "e    :" << e <<endl;
						//cout  << "r->n :" << n_ <<endl;
						//cout  << "radius:" << radius_ <<endl;
						
						
						hshift= ((VACUUM_PERMEABILITY * (double)n * 
											ELEMENTARY_CHARGE * ELEMENTARY_CHARGE)
										/(4 * PI * 6 * PI * ELECTRON_MASS * radius));
					
						hshift*= (1/sqrt( ((1 + p)*(1 + p)) + (z*z)) );
					
						hshift*= (k + ( (1-p*p-z*z)/( (1-p)*(1-p) + z*z ) ) *e );
					
						shift +=hshift;
					
						//cout  << "hschift:" << hshift << endl;
					
						vzaehler=0;
						}
						// Schleife ueber die Anzahl der Ringe des Residues beendet
						
					}// Berechnung wenn residue nicht den aktuellen ring enthaelt
				}// Schleife ueber alle Ringe
			hshift=shift * 1e6;
			shift = ((*atom_iter)->getProperty("chemical_shift")).getFloat();
			shift += hshift;
			(*atom_iter)->setProperty("chemical_shift", shift);
			(*atom_iter)->setProperty("JB", hshift);
			
			//Log.level(LogStream::INFORMATION)  << "chemical_shift :" << shift << endl;
			//cout <<"JB:Residue :"<<(*proton_iter)->getResidue()->getName()<<" Atom :"<<(*proton_iter)->getName()<< " chemical_shift :" << shift << endl;
			
			}// Schleife ueber alle Atome
				
		return 1;
		}
		
		
	//apply Funktion

	Processor::Result JohnsonBoveyShift::operator()(Composite&  object)
	{
		// ueberpruefe fuer jedes Residue ob es in residues_with_rings ist und fuege es in die Liste aromat_list_ ein.
		// ueberpruefe fuer jedes Atom die Liste der Expressions und falls eine wahr ist fuege das Atom in die Liste ein.
		if (RTTI::isKindOf<Residue>(object))  // erganze aromat_list_ um aromatische Residues 			
		{
			Residue* residue = RTTI::castTo<Residue>(object);
			if (residues_with_rings_.has(residue->getName())) aromat_list_.push_back(residue);
		}
			// Liste um Aromaten erweitert
		
		if (RTTI::isKindOf<PDBAtom>(object))
		{
			//cout  << endl << "Object is PDBAtom";
			
			PDBAtom* atom_ptr = RTTI::castTo<PDBAtom>(object);
			
			//cout  << endl << "		atom name :" << atom_ptr->getName();
			//cout  << endl << "		Element :" << (atom_ptr->getElement()).getName();
			
			for (Size counter = 0; counter < expressions_.size(); counter++)
			{
				if (expressions_[counter](*atom_ptr))
				{
					atom_list_.push_back(atom_ptr);
					counter=MAX_EXPR+1;
				}
			}
			
		} // Ende is kind of PDBAtom : die Liste wurde um alle entsprechenden Atome  erweitert
		
		return Processor::CONTINUE;
	}

} // namespace BALL
