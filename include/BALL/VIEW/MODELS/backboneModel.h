// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: backboneModel.h,v 1.17.4.4 2005/09/01 22:17:56 amoll Exp $
//

#ifndef BALL_VIEW_MODELS_BACKBONEMODEL_H
#define BALL_VIEW_MODELS_BACKBONEMODEL_H

#ifndef BALL_VIEW_MODELS_MODELPROCESSOR_H
#	include <BALL/VIEW/MODELS/modelProcessor.h>
#endif

#ifndef BALL_MATHS_VECTOR3_H
# include <BALL/MATHS/vector3.h>
#endif 

namespace BALL
{
	class Atom;
	class Composite;
	class Residue;

	namespace VIEW
	{
		/** AddBackboneModel class.
				The class AddBackboneModel is a model processor that creates a backbone model
				through the <b>CA</b>-atoms of the processed Composite object.
				For information about the processor concept see Processor in the BALL
				documentation.
				\ingroup  ViewModels
		*/
		class BALL_VIEW_EXPORT AddBackboneModel: public ModelProcessor
		{
			//_
			protected:

			struct SplinePoint
			{
			  public:

				SplinePoint();
				SplinePoint(const Vector3& new_point, const Atom* new_atom);

				Vector3 point;
				Vector3 tangent;
				const Atom* atom;
			};

			public:

			BALL_CREATE(AddBackboneModel)

			/**	@name	Constructors and Destructors
			*/	
			//@{

			/** Default Constructor.
			*/
			AddBackboneModel()
				throw();

			/** Copy constructor.
			*/
			AddBackboneModel(const AddBackboneModel& add_Backbone_model)
				throw();

			/** Destructor.
			*/
			virtual ~AddBackboneModel()
				throw();

			/** Explicit default initialization.
					Calls ModelProcessor::clear.
			*/
			virtual void clear()
				throw();

			//@} 
			/**	@name Processor specific methods 
			*/ 
			//@{
		
			/**	Operator method.
					This method iterates over each Composite object reachable in the 
					Composite tree. If a Composite is of kind Atom and has the
					substring <b>CA</b> in its name (this method collects only <b>CA</b>-atoms) than
					that atom	is stored for later processing in the finish method.
					\param  composite the Composite object that will be processed
			*/
			virtual Processor::Result operator() (Composite& composite);

			//@} 
			/**	@name	debuggers and diagnostics 
			*/ 
			//@{

			/** Internal value dump.
					Dump the current state to the output ostream <tt>s</tt> with 
					dumping depth <tt>depth</tt>.
					Calls ModelProcessor::dump.
					\param   s output stream where to output the state 
					\param   depth the dumping depth
			*/
			virtual void dump(std::ostream& s = std::cout, Size depth = 0) const
				throw();

			///
			void setTubeRadius(float radius)
				throw() { tube_radius_ = radius;}

			///
			float getTubeRadius() const
				throw() { return tube_radius_;}

			///
			virtual bool createGeometricObjects()
				throw();

			//@}

			protected:

			//_ computes the actual spline path through the given support points
			//_ in the splinepoint array
			void createSplinePath_(Position to_stop);
			
			// calculate the interpolated points up to spline point pos
			virtual void createPart_(Position pos);
			virtual void drawPart_(Position pos);

			// build the graphical representaion between the given spline points
			virtual void buildGraphicalRepresentation_(Position start, Position end, Position type);

			//_ collect the atoms, for which the spline points will be calculated
			virtual void collectAtoms_(const Residue& residue);

			virtual Position getType_(const Residue& residue);

			virtual void clear_();

			// test if we have dont have a connection to last drawn residue
			bool checkBuildNow_(const Residue& residue);

			// build a tube model
			void buildTube_(Position start, Position end);

			vector<SplinePoint>  splines_;
			vector<Vector3>  		 points_;
			vector<const Atom*>  atoms_of_points_;

			//_ 
			const Residue* last_residue_;

			//_
			Position last_build_;
			
			//_
			float tube_radius_;

			//_
			Size 	interpolation_steps_;
		};

	} // namespace VIEW
} // namespace BALL

#endif // BALL_VIEW_MODELS_BACKBONEMODEL_H
