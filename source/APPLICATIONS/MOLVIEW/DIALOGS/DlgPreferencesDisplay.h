/**********************************************************************

	--- Qt Architect generated file ---

	File: DlgPreferencesDisplay.h
	Last generated: Fri Jan 14 13:15:10 2000

 *********************************************************************/

#ifndef DlgPreferencesDisplay_included
#define DlgPreferencesDisplay_included

#include "DlgPreferencesDisplayData.h"

#include <BALL/VIEW/DATATYPE/color.h>
#include <BALL/FORMAT/INIFile.h>

using namespace BALL; 
using namespace BALL::VIEW; 

class DlgPreferencesDisplay : public DlgPreferencesDisplayData
{
    Q_OBJECT

public:

    DlgPreferencesDisplay(
        QWidget *parent = NULL,
        const char *name = NULL );

    virtual ~DlgPreferencesDisplay();

		void setPreferences(INIFile& inifile);
		void getPreferences(INIFile& inifile);

		const QColor& getQColor() const;
		ColorRGBA getColor() const;

		bool isDepthCueingEnabled() const;
		bool isCoordinateAxesEnabled() const;
		
protected slots:
		void editColor();
};
#endif // DlgPreferencesDisplay_included
