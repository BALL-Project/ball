// $Id: johnsonBovey.C,v 1.2 1999/09/22 17:43:01 oliver Exp $


#include <BALL/NMR/johnsonBovey.h>

///////////////////////////////////////////////////////////////////////////////////////////////////
// noch zu erledigen :
//
// - Werte vergleichen, relative Werte scheinen zu stimmen : an RingProtonen deutlich groessere Verschiebung
//   Groessenordnung ppm stimmt auch, aber die genauen zahlen bleiben zu verifizieren
//
// - Aufrufe des apply operators ueberpruefen (Breitensuche ???????? wohl eher nicht scheisse
//------ meine "Wunschliste" (O.K.):
// - Ueberpruefung der Vektoroperationen: Kreuzprodukt u.ae. sind alle in Vector3
//   definiert
// - Wenn SQR definiert wird, dann wenigstens als Macro oder inline!							 
// ----- Korrekturen
// Auch Fuenfringe haben sechs Elektronen (Hueckel-Regel)
// BALL_MAX3/MIN3 eingefuehrt stat BALL_MAX(BALL_MAX())
// BALL_VACUUM_PERMEABILITY statt BALL_MAGNETIC_FIELD (obsolet) eingefuehrt
///////////////////////////////////////////////////////////////////////////////////////////////////

#include <BALL/KERNEL/atomIterator.h>
#include <BALL/KERNEL/PSE.h>

#include <list>
using std::list;

namespace BALL 
{

	//// diverse leider notwendige Funktionen

	BALL_INLINE float SQR(float x)
	{
		return (x*x);
	}
		
	// Definition der Konstanten

	const float radius1 = 1.39e-10;	//Schleifenradius gross
	const float radius2 = 1.182e-10;	//Schleifenradius klein
	const float n1 = 3;		// number of aromatic electrons /2  for six-memeberd rings
	const float n2 = 3;		//		-- " --                       for five-membered rings

	// diverse Funktionen

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

	float JohnsonBoveyShift::rf(float x,float y,float z)
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
		}	else 	{
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
		
	float JohnsonBoveyShift::rd(float x,float y,float z)
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
			Log.level(LogStream::ERROR) << endl << "Funktion rd : Fehler bei den Argumenten";
			return 0;
		} else {
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
		
	float JohnsonBoveyShift::ellf(float phi,float ak)
	{
		/*
		Legendre elliptic integral of the 1st kind f(phi,k) , evaluated using Carlson's function rf.
		The argument ranges are 0 <=phi <=PI/2 , 0 <=k*sin(phi) <=1.
		*/
		
		float s;
		
		s=sin(phi);
		return s*rf(SQR(cos(phi)),(1.0-s*ak)*(1.0 + s*ak),1.0);
	}
		
	float JohnsonBoveyShift::elle(float phi,float ak)
	{
		/*
		Legendre elliptic integral of the 2nd kind E(phi,k). evaluated usin Carlson's functions Rd and Rf.
		The argument ranges are 0 <=phi <=PI/2, o <=ksin(phi) <=1.
		*/
		
		float cc,q,s;
		
		s=sin(phi);
		cc=SQR(cos(phi));
		q=(1.0-s*ak)*(1.0 + s*ak);

		return s*(rf(cc,q,1.0)-(SQR(s*ak))*rd(cc,q,1.0)/3.0);
	}
		
					 
			

	//Konstruktor

	JohnsonBoveyShift::JohnsonBoveyShift()
	{
		asrings_=new String*[4];
		
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

		
	//Destruktor

	JohnsonBoveyShift::~JohnsonBoveyShift()
	{
	}


	//StartFunktion

	bool JohnsonBoveyShift::start()
	{
		shift_ = 0;
		return 1;
	}
		

	//FinishFunktion

	bool JohnsonBoveyShift::finish()
	{
		//Definition der lokalen Variablen:
		
		list < Residue*>::iterator			arom_iter;
		list < PDBAtom*>::iterator	proton_iter;
		AtomIterator atom_iter;
		
		int zaehler, zaehler2, anzahl, vzaehler, hilf;
		int index, found;
		float wert, p, z, lambda, k, e, hshift;
		Vector3 links, mitte, rechts, v;
		Vector3* vector_field_;
		
			
		// iteriere ueber alle Protonen
		// fuer jedes Proton iteriere ueber alle Ringe und berechne
		// chemical_shift
		
		vector_field_ = new Vector3[6];
		
		for (proton_iter = proton_list_.begin(); proton_iter != proton_list_.end(); ++proton_iter)  //Protonen Iterator
		{
			shift_=0;
			for(arom_iter = aromat_list_.begin(); arom_iter != aromat_list_.end(); ++arom_iter)
			{
				if (residue_->getName()=="TRP"){zaehler=0;anzahl=2;n_=n1;radius_=radius1;}
				if (residue_->getName()=="PHE"){zaehler=1;anzahl=1;n_=n1;radius_=radius1;}
				if (residue_->getName()=="TYR"){zaehler=2;anzahl=1;n_=n1;radius_=radius1;}
				if (residue_->getName()=="HIS"){zaehler=3;anzahl=1;n_=n2;radius_=radius2;}
	
				vzaehler=0;
				hilf=0;
				
				do
				{
					//Aufbau von vector_field_
					for(zaehler2=hilf;zaehler2 < hilf + 6;zaehler2++ )
					{
						if (asrings_[zaehler][1 + zaehler2]=="NULL") 
						{
							break;
						}
						found=0;
						for(atom_iter=residue_->beginAtom(); + atom_iter;++ atom_iter)
						{
							if(asrings_[zaehler][1 + zaehler2]==(*atom_iter).getName())
							{
								vector_field_[vzaehler]=(*atom_iter).getPosition();
								vzaehler++ ;
								found=1;
							}	
							if (found)
							{
								break;
							}
						}
					}
					// vector_field_ ist bestimmt und vzaehler zeigt hinter letzten gueltigen vector
					
					// bestimme den Mittelpunkt
					
					mittelpunkt_.set(0.0,0.0,0.0);
						
					for (zaehler2=0;zaehler2 < vzaehler;zaehler2++ )
					{
						mittelpunkt_ +=vector_field_[zaehler2];
					}
					mittelpunkt_ /= vzaehler;
					
					// bestimme den NormalenVektor der Ringebene
					
					normal_.set(0.0,0.0,0.0);
						
					for (zaehler2=0;zaehler2 < vzaehler;zaehler2++ )
					{
						index=(zaehler2 + 0)%(vzaehler);
						links=vector_field_[index];
						index=(zaehler2 + 1)%(vzaehler);
						mitte=vector_field_[index];
						index=(zaehler2 + 2)%(vzaehler);
						rechts=vector_field_[index];
						normal_ +=kreuz((mitte-links),(mitte-rechts));  // kreuz bezeichnet das Kreuzprodukt
					}
					
					normal_ /= vzaehler; // Normalenvektor wurde gemittelt
					normal_ /= betrag(normal_); //Normalenvektor ist jetzt NormalenEinheitsvektor
					
					// bestimme den chemicalshift des Protons durch diesen Ring
					v = (*proton_iter)->getPosition();
					
					// berechne p und z;
					
					z = normal_ * v  -  normal_ * mittelpunkt_;
					
					lambda = normal_*(v-mittelpunkt_)/(normal_*normal_);
					
					p = betrag((mittelpunkt_   +   lambda * normal_)  -  v);
					
					p *= 1e-10;
					z *= 1e-10;
					
					p /= radius_;
					z /= radius_;
					
					// p und z sind berechnet, berechne nun die Integrale
					//
					// Funktionsaufruf zur Integralberechnung
						
					using namespace Constants;

					wert=sqrt(4*p/(SQR(1 + p)  +  SQR(z)));
					k = ellf(PI / 2,wert);
					e = elle(PI / 2,wert);
					
					//
					//cout  <<  endl  << "p    :" << p;
					//cout  <<  endl  << "z    :" << z;
					//cout  <<  endl  << "wert :" << wert;
					//cout  <<  endl  << "k    :" << k;
					//cout  <<  endl  << "e    :" << e;
					//cout  <<  endl  << "r->n :" << n_;
					//cout  <<  endl  << "radius:" << radius_;
					//
					// Einsetzen in die Formel
					//
					hshift= ((VACUUM_PERMEABILITY * n_ * 
										ELEMENTARY_CHARGE * ELEMENTARY_CHARGE)
									/(4 * PI * 6 * PI * ELECTRON_MASS * radius_));
					
					hshift *= (1/sqrt( ((1 + p)*(1 + p)) + (z*z)) );
					
					hshift *= (k + ( (1-p*p-z*z)/( (1-p)*(1-p) + z*z ) ) *e );
					
					shift_ +=hshift;
					
					//cout  <<  endl  << "hschift:" << hshift;
					
					vzaehler=0;
					anzahl--;
					hilf=6; // fuer den naechsten schleifendurchlauf

				} while(anzahl);

			}
			
			(*proton_iter)->setProperty("chemical_shift", shift_);
			Log.level(LogStream::INFORMATION)  << "chemical_shift :" << shift_ << endl;
			
		}
				
		return 1;
	}
		
		
	//apply Funktion

	Processor::Result JohnsonBoveyShift::operator()(Object&  object)
	{
		// Definition von lokalen Variablen
		
		int zaehler;
		int found;
		
		// identifiziere die Art des Objectes 
		// wenn es ein Residue mit aromatischen Ring ist, fuege es in aromat_list_ ein
		// wenn es ein Hydrogen ist, fuege es in proton_list_ ein
		
		if (RTTI<Residue>::isKindOf(object))  // erganze aromat_list_ um aromatische Residues
		{
			residue_ = RTTI<Residue>::castTo(object);
			for(zaehler=0;zaehler < 4;zaehler++ )
			{
				found =0;
				if (asrings_[zaehler][0]==residue_->getName())
				{
					aromat_list_.push_back(residue_);
					cout  << "...eingefuegt.";
					found=1; // gefunden und schleife hinterher beenden
				}
				if (found) 
				{
					break;
				}
			}		
		} // Ende is kind of Residue : die Liste wurde um die entsprechenden Residues erweitert
		
		if (RTTI<PDBAtom>::isKindOf(object))
		{
			cout  << endl << "Object is PDBAtom";
			patom_ = RTTI<PDBAtom>::castTo(object);
			cout  << endl << "		atom name :" << patom_->getName();
			cout  << endl << "		Element :" << (patom_->getElement()).getName();
			if (patom_->getElement()==PSE[Element::H])
			{
				proton_list_.push_back(patom_);
				cout  << "...eingefuegt.";
			}
		} // Ende is kind of PDBAtom : die Liste wurde um protonen erweitert
		
		return Processor::CONTINUE;
	}

} // namespace BALL
