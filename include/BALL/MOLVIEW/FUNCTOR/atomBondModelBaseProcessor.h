// $Id: atomBondModelBaseProcessor.h,v 1.3 2001/05/13 14:55:21 hekl Exp $

#ifndef BALL_MOLVIEW_FUNCTOR_ATOMBONDMODELBASEPROCESSOR_H
#define BALL_MOLVIEW_FUNCTOR_ATOMBONDMODELBASEPROCESSOR_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

#ifndef BALL_CONCEPT_COMPOSITE_H
#	include <BALL/CONCEPT/composite.h>
#endif

#ifndef BALL_DATATYPE_LIST_H
#	include <BALL/DATATYPE/list.h>
#endif

#ifndef BALL_DATATYPE_HASHSET_H
#	include <BALL/DATATYPE/hashSet.h>
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

#ifndef BALL_MOLVIEW_FUNCTOR_BASEMODEL_H
#	include <BALL/MOLVIEW/FUNCTOR/baseModel.h>
#endif



namespace BALL
{

	namespace MOLVIEW
	{


		/** AtomBondModelBaseProcessor class.
				{\bf Framework:} BALL/MOLVIEW/FUNCTOR\\
				{\bf Definition:} \URL{BALL/MOLVIEW/FUNCTOR/atomBondModelBaseProcessor.h}\\ \\
				The class AtomBondModelBaseProcessor is the base class for all models
				whose structure is somewhat orientated after the \Ref{Atom} and \Ref{Bond}
				structure (e.g. the model {\em Ball And Stick} has the geometric primitives
				appended to the \Ref{Atom} and \Ref{Bond} objects).
				Because it is difficult the create the connecting model between two models
				this class has helper methods to make the generation process of the above
				described models easier.
				All \Ref{Atom} objects that are processed to build the new model can be inserted
				into {\em *this} atomBondModelBaseProcessor so that the \Ref{finish} method
				can call the method \Ref{buildBondModels_} so that all \Ref{Bond} objects
				that can be reached from the inserted \Ref{Atom} objects can get their
				connecting models. This is achieved by the class \Ref{BaseModelConnector} and
				its derived classes.
				@see     finish
				@see     buildBondModels_
				@see     BaseModelConnector
				@see     Atom
				@see     Bond
				@memo    AtomBondModelBaseProcessor class (BALL MOLVIEW functor framework)
				@author  $Author: hekl $
				@version $Revision: 1.3 $
				@date    $Date: 2001/05/13 14:55:21 $
		*/
		class AtomBondModelBaseProcessor:	public BaseModelProcessor
		{
			public:

			/**	@name	Constructors
			*/	
			//@{

			/** Default Constructor.
					Construct new atomBondModelBaseProcessor.
					Clear all used \Ref{Atom} objects.
					@return      AtomBondModelBaseProcessor new constructed atomBondModelBaseProcessor
					@see         BaseModelProcessor
			*/
			AtomBondModelBaseProcessor()
				throw();

			/** Copy constructor.
					Construct new atomBondModelBaseProcessor by copying the atomBondModelBaseProcessor
					{\em baseProcessor}.
					@param       baseProcessor the atomBondModelBaseProcessor to be copied
					@param       deep make a deep (={\tt true}) or shallow (={\tt false}) copy of {\em baseProcessor}
					@return      AtomBondModelBaseProcessor new constructed atomBondModelBaseProcessor copied from {\em baseProcessor}
					@see         BaseModelProcessor
			*/
			AtomBondModelBaseProcessor
				(const AtomBondModelBaseProcessor& baseProcessor, bool deep = true)
				throw();

			//@}

			/** @name Destructors 
			*/
			//@{

			/** Destructor.
					Default destruction of {\em *this} atomBondModelBaseProcessor.
					Calls \Ref{destroy}.
					@see         destroy
			*/
			virtual ~AtomBondModelBaseProcessor()
				throw();

			/** Explicit default initialization.
					Reset the state of {\em *this} atomBondModelBaseProcessor to:
					\begin{itemize}
					  \item  clear the used atoms
					  \item  clear the \Ref{BaseModelProcessor}
					\end{itemize}
					Calls \Ref{BaseModelProcessor::clear}.
					Calls \Ref{clearUsedAtoms_}.
					@see  BaseModelProcessor
					@see  clearUsedAtoms_
			*/
			virtual void clear()
				throw();

			/** Explicit destructor.
					Empty for further purpose.
			*/
			virtual void destroy()
				throw();
			//@}

			
			/**	@name	Assignment
			*/
			//@{

			/** Assignment.
					Assign the atomBondModelBaseProcessor {\em processor} to {\em *this} 
					atomBondModelBaseProcessor.
					Initializes the \Ref{BaseModelProcessor} of {\em *this} baseModelProcessor
					to that of {\em processor}.
					Calls \Ref{clearUsedAtoms_}.
					Calls \Ref{BaseModelProcessor::set}.
					@param       processor the atomBondModelBaseProcessor to be copied 
					@param       deep make a deep (={\tt true}) or shallow (={\tt false}) copy of {\em processor}
					@see         clearUsedAtoms_
					@see         BaseModelProcessor
			*/
			void set
				(const AtomBondModelBaseProcessor& processor, bool deep = true)
				throw();

			/** Assignment operator.
					Assign the atomBondModelBaseProcessor {\em processor} to {\em *this} 
					atomBondModelBaseProcessor.
					Initializes the \Ref{BaseModelProcessor} of {\em *this} baseModelProcessor
					to that of {\em processor}.
					Calls \Ref{set}.
					The assignment will be deep.
					@param       processor the atomBondModelBaseProcessor to be copied
					@return      AtomBondModelBaseProcessor& constant reference of {\em *this} atomBondModelBaseProcessor
					@see         set
			*/
			const AtomBondModelBaseProcessor& operator =
				(const AtomBondModelBaseProcessor& processor)
				throw();

			/** Copying.
					Copy {\em *this} atomBondModelBaseProcessor to the atomBondModelBaseProcessor
					{\em processor}.
					Initializes the \Ref{BaseModelProcessor} of {\em *this} atomBondModelBaseProcessor
					to that of {\em processor}.
					Calls \Ref{set}.
					@param       processor the atomBondModelBaseProcessor to be assigned to
					@param       deep make a deep (={\tt true}) or shallow (={\tt false}) copy of {\em processor}
					@see         set
			*/
			void get
				(AtomBondModelBaseProcessor& processor, bool deep = true) const
				throw();

			/** Swapping of atomBondModelBaseProcessor's.
					Swap the \Ref{BaseModelProcessor} of {\em *this} atomBondModelBaseProcessor
					with that of {\em processor}.
					Calls \Ref{BaseModelProcessor::swap}
					@param       processor the atomBondModelBaseProcessor being swapped with {\em *this} atomBondModelBaseProcessor 
					@see         BaseModelProcessor
			*/
			void swap
				(AtomBondModelBaseProcessor& processor)
				throw();
			//@}

			/**	@name Processor specific methods
			*/
			//@{
			/** Start method.
					Clear the used \Ref{Atom} objects.
					Calls \Ref{clearUsedAtoms_}.
					Calls \Ref{BaseModelProcessor::start}.
					@return bool {\tt true} if the start of {\em *this} atomBondModelBaseProcessor was successful, {\tt false} otherwise
					@see    clearUsedAtoms_
					@see    operator()
					@see    BaseModelProcessor
			*/
			virtual bool start()
				throw();
			
			/** Finish method.
					This method will be internally called from the processor mechanism if the processor
					has finished processing the \Ref{Composite} tree.
					All previously inserted \Ref{Atom} objects (inserted with the method \Ref{insertAtom_})
					will be processed with the method \Ref{buildBondModels_} to create the graphical 
					representation of the \Ref{Bond} objects.
					@return bool {\tt true} if the finish of {\em *this} atomBondModelBaseProcessor was successful, {\tt false} otherwise
					@see    buildBondModels_
					@see    operator()
					@see    Atom
					@see    Bond
					@see    Composite
			*/
			virtual bool finish()
				throw();

			/**	Operator method.
					This method must be overridden to implement the creation of the different models.
					If a model should be created that uses \Ref{Atom} and \Ref{Bond} objects for
					structure (e.g. Ball And Stick Model) than the processed \Ref{Atom} objects
					must be inserted into {\em *this} atomBondModelBaseProcessor with the method
					\Ref{insertAtom_}. This method in correlation with the method \Ref{buildBondModels_}
					will create the needed models for the connecting \Ref{Bond} objects.
					@param  composite the \Ref{Composite} object that will be processed
					@return Processor::Result the result of {\em *this} atomBondModelBaseProcessor
					@see    start
					@see    finish
					@see    insertAtom_
					@see    Composite
					@see    Atom
					@see    Bond
			*/
			virtual Processor::Result operator() (Composite& composite)
				throw();
			//@}
				
			/**	@name	debuggers and diagnostics
			*/
			//@{

			/** Internal state and consistency self-validation.
					Initiate self-validation of the internal state and data structure consistencies
					of {\em *this} atomBondModelBaseProcessor.
					If the internal state of {\em *this} atomBondModelBaseProcessor is correct 
					(self-validated) and consistent {\tt true} is returned, {\tt false} otherwise. 
					{\em *this} atomBondModelBaseProcessor is valid if:
					\Ref{BaseModelProcessor} is valid.
					Calls \Ref{BaseModelProcessor::isValid}.
					@return			bool {\tt true} if the internal state of {\em *this} atomBondModelBaseProcessor is correct (self-validated) and consistent, {\tt false} otherwise
					@see       BaseModelProcessor
			*/
			virtual bool isValid() const
				throw();

			/** Internal value dump.
					Dump the current state of {\em *this} atomBondModelBaseProcessor to 
					the output ostream {\em s} with dumping depth {\em depth}.
					Calls \Ref{BaseModelProcessor::dump}.
					@param   s output stream where to output the state of {\em *this} atomBondModelBaseProcessor
					@param   depth the dumping depth
					@see     BaseModelProcessor
			*/
			virtual void dump
				(std::ostream& s = std::cout, Size depth = 0) const
				throw();
			//@}


			protected:

			/** @name Protected members
					This methods are provided for easy generation of models. With the method 
					\Ref{insertAtom_} all \Ref{Atom} objects that will be part of the newly created
					model will be stored. This stored atoms are later used (in the method 
					\Ref{buildBondModels_}) to create the models of the \Ref{Bond} objects that can
					be reached from the atoms.
					The other methods are access method for the stored atoms.			 
			*/
			//@{
			/** Insert atom for later processing.
					Insert \Ref{Atom} {\em atom} into the used atoms structure. 
					All the inserted atoms will be later processed to generate the graphical representation
					of the \Ref{Bond} objects that can be reached from each inserted atom.
					@param  atom the \Ref{Atom} object to inserted
					@see    clearUsedAtoms_
					@see    getAtomList_
					@see    getAtomSet_
					@see    buildBondModels_
					@see    finish
					@see    Atom
					@see    Bond
			*/
			void insertAtom_(Atom* atom)
				throw();

			/** Clear all used atoms.
					Clear the structures for the used \Ref{Atom} objects.
					@see    insertAtom_
					@see    getAtomList_
					@see    getAtomSet_
					@see    buildBondModels_
			*/
			void clearUsedAtoms_()
				throw();

			/** Mutable inspection of the list of used atoms.
					Access the mutable reference of the list of used \Ref{Atom} objects inserted with
					the method \Ref{insertAtom_}.
					@return  List<Atom*>& the list of used \Ref{Atom} objects.
					@see    insertAtom_
					@see    clearUsedAtoms_
					@see    getAtomSet_
					@see    buildBondModels_
					@see    Atom
			*/
			List<Atom*>& getAtomList_()
				throw();

			/** Mutable inspection of the hash set of used atoms.
					Access the mutable reference of the hast set of used \Ref{Atom} objects inserted with
					the method \Ref{insertAtom_}.
					@return  HashSet<Atom*>& the hash set of used \Ref{Atom} objects.
					@see    insertAtom_
					@see    clearUsedAtoms_
					@see    getAtomList_
					@see    buildBondModels_
					@see    Atom
			*/
			HashSet<Atom*>& getAtomSet_()
				throw();

			/** Build the bond models.
					This method is called from the \Ref{finish} method. It iterates over every \Ref{Bond}
					object that can be reached from the stored \Ref{Atom} objects and removes all
					previously attached \Ref{GeometricObject} objects and hosts the \Ref{BaseModelConnector}
					returned with the method \Ref{getModelConnector} on it.
					This method is the control method that is responsible for creating the needed models
					for \Ref{Bond} objects between different models.
					See \Ref{BaseModelConnector} for more information about connecting different models.
					@see  finish
					@see  insertAtom_
					@see  getAtomList_
					@see  getAtomSet_
					@see  BaseModelConnector
					@see  getModelConnector
					@see  removeGeometricObjects_
			*/
			void buildBondModels_()
				throw();
			//@}

			private:

			List<Atom*> used_atoms_;
			HashSet<Atom*> hashed_atoms_;
		};

#			ifndef BALL_NO_INLINE_FUNCTIONS
#				include <BALL/MOLVIEW/FUNCTOR/atomBondModelBaseProcessor.iC>
#			endif

	} // namespace MOLVIEW

} // namespace BALL

#endif // BALL_MOLVIEW_FUNCTOR_ATOMBONDMODELBASEPROCESSOR_H 
