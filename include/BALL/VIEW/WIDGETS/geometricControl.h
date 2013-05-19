// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

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

#include <QtCore/QPoint>
#include <QtGui/QMenu>

namespace BALL
{
	namespace VIEW
	{
		class Representation;
		class ModifyRepresentationDialog;
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
			GeometricControl(QWidget* parent = 0, const char* name = 0);
			
			/// Destructor.
			virtual ~GeometricControl();

			/** Explicit default initialization.
					Empty for further purpose.
			*/
			virtual void clear(){};

			///
			virtual void initializeWidget(MainControl& main_control);
			
			//@} /**	@name	Accessors: inspectors and mutators */ 
			//@{

			/** Insert a Representation.
			*/
			virtual void addRepresentation(Representation& rep);

			/** Remove a Representation.
			*/
			virtual void removeRepresentation(Representation& rep);

			/** Update a Representation.
			*/
			virtual void updateRepresentation(Representation& rep, bool force=false);
			
			/** Mutable inspection of the selection.
			*/
			
			/** Non-mutable inspection of the selection.
			*/
			std::list<Representation*> getHighlightedRepresentations() const;
			
			/** Message handling.
					\see    Message
			*/
			virtual void onNotify(Message *message);

			/** Build a context menu for the Representation object 
					the QListViewItem object.
					\param   representation the Representation object for that a context menu should be created
					\see     insertContextMenuEntry
					\see     onContextMenu
			*/
			virtual void buildContextMenu();

			/// Overloaded from ModularWidget
			virtual void checkMenu(MainControl& main_control);

			///
			void moveItems(const TransformationMessage& msg);

			void updateClippingPlanes();

			public slots:
				
			//@}
			/** @name Public slots 
			*/ 
			//@{
			
			///
			virtual void renameRepresentation();

			/// 
			virtual void selectAtoms();

			///
			virtual void selectedRepresentation(Representation& representation, bool state);

			///
			virtual void updateSelection();

			///
			virtual void focus();

			///
			virtual void enterMoveMode();
			
			///
			virtual void flipClippingCapping();

			///
			virtual void flipClippingPlane();
			
			///
			virtual void setClippingPosition();

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
			void duplicate();

			///
			void saveSurface();

			///
			void loadSurface();

			///
			ModifyRepresentationDialog* getModifySurfaceDialog();

		  protected slots:
			
			//@} 
			///** @name Protected members */ 
			//@{

			///
			virtual void modifyRepresentation_();

			/** Generate the item for a representation.
			*/
			virtual void generateListViewItem_(Representation& rep);
			
			/// Overloaded from GenericControl
			virtual void deleteCurrentItems();

			///
			virtual void setupProximityLight();

			//@}
			
		  protected slots:

			virtual void onItemClicked(QTreeWidgetItem* item, int col);
			void showGuestContextMenu(const QPoint& pos);

		  protected:

			void addItem_(const String& text, const char* member, QWidget* widget = 0);
			
			// only for Python Interface
			GeometricControl(const GeometricControl& control);

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

			ModifyRepresentationDialog* 	modify_rep_dialog_;

			bool creating_representations_;
			bool ignore_change_;

			QAction* menu_clipping_plane_, *menu_load_surface_, *modify_surface_;
			vector<QAction*> context_menu_actions_;
		};
		
} } // namespaces

#endif // BALL_VIEW_WIDGETS_GEOMETRICCONTROL_H
