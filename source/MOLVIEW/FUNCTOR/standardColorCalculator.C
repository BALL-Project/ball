// $Id: standardColorCalculator.C,v 1.6 2000/05/23 16:25:17 hekl Exp $

#include <BALL/MOLVIEW/FUNCTOR/standardColorCalculator.h>

namespace BALL
{
	namespace MOLVIEW
	{

	//    ElementColorCalculator
	ElementColorCalculator::ElementColorCalculator()
	{
		const unsigned char color_values[111][3] =
		{
			{255, 255, 255},   // nomatch color 0
			{255, 255, 255},   // HYDROGEN 1
			{216, 255, 255},   // HELIUM 2
			{205, 126, 255},   // LITHIUM 3
			{196, 255, 000},   // BERYLLIUM 4
			{255, 182, 182},   // BORON 5
			{144, 144, 144},   // CARBON 6
			{142, 142, 255},   // NITROGEN 7
			{240, 000, 000},   // OXYGEN 8
			{179, 255, 255},   // FLUORINE 9
			//10
			{175, 226, 244},   // NEON 10
			{170,  93, 242},   // SODIUM 11
			{137, 255, 000},   // MAGNESIUM 12
			{209, 165, 165},   // ALUMINIUM 13    
			{128, 154, 154},   // SILICON 14
			{255, 128, 000},   // PHOSPHORUS 15
			{255, 200,  40},   // SULPHUR 16
			{ 26, 240,  26},   // CHLORINE 17
			{128, 209, 228},   // ARGON 18
			{142,  65, 211},   // POTASSIUM 19
			//20
			{ 61, 255, 000},   // CALCIUM 20
			{230, 230, 228},   // SCANDIUM 21
			{191, 195, 198},   // TITANIUM 22
			{167, 165, 172},   // VANADIUM 23
			{137, 153, 198},   // CHROMIUM 24
			{156, 123, 198},   // MANGANESE 25
			{128, 123, 198},   // IRON 26
			{ 93, 109, 255},   // COBALT 27
			{ 93, 123, 195},   // NICKEL 28
			{255, 123,  98},   // COPPER 29
			//30                  
			{124, 128, 175},   // ZINC 30
			{195, 144, 144},   // GALLIUM 31
			{102, 144, 144},   // GERMANIUM 32
			{188, 128, 226},   // ARSENIC 33
			{255, 161, 000},   // SELENIUM 34
			{165,  33,  33},   // BROMINE 35
			{ 93, 186, 209},   // KRYPTON 36
			{112,  45, 177},   // RUBIDIUM 37
			{000, 255, 000},   // STRONTIUM 38
			{149, 253, 255},   // YTTRIUM 39
			//40
			{149, 225, 225},   // ZIRCONIUM 40
			{116, 195, 203},   // NIOBIUM 41
			{ 84, 181, 182},   // MOLYBDENUM 42
			{ 59, 158, 168},   // TECHNETIUM 43
			{ 35, 142, 151},   // RUTHENIUM 44  
			{ 10, 124, 140},   // RHODIUM 45
			{000, 105, 133},   // PALLADIUM 46
			{153, 198, 255},   // SILVER 47
			{255, 216, 144},   // CADMIUM 48
			{167, 117, 114},   // INDIUM 49
			//50
			{102, 128, 128},   // TIN 50
			{158, 100, 181},   // ANTIMONY 51
			{212, 123,   0},   // TELLURIUM 52
			{147, 000, 147},   // IODINE 53
			{ 66, 158, 175},   // XENON 54
			{ 86,  24, 142},   // CAESIUM 55
			{  0, 203,   0},   // BARIUM 56
			{112, 221, 255},   // LANTHANUM 57
			{255, 255, 255},   // CERIUM 58   -----
			{255, 255, 255},   // PRASEODYMIUM 59          
			//60
			{255, 255, 255},   // NEODYMIUM 60
			{255, 255, 255},   // PROMETHIUM 61
			{255, 255, 255},   // SAMARIUM 62
			{255, 255, 255},   // EUROPIUM 63
			{255, 255, 255},   // GADOLINIUM 64
			{255, 255, 255},   // TERBIUM 65
			{255, 255, 255},   // DYSPROSIUM 66
			{255, 255, 255},   // HOLMIUM 67
			{255, 255, 255},   // ERBIUM 68
			{255, 255, 255},   // THULIUM 69
			//70
			{255, 255, 255},   // YTTERBIUM 70
			{255, 255, 255},   // LUTETIUM 71
			{ 77, 193, 255},   // HAFNIUM 72
			{ 77, 167, 255},   // TANTALUM 73
			{ 38, 147, 214},   // TUNGSTEN 74         
			{ 38, 126, 172},   // RHENIUM 75
			{ 38, 103, 151},   // OSMIUM 76
			{ 24,  84, 135},   // IRIDIUM 77
			{ 24,  91, 144},   // PLATINUM 78
			{255, 209,  35},   // GOLD 79
			//80
			{181, 181, 195},   // MERCURY 80
			{167,  84,  77},   // THALLIUM 81
			{ 80,  89,  96},   // LEAD 82
			{158,  79, 181},   // BISMUTH 83
			{172,  93,   0},   // POLONIUM 84
			{117,  79,  68},   // ASTATINE 85
			{ 66, 131, 151},   // RADON 86
			{ 66,   0, 102},   // FRANCIUM 87
			{  0, 124,   0},   // RADIUM 88       
			{112, 170, 251},   // ACTINIUM 89
			//90
			{255, 255, 255},   // THORIUM 90
			{255, 255, 255},   // PROTACTINIUM 91
			{255, 255, 255},   // URANIUM 92
			{255, 255, 255},   // NEPTUNIUM 93
			{255, 255, 255},   // PLUTONIUM 94
			{255, 255, 255},   // AMERICIUM 95
			{255, 255, 255},   // CURIUM 96
			{255, 255, 255},   // BERKELIUM 97
			{255, 255, 255},   // CALIFORNIUM 98
			{255, 255, 255},   // EINSTEINIUM 99
			//100
			{255, 255, 255},   // FERMIUM 100
			{255, 255, 255},   // MENDELEVIUM 101
			{255, 255, 255},   // NOBELIUM 102
			{255, 255, 255},   // LAWRENCIUM 103        
			{255, 255, 255},   // RUTHERFORDIUM 104
			{255, 255, 255},   // HAHNIUM 105
			{255, 255, 255},   // SEABORGIUM 106
			{255, 255, 255},   // BOHRIUM 107
			{255, 255, 255},   // HASSIUM 108
			{255, 255, 255},   // MEITNERIUM 109
			{255, 255, 255}    // DUBNIUM 105
			//110
		};                                       
		
		for (Size i = 0; i < 111; i++)
		{
			color_map_.insert
				(StringHashMap<ColorRGBA>::ValueType(PTE[i].getSymbol(), 
				 ColorRGBA(color_values[i][0], color_values[i][1], color_values[i][2])));
		}
	}

	//    ResidueNameColorCalculator
	ResidueNameColorCalculator::ResidueNameColorCalculator()
	{
		const unsigned char color_values[21][3] =
		{
			{255, 255, 255},   // nomatch color 0
			{255, 255, 255},   // GLY
			{216, 255, 255},   // ALA
			{205, 126, 255},   // VAL
			{196, 255, 000},   // LEU
			{255, 182, 182},   // ILE
			{144, 144, 144},   // SER
			{142, 142, 255},   // THR
			{240, 000, 000},   // CYS
			{179, 255, 255},   // MET
			{175, 226, 244},   // PRO
			{170,  93, 242},   // ASP
			{137, 255, 000},   // ASN
			{209, 165, 165},   // GLU
			{128, 154, 154},   // GLN
			{255, 128, 000},   // LYS
			{255, 200,  40},   // ARG
			{ 26, 240,  26},   // HIS
			{128, 209, 228},   // PHE
			{142,  65, 211},   // TYR
			{ 61, 255, 000}    // TRP
		};                                       

		const char* residue_names[21] = 
		{
			{"---"},
			{"GLY"},
			{"ALA"},
			{"VAL"},
			{"LEU"},
			{"ILE"},
			{"SER"},
			{"THR"},
			{"CYS"},
			{"MET"},
			{"PRO"},
			{"ASP"},
			{"ASN"},
			{"GLU"},
			{"GLN"},
			{"LYS"},
			{"ARG"},
			{"HIS"},
			{"PHE"},
			{"TYR"},
			{"TRP"}
		};
		
		for (Size i = 0; i < 21; i++)
		{
			color_map_.insert
				(StringHashMap<ColorRGBA>::ValueType(residue_names[i],
				 ColorRGBA(color_values[i][0], color_values[i][1], color_values[i][2])));
		}
	}

	const String& ResidueNameColorCalculator::calculateKey(const Atom& atom) const
	{
		static String empty_key;
		// BAUSTELLE ---------
		const Residue* residue = atom.getAncestor(RTTI::getDefault<Residue>());
		if (residue == 0)
		{
			return empty_key;
		} else {
			return residue->getName();
		}
	}

	AtomChargeColorCalculator::AtomChargeColorCalculator()
		:	positive_color_("0000FFFF"),
			neutral_color_("FFFFFFFF"),
			negative_color_("FF0000FF")
	{
	}
	
  AtomChargeColorCalculator::AtomChargeColorCalculator(const AtomChargeColorCalculator& color_calculator)
    : positive_color_(color_calculator.positive_color_),
      neutral_color_(color_calculator.neutral_color_),
      negative_color_(color_calculator.negative_color_)
  {
	}

	void AtomChargeColorCalculator::visit(Composite& composite) 
  {
		if (RTTI::isKindOf<Atom>(composite))
		{
			visit(*RTTI::castTo<Atom>(composite));
		}
  }

 void AtomChargeColorCalculator::visit(Atom& atom)
	{
		float charge = atom.getCharge();
		float red1, green1, blue1;
		float red2, green2, blue2;

		// clip the charges to +/- 1.0
		if (charge > 1.0) charge =  1.0;
		if (charge < -1.0) charge = -1.0;

		// interpolate the color
		if (charge >= 0)
		{
			red1   = positive_color_.red();
			green1 = positive_color_.green();
			blue1  = positive_color_.blue();
		} else {
			red1   = negative_color_.red();
			green1 = negative_color_.green();
			blue1  = negative_color_.blue();
		}

		if (charge < 0.0)
		{
			charge = -charge;
		}

		red2   = neutral_color_.red();
		green2 = neutral_color_.green();
		blue2  = neutral_color_.blue();

		color_.set
			(red1 * charge + (1.0 - charge) * red2,
			 green1 * charge + (1.0 - charge) * green2,
			 blue1 * charge + (1.0 - charge) * blue2);
	}
	





	AtomDistanceColorCalculator::AtomDistanceColorCalculator()
		: UnaryProcessor<Atom>(),
			UnaryProcessor<Composite>(),
		  atom_2_distance_(),
			distance_((float)10),
			null_distance_color_("FF0000FF"),
			full_distance_color_("00FF00FF")
	{
	}
	
  AtomDistanceColorCalculator::AtomDistanceColorCalculator(const AtomDistanceColorCalculator& color_calculator)
		:	UnaryProcessor<Atom>(color_calculator),
			UnaryProcessor<Composite>(color_calculator),
			atom_2_distance_(),
			distance_(color_calculator.distance_),
			null_distance_color_(color_calculator.null_distance_color_),
			full_distance_color_(color_calculator.full_distance_color_)
  {
	}

	void AtomDistanceColorCalculator::clear()
  {
		atom_2_distance_.destroy();
		distance_ = 10.0;
		null_distance_color_.set("FF0000FF");
		full_distance_color_.set("00FF00FF");
  }

	void AtomDistanceColorCalculator::destroy()
  {
		atom_2_distance_.destroy();
  }

	void AtomDistanceColorCalculator::calculateDistances()
  {
 		AtomDistanceHashMap::Iterator it1 = atom_2_distance_.begin();
 		AtomDistanceHashMap::Iterator it1_old;

		// brute force
		for(; it1 != atom_2_distance_.end();)
		{
			Atom* atom1 = (Atom*)(it1->first);

			it1_old = it1;

			AtomDistanceHashMap::Iterator it2 = ++it1;

			for(; it2 != atom_2_distance_.end(); ++it2)
			{
				Atom* atom2 = (Atom*)(it2->first);

				if (atom1->getRoot() != atom2->getRoot())
				{
					float distance = (atom2->getPosition() - atom1->getPosition()).getLength();
					
					if (it1_old->second > distance)
					{
						it1_old->second = distance;
					}
					
					if (it2->second > distance)
					{
						it2->second = distance;
					}
				}
			}
		}
  }
 
	bool AtomDistanceColorCalculator::start()
  {
		return true;
  }
			
	bool AtomDistanceColorCalculator::finish()
  {
		return true;
  }

	Processor::Result AtomDistanceColorCalculator::operator() (Composite& composite)
  {
		if (RTTI::isKindOf<Atom>(composite))
		{
			return this->operator()(*RTTI::castTo<Atom>(composite));
		}
		
		return Processor::CONTINUE;
  }
	
	Processor::Result AtomDistanceColorCalculator::operator() (Atom& atom)
  {
 		AtomDistanceHashMap::Iterator it = atom_2_distance_.find((void*)&atom);

		// atom in hashmap ? => insert into hashmap with start distance = distance_
		if (it == atom_2_distance_.end())
		{
			atom_2_distance_.insert(AtomDistanceHashMap::ValueType((void*)&atom, distance_));
		}		

		return Processor::CONTINUE;
  }

	void AtomDistanceColorCalculator::visit(Composite& composite) 
  {
		if (RTTI::isKindOf<Atom>(composite))
		{
			visit(*RTTI::castTo<Atom>(composite));
		}
  }
	
	void AtomDistanceColorCalculator::visit(Atom& atom)
	{
 		AtomDistanceHashMap::Iterator it = atom_2_distance_.find((void*)&atom);

		float distance = distance_;

		// atom in hashmap ?
		if (it != atom_2_distance_.end())
		{
			// get distance
			distance = it->second;
		}

		float red1, green1, blue1;
		float red2, green2, blue2;

		// clip the distance to  0 - distance_
		if (distance > distance_) distance = distance_;
		if (distance < 0.0) distance = 0.0;

		red1   = null_distance_color_.red();
		green1 = null_distance_color_.green();
		blue1  = null_distance_color_.blue();

		red2   = full_distance_color_.red();
		green2 = full_distance_color_.green();
		blue2  = full_distance_color_.blue();

		color_.set
			(red1 + (distance * (red2 - red1)) / distance_,
			 green1 + (distance * (green2 - green1)) / distance_,
			 blue1 + (distance * (blue2 - blue1)) / distance_);
	}
	


#	ifndef BALL_MOLVIEW_FUNCTOR_STANDARDCOLORCALCULATOR_H
#		include <BALL/MOLVIEW/FUNCTOR/standardColorCalculator.iC>
#	endif
	}
} // namespace BALL

