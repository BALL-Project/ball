// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: cartoonModel.h,v 1.26.2.5 2005/01/04 22:41:59 amoll Exp $
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
		class BALL_EXPORT AddCartoonModel
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
		
			/** 
			*/
			virtual bool createGeometricObjects()
				throw();
			
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

			//@}

			protected:

			//_
			virtual void clear_()
				throw();

			void drawHelix_(SecondaryStructure& ss)
				throw();

			void drawStrand_(SecondaryStructure& ss)
				throw();

			void drawTube_(SecondaryStructure& ss)
				throw();

			void drawDNA_(SecondaryStructure& ss)
				throw();

			void drawWatsonCrickModel_(const SecondaryStructure& ss)
				throw();

			void drawRibbon_(Size start, Size end)
				throw();

			void computeSpline_(AtomContainer& ac);

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

			Size getStartPosition_(const SecondaryStructure& ss)
				throw();

			Composite* last_chain_;

			// used to speed up drawTube_
			Index spline_vector_position_;

			float helix_radius_;
			float arrow_width_;
			float arrow_height_;
			float DNA_helix_radius_;
			float DNA_ladder_radius_;
			float DNA_base_radius_;
			float ribbon_width_;
			float ribbon_radius_;

			bool  draw_DNA_as_ladder_;
			bool  draw_ribbon_;

			HashMap<Residue*, Residue*> complementary_bases_;
	};

	} // namespace VIEW
} // namespace BALL

#endif // BALL_VIEW_MODELS_CARTOONMODEL_H
