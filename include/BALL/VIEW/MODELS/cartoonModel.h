// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: cartoonModel.h,v 1.1 2003/08/26 12:11:29 oliver Exp $

#ifndef BALL_MOLVIEW_FUNCTOR_CARTOONMODEL_H
#define BALL_MOLVIEW_FUNCTOR_CARTOONMODEL_H

#ifndef BALL_MOLVIEW_FUNCTOR_BACKBONE_MODEL_H
#	include <BALL/MOLVIEW/FUNCTOR/backboneModel.h>
#endif

#ifndef BALL_VIEW_PRIMITIV_MESH_H
# include <BALL/VIEW/PRIMITIV/mesh.h>
#endif

namespace BALL
{
	class SecondaryStructure;
	class Chain;

	using VIEW::Mesh;
	
	namespace MOLVIEW
	{
		/** AddCartoonModel class.
				The class AddCartoonModel is a model processor that creates a Cartoon model
				through the <b>CA</b>-atoms of the processed Composite object.
				For information about the processor concept see Processor in the BALL
				documentation.
				\ingroup  MolviewFunctorsModels
		*/
		class AddCartoonModel: public AddBackboneModel
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
					(inserted in the method operator()) will be used to create a Cartoon.
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
					generation of the Cartoon model.
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

			void drawHelix_(SecondaryStructure& ss);

			void drawStrand_(SecondaryStructure& ss);

			//_ create a spline segment between two spline points a and b
			void createSplineSegment2_(const SplinePoint &a, const SplinePoint &b);

			void computeSpline_(Chain& chain);

			void insertTriangle_(Position v1, Position v2, Position v3, Mesh& mesh);

			vector<Vector3> spline_;
	};

	} // namespace MOLVIEW
} // namespace BALL

#endif // BALL_MOLVIEW_FUNCTOR_CARTOONMODEL_H
