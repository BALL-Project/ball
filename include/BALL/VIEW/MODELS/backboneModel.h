// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: backboneModel.h,v 1.13 2004/09/04 01:50:47 amoll Exp $
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

			  private:

				Vector3 point_;
				Vector3 tangent_;
				const Atom* atom_;
			};

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

			/** Finish method.
					This method will be internally called from the processor mechanism when the processor
					has finished processing the Composite tree.
					All previously inserted Atom objects 
					(inserted in the method operator()) will be used to create a backbone.
					\return bool true if the finish was successful, false otherwise
					@exception OutOfMemory thrown if the memory allocation failed
			*/
			virtual bool finish();
			
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
			void buildGraphicalRepresentation_(const Vector3 &point, const Atom* atom);

			//_
			void createBackbone_()
				throw();

			//_
			vector<SplinePoint> spline_vector_;
			std::vector<Vector3> spline_;

			//_
			bool have_start_point_;
			//_
			Vector3 last_point_;
			//_ Pointer to the parent of the last processed composite
			Composite* last_parent_;

			float tube_radius_;
		};

	} // namespace VIEW
} // namespace BALL

#endif // BALL_VIEW_MODELS_BACKBONEMODEL_H
