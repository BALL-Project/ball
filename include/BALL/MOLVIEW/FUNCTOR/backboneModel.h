// $Id: backboneModel.h,v 1.3 2001/05/13 14:55:21 hekl Exp $

#ifndef BALL_MOLVIEW_FUNCTOR_BACKBONEMODEL_H
#define BALL_MOLVIEW_FUNCTOR_BACKBONEMODEL_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

#ifndef BALL_DATATYPE_LIST_H
#	include <BALL/DATATYPE/list.h>
#endif

#ifndef BALL_KERNEL_ATOM_H
#	include <BALL/KERNEL/atom.h>
#endif

#ifndef BALL_KERNEL_FOREACH_H
#	include <BALL/KERNEL/forEach.h>
#endif

#ifndef BALL_VIEW_DATATYPE_COLOR_H
#	include <BALL/VIEW/DATATYPE/color.h>
#endif

#ifndef BALL_VIEW_KERNEL_GEOMETRICOBJECT_H
#	include <BALL/VIEW/KERNEL/geometricObject.h>
#endif

#ifndef BALL_MOLVIEW_COMMON_COMMON_H
#	include <BALL/MOLVIEW/COMMON/common.h>
#endif

#ifndef BALL_MOLVIEW_FUNCTOR_BASEMODEL_H
#	include <BALL/MOLVIEW/FUNCTOR/baseModel.h>
#endif

#ifndef BALL_MOLVIEW_PRIMITIV_BACKBONE_H
#	include <BALL/MOLVIEW/PRIMITIV/backbone.h>
#endif


namespace BALL
{
	
	namespace MOLVIEW
	{

		/** AddBackboneModel class.
				{\bf Framework:} BALL/MOLVIEW/FUNCTOR\\
				{\bf Definition:} \URL{BALL/MOLVIEW/FUNCTOR/backboneModel.h}\\ \\
				The class AddBackboneModel is a model processor that creates a backbone model
				through the {\em CA}-atoms of the processed \Ref{Composite} object.
				The created \Ref{Backbone} object will be inserted to the root of the 
				start \Ref{Composite} object if the root is of kind \Ref{System}.
				This processor creates only the \Ref{Backbone} object  without openGL implementation.
				The derived class \Ref{AddGLBackboneModel} uses the \Ref{GLBackBone} with
				openGL implementation.
				For information about the processor concept see \Ref{Processor} in tbe BALL
				documentation.
				@memo    AddBackboneModel class (BALL MOLVIEW functor framework)
				@author  $Author: hekl $
				@version $Revision: 1.3 $
				@date    $Date: 2001/05/13 14:55:21 $
		*/
		class AddBackboneModel: public BaseModelProcessor
		{

			public:

			/**	@name	Constructors
			*/	
			//@{

			/** Default Constructor.
					Construct new addBackboneModel.
					@return      AddBackboneModel new constructed addBackboneModel
					@see         BaseModelProcessor
			*/
			AddBackboneModel()
				throw();

			/** Copy constructor.
					Construct new addBackboneModel by copying the addBackboneModel
					{\em add_Backbone_model}.
					@param       add_Backbone_model the addBackboneModel to be copied
					@param       deep make a deep (={\tt true}) or shallow (={\tt false}) copy of {\em add_Backbone_model}
					@return      AddBackboneModel new constructed addBackboneModel copied from {\em add_Backbone_model}
					@see         BaseModelProcessor
			*/
			AddBackboneModel
				(const AddBackboneModel& add_Backbone_model, bool deep = true)
				throw();

			//@}

			/** @name Destructors 
			*/
			//@{

			/** Destructor.
					Default destruction of {\em *this} addBackboneModel.
					Calls \Ref{destroy}.
					@see         destroy
			*/
			virtual ~AddBackboneModel()
				throw();

			/** Explicit default initialization.
					Clears all internal structures.
					Calls \Ref{BaseModelProcessor::clear}.
					@see  BaseModelProcessor
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
					Clear all internal structures.
					Calls \Ref{BaseModelProcessor::start}.
					@return bool {\tt true} if the start of {\em *this} addBackboneModel was successful, {\tt false} otherwise
					@see    operator()
					@see    BaseModelProcessor
			*/
			virtual bool start()
				throw();
			
			/** Finish method.
					This method will be internally called from the processor mechanism if the processor
					has finished processing the \Ref{Composite} tree.
					All previously inserted \Ref{Atom} objects and their calculated colores
					(inserted in the method \Ref{operator()}) will be used to create a \Ref{Backbone}
					object.
					The created backbone will be appended to the root of the start \Ref{Composite}
					if the root is of kind \Ref{System}.
					@return bool {\tt true} if the finish of {\em *this} addBackboneModel was successful, {\tt false} otherwise
					@exeception OutOfMemory thrown if the memory allocation for the \Ref{Backbone} object failed
					@see    Backbone
					@see    operator()
					@see    Atom
					@see    Bond
					@see    Composite
			*/
			virtual bool finish()
				throw(Exception::OutOfMemory);
			
			/**	Operator method.
					This method iterates over each \Ref{Composite} object reachable in the 
					\Ref{Composite} tree. If {\em composite} is of kind \Ref{Atom} and has the
					substring {\tt CA} in its name (this method collects only {\em CA}-atoms) than
					that atom	is stored for later processing in the \Ref{finish} method.
					The color for that \Ref{Atom} object is calculated with the \Ref{ColorCalculator}
					object retrieved with the method \Ref{getColorCalculator} and stored for later
					generation of the backbone model.
					The created backbone will be appended to the root of the \Ref{Composite} object 
					that has started {\em *this} addBackboneModel.
					@param  composite the \Ref{Composite} object that will be processed
					@return Processor::Result the result of {\em *this} addBackboneModel
					@see    ColorCalculator
					@see    getColorCalculator
					@see    start
					@see    finish
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
			/** Internal value dump.
					Dump the current state of {\em *this} addBackboneModel to 
					the output ostream {\em s} with dumping depth {\em depth}.
					Calls \Ref{BaseModelProcessor::dump}.
					@param   s output stream where to output the state of {\em *this} addBackboneModel
					@param   depth the dumping depth
					@see     BaseModelProcessor
			*/
			virtual void dump
				(std::ostream& s = std::cout, Size depth = 0) const
				throw();

			//@}


		  protected:

			/** @name Creation method
					This method creates a primitive without OpenGL implementation.
					This method is overridden by the class \Ref{AddGLBackboneModel}
					to create primitives with OpenGL implementation to generate the graphical
					representation of the shapes they represent.
					@see  AddGLBackboneModel
			*/
			//@{
			/** Create a backbone.
					Create a \Ref{Backbone} object.
					This method is overridden by the class \Ref{AddGLBackboneModel} to
					create a primitive with openGL implementation.
					The method \Ref{operator()} uses this method to create a backbone
					through the {\em CA} \Ref{Atom} objects.
					@see  Backbone
					@see  AddGLBackboneModel
					@see  Atom
			*/
			virtual Backbone *createBackbone_();
			//@}

			private:

			//_
			bool get_composite_;

			//_
			Composite* start_composite_;

			//_ list of used atoms for the creation of the backbone
			List<Atom*> atoms_;

			//_ list of colors correlating to the atoms saved in atoms_.
			List<ColorRGBA> atoms_color_;

		};

#			ifndef BALL_NO_INLINE_FUNCTIONS
#				include <BALL/MOLVIEW/FUNCTOR/backboneModel.iC>
#			endif

	} // namespace MOLVIEW

} // namespace BALL

#endif // BALL_MOLVIEW_FUNCTOR_BACKBONEMODEL_H
