// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: standardColorProcessor.h,v 1.26 2004/09/01 14:14:11 amoll Exp $
//

#ifndef BALL_VIEW_MODELS_STANDARDCOLORPROCESSOR_H
#define BALL_VIEW_MODELS_STANDARDCOLORPROCESSOR_H

#ifndef BALL_VIEW_MODELS_COLORPROCESSOR_H
#	include <BALL/VIEW/MODELS/colorProcessor.h>
#endif

#ifndef BALL_VIEW_DATATYPTE_COLORTABLE_H
# include <BALL/VIEW/DATATYPE/colorTable.h>
#endif

namespace BALL
{
	namespace VIEW
	{

		typedef List<GeometricObject*> GeometricObjectList;

		/** ElementColorProcessor class.
				The class ElementColorProcessor is derived from the class ColorProcessor.
				It is used for calculating element colors of a given Atom object.
				In the constructor a ColorMap, that contains all colors
				corresponding to the element symbols of atoms, is initialised.
				The ColorProcessor can be set in the class DisplayProperties
				to color an entire molecular object in its element colors. 
				\ingroup  ViewModels
		*/
		class BALL_EXPORT ElementColorProcessor
			: public ColorProcessor
		{
			public:
			
			/**	@name	Constructors
			*/
			//@{

			/** Default Constructor.
					Initialize the internal ColorMap to contain all element colors with 
					element numbers as names.
			*/
			ElementColorProcessor()
				throw();

			///
			virtual ColorRGBA getColor(const Composite* composite);

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
		class ResidueNumberColorProcessor
			: public ColorProcessor
		{
			public:

				///
				ResidueNumberColorProcessor()
					throw();

				///
				virtual ColorRGBA getColor(const Composite* composite);

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
				virtual bool start()
					throw();

			protected:
				GeometricObjectList list_;
				ColorRGBA first_color_, middle_color_, last_color_;
				ColorTable table_;
				Position min_;
				Position max_;
		};


		/** ResidueNameColorProcessor is derived from the class ColorProcessor.
				It is used for calculating residue colors of a given Atom object 
				(A residue color of a given atom is the color of the residue the atom is contained in).
				In the constructor a ColorMap is initialized that contains all colors
				corresponding to the names of the residues.
				\ingroup  ViewModels
		*/
		class ResidueNameColorProcessor
			: public ColorProcessor
		{
			public:
			
			/**	@name	Constructors
			*/
			//@{

			/** Default Constructor.
					Initialize the internal ColorMap to contain all residue colors with 
					residue names as names.
			*/
			ResidueNameColorProcessor()
				throw();

			//@} 
			/**	@name	Accessors: inspectors and mutators 
			*/ 
			//@{

			///
			virtual ColorRGBA getColor(const Composite* composite);
			//@}
			
			///
			StringHashMap<ColorRGBA>& getColorMap() { return color_map_;}
			
			///
			const StringHashMap<ColorRGBA>& getColorMap() const { return color_map_;}
			protected:	
				StringHashMap<ColorRGBA> color_map_;
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
		class AtomChargeColorProcessor
			: public ColorProcessor
		{
			public:
			
			/**	@name	Constructors 
			*/	
			//@{

			/** Default Constructor.
					Initialize the colors to:
					  - positive color set to blue ("0000FFFF")
					  - neutral color set to white ("FFFFFFFF")
					  - negative color set to red ("FF0000FF")
			*/
      AtomChargeColorProcessor()
				throw();

			/** Copy constructor.
			*/
 			AtomChargeColorProcessor(const AtomChargeColorProcessor& color_Processor)
				throw();

			//@} 
			/**	@name	Accessors: inspectors and mutators 
			*/ 
			//@{

			/** Change the positive color.
					If the charge of the given Atom object is greater +1, this color will be
					used. This positive color is used in the visit() method to calculate the
					actual charge color of the given Atom object.
					\param color the new positive color.
			*/
			void setPositiveColor(const ColorRGBA& color)
				throw();

			/** Non-mutable inspection of the positive color.
					\return  ColorRGBA& a constant reference to the positive color
			*/
			const ColorRGBA& getPositiveColor() const
				throw();

			/** Change the neutral color.
					If the charge of the given Atom object is = 0 than this color will be
					used. This neutral color is used in the visit method to calculate the
					actual charge color of the given Atom object.
			*/
			void setNeutralColor(const ColorRGBA& color)
				throw();

			/** Non-mutable inspection of the neutral color.
					\return  ColorRGBA& a constant reference to the neutral color
			*/
			const ColorRGBA& getNeutralColor() const
				throw();

			/** Change the negative color.
					If the charge of the given Atom object is = 0 than this color will be
					used. This negative color is used in the visit() method to calculate the
					actual charge color of the given Atom object.
			*/
			void setNegativeColor(const ColorRGBA& color)
				throw();

			/** Non-mutable inspection of the negative color.
					\return  ColorRGBA& a constant reference to the negative color
			*/
			const ColorRGBA& getNegativeColor() const
				throw();

			///
			virtual ColorRGBA getColor(const Composite* composite);

			//@}

			private:

			ColorRGBA		positive_color_, neutral_color_, negative_color_;
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
		class AtomDistanceColorProcessor
			:  public ColorProcessor
		{
			public:
			
			/**	@name	Constructors 
			*/	
			//@{

			/** Default Constructor.
					Initialize to:
					  - distance set to 10
					  - null distance color set to red ("FF0000FF"). 
					  - max distance color set to blue ("00FF00FF"). 
					\see         ColorRGBA
			*/
      AtomDistanceColorProcessor()
				throw();

			/** Construct with copying the colors and the distance.
			*/
 			AtomDistanceColorProcessor(const AtomDistanceColorProcessor& color_Processor)
				throw();

			//@} 
			/**	@name	Accessors: inspectors and mutators 
			*/ 
			//@{
			
			/** Change the max distance.
					Every new Atom object is inserted into the AtomDistanceColorProcessor
					with this max distance. If the method calculateDistances() is called
					the distances of the inserted atoms are calculated.
					\param distance the new max distance 
			*/
			void setDistance(float distance)
				throw();

			/** Inspection of the max distance.
			*/
			float getDistance() const
				throw();

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
			void setNullDistanceColor(const ColorRGBA& color)
				throw();

			/** Non-mutable inspection of the null distance color.
			*/
			const ColorRGBA& getNullDistanceColor() const
				throw();

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
			void setMaxDistanceColor(const ColorRGBA& color)
				throw();

			/** Non-mutable inspection of the max distance color.
					\see     setMaxDistanceColor
					\see     ColorRGBA
			*/
			const ColorRGBA& getMaxDistanceColor() const
				throw();

			/// Set wheter the selected molecular entities are also shown
			void setShowSelected(bool state) 
				throw() {show_selected_ = state;}

			///
			bool showSelected() const
				throw() { return show_selected_;}
			
			/** Calculate the distances.
					If this method is called the distances of all previously inserted Atom objects 
					to each other are calculated. Only Atom objects are used for the distance
					calculation that have not the same root. All other atom objects will keep
					the max distance as set with the method setDistance().
					Use this method after all atom containers have applied this processor.
			*/
			void calculateDistances()
				throw();

			/** Calculate the distance color of the given Atom by interpolating
					the null and max distance color according to the previously calculated 
					distance color (see calculateDistances()) of the given atom.
					Access to the calculated color with the method ColorProcessor::getColor.
					\param  atom the Atom whose distance color should be computed
			*/
			virtual void addAtom(const Atom& atom);

			///
			virtual bool finish()
				throw();
			//@} 
			/**	@name Processor specific methods 
			*/ 
			//@{

			///	Operator ().
			virtual ColorRGBA getColor(const Composite* composite);

			/** Collect all atoms from the geometric objects
			*/
			virtual Processor::Result operator() (GeometricObject*& object)
				throw();
			//@}

			private:

			typedef HashMap<const Atom*, float> AtomDistanceHashMap;

			AtomDistanceHashMap atom_2_distance_;
			GeometricObjectList list_;

			float 			distance_;

			bool 				show_selected_;

			ColorRGBA		null_distance_color_;
			ColorRGBA		full_distance_color_;
		};


		/** CustomColorProcessor colorizes every GeometricObject with the default color.
				\ingroup  ViewModels
		*/
		class CustomColorProcessor
			: public ColorProcessor
		{
			public:
				CustomColorProcessor()
					: ColorProcessor()
				{}
		};


		/** Coloring by the temperature factor of atoms from a PDBFile
				\ingroup  ViewModels
		*/
		class TemperatureFactorColorProcessor
			: public InterpolateColorProcessor
		{
			public:

			///
			TemperatureFactorColorProcessor();

			///	Operator ()
			virtual ColorRGBA getColor(const Composite* composite);
		};

		
		/** Coloring by the occupancy of atoms from a PDBFile
				\ingroup  ViewModels
		*/
		class OccupancyColorProcessor
			: public InterpolateColorProcessor
		{
			public:

			///
			OccupancyColorProcessor();

			///	Operator ()
			virtual ColorRGBA getColor(const Composite* composite);
		};

		/** Coloring by the forces, acting on atoms.
				\ingroup  ViewModels
		*/
		class ForceColorProcessor
			: public InterpolateColorProcessor
		{
			public:

			///
			ForceColorProcessor();

			///	Operator ()
			virtual ColorRGBA getColor(const Composite* composite);
		};


		/** Coloring by the secondary structure, a Composite is in
				\ingroup  ViewModels
		*/
		class SecondaryStructureColorProcessor
			: public ColorProcessor
		{
			public:

			///
			SecondaryStructureColorProcessor();

			/// Operator ()
			virtual ColorRGBA getColor(const Composite* composite);

			///
			void setHelixColor(const ColorRGBA& color)
				throw();

			///
			void setCoilColor(const ColorRGBA& color)
				throw();

			///
			void setStrandColor(const ColorRGBA& color)
				throw();

			///
			void setTurnColor(const ColorRGBA& color)
				throw();

			///
			const ColorRGBA& getHelixColor() const
				throw();

			///
			const ColorRGBA& getCoilColor() const
				throw();

			///
			const ColorRGBA& getStrandColor() const
				throw();

			///
			const ColorRGBA& getTurnColor() const
				throw();

			///
			virtual void setTransparency(Size t)
				throw();

			private:

			///
			ColorRGBA helix_color_,
								coil_color_,
								strand_color_,
								turn_color_;
		};
			
				
#	ifndef BALL_NO_INLINE_FUNCTIONS
#		include <BALL/VIEW/MODELS/standardColorProcessor.iC>
#	endif

} } // namespaces

#endif // BALL_VIEW_MODELS_STANDARDCOLORPROCESSOR_H
