// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: cartoonModel.h,v 1.32.18.1 2007/03/25 21:26:05 oliver Exp $
//

#ifndef BALL_VIEW_MODELS_CARTOONMODEL_H
#define BALL_VIEW_MODELS_CARTOONMODEL_H

#ifndef BALL_VIEW_MODELS_BACKBONEMODEL_H
#	include <BALL/VIEW/MODELS/backboneModel.h>
#endif

namespace BALL
{
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

		class BALL_VIEW_EXPORT AddCartoonModel
			: public AddBackboneModel
		{
			public:

			enum Types
			{
				STRAND = NUCLEIC_ACID + 1,
				HELIX
			};

			BALL_CREATE(AddCartoonModel)

			/// Default Constructor.
			AddCartoonModel();

			/// Copy constructor.
			AddCartoonModel(const AddCartoonModel& cartoon_model);

			/// Destructor.
			virtual ~AddCartoonModel();

			///
			void setHelixRadius(float radius)
				{ helix_radius_ = radius;}

			///
			float getHelixRadius() const
				{ return helix_radius_;}

			///
			void setArrowWidth(float width)
				{arrow_width_ = width;}

			///
			float getArrowWidth() const
				{ return arrow_width_;}

			///
			void setStrandHeight(float heigth)
				{ strand_height_ = heigth;}

			///
			float getStrandHeight() const
				{ return strand_height_;}
			
			///
			void setStrandWidth(float w)
				{ strand_width_ = w;}

			///
			float getStrandWidth() const
				{ return strand_width_;}

			///
			void setDNABaseRadius(float r)
				{ DNA_base_radius_ = r;}

			///
			float getDNABaseRadius() const
				{ return DNA_base_radius_;}

			///
			void setDNAHelixRadius(float r)
				{ DNA_helix_radius_ = r;}

			///
			float getDNAHelixRadius() const
				{ return DNA_helix_radius_;}

			///
			void setDNALadderRadius(float r)
				{ DNA_ladder_radius_ = r;}

			///
			float getDNALadderRadius() const
				{ return DNA_ladder_radius_;}

			///
			void setDrawDNAAsLadderModel(bool state)
				{ draw_DNA_as_ladder_ = state;}

			///
			bool drawDNAAsLadderModel()
				{ return draw_DNA_as_ladder_;}

			///
			void enableRibbons(bool state)
				{ draw_ribbon_ = state;}

			///
			bool ribbonsEnabled() const
				{return draw_ribbon_;}
			
			protected:

			void insertTriangle_(Position v1, Position v2, Position v3, Mesh& mesh);
			inline void drawStrand_(const Vector3& start,
															 Vector3& right,
															 Vector3& hn,
															 float arrow_width,
															 Position& last_vertices,
															 Mesh& mesh);

			void calculateComplementaryBases_(const Composite& composite);

			bool assignNucleotideAtoms_(Residue& r, Size nr_atoms, String atom_names[10], Atom* atoms[10]);

			void drawRiboseAtoms_(const Atom* atom1, const Atom* atom2, const Vector3& v1, const Vector3& v2);

			void calculateModelParts(Protein& protein);
			void assignModelType(ModelPart& part);
			void createModel_(Position set_pos, Position part_pos);

			void createStrand_(Position set_pos, Position part_pos);
			void createHelix_(Position set_first, Position part_pos);
			void createTubeHelix_(Position set_first, Position part_pos);
			void createWatsonCrickModel_(Position set_pos, Position part_pos);
			void createSimpleNucleicAcid_(Position set_pos, Position part_pos);

			void renderNucleotideOutline_(const vector<Vector3>& positions, Vector3 uv, Mesh& mesh);
			Mesh* createDoubleRing_(const vector<Vector3>& positions);
			Mesh* create6Ring_(vector<Vector3> positions);
			virtual void refineGuidePoints_();

			float helix_radius_;
			float helix_height_;
			float helix_width_;
			float arrow_width_;
			float strand_width_;
			float strand_height_;
			float DNA_helix_radius_;
			float DNA_ladder_radius_;
			float DNA_base_radius_;
			float ribbon_width_;
			float ribbon_radius_;

			bool  draw_DNA_as_ladder_;
			bool  draw_ribbon_;

			HashMap<const Residue*, const Residue*> complementary_bases_;
	};


	} // namespace VIEW
} // namespace BALL

#endif // BALL_VIEW_MODELS_CARTOONMODEL_H
