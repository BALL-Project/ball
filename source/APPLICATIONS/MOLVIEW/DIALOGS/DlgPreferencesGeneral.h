/**********************************************************************

	--- Qt Architect generated file ---

	File: DlgPreferencesGeneral.h
	Last generated: Thu Jan 13 15:01:42 2000

 *********************************************************************/

#ifndef DlgPreferencesGeneral_included
#define DlgPreferencesGeneral_included

#include "DlgPreferencesGeneralData.h"
#include <BALL/FORMAT/INIFile.h>

using namespace BALL; 

class DlgPreferencesGeneral : public DlgPreferencesGeneralData
{
    Q_OBJECT

public:

    DlgPreferencesGeneral(
        QWidget *parent = NULL,
        const char *name = NULL );

    virtual ~DlgPreferencesGeneral();

		QStyle* getStyle();

		void setPreferences(INIFile& inifile);
		void getPreferences(INIFile& inifile);
};
#endif // DlgPreferencesGeneral_included
