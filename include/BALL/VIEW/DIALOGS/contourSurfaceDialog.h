// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
#ifndef BALL_VIEW_DIALOGS_CONTOURSURFACEDIALOG_H
#define BALL_VIEW_DIALOGS_CONTOURSURFACEDIALOG_H

#include <BALL/VIEW/UIC/contourSurfaceDialogData.h>
#include <BALL/SYSTEM/path.h>
#include <BALL/DATATYPE/string.h>

namespace BALL
{
 namespace VIEW
   {

/** ContourSurfaceDialog
    \ingroup  ViewDialogs
 */
class ContourSurfaceDialog 
	: public ContourSurfaceDialogData
{ 
    Q_OBJECT

	public:
    ContourSurfaceDialog( QWidget* parent = 0, const char* name = 0);
    ~ContourSurfaceDialog();

    String getFileName() const;
    double getThreshold() const;

	protected slots: 
    virtual void browseFiles(); 
		virtual void fileNameChanged();
};

} } // namespaces
#endif // CONTOURSURFACEDIALOG_H
