// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: molecularProperties.h,v 1.15 2003/02/21 16:05:27 anhi Exp $

#ifndef BALL_MOLVIEW_GUI_WIDGETS_MOLECULARPROPERTIES_H
#define BALL_MOLVIEW_GUI_WIDGETS_MOLECULARPROPERTIES_H

#ifndef BALL_STRUCTURE_FRAGMENTDB_H
#	include <BALL/STRUCTURE/fragmentDB.h>
#endif

#ifndef BALL_VIEW_GUI_WIDGETS_MODULARWIDGET_H
# include <BALL/VIEW/GUI/WIDGETS/modularWidget.h>
#endif


namespace BALL
{
	using VIEW::ModularWidget;
	using VIEW::Message;
	using VIEW::MainControl;

	namespace MOLVIEW
	{

		/**	The MolecularProperties class.
				The class MolecularProperties is a widget that converts certain  \link Message Message \endlink 
				objects to other  \link Message Message \endlink  objects.
				This class is necessary to convert new  \link Composite Composite \endlink  objects opened with either
				 \link openPDBFile openPDBFile \endlink  or  \link openHINFile openHINFile \endlink  to \Ref{Composite} objects
				that have certain molecular properties. Further it converts the geometric selection
				sent by  \link Scene Scene \endlink  to a molecular selection whose objects can be given a new
				graphical representation by the  \link DisplayProperties DisplayProperties \endlink  dialog.
				See  \link onNotify onNotify \endlink  for information concerning the conversion mechanism.  \par
				<b>Definition:</b> BALL/MOLVIEW/GUI/WIDGETS/molecularProperties.h
		*/
		class MolecularProperties
			: public QWidget, 
				public ModularWidget
		{
			Q_OBJECT
			BALL_EMBEDDABLE(MolecularProperties)

			public:
			
			/**	@name	Constructors
			*/	
			//@{

			/** Default Constructor.
					Constructs new molecularProperties.
					Calls  \link registerWidget registerWidget \endlink .
					@param      parent the parent widget of {\em *this} molecularProperties 
					@param      name the name of {\em *this} molecularProperties 
					@return     MolecularProperties new constructed molecularProperties
					@see        QWidget
					@see        ModularWidget
			*/
			MolecularProperties(QWidget* parent = 0, const char* name = 0)
				throw();
			
			//@}
			/** @name Destructors 
			*/
			//@{

			/** Destructor.
					Default destruction of {\em *this} molecularProperties.
			*/
			virtual ~MolecularProperties()
				throw();

			//@}
			/**	@name	Accessors: inspectors and mutators 
			 */
			//@{

			/** Message handling method.
					Handles messages sent by other registered  \link ConnectionObject ConnectionObject \endlink  objects.
					Converts  \link NewCompositeMessage NewCompositeMessage \endlink  to  \link NewMolecularMessage NewMolecularMessage \endlink  if the
					retrieved  \link Composite Composite \endlink  object is kind of  \link AtomContainer AtomContainer \endlink  and
					applies molecular properties to it (like  \link normalize_names normalize_names \endlink  and
					 \link build_bonds build_bonds \endlink ). \par
					Converts  \link GeometricObjectSelectionMessage GeometricObjectSelectionMessage \endlink  to  \link NewMolecularMessage NewMolecularMessage \endlink 
					if every  \link Composite Composite \endlink  object in the selection has an ancestor that is
					an  \link AtomContainer AtomContainer \endlink . These found ancestors are put into a new selection
					that is sent with the  \link NewMolecularMessage NewMolecularMessage \endlink .
					@param message the pointer to the message that should be processed
					@see   Message
					@see   NewCompositeMessage
					@see   NewMolecularMessage
					@see   GeometricObjectSelectionMessage
					@see   ConnectionObject
		  */
			void onNotify(Message *message)
				throw();


			/**	Check the menu entries.
			 */
			void checkMenu(MainControl& main_control)
				throw();

			public slots:

			/** Centers the camera.
					Centers the camera of  \link Scene Scene \endlink  to the geometric center of the molecular objects
					in the selection list.
					The messages  \link WindowMessage WindowMessage \endlink  and  \link SceneMessage SceneMessage \endlink  will
					be sent to inform the  \link MainControl MainControl \endlink  and the  \link Scene Scene \endlink  about the change.
			*/
			void centerCamera(Composite* composite = 0);

			/** Creates bonds.
					If selected molecular objects are available  \link Bond Bond \endlink  objects will be created
					for each object in the selection list
					using the  \link build_bonds build_bonds \endlink  processor of the  \link FragmentDB FragmentDB \endlink 
					The message  \link ChangedCompositeMessage ChangedCompositeMessage \endlink  will be sent for each object in the
					selection list. The messages  \link WindowMessage WindowMessage \endlink  and  \link SceneMessage SceneMessage \endlink  will
					be sent to inform the  \link MainControl MainControl \endlink  and the  \link Scene Scene \endlink  about the change.
					The number of bonds created will be written into the  \link Log Log \endlink  object.
			*/
			void buildBonds();
			
			/** Adds hydrogens.
					If selected molecular objects are available hydrogens will be created
					for each object in the selection list
					using the  \link add_hydrogens add_hydrogens \endlink  processor of the  \link FragmentDB FragmentDB \endlink 
					The message  \link ChangedCompositeMessage ChangedCompositeMessage \endlink  will be sent for each object in the
					selection list. The messages  \link WindowMessage WindowMessage \endlink  and  \link SceneMessage SceneMessage \endlink  will
					be sent to inform the  \link MainControl MainControl \endlink  and the  \link Scene Scene \endlink  about the change.
					The number of hydrogens created will be written into the  \link Log Log \endlink  object.
			*/
			void addHydrogens();
			

			/** Colors selected objects uniquely.
					If selected molecular objects are available they will be colored according to
					the selected color as specified in  \link GeometricObject GeometricObject \endlink .
					The message  \link ChangedCompositeMessage ChangedCompositeMessage \endlink  will be sent for each object in the
					selection list. The messages  \link WindowMessage WindowMessage \endlink  and  \link SceneMessage SceneMessage \endlink  will
					be sent to inform the  \link MainControl MainControl \endlink  and the  \link Scene Scene \endlink  about the change.
			*/
			void select();

			/** Colors deselected objects in their own color.
					If selected molecular objects are available they will be colored according to
					their own color as specified in the objects. This method reverses the process
					done in the  \link select select \endlink  method.
					The message  \link ChangedCompositeMessage ChangedCompositeMessage \endlink  will be sent for each object in the
					selection list. The messages  \link WindowMessage WindowMessage \endlink  and  \link SceneMessage SceneMessage \endlink  will
					be sent to inform the  \link MainControl MainControl \endlink  and the  \link Scene Scene \endlink  about the change.
			*/
			void deselect();

			/** Check the residues
			 */
			virtual bool checkResidue();

			//@}

			
		private:
			
			virtual void calculateCenter_(Composite& composite);

			FragmentDB fragment_db_;  			

			int center_camera_id_;
			int build_bonds_id_;
			int add_hydrogens_id_;
			int check_structure_id_;
			int select_id_;
			int deselect_id_;
			
			Vector3 										view_center_vector_;
			int 												view_direction_;
			Real 												view_distance_;

		};

	} // namespace MOLVIEW

} // namespace BALL

#endif // BALL_MOLVIE_MOLVIEW_GUI_WIDGETS_MOLECULARPROPERTIES_H_
