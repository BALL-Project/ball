/**********************************************************************

	--- Qt Architect generated file ---

	File: DlgPreferences.h
	Last generated: Thu Jan 13 14:59:16 2000

 *********************************************************************/

#ifndef DlgPreferences_included
#define DlgPreferences_included

#include "DlgPreferencesData.h"
#include <BALL/FORMAT/INIFile.h>

using namespace BALL;

class DlgPreferences : public DlgPreferencesData
{
    Q_OBJECT

public:

    DlgPreferences(
        QWidget *parent = NULL,
        const char *name = NULL );

    virtual ~DlgPreferences();

		void setPreferences(INIFile& inifile);
		void getPreferences(INIFile& inifile);
};
#endif // DlgPreferences_included
