// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: cartoonModel.h,v 1.12 2004/02/24 17:16:20 amoll Exp $
//

#ifndef BALL_VIEW_MODELS_CARTOONMODEL_H
#define BALL_VIEW_MODELS_CARTOONMODEL_H

#ifndef BALL_VIEW_MODELS_BACKBONEMODEL_H
#	include <BALL/VIEW/MODELS/backboneModel.h>
#endif

namespace BALL
{
	class SecondaryStructure;
	class AtomContainer;

	namespace VIEW
	{
		class Mesh;

		/** AddCartoonModel class.
				The class AddCartoonModel is a model processor that creates a Cartoon model
				through the <b>CA</b>-atoms of the processed Composite object.
				For information about the processor concept see Processor in the BALL
				documentation.
				\ingroup  ViewModels
		*/
		class AddCartoonModel
			: public AddBackboneModel
		{
			public:

			/**	@name	Constructors and Destructors
			*/	
			//@{

			/** Default Constructor.
			*/
			AddCartoonModel()
				throw();

			/** Copy constructor.
			*/
			AddCartoonModel(const AddCartoonModel& cartoon_model)
				throw();

			/** Destructor.
			*/
			virtual ~AddCartoonModel()
				throw();

			//@} 
			/**	@name Processor specific methods 
			*/ 
			//@{
		
			/** Finish method.
					This method will be internally called from the processor mechanism when the processor
					has finished processing the Composite tree.
					All previously inserted Atom objects 
					(inserted in the method operator()) will be used to create a Cartoon.
					\return bool true if the finish was successful, false otherwise
					@exception OutOfMemory thrown if the memory allocation failed
			*/
			virtual bool finish();
			
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
			void setHelixRadius(float radius)
				throw() { helix_radius_ = radius;}

			///
			float getHelixRadius() const
				throw() { return helix_radius_;}

			///
			void setArrowWidth(float width)
				throw() {arrow_width_ = width;}

			///
			float getArrowWidth() const
				throw() { return arrow_width_;}

			///
			void setArrowHeight(float heigth)
				throw() { arrow_height_ = heigth;}

			///
			float getArrowHeight() const
				throw() { return arrow_height_;}

			//@}

			protected:

			void drawHelix_(SecondaryStructure& ss);

			void drawStrand_(SecondaryStructure& ss);

			//_ create a spline segment between two spline points a and b
			void createSplineSegment2_(const SplinePoint &a, const SplinePoint &b);

			void computeSpline_(AtomContainer& ac);

			void insertTriangle_(Position v1, Position v2, Position v3, Mesh& mesh);

			std::vector<Vector3> spline_;

			Composite* last_chain_;

			SplinePoint last_spline_point_;

			float helix_radius_;
			float arrow_width_;
			float arrow_height_;
	};

	} // namespace VIEW
} // namespace BALL

#endif // BALL_VIEW_MODELS_CARTOONMODEL_H
