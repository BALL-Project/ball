// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: molecularInformation.h,v 1.13 2003/03/26 13:56:48 anhi Exp $

#ifndef BALL_MOLVIEW_FUNCTOR_MOLECULARINFORMATION_H
#define BALL_MOLVIEW_FUNCTOR_MOLECULARINFORMATION_H

#ifndef BALL_CONCEPT_COMPOSITE_H
#	include <BALL/CONCEPT/composite.h>
#endif

#ifndef BALL_VIEW_FUNCTOR_INFORMATION_H
#	include <BALL/VIEW/FUNCTOR/information.h>
#endif

namespace BALL
{
	namespace MOLVIEW
	{
		/** MolecularInformation class.
				The class MolecularInformation is derived from the class  \link Information Information \endlink .
				It overrides the  \link visit visit \endlink  method to extract the name and the type of
				 \link Composite Composite \endlink  objects that are of a molecular kind. The new types that will be
				recognized are defined in  \link Type Type \endlink . If the  \link Composite Composite \endlink  object is not of the
				types specified in the  \link Type Type \endlink  enumeration than the request will be given to
				the  \link Information Information \endlink  class.
				This class is used by the class  \link MolecularControl MolecularControl \endlink  to retrieve names and types
				of the displayed  \link Composite Composite \endlink  objects.  \par
				
				@see     Information

    		\ingroup  MolviewFunctorsHelper
		*/
		class MolecularInformation
			: public VIEW::Information
		{
			public:
			
			/**	@name	Enums
			 */
			//@{
			/** Type.
					This types are possible.
			*/
			enum Type
			{
				/// Unkown type. This value will be set in  \link clear clear \endlink 
				TYPE__UNKNOWN              = -1,

				/// System type. This value will be set if the given  \link Composite Composite \endlink  object is of kind  \link System System \endlink 
				TYPE__SYSTEM               = 0,

				/// Protein type. This value will be set if the given  \link Composite Composite \endlink  object is of kind  \link Protein Protein \endlink 
				TYPE__PROTEIN              = 1,

				/// Molecule type. This value will be set if the given  \link Composite Composite \endlink  object is of kind  \link Molecule Molecule \endlink 
				TYPE__MOLECULE             = 2,

				/// Chain type. This value will be set if the given  \link Composite Composite \endlink  object is of kind  \link Chain Chain \endlink 
				TYPE__CHAIN                = 3,

				/// Fragment type. This value will be set if the given  \link Composite Composite \endlink  object is of kind  \link Fragment Fragment \endlink 
				TYPE__FRAGMENT             = 4,

				/// Residue type. This value will be set if the given  \link Composite Composite \endlink  object is of kind  \link Residue Residue \endlink 
				TYPE__RESIDUE              = 5,

				/// Secondary structure type. This value will be set if the given  \link Composite Composite \endlink  object is of kind  \link SecondaryStructure SecondaryStructure \endlink 
				TYPE__SECONDARY_STRUCTURE  = 6,

				/// Atom type. This value will be set if the given  \link Composite Composite \endlink  object is of kind  \link Atom Atom \endlink 
				TYPE__ATOM                 = 7
			};
			//@}

			/**	@name	Constructors
			*/	
			//@{

			/** Default Constructor.
					Construct new moleculareInformation.
					Calls  \link clear clear \endlink .
					@return      MolecularInformation new constructed moleculareInformation
					@see         Information
			*/
			MolecularInformation()
			  throw();

			//@}

			/** @name Destructors 
			*/
			//@{

			/** Destructor.
					Default destruction of {\em *this} moleculareInformation.
					Calls  \link destroy destroy \endlink .
					@see         destroy
			*/
			virtual ~MolecularInformation()
				throw();

			/** Explicit default initialization.
					Reset the state of {\em *this} moleculareInformation to:
					Calls  \link Information::clear Information::clear \endlink .

					  -  type is set to <tt>TYPE__UNKNOWN</tt>
					
					@see   Filter::clear
			*/
			virtual void clear()
				throw();

			/** Explicit destructor.
					Empty for further purpose.
			*/
			virtual void destroy()
				throw();
			//@}

			/**	@name	Accessors: inspectors and mutators 
			*/
			//@{

			/** Return the type of the given composite.
					If the given  \link Composite Composite \endlink  is of any kind that is defined by the 
					enumeration  \link Type Type \endlink  this type will be returned.
					@return  Type the type of the  \link Composite Composite \endlink 
					@see     Type
					@see     visit
			*/
			Type getType() const
			  throw();

			/** Visit method.
					This method tries to determine the name and the type of the  \link Composite Composite \endlink 
					object. If the type of {\em composite} returned from the method  \link getType getType \endlink 
					is one of the defined type of  \link Type Type \endlink 
					than that name and type is set to the variables  \link name_ name_ \endlink  and  \link type_name_ type_name_ \endlink .
					If the type is {\em TYPE__UNKOWN} than the method  \link Information::getType Information::getType \endlink  is
					called to determine the type.
					@param  composite the  \link Composite Composite \endlink  object whose information should be extracted
					@see     Information
					@see     getType
			*/
			virtual void visit(Composite& composite);
			//@}
			
			protected:
			
  		private:

			void getType_(Composite& composite);
			void getTypeName_(Composite& composite);
			void getName_(Composite& composite);

			Type type_;
		};

			
#			ifndef BALL_NO_INLINE_FUNCTIONS
#				include <BALL/MOLVIEW/FUNCTOR/molecularInformation.iC>
#			endif
	} // namespace MOLVIEW

} // namespace BALL

#endif // BALL_MOLVIEW_FUNCTOR_MOLECULARINFORMATION_H
