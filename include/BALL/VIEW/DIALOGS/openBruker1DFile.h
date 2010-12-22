// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#ifndef BALL_VIEW_DIALOGS_OPENBRUKER1DFILE_H
#define BALL_VIEW_DIALOGS_OPENBRUKER1DFILE_H

#ifndef BALL_COMMON_GLOBAL_H
# include <BALL/COMMON/global.h>
#endif

#ifndef BALL_COMMON_H
# include <BALL/common.h>
#endif

#ifndef BALL_VIEW_WIDGETS_REGULARDATA1DWIDGET_H
# include <BALL/VIEW/WIDGETS/regularData1DWidget.h>
#endif

#ifndef BALL_FORMAT_BRUKER1DFILE_H
# include <BALL/FORMAT/bruker1DFile.h>
#endif

#include <QtGui/QWidget>

namespace BALL
{
	namespace VIEW
	{
		/** Interface to select and open spectra stored in the bruker file format.
    		\ingroup  ViewDialogs
		*/
		class BALL_VIEW_EXPORT OpenBruker1DFile
			: public QWidget,
				public ModularWidget
		{
			Q_OBJECT
			BALL_EMBEDDABLE(OpenBruker1DFile, ModularWidget)
			public:

			/** Constructors and Destructors
			*/
			//@{
			/// Constructor.
			OpenBruker1DFile(QWidget *parent = 0, const char *name = 0);

			/// Destructor
			virtual ~OpenBruker1DFile();
			//@}

			/** Assignment
			*/

			/** Initialization. This method is called automatically before the main application is started. 
					It adds the	dialog's menu entries and connections.
			*/
			virtual void initializeWidget(MainControl& main_control);

			public slots:

			/** Open the bruker1DFile.
			 */
			virtual void openFile();

		};
	}
}

#endif
