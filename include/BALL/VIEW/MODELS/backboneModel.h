// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: backboneModel.h,v 1.1 2003/08/26 12:11:28 oliver Exp $
//

#ifndef BALL_MOLVIEW_FUNCTOR_BACKBONEMODEL_H
#define BALL_MOLVIEW_FUNCTOR_BACKBONEMODEL_H

#ifndef BALL_MOLVIEW_FUNCTOR_MOLECULARMODEL_H
#	include <BALL/MOLVIEW/FUNCTOR/molecularModel.h>
#endif

#ifndef BALL_VIEW_DATATYPE_COLORRGBA_H
#include <BALL/VIEW/DATATYPE/colorRGBA.h>
#endif

#ifndef BALL_MATHS_VECTOR3_H
# include <BALL/MATHS/vector3.h>
#endif 

namespace BALL
{
	class Atom;
	class Composite;

	using VIEW::ColorRGBA;
	
	namespace MOLVIEW
	{
		class Backbone;

		/** AddBackboneModel class.
				The class AddBackboneModel is a model processor that creates a backbone model
				through the <b>CA</b>-atoms of the processed Composite object.
				For information about the processor concept see Processor in the BALL
				documentation.
				\ingroup  MolviewFunctorsModels
		*/
		class AddBackboneModel: public MolecularModelProcessor
		{
			//_
			protected:

			class SplinePoint
			{
			  public:

				SplinePoint() {}
				SplinePoint(const Vector3& point, const ColorRGBA& color);
				~SplinePoint() {}

				void setVector(const Vector3& point) {point_ = point;}
				const Vector3 &getVector() const {return point_;}

				void setTangentialVector(const Vector3& tangent) {tangent_ = tangent;}
				const Vector3 &getTangentialVector() const {return tangent_;}
				
				void setColor(const ColorRGBA& color) {color_ = color;}
				const ColorRGBA &getColor() const {return color_;}

			  private:

				Vector3 point_;
				Vector3 tangent_;
				ColorRGBA color_;
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
					Calls MolecularModelProcessor::clear.
			*/
			virtual void clear()
				throw();

			//@} 
			/**	@name Processor specific methods 
			*/ 
			//@{
		
			/** Start method.
					Clear all internal structures.
					Calls MolecularModelProcessor::start.
					\return bool <tt>true<\tt> if the start was successful
					\see    operator()
			*/
			virtual bool start();
			
			/** Finish method.
					This method will be internally called from the processor mechanism when the processor
					has finished processing the Composite tree.
					All previously inserted Atom objects and their calculated colors
					(inserted in the method operator()) will be used to create a backbone.
					\return bool true if the finish was successful, false otherwise
					@exeception OutOfMemory thrown if the memory allocation failed
			*/
			virtual bool finish();
			
			/**	Operator method.
					This method iterates over each Composite object reachable in the 
					Composite tree. If a Composite is of kind Atom and has the
					substring <b>CA</b> in its name (this method collects only <b>CA</b>-atoms) than
					that atom	is stored for later processing in the finish method.
					The color for that Atom object is calculated with the ColorCalculator
					object retrieved with the method getColorCalculator() and stored for later
					generation of the backbone model.
					\param  composite the Composite object that will be processed
					\see    ColorCalculator
					\see    getColorCalculator()
			*/
			virtual Processor::Result operator() (Composite& composite);

			//@} 
			/**	@name	debuggers and diagnostics 
			*/ 
			//@{

			/** Internal value dump.
					Dump the current state to 
					the output ostream <tt>s</tt> with dumping depth <tt>depth</tt>.
					Calls MolecularModelProcessor::dump.
					\param   s output stream where to output the state 
					\param   depth the dumping depth
			*/
			virtual void dump(std::ostream& s = std::cout, Size depth = 0) const
				throw();

			//@}

			protected:
			//_ init the spline array with both the positions from the atom list
			//_ and the colors from the color list
			void initSplineArray_();

			//_ calculates to every splinepoint the tangential vector
			void calculateTangentialVectors_();
			
			//_ computes the actual spline path through the given support points
			//_ in the splinepoint array
			void createSplinePath_();
			//_ create a spline segment between two spline points a and b
			void createSplineSegment_(const SplinePoint &a, const SplinePoint &b);
			//_ builds a graphical representation to this point with color
			void buildGraphicalRepresentation_(const Vector3 &point, const ColorRGBA &color);

			//_
			void createBackbone_()
				throw();

			//_
			vector<SplinePoint> spline_vector_;

			//_
			bool have_start_point_;
			//_
			Vector3 last_point_;
			//_ Pointer to the parent of the last processed composite
			Composite* last_parent_;
		};

	} // namespace MOLVIEW

} // namespace BALL

#endif // BALL_MOLVIEW_FUNCTOR_BACKBONEMODEL_H
