// $Id: PTE.h,v 1.16 2001/07/15 21:21:01 amoll Exp $

#ifndef BALL_KERNEL_PTE_H
#define BALL_KERNEL_PTE_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

#ifndef BALL_CONCEPT_PROPERTY_H
#	include <BALL/CONCEPT/property.h>
#endif

#ifndef BALL_DATATYPE_STRING_H
#	include <BALL/DATATYPE/string.h>
#endif

#ifndef BALL_CONCEPT_PROCESSOR_H
#	include <BALL/CONCEPT/processor.h>
#endif

#include <ctype.h> 	//  toupper, tolower
#include <stdlib.h> //  bsearch

#define BALL_ELEMENT_NAME_DEFAULT                "Unknown"
#define BALL_ELEMENT_SYMBOL_DEFAULT              "?"
#define BALL_ELEMENT_GROUP_DEFAULT               0
#define BALL_ELEMENT_PERIOD_DEFAULT              0
#define BALL_ELEMENT_ATOMICNUMBER_DEFAULT        0
#define BALL_ELEMENT_ATOMICWEIGHT_DEFAULT        0
#define BALL_ELEMENT_ATOMICRADIUS_DEFAULT        0
#define BALL_ELEMENT_COVALENTRADIUS_DEFAULT      0
#define BALL_ELEMENT_VANDERWAALSRADIUS_DEFAULT   0
#define BALL_ELEMENT_ELECTRONEGATIVITY_DEFAULT   0


namespace BALL 
{

	/**	@name	Periodic Table of Elements
			These classes represent elements and the periodic table of elements.
			The \Ref{Element} class is used to describe common properties of atoms
			(e.g., mass, nuclear charge, element symbol). Elements can be retrieved
			from the \Ref{PTE} object ({\bf P}eriodic {\bf T}able of {\bf E}lements)
			which is a global instance of \Ref{PTE_}.\\
			{\bf Definition:}\URL{BALL/KERNEL/PTE.h}
			@see Atom::setElement
			@see Atom::getElement
	*/
	//@{

	/**	Element class.
			{\bf Definition:}\URL{BALL/KERNEL/PTE.h}
	*/
	class Element
		: public PropertyManager
	{
		public:

			BALL_CREATE(Element)

			/**	Group
			*/
			typedef short Group;

			/**	Period
			*/
			typedef short Period;

			/**	AtomicNumber
			*/
			typedef short AtomicNumber;

			/**	Name
					Element names.
			*/
			enum Name
			{
				ACTINIUM = 0,
				ALUMINUM,
				AMERICIUM,
				ANTIMONY, STIBIUM = ANTIMONY,
				ARGON,
				ARSENIC,
				ASTATINE,
				BARIUM,
				BERKELIUM,
				BERYLLIUM,
				BISMUTH, WISMUT = BISMUTH,
				BOHRIUM,
				BORON,
				BROMINE,
				CADMIUM,
				CAESIUM,
				CALCIUM,
				CALIFORNIUM,
				CARBON, CARBONEUM = CARBON,
				CERIUM,
				CHLORINE,
				CHROMIUM,
				COBALT,
				COPPER, CUPRUM = COPPER,
				CURIUM,
				DUBNIUM,
				DYSPROSIUM,
				EINSTEINIUM,
				ERBIUM,
				EUROPIUM,
				FERMIUM,
				FLUORINE,
				FRANCIUM,
				GADOLINIUM,
				GALLIUM,
				GERMANIUM,
				GOLD, AURUM = GOLD,
				HAFNIUM,
				HAHNIUM,
				HELIUM,
				HOLMIUM,
				HYDROGEN, HYDROGENIUM = HYDROGEN,
				INDIUM,
				IODINE, JOD = IODINE,
				IRIDIUM,
				IRON, FERRUM = IRON,
				JOLIOTIUM,
				KRYPTON,
				LANTHANUM,
				LAWRENCIUM,
				LEAD, PLUMBUM = LEAD,
				LITHIUM,
				LUTETIUM,
				MAGNESIUM,
				MANGANESE, MANGAN = MANGANESE,
				MEITNERIUM,
				MENDELEVIUM,
				MERCURY, HYDRARGYRUM = MERCURY, MERCURIUM = MERCURY,
				MOLYBDENUM,
				NEODYMIUM,
				NEON,
				NEPTUNIUM,
				NICKEL,
				NIOBIUM,
				NITROGEN, NITROGENIUM = NITROGEN,
				NOBELIUM,
				OSMIUM,
				OXYGEN, OXYGENIUM = OXYGEN,
				PALLADIUM,
				PHOSPHORUS,
				PLATINUM,
				PLUTONIUM,
				POLONIUM,
				POTASSIUM, KALIUM = POTASSIUM,
				PRASEODYMIUM,
				PROMETHIUM,
				PROTACTINIUM,
				RADIUM,
				RADON,
				RHENIUM,
				RHODIUM,
				RUBIDIUM,
				RUTHENIUM,
				RUTHERFORDIUM,
				SAMARIUM,
				SCANDIUM,
				SELENIUM,
				SILICON, SILICIUM = SILICON,
				SILVER, ARGENTUM = SILVER,
				SODIUM, NATRIUM = SODIUM,
				STRONTIUM,
				SULPHUR, SULFUR = SULPHUR,
				TANTALUM,
				TECHNETIUM,
				TELLURIUM,
				TERBIUM,
				THALLIUM,
				THORIUM,
				THULIUM,
				TIN, STANNUM = TIN,
				TITANIUM,
				TUNGSTEN, WOLFRAM = TUNGSTEN,
				UNUNBIUM,
				UNUNNILIUM,
				UNUNNINIUM,
				URANIUM,
				VANADIUM,
				XENON,
				YTTERBIUM,
				YTTRIUM,
				ZINC, ZINCUM = ZINC,
				ZIRCONIUM,
				
				NUMBER_OF_ELEMENTS //  111
			};

			/**	@name	Symbol
					Element symbols.
			*/
			enum Symbol
			{
				Ac = 0,
				Al,
				Am,
				Sb,
				Ar,
				As,
				At,
				Ba,
				Bk,
				Be,
				Bi,
				Bh,
				B,
				Br,
				Cd,
				Cs,
				Ca,
				Cf,
				C,
				Ce,
				Cl,
				Cr,
				Co,
				Cu,
				Cm,
				Db,
				Dy,
				Es,
				Er,
				Eu,
				Fm,
				F,
				Fr,
				Gd,
				Ga,
				Ge,
				Au,
				Hf,
				Hn,
				He,
				Ho,
				H,
				In,
				I,
				Ir,
				Fe,
				Jl,
				Kr,
				La,
				Lr,
				Pb,
				Li,
				Lu,
				Mg,
				Mn,
				Mt,
				Md,
				Hg,
				Mo,
				Nd,
				Ne,
				Np,
				Ni,
				Nb,
				N,
				No,
				Os,
				O,
				Pd,
				P,
				Pt,
				Pu,
				Po,
				K,
				Pr,
				Pm,
				Pa,
				Ra,
				Rn,
				Re,
				Rh,
				Rb,
				Ru,
				Rf,
				Sm,
				Sc,
				Se,
				Si,
				Ag,
				Na,
				Sr,
				S,
				Ta,
				Tc,
				Te,
				Tb,
				Tl,
				Th,
				Tm,
				Sn,
				Ti,
				W,
				Uub,
				Uun,
				Uuu,
				U,
				V,
				Xe,
				Yb,
				Y,
				Zn,
				Zr
			};

			/**	@name	Property
					Enum to define atom element properties.
			*/
			enum Property
			{
				/**	@name NUMBER_OF_PROPERTIES
				*/
				NUMBER_OF_PROPERTIES
			};
			
			/**	@name	Constructors and Destructors.
			*/
			//@{
			
			/**	Default constructor
					The instance is set to the default values
					(= \Ref{UNKNOWN} element).
			*/
			Element()
				throw();

			/**	Constructor
			*/
			Element(const Element& element)
				throw();

			/**	Detailed constructor
			*/
			Element
				(const String& name,
				 const String& symbol,
				 Group group,
				 Period period,
				 AtomicNumber atomic_umber,
				 float atomic_weight,
				 float atomic_radius,
				 float covalent_radius,
				 float van_der_waals_radius,
				 float electronegativity)
			 	throw();

			/**	Destructor
			*/
			virtual ~Element()
				throw();

			/** Clear method.
					The instance is set to the default values
					(= \Ref{UNKNOWN} element).
			*/
			virtual void clear()
				throw();
			
			//@}
			/**	@name	Assignment 
			*/
			//@{

			/** Assignment operator.
					@param   element the Element to be copied (cloned)
					@return  Element& - this instance
			*/
			const Element& operator = (const Element& element)
				throw();

			//@}
			/** @name	Inspectors and Mutators
			*/
			//@{
		
			/** Set the name.
					@param name the new name
			*/
			void setName(const String& name)
				throw();

			/** Get the name.
					@return String - the name
			*/
			const String& getName() const
				throw();

			/** Set the atomic symbol.
					@param symbol the new symbol
			*/
			void setSymbol(const String& symbol)
				throw();

			/** Get the atomic symbol.
					@return String the symbol
			*/
			const String& getSymbol() const
				throw();

			/** Set the atomic group.
					@param group (short) the new group
			*/
			void setGroup(Group group)
				throw();

			/** Get the atomic group.
					@return Group (short) - the group
			*/
			Group getGroup() const
				throw();

			/** Set the atomic period.
					@param period (short) the new period
			*/
			void setPeriod(Period period)
				throw();

			/** Get the atomic period.
					@return Period (short) the period
			*/
			Period getPeriod() const
				throw();

			/** Set the atomic number.
					@param atomic_number the new atomic number
			*/
			void setAtomicNumber(AtomicNumber atomic_number)
				throw();

			/** Get the atomic number.
					@return AtomicNumber (short) - the atomic number
			*/
			AtomicNumber getAtomicNumber() const
				throw();

			/** Set the atomic weight.
					@param atomic_weight the new atomic weight
			*/
			void setAtomicWeight(float atomic_weight)
				throw();

			/** Get the atomic weight.
					@return float - the atomic weight
			*/
			float getAtomicWeight() const
				throw();

			/** Set the atomic radius.
					@param atomic_radius the new atomic radius (in Angstrom)
			*/
			void setAtomicRadius(float atomic_radius)
				throw();

			/** Get the atomic radius.
					@return float the atomic radius (in Angstrom)
			*/
			float getAtomicRadius() const
				throw();

			/** Set the covalent radius.
					@param covalent_radius the new covalent radius (in Angstrom)
			*/
			void setCovalentRadius(float covalent_radius)
				throw();

			/** Get the covalent radius.
					@return float the covalent radius (in Angstrom)
			*/
			float getCovalentRadius() const
				throw();

			/** Set the VanDerWaals radius.
					@param van_der_waals_radius the new VanDerWaals radius (in Angstrom)
			*/
			void setVanDerWaalsRadius(float van_der_waals_radius)
				throw();

			/** Get the VanDerWaals radius.
					@return float the VanDerWaals radius (in Angstrom)
			*/
			float getVanDerWaalsRadius() const
				throw();

			/** Set the electronegativity.
					@param electronegativity the new electronegativity (according to the Pauling scale)
			*/
			void setElectronegativity(float electronegativity)
				throw();

			/** Get the electronegativity.
					@return float the electronegativity (according to the Pauling scale)
			*/
			float getElectronegativity() const
				throw();

			//@}
			/** @name	Operators
			*/
			//@{

			/** Equality operator.
					Tests if the atomic number of two elements are equal.
					@param element, the Element to compare with
					@return bool
			*/
			bool operator == (const Element& element) const
				throw();

			/** Inequality operator.
					Tests if the atomic number of two elements are inequal.
					@param element, the Element to compare with
					@return bool
			*/
			bool operator != (const Element& element) const
				throw();

			/** Less operator.
					Tests if the atomic number of this instance is less
					compared with {\em element}.
					@param element, the Element to compare with
					@return bool
			*/
			bool operator < (const Element& element) const
				throw();

			/** Less or equal operator.
					Tests if the atomic number of this instance is less
					or equal compared with {\em element}.
					@param element, the Element to compare with
					@return bool
			*/
			bool operator <= (const Element& element) const
				throw();

			/** Greater or equal operator.
					Tests if the atomic number of this instance is greater
					or equal compared with {\em element}.
					@param element, the Element to compare with
					@return bool
			*/
			bool operator >= (const Element& element) const
				throw();

			/** Greater operator.
					Tests if the atomic number of this instance is greater
					than {\em element}.
					@param element, the Element to compare with
					@return bool
			*/
			bool operator >(const Element& element) const
				throw();

			/**	Output operator
					Prints {\em this} instance of Element.
			*/
			friend std::ostream& operator << (std::ostream& s, const Element& element)
				throw();

			//@}
			/** @name	Predicates
			*/
			//@{

			/** Test if this instance is unknown.
					Returns true if instance is equal the static Element UNKNOWN
					(The default constructor returns such an element).
					@return bool
			*/
			bool isUnknown() const
				throw();

			//@}

			/** Standard element.
					(The default constructor returns such an element).					
			*/
			static Element UNKNOWN;


		private:

			/** the name of the element 
			*/
			String name_;

			/** the uppercased symbol of the element 
			*/
			String symbol_;

			/** The group number is an identifier used to describe the 
					column of the standard periodic table in which the 
				  element appears. 

				  NOTES:
				  There is considerable confusion surrounding the Group 
				  labels. The scheme used in WebElements is numeric and 
				  is the current IUPAC convention. The other two systems 
				  are less desirable since they are confusing, but still 
				  in common usage. The designations A and B are completely
				  arbitrary. The first of these (A left, B right) is based 
				  upon older IUPAC recommendations and frequently used in 
				  Europe. The last set (main group elements A, transition 
				  elements B) was in common use in America.

				  IUPAC, European, and American Group labelling schemes

					Group   European  American
					1       IA        IA    
					2       IIA       IIA            
					3       IIIA      IIIB    
					4       IVA       IVAB   
					5       VA        VB  
					6       VIA       VIB   
					7       VIIA      VIIB    
					8       VIIIA     VIIIB     
					9       VIIIA     VIIIB     
					10      VIIIA     VIIIB      
					11      IB        IB   
					12      IIB       IIB    
					13      IIIB      IIIA     
					14      IVB       IVA    
					15      VB        VA                    
					16      VIB       VIA    
					17      VIIB      VIIA     
					18      VIIIB     VIIIA      
			*/
			Group 	group_;

			/** the period of the element 
			*/
			Period	period_;

			/** The atomic number corresponds to the number of protons 
			    in the nucleus of an atom of that element. 
			 */
			AtomicNumber atomic_number_;

			/** The atomic weight of an element (resp. of its most stabile 
			    isotope) is the ratio of the mass of one mole of the 
			    element in a defined source to 1/12 of the mass of 
			    chemically unbound C-12 in its nuclear and electronic
			    ground state. Adapted from the 1993 report of the IUPAC 
			    Commission on Atomic Weights and Isotopic Abundances 
			*/
			float atomic_weight_;
		
			/** atomic radius (in Angstrom) 
			*/
			float atomic_radius_;

			/** covalent radius (in Angstrom) 
			*/
			float  covalent_radius_;

			/** van der Waals radius (in Angstrom) 
			*/
			float van_der_waals_radius_;

			/** electronegativity (according to the Pauling scale) 
			*/
			float electronegativity_;
	};


	/**	Element output operator.
			Prints the contents of an instance of \Ref{Element} to an {\tt ostream}.
	*/
	std::ostream& operator << (std::ostream& s, const Element& element)
		throw();


	/**	Periodic Table of Elements Class
	*/
	class PTE_
		: public PropertyManager
	{
		public:

			BALL_CREATE(PTE_)

			/**	
			*/
			enum Property
			{
				NUMBER_OF_PROPERTIES
			};

			/**	@name	Constructors and Destructors
			*/
			//@{

			/**	Default constructor
			*/
			PTE_()
				throw();

			/**	Default constructor
			*/
			PTE_(const PTE_& pse)
				throw();

			/**	Destructor
			*/
			virtual ~PTE_()
				throw();

			/** Clear method.
					This method just calls \Ref{PropertyManager::clear}.
			*/
			virtual void clear()
				throw();

			//@}
			/**	@name	Assignment 
			*/
			//@{
		
			/** Assignment operator
					Does not assign anything.
					Implemented just for the OCI.
			*/
			const PTE_& operator = (const PTE_& /*pte*/)
				throw();
			
			//@}
			/**	@name	Accessors
			*/
			//@{

			/** Return the element at a given position in the PTE.
					The position is equal to the atomic number.
					@return Element the static Element at the position {\em position}
			*/
			static Element& getElement(Position position)
				throw();
		
			/** Return the element with a given symbol.
					@return Element the static Element with the symbol {\em symbol}
			*/
			static Element& getElement(const String& symbol)
				throw();

			//@}
			/**	@name	Operators
			*/
			//@{
		
			/** Equality operator.
					Always returns true.
					Implemented just for the OCI.
			*/
			bool operator == (const PTE_& pte) const
				throw();

			/** Access operator with a given symbol.
					@return Element the Element with the symbol {\em symbol}
			*/
			Element& operator [] (const String& symbol)
				throw();

			/** Access operator with a given symbol.
					@return Element the const Element with the symbol {\em symbol}
			*/
			const Element& operator [] (const String& symbol) const
				throw();

			/** Access operator with a given name.
					@return Element the Element with the name {\em name}
			*/
			Element& operator [] (Element::Name name)
				throw();

			/** Access operator with a given name.
					@return Element the const Element with the name {\em name}
			*/
			const Element& operator [] (Element::Name name) const
				throw();

			/** Access operator with a given Element::Symbol.
					@return Element the Element with the symbol {\em symbol}
			*/
			Element& operator [] (Element::Symbol symbol)
				throw();

			/** Access operator with a given Element::Symbol.
					@return Element the const Element with the symbol {\em symbol}
			*/
			const Element& operator [] (Element::Symbol symbol) const
				throw();

			/** Access operator with a given position.
					@return Element the Element with the position {\em position}
			*/
			Element& operator [] (Position position)
				throw();

			/** Access operator with a given position.
					@return Element the const Element with the position {\em position}
			*/
			const Element& operator [] (Position position) const
				throw();

			//@}
			/**	@name	Application methods
			*/
			//@{
		
			/** Application of an unary processor on every contained element.
					@param  processor a typed unary processor for Element instances
					@return  bool - {\tt true} if application has been terminated successfully,
													{\tt false} otherwise
			*/
			static bool apply(UnaryProcessor<Element>& applicator)
				throw();

			//@}

			///
			struct SymbolToElement
			{
				///
				const char* symbol;

				///
				Element* 		element;
			};

		private:

			///
			static Element element_[];

			///
			static SymbolToElement symbol_to_element_[];

			///
			static Element* atomic_number_to_element_[];
	};


	/**	@memo	Global static instance of the periodic table.
	*/
	extern PTE_ PTE;

	//@}


#	ifndef BALL_NO_INLINE_FUNCTIONS
#		include <BALL/KERNEL/PTE.iC>
#	endif

} //namespace BALL 


#endif // BALL_KERNEL_PTE_H
