// $Id: standardColorCalculator.h,v 1.11 2001/07/16 14:49:23 amoll Exp $

#ifndef BALL_MOLVIEW_FUNCTOR_STANDARDCOLORCALCULATOR_H
#define BALL_MOLVIEW_FUNCTOR_STANDARDCOLORCALCULATOR_H

#ifndef BALL_MOLVIEW_FUNCTOR_COLORCALCULATOR_H
#	include <BALL/MOLVIEW/FUNCTOR/colorCalculator.h>
#endif

#ifndef BALL_DATATYPE_HASHMAP_H
#	include <BALL/DATATYPE/hashMap.h>
#endif

#ifndef BALL_KERNEL_RESIDUE_H
#	include <BALL/KERNEL/residue.h>
#endif

  
namespace BALL
{

	namespace MOLVIEW
	{
			
		/** ElementColorCalculator class.
				The class ElementColorCalculator is derived from the class \Ref{ColorCalculator}.
				It is used for calculating element colors of a given \Ref{Atom} object.
				In the constructor a \Ref{ColorMap} is initialized that contains all colors
				corresponding to the element symbols of atoms.
				This colorCalculator can be set in the class \Ref{DisplayProperties}
				to color an entire object in its element colors. \\
				{\bf Definition:} \URL{BALL/MOLVIEW/FUNCTOR/standardColorCalculator.h}
		*/
		class ElementColorCalculator
			: public ColorCalculator
		{
			public:
			
			/**	@name	Constructors
			*/
			//@{

			/** Default Constructor.
					Construct new elementColorCalculator.
					Initialize the internal \Ref{ColorMap} to contain all element colors with 
					element symbols as names.
					@return      ElementColorCalculator new constructed elementColorCalculator
					@see         ColorCalculator
			*/
			ElementColorCalculator()
				throw();
			//@}
		};

		/** ResidueNameColorCalculator class.
				The class ResidueNameColorCalculator is derived from the class \Ref{ColorCalculator}.
				It is used for calculating residue colors of a given \Ref{Atom} object 
				(A residue color of a given atom is the color of the residue the atom is contained in).
				In the constructor a \Ref{ColorMap} is initialized that contains all colors
				corresponding to the residue names of the residues.
				This colorCalculator can be set in the class \Ref{DisplayProperties}
				to color an entire object in its residue colors. \\
				{\bf Definition:} \URL{BALL/MOLVIEW/FUNCTOR/standardColorCalculator.h}
		*/
		class ResidueNameColorCalculator
			: public ColorCalculator
		{
			public:
			
			/**	@name	Constructors
			*/
			//@{

			/** Default Constructor.
					Construct new residueNameColorCalculator.
					Initialize the internal \Ref{ColorMap} to contain all residue colors with 
					residue names as names.
					@return      ResidueNameColorCalculator new constructed residueNameColorCalculator
					@see         ColorCalculator
			*/
			ResidueNameColorCalculator()
				throw();

			//@}

			/**	@name	Accessors: inspectors and mutators 
			*/
			//@{

			/** Calculate a key to an atom.
					Calculate a key (the residue name) to the given \Ref{Atom} {\em atom}.
					This method is overridden to calculate a string key of the \Ref{Atom} object.
					In this implementation the key of {\em atom} is the name of the \Ref{Residue} the
					\Ref{Atom} object is contained in.
					This calculated key will be used to access the \Ref{ColorMap} to get a
					\Ref{ColorRGBA} for the given \Ref{Atom} object.
					@param  atom the \Ref{Atom} object for which a string key should be calculated
					@return String& a constant reference to a string key
					see     ColorCalculator
			*/
			virtual const String& calculateKey(const Atom& atom) const
				throw();
			//@}
			
		};

			
		/** AtomChargeColorCalculator class.
				The class AtomChargeColorCalculator is derived from the class \Ref{ColorCalculator}.
				It is used for calculating charge colors of a given \Ref{Atom} object 
				There can be set three colors for the charge boundaries. A color for a negative charge 
				({\tt charge = -1}), a color for the neutral charge ({\tt charge = 0})
				and a color for the positive charge ({\tt charge = +1}).
				If a charge of an \Ref{Atom} object lies between these boundaries the resulting
				color will be interpolated according to the charge. If a charge of an \Ref{Atom} object
				is greater than {\tt +1} or lower than {\tt -1} it will be set to {\tt +1} or
				{\tt -1}.
				This colorCalculator can be set in the class \Ref{DisplayProperties}
				to color an entire object in its atom charge colors. \\
				{\bf Definition:} \URL{BALL/MOLVIEW/FUNCTOR/standardColorCalculator.h}
		*/
		class AtomChargeColorCalculator
			: public ColorCalculator
		{
			public:
			
			/**	@name	Constructors
			*/	
			//@{

			/** Default Constructor.
					Construct new atomChargeColorCalculator.
					Initialize the colors to:
					\begin{itemize}
					  \item  positive color set to blue {\tt "0000FFFF"}. See \Ref{ColorRGBA} for information on how to set colors
					  \item  neutral color set to white {\tt "FFFFFFFF"}. See \Ref{ColorRGBA} for information on how to set colors
					  \item  negative color set to red {\tt "FF0000FF"}. See \Ref{ColorRGBA} for information on how to set colors
					\end{itemize}
					@return      AtomChargeColorCalculator new constructed atomChargeColorCalculator
					@see         ColorCalculator
					@see         ColorRGBA
			*/
      AtomChargeColorCalculator()
				throw();

			/** Copy constructor.
					Construct new atomChargeColorCalculator by copying the colors of the 
					atomChargeColorCalculator {\em color_calculator}.
					@param       color_calculator the atomChargeColorCalculator to be copied
					@return      AtomChargeColorCalculator new constructed atomChargeColorCalculator copied from {\em colorCalculator}
					@see         ColorCalculator
			*/
 			AtomChargeColorCalculator(const AtomChargeColorCalculator& color_calculator)
				throw();

			//@}

			/** @name Destructors 
			*/
			//@{

			/** Destructor.
					Default destruction of {\em *this} atomChargeColorCalculator.
					Calls \Ref{destroy}.
					@see         destroy
			*/
			virtual ~AtomChargeColorCalculator()
				throw();

			/** Explicit default initialization.
					Reset the colors of {\em *this} atomChargeColorCalculator to:
					\begin{itemize}
					  \item  positive color set to blue {\tt "0000FFFF"}. See \Ref{ColorRGBA} for information on how to set colors
					  \item  neutral color set to white {\tt "FFFFFFFF"}. See \Ref{ColorRGBA} for information on how to set colors
					  \item  negative color set to red {\tt "FF0000FF"}. See \Ref{ColorRGBA} for information on how to set colors
					\end{itemize}
					@see  ColorRGBA
			*/
			virtual void clear()
				throw();

			/** Explicit destructor.
					Empty for further purpose.
			*/
			virtual void destroy()
				throw();
			//@}

			/**	@name	Assignment
			*/
			//@{

			/** Assignment.
					Assign the atomChargeColorCalculator {\em color_calculator} to {\em *this}
					atomChargeColorCalculator.
					The colors of {\em *this} atomChargeColorCalculator are initialized to the colors
					of the atomChargeColorCalculator {\em color_calculator}.
					@param       color_calculator the atomChargeColorCalculator to be copied 
			*/
			void set
				(const AtomChargeColorCalculator& color_calculator)
				throw();

			/** Assignment operator.
					Assign the atomChargeColorCalculator {\em color_calculator} to {\em *this}
					atomChargeColorCalculator.
					Calls \Ref{set}.
					The colors of {\em *this} atomChargeColorCalculator are initialized to the colors
					of the atomChargeColorCalculator {\em color_calculator}. 
					@param       color_calculator the atomChargeColorCalculator to be copied
					@return      AtomDistanceColorCalculator& constant reference of {\em *this} atomChargeColorCalculator
					@see         set
			*/
			const AtomChargeColorCalculator& operator =
				(const AtomChargeColorCalculator& color_calculator)
				throw();

			/** Copying.
					Copy {\em *this} atomChargeColorCalculator to the atomChargeColorCalculator 
					{\em color_calculator}.
					Calls \Ref{set}.
					The colors of {\em *this} atomChargeColorCalculator are initialized to the colors
					of the atomChargeColorCalculator {\em color_calculator}. 
					@param       color_calculator the atomChargeColorCalculator to be assigned to
					@see         set
			*/
			void get
				(AtomChargeColorCalculator& color_calculator) const
				throw();

			/** Swapping of atomChargeColorCalculator's.
					Swap the colors of {\em *this} atomChargeColorCalculator with the
					atomChargeColorCalculator	{\em color_calculator}.
					Calls \Ref{ColorRGBA::swap}.
					@param       color_calculator the atomChargeColorCalculator being swapped with {\em *this} atomChargeColorCalculator 
			*/
			void swap
				(AtomChargeColorCalculator& color_calculator)
				throw();
			//@}

			/**	@name	Accessors: inspectors and mutators 
			*/
			//@{
			/** Change the positive color.
					Change the positive color of {\em *this} atomChargeColorCalculator.
					If the charge of the given \Ref{Atom} object is {\tt >= +1} than this color will be
					used. This positive color is used in the \Ref{visit} method to calculate the
					actual charge color of the given \Ref{Atom} object.
					@param color the new positive color.
					@see   getPositiveColor
					@see   visit
					@see   ColorRGBA
			*/
			void setPositiveColor(const ColorRGBA& color)
				throw();

			/** Non-mutable inspection of the positive color.
					Access the constant reference of the positive color of {\em *this}
					atomChargeColorCalculator.
					@return  ColorRGBA& a constant reference to the positive \Ref{ColorRGBA} of {\em *this} atomChargeColorCalculator
					@see     setPositiveColor
					@see     ColorRGBA
			*/
			const ColorRGBA& getPositiveColor() const
				throw();

			/** Change the neutral color.
					Change the neutral color of {\em *this} atomChargeColorCalculator.
					If the charge of the given \Ref{Atom} object is {\tt = 0} than this color will be
					used. This neutral color is used in the \Ref{visit} method to calculate the
					actual charge color of the given \Ref{Atom} object.
					@param color the new neutral color.
					@see   getNeutralColor
					@see   visit
					@see   ColorRGBA
			*/
			void setNeutralColor(const ColorRGBA& color)
				throw();

			/** Non-mutable inspection of the neutral color.
					Access the constant reference of the neutral color of {\em *this}
					atomChargeColorCalculator.
					@return  ColorRGBA& a constant reference to the neutral \Ref{ColorRGBA} of {\em *this} atomChargeColorCalculator
					@see     setNeutralColor
					@see     ColorRGBA
			*/
			const ColorRGBA& getNeutralColor() const
				throw();

			/** Change the negative color.
					Change the negative color of {\em *this} atomChargeColorCalculator.
					If the charge of the given \Ref{Atom} object is {\tt = 0} than this color will be
					used. This negative color is used in the \Ref{visit} method to calculate the
					actual charge color of the given \Ref{Atom} object.
					@param color the new negative color.
					@see   getNegativeColor
					@see   visit
					@see   ColorRGBA
			*/
			void setNegativeColor(const ColorRGBA& color)
				throw();

			/** Non-mutable inspection of the negative color.
					Access the constant reference of the negative color of {\em *this}
					atomChargeColorCalculator.
					@return  ColorRGBA& a constant reference to the negative \Ref{ColorRGBA} of {\em *this} atomChargeColorCalculator
					@see     setNegativeColor
					@see     ColorRGBA
			*/
			const ColorRGBA& getNegativeColor() const
				throw();

			/** Visit method.
					Calculate the charge color of the given \Ref{Atom} {\em atom} by interpolating
					the positive, neutral and negative color according to the charge of {\em atom}.
					Access to the calculated color with the method \Ref{getColor} from the
					\Ref{ColorCalculator} class.
					@param  atom the \Ref{Atom} object whose charge color should be calculated
					@see    ColorCalculator
					@see    Atom
					@see    setPositiveColor
					@see    setNeutralColor
					@see    setNegativeColor
			*/
			virtual void visit(Atom& atom);
			//@}

			private:

			ColorRGBA		positive_color_;
			ColorRGBA		neutral_color_;
			ColorRGBA		negative_color_;
		};



		/** AtomDistanceColorCalculator class.
				The class AtomDistanceColorCalculator is derived from the class \Ref{ColorCalculator}
				and from \Ref{UnaryProcessor} for \Ref{Atom} and \Ref{Composite}.
				It is used for calculating distance colors of \Ref{Atom} objects that are stored in
				multiple atom containers.
				There can be two colors for the distance of atom objects. A color for the null distance 
				and a color for the max distance.
				If a distance of an \Ref{Atom} object to another atom object (stored in different
				atom containers) lies between the null and the max distance the resulting 
				color will be interpolated according to the distance.
				With this class it is possible to color two or more atom container (e.g. Molecules)
				according to their atom distances.
				This colorCalculator can be set in the class \Ref{DisplayProperties}
				to color the atoms in multiple atom container according to their distance. \\
				{\bf Definition:} \URL{BALL/MOLVIEW/FUNCTOR/standardColorCalculator.h}
		*/
		class AtomDistanceColorCalculator
			:	public UnaryProcessor<Atom>,
			  public UnaryProcessor<Composite>,
			  public ColorCalculator
		{
			public:
			
			/**	@name	Constructors
			*/	
			//@{

			/** Default Constructor.
					Construct new atomDistanceColorCalculator.
					Initialize the colors to:
					\begin{itemize}
					  \item  internal structures cleared
					  \item  distance set to {\tt 10}
					  \item  null distance color set to red {\tt "FF0000FF"}. See \Ref{ColorRGBA} for information on how to set colors
					  \item  max distance color set to blue {\tt "00FF00FF"}. See \Ref{ColorRGBA} for information on how to set colors
					\end{itemize}
					@return      AtomDistanceColorCalculator new constructed atomDistanceColorCalculator
					@see         ColorCalculator
					@see         ColorRGBA
			*/
      AtomDistanceColorCalculator()
				throw();

			/** Copy constructor.
					Construct new atomDistanceColorCalculator by copying the colors and the distance of the 
					atomDistanceColorCalculator {\em color_calculator}.
					@param       color_calculator the atomDistanceColorCalculator to be copied
					@return      AtomDistanceColorCalculator new constructed atomDistanceColorCalculator copied from {\em colorCalculator}
					@see         ColorCalculator
			*/
 			AtomDistanceColorCalculator(const AtomDistanceColorCalculator& color_calculator)
				throw();

			//@}

			/** @name Destructors 
			*/
			//@{

			/** Destructor.
					Default destruction of {\em *this} atomDistanceColorCalculator.
					Calls \Ref{destroy}.
					@see         destroy
			*/
      virtual ~AtomDistanceColorCalculator()
				throw();

			/** Explicit default initialization.
					Reset the colors of {\em *this} atomDistanceColorCalculator to:
					\begin{itemize}
					  \item  internal structures cleared
					  \item  distance set to {\tt 10}
					  \item  null distance color set to red {\tt "FF0000FF"}. See \Ref{ColorRGBA} for information on how to set colors
					  \item  max distance color set to blue {\tt "00FF00FF"}. See \Ref{ColorRGBA} for information on how to set colors
					\end{itemize}
					@see  ColorRGBA
			*/
			virtual void clear()
				throw();

			/** Explicit destructor.
					Empty for further purpose.
			*/
			virtual void destroy()
				throw();

			//@}

			/**	@name	Assignment
			*/
			//@{

			/** Assignment.
					Assign the atomDistanceColorCalculator {\em color_calculator} to {\em *this}
					atomDistanceColorCalculator.
					The colors and the distance of {\em *this} atomDistanceColorCalculator are
					initialized to the colors and distance of the atomDistanceColorCalculator
					{\em color_calculator}.
					@param       color_calculator the atomDistanceColorCalculator to be copied 
			*/
			void set
				(const AtomDistanceColorCalculator& color_calculator)
				throw();

			/** Assignment operator.
					Assign the atomDistanceColorCalculator {\em color_calculator} to {\em *this}
					atomDistanceColorCalculator.
					Calls \Ref{set}.
					The colors and distance of {\em *this} atomDistanceColorCalculator are initialized
					to the colors and distance of the atomDistanceColorCalculator {\em color_calculator}. 
					@param       color_calculator the atomDistanceColorCalculator to be copied
					@return      AtomDistanceColorCalculator& constant reference of {\em *this} atomDistanceColorCalculator
					@see         set
			*/
			const AtomDistanceColorCalculator& operator =
				(const AtomDistanceColorCalculator& color_calculator)
				throw();

			/** Copying.
					Copy {\em *this} atomDistanceColorCalculator to the atomDistanceColorCalculator 
					{\em color_calculator}.
					Calls \Ref{set}.
					The colors and distance of {\em *this} atomDistanceColorCalculator are initialized
					to the colors and distance of the atomDistanceColorCalculator {\em color_calculator}. 
					@param       color_calculator the atomDistanceColorCalculator to be assigned to
					@see         set
			*/
			void get
				(AtomDistanceColorCalculator& color_calculator) const
				throw();

			/** Swapping of atomDistanceColorCalculator's.
					Swap the colors  and distance of {\em *this} atomDistanceColorCalculator with the
					atomDistanceColorCalculator	{\em color_calculator}.
					Calls \Ref{ColorRGBA::swap}.
					@param       color_calculator the atomDistanceColorCalculator being swapped with {\em *this} atomDistanceColorCalculator 
			*/
			void swap
				(AtomDistanceColorCalculator& color_calculator)
				throw();
			//@}

			/**	@name	Accessors: inspectors and mutators 
			*/
			//@{
			/** Change the max distance.
					Change the max distance of {\em *this} atomDistanceColorCalculator.
					Every new \Ref{Atom} object is inserted into {\em *this} atomDistanceColorCalculator
					with this max distance. If the method \Ref{calculateDistances} is called
					the distances of the inserted atoms are calculated.
					@param distance the new max distance of {\em *this} atomDistanceColorCalculator
					@see   getDistance
					@see   visit
			*/
			void setDistance(float distance)
				throw();

			/** Inspection of the max distance.
					Access the the max distance of {\em *this} atomDistanceColorCalculator.
					@return  float the max distance of {\em *this} atomDistanceColorCalculator
					@see     setDistance
			*/
			float getDistance() const
				throw();

			/** Change the null distance color.
					Change the null distance color of {\em *this} atomDistanceColorCalculator.
					If the calculated distance between two \Ref{Atom} objects becomes {\tt 0} this
					color will be used to color these two atoms. Otherwise the color will be
					interpolated between this color and the max distance color according to the
					calculated distance.
					@param color the new null distance color of {\em *this} atomDistanceColorCalculator
					@see   getNullDistanceColor
					@see   setMaxDistanceColor
					@see   ColorRGBA
					@see   visit
			*/
			void setNullDistanceColor(const ColorRGBA& color)
				throw();

			/** Non-mutable inspection of the null distance color.
					Access the constant reference of the null distance color of {\em *this}
					atomDistanceColorCalculator.
					@return  ColorRGBA& a constant reference to the null distance \Ref{ColorRGBA} of {\em *this} atomDistanceColorCalculator
					@see     setNullDistanceColor
					@see     ColorRGBA
			*/
			const ColorRGBA& getNullDistanceColor() const
				throw();

			/** Change the max distance color.
					Change the max distance color of {\em *this} atomDistanceColorCalculator.
					If the calculated distance between two \Ref{Atom} objects becomes the max distance
					or greater this	color will be used to color these two atoms. Otherwise the color will be
					interpolated between this color and the null distance color according to the
					calculated distance.
					@param color the new max distance color of {\em *this} atomDistanceColorCalculator
					@see   getMaxDistanceColor
					@see   setNullDistanceColor
					@see   ColorRGBA
					@see   visit
			*/
			void setMaxDistanceColor(const ColorRGBA& color)
				throw();

			/** Non-mutable inspection of the max distance color.
					Access the constant reference of the max distance color of {\em *this}
					atomDistanceColorCalculator.
					@return  ColorRGBA& a constant reference to the max distance \Ref{ColorRGBA} of {\em *this} atomDistanceColorCalculator
					@see     setMaxDistanceColor
					@see     ColorRGBA
			*/
			const ColorRGBA& getMaxDistanceColor() const
				throw();
			
			/** Calculate the distances.
					If this method is called the distances of all previously inserted \Ref{Atom} objects 
					to each other are calculated. Only \Ref{Atom} objects are used for the distance
					calculation that have not the same root. All other atom objects will keep
					the max distance as set with the method \Ref{setDistance}.
					Use this method after all atom containers have applied {\em *this} 
					atomDistanceColorCalculator.
					@see setDistance
					@see setNullDistanceColor
					@see setMaxDistanceColor
					@see visit
			*/
			void calculateDistances()
				throw();

			/** Visit method.
					Calculate the distance color of the given \Ref{Atom} {\em atom} by interpolating
					the null and max distance color according to the previously calculated 
					distance color (see \Ref{calculateDistances}) of {\em atom}.
					Access to the calculated color with the method \Ref{getColor} from the
					\Ref{ColorCalculator} class.
					@param  atom the \Ref{Atom} object whose distance color should be computed
					@see    ColorCalculator
					@see    Atom
					@see    setDistance
					@see    setNullDistanceColor
					@see    setMaxDistanceColor
			*/
			virtual void visit(Atom& atom);
			//@}

			/**	@name Processor specific methods
			*/
			//@{
			/** Start method.
					Empty for further purpose. Override this method for initializing
					the atomDistanceColorCalculator.
					@return bool {\tt true} if the start of {\em *this} atomDistanceColorCalculator was successful, {\tt false} otherwise
			*/
			virtual bool start();
			
			/** Finish method.
					Empty for further purpose. Override this method for finalizing
					the atomDistanceColorCalculator.
					@return bool {\tt true} if the finish of {\em *this} atomDistanceColorCalculator was successful, {\tt false} otherwise
			*/
			virtual bool finish();

			/**	Operator ().
					Insert the \Ref{Atom} {\em atom} with the max distance as specified by the method
					\Ref{setDistance} into {\em *this} atomDistanceColorCalculator.
					@param  atom the \Ref{Atom} to be inserted into {\em *this} atomDistanceColorCalculator
					@return Processor::Result the result of {\em *this} atomDistanceColorCalculator
					@see    setDistance
					@see    Atom
					@see    Processor
			*/
			virtual Processor::Result operator() (Atom& atom);

			/**	Operator ().
					Calls the previously declare method \Ref{operator()} if the \Ref{Composite} 
					{\em composite} is of kind \Ref{Atom}.
					This method is provided for convenience.
					@param  composite the \Ref{Composite} to be inserted into {\em *this} atomDistanceColorCalculator
					@return Processor::Result the result of {\em *this} atomDistanceColorCalculator
					@see    setDistance
					@see    Atom
					@see    Composite
					@see    Processor
			*/
			virtual Processor::Result operator() (Composite& composite);
			//@}


			private:

			typedef HashMap<void*, float> AtomDistanceHashMap;

			AtomDistanceHashMap atom_2_distance_;

			float distance_;

			ColorRGBA		null_distance_color_;
			ColorRGBA		full_distance_color_;
		};


		/** CustomColorCalculator class.
				The class CustomColorCalculator is in realty only the class ColorCalculator because
				for every given object only the default color will be returned by the method
				\Ref{getColor}. This colorCalculator can be set in the class \Ref{DisplayProperties}
				to color an entire object in one color.
				@see  ColorCalculator
				@see  DisplayProperties
		*/
		typedef ColorCalculator CustomColorCalculator;



#			ifndef BALL_NO_INLINE_FUNCTIONS
#				include <BALL/MOLVIEW/FUNCTOR/standardColorCalculator.iC>
#			endif

	} // namespace MOLVIEW

} // namespace BALL

#endif // BALL_MOLVIEW_FUNCTOR_STANDARDCOLORCALCULATOR_H
