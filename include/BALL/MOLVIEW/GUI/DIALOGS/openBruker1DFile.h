// $Id: openBruker1DFile.h,v 1.2 2001/05/13 16:32:29 hekl Exp $

#ifndef BALL_MOLVIEW_GUI_DIALOGS_OPENBRUKER1DFILE_H
#define BALL_MOLVIEW_GUI_DIALOGS_OPENBRUKER1DFILE_H

#ifndef BALL_COMMON_H
#       include <BALL/common.h>
#endif

#ifndef BALL_VIEW_GUI_WIDGETS_REGULARDATA1DWIDGET_H
#       include <BALL/VIEW/GUI/WIDGETS/regularData1DWidget.h>
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
  class OpenBruker1DFile
    : public FileDialog
	{
    public:

		// macro for create method
	  //    BALL_CREATE(OpenBruker1DFile)

		/** Constructors and Destructors
		*/
		//@{
      /// Constructor.
    OpenBruker1DFile(QWidget *parent = 0, const char *name = 0);

    /// Destructor
    virtual ~OpenBruker1DFile()
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

    /** Open the bruker1DFile.
     */
    virtual void openFile_()
		throw();

	};


	}
}

#endif
