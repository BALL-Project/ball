// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: molecularControl.h,v 1.4 2003/09/04 23:14:15 amoll Exp $

#ifndef BALL_VIEW_WIDGETS_MOLECULARCONTROL_H
#define BALL_VIEW_WIDGETS_MOLECULARCONTROL_H

#ifndef BALL_VIEW_WIDGETS_CONTROL_H
#	include <BALL/VIEW/WIDGETS/control.h>
#endif

namespace BALL
{
	namespace VIEW
	{
		/**	MolecularControl is a widget to display the molecular structure of Composite objects. 
				This class is derived from the class Control and extends it for showing and modifiying
				molecular structures. The methods checkMenu() and buildContextMenu() are overridden 
				for performing special molecular tasks.
				\ingroup ViewWidgets
		*/
		class MolecularControl
			: public Control
		{			
			Q_OBJECT
			
			enum MolecularMenuEntries
			{
				CAMERA__CENTER               = 110,
				BONDS__BUILD                 = 120,
				BONDS__REMOVE                = 121,
				RESIDUE__CHECK               = 140,
				DISPLAY__CHANGE              = 150,
				ATOM__PROPERTIES						 = 160,
				SHOW__FILENAME 							 = 170
			};

			public:
			
			/**	@name	Constructors and Destructor
			*/	
			//@{

			/** Default Constructor.
					\param      parent the parent widget 
					\param      name the name of this widget
			*/
			MolecularControl(QWidget* parent = 0, const char* name = 0)
				throw();
			
			/** Destructor.
			*/
			virtual ~MolecularControl()
				throw();
			
			//@} 
			/**	@name	Accessors: inspectors and mutators 
			*/ 
			//@{
			
			/**	Menu checking method.
					This method is called by MainControl::checkMenus before a popup menu is shown.
					The copy menu entry is only enabled for selected System objects.
					\param main_control the MainControl object whose menus should be checked
					\see   Control::checkMenu
			*/
			virtual void checkMenu(VIEW::MainControl& main_control)
				throw();

			/** Builds a context menu with items for molecular tasks.
					Calls Control::buildContextMenu.\par
					\param   composite the Composite for that a context menu should be created
			*/
			virtual void buildContextMenu(Composite& composite)
				throw();

		public slots:
				
			//@}
			/** @name Public slots
			*/
			//@{

			/// Atom properties dialog requested
			void atomProperties();

			/** Check residues
			 */
			void checkResidue();

			/** Build bonds
			 */
			void buildBonds();
			
			/** Center the camera on a composite.
			 */
			void centerCamera();

			/// Show the filename from which the composite was loaded.
			void showFilename();

			/** Prints the filename from which a Systems was loaded, after its selection.\par
			 		Calls showFilename()
			*/
			virtual void updateSelection();

		protected:

			//@}
			/** @name Internal creation and message handling
			*/
			//@{

			/** Message handling.
					Catches the Message objects and reacts accordingly to the different
					messages.
					  -  NewMolecularMessage - update always necessary
					\par
					Calls Control::reactToMessages_()
					\param   message a pointer to the Message object
					\return  bool <tt>true</tt> if an update of the item tree is necessary
					\see     onNotify
			*/
			virtual bool reactToMessages_(VIEW::Message* message)
				throw();
			//@}
		};

}} // namespaces

#endif // BALL_VIEW_WIDGETS_MOLECULARCONTROL_H
