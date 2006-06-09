// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: mainframe.h,v 1.16.2.4.2.1 2006/06/09 15:00:15 leonhardt Exp $
//

#ifndef BALL_APPLICATIONS_BALLVIEW_MAINFRAME_H
#define BALL_APPLICATIONS_BALLVIEW_MAINFRAME_H

#ifndef BALL_VIEW_KERNEL_MAINCONTROL_H
# include <BALL/VIEW/KERNEL/mainControl.h>
#endif

#ifndef BALL_VIEW_WIDGETS_SCENE_H
# include <BALL/VIEW/WIDGETS/scene.h>
#endif

#ifndef BALL_VIEW_DIALOGS_MOLECULARFILEDIALOG_H
# include <BALL/VIEW/DIALOGS/molecularFileDialog.h>
#endif

#include <QtGui/qwidget.h>

namespace BALL
{
	using namespace BALL::VIEW;

	class Mainframe	
		: public BALL::VIEW::MainControl
	{
		Q_OBJECT

		public:

		///
		BALL_EMBEDDABLE(Mainframe, MainControl)

		///
		Mainframe(QWidget* parent = 0, const char* name = 0);

		///
		virtual ~Mainframe() throw();

		public slots:

		///
		void exportPOVRay();

		///
		void printScene();

		/// Catch key events
		void keyPressEvent(QKeyEvent* e);
			
		///
		void toggleFullScreen();

		/// remove all loaded Molecules and Representations, reset Coloring options
		void reset();

		// Help menu
		void about();

		///
		void howToCite();

		/** Open a file.
				Calls MolecularFileDialog::openFile
		*/
		virtual void openFile(const String& file) throw();

		///
 		void saveBALLViewProjectFile() throw();

		///
		void loadBALLViewProjectFile() throw();

		///
		virtual void checkMenus();

		protected:

		Scene*				scene_;
		bool 					fullscreen_;
		QPoint 				last_point_;
		QSize 				last_size_;
		QByteArray 		last_state_;
	};

} // namespace BALL

#endif // BALL_APPLICATIONS_BALLVIEW_MAINFRAME_H
