// $Id: molecularInformation.h,v 1.4 2001/05/13 14:55:22 hekl Exp $

#ifndef BALL_MOLVIEW_FUNCTOR_MOLECULARINFORMATION_H
#define BALL_MOLVIEW_FUNCTOR_MOLECULARINFORMATION_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

#ifndef BALL_CONCEPT_COMPOSITE_H
#	include <BALL/CONCEPT/composite.h>
#endif

#ifndef BALL_KERNEL_ATOM_H
#	include <BALL/KERNEL/atom.h>
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
        
#ifndef BALL_MOLVIEW_COMMON_COMMON_H
#	include <BALL/MOLVIEW/COMMON/common.h>
#endif

#ifndef BALL_VIEW_FUNCTOR_INFORMATION_H
#	include <BALL/VIEW/FUNCTOR/information.h>
#endif

using namespace BALL::VIEW;

namespace BALL
{

	namespace MOLVIEW
	{
		
		/** MolecularInformation class.
				{\bf Framework:} BALL/MOLVIEW/FUNCTOR\\
				{\bf Definition:} \URL{BALL/MOLVIEW/FUNCTOR/molecularInformation.h} \\
				{\bf Concept:} visitor design pattern\\	\\
				The class MolecularInformation is derived from the class \Ref{Information}.
				It overrides the \Ref{visit} method to extract the name and the type of
				\Ref{Composite} objects that are of a molecular kind. The new types that will be
				recognized are defined in \Ref{Type}. If the \Ref{Composite} object is not of the
				types specified in the \Ref{Type} enumeration than the request will be given to
				the \Ref{Information} class.
				This class is used by the class \Ref{MolecularControl} to retrieve names and types
				of the displayed \Ref{Composite} objects.
				@see     Information
				@memo    MolecularInformation class (BALL MOLVIEW functor framework)
				@author  $Author: hekl $
				@version $Revision: 1.4 $
				@date    $Date: 2001/05/13 14:55:22 $
		*/
		class MolecularInformation: public Information
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
				/// Unkown type. This value will be set in \Ref{clear}
				TYPE__UNKNOWN              = -1,

				/// System type. This value will be set if the given \Ref{Composite} object is of kind \Ref{System}
				TYPE__SYSTEM               = 0,

				/// Protein type. This value will be set if the given \Ref{Composite} object is of kind \Ref{Protein}
				TYPE__PROTEIN              = 1,

				/// Molecule type. This value will be set if the given \Ref{Composite} object is of kind \Ref{Molecule}
				TYPE__MOLECULE             = 2,

				/// Chain type. This value will be set if the given \Ref{Composite} object is of kind \Ref{Chain}
				TYPE__CHAIN                = 3,

				/// Fragment type. This value will be set if the given \Ref{Composite} object is of kind \Ref{Fragment}
				TYPE__FRAGMENT             = 4,

				/// Residue type. This value will be set if the given \Ref{Composite} object is of kind \Ref{Residue}
				TYPE__RESIDUE              = 5,

				/// Secondary structure type. This value will be set if the given \Ref{Composite} object is of kind \Ref{SecondaryStructure}
				TYPE__SECONDARY_STRUCTURE  = 6,

				/// Atom type. This value will be set if the given \Ref{Composite} object is of kind \Ref{Atom}
				TYPE__ATOM                 = 7
			};
			//@}

			/**	@name	Constructors
			*/	
			//@{

			/** Default Constructor.
					Construct new moleculareInformation.
					Calls \Ref{clear}.
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
					Calls \Ref{destroy}.
					@see         destroy
			*/
			virtual ~MolecularInformation()
				throw();

			/** Explicit default initialization.
					Reset the state of {\em *this} moleculareInformation to:
					Calls \Ref{Information::clear}.
					\begin{itemize}
					  \item  type is set to {\tt TYPE__UNKNOWN}
					\end{itemize}
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
					If the given \Ref{Composite} is of any kind that is defined by the 
					enumeration \Ref{Type} this type will be returned.
					@return  Type the type of the \Ref{Composite}
					@see     Type
					@see     visit
			*/
			Type getType() const
			  throw();

			/** Visit method.
					This method tries to determine the name and the type of the \Ref{Composite}
					object. If the type of {\em composite} returned from the method \Ref{getType}
					is one of the defined type of \Ref{Type}
					than that name and type is set to the variables \Ref{name_} and \Ref{type_name_}.
					If the type is {\em TYPE__UNKOWN} than the method \Ref{Information::getType} is
					called to determine the type.
					@param  composite the \Ref{Composite} object whose information should be extracted
					@see     Information
					@see     getType
			*/
			virtual void visit(Composite& composite)
			  throw();
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
