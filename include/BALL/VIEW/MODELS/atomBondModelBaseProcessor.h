// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: atomBondModelBaseProcessor.h,v 1.3 2003/08/29 10:36:40 amoll Exp $
//

#ifndef BALL_VIEW_MODELS_ATOMBONDMODELBASEPROCESSOR_H
#define BALL_VIEW_MODELS_ATOMBONDMODELBASEPROCESSOR_H

#ifndef BALL_CONCEPT_COMPOSITE_H
#	include <BALL/CONCEPT/composite.h>
#endif

#ifndef BALL_DATATYPE_LIST_H
#	include <BALL/DATATYPE/list.h>
#endif

#ifndef BALL_DATATYPE_HASHSET_H
#	include <BALL/DATATYPE/hashSet.h>
#endif

#ifndef BALL_VIEW_MODELS_MOLECULARMODEL_H
#	include <BALL/VIEW/MODELS/molecularModel.h>
#endif

#ifndef BALL_VIEW_COMMON_H
# include <BALL/VIEW/KERNEL/common.h>
#endif


namespace BALL
{
	class Atom;

	namespace VIEW
	{
		/** AtomBondModelBaseProcessor class.
				The class AtomBondModelBaseProcessor is the base class for all
				models whose structure is somewhat orientated after the Atom
				and Bond structure (e.g. the model <b>Ball And Stick</b>)
				Because it is difficult to create the connecting model
				between two models this class has helper methods to make the
				generation process between different models easier. All
				Atom objects that are processed to build the new model can be
				inserted into the AtomBondModelBaseProcessor so that the
				finish method can call the method buildBondModels_() so
				that all Bond objects can get their connecting models. This is
				achieved by the class BaseModelConnector and its derived
				classes. 
				\ingroup  ViewModels
		*/
		class AtomBondModelBaseProcessor
			:	public MolecularModelProcessor
		{
			public:

			/**	@name	Constructors and Destructors
			*/	
			//@{

			/** Default Constructor.
					Clear all used Atom objects.
			*/
			AtomBondModelBaseProcessor()
				throw();

			/** Copy constructor.
			*/
			AtomBondModelBaseProcessor(const AtomBondModelBaseProcessor& baseProcessor)
				throw();

			/** Destructor.
			*/
			virtual ~AtomBondModelBaseProcessor()
				throw();

			/** Explicit default initialization.
					Reset the state of this AtomBondModelBaseProcessor:
					  - clear the used atoms
					  ­ clear the MolecularModelProcessor
					Calls MolecularModelProcessor::clear.
					Calls clearUsedAtoms_().
			*/
			virtual void clear()
				throw();

			//@} 
			/**	@name	Assignment 
			*/ 
			//@{

			/** Assignment.
					Calls clearUsedAtoms_().
					Calls MolecularModelProcessor::set.
					\param       processor the AtomBondModelBaseProcessor to be copied 
			*/
			virtual void set(const AtomBondModelBaseProcessor& processor)
				throw();

			/** Assignment operator.
					Calls set.
					\param       processor the AtomBondModelBaseProcessor to be copied
			*/
			const AtomBondModelBaseProcessor& operator = (const AtomBondModelBaseProcessor& processor)
				throw();

			/** Swapping of AtomBondModelBaseProcessor.
					Calls MolecularModelProcessor::swap
			*/
			void swap(AtomBondModelBaseProcessor& processor)
				throw();

			//@} 
			/**	@name Processor specific methods 
			*/ 
			//@{
			
			/** Start method.
					Calls clearUsedAtoms_().
					Calls MolecularModelProcessor::start.
					\return bool true if the start was successful
			*/
			virtual bool start();
			
			/** Finish method.
					This method will be internally called from the processor mechanism if the processor
					has finished processing the Composite tree.
					All previously inserted Atom objects (inserted with the method insertAtom_())
					will be processed with the method buildBondModels_() to create the graphical 
					representation of the Bond objects.
					\return bool true if the finish was successful
			*/
			virtual bool finish();

			/**	Operator method.
					This method must be overridden to implement the creation of the different models.
					If a model should be created that uses Atom and Bond objects for
					structure (e.g. Ball And Stick Model) than the processed Atom objects
					must be inserted into the AtomBondModelBaseProcessor with the method
					insertAtom_(). This method in correlation with the method buildBondModels_()
					will create the needed models for the connecting Bond objects.
					\param  composite the Composite object that will be processed
					\return Processor::Result the result 
			*/
			virtual Processor::Result operator() (Composite& composite);
						
			//@} 
			/**	@name	debuggers and diagnostics 
			*/ 
			//@{

			/** Internal state and consistency self-validation.
					Calls MolecularModelProcessor::isValid.
					\return			bool true if the internal state is correct 
			*/
			virtual bool isValid() const
				throw();

			/** Internal value dump.
					Dump the current state to the output ostream <b>s</b> with 
					dumping depth <b>depth</b>.
					Calls MolecularModelProcessor::dump.
					\param   s output stream where to output the state 
					\param   depth the dumping depth
			*/
			virtual void dump(std::ostream& s = std::cout, Size depth = 0) const
				throw();
							
			protected:

			//@}
			/** @name Protected members
					This methods are provided for easy generation of models. With the method 
					insertAtom_() all Atom objects that will be part of the newly created
					model will be stored. This stored atoms are later used (in the method 
					buildBondModels_()) to create the models of the Bond objects that can
					be reached from the atoms.
					The other methods are access method for the stored atoms.			 
			*/
			//@{

			/** Insert an Atom into the used atoms structure. 
					All the inserted atoms will be later processed to generate the graphical representation
					of the Bond objects that can be reached from each inserted atom.
					\param  atom the Atom object to inserted
			*/
			void insertAtom_(const Atom* atom)
				throw();

			/** Clear all used atoms.
			*/
			void clearUsedAtoms_()
				throw();

			/** Mutable inspection of the list of used atoms.
					Access the mutable reference of the list of used Atom objects inserted with
					the method insertAtom_().
					\return  the list of used Atom objects.
			*/
			List<const Atom*>& getAtomList_()
				throw();

			/** Mutable inspection of the hash set of used atoms.
					Access the mutable reference of the hast set of used Atom objects inserted with
					the method insertAtom_().
					\return  the hash set of used Atom objects.
			*/
			HashSet<const Atom*>& getAtomSet_()
				throw();

			/** Build the bond models.
					This method is called from the finish method. It iterates over every Bond
					object that can be reached from the stored Atom objects and hosts the 
					BaseModelConnector on it.
					This method is the control method that is responsible for creating the needed models
					for Bond objects between different models.
					See BaseModelConnector for more information about connecting different models.
			*/
			void buildBondModels_();
			//@}

			private:

			List<const Atom*> used_atoms_;
			HashSet<const Atom*> hashed_atoms_;
		};

#	ifndef BALL_NO_INLINE_FUNCTIONS
#		include <BALL/VIEW/MODELS/atomBondModelBaseProcessor.iC>
#	endif

	} // namespace VIEW

} // namespace BALL

#endif // BALL_VIEW_MODELS_ATOMBONDMODELBASEPROCESSOR_H 
