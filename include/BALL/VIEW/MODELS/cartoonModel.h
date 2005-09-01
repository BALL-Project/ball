// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: cartoonModel.h,v 1.29.4.6 2005/09/01 22:17:56 amoll Exp $
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
	class Residue;
	class Chain;

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

			BALL_CREATE(AddCartoonModel)

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
			
			///
			void enableTwoColors(bool state)
				throw() { use_two_colors_ = state;}

			///
			bool twoColorsEnabled() const
				throw() {return use_two_colors_;}

			//@}

			protected:

			//_
			virtual void clear_()
				throw();

			//_ collect the atoms, for which the spline points will be calculated
			virtual void collectAtoms_(SecondaryStructure& ss);

			void buildGraphicalRepresentation_(Position start, Position end, Position type);

			void buildHelix_(Position start, Position end);

			void buildStrand_(Position start, Position end);

			void buildDNA_(Position start, Position end);

			void buildWatsonCrickModel_(Position start, Position end);

			void buildRibbon_(Position start, Position end);

			void insertTriangle_(Position v1, Position v2, Position v3, Mesh& mesh);
			void drawStrand_(const Vector3& start,
											 Vector3& normal,
											 Vector3& right,
											 float arrow_width,
											 Position& last_vertices,
											 Mesh& mesh);

			void calculateComplementaryBases_(const Composite& composite) throw();

			void createTriangle_(Mesh& mesh, const Atom& a1, const Atom& a2, const Atom& a3,
																			 const Atom* sa1, const Atom* sa2, const Atom* sa3)
 				throw();

			bool assignNucleotideAtoms_(Residue& r, Size nr_atoms, String atom_names[10], Atom* atoms[10])
				throw();

			void drawRiboseAtoms_(const Atom* atom1, const Atom* atom2, const Vector3& v1, const Vector3& v2);

			virtual void drawPart_(Position pos);
			Position getType_(const Residue& residue);

			Composite* last_chain_;

			float helix_radius_;
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
			bool  use_two_colors_;

			HashMap<const Residue*, const Residue*> complementary_bases_;
			HashMap<const SecondaryStructure*, Position> ss_to_spline_start_;
			HashMap<const SecondaryStructure*, Position> ss_nr_splines_;

			bool no_ss_;
	};

	} // namespace VIEW
} // namespace BALL

#endif // BALL_VIEW_MODELS_CARTOONMODEL_H
