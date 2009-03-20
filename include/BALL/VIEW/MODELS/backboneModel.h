// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: backboneModel.h,v 1.20.18.1 2007/03/25 21:26:05 oliver Exp $
//

#ifndef BALL_VIEW_MODELS_BACKBONEMODEL_H
#define BALL_VIEW_MODELS_BACKBONEMODEL_H

#ifndef BALL_VIEW_MODELS_MODELPROCESSOR_H
#	include <BALL/VIEW/MODELS/modelProcessor.h>
#endif

#ifndef BALL_MATHS_VECTOR3_H
# include <BALL/MATHS/vector3.h>
#endif 

#ifndef BALL_KERNEL_SECONDARYSTRUCTURE_H
 #include <BALL/KERNEL/secondaryStructure.h>
#endif

#ifndef BALL_KERNEL_PROTEIN_H
 #include <BALL/KERNEL/protein.h>
#endif

#ifndef BALL_MATHS_MATRIX44_H
 #include <BALL/MATHS/matrix44.h>
#endif

namespace BALL
{
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

			struct ModelPart
			{
				ModelPart();
				ModelPart(const ModelPart& m);
				const ModelPart& operator = (const ModelPart& p);

				// residues for this ModelPart
				vector<Residue*> residues;
				// Model Type, e.g. Strand, Helix, Ribbon
				Index type;
				// start and end index in guide_points_
				Position first_guide_point;
				Position last_guide_point;
				// start and end index in interpolated_points_
				Position first_point;
				Position last_point;
			};
				
			enum ModelTypes
			{
				TUBE,
				RIBBON,
				NUCLEIC_ACID
			};

			public:

			BALL_CREATE(AddBackboneModel)

			/// Default Constructor.
			AddBackboneModel();

			/// Copy constructor.
			AddBackboneModel(const AddBackboneModel& add_Backbone_model);

			/// Destructor.
			virtual ~AddBackboneModel();

			/** Explicit default initialization.
					Calls ModelProcessor::clear.
			*/
			virtual void clear();

			///
			virtual bool start();

			/**	Operator method.
					This method iterates over each Composite object reachable in the 
					Composite tree. If a Composite is of kind Atom and has the
					substring <b>CA</b> in its name (this method collects only <b>CA</b>-atoms) than
					that atom	is stored for later processing in the finish method.
					\param  composite the Composite object that will be processed
			*/
			virtual Processor::Result operator() (Composite& composite);

			/** Internal value dump.
					Dump the current state to the output ostream <tt>s</tt> with 
					dumping depth <tt>depth</tt>.
					Calls ModelProcessor::dump.
					\param   s output stream where to output the state 
					\param   depth the dumping depth
			*/
			virtual void dump(std::ostream& s = std::cout, Size depth = 0) const;

			///
			void setTubeRadius(float radius)
				{ tube_radius_ = radius;}

			///
			float getTubeRadius() const
				{ return tube_radius_;}

			///
			virtual bool createGeometricObjects();

			///
			void setRibbonMode(bool state)
				{ribbon_mode_ = state;}

			///
			bool ribbonModeEnabled() const
				{return ribbon_mode_;}

			protected:

			virtual void calculateModelParts(Protein& protein);
			virtual bool collectPositions(vector<Residue*> residues);
			virtual void calculateGuidePoints_();

			virtual void assignModelType(ModelPart& part);

			virtual void createModel_(Position set_pos, Position model_pos);
			// build a tube model
			virtual void createTube_(Position set_pos, Position model_pos);
			// build a ribbon / helix
			virtual void createRibbon_(Position set_pos, Position model_pos);


			virtual void refineGuidePoints_() {};
			virtual void interpolate_();
			virtual void refineModelParts_();

			virtual void clear_();
			
			inline void evaluateBSpline(const Vector3& v1, const Vector3& v2, 
																  const Vector3& v3, float x, Vector3& result);

			inline bool residuesAreConnected_(Residue& residue1, Residue& residue2);

			inline void calculateTubePoints_(Vector3 right, Vector3 dir, vector<Vector3>& points);
			inline void calculateRibbonPoints_(Vector3 xn, Vector3 dir, vector<Vector3>& points);
			inline void calculateRibbonEllipse_(float ribbon_width, float ribbon_height);

			//_
			float tube_radius_;

			//_
			Size 	interpolation_steps_;
			// must be alwas uneven:
			Size number_of_ribbons_;
			float ribbon_width_;
			float ribbon_height_;
			bool care_for_SS_;
			bool ribbon_mode_;

			vector<vector<vector<Vector3> > > guide_points_;
			vector<vector<vector<Vector3> > > interpolated_points_;
			vector<vector<Vector3> > backbone_;
			vector<vector<Vector3> > offsets_;
			vector<vector<ModelPart> > model_parts_;

			Protein dummy_protein_;
			Chain dummy_chain_;
			SecondaryStructure dummy_ss_;
			HashSet<Residue*> residues_to_be_rendered_;
			Protein* last_protein_;
			vector<vector<Index> > ss_;
			// temp variables for speedup:
			Size 				slides_;
			Position 		middle_slide_;
			Position 		middle_ribbon_;
			Angle 			slides_angle_;
			Matrix4x4 	temp_matrix_;
			vector<float> xs_, ys_;
		};

	} // namespace VIEW
} // namespace BALL

#endif // BALL_VIEW_MODELS_BACKBONEMODEL_H
