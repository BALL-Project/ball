/**********************************************************************

	--- Qt Architect generated file ---

	File: DlgPreferencesNetwork.h
	Last generated: Fri Jan 14 11:14:45 2000

 *********************************************************************/

#ifndef DlgPreferencesNetwork_included
#define DlgPreferencesNetwork_included

#include "DlgPreferencesNetworkData.h"
#include <BALL/FORMAT/INIFile.h>

using namespace BALL;

class DlgPreferencesNetwork : public DlgPreferencesNetworkData
{
    Q_OBJECT

public:

    DlgPreferencesNetwork(
        QWidget *parent = NULL,
        const char *name = NULL );

    virtual ~DlgPreferencesNetwork();

		void setPreferences(INIFile& inifile);
		void getPreferences(INIFile& inifile);

		int getPort();

		bool getServerStatus();

public slots:
		void resetPort();
};
#endif // DlgPreferencesNetwork_included
