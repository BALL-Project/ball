// $Id: control.h,v 1.1 2000/09/23 15:38:16 hekl Exp $

#ifndef BALL_MOLVIEW_GUI_WIDGETS_CONTROL_H
#define BALL_MOLVIEW_GUI_WIDGETS_CONTROL_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

#include <iostream>

#include <qlistview.h>
#include <qmessagebox.h>
#include <qpoint.h>
#include <qpopupmenu.h>


#ifndef BALL_CONCEPT_COMPOSITE_H
#	include <BALL/CONCEPT/composite.h>
#endif

#ifndef BALL_KERNEL_CHAIN_H
#	include <BALL/KERNEL/chain.h>
#endif

#ifndef BALL_KERNEL_FRAGMENT_H
#	include <BALL/KERNEL/fragment.h>
#endif

#ifndef BALL_KERNEL_MOLECULE_H
#	include <BALL/KERNEL/molecule.h>
#endif

#ifndef BALL_KERNEL_PROTEIN_H
# include <BALL/KERNEL/protein.h>
#endif

#ifndef BALL_KERNEL_RESIDUE_H
#	include <BALL/KERNEL/residue.h>
#endif

#ifndef BALL_KERNEL_SYSTEM_H
# include <BALL/KERNEL/system.h>
#endif
        
#ifndef BALL_VIEW_KERNEL_CONNECTIONOBJECT_H
#	include <BALL/VIEW/KERNEL/connectionObject.h>
#endif

#ifndef BALL_VIEW_KERNEL_GEOMETRICOBJECT_H
#	include <BALL/VIEW/KERNEL/geometricObject.h>
#endif

#ifndef BALL_MOLVIEW_FUNCTOR_MOLECULARINFORMATION_H
#	include <BALL/MOLVIEW/FUNCTOR/molecularInformation.h>
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
		class Control
			: public QListView,
				public ConnectionObject
		{
			
			Q_OBJECT
			
		public:
			
			/**	@name	Enums
			 */
			//@{
			enum Type
			{
			  TYPE__UNKNOWN              = -1,
				TYPE__SYSTEM               = 0,
				TYPE__PROTEIN              = 1,
				TYPE__MOLECULE             = 2,
				TYPE__CHAIN                = 3,
				TYPE__FRAGMENT             = 4,
				TYPE__RESIDUE              = 5,
				TYPE__SECONDARY_STRUCTURE  = 6,
				TYPE__ATOM                 = 7,
				TYPE__GEOMETRIC_OBJECT     = 20
			};
			
			enum ColumnID
			{
			  COLUMN_ID__NAME      = 0,
				COLUMN_ID__TYPE      = 1,
				COLUMN_ID__ADDRESS   = 6
			};
			//@}
			
			/**	@name	Type Definitions
			 */
			
			/**	@name	Constructors and Destructors
			 */
			//@{
			
			Control(QWidget* parent = 0, const char* name = 0);
			
			virtual ~Control();
			//@}
			
			// --- ACCESSORS: INSPECTORS and MUTATORS
			
			void updateContent();
			
			bool addComposite
				(Composite* composite, QString* name = 0);
			
			bool removeComposite
				(Composite* composite);
			
			bool updateComposite
				(Composite* composite);
			
			const List<Composite*>& getSelection();
			
			void onNotify(Message *message);
			
		public slots:
				
			void ContextMenu
		    (QListViewItem* item, const QPoint& point, int column);
		
			void invalidateSelection();
			void newSelection();
			
			void cut();
			void copy();
			void paste();
			void clearClipboard();
			
		signals:
			
		private:
			
			QListViewItem*	getRoot_(QListViewItem* item);
			Composite*			getCompositeAddress_(QListViewItem* item);
			QString					getRootName_(QListViewItem* item);
			QString					getName_(QListViewItem* item);
			QString					getTypeName_(QListViewItem* item);
			QString					getRootTypeName_(QListViewItem* item);
			
			void genListViewItem_
				(QListViewItem* item, Composite* composite, QString* default_name = 0);
			
			void updateListViewItem_
				(QListViewItem* item, Composite* composite, QString* default_name = 0);
			
			QListViewItem* findListViewItem_
				(Composite* composite);
			
			// ATRTIBUTES
			
			List<Composite*> selected_;
			List<Composite*> copy_list_;
			
			bool selection_changed_;
		};

#		ifndef BALL_NO_INLINE_FUNCTIONS
#			include <BALL/MOLVIEW/GUI/WIDGETS/control.iC>
#		endif

//	} // namespace MOLVIEW

//} // namespace BALL

#endif // BALL_MOLVIE_MOLVIEW_GUI_WIDGETS_CONTROL_H_
