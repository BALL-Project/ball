// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
#ifndef BALL_VIEW_DIALOGS_ATOMPROPERTIES_H
#define BALL_VIEW_DIALOGS_ATOMPROPERTIES_H

#include <BALL/VIEW/UIC/atomPropertiesData.h>

#ifndef BALL_KERNEL_ATOM_H
# include <BALL/KERNEL/atom.h>
#endif

namespace BALL
{
	namespace VIEW
	{

/** AtomProperties
    \ingroup  MolviewDialogs
*/
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

#endif // BALL_VIEW_DIALOGS_ATOMPROPERTIES_H
