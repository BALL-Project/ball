// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: atomBondModelBaseProcessor.h,v 1.11 2003/02/25 15:53:33 sturm Exp $

#ifndef BALL_MOLVIEW_FUNCTOR_ATOMBONDMODELBASEPROCESSOR_H
#define BALL_MOLVIEW_FUNCTOR_ATOMBONDMODELBASEPROCESSOR_H

#ifndef BALL_CONCEPT_COMPOSITE_H
#	include <BALL/CONCEPT/composite.h>
#endif

#ifndef BALL_DATATYPE_LIST_H
#	include <BALL/DATATYPE/list.h>
#endif

#ifndef BALL_DATATYPE_HASHSET_H
#	include <BALL/DATATYPE/hashSet.h>
#endif

#ifndef BALL_MOLVIEW_FUNCTOR_BASEMODEL_H
#	include <BALL/MOLVIEW/FUNCTOR/baseModel.h>
#endif

#ifndef BALL_MOLVIEW_COMMON_H
# include <BALL/MOLVIEW/COMMON/common.h>
#endif


namespace BALL
{
	class Atom;

	namespace MOLVIEW
	{
    /** \ingroup MolviewFunctorsBase
     *  @{
     */
		/** AtomBondModelBaseProcessor class.
				The class AtomBondModelBaseProcessor is the base class for all
				models whose structure is somewhat orientated after the  \link Atom Atom \endlink 
				and  \link Bond Bond \endlink  structure (e.g. the model {\em Ball And Stick} has
				the geometric primitives appended to the  \link Atom Atom \endlink  and  \link Bond Bond \endlink 
				objects). Because it is difficult the create the connecting model
				between two models this class has helper methods to make the
				generation process of the above described models easier. All
				 \link Atom Atom \endlink  objects that are processed to build the new model can be
				inserted into {\em *this} atomBondModelBaseProcessor so that the
				 \link finish finish \endlink  method can call the method  \link buildBondModels_ buildBondModels_ \endlink  so
				that all  \link Bond Bond \endlink  objects that can be reached from the inserted
				 \link Atom Atom \endlink  objects can get their connecting models. This is
				achieved by the class  \link BaseModelConnector BaseModelConnector \endlink  and its derived
				classes.  \par
				<b>Definition:</b> BALL/MOLVIEW/FUNCTOR/atomBondModelBaseProcessor.h
				@see     finish
				@see     buildBondModels_
				@see     BaseModelConnector
				@see     Atom
				@see     Bond
		*/
		class AtomBondModelBaseProcessor
			:	public BaseModelProcessor
		{
			public:

			/**	@name	Constructors
			*/	
			//@{

			/** Default Constructor.
					Construct new atomBondModelBaseProcessor.
					Clear all used  \link Atom Atom \endlink  objects.
					@return      AtomBondModelBaseProcessor new constructed atomBondModelBaseProcessor
					@see         BaseModelProcessor
			*/
			AtomBondModelBaseProcessor()
				throw();

			/** Copy constructor.
					Construct new atomBondModelBaseProcessor by copying the
					atomBondModelBaseProcessor {\em baseProcessor}.
					@param       baseProcessor the atomBondModelBaseProcessor to be copied
					@param       deep make a deep (=<tt>true</tt>) or shallow (=<tt>false</tt>) copy of {\em baseProcessor}
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
					Calls  \link destroy destroy \endlink .
					@see         destroy
			*/
			virtual ~AtomBondModelBaseProcessor()
				throw();

			/** Explicit default initialization.
					Reset the state of {\em *this} atomBondModelBaseProcessor to:
					\begin{itemize}
					  \item  clear the used atoms
					  \item  clear the  \link BaseModelProcessor BaseModelProcessor \endlink 
					\end{itemize}
					Calls  \link BaseModelProcessor::clear BaseModelProcessor::clear \endlink .
					Calls  \link clearUsedAtoms_ clearUsedAtoms_ \endlink .
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
					Initializes the  \link BaseModelProcessor BaseModelProcessor \endlink  of {\em *this} baseModelProcessor
					to that of {\em processor}.
					Calls  \link clearUsedAtoms_ clearUsedAtoms_ \endlink .
					Calls  \link BaseModelProcessor::set BaseModelProcessor::set \endlink .
					@param       processor the atomBondModelBaseProcessor to be copied 
					@param       deep make a deep (=<tt>true</tt>) or shallow (=<tt>false</tt>) copy of {\em processor}
					@see         clearUsedAtoms_
					@see         BaseModelProcessor
			*/
			void set
				(const AtomBondModelBaseProcessor& processor, bool deep = true)
				throw();

			/** Assignment operator.
					Assign the atomBondModelBaseProcessor {\em processor} to {\em *this} 
					atomBondModelBaseProcessor.
					Initializes the  \link BaseModelProcessor BaseModelProcessor \endlink  of {\em *this} baseModelProcessor
					to that of {\em processor}.
					Calls  \link set set \endlink .
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
					Initializes the  \link BaseModelProcessor BaseModelProcessor \endlink  of {\em *this} atomBondModelBaseProcessor
					to that of {\em processor}.
					Calls  \link set set \endlink .
					@param       processor the atomBondModelBaseProcessor to be assigned to
					@param       deep make a deep (=<tt>true</tt>) or shallow (=<tt>false</tt>) copy of {\em processor}
					@see         set
			*/
			void get
				(AtomBondModelBaseProcessor& processor, bool deep = true) const
				throw();

			/** Swapping of atomBondModelBaseProcessor's.
					Swap the  \link BaseModelProcessor BaseModelProcessor \endlink  of {\em *this} atomBondModelBaseProcessor
					with that of {\em processor}.
					Calls  \link BaseModelProcessor::swap BaseModelProcessor::swap \endlink 
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
					Clear the used  \link Atom Atom \endlink  objects.
					Calls  \link clearUsedAtoms_ clearUsedAtoms_ \endlink .
					Calls  \link BaseModelProcessor::start BaseModelProcessor::start \endlink .
					@return bool <tt>true</tt> if the start of {\em *this} atomBondModelBaseProcessor was successful
					@see    clearUsedAtoms_
					@see    operator()
					@see    BaseModelProcessor
			*/
			virtual bool start();
			
			/** Finish method.
					This method will be internally called from the processor mechanism if the processor
					has finished processing the  \link Composite Composite \endlink  tree.
					All previously inserted  \link Atom Atom \endlink  objects (inserted with the method  \link insertAtom_ insertAtom_ \endlink )
					will be processed with the method  \link buildBondModels_ buildBondModels_ \endlink  to create the graphical 
					representation of the  \link Bond Bond \endlink  objects.
					@return bool <tt>true</tt> if the finish of {\em *this} atomBondModelBaseProcessor was successful
					@see    buildBondModels_
					@see    operator()
					@see    Atom
					@see    Bond
					@see    Composite
			*/
			virtual bool finish();

			/**	Operator method.
					This method must be overridden to implement the creation of the different models.
					If a model should be created that uses  \link Atom Atom \endlink  and  \link Bond Bond \endlink  objects for
					structure (e.g. Ball And Stick Model) than the processed  \link Atom Atom \endlink  objects
					must be inserted into {\em *this} atomBondModelBaseProcessor with the method
					 \link insertAtom_ insertAtom_ \endlink . This method in correlation with the method  \link buildBondModels_ buildBondModels_ \endlink 
					will create the needed models for the connecting  \link Bond Bond \endlink  objects.
					@param  composite the  \link Composite Composite \endlink  object that will be processed
					@return Processor::Result the result of {\em *this} atomBondModelBaseProcessor
					@see    start
					@see    finish
					@see    insertAtom_
					@see    Composite
					@see    Atom
					@see    Bond
			*/
			virtual Processor::Result operator() (Composite& composite);
						
			//@}
			/**	@name	debuggers and diagnostics
			*/
			//@{

			/** Internal state and consistency self-validation.
					Initiate self-validation of the internal state and data structure consistencies
					of {\em *this} atomBondModelBaseProcessor.
					If the internal state of {\em *this} atomBondModelBaseProcessor is correct 
					(self-validated) and consistent <tt>true</tt> is returned, <tt>false</tt> otherwise. 
					{\em *this} atomBondModelBaseProcessor is valid if:
					 \link BaseModelProcessor BaseModelProcessor \endlink  is valid.
					Calls  \link BaseModelProcessor::isValid BaseModelProcessor::isValid \endlink .
					@return			bool <tt>true</tt> if the internal state of {\em *this} atomBondModelBaseProcessor is correct 
					@see       BaseModelProcessor
			*/
			virtual bool isValid() const
				throw();

			/** Internal value dump.
					Dump the current state of {\em *this} atomBondModelBaseProcessor to 
					the output ostream {\em s} with dumping depth {\em depth}.
					Calls  \link BaseModelProcessor::dump BaseModelProcessor::dump \endlink .
					@param   s output stream where to output the state of {\em *this} atomBondModelBaseProcessor
					@param   depth the dumping depth
					@see     BaseModelProcessor
			*/
			virtual void dump
				(std::ostream& s = std::cout, Size depth = 0) const
				throw();
							
			protected:

			//@}
			/** @name Protected members
					This methods are provided for easy generation of models. With the method 
					 \link insertAtom_ insertAtom_ \endlink  all  \link Atom Atom \endlink  objects that will be part of the newly created
					model will be stored. This stored atoms are later used (in the method 
					 \link buildBondModels_ buildBondModels_ \endlink ) to create the models of the  \link Bond Bond \endlink  objects that can
					be reached from the atoms.
					The other methods are access method for the stored atoms.			 
			*/
			//@{

			/** Insert atom for later processing.
					Insert  \link Atom Atom \endlink  {\em atom} into the used atoms structure. 
					All the inserted atoms will be later processed to generate the graphical representation
					of the  \link Bond Bond \endlink  objects that can be reached from each inserted atom.
					@param  atom the  \link Atom Atom \endlink  object to inserted
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
					Clear the structures for the used  \link Atom Atom \endlink  objects.
					@see    insertAtom_
					@see    getAtomList_
					@see    getAtomSet_
					@see    buildBondModels_
			*/
			void clearUsedAtoms_()
				throw();

			/** Mutable inspection of the list of used atoms.
					Access the mutable reference of the list of used  \link Atom Atom \endlink  objects inserted with
					the method  \link insertAtom_ insertAtom_ \endlink .
					@return  List<Atom*>& the list of used  \link Atom Atom \endlink  objects.
					@see    insertAtom_
					@see    clearUsedAtoms_
					@see    getAtomSet_
					@see    buildBondModels_
					@see    Atom
			*/
			List<Atom*>& getAtomList_()
				throw();

			/** Mutable inspection of the hash set of used atoms.
					Access the mutable reference of the hast set of used  \link Atom Atom \endlink  objects inserted with
					the method  \link insertAtom_ insertAtom_ \endlink .
					@return  HashSet<Atom*>& the hash set of used  \link Atom Atom \endlink  objects.
					@see    insertAtom_
					@see    clearUsedAtoms_
					@see    getAtomList_
					@see    buildBondModels_
					@see    Atom
			*/
			HashSet<Atom*>& getAtomSet_()
				throw();

			/** Build the bond models.
					This method is called from the  \link finish finish \endlink  method. It iterates over every  \link Bond Bond \endlink 
					object that can be reached from the stored  \link Atom Atom \endlink  objects and removes all
					previously attached  \link GeometricObject GeometricObject \endlink  objects and hosts the  \link BaseModelConnector BaseModelConnector \endlink 
					returned with the method  \link getModelConnector getModelConnector \endlink  on it.
					This method is the control method that is responsible for creating the needed models
					for  \link Bond Bond \endlink  objects between different models.
					See  \link BaseModelConnector BaseModelConnector \endlink  for more information about connecting different models.
			*/
			void buildBondModels_();
			//@}

			private:

			List<Atom*> used_atoms_;
			HashSet<Atom*> hashed_atoms_;
		};

#			ifndef BALL_NO_INLINE_FUNCTIONS
#				include <BALL/MOLVIEW/FUNCTOR/atomBondModelBaseProcessor.iC>
#			endif
  /** @} */
	} // namespace MOLVIEW

} // namespace BALL

#endif // BALL_MOLVIEW_FUNCTOR_ATOMBONDMODELBASEPROCESSOR_H 
