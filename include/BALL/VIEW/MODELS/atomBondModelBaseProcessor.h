// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: atomBondModelBaseProcessor.h,v 1.15 2005/02/23 12:55:22 amoll Exp $
//

#ifndef BALL_VIEW_MODELS_ATOMBONDMODELBASEPROCESSOR_H
#define BALL_VIEW_MODELS_ATOMBONDMODELBASEPROCESSOR_H

#ifndef BALL_DATATYPE_LIST_H
#	include <BALL/DATATYPE/list.h>
#endif

#ifndef BALL_DATATYPE_HASHSET_H
#	include <BALL/DATATYPE/hashSet.h>
#endif

#ifndef BALL_VIEW_MODELS_MODELPROCESSOR_H
#	include <BALL/VIEW/MODELS/modelProcessor.h>
#endif

namespace BALL
{
	class Atom;
	class Bond;

	namespace VIEW
	{
		/** AtomBondModelBaseProcessor class.
				The class AtomBondModelBaseProcessor is the base class for all
				models whose structure is somewhat orientated after the Atom
				and Bond structure (e.g. the model <b>Ball And Stick</b>)
				\ingroup  ViewModels
		*/
		class BALL_EXPORT AtomBondModelBaseProcessor
			:	public ModelProcessor
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
					  - clear the ModelProcessor
					Calls ModelProcessor::clear.
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
					Calls ModelProcessor::set.
			*/
			virtual void set(const AtomBondModelBaseProcessor& processor)
				throw();

			/** Assignment operator.
					Calls set.
			*/
			const AtomBondModelBaseProcessor& operator = (const AtomBondModelBaseProcessor& processor)
				throw();

			//@} 
			/**	@name Processor specific methods 
			*/ 
			//@{

			/**	Operator method.
					This method iterates over each Composite object reachable in the 
					Composite tree. If a Composite is of kind Moleculue and has 
					aromatic rings, these rings are stored for later processing in 
					the finish method.
					\param  composite the Composite object that will be processed
			*/
			virtual Processor::Result operator() (Composite& composite);
		
			//@} 
			/**	@name	debuggers and diagnostics 
			*/ 
			//@{

			/** Internal value dump.
					Dump the current state to the output ostream <b>s</b> with 
					dumping depth <b>depth</b>.
					Calls ModelProcessor::dump.
					\param   s output stream where to output the state 
					\param   depth the dumping depth
			*/
			virtual void dump(std::ostream& s = std::cout, Size depth = 0) const
				throw();

			///
			virtual void clearComposites()
				throw();

			/// 
			virtual bool createGeometricObjects()
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
			*/
			List<const Atom*>& getAtomList_()
				throw();

			/** Mutable inspection of the hash set of used atoms.
			*/
			HashSet<const Atom*>& getAtomSet_()
				throw();

			/** Build the bond models.
					This method is called from the finish method. It iterates over every Bond
					object that can be reached from the stored Atom objects and calls visualiseBond_(),
					which has to be overloaded in derived classes.
			*/
			void buildBondModels_();

			//@}
			
			virtual void visualiseBond_(const Bond& bond)
				throw();

			virtual void visualiseRings_()
				throw(){};

			vector<vector<Atom*> > rings_;
			HashSet<const Atom*> ring_atoms_;

			private:

			List<const Atom*> used_atoms_;
			HashSet<const Atom*> atom_set_;
		};

#	ifndef BALL_NO_INLINE_FUNCTIONS
#		include <BALL/VIEW/MODELS/atomBondModelBaseProcessor.iC>
#	endif

} } // namespaces

#endif // BALL_VIEW_MODELS_ATOMBONDMODELBASEPROCESSOR_H 
