// $Id: control.h,v 1.7 2000/01/14 20:48:38 oliver Exp $

#ifndef BALL_APPLICATIONS_MOLVIEW_CONTROL_H
#define BALL_APPLICATIONS_MOLVIEW_CONTROL_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

#include <iostream>

#include <qlistview.h>
#include <qmessagebox.h>
#include <qpoint.h>
#include <qpopupmenu.h>


#ifndef BALL_CONCEPT_NOTIFICATION_H
#	include <BALL/CONCEPT/notification.h>
#endif

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
        
#ifndef BALL_VIEW_OPENGL_KERNEL_SCENE_H
#	include <BALL/VIEW/OPENGL/KERNEL/scene.h>
#endif

#ifndef BALL_VIEW_OPENGL_KERNEL_COMPOSITEDESCRIPTOR_H
#	include <BALL/VIEW/OPENGL/KERNEL/compositeDescriptor.h>
#endif

#ifndef BALL_MOLVIEW_OPENGL_FUNCTOR_MOLECULEOBJECTPROCESSOR_H
# include <BALL/MOLVIEW/OPENGL/FUNCTOR/moleculeObjectProcessor.h>
#endif

using namespace BALL;
using namespace BALL::VIEW;
using namespace BALL::MOLVIEW;


/**
 */
class Control
  : public QListView
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
			TYPE__ATOM                 = 7
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

	  static QString getTypeName
			(Composite* composite);

	  static Type getType
			(Composite* composite);

	  static QString getName
			(Composite* composite);

		const List<Composite*>& getSelection();

  public slots:

		void ContextMenu
		  (QListViewItem* item, const QPoint& point, int column);

		void invalidateSelection();


  signals:

		void cut();
		void copy();
		void paste();
		void select();
		void deselect();
		void centerCamera();
		void removeObject();

  private:

		QListViewItem*	getRoot_(QListViewItem* item);
		Composite*			getCompositeAddress_(QListViewItem* item);
		QString					getRootName_(QListViewItem* item);
		QString					getName_(QListViewItem* item);
		QString					getTypeName_(QListViewItem* item);
		QString					getRootTypeName_(QListViewItem* item);

	  void genListViewItem_
			(QListViewItem* item, Composite* composite, QString* default_name = 0);

	  QListViewItem* findListViewItem_
			(Composite* composite);

		// ATRTIBUTES

		List<Composite*> selected_;

		bool selection_changed_;
};


#		ifndef BALL_NO_INLINE_FUNCTIONS
#			include "control.iC"
#		endif

#endif // BALL_MOLVIE_APPLICATIONS_MOLVIEW_CONTROL_H_
