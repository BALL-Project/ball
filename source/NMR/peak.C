// $Id: peak.C,v 1.1 2000/07/03 20:02:24 oliver Exp $

#include<BALL/NMR/peak.h>

using namespace std;

namespace BALL {

peak::peak()
	{
	min_=0;
	max_=0;
	ppm_=0;
	breadth_=0.1;
	height_=1;
	anzahl_=0;
	}

peak::peak(peak p, int i)
	{
	min_=p.min_;
	max_=p.max_;
	ppm_=p.ppm_;
	breadth_=p.breadth_;
	height_=p.height_;
	anzahl_=p.anzahl_;
	atomlist_=p.atomlist_;
	}
	
peak::~peak()
	{
	}
	
float peak::get_ppm() const
	{
	return ppm_;
	}
	
float peak::get_breadth()
	{
	return breadth_;
	}
	
float peak::get_height()
	{
	return height_;
	}

float peak::get_anzahl()
	{
	return anzahl_;
	}

float peak::get_min()
	{
	return min_;
	}
	
float peak::get_max()
	{
	return max_;
	}

list<PDBAtom*> peak::get_atomlist()
	{
	return atomlist_;
	}

void peak::set_ppm(float wert)
	{
	ppm_=wert;
	}

void peak::set_breadth(float wert)
	{
	breadth_=wert;
	}

void peak::set_anzahl(float wert)
	{
	anzahl_=wert;
	}
	
void peak::set_atomlist(list<PDBAtom*> liste)
	{
	atomlist_=liste;
	}

void peak::add(PDBAtom *proteinatom)
	{
	float wert;
	
	wert=proteinatom->getProperty("chemical_shift").getFloat();
	ppm_=ppm_*anzahl_+wert;
	atomlist_.push_back(proteinatom);
	anzahl_++;
	ppm_/=anzahl_;
	
	if (anzahl_==1)
		{
		min_=wert;
		max_=wert;
		}
		else if (wert<min_) min_=wert;
			else if (wert>max_) max_=wert;
	
	}
	
void peak::set_height(float wert)
	{
	height_=wert;
	}

void peak::set_min(float wert)
	{
	min_=wert;
	}
	
void peak::set_max(float wert)
	{
	max_=wert;
	}

void peak::operator=(peak p)
	{
	min_=p.min_;
	max_=p.max_;
	ppm_=p.ppm_;
	breadth_=p.breadth_;
	height_=p.height_;
	anzahl_=p.anzahl_;
	atomlist_=p.atomlist_;
	}

bool operator==(peak p1,peak p2)
	{
	return (bool) (p1.get_ppm()==p2.get_ppm());
	}

bool operator<(peak p1,peak p2)
	{
	return (bool) (p1.get_ppm()<p2.get_ppm());
	}

ostream& operator<<(ostream &s, peak& p)
	{
	return s << p.get_ppm();
	}

} // namespace Ball

