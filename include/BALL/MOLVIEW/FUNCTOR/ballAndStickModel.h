// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: ballAndStickModel.h,v 1.14 2003/02/25 15:53:34 sturm Exp $

#ifndef BALL_MOLVIEW_FUNCTOR_BALLANDSTICKMODEL_H
#define BALL_MOLVIEW_FUNCTOR_BALLANDSTICKMODEL_H

#ifndef BALL_VIEW_PRIMITIV_SPHERE_H
#	include <BALL/VIEW/PRIMITIV/sphere.h>
#endif

#ifndef BALL_VIEW_PRIMITIV_TUBE_H
#	include <BALL/VIEW/PRIMITIV/tube.h>
#endif

#ifndef BALL_MOLVIEW_FUNCTOR_ATOMBONDMODELBASEPROCESSOR_H
#	include <BALL/MOLVIEW/FUNCTOR/atomBondModelBaseProcessor.h>
#endif

#ifndef BALL_MOLVIEW_PRIMITIV_TWOCOLOREDTUBE_H
#	include <BALL/MOLVIEW/PRIMITIV/twoColoredTube.h>
#endif

namespace BALL
{
	class Atom;

	namespace MOLVIEW
	{
    /** \ingroup MolviewFunctorsModels
     *  @{
     */
		using VIEW::Sphere;
		using VIEW::Tube;

		/** AddBallAndStickModel class.
				The class AddBallAndStickModel is a model processor that is responsible
				for creating either a {\em ball and stick} model or only a {\em stick} model.
				In a ball and stick model the graphical representation of the  \link Atom Atom \endlink  objects
				( \link Sphere Sphere \endlink  objects) have a different radius than the representation of
				the  \link Bond Bond \endlink  objects ( \link Tube Tube \endlink  objects). Otherwise in the stick model
				the radi of the ball and the stick components are equal.
				This processor creates only the primitive objects without openGL implementation.
				The derived class  \link AddGLBallAndStickModel AddGLBallAndStickModel \endlink  uses the primitives with
				openGL implementation.
				For information about the processor concept see  \link Processor Processor \endlink  in tbe BALL
				documentation.  \par
				<b>Definition:</b> BALL/MOLVIEW/FUNCTOR/ballAndStickModel.h
		*/
		class AddBallAndStickModel: public AtomBondModelBaseProcessor
		{
			public:

			/**	@name	Constructors
			*/	
			//@{

			/** Default Constructor.
					Construct new addBallAndStickModel.
					Set the appearance to:
					\begin{itemize}
					  \item  set the appearance to {\em ball and stick} model
						\item  set the ball radius to <tt>0.4</tt>
						\item  set the stick radius to <tt>0.2</tt>
					\end{itemize}
					@return      AddBallAndStickModel new constructed addBallAndStickModel
					@see         AtomBondModelBaseProcessor
			*/
			AddBallAndStickModel()
				throw();

			/** Copy constructor.
					Construct new addBallAndStickModel by copying the addBallAndStickModel
					{\em add_ball_and_stick_model}.
					Copy the appearance of {\em add_ball_and_stick_model} to {\em *this}
					addBallAndStickModel.
					@param       add_ball_and_stick_model the addBallAndStickModel to be copied
					@param       deep make a deep (=<tt>true</tt>) or shallow (=<tt>false</tt>) copy of {\em add_ball_and_stick_model}
					@return      AddBallAndStickModel new constructed addBallAndStickModel copied from {\em add_ball_and_stick_model}
					@see         AtomBondModelBaseProcessor
			*/
			AddBallAndStickModel
				(const AddBallAndStickModel& add_ball_and_stick_model, bool deep = true)
				throw();

			//@}

			/** @name Destructors 
			*/
			//@{

			/** Destructor.
					Default destruction of {\em *this} addBallAndStickModel.
					Calls  \link destroy destroy \endlink .
					@see         destroy
			*/
			virtual ~AddBallAndStickModel()
				throw();

			/** Explicit default initialization.
					Reset the state of {\em *this} addBallAndStickModel to:
					\begin{itemize}
					  \item  set the appearance to {\em ball and stick} model
						\item  set the ball radius to <tt>0.4</tt>
						\item  set the stick radius to <tt>0.2</tt>
					  \item  clear the  \link BaseModelProcessor BaseModelProcessor \endlink 
					\end{itemize}
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


			/**	@name	Assignment
			*/
			//@{

			/** Assignment.
					Assign the addBallAndStickModel {\em add_ball_and_stick_model} to {\em *this} 
					addBallAndStickModel.
					Initialize the appearance of {\em *this} addBallAndStickModel to the appearance
					of {\em add_ball_and_stick_model}.
					Calls  \link AtomBondModelBaseProcessor::set AtomBondModelBaseProcessor::set \endlink .
					@param       add_ball_and_stick_model the addBallAndStickModel to be copied 
					@param       deep make a deep (=<tt>true</tt>) or shallow (=<tt>false</tt>) copy of {\em add_ball_and_stick_model}
					@see         setBallRadius
					@see         setStickRadius
					@see         enableBallAndStickModel
					@see         enableStickModel
					@see         AtomBondModelBaseProcessor
			*/
			void set
				(const AddBallAndStickModel& add_ball_and_stick_model, bool deep = true)
				throw();

			/** Assignment operator.
					Assign the addBallAndStickModel {\em add_ball_and_stick_model} to {\em *this} 
					addBallAndStickModel.
					Initialize the appearance of {\em *this} addBallAndStickModel to the appearance
					of {\em add_ball_and_stick_model}.
					Calls  \link set set \endlink .
					The assignment will be deep.
					@param       add_ball_and_stick_model the addBallAndStickModel to be copied
					@return      AddBallAndStickModel& constant reference of {\em *this} addBallAndStickModel
					@see         set
			*/
			const AddBallAndStickModel& operator =
				(const AddBallAndStickModel& add_ball_and_stick_model)
				throw();

			/** Copying.
					Copy {\em *this} addBallAndStickModel to the addBallAndStickModel
					Initialize the appearance of {\em *this} addBallAndStickModel to the appearance
					of {\em add_ball_and_stick_model}.
					Calls  \link set set \endlink .
					@param       add_ball_and_stick_model the addBallAndStickModel to be assigned to
					@param       deep make a deep (=<tt>true</tt>) or shallow (=<tt>false</tt>) copy of {\em add_ball_and_stick_model}
					@see         set
			*/
			void get
				(AddBallAndStickModel& add_ball_and_stick_model, bool deep = true) const
				throw();

			/** Swapping of addBallAndStickModel's.
					Swap the  \link AtomBondModelBaseProcessor AtomBondModelBaseProcessor \endlink  and the appearance of {\em *this}
					addBallAndStickModel
					with that of {\em add_ball_and_stick_model}.
					Calls  \link AtomBondModelBaseProcessor::swap AtomBondModelBaseProcessor::swap \endlink 
					@param       add_ball_and_stick_model the addBallAndStickModel being swapped with {\em *this} addBallAndStickModel 
					@see         AtomBondModelBaseProcessor
					@see         setBallRadius
					@see         setStickRadius
					@see         enableBallAndStickModel
					@see         enableStickModel
					@see         AtomBondModelBaseProcessor
			*/
			void swap(AddBallAndStickModel& add_ball_and_stick_model)
				throw();
			//@}

			/**	@name	Accessors: inspectors and mutators 
					This methods are used to specify the appearance of the model. This processor
					can either be used for creating a {\em ball and stick} model or a {\em stick} model.
					For the ball-component (the graphical representation of the  \link Atom Atom \endlink  object) and
					the stick-component (the graphical representation of the   \link Bond Bond \endlink  object)
					different radi can be assigned.
					@see   Atom
					@see   Bond
			*/
			//@{

			/** Change the radius of the ball-component.
					Change the radius of the ball-component (the graphical representation of 
					the  \link Atom Atom \endlink  object) to the value of the parameter {\em radius}.
					@param       radius the new radius of the ball-component: (<tt>radius > 0</tt>)
					@exeception  OutOfRange thrown if <tt>radius <= 0</tt>
					@see         getBallRadius
					@see         Atom
			*/
			void setBallRadius(const Real radius)
				throw(Exception::OutOfRange);

			/** Inspection of the radius of the ball-component.
					Access the radius of the ball-component of {\em *this} addBallAndStickModel.
					@see    setBallRadius
			*/
			Real getBallRadius() const
				throw();

			/** Copy the radius of the ball-component to real.
					Copy the radius of the ball-component of {\em *this} addBallAndStickModel
					to the real {\em radius}.
					@param  radius the real receiving the radius of the ball-component
					@see    setBallRadius
			*/
			void getBallRadius(Real& radius)
				throw();

			/** Change the radius of the stick-component.
					Change the radius of the stick-component (the graphical representation of 
					the  \link Bond Bond \endlink  object) to the value of the parameter {\em radius}.
					@param       radius the new radius of the stick-component: (<tt>radius > 0</tt>)
					@exeception  OutOfRange thrown if <tt>radius <= 0</tt>
					@see         getStickRadius
					@see         Bond
			*/
			void setStickRadius(const Real radius)
				throw(Exception::OutOfRange);

			/** Inspection of the radius of the stick-component.
					Access the radius of the stick-component of {\em *this} addBallAndStickModel.
					@see    setStickRadius
			*/
			Real getStickRadius() const
				throw();

			/** Copy the radius of the stick-component to real.
					Copy the radius of the stick-component of {\em *this} addBallAndStickModel to
					the real {\em radius}.
					@param  radius the real receiving the radius of the stick-component
					@see    setStickRadius
			*/
			void getStickRadius(Real& radius)
				throw();


			/** Enable the creation of the ball and stick model.
					Enables {\em *this} addBallAndStickModel to create a ball and stick model.
					@see    enableStickModel
					@see    isBallAndStickModel
			*/
			void enableBallAndStickModel()
				throw();

			/** Enable the creation of the stick model.
					Enables {\em *this} addBallAndStickModel to create a stick model.
					@see    enableBallAndStickModel
					@see    isStickModel
			*/
			void enableStickModel()
				throw();
			//@}

			/**	@name Processor specific methods
			*/
			//@{
			/** Start method.
					Initialize the properties of the  \link BaseModelConnector BaseModelConnector \endlink  to {\em *this}
					addBallAndStickModel.
					Calls  \link AtomBondModelBaseProcessor::start AtomBondModelBaseProcessor::start \endlink .
					@return bool <tt>true</tt> if the start of {\em *this} addBallAndStickModel was successful, <tt>false</tt> otherwise
					@see    operator()
					@see    getModelConnector
					@see    AtomBondModelBaseProcessor
					@see    BaseModelConnector
			*/
			virtual bool start();
			
			/** Finish method.
					This method will be internally called from the processor mechanism if the processor
					has finished processing the  \link Composite Composite \endlink  tree.
					All previously inserted  \link Atom Atom \endlink  objects (inserted with the method  \link insertAtom_ insertAtom_ \endlink )
					will be processed with the method  \link buildBondModels_ buildBondModels_ \endlink  to create the graphical 
					representation of the  \link Bond Bond \endlink  objects.
					@return bool <tt>true</tt> if the finish of {\em *this} addBallAndStickModel was successful, <tt>false</tt> otherwise
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
					is created for that atom, appended to {\em composite} and inserted with 
					the method  \link insertAtom_ insertAtom_ \endlink .
					All previously appended primitives to {\em composite}	are removed using the method
					 \link removeGeometricObjects_ removeGeometricObjects_ \endlink .
					The color for that  \link Sphere Sphere \endlink  object is calculated with the  \link ColorCalculator ColorCalculator \endlink 
					object retrieved with the method  \link getColorCalculator getColorCalculator \endlink .
					If {\em *this} addBallAndStickModel should create a ball and stick model
					the radius assigned to the sphere will be the ball radius (see method
					 \link setBallRadius setBallRadius \endlink ); if a stick model should be created than the stick radius
					is assigned to the sphere (see method  \link setStickRadius setStickRadius \endlink ).
					All atoms inserted with the method  \link insertAtom_ insertAtom_ \endlink  will later used for creating
					the model of the reachable  \link Bond Bond \endlink  objects. Those models will be created
					with the method  \link buildBondModels_ buildBondModels_ \endlink .
					@param  composite the  \link Composite Composite \endlink  object that will be processed
					@return Processor::Result the result of {\em *this} addLineModel
					@exeception OutOfMemory thrown if the memory allocation for a  \link Sphere Sphere \endlink  object failed
					@see    Sphere
					@see    ColorCalculator
					@see    getColorCalculator
					@see    setBallRadius
					@see    setStickRadius
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

			/**	@name	Predicates
			*/
			//@{

			/** Creation of ball and stick model enabled.
					Test if {\em *this} addBallAndStickModel should create a ball and
					stick model rather than a stick model.
					@return   bool <tt>true</tt> if a ball and stick model should be created, <tt>false</tt> if a stick model should be created
					@see      isStickModel
					@see      enableBallAndStickModel
					@see      enableStickModel
			*/
			bool isBallAndStickModel() const
				throw();

			/** Creation of stick model enabled.
					Test if {\em *this} addBallAndStickModel should create a
					stick model rather than a ball and stick model.
					@return   bool <tt>true</tt> if a stick model should be created, <tt>false</tt> if a ball and stick model should be created
					@see      isBallAndStickModel
					@see      enableBallAndStickModel
					@see      enableStickModel
			*/
			bool isStickModel() const
				throw();
			//@}


			/**	@name	debuggers and diagnostics
			*/
			//@{
			/** Internal value dump.
					Dump the current state of {\em *this} addBallAndStickModel to 
					the output ostream {\em s} with dumping depth {\em depth}.
					Calls  \link AtomBondModelBaseProcessor::dump AtomBondModelBaseProcessor::dump \endlink .
					@param   s output stream where to output the state of {\em *this} addBallAndStickModel
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
					This methods are overridden by the class  \link AddGLBallAndStickModel AddGLBallAndStickModel \endlink 
					to create primitives with OpenGL implementation to generate the graphical
					representation of the shapes they represent.
					@see  AddGLBallAndStickModel
			*/
			//@{
			/** Create a sphere.
					Create a  \link Sphere Sphere \endlink  object.
					This method is overridden by the class  \link AddGLBallAndStickModel AddGLBallAndStickModel \endlink  to
					create a primitive with openGL implementation.
					The method  \link operator() operator() \endlink  uses this method to create a sphere
					for  \link Atom Atom \endlink  objects.
					@see  Sphere
					@see  AddGLBallAndStickModel
					@see  Atom
			*/
			virtual Sphere* createSphere_();

			/** Create a one colored tube.
					Create a  \link Tube Tube \endlink  object.
					This method is overridden by the class  \link AddGLBallAndStickModel AddGLBallAndStickModel \endlink  to
					create a primitive with openGL implementation.
					The method  \link operator() operator() \endlink  uses this method to create one colored
					tubes for  \link Bond Bond \endlink  objects whose  \link Atom Atom \endlink  objects have the same
					element types and thus the same color.
					@see  Tube
					@see  AddGLBallAndStickModel
					@see  Atom
					@see  Bond
			*/
			virtual Tube* createTube_();

			/** Create a two colored tube.
					Create a  \link TwoColoredTube TwoColoredTube \endlink  object.
					This method is overridden by the class  \link AddGLBallAndStickModel AddGLBallAndStickModel \endlink  to
					create a primitive with openGL implementation.
					The method  \link operator() operator() \endlink  uses this method to create two colored
					tubes for  \link Bond Bond \endlink  objects whose  \link Atom Atom \endlink  objects have different
					element types and thus different colors.
					@see  TwoColoredTube
					@see  AddGLBallAndStickModel
					@see  Atom
					@see  Bond
			*/
			virtual TwoColoredTube* createTwoColoredTube_();
			//@}

			private:

			Real ball_radius_;
			Real stick_radius_;
		
			bool ball_and_stick_;
		};

#			ifndef BALL_NO_INLINE_FUNCTIONS
#				include <BALL/MOLVIEW/FUNCTOR/ballAndStickModel.iC>
#			endif

	} // namespace MOLVIEW

} // namespace BALL

#endif // BALL_MOLVIEW_FUNCTOR_BALLANDSTICKMODEL_H
