// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: openBruker1DFile.h,v 1.5 2003/09/04 22:17:47 amoll Exp $

#ifndef BALL_VIEW_DIALOGS_OPENBRUKER1DFILE_H
#define BALL_VIEW_DIALOGS_OPENBRUKER1DFILE_H

#ifndef BALL_COMMON_H
# include <BALL/common.h>
#endif

#ifndef BALL_VIEW_WIDGETS_REGULARDATA1DWIDGET_H
# include <BALL/VIEW/WIDGETS/regularData1DWidget.h>
#endif

#ifndef BALL_FORMAT_BRUKER1DFILE_H
# include <BALL/FORMAT/bruker1DFile.h>
#endif

#include <qwidget.h>

namespace BALL
{
	namespace VIEW
	{
		/** Interface to select and open spectra stored in the bruker file format.
    		\ingroup  ViewDialogs
		*/
		class OpenBruker1DFile
			: public QWidget,
				public ModularWidget
		{
			Q_OBJECT
			BALL_EMBEDDABLE(OpenBruker1DFile)
			public:

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


			protected slots:

			/** Open the bruker1DFile.
			 */
			virtual void openFile_()
				throw();

		};
	}
}

#endif
