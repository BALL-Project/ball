// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
#ifndef BALL_VIEW_DIALOGS_BONDPROPERTIES_H
#define BALL_VIEW_DIALOGS_BONDPROPERTIES_H

#include <BALL/VIEW/UIC/bondPropertiesData.h>

#ifndef BALL_KERNEL_ATOM_H
# include <BALL/KERNEL/atom.h>
#endif

#ifndef BALL_KERNEL_BOND_H
# include <BALL/KERNEL/bond.h>
#endif

namespace BALL
{
	namespace VIEW
	{

/** Dialog for showing and changing the properties of the bonds of an atom
    \ingroup  ViewDialogs
*/
class BondProperties 
	: public BondPropertiesData
{ 
	Q_OBJECT

public:
	BondProperties(Atom* atom,  QWidget* parent = 0, const char* name = 0, bool modal = FALSE, WFlags fl = 0 );
	~BondProperties();

public slots:
	void bondSelected();

private:
	Atom* atom_;
};

} } // namespaces

#endif // BALL_VIEW_DIALOGS_BONDPROPERTIES_H
