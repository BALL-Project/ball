// $Id: haighMallionShiftProcessor.C,v 1.2 2000/09/19 13:34:28 oliver Exp $

#include <BALL/NMR/haighMallionShiftProcessor.h>
#include <BALL/KERNEL/atomIterator.h>
#include <BALL/KERNEL/PTE.h>

using namespace std;

namespace BALL 
{


#	ifndef RINGCONSTANTS
#		define radius1 1.39e-10; //Schleifenradius gross
#		define radius2 1.182e-10;  //Schleifenradius klein
#		define n1 3;   // number of aromatic electrons /2  for six-memeberd rings
#		define n2 3;   //    -- " --                       for five-membered rings
#		define B 5.455e-6;   //Konstante in IBG(r)
#		define RINGCONSTANTS
#	endif

	//// diverse Funktionen

	float getLambda(const Vector3 &v1,const Vector3 &v2)
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
		
	// default constructor
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

		
	// destructor
	HaighMallionShiftProcessor::~HaighMallionShiftProcessor()
		throw()
	{
	}


	// Processor start method
	bool HaighMallionShiftProcessor::start()
		throw()
	{
		return true;
	}
		

	//FinishFunktion

	bool HaighMallionShiftProcessor::finish()
		throw()
	{
		// Hier erfolgt die Berechnung des shifts fuer jedes Proton der liste _proton_list
		
		//Definition der lokalen Variablen:
		
		list < Residue*>::iterator arom_iter;
		list < PDBAtom*>::iterator proton_iter;
		AtomIterator atom_iter;
		
		int zaehler,zaehler2,anzahl,vzaehler,hilf;
		int index,found;
		float hshift,a,a_eins,a_zwei,h,f,ts,gs,ring_flaeche;
		Vector3 n0,eins_zwei,eins,zwei,v,v_ring,links,mitte,rechts,ein_vector;
		Vector3 *vector_feld_;
		
		// hshift bezeichnet den chemicalshift der durch den gerade abgearbeiteten Ring verursacht wird.
		// shift_ ist anfangs auf Null und hshift wird bei jedem Durchlauf dazu addiert.
			
		// iteriere ueber alle Protonen
		// fuer jedes Proton iteriere ueber alle Ringe und berechne
		// chemical_shift
		
		vector_feld_=new Vector3[6];
		
		for(proton_iter=proton_list_.begin(); proton_iter!=proton_list_.end() ;++ proton_iter)  //Protonen iterator
			{
			float shift = 0;
			Size n;
			float radius;
			for(arom_iter=aromat_list_.begin(); arom_iter!=aromat_list_.end();++ arom_iter)
			{
				Residue* residue = (*arom_iter);
				if ((*proton_iter)->getResidue()!=residue)
					{
					if (residue->getName()=="TRP"){zaehler=0;anzahl=2;n=n1;radius=radius1;}
					if (residue->getName()=="PHE"){zaehler=1;anzahl=1;n=n1;radius=radius1;}
					if (residue->getName()=="TYR"){zaehler=2;anzahl=1;n=n1;radius=radius1;}
					if (residue->getName()=="HIS"){zaehler=3;anzahl=1;n=n2;radius=radius2;}
					vzaehler=0;
					hilf=0;
					do
						{
						//Aufbau von _vector_feld
						for(zaehler2=hilf;zaehler2 < hilf + 6;zaehler2++ )
							{
							if (asrings_[zaehler][1 + zaehler2]=="NULL") break;
							found=0;
							for(atom_iter=residue->beginAtom(); + atom_iter;++ atom_iter)
								{
								// cout <<endl<<"asrings_name:"<<asrings_[zaehler][1+zaehler2] <<"  atom_iter Name:"<<(*atom_iter).getName();
								// cout <<"   vgl:"<<(asrings_[zaehler][1+zaehler2]==(*atom_iter).getName());
								if(asrings_[zaehler][1 + zaehler2]==(*atom_iter).getName())
									{
									vector_feld_[vzaehler]=(*atom_iter).getPosition();
									vzaehler++ ;
									found=1;
									}	
								if (found) break;
								}
							}
						 //cout <<endl <<"vzaehler : "<<vzaehler;
					
						// _vector_feld ist bestimmt und vzaehler zeigt hinter letzten gueltigen vector
					
						// bestimme den Mittelpunkt
					
						Vector3 center;
						
						for (zaehler2=0;zaehler2 < vzaehler;zaehler2++ )
							center +=vector_feld_[zaehler2];
						center/=vzaehler;
					
						// bestimme den NormalenVektor der Ringebene
					
						Vector3 normal;
						
						for (zaehler2=0;zaehler2 < vzaehler;zaehler2++ )
							{
							index=(zaehler2 + 0)%(vzaehler);
							links=vector_feld_[index];
							index=(zaehler2 + 1)%(vzaehler);
							mitte=vector_feld_[index];
							index=(zaehler2 + 2)%(vzaehler);
							rechts=vector_feld_[index];
							normal +=(mitte-links)%(mitte-rechts);  
							}
					
						normal/=vzaehler; // Normalenvektor wurde gemittelt
						normal/=normal.getLength(); //Normalenvektor ist jetzt NormalenEinheitsvektor
					
						// Die Ringebene wurde ermittelt

						// berechne den Flaecheninhalt des Rings
						ein_vector=vector_feld_[5]-vector_feld_[1];
						v=vector_feld_[1]-vector_feld_[2];
						ring_flaeche=((vector_feld_[0]-vector_feld_[1])*(vector_feld_[5]-vector_feld_[1]));
						ring_flaeche+=( ein_vector.getLength() * v.getLength() );
					
						// bestimme den chemicalshift des Protons durch diesen Ring
						v=(*proton_iter)->getPosition();
						//cout << endl<<"Proton Position :"<<v;
					
						// berechne Projektion des Protons auf die Ringebene
						a=normal*v-normal*center;
						if (a<0) v_ring=v-a*normal;
							else v_ring=v+a*normal;
						//cout <<endl<<_normal*v_ring-_normal*_mittelpunkt;
						//cout <<endl<<"v_ring :"<<v_ring;
					
						// Schleife ueber alle Bindungen des Rings
					
						gs=0;ts=0;
						for (zaehler2=0;zaehler2 < vzaehler;zaehler2++ )
							{
							ts=0;
							index=(zaehler2 + 0)%(vzaehler);
							eins=vector_feld_[index];
							index=(zaehler2 + 1)%(vzaehler);
							zwei=vector_feld_[index];
						
							// berechne Abstand von Proton zu eins und zwei
							ein_vector=v-eins;
							a_eins=(ein_vector.getLength()/radius)*1e-10;
							ein_vector=v-zwei;
							a_zwei=(ein_vector.getLength()/radius)*1e-10;
						
							// berechne Flaecheninhalt des 3Ecks aus eins,zwei,v_ring
							eins_zwei=eins-zwei;
						
							n0=normal%eins_zwei;
							n0/=n0.getLength();
						
							h=((n0*v_ring)-(n0*zwei));
							if (h<0) h=-h;
						
							f=0.5*h*(eins_zwei.getLength())/ring_flaeche;
						
							// bestimme das Vorzeichen der Dreiecksflaeche
							if ( getLambda(((v_ring-eins)%(v_ring-zwei)),normal) > 0) f*=-1; 
						
							// berechne Teilsumme
							ts=f*( (1/(a_eins*a_eins*a_eins)) + (1/(a_zwei*a_zwei*a_zwei)) );
							//cout <<endl<<"Teilsumme :"<<ts;
						
							// addiere gesamtsumme	
							gs+=ts;
							}
							//cout <<endl<<"Gesamtsumme :"<<gs;
							
						hshift=gs*B; // setze aktuellen chemicalshift
					
						shift +=hshift; // zaehle aktuellen chemicalshift zum Gesamtwert dazu
					
						//cout  <<  endl  << "hschift:" << hshift;
					
						vzaehler=0;
						anzahl--;
						hilf=6; // fuer den naechsten schleifendurchlauf

						}
					while(anzahl); // falls 2 Ringe in Aminosaure vorhanden sind
					}
				}
			
			// Setze Property chemicalshift des gerade bearbeiteten Protons auf den entsprechenden Wert.
			hshift=shift*1e6;
			shift=(*proton_iter)->getProperty("chemical_shift").getFloat();
			shift+=hshift;
			(*proton_iter)->setProperty("chemical_shift", shift);
			(*proton_iter)->setProperty("HM",hshift);			
		}
				
		return true;
	}
		
		
	//apply Funktion

	Processor::Result HaighMallionShiftProcessor::operator () (Composite& object)
		throw()
	{
		// Arbeitet als Kollektor :
		// - Alle Ringe werden in _aromat_list gespeichert
		// - Alle Protonen werden in _protonlist gespeichert
		// - die Berechnung erfolgt dann in der Finish Prozedur
		
		
		// Definition von lokalen Variablen
		//cout <<endl<<"HaighMallionShiftProcessor::operator()";
		int zaehler;
		int found;
		
		// identifiziere die Art des Objectes 
		// wenn es ein Residue mit aromatischen Ring ist, fuege es in _aromat_list ein
		// wenn es ein Hydrogen ist, fuege es in _protonlist ein
		
		if ( RTTI::isKindOf<Residue>(object) ) // erganze _aromat_listum aromatische Residues 		
		{
			//cout  <<  endl  <<  "Object is residue";
			Residue* residue = RTTI::castTo<Residue>(object);
			//cout  <<  endl  <<  "	residue name : " << residue->getName();
			for(zaehler=0;zaehler < 4;zaehler++ )
			{
				found =0;
				if (asrings_[zaehler][0]==residue->getName())
					{
					aromat_list_.push_back(residue);
					//cout  << "...eingefuegt.";
					found=1; // gefunden und schleife hinterher beenden
				}
			if (found) break;
			}		
		} // Ende is kind of Residue : die liste wurde um die entsprechenden Residues erweitert
		
		if ( RTTI::isKindOf<PDBAtom>(object) )
		{
			//cout  << endl << "Object is ProteinAtom";
			PDBAtom* atom_ptr = RTTI::castTo<PDBAtom>(object);
			//cout  << endl << "		atom name : " << atom_ptr->getName();
			//cout  << endl << "		Element : " << (atom_ptr->getElement()).getName();
			if (atom_ptr->getElement()==PTE[Element::H])
			{
				proton_list_.push_back(atom_ptr);
				//cout  << "...eingefuegt.";
			}
		} // Ende is kind of ProteinAtom : die liste wurde um protonen erweitert
		
		return Processor::CONTINUE;
	}

} // namespace Ball
