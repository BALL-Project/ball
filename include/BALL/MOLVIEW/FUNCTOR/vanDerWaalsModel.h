// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: vanDerWaalsModel.h,v 1.15 2003/02/25 15:53:38 sturm Exp $

#ifndef BALL_MOLVIEW_FUNCTOR_VANDERWAALSMODEL_H
#define BALL_MOLVIEW_FUNCTOR_VANDERWAALSMODEL_H

#ifndef BALL_VIEW_PRIMITIV_SPHERE_H
#	include <BALL/VIEW/PRIMITIV/sphere.h>
#endif

#ifndef BALL_MOLVIEW_FUNCTOR_ATOMBONDMODELBASEPROCESSOR_H
# include <BALL/MOLVIEW/FUNCTOR/atomBondModelBaseProcessor.h>
#endif

namespace BALL
{
	namespace MOLVIEW
	{
    /** \ingroup MolviewFunctorsModels
     *  @{
     */
		using VIEW::Sphere;

		/** AddVanDerWaalsModel class.
				The class AddVanDerWaalsModel is a model processor that is responsible
				for creating a {\em Van der Waals} model.
				This processor creates only the primitive objects without openGL implementation.
				The derived class  \link AddGLVanDerWaalsModel AddGLVanDerWaalsModel \endlink  uses the primitives with openGL implementation.
				For information about the processor concept see  \link Processor Processor \endlink  in tbe BALL documentation. \par
				<b>Definition:</b> BALL/MOLVIEW/FUNCTOR/vanDerWaalsModel.h
		*/
		class AddVanDerWaalsModel: public AtomBondModelBaseProcessor
		{
			public:

			/**	@name	Constructors
			*/	
			//@{

			/** Default Constructor.
					Construct new addVanDerWaalsModel.
					@return      AddVanDerWaalsModel new constructed addVanDerWaalsModel
					@see         AtomBondModelBaseProcessor
			*/
			AddVanDerWaalsModel()
				throw();

			/** Copy constructor.
					Construct new addVanDerWaalsModel by copying the addVanDerWaalsModel
					{\em add_van_der_waals_model}.
					@param       add_van_der_waals_model the addVanDerWaalsModel to be copied
					@param       deep make a deep (=<tt>true</tt>) or shallow (=<tt>false</tt>) copy of {\em add_van_der_waals_model}
					@return      AddVanDerWaalsModel new constructed addVanDerWaalsModel copied from {\em add_van_der_waals_model}
					@see         AtomBondModelBaseProcessor
			*/
			AddVanDerWaalsModel
				(const AddVanDerWaalsModel& add_van_der_waals_model, bool deep = true)
				throw();

			//@}
			/** @name Destructors 
			*/
			//@{

			/** Destructor.
					Default destruction of {\em *this} addVanDerWaalsModel.
					Calls  \link destroy destroy \endlink .
					@see         destroy
			*/
			virtual ~AddVanDerWaalsModel()
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
					Initialize the properties of the  \link BaseModelConnector BaseModelConnector \endlink  to {\em *this}
					addVanDerWaalsModel.
					Calls  \link AtomBondModelBaseProcessor::start AtomBondModelBaseProcessor::start \endlink .
					@return bool <tt>true</tt> if the start of {\em *this} addVanDerWaalsModel was successful, <tt>false</tt> otherwise
					@see    operator()
					@see    AtomBondModelBaseProcessor
					@see    getModelConnector
					@see    BaseModelConnector
			*/
			virtual bool start();
			
			/** Finish method.
					This method will be internally called from the processor mechanism if the processor
					has finished processing the  \link Composite Composite \endlink  tree.
					All previously inserted  \link Atom Atom \endlink  objects (inserted with the method  \link insertAtom_ insertAtom_ \endlink )
					will be processed with the method  \link buildBondModels_ buildBondModels_ \endlink  to create the graphical 
					representation of the  \link Bond Bond \endlink  objects.
					@return bool <tt>true</tt> if the finish of {\em *this} addVanDerWaalsModel was successful, <tt>false</tt> otherwise
					@see    insertAtom_
					@see    buildBondModels_
					@see    operator()
					@see    Atom
					@see    Bond
					@see    Composite
			*/
			virtual bool finish();

			/**	Operator method.
					This method iterates over each  \link Composite Composite \endlink  object reachable in the 
					 \link Composite Composite \endlink  tree. If {\em composite} is of kind  \link Atom Atom \endlink  than a \Ref{Sphere}
					with radius  \link getVanDerWaalsRadius getVanDerWaalsRadius \endlink 
					is created for that atom, appended to {\em composite} and inserted with the
					method  \link insertAtom_ insertAtom_ \endlink .
					All previously appended primitives to {\em composite}	are removed using the method
					 \link removeGeometricObjects_ removeGeometricObjects_ \endlink .
					The color for that  \link Sphere Sphere \endlink  object is calculated with the  \link ColorCalculator ColorCalculator \endlink 
					object retrieved with the method  \link getColorCalculator getColorCalculator \endlink .
					All atoms inserted with the method  \link insertAtom_ insertAtom_ \endlink  will later used for creating
					the model of the reachable  \link Bond Bond \endlink  objects. Those models will be created
					with the method  \link buildBondModels_ buildBondModels_ \endlink .
					@param  composite the  \link Composite Composite \endlink  object that will be processed
					@return Processor::Result the result of {\em *this} addVanDerWaalsModel
					@exeception OutOfMemory thrown if the memory allocation for a  \link Sphere Sphere \endlink  object failed
					@see    Sphere
					@see    ColorCalculator
					@see    getColorCalculator
					@see    start
					@see    finish
					@see    insertAtom_
					@see    buildBondModels_
					@see    removeGeometricObjects_
					@see    AtomBondModelBaseProcessor
					@see    Composite
					@see    Atom
					@see    getVanDerWaalsRadius
					@see    Bond
			*/
			virtual Processor::Result operator() (Composite& composite);

			//@}
			/**	@name	debuggers and diagnostics
			*/
			//@{
			/** Internal value dump.
					Dump the current state of {\em *this} addVanDerWaalsModel to 
					the output ostream {\em s} with dumping depth {\em depth}.
					Calls  \link AtomBondModelBaseProcessor::dump AtomBondModelBaseProcessor::dump \endlink .
					@param   s output stream where to output the state of {\em *this} addVanDerWaalsModel
					@param   depth the dumping depth
					@see     AtomBondModelBaseProcessor
			*/
			virtual void dump
				(std::ostream& s = std::cout, Size depth = 0) const
				throw();
							

  		protected:

			//@}
			/** @name Creation methods
					This methods create primitives without OpenGL implementation.
					This methods are overridden by the class  \link AddGLVanDerWaalsModel AddGLVanDerWaalsModel \endlink 
					to create primitives with OpenGL implementation to generate the graphical
					representation of the shapes they represent.
					@see  AddGLVanDerWaalsModel
			*/
			//@{

			/** Create a sphere.
					Create a  \link Sphere Sphere \endlink  object.
					This method is overridden by the class  \link AddGLVanDerWaalsModel AddGLVanDerWaalsModel \endlink  to
					create a primitive with openGL implementation.
					The method  \link operator() operator() \endlink  uses this method to create a sphere
					for  \link Atom Atom \endlink  objects.
					@see  Sphere
					@see  AddGLVanDerWaalsModel
					@see  Atom
			*/
			virtual Sphere* createSphere_();
			//@}
		};
  /** @} */
	} // namespace MOLVIEW

} // namespace BALL

#endif // BALL_MOLVIEW_FUNCTOR_VANDERWAALSMODEL_H
