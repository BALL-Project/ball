// $Id: vanDerWaalsModel.h,v 1.11 2001/07/16 14:49:23 amoll Exp $

#ifndef BALL_MOLVIEW_FUNCTOR_VANDERWAALSMODEL_H
#define BALL_MOLVIEW_FUNCTOR_VANDERWAALSMODEL_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

#ifndef BALL_KERNEL_ATOM_H
#	include <BALL/KERNEL/atom.h>
#endif

#ifndef BALL_KERNEL_PTE_H
#	include <BALL/KERNEL/PTE.h>
#endif

#ifndef BALL_VIEW_KERNEL_GEOMETRICOBJECT_H
#	include <BALL/VIEW/KERNEL/geometricObject.h>
#endif

#ifndef BALL_VIEW_PRIMITIV_SPHERE_H
#	include <BALL/VIEW/PRIMITIV/sphere.h>
#endif

#ifndef BALL_MOLVIEW_COMMON_COMMON_H
#	include <BALL/MOLVIEW/COMMON/common.h>
#endif

#ifndef BALL_MOLVIEW_FUNCTOR_ATOMBONDMODELBASEPROCESSOR_H
#include <BALL/MOLVIEW/FUNCTOR/atomBondModelBaseProcessor.h>
#endif

namespace BALL
{
	namespace MOLVIEW
	{
		using VIEW::Sphere;

		/** AddVanDerWaalsModel class.
				The class AddVanDerWaalsModel is a model processor that is responsible
				for creating a {\em Van der Waals} model.
				This processor creates only the primitive objects without openGL implementation.
				The derived class \Ref{AddGLVanDerWaalsModel} uses the primitives with
				openGL implementation.
				For information about the processor concept see \Ref{Processor} in tbe BALL
				documentation. \\
				{\bf Definition:} \URL{BALL/MOLVIEW/FUNCTOR/vanDerWaalsModel.h}
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
					@param       deep make a deep (={\tt true}) or shallow (={\tt false}) copy of {\em add_van_der_waals_model}
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
					Calls \Ref{destroy}.
					@see         destroy
			*/
			virtual ~AddVanDerWaalsModel()
				throw();

			/** Explicit default initialization.
					Calls \Ref{AtomBondModelBaseProcessor::clear}.
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
					Initialize the properties of the \Ref{BaseModelConnector} to {\em *this}
					addVanDerWaalsModel.
					Calls \Ref{AtomBondModelBaseProcessor::start}.
					@return bool {\tt true} if the start of {\em *this} addVanDerWaalsModel was successful, {\tt false} otherwise
					@see    operator()
					@see    AtomBondModelBaseProcessor
					@see    getModelConnector
					@see    BaseModelConnector
			*/
			virtual bool start();
			
			/** Finish method.
					This method will be internally called from the processor mechanism if the processor
					has finished processing the \Ref{Composite} tree.
					All previously inserted \Ref{Atom} objects (inserted with the method \Ref{insertAtom_})
					will be processed with the method \Ref{buildBondModels_} to create the graphical 
					representation of the \Ref{Bond} objects.
					@return bool {\tt true} if the finish of {\em *this} addVanDerWaalsModel was successful, {\tt false} otherwise
					@see    insertAtom_
					@see    buildBondModels_
					@see    operator()
					@see    Atom
					@see    Bond
					@see    Composite
			*/
			virtual bool finish();

			/**	Operator method.
					This method iterates over each \Ref{Composite} object reachable in the 
					\Ref{Composite} tree. If {\em composite} is of kind \Ref{Atom} than a \Ref{Sphere}
					with radius \Ref{getVanDerWaalsRadius}
					is created for that atom, appended to {\em composite} and inserted with the
					method \Ref{insertAtom_}.
					All previously appended primitives to {\em composite}	are removed using the method
					\Ref{removeGeometricObjects_}.
					The color for that \Ref{Sphere} object is calculated with the \Ref{ColorCalculator}
					object retrieved with the method \Ref{getColorCalculator}.
					All atoms inserted with the method \Ref{insertAtom_} will later used for creating
					the model of the reachable \Ref{Bond} objects. Those models will be created
					with the method \Ref{buildBondModels_}.
					@param  composite the \Ref{Composite} object that will be processed
					@return Processor::Result the result of {\em *this} addVanDerWaalsModel
					@exeception OutOfMemory thrown if the memory allocation for a \Ref{Sphere} object failed
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
					Calls \Ref{AtomBondModelBaseProcessor::dump}.
					@param   s output stream where to output the state of {\em *this} addVanDerWaalsModel
					@param   depth the dumping depth
					@see     AtomBondModelBaseProcessor
			*/
			virtual void dump
				(std::ostream& s = std::cout, Size depth = 0) const
				throw();
			//@}

  		protected:

			/** @name Creation methods
					This methods create primitives without OpenGL implementation.
					This methods are overridden by the class \Ref{AddGLVanDerWaalsModel}
					to create primitives with OpenGL implementation to generate the graphical
					representation of the shapes they represent.
					@see  AddGLVanDerWaalsModel
			*/
			//@{
			/** Create a sphere.
					Create a \Ref{Sphere} object.
					This method is overridden by the class \Ref{AddGLVanDerWaalsModel} to
					create a primitive with openGL implementation.
					The method \Ref{operator()} uses this method to create a sphere
					for \Ref{Atom} objects.
					@see  Sphere
					@see  AddGLVanDerWaalsModel
					@see  Atom
			*/
			virtual Sphere* createSphere_();
			//@}
		};

#			ifndef BALL_NO_INLINE_FUNCTIONS
#				include <BALL/MOLVIEW/FUNCTOR/vanDerWaalsModel.iC>
#			endif

	} // namespace MOLVIEW

} // namespace BALL

#endif // BALL_MOLVIEW_FUNCTOR_VANDERWAALSMODEL_H
