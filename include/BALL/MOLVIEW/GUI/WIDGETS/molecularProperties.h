// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: molecularProperties.h,v 1.13 2002/12/18 16:00:29 sturm Exp $

#ifndef BALL_MOLVIEW_GUI_WIDGETS_MOLECULARPROPERTIES_H
#define BALL_MOLVIEW_GUI_WIDGETS_MOLECULARPROPERTIES_H

#ifndef BALL_STRUCTURE_FRAGMENTDB_H
#	include <BALL/STRUCTURE/fragmentDB.h>
#endif

#ifndef BALL_VIEW_GUI_WIDGETS_MODULARWIDGET_H
# include <BALL/VIEW/GUI/WIDGETS/modularWidget.h>
#endif

using namespace BALL::VIEW;

namespace BALL
{
	namespace MOLVIEW
	{

		/**	The MolecularProperties class.
				The class MolecularProperties is a widget that converts certain \Ref{Message}
				objects to other \Ref{Message} objects.
				This class is necessary to convert new \Ref{Composite} objects opened with either
				\Ref{openPDBFile} or \Ref{openHINFile} to \Ref{Composite} objects
				that have certain molecular properties. Further it converts the geometric selection
				sent by \Ref{Scene} to a molecular selection whose objects can be given a new
				graphical representation by the \Ref{DisplayProperties} dialog.
				See \Ref{onNotify} for information concerning the conversion mechanism. \\
				{\bf Definition:} \URL{BALL/MOLVIEW/GUI/WIDGETS/molecularProperties.h}
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
					Calls \Ref{registerWidget}.
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
					Handles messages sent by other registered \Ref{ConnectionObject} objects.
					Converts \Ref{NewCompositeMessage} to \Ref{NewMolecularMessage} if the
					retrieved \Ref{Composite} object is kind of \Ref{AtomContainer} and
					applies molecular properties to it (like \Ref{normalize_names} and
					\Ref{build_bonds}).\\
					Converts \Ref{GeometricObjectSelectionMessage} to \Ref{NewMolecularMessage}
					if every \Ref{Composite} object in the selection has an ancestor that is
					an \Ref{AtomContainer}. These found ancestors are put into a new selection
					that is sent with the \Ref{NewMolecularMessage}.
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
					Centers the camera of \Ref{Scene} to the geometric center of the molecular objects
					in the selection list.
					The messages \Ref{WindowMessage} and \Ref{SceneMessage} will
					be sent to inform the \Ref{MainControl} and the \Ref{Scene} about the change.
			*/
			void centerCamera(Composite* composite = 0);

			/** Creates bonds.
					If selected molecular objects are available \Ref{Bond} objects will be created
					for each object in the selection list
					using the \Ref{build_bonds} processor of the \Ref{FragmentDB}
					The message \Ref{ChangedCompositeMessage} will be sent for each object in the
					selection list. The messages \Ref{WindowMessage} and \Ref{SceneMessage} will
					be sent to inform the \Ref{MainControl} and the \Ref{Scene} about the change.
					The number of bonds created will be written into the \Ref{Log} object.
			*/
			void buildBonds();
			
			/** Adds hydrogens.
					If selected molecular objects are available hydrogens will be created
					for each object in the selection list
					using the \Ref{add_hydrogens} processor of the \Ref{FragmentDB}
					The message \Ref{ChangedCompositeMessage} will be sent for each object in the
					selection list. The messages \Ref{WindowMessage} and \Ref{SceneMessage} will
					be sent to inform the \Ref{MainControl} and the \Ref{Scene} about the change.
					The number of hydrogens created will be written into the \Ref{Log} object.
			*/
			void addHydrogens();
			

			/** Colors selected objects uniquely.
					If selected molecular objects are available they will be colored according to
					the selected color as specified in \Ref{GeometricObject}.
					The message \Ref{ChangedCompositeMessage} will be sent for each object in the
					selection list. The messages \Ref{WindowMessage} and \Ref{SceneMessage} will
					be sent to inform the \Ref{MainControl} and the \Ref{Scene} about the change.
			*/
			void select();

			/** Colors deselected objects in their own color.
					If selected molecular objects are available they will be colored according to
					their own color as specified in the objects. This method reverses the process
					done in the \Ref{select} method.
					The message \Ref{ChangedCompositeMessage} will be sent for each object in the
					selection list. The messages \Ref{WindowMessage} and \Ref{SceneMessage} will
					be sent to inform the \Ref{MainControl} and the \Ref{Scene} about the change.
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
