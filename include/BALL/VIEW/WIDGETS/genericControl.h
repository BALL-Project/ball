// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: genericControl.h,v 1.1 2003/09/07 23:19:41 amoll Exp $

#ifndef BALL_VIEW_WIDGETS_GenericControl_H
#define BALL_VIEW_WIDGETS_GenericControl_H

#ifndef BALL_VIEW_KERNEL_MODULARWIDGET_H
#	include <BALL/VIEW/KERNEL/modularWidget.h>
#endif

#ifndef BALL_VIEW_KERNEL_COMMON_H
//# include <BALL/VIEW/KERNEL/common.h>
#endif

#include <BALL/VIEW/UIC/genericControlData.h>

namespace BALL
{
	namespace VIEW
	{
		/**	GenericControl is a widget to display the structure of Composite objects. 
		 		It uses the datastructure QListView from the QT-libary.
				There are two columns. The <b>Name</b> column and the
				<b>Type</b> column. In the Name column the item tree will be shown and in 
				the Type column the type of each item.
		    There are methods available to change the structure as well as
				copy or paste objects into the GenericControl.
				Various virtual methods can be overridden to customize the behavior of these
				structure changing methods. 
				The method buildContextMenu() can create a customizable context menu.
				To use this widget in the application just create it with MainGenericControl as
				parent and all inserted Composite objects sent with NewCompositeMessage
				will be shown in the structure view of the GenericControl.
				\ingroup ViewWidgets
		*/
		class GenericControl
			: public GenericControlData, 
				public ModularWidget
		{
			Q_OBJECT

			public:

			/** @name Macros.
		  */
			//@{

			/** Embeddable Macro.
			*/
			BALL_EMBEDDABLE(GenericControl)

			//@}
			/**	@name	Constructors and Destructors
			*/	
			//@{

			/** Default Constructor.
					There is a connection from the signal rightButtonClicked from the
					QT-library to the method onContextMenu(). 
					(See documentation of QT-library for information concerning widgets and 
					signal/slot mechanism.) \par
					Calls registerWidget().
					\param      parent the parent widget of the GenericControl 
					\param      name the name of the GenericControl 
					\see        ModularWidget
			*/
			GenericControl(QWidget* parent = 0, const char* name = 0)
				throw();
			
			/** Destructor.
			*/
			virtual ~GenericControl()
				throw();

			//@} 
			/**	@name	Accessors: inspectors and mutators 
			*/ 
			//@{

			/**	Initialize the widget.
			*/
			virtual void initializeWidget(MainControl& main_control)
				throw();

			/**	Remove the widget.
					Reverse all actions performed in initializeWidget
					(remove menu entries of this GenericControl).
					This method will be called by MainGenericControl::aboutToExit.
					\param main_GenericControl the MainTrajectoryControl object to be finalized with this TrajectoryControl
			*/
			virtual void finalizeWidget(MainControl& main_control)
				throw();

			///
			virtual void fetchPreferences(INIFile& inifile)
				throw();
			
			///
			virtual void writePreferences(INIFile& inifile)
				throw();

			//@}
			/** @name Public slots 
			*/ 
			//@{
			public slots:

			/// Show or hide widget (Called by menu entry in "WINDOWS")
			void switchShowWidget()
				throw();

			//@} 

		  protected slots:
			
			void onContextMenu_(QListViewItem* item, const QPoint& point, int column){};

		  protected:
			QListViewItem* 								context_item_;
		};
		
} } // namespaces
#endif // BALL_VIEW_WIDGETS_GenericControl_H
