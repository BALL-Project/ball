// $Id: openBruker2DFile.h,v 1.5 2001/12/28 02:33:38 oliver Exp $

#ifndef BALL_MOLVIEW_GUI_DIALOGS_OPENBRUKER2DFILE_H
#define BALL_MOLVIEW_GUI_DIALOGS_OPENBRUKER2DFILE_H

#ifndef BALL_COMMON_H
#       include <BALL/common.h>
#endif

#ifndef BALL_VIEW_GUI_WIDGETS_REGULARDATA2DWIDGET_H
#       include <BALL/VIEW/GUI/WIDGETS/regularData2DWidget.h>
#endif

#ifndef BALL_VIEW_GUI_DIALOGS_FILEDIALOG_H
#       include <BALL/VIEW/GUI/DIALOGS/fileDialog.h>
#endif

using namespace std;
using namespace BALL;
using namespace BALL::VIEW;

namespace BALL
{

	namespace MOLVIEW
	{

	/** This class provides an interface to select and open spectra stored in the bruker file format.
	*/
  class OpenBruker2DFile
    : public FileDialog
	{
    public:
		
		BALL_EMBEDDABLE(OpenBruker2DFile)

		/** Constructors and Destructors
		*/
		//@{
    /// Constructor.
    OpenBruker2DFile(QWidget *parent = 0, const char *name = 0);

    /// Destructor
    virtual ~OpenBruker2DFile()
			throw();
		//@}

		/** Assignment
		*/

    /** Initialization. This method is called automatically before the main application is started. 
				It adds the	dialog's menu entries and connections.
    */
    virtual void initializeWidget(MainControl& main_control)
		throw();

    /** Remove the dialog.
				This method is called by the dialog's destructor.
				It reverses all actions performed in 
				initializeWidget (remove menu entries and connections).
    */
    virtual void finalizeWidget(MainControl& main_control)
		throw();


		protected:

    /** Open the bruker2DFile.
     */
    virtual void openFile_()
		throw();

	};


	}
}

#endif
