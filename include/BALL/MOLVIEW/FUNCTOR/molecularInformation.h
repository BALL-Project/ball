// $Id: molecularInformation.h,v 1.6.4.2 2002/11/26 15:28:26 oliver Exp $

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
				The class MolecularInformation is derived from the class \Ref{Information}.
				It overrides the \Ref{visit} method to extract the name and the type of
				\Ref{Composite} objects that are of a molecular kind. The new types that will be
				recognized are defined in \Ref{Type}. If the \Ref{Composite} object is not of the
				types specified in the \Ref{Type} enumeration than the request will be given to
				the \Ref{Information} class.
				This class is used by the class \Ref{MolecularControl} to retrieve names and types
				of the displayed \Ref{Composite} objects. \\
				{\bf Definition:} \URL{BALL/MOLVIEW/FUNCTOR/molecularInformation.h} 
				@see     Information
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
