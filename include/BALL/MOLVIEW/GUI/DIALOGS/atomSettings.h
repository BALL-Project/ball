#ifndef ATOMSETTINGS_H
#define ATOMSETTINGS_H
#include "atomSettingsData.h"

#ifndef BALL_KERNEL_ATOM_H
# include <BALL/KERNEL/atom.h>
#endif

namespace BALL
{
	namespace MOLVIEW
	{

class AtomSettings : public AtomSettingsData
{ 
	Q_OBJECT

public:
	AtomSettings(Atom* atom,  QWidget* parent = 0, const char* name = 0, bool modal = FALSE, WFlags fl = 0 );
	~AtomSettings();

public slots:
	void changed( const QString & );
	void applyClicked();
	void closeClicked();

private:
	bool changed_;
	Atom* atom_;
};

	}
}

#endif // ATOMSETTINGS_H
