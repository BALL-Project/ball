// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: backboneModel.h,v 1.14.2.10 2005/02/01 14:27:37 amoll Exp $
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
		class BALL_EXPORT AddBackboneModel: public ModelProcessor
		{
			//_
			protected:

			class SplinePoint
			{
			  public:

				SplinePoint();
				SplinePoint(const Vector3& point, const Atom* atom);
				~SplinePoint() {}

				void setVector(const Vector3& point) {point_ = point;}
				const Vector3 &getVector() const {return point_;}

				void setTangentialVector(const Vector3& tangent) {tangent_ = tangent;}
				const Vector3 &getTangentialVector() const {return tangent_;}
				
				void setAtom(const Atom* atom) {atom_ = atom;}
				const Atom* getAtom() const { return atom_;}

				/** Needed for sort, sorting is needed for creation of backbone for
				 		a selection of Residues, because the Processor can get the Residues
						in wrong order from the Representation.
				*/
				bool operator < (const SplinePoint& point) const 
					throw();

			  private:

				Vector3 point_;
				Vector3 tangent_;
				const Atom* atom_;
			};

			friend class SplinePoint;

			public:

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
					Dump the current state to 
					the output ostream <tt>s</tt> with dumping depth <tt>depth</tt>.
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
			//_ init the spline array with both the positions from the atom list
			void initSplineArray_();

			//_ calculates to every splinepoint the tangential vector
			void calculateTangentialVectors_();
			
			//_ computes the actual spline path through the given support points
			//_ in the splinepoint array
			void createSplinePath_();
			//_ create a spline segment between two spline points a and b
			void createSplineSegment_(const SplinePoint &a, const SplinePoint &b);
			//_ builds a graphical representation to this point 
			void buildGraphicalRepresentation_(Size start = 0, Size end = 0)
				throw(Exception::OutOfMemory);

			//_ collect the atoms, for which the spline points will be calculated
			virtual void collectAtoms_(const Residue& residue)
				throw();

			//_
			void createBackbone_()
				throw();

			virtual void clear_()
				throw();

			bool checkBuildBackboneNow_(const Residue& residue);

			//_
			vector<SplinePoint> spline_vector_;
			//_
			vector<Vector3> 		spline_points_;
			//_
			vector<const Atom*> atoms_of_spline_points_;
			//_ Pointer to the parent of the last processed composite
			const Composite* last_parent_;
			//_
			float tube_radius_;

			//_
			Size 	interpolation_steps_;

			//_
			Index last_spline_point_;

			//_
			static HashMap<const Residue*, Position> residue_map_;
		};

	} // namespace VIEW
} // namespace BALL

#endif // BALL_VIEW_MODELS_BACKBONEMODEL_H
