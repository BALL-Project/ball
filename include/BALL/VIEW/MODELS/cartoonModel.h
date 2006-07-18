// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: cartoonModel.h,v 1.32.2.2 2006/07/18 22:59:39 amoll Exp $
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
			AddCartoonModel()
				throw();

			/// Copy constructor.
			AddCartoonModel(const AddCartoonModel& cartoon_model)
				throw();

			/// Destructor.
			virtual ~AddCartoonModel()
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
			void setStrandHeight(float heigth)
				throw() { strand_height_ = heigth;}

			///
			float getStrandHeight() const
				throw() { return strand_height_;}
			
			///
			void setStrandWidth(float w)
				throw() { strand_width_ = w;}

			///
			float getStrandWidth() const
				throw() { return strand_width_;}

			///
			void setDNABaseRadius(float r)
				throw() { DNA_base_radius_ = r;}

			///
			float getDNABaseRadius() const
				throw() { return DNA_base_radius_;}

			///
			void setDNAHelixRadius(float r)
				throw() { DNA_helix_radius_ = r;}

			///
			float getDNAHelixRadius() const
				throw() { return DNA_helix_radius_;}

			///
			void setDNALadderRadius(float r)
				throw() { DNA_ladder_radius_ = r;}

			///
			float getDNALadderRadius() const
				throw() { return DNA_ladder_radius_;}

			///
			void setDrawDNAAsLadderModel(bool state)
				throw() { draw_DNA_as_ladder_ = state;}

			///
			bool drawDNAAsLadderModel()
				throw() { return draw_DNA_as_ladder_;}

			///
			void enableRibbons(bool state)
				throw() { draw_ribbon_ = state;}

			///
			bool ribbonsEnabled() const
				throw() {return draw_ribbon_;}
			
			protected:

			void insertTriangle_(Position v1, Position v2, Position v3, Mesh& mesh);
			inline void drawStrand_(const Vector3& start,
															 Vector3& right,
															 Vector3& hn,
															 float arrow_width,
															 Position& last_vertices,
															 Mesh& mesh);

			void calculateComplementaryBases_(const Composite& composite) throw();

			bool assignNucleotideAtoms_(Residue& r, Size nr_atoms, String atom_names[10], Atom* atoms[10])
				throw();

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
