// $Id: molecularControl.h,v 1.1 2000/09/23 15:38:17 hekl Exp $

#ifndef BALL_MOLVIEW_GUI_WIDGETS_MOLECULARCONTROL_H
#define BALL_MOLVIEW_GUI_WIDGETS_MOLECULARCONTROL_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

#ifndef BALL_KERNEL_ATOM_H
#	include <BALL/KERNEL/atom.h>
#endif

#ifndef BALL_VIEW_GUI_WIDGETS_CONTROL_H
#	include <BALL/VIEW/GUI/WIDGETS/control.h>
#endif

#ifndef BALL_MOLVIEW_FUNCTOR_MOLECULARINFORMATION_H
#	include <BALL/MOLVIEW/FUNCTOR/molecularInformation.h>
#endif

#ifndef BALL_MOLVIEW_FUNCTOR_MOLECULARFILTER_H
#	include <BALL/MOLVIEW/FUNCTOR/molecularFilter.h>
#endif

#ifndef BALL_MOLVIEW_KERNEL_MOLECULARMESSAGE_H
#	include <BALL/MOLVIEW/KERNEL/molecularMessage.h>
#endif


using namespace BALL;
using namespace BALL::VIEW;
using namespace BALL::MOLVIEW;


//namespace BALL
//{

//	namespace MOLVIEW
//	{
		/**
		 */
		class MolecularControl
		: public Control
		{			
			Q_OBJECT
			
		public:
			
			/**	@name	Enums
			 */
			//@{
			//@}
			
			/**	@name	Type Definitions
			 */
			
			/**	@name	Constructors and Destructors
			 */
			//@{
			
			MolecularControl(QWidget* parent = 0, const char* name = 0);
			
			virtual ~MolecularControl();
			//@}
			
			// --- ACCESSORS: INSPECTORS and MUTATORS
			
		public slots:
				
			// override for distinguishing selected composites
			virtual void sentSelection();

		signals:
			
		protected:

		  // returns the actual Information visitor
		  virtual Information& getInformationVisitor_();

		  // recursive iteration methode for "genListViewItem_"
		  // item is the item belonging to the composite parameter
			// all subtrees of composite will be inserted to item
			virtual void recurseGeneration_(QListViewItem* item, Composite* composite);
			
			// recursive iteration methode for "updateListViewItem_"
			// item is the item belonging to the composite parameter
			// all subtrees of composite will be updated below the item
			// returns true, if subtree changed
			virtual bool recurseUpdate_(QListViewItem* item, Composite* composite);
		
			// override this function to catch different messages and 
			// act accordingly 
			// return true if update of control necessary
			virtual bool reactToMessages_(Message* message);
			

		private:
			
			// retrieves information of a given composite (name, type)
			MolecularInformation molecular_information_;

			// tests wether a composite holds the criterias
			MolecularFilter molecular_filter_;
		};

#		ifndef BALL_NO_INLINE_FUNCTIONS
#			include <BALL/MOLVIEW/GUI/WIDGETS/molecularControl.iC>
#		endif

//	} // namespace MOLVIEW

//} // namespace BALL

#endif // BALL_MOLVIE_MOLVIEW_GUI_WIDGETS_MOLECULARCONTROL_H_
