// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: moleculeObjectCreator.h,v 1.7 2003/02/21 16:05:20 anhi Exp $

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

		/** MoleculeObjectCreator class.
				The class MoleculeObjectCreator is a class for converting  \link PersistentObject PersistentObject \endlink 
				objects that represent molecular structures into  \link Composite Composite \endlink  objects.
				Objects that do not represent molecular structures will not be converted.
				All necessary initialization and registration of molecular structure patterns
				will be done in the method  \link initPersistenceManager initPersistenceManager \endlink .
				The conversion will be executed in the method  \link convertObject convertObject \endlink .
				This class is used by the class  \link Server Server \endlink  to convert incoming
				 \link PersistentObject PersistentObject \endlink  objects to  \link Composite Composite \endlink  objects.  \par
				<b>Definition:</b> BALL/MOLVIEW/GUI/KERNEL/moleculeObjectCreator.h
		*/
		class MoleculeObjectCreator: public ObjectCreator
		{
			public:

			/**	@name	Constructors
			*/	
			//@{

			/** Default Constructor.
					Constructs new moleculeObjectCreator.
					@return      MoleculeObjectCreator new constructed moleculeObjectCreator
					@see         ObjectCreator
			*/
			MoleculeObjectCreator()
				throw();

			//@}

			/** @name Destructors 
			*/
			//@{

			/** Destructor.
					Default destruction of {\em *this} moleculeObjectCreator.
					Calls  \link destroy destroy \endlink .
					@see         destroy
			*/
			virtual ~MoleculeObjectCreator()
				throw();

			/** Explicit default initialization.
					Calls  \link ObjectCreator::clear ObjectCreator::clear \endlink .
					@see  ObjectCreator::clear
			*/
			virtual void clear()
				throw();

			/** Explicit destructor.
					Calls  \link ObjectCreator::destroy ObjectCreator::destroy \endlink .
					@see  ObjectCreator::destroy
			*/
			virtual void destroy()
				throw();
			//@}


			/**	@name	Accessors: inspectors and mutators 
			*/
			//@{		
			/** Initializes the persistence manager.
					Initializes the  \link TextPersistenceManager TextPersistenceManager \endlink  of {\em *this} moleculeObjectCreator.
					Registers all necessary molecular structures available in BALL.
					See  \link TextPersistenceManager TextPersistenceManager \endlink  for information on how and why
					to register  \link PersistentObject PersistentObject \endlink  classes.
					@param  pm the  \link TextPersistenceManager TextPersistenceManager \endlink  to be initialized
					@see    TextPersistenceManager
					@see    convertObject
			*/
			virtual void initPersistenceManager(TextPersistenceManager &pm)
				throw();

			/** Converts persistent objects.
					If {\em po} is an object that represents a molecular structure (an  \link Atom Atom \endlink ,
					 \link Protein Protein \endlink  or  \link System System \endlink ) it will be converted into a \Ref{Composite}
					object and returned. If it is none of the above <tt>0</tt> will be returned.
					See  \link TextPersistenceManager TextPersistenceManager \endlink  for information
					on how to convert  \link PersistentObject PersistentObject \endlink  objects into other objects.				
					@param  po a reference to a  \link PersistentObject PersistentObject \endlink  to be converted
					@return Composite* a converted  \link Composite Composite \endlink  object if {\em po} could successfully be converted into a  \link Composite Composite \endlink  object
					@see    initPersistenceManager
			*/
			virtual Composite *convertObject(PersistentObject &po)
				throw();
			//@}
		};

	} // namespace MOLVIEW

} // namespace BALL

#endif // BALL_MOLVIEW_GUI_KERNEL_MOLECULEOBJECTCREATOR_H 
