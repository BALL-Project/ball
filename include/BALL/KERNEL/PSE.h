// $Id: PSE.h,v 1.4 2000/01/10 15:51:00 oliver Exp $

#ifndef BALL_KERNEL_PSE_H
#define BALL_KERNEL_PSE_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

#include <ctype.h> 	//  toupper, tolower
#include <stdlib.h> //  bsearch

#ifndef BALL_CONCEPT_PROPERTY_H
#	include <BALL/CONCEPT/property.h>
#endif

#ifndef BALL_DATATYPE_STRING_H
#	include <BALL/DATATYPE/string.h>
#endif

#ifndef BALL_CONCEPT_PROCESSOR_H
#	include <BALL/CONCEPT/processor.h>
#endif

/**@name	Periodic System of Elements
*/
//@{

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


	/**	Element class
	*/
	class Element
		: public PropertyManager
	{
		public:

			BALL_CREATE(Element)

			/**		Group
			*/
			typedef short Group;

			/**	 Period
			*/
			typedef short Period;

			/**	 AtomicNumber
			*/
			typedef short AtomicNumber;

			/**	Name
					Element names.
			*/
			enum Name
			{
				ACTINIUM = 0,
				ALUMINIUM,
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
			*/
			Element();

			/**	Constructor
			*/
			Element(const Element& element, bool deep = true);

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
				 float electronegativity);

			/**	Destructor
			*/
			virtual ~Element();

			//@}


			/**@name	Debugging and Diagnostics
			*/
			//@{

			/**
			*/
			static const char *getErrorMessage(ErrorCode error_code);

			//@}

			/**@name	Inspectors and Mutators
			*/
			//@{
		
			/**
			*/
			void setName(const String& name);

			/**
			*/
			String& getName();

			/**
			*/
			const String& getName() const;

			///
			void setSymbol(const String& symbol);

			///
			String& getSymbol();

			///
			const String& getSymbol() const;

			///
			void setGroup(Group group);

			///
			Group getGroup() const;

			///
			void setPeriod(Period period);

			///
			Period getPeriod() const;

			///
			void setAtomicNumber(AtomicNumber atomic_number);

			///
			AtomicNumber getAtomicNumber() const;

			///
			void setAtomicWeight(float atomic_weight);

			///
			float getAtomicWeight() const;

			///
			void setAtomicRadius(float atomic_radius);

			///
			float getAtomicRadius() const;

			///
			void setCovalentRadius(float covalent_radius);

			///
			float getCovalentRadius() const;

			///
			void setVanDerWaalsRadius(float van_der_waals_radius);

			///
			float getVanDerWaalsRadius() const;

			///
			void setElectronegativity(float electronegativity);

			///
			float getElectronegativity() const;

			//@}

			/**@name	Operators
			*/
			//@{

			///
			bool operator ==(const Element& element) const;

			///
			bool operator !=(const Element& element) const;

			///
			bool operator <(const Element& element) const;

			///
			bool operator <=(const Element& element) const;

			///
			bool operator >=(const Element& element) const;

			///
			bool operator >(const Element& element) const;

			///
			friend std::ostream &operator << (std::ostream& s, const Element& element);

			//@}

			/**@name	Predicates
			*/
			//@{

			///
			bool isUnknown() const;

			//@}

			///		
			static Element UNKNOWN;


		protected:

		private:

			/** the name of the element */
			String name_;

			/** the uppercased symbol of the element */
			String symbol_;

			/** The group number is an identifier used to describe the 
			 * column of the standard periodic table in which the 
			 * element appears. 
			 *
			 * NOTES:
			 * There is considerable confusion surrounding the Group 
			 * labels. The scheme used in WebElements is numeric and 
			 * is the current IUPAC convention. The other two systems 
			 * are less desirable since they are confusing, but still 
			 * in common usage. The designations A and B are completely
			 * arbitrary. The first of these (A left, B right) is based 
			 * upon older IUPAC recommendations and frequently used in 
			 * Europe. The last set (main group elements A, transition 
			 * elements B) was in common use in America.
			 *
			 * IUPAC, European, and American Group labelling schemes
			 *
			 *  Group   European  American
			 *  1       IA        IA    
			 *  2       IIA       IIA            
			 *  3       IIIA      IIIB    
			 *  4       IVA       IVAB   
			 *  5       VA        VB  
			 *  6       VIA       VIB   
			 *  7       VIIA      VIIB    
			 *  8       VIIIA     VIIIB     
			 *  9       VIIIA     VIIIB     
			 *  10      VIIIA     VIIIB      
			 *  11      IB        IB   
			 *  12      IIB       IIB    
			 *  13      IIIB      IIIA     
			 *  14      IVB       IVA    
			 *  15      VB        VA                    
			 *  16      VIB       VIA    
			 *  17      VIIB      VIIA     
			 *  18      VIIIB     VIIIA      
			 */
			Group 	group_;

			/** the period of the element */
			Period	period_;

			/** The atomic number corresponds to the number of protons 
			 * in the nucleus of an atom of that element. */
			AtomicNumber atomic_number_;

			/** The atomic weight of an element (resp. of its most stabile 
			 * isotope) is the ratio of the mass of one mole of the 
			 * element in a defined source to 1/12 of the mass of 
			 * chemically unbound C-12 in its nuclear and electronic
			 * ground state. Adapted from the 1993 report of the IUPAC 
			 * Commission on Atomic Weights and Isotopic Abundances */
			float atomic_weight_;
		
			/** atomic radius (in Angstrom) */
			float atomic_radius_;

			/** covalent radius (in Angstrom) */
			float  covalent_radius_;

			/** van der Waals radius (in Angstrom) */
			float van_der_waals_radius_;

			/** electronegativity (according to the Pauling scale) */
			float electronegativity_;
	};


	/**	Periodic System class
	*/
	class PSE_
		: public PropertyManager
	{
		public:

			BALL_CREATE(PSE_)

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
			PSE_();

			/**	Default constructor
			*/
			PSE_(const PSE_& pse, bool deep);

			/**	Destructor
			*/
			virtual ~PSE_();

			//@}

			/**	@name	Accessors
			*/
			//@{

			/**
			*/
			static Element& getElement(Index index);
		
			/**
			*/
			static Element& getElement(const String& symbol);

			//@}

			/**	@name	Operators
			*/
			//@{
		
			/**
			*/
			Element &operator [](const String& symbol);

			/**
			*/
			const Element &operator [](const String& symbol) const;

			/**
			*/
			Element &operator [](Element::Name name);

			/**
			*/
			const Element &operator [](Element::Name name) const;

			/**
			*/
			Element &operator [](Element::Symbol symbol);

			/**
			*/
			const Element &operator [](Element::Symbol symbol) const;

			/**
			*/
			Element &operator [](Index index);

			/**
			*/
			const Element &operator [](Index index) const;

			//@}

			/**	@name	Application methods
			*/
			//@{
		
			/**
			*/
			static bool apply(UnaryProcessor<Element>& applicator);

			//@}

		protected:
		 
		private:

			///
			struct SymbolToElement_
			{
				///
				const char* symbol;

				///
				Element* 		element;
			};

			///
			static int compare_(const SymbolToElement_* a, const SymbolToElement_* b);

			///
			static Element element_[];

			///
			static SymbolToElement_ symbol_to_element_[];

			///
			static Element* atomic_number_to_element_[];
	};


	/**	@memo	Global static instance of periodic system.
	*/
	extern PSE_ PSE;

	//@}


#	ifndef BALL_NO_INLINE_FUNCTIONS
#		include <BALL/KERNEL/PSE.iC>
#	endif

} //namespace BALL 


#endif // BALL_KERNEL_PSE_H
