// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: geometricControl.h,v 1.27.2.4 2006/03/15 22:00:24 amoll Exp $

#ifndef BALL_VIEW_WIDGETS_GEOMETRICCONTROL_H
#define BALL_VIEW_WIDGETS_GEOMETRICCONTROL_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

#ifndef BALL_VIEW_WIDGETS_GENERICCONTROL_H
#	include <BALL/VIEW/WIDGETS/genericControl.h>
#endif

#ifndef BALL_VIEW_KERNEL_COMMON_H
# include <BALL/VIEW/KERNEL/common.h>
#endif 


#include <QtCore/qpoint.h>
#include <QtGui/QMenu>

namespace BALL
{
	namespace VIEW
	{
		class Representation;
		class ModifySurfaceDialog;
		class ClippingPlane;
		class TransformationMessage;

		/**	GeometricControl is a widget to display the list of representations.
				There are methods available to modify the representations.
				The method buildContextMenu is a means to 
				create a customizable context menu entries.
				To use this widget in the application just create it with MainControl as
				parent and all in the RepresentationManager created Representation objects will show
				up in the GeometricControl.
				\ingroup ViewWidgets
		*/
		class BALL_VIEW_EXPORT GeometricControl
			: public GenericControl
		{
			Q_OBJECT

			public:
			
			BALL_EMBEDDABLE(GeometricControl, GenericControl)

			/**	@name	Constructors and Destructors
			*/	
			//@{

			/** Default Constructor.
					There are two columns for this control. The <b> Type</b> column and the
					<b> Properties</b> column. The Type column shows the model type (e.g. LineModel),
					while the Properties column contains the number of Composites and GeometricObjects.
					There is a connection from the signal rightButtonClicked from the
					QT-library to the method onContextMenu. See documentation of QT-library 
					for information concerning signal/slot mechanism.
					Calls registerWidget.
					\param      parent the parent widget of this control 
											(See documentation of QT-library for information concerning widgets)
					\param      name the name of this control 
					\see        QListView
					\see        ModularWidget
					\see        onContextMenu
			*/
			GeometricControl(QWidget* parent = 0, const char* name = 0)
				throw();
			
			/// Destructor.
			virtual ~GeometricControl()
				throw();

			/** Explicit default initialization.
					Empty for further purpose.
			*/
			virtual void clear()
				throw() {};

			///
			void initializeWidget(MainControl& main_control)
				throw();
			
			//@} /**	@name	Accessors: inspectors and mutators */ 
			//@{

			/** Insert a Representation.
			*/
			void addRepresentation(Representation& rep)
				throw();

			/** Remove a Representation.
			*/
			void removeRepresentation(Representation& rep)
				throw();

			/** Update a Representation.
			*/
			void updateRepresentation(Representation& rep)
				throw();
			
			/** Mutable inspection of the selection.
			*/
			
			/** Non-mutable inspection of the selection.
			*/
			List<Representation*> getHighlightedRepresentations() const
				throw();
			
			/** Message handling.
					\see    Message
			*/
			virtual void onNotify(Message *message)
				throw();

			/** Build a context menu for the Representation object 
					the QListViewItem object.
					\param   representation the Representation object for that a context menu should be created
					\see     insertContextMenuEntry
					\see     onContextMenu
			*/
			virtual void buildContextMenu()
				throw();

			/// Overloaded from ModularWidget
			virtual void checkMenu(MainControl& main_control)
				throw();

			///
			void moveItems(const TransformationMessage& msg)
				throw();

			void updateClippingPlanes();

			public slots:
				
			//@}
			/** @name Public slots 
			*/ 
			//@{

			/// 
			virtual void selectAtoms();

			///
			virtual void selectedRepresentation(Representation& representation, bool state);

			///
			virtual void updateSelection();

			///
			virtual void focusRepresentation();

			///
			virtual void moveClippingPlane();

			///
			virtual void flipClippingPlane();

			///
			virtual void setClippingPlaneX();

			///
			virtual void setClippingPlaneY();

			///
			virtual void setClippingPlaneZ();

			///
			void hideShowClippingPlane();

			///
			virtual void selectClipRepresentations();

			///
			void createNewClippingPlane();

			/// 
			void duplicateRepresentation();

			///
			void createCoordinateSystem();
			
		  protected slots:
			
			//@} 
			///** @name Protected members */ 
			//@{

			///
			virtual void modifyRepresentation_()
				throw();

			/** Generate the item for a representation.
			*/
			virtual void generateListViewItem_(Representation& rep)
				throw();
			
			/// Overloaded from GenericControl
			virtual void deleteCurrentItems()
				throw();

			//@}
			
		  protected slots:

			virtual void onItemClicked(QTreeWidgetItem* item);
			void showGuestContextMenu(const QPoint& pos);

		  protected:
			
			// only for Python Interface
			GeometricControl(const GeometricControl& control) throw();

			enum ColumnID
			{
				COLUMN_ID__TYPE       = 0,
				COLUMN_ID__Properties
			};

			void setClippingPlane_(const Vector3& n);

			// the context menu
			QMenu 						context_menu_;
			QMenu 						clipping_plane_context_menu_;

			Representation* 	context_representation_;
			ClippingPlane* 		context_plane_;

 			HashMap<Representation*, QTreeWidgetItem*> representation_to_item_;
 			HashMap<ClippingPlane*, QTreeWidgetItem*> plane_to_item_;
 			HashMap<QTreeWidgetItem*, Representation*> item_to_representation_;
 			HashMap<QTreeWidgetItem*, ClippingPlane*> item_to_plane_;

			ModifySurfaceDialog* 	modify_surface_dialog_;

			bool creating_representations_;
			bool ignore_change_;

			QAction* menu_clipping_plane_, *menu_coordinate_;
		};
		
} } // namespaces

#endif // BALL_VIEW_WIDGETS_GEOMETRICCONTROL_H
