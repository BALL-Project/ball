// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: ballAndStickModel.h,v 1.2 2003/08/26 18:35:33 amoll Exp $
//

#ifndef BALL_VIEW_MODELS_BALLANDSTICKMODEL_H
#define BALL_VIEW_MODELS_BALLANDSTICKMODEL_H

#ifndef BALL_VIEW_PRIMITIVES_SPHERE_H
#	include <BALL/VIEW/PRIMITIVES/sphere.h>
#endif

#ifndef BALL_VIEW_PRIMITIVES_TUBE_H
#	include <BALL/VIEW/PRIMITIVES/tube.h>
#endif

#ifndef BALL_VIEW_MODELS_ATOMBONDMODELBASEPROCESSOR_H
#	include <BALL/VIEW/MODELS/atomBondModelBaseProcessor.h>
#endif

#ifndef BALL_VIEW_PRIMITIVES_TWOCOLOREDTUBE_H
#	include <BALL/VIEW/PRIMITIVES/twoColoredTube.h>
#endif

namespace BALL
{
	class Atom;

	namespace VIEW
	{
		using VIEW::Sphere;
		using VIEW::Tube;
		using VIEW::TwoColoredTube;

		/** AddBallAndStickModel class.
				The class AddBallAndStickModel is a model processor that is responsible
				for creating either a <b>ball and stick</b> model or a <b>stick</b> model.
				In a ball and stick model the graphical representation of the Atom objects
				(Sphere objects) have a different radius than the representation of
				the Bond objects (Tube objects). Otherwise in the stick model
				the radi of the ball and the stick components are equal.
				For information about the processor concept see Processor in the BALL
				documentation. 
				\ingroup  MolviewModels
		*/
		class AddBallAndStickModel: public AtomBondModelBaseProcessor
		{
			public:

			/**	@name	Constructors and Destructors
			*/	
			//@{

			/** Default Constructor.
					Set the appearance to:
					 - set the appearance to ball and stick model
					 - set the ball radius to 0.4
					 - set the stick radius to 0.2
			*/
			AddBallAndStickModel()
				throw();

			/** Copy constructor.
			*/
			AddBallAndStickModel(const AddBallAndStickModel& add_ball_and_stick_model)
				throw();

			/** Destructor.
			*/
			virtual ~AddBallAndStickModel()
				throw();

			/** Explicit default initialization.
					Reset the state of this AddBallAndStickModel to:
					 - set the appearance to ball and stick model
					 - set the ball radius to 0.4
					 - set the stick radius to 0.2
					Calls AtomBondModelBaseProcessor::clear.
			*/
			virtual void clear()
				throw();

			//@} 
			/**	@name	Assignment 
			*/ 
			//@{

			/** Assignment.
					Calls AtomBondModelBaseProcessor::set.
					\param       add_ball_and_stick_model the AddBallAndStickModel to be copied 
			*/
			void set(const AddBallAndStickModel& add_ball_and_stick_model)
				throw();

			/** Assignment operator.
					Calls set().
					\param       add_ball_and_stick_model the addBallAndStickModel to be copied
			*/
			const AddBallAndStickModel& operator = (const AddBallAndStickModel& add_ball_and_stick_model)
				throw();

			/** Swapping of addBallAndStickModel's.
					Calls AtomBondModelBaseProcessor::swap
			*/
			void swap(AddBallAndStickModel& add_ball_and_stick_model)
				throw();

			//@}
			/**	@name	Accessors: inspectors and mutators 
					This methods are used to specify the appearance of the model. This processor
					can either be used for creating a <b>ball and stick</b> model or a <b>stick</b> model.
					For the ball-component (the graphical representation of the Atom object) and
					the stick-component (the graphical representation of the Bond object)
					different radi can be assigned.
			*/
			//@{

			/** Change the radius of the ball-component.
					\param       radius the new radius of the ball-component: (<b>radius > 0</b>)
					\exception  OutOfRange thrown if <b>radius <= 0</b>
			*/
			void setBallRadius(const Real radius)
				throw(Exception::OutOfRange);

			/** Inspection of the radius of the ball-component.
			*/
			Real getBallRadius() const
				throw();

			/** Copy the radius of the ball-component to real.
					\param  radius the real receiving the radius of the ball-component
			*/
			void getBallRadius(Real& radius)
				throw();

			/** Change the radius of the stick-component.
					\param       radius the new radius of the stick-component: (radius > 0)
					\exception  OutOfRange thrown if radius <= 0
			*/
			void setStickRadius(const Real radius)
				throw(Exception::OutOfRange);

			/** Inspection of the radius of the stick-component.
			*/
			Real getStickRadius() const
				throw();

			/** Enable the creation of the ball and stick model.
			*/
			void enableBallAndStickModel()
				throw();

			/** Enable the creation of the stick model.
			*/
			void enableStickModel()
				throw();

			//@} 
			/**	@name Processor specific methods 
			*/ 
			//@{

			/** Start method.
					Calls AtomBondModelBaseProcessor::start.
					\return bool true if the start was successful, false otherwise
			*/
			virtual bool start();
			
			/** Finish method.
					This method will be internally called from the processor mechanism if the processor
					has finished processing the Composite tree.
					All previously inserted Atom objects (inserted with the method insertAtom_())
					will be processed with the method buildBondModels_() to create the graphical 
					representation of the Bond objects.
					\return bool true if the finish was successful
			*/
			virtual bool finish();
			
			/**	Operator method.
					This method iterates over each Composite object reachable in the 
					Composite tree. If the composite is of kind Atom than a Sphere
					is created for that atom, and the atom is inserted with 
					the method insertAtom_().
					The color for the Sphere object is calculated with the ColorCalculator
					object retrieved with the method getColorCalculator().
					If the AddBallAndStickModel should create a ball and stick model
					the radius assigned to the sphere will be the ball radius (see method
					setBallRadius()); if a stick model should be created than the stick radius
					is assigned to the sphere (see method setStickRadius()).
					All atoms inserted with the method insertAtom_() will later used for creating
					the model of the reachable Bond objects. Those models will be created
					with the method buildBondModels_().
					\param  composite the Composite object that will be processed
					\return Processor::Result the result 
					\exception OutOfMemory thrown if the memory allocation for a Sphere object failed
			*/
			virtual Processor::Result operator() (Composite& composite);

			//@} 
			/**	@name	Predicates 
			*/ 
			//@{

			/** Test if create ball and stick model.
					\return   bool true if a ball and stick model should be created, false if stick model 
			*/
			bool isBallAndStickModel() const
				throw();

			/** Test if create a stick model.
					\return   bool true if a stick model should be created, false if ball and stick model 
			*/
			bool isStickModel() const
				throw();

			//@} 
			/**	@name	debuggers and diagnostics 
			*/ 
			//@{

			/** Internal value dump.
					Dump the current state of this AddBallAndStickModel to 
					the output ostream <b>s</b> with dumping depth <b>depth</b>.
					Calls AtomBondModelBaseProcessor::dump.
					\param   s output stream where to output the state 
					\param   depth the dumping depth
			*/
			virtual void dump(std::ostream& s = std::cout, Size depth = 0) const
				throw();
			//@}

			private:

			Real ball_radius_;
			Real stick_radius_;
		
			bool ball_and_stick_;
		};

#	ifndef BALL_NO_INLINE_FUNCTIONS
#		include <BALL/VIEW/MODELS/ballAndStickModel.iC>
#	endif

	} // namespace VIEW
} // namespace BALL

#endif // BALL_VIEW_MODELS_BALLANDSTICKMODEL_H
