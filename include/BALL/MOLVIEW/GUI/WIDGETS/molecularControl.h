// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: molecularControl.h,v 1.16 2003/03/26 13:56:54 anhi Exp $

#ifndef BALL_MOLVIEW_GUI_WIDGETS_MOLECULARCONTROL_H
#define BALL_MOLVIEW_GUI_WIDGETS_MOLECULARCONTROL_H

#ifndef BALL_VIEW_GUI_WIDGETS_CONTROL_H
#	include <BALL/VIEW/GUI/WIDGETS/control.h>
#endif

#ifndef BALL_MOLVIEW_FUNCTOR_MOLECULARINFORMATION_H
#	include <BALL/MOLVIEW/FUNCTOR/molecularInformation.h>
#endif

#ifndef BALL_MOLVIEW_FUNCTOR_MOLECULARFILTER_H
#	include <BALL/MOLVIEW/FUNCTOR/molecularFilter.h>
#endif

#ifndef BALL_KERNEL_ATOM_H
#	include <BALL/KERNEL/atom.h>
#endif

namespace BALL
{
	namespace MOLVIEW
	{
		using VIEW::Control;

		/**	The MolecularControl class.
				The MolecularControl class is a widget to display the molecular structure of 
				 \link Composite Composite \endlink 	objects. 
				This class is derived from the class  \link Control Control \endlink  and extends it for showing
				molecular structures. The method  \link checkMenu checkMenu \endlink  is overridden to enable and
				disable the menu entries properly. The cut, copy and paste menus will work
				only with  \link System System \endlink  objects. Further the internal selection mechanism
				is reimplemented to work on molecular objects instead of  \link GeometricObject GeometricObject \endlink 
				objects.  \par
				
    		\ingroup  MolviewGuiWidgets
		*/
		class MolecularControl
			: public VIEW::Control
		{			
			Q_OBJECT
			
			enum MenuEntries
			{
				OBJECT__REMOVE               = 0,
				OBJECT__CUT                  = 1,
				OBJECT__COPY                 = 2,
				OBJECT__PASTE                = 3,
				CAMERA__CENTER               = 10,
				BONDS__BUILD                 = 20,
				BONDS__REMOVE                = 21,
				SELECT                       = 30,
				DESELECT                     = 31,
				RESIDUE__CHECK               = 40,
				DISPLAY__CHANGE              = 50,
				ATOM__PROPERTIES						 = 60
			};

			public:
			
			/**	@name	Constructors
			*/	
			//@{

			/** Default Constructor.
					Constructs new molecularControl.
					@param      parent the parent widget of {\em *this} molecularControl 
					@param      name the name of {\em *this} molecularControl 
					@return     MolecularControl new constructed molecularControl
					@see        Control
			*/
			MolecularControl(QWidget* parent = 0, const char* name = 0)
				throw();
			
			//@}
			/** @name Destructors 
			*/
			//@{

			/** Destructor.
					Default destruction of {\em *this} molecularControl.
			*/
			virtual ~MolecularControl()
				throw();
			
			//@}
			/**	@name	Accessors: inspectors and mutators 
			*/
			//@{
			/**	Menu checking method.
					This method is called by the method  \link checkMenus checkMenus \endlink  from the
					 \link MainControl MainControl \endlink  object before a popup menu is shown.
					The menus  \link cut cut \endlink ,  \link copy copy \endlink ,  \link paste paste \endlink  and \Ref{clearClipboard} will
					be enabled and disabled as molecular structures are selected.  \link Paste Paste \endlink 
					and  \link clearClipboard clearClipboard \endlink  will only be enabled if  \link System System \endlink  objects
					were cutted or copied before.
					The  \link cut cut \endlink  and  \link copy copy \endlink  menu entry are only enabled if all selected objects
					were  \link System System \endlink  objects, disabled otherwise.				
					@param main_control the  \link MainControl MainControl \endlink  object whose menus should be checked
					@see   Control::checkMenu
					@see   show
					@see   checkMenus
			*/
			virtual void checkMenu(VIEW::MainControl& main_control)
				throw();

			/** Builds a context menu.
					Calls  \link Control::buildContextMenu Control::buildContextMenu \endlink . \par
					<b>Note:</b> context menus for molecular structures must be added.
					@param   composite the  \link Composite Composite \endlink  object for that a context menu should be created
					@param   item the  \link QListViewItem QListViewItem \endlink  object for that a context menu should be created
					@see     Control::buildContextMenu
			*/
			virtual void buildContextMenu(Composite* composite, QListViewItem* item)
				throw();

		public slots:
				
			//@}
			/** @name Public slots
			*/
			//@{
			/** Sents the selection.
					Calls  \link Control::sentSelection Control::sentSelection \endlink .
					Filters the selection with the  \link MolecularFilter MolecularFilter \endlink  and sents a message
					 \link MolecularSelectionMessage MolecularSelectionMessage \endlink  through the  \link ConnectionObject ConnectionObject \endlink  tree.
					@see  getSelection
					@see  ConnectionObject
					@see  MolecularFilter
					@see  MolecularSelectionMessage
					@see  notify_
			*/
			virtual void sentSelection();

			/** Atom properties dialog requested */
			void atomProperties();

			/** Check residues
			 */
			void checkResidue();

			/** Select 
			 */
			void select();
			
			/** Deselect
			 */
			void deselect();
			
			/** Build bonds
			 */
			void buildBonds();
			
			/** Center the camera on a composite.
			 */
			void centerCamera();

		protected:

			//@}
			/** @name Internal creation and message handling
			*/
			//@{
			/** Accesses the molecular information visitor.
					Accesses the  \link MolecularInformation MolecularInformation \endlink  visitor of {\em *this} molecularControl.
					This method is used in the method  \link generateListViewItem_ generateListViewItem_ \endlink  to
					retrieve certain information of the given  \link Composite Composite \endlink  object.
					@return  Information a reference to a  \link Information Information \endlink  visitor.
					@see     MolecularInformation
					@see     generateListViewItem_
			*/
		  virtual VIEW::Information& getInformationVisitor_()
				throw();

			/** Recursive iteration method.
					Iterates over the children of the  \link Composite Composite \endlink  {\em composite} and
					calls for each of these children the method  \link generateListViewItem_ generateListViewItem_ \endlink .
					@param   item a pointer to a  \link QListViewItem QListViewItem \endlink  to which all children of {\em composite} will be inserted
					@param   composite a pointer to a  \link Composite Composite \endlink  object whose children will be inserted into {\em item}
					@see     generateListViewItem_
			*/
			virtual void recurseGeneration_(QListViewItem* item, Composite* composite)
				throw();
			
			/** Recursive update method.
					Iterates over the children of the  \link Composite Composite \endlink  {\em composite} and
					calls for each of these children the method  \link updateListViewItem_ updateListViewItem_ \endlink .
					@param   item a pointer to a  \link QListViewItem QListViewItem \endlink  containing the subtree structure 
					@param   composite a pointer to a  \link Composite Composite \endlink  object containing the (possibly) new substructure
					@return  bool <tt>true</tt> if the subtree structure of {\em composite} and the subtree structure of 
												{\em item} are unequal, <tt>false</tt> otherwise 
					@see     updateListViewItem_
			*/
			virtual bool recurseUpdate_(QListViewItem* item, Composite* composite)
				throw();
		
			/** Message handling.
					Catches the  \link Message Message \endlink  objects and reacts accordingly to the different
					messages.
			
					  -   \link NewMolecularMessage NewMolecularMessage \endlink  - update always necessary
					  -   \link RemovedCompositeMessage RemovedCompositeMessage \endlink  - update necessary if already inserted
					  -   \link ChangedCompositeMessage ChangedCompositeMessage \endlink  - update necessary if  \link Composite Composite \endlink  object has changed
					
					@param   message a pointer to the  \link Message Message \endlink  object
					@return  bool <tt>true</tt> if an update of {\em *this} molecularControl is necessary, <tt>false</tt> otherwise
					@see     onNotify
			*/
			virtual bool reactToMessages_(VIEW::Message* message)
				throw();
			//@}
			

		private:
			
			// retrieves information of a given composite (name, type)
			MolecularInformation molecular_information_;

			// tests wether a composite holds the criterias
			MolecularFilter molecular_filter_;
		};
	} // namespace MOLVIEW

} // namespace BALL

#endif // BALL_MOLVIE_MOLVIEW_GUI_WIDGETS_MOLECULARCONTROL_H_
