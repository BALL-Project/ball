// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: standardColorProcessor.h,v 1.32.18.2 2007/03/28 13:51:48 amoll Exp $
//

#ifndef BALL_VIEW_MODELS_STANDARDCOLORPROCESSOR_H
#define BALL_VIEW_MODELS_STANDARDCOLORPROCESSOR_H

#ifndef BALL_VIEW_MODELS_COLORPROCESSOR_H
#	include <BALL/VIEW/MODELS/colorProcessor.h>
#endif

#ifndef BALL_VIEW_DATATYPTE_COLORMAP_H
# include <BALL/VIEW/DATATYPE/colorMap.h>
#endif

#ifndef BALL_KERNEL_RESIDUE_H
# include<BALL/KERNEL/residue.h>
#endif

#ifndef BALL_KERNEL_CHAIN_H
# include<BALL/KERNEL/chain.h>
#endif

#ifndef BALL_KERNEL_MOLECULE_H
# include<BALL/KERNEL/molecule.h>
#endif

#ifndef BALL_KERNEL_SECONDARYSTRUCTURE_H
# include<BALL/KERNEL/secondaryStructure.h>
#endif


namespace BALL
{
	namespace VIEW
	{

		/** ElementColorProcessor class.
				The class ElementColorProcessor is derived from the class ColorProcessor.
				It is used for calculating element colors of a given Atom object.
				In the constructor a ColorMap, that contains all colors
				corresponding to the element symbols of atoms, is initialised.
				The ColorProcessor can be set in the class DisplayProperties
				to color an entire molecular object in its element colors. 
				\ingroup  ViewModels
		*/
		class BALL_VIEW_EXPORT ElementColorProcessor
			: public ColorProcessor
		{
			public:

			BALL_CREATE(ElementColorProcessor)
			
			/**	@name	Constructors
			*/
			//@{

			/** Default Constructor.
					Initialize the internal ColorMap to contain all element colors with 
					element numbers as names.
			*/
			ElementColorProcessor();

			///
			void setTransparency(Size value);

			///
			virtual void getColor(const Composite& composite, ColorRGBA& color_to_be_set);

			///
			HashMap<Position, ColorRGBA>& getColorMap() { return color_map_;}
			
			///
			const HashMap<Position, ColorRGBA>& getColorMap() const { return color_map_;}

			//@}
			protected: 

			HashMap<Position, ColorRGBA> color_map_;
		};


		/** ColorProcessor for coloring by the numbers of the Residue 's.
				\ingroup  ViewModels
		*/
		class BALL_VIEW_EXPORT ResidueNumberColorProcessor
			: public ColorProcessor
		{
			public:

				BALL_CREATE(ResidueNumberColorProcessor)

				///
				ResidueNumberColorProcessor();

				///
				virtual void getColor(const Composite& composite, ColorRGBA& color_to_be_set);

				///
				void setFirstColor(const ColorRGBA& color) { first_color_ = color;}

				///
				void setMiddleColor(const ColorRGBA& color) { middle_color_ = color;}

				///
				void setLastColor(const ColorRGBA& color) { last_color_ = color;}

				///
				const ColorRGBA& getFirstColor() const { return first_color_;}

				///
				const ColorRGBA& getMiddleColor() const { return middle_color_;}

				///
				const ColorRGBA& getLastColor() const { return last_color_;}

				///
				virtual bool start();

			protected:
				bool canUseMeshShortcut_(const Composite& composite);

				GeometricObjectList list_;
				ColorRGBA first_color_, middle_color_, last_color_;
				ColorMap table_;
				Residue dummy_residue_;
				HashMap<const Residue*, Position> residue_map_;
		};

		///
		class BALL_VIEW_EXPORT PositionColorProcessor
			: public ColorProcessor
		{
			public:

			BALL_CREATE(PositionColorProcessor)

			///
			PositionColorProcessor();

			///
			virtual void getColor(const Composite& composite, ColorRGBA& color_to_be_set);

			///
			void setColors(const vector<ColorRGBA>& colors) { colors_ = colors;}

			///
			vector<ColorRGBA>& getColors() { return colors_;}

			///
			const vector<ColorRGBA>& getColors() const { return colors_;}

			///
			bool start();

			protected:

			virtual const Composite* getAncestor_(const Composite&) { return 0;}

			virtual bool isOK_(const Composite&) { return false;}

			vector<ColorRGBA> 										colors_;
			HashMap<const Composite*, Position> 	composite_to_position_;
		};


		///
		class BALL_VIEW_EXPORT ChainColorProcessor
			: public PositionColorProcessor
		{
			public:

			BALL_CREATE(ChainColorProcessor)

			///
			ChainColorProcessor();

			protected:

			virtual const Composite* getAncestor_(const Composite& composite) 
				{ return composite.getAncestor(dummy_chain_);}

                        virtual bool isOK_(const Composite& composite) { return RTTI::isKindOf<Chain>(&composite);}
			bool canUseMeshShortcut_(const Composite& composite);

 			Chain 	dummy_chain_;
		};


		///
		class BALL_VIEW_EXPORT MoleculeColorProcessor
			: public PositionColorProcessor
		{
			public:

			BALL_CREATE(MoleculeColorProcessor)

			///
			MoleculeColorProcessor();

			protected:

			virtual const Composite* getAncestor_(const Composite& composite) 
				{ return composite.getAncestor(dummy_molecule_);}

                        virtual bool isOK_(const Composite& composite) { return RTTI::isKindOf<Molecule>(&composite);}
			bool canUseMeshShortcut_(const Composite& composite);

 			Molecule 	dummy_molecule_;
		};


		/** ResidueNameColorProcessor is derived from the class ColorProcessor.
				It is used for calculating residue colors of a given Atom object 
				(A residue color of a given atom is the color of the residue the atom is contained in).
				In the constructor a ColorMap is initialized that contains all colors
				corresponding to the names of the residues.
				\ingroup  ViewModels
		*/
		class BALL_VIEW_EXPORT ResidueNameColorProcessor
			: public ColorProcessor
		{
			public:

			BALL_CREATE(ResidueNameColorProcessor)
			
			/** Default Constructor.
					Initialize the internal ColorMap to contain all residue colors with 
					residue names as names.
			*/
			ResidueNameColorProcessor();

			///
			void setTransparency(Size value);

			///
			virtual void getColor(const Composite& composite, ColorRGBA& color_to_be_set);
			
			///
			StringHashMap<ColorRGBA>& getColorMap() { return color_map_;}
			
			///
			const StringHashMap<ColorRGBA>& getColorMap() const { return color_map_;}

			protected:	
				StringHashMap<ColorRGBA> color_map_;
				Residue 								 dummy_residue;
				bool canUseMeshShortcut_(const Composite& composite);
		};

			
		/** AtomChargeColorProcessor class.
				The class AtomChargeColorProcessor is derived from the class ColorProcessor.
				There can be set three colors for the charge boundaries. A color for a negative charge 
				(charge = -1), a color for the neutral charge (charge = 0)
				and a color for the positive charge (charge = +1).
				If a charge lies between these boundaries the resulting
				color will be interpolated according to the charge. If a charge 
				is greater than +1 or lower than -1 it will be set to +1 or -1.
				\ingroup  ViewModels
		*/
		class BALL_VIEW_EXPORT AtomChargeColorProcessor
			: public InterpolateColorProcessor
		{
			public:

			BALL_CREATE(AtomChargeColorProcessor)
			
			/** Default Constructor.
					Initialize the colors to:
					  - positive color set to blue ("0000FFFF")
					  - neutral color set to white ("FFFFFFFF")
					  - negative color set to red ("FF0000FF")
			*/
      AtomChargeColorProcessor();

			/// Copy constructor.
 			AtomChargeColorProcessor(const AtomChargeColorProcessor& color_Processor);

			///
			virtual void getColor(const Composite& composite, ColorRGBA& color_to_be_set);
		};


		/** AtomDistanceColorProcessor class.
				AtomDistanceColorProcessor can be choosen in the class DisplayProperties
				to color the atoms in multiple AtomContainer according to their distance. 
				There are two colors for the distance of Atom objects. 
				A color for the null distance and a color for the max distance.
				If a distance of an Atom object to another atom object (stored in different
				atom containers) lies between the null and the max distance, the resulting 
				color will be interpolated according to the distance.
				\ingroup  ViewModels
		*/
		class BALL_VIEW_EXPORT AtomDistanceColorProcessor
			:  public ColorProcessor
		{
			public:

			BALL_CREATE(AtomDistanceColorProcessor)
			
			/** Default Constructor.
					Initialize to:
					  - distance set to 10
					  - null distance color set to red ("FF0000FF"). 
					  - max distance color set to blue ("00FF00FF"). 
					\see         ColorRGBA
			*/
      AtomDistanceColorProcessor();

			/// Construct with copying the colors and the distance.
 			AtomDistanceColorProcessor(const AtomDistanceColorProcessor& color_Processor);

			/** Change the max distance.
					Every new Atom object is inserted into the AtomDistanceColorProcessor
					with this max distance. If the method calculateDistances() is called
					the distances of the inserted atoms are calculated.
					\param distance the new max distance 
			*/
			void setDistance(float distance) { distance_ = distance;}

			/** Inspection of the max distance.
			*/
			float getDistance() const;

			/** Change the null distance color.
					If the calculated distance between two Atom objects becomes 0, this
					color will be used to color these two atoms. Otherwise the color will be
					interpolated between this color and the max distance color according to the
					calculated distance.
					\param color the new null distance color 
					\see   getNullDistanceColor
					\see   setMaxDistanceColor
					\see   ColorRGBA
			*/
			void setNullDistanceColor(const ColorRGBA& color);

			/** Non-mutable inspection of the null distance color.
			*/
			const ColorRGBA& getNullDistanceColor() const;

			/** Change the max distance color.
					If the calculated distance between two Atom objects becomes the max distance
					or greater this	color will be used to color these two atoms. Otherwise the color will be
					interpolated between this color and the null distance color according to the
					calculated distance.
					\param color the new max distance color 
					\see   getMaxDistanceColor
					\see   setNullDistanceColor
					\see   ColorRGBA
			*/
			void setMaxDistanceColor(const ColorRGBA& color);

			/** Non-mutable inspection of the max distance color.
					\see     setMaxDistanceColor
					\see     ColorRGBA
			*/
			const ColorRGBA& getMaxDistanceColor() const;

			/** Calculate the distances.
					If this method is called the distances of all previously inserted Atom objects 
					to each other are calculated. Only Atom objects are used for the distance
					calculation that have not the same root. All other atom objects will keep
					the max distance as set with the method setDistance().
					Use this method after all atom containers have applied this processor.
			*/
			void calculateDistances();

			/** Calculate the distance color of the given Atom by interpolating
					the null and max distance color according to the previously calculated 
					distance color (see calculateDistances()) of the given atom.
					Access to the calculated color with the method ColorProcessor::getColor.
					\param  atom the Atom whose distance color should be computed
			*/
			virtual void addAtom(const Atom& atom);

			///
			virtual bool finish();

			///
			virtual void getColor(const Composite& composite, ColorRGBA& color_to_be_set);

			///
			bool showSelected() { return show_selection_;}

			///
			void setShowSelected(bool state) { show_selection_ = state;}
			
			/** Collect all atoms from the geometric objects
			*/
			virtual Processor::Result operator() (GeometricObject*& object);

			private:

			void colorGeometricObject_(GeometricObject& object);

			//_ Colorize the mesh with the computed grid.
			virtual void colorMeshFromGrid_(Mesh& mesh);

			typedef HashMap<const Atom*, float> AtomDistanceHashMap;

			AtomDistanceHashMap atom_2_distance_;
			GeometricObjectList list_;

			float 			distance_;
			bool 				show_selection_;

			ColorRGBA		null_distance_color_;
			ColorRGBA		full_distance_color_;
		};


		/** CustomColorProcessor colorizes every GeometricObject with the default color.
				\ingroup  ViewModels
		*/
		class BALL_VIEW_EXPORT CustomColorProcessor
			: public ColorProcessor
		{
			public:

				BALL_CREATE(CustomColorProcessor)

				CustomColorProcessor()
					: ColorProcessor()
				{}

				bool canUseMeshShortcut_(const Composite&) { return true;}
		};


		/** Coloring by the temperature factor of atoms from a PDBFile
				\ingroup  ViewModels
		*/
		class BALL_VIEW_EXPORT TemperatureFactorColorProcessor
			: public InterpolateColorProcessor
		{
			public:

			BALL_CREATE(TemperatureFactorColorProcessor)

			///
			TemperatureFactorColorProcessor();

			///
			virtual void getColor(const Composite& composite, ColorRGBA& color_to_be_set);
		};

		
		/** Coloring by the occupancy of atoms from a PDBFile
				\ingroup  ViewModels
		*/
		class BALL_VIEW_EXPORT OccupancyColorProcessor
			: public InterpolateColorProcessor
		{
			public:

			BALL_CREATE(OccupancyColorProcessor)

			///
			OccupancyColorProcessor();

			///
			virtual void getColor(const Composite& composite, ColorRGBA& color_to_be_set);
		};

		/** Coloring by the forces, acting on atoms.
				\ingroup  ViewModels
		*/
		class BALL_VIEW_EXPORT ForceColorProcessor
			: public InterpolateColorProcessor
		{
			public:

			BALL_CREATE(ForceColorProcessor)

			///
			ForceColorProcessor();

			///
			virtual void getColor(const Composite& composite, ColorRGBA& color_to_be_set);
		};


		/** Coloring by the secondary structure, a Composite is in
				\ingroup  ViewModels
		*/
		class BALL_VIEW_EXPORT SecondaryStructureColorProcessor
			: public ColorProcessor
		{
			public:

			BALL_CREATE(SecondaryStructureColorProcessor)

			///
			SecondaryStructureColorProcessor();

			///
			virtual void getColor(const Composite& composite, ColorRGBA& color_to_be_set);

			///
			void setHelixColor(const ColorRGBA& color);

			///
			void setCoilColor(const ColorRGBA& color);

			///
			void setStrandColor(const ColorRGBA& color);

			///
			void setTurnColor(const ColorRGBA& color);

			///
			const ColorRGBA& getHelixColor() const;

			///
			const ColorRGBA& getCoilColor() const;

			///
			const ColorRGBA& getStrandColor() const;

			///
			const ColorRGBA& getTurnColor() const;

			///
			virtual void setTransparency(Size t);

			private:

			bool canUseMeshShortcut_(const Composite& composite);

			///
			ColorRGBA helix_color_,
								coil_color_,
								strand_color_,
								turn_color_;

			SecondaryStructure dummy_ss_;
		};
			
	
		/** Coloring by the type of a Residue
				\ingroup  ViewModels
		*/
		class BALL_VIEW_EXPORT ResidueTypeColorProcessor
			: public ColorProcessor
		{
			public:

			BALL_CREATE(ResidueTypeColorProcessor)

			///
			ResidueTypeColorProcessor();

			///
			virtual void getColor(const Composite& composite, ColorRGBA& color_to_be_set);

			///
			void setBasicColor(const ColorRGBA& color);

			///
			void setAcidicColor(const ColorRGBA& color);

			///
			void setPolarColor(const ColorRGBA& color);

			///
			void setHydrophobicColor(const ColorRGBA& color);

			///
			void setAromaticColor(const ColorRGBA& color);

			///
			void setOtherColor(const ColorRGBA& color);

			///
			const ColorRGBA& getBasicColor() const;

			///
			const ColorRGBA& getAcidicColor() const;

			///
			const ColorRGBA& getPolarColor() const;

			///
			const ColorRGBA& getHydrophobicColor() const;

			///
			const ColorRGBA& getAromaticColor() const;

			///
			const ColorRGBA& getOtherColor() const;

			///
			virtual void setTransparency(Size t);

			private:

			bool canUseMeshShortcut_(const Composite& composite);

			///
			ColorRGBA basic_color_,
								acidic_color_,
								polar_color_,
								hydrophobic_color_,
								aromatic_color_,
								other_color_;

			Residue   dummy_residue_;
		};
			
#	ifndef BALL_NO_INLINE_FUNCTIONS
#		include <BALL/VIEW/MODELS/standardColorProcessor.iC>
#	endif

} } // namespaces

#endif // BALL_VIEW_MODELS_STANDARDCOLORPROCESSOR_H
