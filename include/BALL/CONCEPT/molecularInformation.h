// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#ifndef BALL_CONCEPT_MOLECULARINFORMATION_H
#define BALL_CONCEPT_MOLECULARINFORMATION_H

#ifndef BALL_CONCEPT_COMPOSITE_H
#	include <BALL/CONCEPT/composite.h>
#endif

namespace BALL
{
	class Atom;

	/** MolecularInformation class.
			This class is used by the class MolecularControl to retrieve names and types
			of the displayed Composite objects.
			\ingroup  Persistence
	*/
	class BALL_EXPORT MolecularInformation
		: public Visitor<Composite>
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
			/// Unkown type. This value will be set in clear
			TYPE__UNKNOWN              = -1,

			/// System type.
			TYPE__SYSTEM               = 0,

			/// Protein type.
			TYPE__PROTEIN              = 1,

			/// Molecule type.
			TYPE__MOLECULE             = 2,

			/// Chain type.
			TYPE__CHAIN                = 3,

			/// Fragment type.
			TYPE__FRAGMENT             = 4,

			/// Residue type.
			TYPE__RESIDUE              = 5,

			/// SecondaryStructure type.
			TYPE__SECONDARY_STRUCTURE  = 6,

			/// Atom type.
			TYPE__ATOM                 = 7,
				
			/// Bond type.
			TYPE__BOND 								 = 8


		};

		//@} 
		/**	@name	Constructors and Destructors */	
		//@{

		/** Default Constructor.
				Calls clear.
		*/
		MolecularInformation()
			;

		/** Destructor.
		*/
		virtual ~MolecularInformation()
			;

		/** Explicit default initialization.
				Reset the state of this moleculareInformation to:
				Calls Information::clear.
					-  type is set to <tt> TYPE__UNKNOWN</tt>
		*/
		virtual void clear()
			;

		//@} 
		/**	@name	Accessors: inspectors and mutators */ 
		//@{

		/** Return the type of the given composite.
				If the given Composite is of any kind that is defined by the 
				enumeration Type this type will be returned.
				\return  Type the type of the Composite
				\see     Type
				\see     visit
		*/
		Type getType() const  { return type_;}

		///
		String getName() const  { return name_;}

		///
		String getTypeName() const  { return type_name_;}

		/** Visit method.
				This method tries to determine the name and the type of the Composite
				object. If the type of composite returned from the method getType
				is one of the defined type of Type
				than that name and type is set to the variables name_ and type_name_.
				If the type is TYPE__UNKOWN than the method getType is
				called to determine the type.
				\param  composite the Composite object whose information should be extracted
				\see     getType
		*/
		virtual void visit(Composite& composite);
		//@}
		
		private:

		String getBondAtomName_(Atom* atom);
		void getType_(Composite& composite);
		void getTypeName_();
		void getName_(Composite& composite);

		Type 	 type_;
		String name_;
		String type_name_;
	};

} // namespace BALL

#endif // BALL_CONCEPT_MOLECULARINFORMATION_H
