// $Id: control.h,v 1.2 2000/01/08 20:34:07 hekl Exp $

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
        
#ifndef BALL_STRUCTURE_RESIDUECHECKER_H
# include <BALL/STRUCTURE/residueChecker.h>
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


#define CONTROL__TYPE_AND_NAME \
selected_type__mQString_.ascii(), selected_name__mQString_.ascii()


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
			TYPE__UNKOWN               = -1,
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

		Control
		  (QWidget* parent__pQWidget = NULL,
			 const char* name__pc = NULL);
	
		virtual ~Control();
 		//@}
		
		// --- CONVERTERS
		
		// --- ASSIGNMENT METHODS
		
		void setScene
			(const Scene& __rScene);

		Scene* getScene
			(void) const;

		void setMoleculeObjectProcessor
			(const MoleculeObjectProcessor &__rMoleculeObjectProcessor);

		MoleculeObjectProcessor* getMoleculeObjectProcessor
			(void) const;

		// --- ACCESSORS: INSPECTORS and MUTATORS
		
		void updateContent
			(void);

		bool addComposite
			(Composite *__pComposite,
			 QString *name__pQString = 0);

	  bool removeComposite
			(Composite *__pComposite);

		// --- DEBUGGERS and DIAGNOSTICS

		// --- STORERS
		
		// --- EXTERNAL ITERATORS

	  static QString getTypeName
			(Composite *__pComposite);

	  static Type getType
			(Composite *__pComposite);

	  static QString getName
			(Composite *__pComposite);


		void outputStatus
			(QString message__QString, 
			 bool prefix__bool = true, 
			 bool composed_prefix__bool = true);

		// --- DEBUGGERS and DIAGNOSTICS

		// --- STORERS
		
		// --- EXTERNAL ITERATORS


  public slots:

		void ContextMenu
		  (QListViewItem *__pQListViewItem, 
			 const QPoint &__rQPoint,
			 int column__i);

	  void objectSelected
			(QListViewItem *__pQListViewItem);
		
	  void cut();
		void copy();
		void paste();

		void buildBonds();
		void removeBonds();

		void select();
		void deselect();

		void checkResidue();

		void removeObject();

		void centerCamera();

		void changeDisplay();

		void clearClipboard();

  signals:

	  void writeText(QString __QString);

		void itemSelected(bool selected__bool, bool residue__bool);
		void itemCutOrCopied(bool copied__bool);

	
  protected:

  private:

		QListViewItem *_getRoot(QListViewItem *__pQListViewItem);
		QString _getRootName(QListViewItem *__pQListViewItem);
		QString _getName(QListViewItem *__pQListViewItem);
		QString _getTypeName(QListViewItem *__pQListViewItem);
		QString _getRootTypeName(QListViewItem *__pQListViewItem);
		Composite *_getCompositeAddress(QListViewItem *__pQListViewItem);

	  void _genListViewItem
			(QListViewItem *__pQListViewItem,
			 Composite *__pComposite,
			 QString *name__pQString = 0);

	  QListViewItem *_findListViewItem
			(Composite *__pComposite);

		Scene *__mpScene_;
    MoleculeObjectProcessor *__mpMoleculeObjectProcessor_;

		Composite *selected__mpComposite_;
		QListViewItem *selected__mpQListViewItem_;
		QString selected_name__mQString_;
		QString selected_root_name__mQString_;
		QString selected_type__mQString_;
		QString selected_root_type__mQString_;

		Composite *copied__mpComposite_;
};


#		ifndef BALL_NO_INLINE_FUNCTIONS
#			include "control.iC"
#		endif

#endif // BALL_MOLVIE_APPLICATIONS_MOLVIEW_CONTROL_H_
