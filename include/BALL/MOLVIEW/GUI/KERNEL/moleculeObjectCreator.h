// $Id: moleculeObjectCreator.h,v 1.1 2000/09/23 15:38:14 hekl Exp $

#ifndef BALL_MOLVIEW_GUI_KERNEL_MOLECULEOBJECTCREATOR_H
#define BALL_MOLVIEW_GUI_KERNEL_MOLECULEOBJECTCREATOR_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

#ifndef BALL_KERNEL_ATOM_H
#	include <BALL/KERNEL/atom.h>
#endif

#ifndef BALL_KERNEL_BOND_H
#	include <BALL/KERNEL/bond.h>
#endif

#ifndef BALL_KERNEL_FOREACH_H
#	include <BALL/KERNEL/forEach.h>
#endif

#ifndef BALL_KERNEL_MOLECULE_H
#	include <BALL/KERNEL/molecule.h>
#endif

#ifndef BALL_KERNEL_PROTEIN_H
# include <BALL/KERNEL/protein.h>
#endif

#ifndef BALL_KERNEL_SYSTEM_H
# include <BALL/KERNEL/system.h>
#endif

#ifndef BALL_VIEW_GUI_KERNEL_OBJECTCREATOR_H
#	include <BALL/VIEW/GUI/KERNEL/objectCreator.h>
#endif
  
namespace BALL
{

	namespace MOLVIEW
	{

		using VIEW::ObjectCreator;

		/**
		*/
		class MoleculeObjectCreator
			: public ObjectCreator
		{
			public:

			/**	@name	Constructors and Destructors
			*/
			//@{

			MoleculeObjectCreator();

			MoleculeObjectCreator
				(const MoleculeObjectCreator& object_creator, bool deep = true);

			virtual ~MoleculeObjectCreator();
			//@}


			/**	@name Accessors
			*/
			//@{
			virtual void initPersistenceManager(TextPersistenceManager &pm);

			virtual Composite *convertObject(PersistentObject &po);
			//@}
		};

#			ifndef BALL_NO_INLINE_FUNCTIONS
#				include <BALL/MOLVIEW/GUI/KERNEL/moleculeObjectCreator.iC>
#			endif

	} // namespace MOLVIEW

} // namespace BALL

#endif // BALL_MOLVIEW_GUI_KERNEL_MOLECULEOBJECTCREATOR_H 
