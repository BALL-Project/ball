// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
#ifndef ATOMPROPERTIES_H
#define ATOMPROPERTIES_H
#include "atomPropertiesData.h"

#ifndef BALL_KERNEL_ATOM_H
# include <BALL/KERNEL/atom.h>
#endif

namespace BALL
{
	namespace MOLVIEW
	{

class AtomProperties : public AtomPropertiesData
{ 
	Q_OBJECT

public:
	AtomProperties(Atom* atom,  QWidget* parent = 0, const char* name = 0, bool modal = FALSE, WFlags fl = 0 );
	~AtomProperties();

public slots:
	void changed();
	void applyClicked();
	void closeClicked();

private:
	Atom* atom_;
};

	}
}

#endif // ATOMProperties_H
