// $Id: ballAndStickModel.h,v 1.9 2001/07/15 18:49:18 oliver Exp $

#ifndef BALL_MOLVIEW_FUNCTOR_BALLANDSTICKMODEL_H
#define BALL_MOLVIEW_FUNCTOR_BALLANDSTICKMODEL_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

#ifndef BALL_KERNEL_ATOM_H
#	include <BALL/KERNEL/atom.h>
#endif

#ifndef BALL_VIEW_KERNEL_GEOMETRICOBJECT_H
#	include <BALL/VIEW/KERNEL/geometricObject.h>
#endif

#ifndef BALL_VIEW_PRIMITIV_SPHERE_H
#	include <BALL/VIEW/PRIMITIV/sphere.h>
#endif

#ifndef BALL_VIEW_PRIMITIV_TUBE_H
#	include <BALL/VIEW/PRIMITIV/tube.h>
#endif

#ifndef BALL_MOLVIEW_COMMON_COMMON_H
#	include <BALL/MOLVIEW/COMMON/common.h>
#endif

#ifndef BALL_MOLVIEW_FUNCTOR_ATOMBONDMODELBASEPROCESSOR_H
#	include <BALL/MOLVIEW/FUNCTOR/atomBondModelBaseProcessor.h>
#endif

#ifndef BALL_MOLVIEW_PRIMITIV_TWOCOLOREDTUBE_H
#	include <BALL/MOLVIEW/PRIMITIV/twoColoredTube.h>
#endif


namespace BALL
{
	
	namespace MOLVIEW
	{

		using VIEW::Sphere;
		using VIEW::Tube;

		/** AddBallAndStickModel class.
				{\bf Framework:} BALL/MOLVIEW/FUNCTOR\\
				{\bf Definition:} \URL{BALL/MOLVIEW/FUNCTOR/ballAndStickModel.h}\\ \\
				The class AddBallAndStickModel is a model processor that is responsible
				for creating either a {\em ball and stick} model or only a {\em stick} model.
				In a ball and stick model the graphical representation of the \Ref{Atom} objects
				(\Ref{Sphere} objects) have a different radius than the representation of
				the \Ref{Bond} objects (\Ref{Tube} objects). Otherwise in the stick model
				the radi of the ball and the stick components are equal.
				This processor creates only the primitive objects without openGL implementation.
				The derived class \Ref{AddGLBallAndStickModel} uses the primitives with
				openGL implementation.
				For information about the processor concept see \Ref{Processor} in tbe BALL
				documentation.
				@memo    AddBallAndStickModel class (BALL MOLVIEW functor framework)
				@author  $Author: oliver $
				@version $Revision: 1.9 $
				@date    $Date: 2001/07/15 18:49:18 $
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
						\item  set the ball radius to {\tt 0.4}
						\item  set the stick radius to {\tt 0.2}
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
					@param       deep make a deep (={\tt true}) or shallow (={\tt false}) copy of {\em add_ball_and_stick_model}
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
					Calls \Ref{destroy}.
					@see         destroy
			*/
			virtual ~AddBallAndStickModel()
				throw();

			/** Explicit default initialization.
					Reset the state of {\em *this} addBallAndStickModel to:
					\begin{itemize}
					  \item  set the appearance to {\em ball and stick} model
						\item  set the ball radius to {\tt 0.4}
						\item  set the stick radius to {\tt 0.2}
					  \item  clear the \Ref{BaseModelProcessor}
					\end{itemize}
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


			/**	@name	Assignment
			*/
			//@{

			/** Assignment.
					Assign the addBallAndStickModel {\em add_ball_and_stick_model} to {\em *this} 
					addBallAndStickModel.
					Initialize the appearance of {\em *this} addBallAndStickModel to the appearance
					of {\em add_ball_and_stick_model}.
					Calls \Ref{AtomBondModelBaseProcessor::set}.
					@param       add_ball_and_stick_model the addBallAndStickModel to be copied 
					@param       deep make a deep (={\tt true}) or shallow (={\tt false}) copy of {\em add_ball_and_stick_model}
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
					Calls \Ref{set}.
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
					Calls \Ref{set}.
					@param       add_ball_and_stick_model the addBallAndStickModel to be assigned to
					@param       deep make a deep (={\tt true}) or shallow (={\tt false}) copy of {\em add_ball_and_stick_model}
					@see         set
			*/
			void get
				(AddBallAndStickModel& add_ball_and_stick_model, bool deep = true) const
				throw();

			/** Swapping of addBallAndStickModel's.
					Swap the \Ref{AtomBondModelBaseProcessor} and the appearance of {\em *this}
					addBallAndStickModel
					with that of {\em add_ball_and_stick_model}.
					Calls \Ref{AtomBondModelBaseProcessor::swap}
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
					For the ball-component (the graphical representation of the \Ref{Atom} object) and
					the stick-component (the graphical representation of the  \Ref{Bond} object)
					different radi can be assigned.
					@see   Atom
					@see   Bond
			*/
			//@{

			/** Change the radius of the ball-component.
					Change the radius of the ball-component (the graphical representation of 
					the \Ref{Atom} object) to the value of the parameter {\em radius}.
					@param       radius the new radius of the ball-component: ({\tt radius > 0})
					@exeception  OutOfRange thrown if {\tt radius <= 0}
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
					the \Ref{Bond} object) to the value of the parameter {\em radius}.
					@param       radius the new radius of the stick-component: ({\tt radius > 0})
					@exeception  OutOfRange thrown if {\tt radius <= 0}
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
					Initialize the properties of the \Ref{BaseModelConnector} to {\em *this}
					addBallAndStickModel.
					Calls \Ref{AtomBondModelBaseProcessor::start}.
					@return bool {\tt true} if the start of {\em *this} addBallAndStickModel was successful, {\tt false} otherwise
					@see    operator()
					@see    getModelConnector
					@see    AtomBondModelBaseProcessor
					@see    BaseModelConnector
			*/
			virtual bool start();
			
			/** Finish method.
					This method will be internally called from the processor mechanism if the processor
					has finished processing the \Ref{Composite} tree.
					All previously inserted \Ref{Atom} objects (inserted with the method \Ref{insertAtom_})
					will be processed with the method \Ref{buildBondModels_} to create the graphical 
					representation of the \Ref{Bond} objects.
					@return bool {\tt true} if the finish of {\em *this} addBallAndStickModel was successful, {\tt false} otherwise
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
					is created for that atom, appended to {\em composite} and inserted with 
					the method \Ref{insertAtom_}.
					All previously appended primitives to {\em composite}	are removed using the method
					\Ref{removeGeometricObjects_}.
					The color for that \Ref{Sphere} object is calculated with the \Ref{ColorCalculator}
					object retrieved with the method \Ref{getColorCalculator}.
					If {\em *this} addBallAndStickModel should create a ball and stick model
					the radius assigned to the sphere will be the ball radius (see method
					\Ref{setBallRadius}); if a stick model should be created than the stick radius
					is assigned to the sphere (see method \Ref{setStickRadius}).
					All atoms inserted with the method \Ref{insertAtom_} will later used for creating
					the model of the reachable \Ref{Bond} objects. Those models will be created
					with the method \Ref{buildBondModels_}.
					@param  composite the \Ref{Composite} object that will be processed
					@return Processor::Result the result of {\em *this} addLineModel
					@exeception OutOfMemory thrown if the memory allocation for a \Ref{Sphere} object failed
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
					@return   bool {\tt true} if a ball and stick model should be created, {\tt false} if a stick model should be created
					@see      isStickModel
					@see      enableBallAndStickModel
					@see      enableStickModel
			*/
			bool isBallAndStickModel() const
				throw();

			/** Creation of stick model enabled.
					Test if {\em *this} addBallAndStickModel should create a
					stick model rather than a ball and stick model.
					@return   bool {\tt true} if a stick model should be created, {\tt false} if a ball and stick model should be created
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
					Calls \Ref{AtomBondModelBaseProcessor::dump}.
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
					This methods are overridden by the class \Ref{AddGLBallAndStickModel}
					to create primitives with OpenGL implementation to generate the graphical
					representation of the shapes they represent.
					@see  AddGLBallAndStickModel
			*/
			//@{
			/** Create a sphere.
					Create a \Ref{Sphere} object.
					This method is overridden by the class \Ref{AddGLBallAndStickModel} to
					create a primitive with openGL implementation.
					The method \Ref{operator()} uses this method to create a sphere
					for \Ref{Atom} objects.
					@see  Sphere
					@see  AddGLBallAndStickModel
					@see  Atom
			*/
			virtual Sphere* createSphere_();

			/** Create a one colored tube.
					Create a \Ref{Tube} object.
					This method is overridden by the class \Ref{AddGLBallAndStickModel} to
					create a primitive with openGL implementation.
					The method \Ref{operator()} uses this method to create one colored
					tubes for \Ref{Bond} objects whose \Ref{Atom} objects have the same
					element types and thus the same color.
					@see  Tube
					@see  AddGLBallAndStickModel
					@see  Atom
					@see  Bond
			*/
			virtual Tube* createTube_();

			/** Create a two colored tube.
					Create a \Ref{TwoColoredTube} object.
					This method is overridden by the class \Ref{AddGLBallAndStickModel} to
					create a primitive with openGL implementation.
					The method \Ref{operator()} uses this method to create two colored
					tubes for \Ref{Bond} objects whose \Ref{Atom} objects have different
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
