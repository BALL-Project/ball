// $Id: lineModel.h,v 1.9 2001/07/16 14:49:23 amoll Exp $

#ifndef BALL_MOLVIEW_FUNCTOR_LINEMODEL_H
#define BALL_MOLVIEW_FUNCTOR_LINEMODEL_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

#ifndef BALL_KERNEL_ATOM_H
#	include <BALL/KERNEL/atom.h>
#endif

#ifndef BALL_VIEW_KERNEL_GEOMETRICOBJECT_H
#	include <BALL/VIEW/KERNEL/geometricObject.h>
#endif

#ifndef BALL_VIEW_PRIMITIV_LINE_H
#	include <BALL/VIEW/PRIMITIV/line.h>
#endif

#ifndef BALL_VIEW_PRIMITIV_POINT_H
#	include <BALL/VIEW/PRIMITIV/point.h>
#endif

#ifndef BALL_MOLVIEW_COMMON_COMMON_H
#	include <BALL/MOLVIEW/COMMON/common.h>
#endif

#ifndef BALL_MOLVIEW_FUNCTOR_ATOMBONDMODELBASEPROCESSOR_H
#	include <BALL/MOLVIEW/FUNCTOR/atomBondModelBaseProcessor.h>
#endif

#ifndef BALL_MOLVIEW_PRIMITIV_TWOCOLOREDLINE_H
#	include <BALL/MOLVIEW/PRIMITIV/twoColoredLine.h>
#endif

namespace BALL
{
			
	namespace MOLVIEW
	{

		using VIEW::Point;
		using VIEW::Line;


		/** AddLineModel class.
				The class AddLineModel is a model processor that is responsible
				for creating a line model.
				This processor creates only the primitive objects without openGL implementation.
				The derived class \Ref{AddGLLineModel} uses the primitives with
				openGL implementation.
				For information about the processor concept see \Ref{Processor} in tbe BALL
				documentation. \\
				{\bf Definition:} \URL{BALL/MOLVIEW/FUNCTOR/lineModel.h}
		*/
		class AddLineModel: public AtomBondModelBaseProcessor
		{
			public:

			/**	@name	Constructors
			*/	
			//@{

			/** Default Constructor.
					Construct new addLineModel.
					@return      AddLineModel new constructed addLineModel
					@see         AtomBondModelBaseProcessor
			*/
			AddLineModel()
				throw();

			/** Copy constructor.
					Construct new addLineModel by copying the addLineModel
					{\em add_line_model}.
					@param       add_line_model the addLineModel to be copied
					@param       deep make a deep (={\tt true}) or shallow (={\tt false}) copy of {\em add_line_model}
					@return      AddLineModel new constructed addLineModel copied from {\em add_line_model}
					@see         AtomBondModelBaseProcessor
			*/
			AddLineModel(const AddLineModel& add_line_model, bool deep = true)
				throw();

			//@}

			/** @name Destructors 
			*/
			//@{

			/** Destructor.
					Default destruction of {\em *this} addLineModel.
					Calls \Ref{destroy}.
					@see         destroy
			*/
			virtual ~AddLineModel()
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
					addLineModel and changes the property of {\em *this} addLineModel from
					{\tt PROPERTY__DRAWING_MODE_SOLID} to {\tt PROPERTY__DRAWING_MODE_WIREFRAME}.
					See \Ref{GeometricObject} for information about properties.
					Calls \Ref{AtomBondModelBaseProcessor::start}.
					@return bool {\tt true} if the start of {\em *this} addLineModel was successful, {\tt false} otherwise
					@see    operator()
					@see    AtomBondModelBaseProcessor
					@see    getModelConnector
					@see    BaseModelConnector
					@see    GeometricObject
			*/
			virtual bool start();
			
			/** Finish method.
					This method will be internally called from the processor mechanism if the processor
					has finished processing the \Ref{Composite} tree.
					All previously inserted \Ref{Atom} objects (inserted with the method \Ref{insertAtom_})
					will be processed with the method \Ref{buildBondModels_} to create the graphical 
					representation of the \Ref{Bond} objects.
					@return bool {\tt true} if the finish of {\em *this} addLineModel was successful, {\tt false} otherwise
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
					\Ref{Composite} tree. If {\em composite} is of kind \Ref{Atom} than a \Ref{Point}
					is created for that atom, appended to {\em composite} and inserted with the
					method \Ref{insertAtom_}.
					All previously appended primitives to {\em composite}	are removed using the method
					\Ref{removeGeometricObjects_}.
					The color for that \Ref{Point} object is calculated with the \Ref{ColorCalculator}
					object retrieved with the method \Ref{getColorCalculator}.
					All atoms inserted with the method \Ref{insertAtom_} will later used for creating
					the model of the reachable \Ref{Bond} objects. Those models will be created
					with the method \Ref{buildBondModels_}.
					@param  composite the \Ref{Composite} object that will be processed
					@return Processor::Result the result of {\em *this} addLineModel
					@exeception OutOfMemory thrown if the memory allocation for a \Ref{Point} object failed
					@see    Point
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
					@see    Bond
			*/
			virtual Processor::Result operator() (Composite& composite);
			//@}

			/**	@name	debuggers and diagnostics
			*/
			//@{
			/** Internal value dump.
					Dump the current state of {\em *this} addLineModel to 
					the output ostream {\em s} with dumping depth {\em depth}.
					Calls \Ref{AtomBondModelBaseProcessor::dump}.
					@param   s output stream where to output the state of {\em *this} addLineModel
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
					This methods are overridden by the class \Ref{AddGLLineModel}
					to create primitives with OpenGL implementation to generate the graphical
					representation of the shapes they represent.
					@see  AddGLLineModel
			*/
			//@{
			/** Create a point.
					Create a \Ref{Point} object.
					This method is overridden by the class \Ref{AddGLLineModel} to
					create a primitive with openGL implementation.
					The method \Ref{operator()} uses this method to create a point
					for \Ref{Atom} objects.
					@see  Point
					@see  AddGLLineModel
					@see  Atom
			*/
			virtual Point* createPoint_();

			/** Create a one colored line.
					Create a \Ref{Line} object.
					This method is overridden by the class \Ref{AddGLLineModel} to
					create a primitive with openGL implementation.
					The method \Ref{operator()} uses this method to create one colored
					lines for \Ref{Bond} objects whose \Ref{Atom} objects have the same
					element types and thus the same color.
					@see  Line
					@see  AddGLLineModel
					@see  Atom
					@see  Bond
			*/
			virtual Line* createLine_();

			/** Create a two colored line.
					Create a \Ref{TwoColoredLine} object.
					This method is overridden by the class \Ref{AddGLLineModel} to
					create a primitive with openGL implementation.
					The method \Ref{operator()} uses this method to create two colored
					lines for \Ref{Bond} objects whose \Ref{Atom} objects have different
					element types and thus different colors.
					@see  TwoColoredLine
					@see  AddGLLineModel
					@see  Atom
					@see  Bond
			*/
			virtual TwoColoredLine* createTwoColoredLine_();
			//@}

			private:

		};

#			ifndef BALL_NO_INLINE_FUNCTIONS
#				include <BALL/MOLVIEW/FUNCTOR/lineModel.iC>
#			endif

	} // namespace MOLVIEW

} // namspace BALL

#endif // BALL_MOLVIEW_FUNCTOR_LINEMODEL_H
