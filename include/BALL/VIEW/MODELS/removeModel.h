// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: removeModel.h,v 1.2 2003/08/26 18:35:34 amoll Exp $

#ifndef BALL_VIEW_MODELS_REMOVEMODEL_H
#define BALL_VIEW_MODELS_REMOVEMODEL_H

#ifndef BALL_VIEW_MODELS_ATOMBONDMODELBASEPROCESSOR_H
#	include <BALL/VIEW/MODELS/atomBondModelBaseProcessor.h>
#endif

namespace BALL
{
	namespace VIEW
	{
		/** RemoveModel class.
				The class RemoveModel is a processor that is responsible for removing models.
				It is started on a  \link Composite Composite \endlink  object and removes all geometric primitives
				that are located on  \link Atom Atom \endlink  and  \link Bond Bond \endlink  objects and that have a model
				property. The method used for this action is  \link removeGeometricObjects_ removeGeometricObjects_ \endlink .
				For information about the processor concept see  \link Processor Processor \endlink  in tbe BALL
				documentation.  \par
				
    		\ingroup  MolviewModels
		*/
		class RemoveModel: 
			public AtomBondModelBaseProcessor,
			public UnaryProcessor<Composite*>
		{
			public:

			/**	@name	Constructors
			*/	
			//@{

			/** Default Constructor.
					Construct new removeModel.
					@return      RemoveModel new constructed removeModel
					@see         AtomBondModelBaseProcessor
			*/
			RemoveModel()
				throw();

			/** Copy constructor.
					Construct new removeModel by copying the removeModel
					{\em add_van_der_waals_model}.
					@param       model the removeModel to be copied
					@param       deep make a deep (=<tt>true</tt>) or shallow (=<tt>false</tt>) copy of {\em model}
					@return      RemoveModel new constructed removeModel copied from {\em model}
					@see         AtomBondModelBaseProcessor
			*/
			RemoveModel
				(const RemoveModel& model, bool deep = true)
				throw();

			//@}
			/** @name Destructors 
			*/
			//@{

			/** Destructor.
					Default destruction of {\em *this} removeModel.
					Calls  \link destroy destroy \endlink .
					@see         destroy
			*/
			virtual ~RemoveModel()
				throw();

			/** Explicit default initialization.
					Calls  \link AtomBondModelBaseProcessor::clear AtomBondModelBaseProcessor::clear \endlink .
					@see  AtomBondModelBaseProcessor
			*/
			virtual void clear()
				throw();

			/** Explicit destructor.
					Empty for further purpose.
			*/
			virtual void destroy()
				throw();

			//@}
			/**	@name Processor specific methods
			*/
			//@{
			
			/** Start method.
					Clear the  \link FindGeometricObjects FindGeometricObjects \endlink .
					Calls  \link AtomBondModelBaseProcessor::start AtomBondModelBaseProcessor::start \endlink .
					@return bool <tt>true</tt> if the start of {\em *this} removeModel was successful, <tt>false</tt> otherwise
					@see    operator()
					@see    AtomBondModelBaseProcessor
					@see    getSearcher_
					@see    FindGeometricObjects
			*/
			virtual bool start();
			
			/** Finish method.
					This method will be internally called from the processor mechanism if the processor
					has finished processing the  \link Composite Composite \endlink  tree.
					Iterates over all previously inserted  \link Atom Atom \endlink  objects 
					(inserted with the method  \link insertAtom_ insertAtom_ \endlink ) and their reachable  \link Bond Bond \endlink  objects
					and removes all  \link GeometricObject GeometricObject \endlink  objects with the method 
					 \link removeGeometricObjects_ removeGeometricObjects_ \endlink .
					@return bool <tt>true</tt> if the finish of {\em *this} removeModel was successful, <tt>false</tt> otherwise
					@see    insertAtom_
					@see    operator()
					@see    removeGeometricObjects_
					@see    GeometricObject
					@see    Atom
					@see    Bond
					@see    Composite
			*/
			virtual bool finish();
			
			/**	Operator method.
					This method iterates over each  \link Composite Composite \endlink  object reachable in the 
					 \link Composite Composite \endlink  tree. If {\em composite} is of kind  \link Atom Atom \endlink  than 
					 \link GeometricObject GeometricObject \endlink  objects are searched that are already appended to the atom
					object. This is accomplished with the  \link FindGeometricObjects FindGeometricObjects \endlink  object accessed
					with the method  \link getSearcher_ getSearcher_ \endlink .
					If geometric objects are available that have a model property that atom
					is inserted with the method  \link insertAtom_ insertAtom_ \endlink . The removal of geometricObjects
					is done in the  \link finish finish \endlink  method.
					@param  composite the  \link Composite Composite \endlink  object that will be processed
					@return Processor::Result the result of {\em *this} removeModel
					@see    start
					@see    finish
					@see    insertAtom_
					@see    FindGeometricObjects
					@see    getSearcher_
					@see    AtomBondModelBaseProcessor
					@see    GeometricObject
					@see    Composite
					@see    Atom
			*/
			virtual Processor::Result operator() (Composite& composite);

			///
			virtual Processor::Result operator() (Composite*& composite)
			{ return operator() (*composite);}

			//@}
			/**	@name	debuggers and diagnostics
			*/
			//@{

			/** Internal value dump.
					Dump the current state of {\em *this} removeModel to 
					the output ostream {\em s} with dumping depth {\em depth}.
					Calls  \link AtomBondModelBaseProcessor::dump AtomBondModelBaseProcessor::dump \endlink .
					@param   s output stream where to output the state of {\em *this} removeModel
					@param   depth the dumping depth
					@see     AtomBondModelBaseProcessor
			*/
			virtual void dump
				(std::ostream& s = std::cout, Size depth = 0) const
				throw();
			//@}

			private:
		};
	} // namespace VIEW

} // namespace BALL

#endif // BALL_VIEW_MODELS_REMOVEMODEL_H
