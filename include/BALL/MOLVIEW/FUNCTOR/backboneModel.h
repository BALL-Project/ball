// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: backboneModel.h,v 1.10 2003/03/03 14:18:03 anhi Exp $

#ifndef BALL_MOLVIEW_FUNCTOR_BACKBONEMODEL_H
#define BALL_MOLVIEW_FUNCTOR_BACKBONEMODEL_H

#ifndef BALL_DATATYPE_LIST_H
#	include <BALL/DATATYPE/list.h>
#endif

#ifndef BALL_MOLVIEW_FUNCTOR_BASEMODEL_H
#	include <BALL/MOLVIEW/FUNCTOR/baseModel.h>
#endif

#ifndef BALL_MOLVIEW_PRIMITIV_BACKBONE_H
#	include <BALL/MOLVIEW/PRIMITIV/backbone.h>
#endif


namespace BALL
{
	class Atom;
	
	namespace VIEW
	{
		class ColorRGBA;
	}

	namespace MOLVIEW
	{
    /**  \addtogroup  MolviewFunctorsModels
     *  @{
     */
		/** AddBackboneModel class.
				The class AddBackboneModel is a model processor that creates a backbone model
				through the {\em CA}-atoms of the processed  \link Composite Composite \endlink  object.
				The created  \link Backbone Backbone \endlink  object will be inserted to the root of the 
				start  \link Composite Composite \endlink  object if the root is of kind  \link System System \endlink .
				This processor creates only the  \link Backbone Backbone \endlink  object  without openGL implementation.
				The derived class  \link AddGLBackboneModel AddGLBackboneModel \endlink  uses the  \link GLBackBone GLBackBone \endlink  with
				openGL implementation.
				For information about the processor concept see  \link Processor Processor \endlink  in tbe BALL
				documentation.  \par
				<b>Definition:</b> BALL/MOLVIEW/FUNCTOR/backboneModel.h
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
					@param       deep make a deep (=<tt>true</tt>) or shallow (=<tt>false</tt>) copy of {\em add_Backbone_model}
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
					Calls  \link destroy destroy \endlink .
					@see         destroy
			*/
			virtual ~AddBackboneModel()
				throw();

			/** Explicit default initialization.
					Clears all internal structures.
					Calls  \link BaseModelProcessor::clear BaseModelProcessor::clear \endlink .
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
					Calls  \link BaseModelProcessor::start BaseModelProcessor::start \endlink .
					@return bool <tt>true</tt> if the start of {\em *this} addBackboneModel was successful, <tt>false</tt> otherwise
					@see    operator()
					@see    BaseModelProcessor
			*/
			virtual bool start();
			
			/** Finish method.
					This method will be internally called from the processor mechanism if the processor
					has finished processing the  \link Composite Composite \endlink  tree.
					All previously inserted  \link Atom Atom \endlink  objects and their calculated colores
					(inserted in the method  \link operator() operator() \endlink ) will be used to create a  \link Backbone Backbone \endlink 
					object.
					The created backbone will be appended to the root of the start  \link Composite Composite \endlink 
					if the root is of kind  \link System System \endlink .
					@return bool <tt>true</tt> if the finish of {\em *this} addBackboneModel was successful, <tt>false</tt> otherwise
					@exeception OutOfMemory thrown if the memory allocation for the  \link Backbone Backbone \endlink  object failed
					@see    Backbone
					@see    operator()
					@see    Atom
					@see    Bond
					@see    Composite
			*/
			virtual bool finish();
			
			/**	Operator method.
					This method iterates over each  \link Composite Composite \endlink  object reachable in the 
					 \link Composite Composite \endlink  tree. If {\em composite} is of kind  \link Atom Atom \endlink  and has the
					substring <tt>CA</tt> in its name (this method collects only {\em CA}-atoms) than
					that atom	is stored for later processing in the  \link finish finish \endlink  method.
					The color for that  \link Atom Atom \endlink  object is calculated with the  \link ColorCalculator ColorCalculator \endlink 
					object retrieved with the method  \link getColorCalculator getColorCalculator \endlink  and stored for later
					generation of the backbone model.
					The created backbone will be appended to the root of the  \link Composite Composite \endlink  object 
					that has started {\em *this} addBackboneModel.
					@param  composite the  \link Composite Composite \endlink  object that will be processed
					@return Processor::Result the result of {\em *this} addBackboneModel
					@see    ColorCalculator
					@see    getColorCalculator
					@see    start
					@see    finish
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
					Dump the current state of {\em *this} addBackboneModel to 
					the output ostream {\em s} with dumping depth {\em depth}.
					Calls  \link BaseModelProcessor::dump BaseModelProcessor::dump \endlink .
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
					This method is overridden by the class  \link AddGLBackboneModel AddGLBackboneModel \endlink 
					to create primitives with OpenGL implementation to generate the graphical
					representation of the shapes they represent.
					@see  AddGLBackboneModel
			*/
			//@{
			/** Create a backbone.
					Create a  \link Backbone Backbone \endlink  object.
					This method is overridden by the class  \link AddGLBackboneModel AddGLBackboneModel \endlink  to
					create a primitive with openGL implementation.
					The method  \link operator() operator() \endlink  uses this method to create a backbone
					through the {\em CA}  \link Atom Atom \endlink  objects.
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
  /** @} */
		};

	} // namespace MOLVIEW

} // namespace BALL

#endif // BALL_MOLVIEW_FUNCTOR_BACKBONEMODEL_H
