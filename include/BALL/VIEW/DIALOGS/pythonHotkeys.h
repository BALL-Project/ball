// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: pythonHotkeys.h,v 1.3 2004/10/08 14:49:20 amoll Exp $
//

#ifndef BALL_VIEW_DIALOGS_PYTHONHOTKEYS_H
#define BALL_VIEW_DIALOGS_PYTHONHOTKEYS_H

#include <BALL/VIEW/UIC/pythonHotkeysData.h>

#ifndef BALL_VIEW_WIDGETS_PYWIDGET_H
 #include <BALL/VIEW/WIDGETS/pyWidget.h>
#endif
			
#ifndef BALL_VIEW_KERNEL_PREFERENCESENTRY
# include <BALL/VIEW/KERNEL/preferencesEntry.h>
#endif

namespace BALL
{
	namespace VIEW
	{

		/** Dialog for setting up the Hotkeys for Python commands.
		 		It is inserted to the Preferences.
				\ingroup ViewDialogs
		*/
		class PythonHotkeys 
			: public PythonHotkeysData,
				public PreferencesEntry
		{ 
			Q_OBJECT

			public:

			/// Constructor
			PythonHotkeys( QWidget* parent = 0, const char* name = 0, WFlags fl = 0 );

			/// Destructor
			~PythonHotkeys() {}

			/// 
			virtual void setDefaults(bool /*all*/ = true)
				throw();

			///
			void setContent(const List<Hotkey>& hotkeys)
				throw();

			///
			List<Hotkey> getContent() const
				throw();

			public slots:

			///
			virtual void removePressed();
			
			///
			virtual void newPressed();
			
			///
			virtual void rowSelected();

			protected:
			
			void addEmtpyLine_() throw();
			
			QStringList modifier_, keys_;
		};

} }

#endif
