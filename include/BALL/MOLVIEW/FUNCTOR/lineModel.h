// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: lineModel.h,v 1.13 2003/02/25 15:53:35 sturm Exp $

#ifndef BALL_MOLVIEW_FUNCTOR_LINEMODEL_H
#define BALL_MOLVIEW_FUNCTOR_LINEMODEL_H

#ifndef BALL_VIEW_PRIMITIV_LINE_H
#	include <BALL/VIEW/PRIMITIV/line.h>
#endif

#ifndef BALL_VIEW_PRIMITIV_POINT_H
#	include <BALL/VIEW/PRIMITIV/point.h>
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
    /** \ingroup MolviewFunctorsModels
     *  @{
     */
		using VIEW::Point;
		using VIEW::Line;

		/** AddLineModel class.
				The class AddLineModel is a model processor that is responsible
				for creating a line model.
				This processor creates only the primitive objects without openGL implementation.
				The derived class  \link AddGLLineModel AddGLLineModel \endlink  uses the primitives with openGL implementation.
				For information about the processor concept see  \link Processor Processor \endlink  in tbe BALL documentation. \par
				<b>Definition:</b> BALL/MOLVIEW/FUNCTOR/lineModel.h
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
					@param       deep make a deep (=<tt>true</tt>) or shallow (=<tt>false</tt>) copy of {\em add_line_model}
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
					Calls  \link destroy destroy \endlink .
					@see         destroy
			*/
			virtual ~AddLineModel()
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
					addLineModel and changes the property of {\em *this} addLineModel from
					<tt>PROPERTY__DRAWING_MODE_SOLID</tt> to <tt>PROPERTY__DRAWING_MODE_WIREFRAME</tt>.
					See  \link GeometricObject GeometricObject \endlink  for information about properties.
					Calls  \link AtomBondModelBaseProcessor::start AtomBondModelBaseProcessor::start \endlink .
					@return bool <tt>true</tt> if the start of {\em *this} addLineModel was successful, <tt>false</tt> otherwise
					@see    operator()
					@see    AtomBondModelBaseProcessor
					@see    getModelConnector
					@see    BaseModelConnector
					@see    GeometricObject
			*/
			virtual bool start();
			
			/** Finish method.
					This method will be internally called from the processor mechanism if the processor
					has finished processing the  \link Composite Composite \endlink  tree.
					All previously inserted  \link Atom Atom \endlink  objects (inserted with the method  \link insertAtom_ insertAtom_ \endlink )
					will be processed with the method  \link buildBondModels_ buildBondModels_ \endlink  to create the graphical 
					representation of the  \link Bond Bond \endlink  objects.
					@return bool <tt>true</tt> if the finish of {\em *this} addLineModel was successful, <tt>false</tt> otherwise
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
					 \link Composite Composite \endlink  tree. If {\em composite} is of kind  \link Atom Atom \endlink  than a \Ref{Point}
					is created for that atom, appended to {\em composite} and inserted with the
					method  \link insertAtom_ insertAtom_ \endlink .
					All previously appended primitives to {\em composite}	are removed using the method
					 \link removeGeometricObjects_ removeGeometricObjects_ \endlink .
					The color for that  \link Point Point \endlink  object is calculated with the  \link ColorCalculator ColorCalculator \endlink 
					object retrieved with the method  \link getColorCalculator getColorCalculator \endlink .
					All atoms inserted with the method  \link insertAtom_ insertAtom_ \endlink  will later used for creating
					the model of the reachable  \link Bond Bond \endlink  objects. Those models will be created
					with the method  \link buildBondModels_ buildBondModels_ \endlink .
					@param  composite the  \link Composite Composite \endlink  object that will be processed
					@return Processor::Result the result of {\em *this} addLineModel
					@exeception OutOfMemory thrown if the memory allocation for a  \link Point Point \endlink  object failed
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
					Calls  \link AtomBondModelBaseProcessor::dump AtomBondModelBaseProcessor::dump \endlink .
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
					This methods are overridden by the class  \link AddGLLineModel AddGLLineModel \endlink 
					to create primitives with OpenGL implementation to generate the graphical
					representation of the shapes they represent.
					@see  AddGLLineModel
			*/
			//@{
			
			/** Create a point.
					Create a  \link Point Point \endlink  object.
					This method is overridden by the class  \link AddGLLineModel AddGLLineModel \endlink  to
					create a primitive with openGL implementation.
					The method  \link operator() operator() \endlink  uses this method to create a point
					for  \link Atom Atom \endlink  objects.
					@see  Point
					@see  AddGLLineModel
					@see  Atom
			*/
			virtual Point* createPoint_();

			/** Create a one colored line.
					Create a  \link Line Line \endlink  object.
					This method is overridden by the class  \link AddGLLineModel AddGLLineModel \endlink  to
					create a primitive with openGL implementation.
					The method  \link operator() operator() \endlink  uses this method to create one colored
					lines for  \link Bond Bond \endlink  objects whose  \link Atom Atom \endlink  objects have the same
					element types and thus the same color.
					@see  Line
					@see  AddGLLineModel
					@see  Atom
					@see  Bond
			*/
			virtual Line* createLine_();

			/** Create a two colored line.
					Create a  \link TwoColoredLine TwoColoredLine \endlink  object.
					This method is overridden by the class  \link AddGLLineModel AddGLLineModel \endlink  to
					create a primitive with openGL implementation.
					The method  \link operator() operator() \endlink  uses this method to create two colored
					lines for  \link Bond Bond \endlink  objects whose  \link Atom Atom \endlink  objects have different
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


#	ifndef BALL_NO_INLINE_FUNCTIONS
#		include <BALL/MOLVIEW/FUNCTOR/lineModel.iC>
#	endif
  /** @} */
	} // namespace MOLVIEW

} // namspace BALL

#endif // BALL_MOLVIEW_FUNCTOR_LINEMODEL_H
