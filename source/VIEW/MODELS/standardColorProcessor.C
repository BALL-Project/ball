// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: standardColorProcessor.C,v 1.45 2004/09/27 22:05:58 amoll Exp $
//

#include <BALL/VIEW/MODELS/standardColorProcessor.h>
#include <BALL/VIEW/PRIMITIVES/mesh.h>
#include <BALL/VIEW/DATATYPE/colorExtension2.h>
#include <BALL/KERNEL/PTE.h>
#include <BALL/KERNEL/residue.h>
#include <BALL/KERNEL/system.h>
#include <BALL/KERNEL/chain.h>
#include <BALL/KERNEL/protein.h>
#include <BALL/KERNEL/bond.h>
#include <BALL/KERNEL/forEach.h>
#include <BALL/KERNEL/secondaryStructure.h>

namespace BALL
{
	namespace VIEW
	{

		ElementColorProcessor::ElementColorProcessor()
			throw()
			: ColorProcessor()
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
					(HashMap<Position, ColorRGBA>::ValueType(i, 
					 ColorRGBA(color_values[i][0], color_values[i][1], color_values[i][2])));
			}
		}


		ColorRGBA ElementColorProcessor::getColor(const Composite* composite)
		{
			if (composite == 0) return default_color_;
			if (composite->isSelected()) return ColorProcessor::getColor(composite);

			if (RTTI::isKindOf<Atom>(*composite))
			{
				Position pos = ((Atom*) composite)->getElement().getAtomicNumber();
				if (color_map_.has(pos))
				{
					ColorRGBA color = color_map_[pos];
					color.setAlpha(255 - transparency_);
					return color;
				}
			}
			
			return default_color_;
		}

		////////////////////////////////////////////////////////////////////
		ResidueNameColorProcessor::ResidueNameColorProcessor()
			throw()
			: ColorProcessor()
		{
#define BALL_NR_RESIDUES 26

			const unsigned char color_values[BALL_NR_RESIDUES][3] =
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
				{ 61, 255, 000},   // TRP

				{255, 255 , 0}, // A
				{255, 0, 0},   	// C
				{0, 255, 0},  	// G
				{0, 0, 255},   	// T
				{100, 100, 255} // U
			};                                       

			const char* residue_names[BALL_NR_RESIDUES] = 
			{
				"---", "GLY", "ALA", "VAL", "LEU",
				"ILE", "SER", "THR", "CYS", "MET",
				"PRO", "ASP", "ASN", "GLU", "GLN",
				"LYS", "ARG", "HIS", "PHE", "TYR",
				"TRP", "A",   "C",   "G",   "T",
				"U"
			};
			
			for (Size i = 0; i < BALL_NR_RESIDUES; i++)
			{
				color_map_.insert
					(StringHashMap<ColorRGBA>::ValueType(residue_names[i],
					 ColorRGBA(color_values[i][0], color_values[i][1], color_values[i][2])));
			}
		}

		ColorRGBA ResidueNameColorProcessor::getColor(const Composite* composite)
		{
			if (composite == 0) return default_color_;
			if (composite->isSelected()) return ColorProcessor::getColor(composite);

			Residue residue;
			if (!RTTI::isKindOf<Residue>(*composite) &&
					composite->getAncestor(residue) == 0)
			{
				return default_color_;
			}
			
			String name; 
			if (RTTI::isKindOf<Residue>(*composite))
			{
				name = ((const Residue*) composite)->getName();
			}
			else
			{
				try
				{
					name = ((const Residue*)(composite->getAncestor(residue)))->getName();
				}
				catch(...)
				{
					return default_color_;
				}
			}

			if (color_map_.has(name))
			{
				ColorRGBA color = color_map_[name];
				color.setAlpha(255 - transparency_);
				return color;
			}

			return default_color_;
		}

		// ========================================================================
		ResidueNumberColorProcessor::ResidueNumberColorProcessor()
			throw()
			: ColorProcessor()
		{
		}

		ColorRGBA ResidueNumberColorProcessor::getColor(const Composite* composite)
		{
			if (composite == 0) return default_color_;
			if (composite->isSelected()) return ColorProcessor::getColor(composite);

			Residue residue;
			if (!RTTI::isKindOf<Residue>(*composite) &&
					composite->getAncestor(residue) == 0)
			{
				return default_color_;
			}

			Position pos;

			try
			{
				if (RTTI::isKindOf<Residue>(*composite))
				{
					pos = ((const Residue*)composite)->getID().toUnsignedShort();
				}
				else
				{
					pos = ((const Residue*)(composite->getAncestor(residue)))->getID().toUnsignedShort();
				}
			}
			catch(...)
			{
				return default_color_;
			}

			ColorRGBA color = table_.map(pos);
			color.setAlpha(255 - transparency_);
			return color;
		}

		bool ResidueNumberColorProcessor::start()
			throw()
		{
			ColorProcessor::start();
			min_ = 999999999;
			max_ = 0;
			table_.clear();
			table_ = ColorTable(500);
			ColorRGBA base_colors[3];
			base_colors[0] = first_color_;
			base_colors[1] = middle_color_;
			base_colors[2] = last_color_;
			table_.setBaseColors(base_colors, 3);


			CompositeSet::ConstIterator it = composites_->begin();
			ResidueIterator res_it;
			for(; it != composites_->end(); it++)
			{
				if (RTTI::isKindOf<System>(**it))
				{
					res_it = ((System*)*it)->beginResidue();
				}
				else if (RTTI::isKindOf<Protein>(**it))
				{
					res_it = ((Protein*)*it)->beginResidue();
				}
				else if (RTTI::isKindOf<Chain>(**it))
				{
					res_it = ((Chain*)*it)->beginResidue();
				}
				else if (RTTI::isKindOf<SecondaryStructure>(**it))
				{
					res_it = ((SecondaryStructure*)*it)->beginResidue();
				}

				for (; +res_it; ++res_it)
				{
					if ((*res_it).getName() == "HOH") continue;

					String id_string = (*res_it).getID();
					Position id;
					try
					{
						id = id_string.toUnsignedInt();
					}
					catch(...)
					{
						continue;
					}

					if (id < min_) min_ = id;
					if (id > max_) max_ = id;
				}
			}

			max_++;
			table_.setRange(min_, max_);
			table_.createTable();
			return true;
		}

		////////////////////////////////////////////////////////////////////
		AtomChargeColorProcessor::AtomChargeColorProcessor()
			throw()
			:	ColorProcessor(),
				positive_color_("0000FFFF"),
				neutral_color_("FFFFFFFF"),
				negative_color_("FF0000FF")
		{
		}

		AtomChargeColorProcessor::AtomChargeColorProcessor(const AtomChargeColorProcessor& color_processor)
			throw()
			: ColorProcessor(color_processor),
				positive_color_(color_processor.positive_color_),
				neutral_color_(color_processor.neutral_color_),
				negative_color_(color_processor.negative_color_)
		{
		}

		ColorRGBA AtomChargeColorProcessor::getColor(const Composite* composite)
		{
			if (composite == 0) return default_color_;
			if (composite->isSelected()) return ColorProcessor::getColor(composite);

			if (!RTTI::isKindOf<Atom>(*composite))
			{
				return default_color_;
			}

			Atom& atom = *(Atom*) composite;
			float charge = atom.getCharge();
			float red1, green1, blue1;
			float red2, green2, blue2;

			// clip the charges to +/- 1.0
			if 			(charge > 1.0) charge =  1.0;
			else if (charge < -1.0) charge = -1.0;

			// interpolate the color
			if (charge >= 0)
			{
				red1   = positive_color_.getRed();
				green1 = positive_color_.getGreen();
				blue1  = positive_color_.getBlue();
			} 
			else 
			{
				red1   = negative_color_.getRed();
				green1 = negative_color_.getGreen();
				blue1  = negative_color_.getBlue();
			
				charge = -charge;
			}

			red2   = neutral_color_.getRed();
			green2 = neutral_color_.getGreen();
			blue2  = neutral_color_.getBlue();

			return ColorRGBA(red1 * charge + (1.0 - charge) * red2,
											 green1 * charge + (1.0 - charge) * green2,
											 blue1 * charge + (1.0 - charge) * blue2,
											 255 - transparency_);
		}

		////////////////////////////////////////////////////////////////////
		AtomDistanceColorProcessor::AtomDistanceColorProcessor()
			throw()
			: ColorProcessor(),
				atom_2_distance_(),
				distance_((float)10),
				show_selected_(false),
				null_distance_color_("FFFF00FF"),
				full_distance_color_("0000FFFF")
		{
		}

		AtomDistanceColorProcessor::AtomDistanceColorProcessor(const AtomDistanceColorProcessor& color_processor)
			throw()
			:	ColorProcessor(color_processor),
				atom_2_distance_(),
				distance_(color_processor.distance_),
				show_selected_(color_processor.show_selected_),
				null_distance_color_(color_processor.null_distance_color_),
				full_distance_color_(color_processor.full_distance_color_)
		{
		}

		void AtomDistanceColorProcessor::calculateDistances()
			throw()
		{
			AtomDistanceHashMap::Iterator it1 = atom_2_distance_.begin();
			AtomDistanceHashMap::Iterator it1_old;
			Molecule dummy;

			// brute force
			for(; it1 != atom_2_distance_.end();)
			{
				Atom* atom1 = (Atom*)(it1->first);

				it1_old = it1;

				AtomDistanceHashMap::Iterator it2 = ++it1;
				
				for(; it2 != atom_2_distance_.end(); ++it2)
				{
					Atom* atom2 = (Atom*)(it2->first);

					if (atom1->isSelected() != atom2->isSelected())
					{
						float distance = (atom2->getPosition() - atom1->getPosition()).getLength();
						
						if (it1_old->second > distance) it1_old->second = distance;
						if (it2->second 		> distance) 	  it2->second = distance;
					}
				}
			}
		}


		void AtomDistanceColorProcessor::addAtom(const Atom& atom)
		{
			AtomDistanceHashMap::Iterator it = atom_2_distance_.find(&atom);

			// atom in hashmap ? => insert into hashmap with start distance = distance_
			if (it == atom_2_distance_.end())
			{
				atom_2_distance_.insert(AtomDistanceHashMap::ValueType(&atom, distance_));
			}		
		}

		ColorRGBA AtomDistanceColorProcessor::getColor(const Composite* composite)
		{
			if (composite == 0 ||
			    !RTTI::isKindOf<Atom>(*composite))
			{
				ColorRGBA result(default_color_);
				default_color_.setAlpha(255 - transparency_);
			}

			// hide selected items?
			if (show_selected_ && composite->isSelected())
			{
				return BALL_SELECTED_COLOR;
			}

			AtomDistanceHashMap::Iterator it = atom_2_distance_.find((Atom*)composite);

			float distance = distance_;

			// atom in hashmap ?
			if (it != atom_2_distance_.end())
			{
				// get distance
				distance = it->second;
			}

			// clip the distance to  0 - distance_
			if (distance > distance_) distance = distance_;
			if (distance < 0.0) distance = 0.0;

			float red1   = null_distance_color_.getRed();
			float green1 = null_distance_color_.getGreen();
			float blue1  = null_distance_color_.getBlue();

			float red2   = full_distance_color_.getRed();
			float green2 = full_distance_color_.getGreen();
			float blue2  = full_distance_color_.getBlue();

			return ColorRGBA(red1 + (distance * (red2 - red1)) 			/ distance_,
											 green1 + (distance * (green2 - green1)) 	/ distance_,
											 blue1 + (distance * (blue2 - blue1)) 		/ distance_,
											 255 - transparency_);
		}

		bool AtomDistanceColorProcessor::finish()
			throw()
		{
			calculateDistances();
			GeometricObjectList::Iterator it = list_.begin();
			for(; it != list_.end(); it++)
			{
				if (!RTTI::isKindOf<ColorExtension2>(**it))
				{
					ColorProcessor::operator () (*it);
					continue;
				}

				// ok, we have a two colored object
				ColorExtension2* two_colored = dynamic_cast<ColorExtension2*>(*it);
				if (RTTI::isKindOf<Bond>(*(*it)->getComposite()))
				{
					Bond* bond = (Bond*) (*it)->getComposite();
					(*it)->setColor(getColor(bond->getFirstAtom()));
					two_colored->setColor2(getColor(bond->getSecondAtom()));
				}
			}

			atom_2_distance_.clear();
			list_.clear();
			
			return true;
		}

		Processor::Result AtomDistanceColorProcessor::operator() (GeometricObject*& object)
			throw()
		{
			if (RTTI::isKindOf<Mesh>(*object))
			{
				if (!atom_grid_created_)  
				{ 
					createAtomGrid_();
				}
				CompositeSet::ConstIterator it = composites_->begin();
				for(; it != composites_->end(); it++)
				{
					if (RTTI::isKindOf<AtomContainer>(**it))
					{
						AtomIterator ait;
						AtomContainer* acont = (AtomContainer*)(*it);
						BALL_FOREACH_ATOM(*acont, ait)
						{
							addAtom(*ait);
						}
					}
					else if (RTTI::isKindOf<Atom>(**it))
					{
						const Atom* atom = dynamic_cast<const Atom*> (*it);
						addAtom(*atom);
					}
				}

				list_.push_back(object);

				return Processor::CONTINUE;
			}

			if (object->getComposite() == 0 ||
					(!RTTI::isKindOf<Atom>(*object->getComposite()) &&
					 !RTTI::isKindOf<Bond>(*object->getComposite())))
			{
				return ColorProcessor::operator () (object);
			}

			list_.push_back(object);

			if (RTTI::isKindOf<Bond>(*object->getComposite()))
			{
				addAtom(*dynamic_cast<const Bond*>(object->getComposite())->getFirstAtom());
				addAtom(*dynamic_cast<const Bond*>(object->getComposite())->getSecondAtom());
			}
			else
			{
				addAtom(*dynamic_cast<const Atom*>(object->getComposite()));
			}
			
			return Processor::CONTINUE;
		}
			
		////////////////////////////////////////////////////////////////////
		TemperatureFactorColorProcessor::TemperatureFactorColorProcessor()
			: InterpolateColorProcessor()
		{
			default_color_ = ColorRGBA(1.0,1.0,1.0);
			min_color_.set(0,0,1.0),
			max_color_.set(1.0,1.0,0),
			min_value_ = 0.00001;
			max_value_ = 50;
		}

		ColorRGBA TemperatureFactorColorProcessor::getColor(const Composite* composite)
		{
			if (composite == 0) return default_color_;
			if (composite->isSelected()) return ColorProcessor::getColor(composite);

			if (!RTTI::isKindOf<PDBAtom>(*composite))
			{
				return default_color_;
			}

			return interpolateColor((dynamic_cast<const PDBAtom*>(composite))->getTemperatureFactor());
		}

		////////////////////////////////////////////////////////////////////
		OccupancyColorProcessor::OccupancyColorProcessor()
			: InterpolateColorProcessor()
		{
			default_color_ = ColorRGBA(1.0, 1.0, 1.0);
			min_color_.set(0, 0, 1.0),
			max_color_.set(1.0, 1.0, 0),
			min_value_ = 0;
			max_value_ = 1;
		}

		ColorRGBA OccupancyColorProcessor::getColor(const Composite* composite)
		{
			if (composite == 0) return default_color_;
			if (composite->isSelected()) return ColorProcessor::getColor(composite);

			const PDBAtom* atom = dynamic_cast<const PDBAtom*>(composite);
			if (atom == 0)			
			{
				return default_color_;
			}
			else
			{
				return interpolateColor(atom->getOccupancy());
			}
		}
		
		////////////////////////////////////////////////////////////////////
		ForceColorProcessor::ForceColorProcessor()
			: InterpolateColorProcessor()
		{
			default_color_ = ColorRGBA(1.0, 1.0, 1.0);
			min_color_.set(0, 0, 1.0),
			max_color_.set(1.0, 0, 0),
			min_value_ = 0;
			max_value_ = 10;
		}

		ColorRGBA ForceColorProcessor::getColor(const Composite* composite)
		{
			if (composite == 0) return default_color_;
			if (composite->isSelected()) return ColorProcessor::getColor(composite);

			if (!RTTI::isKindOf<Atom>(*composite)) return default_color_;

			Vector3 force = (dynamic_cast<const Atom*>(composite))->getForce();
			if (force.getSquareLength() == 0) return min_color_;
			force *= pow(10.0, 12.0);

			float forcev = log(force.getLength());

			return interpolateColor(forcev);
		}

		////////////////////////////////////////////////////////////////////
		SecondaryStructureColorProcessor::SecondaryStructureColorProcessor()
			: ColorProcessor(),
			  helix_color_(0,0,255),
				coil_color_(0,155,155),
				strand_color_(255,0,0),
				turn_color_(255,255,0)
		{
		}

		ColorRGBA SecondaryStructureColorProcessor::getColor(const Composite* composite)
		{
			if (composite == 0) return default_color_;
			if (composite->isSelected()) return ColorProcessor::getColor(composite);

			const SecondaryStructure* ss = 0;
			if (RTTI::isKindOf<SecondaryStructure>(*composite))
			{
				ss = (const SecondaryStructure*) composite;
			}
			else 
			{
				SecondaryStructure dummy;
				ss = (const SecondaryStructure*) composite->getAncestor(dummy);
			}

			ColorRGBA color = default_color_;
			if (ss != 0) 
			{
				if (ss->getType() == SecondaryStructure::HELIX)
				{
					color = helix_color_;
				}
				else if (ss->getType() == SecondaryStructure::COIL)
				{
					color = coil_color_;
				}
				else if (ss->getType() == SecondaryStructure::STRAND)
				{
					color = strand_color_;
				}
				else if (ss->getType() == SecondaryStructure::TURN)
				{
					color = turn_color_;
				}
			}
			return color;
		}

		void SecondaryStructureColorProcessor::setTransparency(Size t)
			throw()
		{
			transparency_ = t;
			helix_color_.setAlpha(255-transparency_);
			coil_color_.setAlpha(255-transparency_);
			strand_color_.setAlpha(255-transparency_);
			turn_color_.setAlpha(255-transparency_);
		}

		void SecondaryStructureColorProcessor::setHelixColor(const ColorRGBA& color)
			throw()
		{
			helix_color_ = color;
			helix_color_.setAlpha(255-transparency_);
		}

		void SecondaryStructureColorProcessor::setCoilColor(const ColorRGBA& color)
			throw()
		{
			coil_color_ = color;
			coil_color_.setAlpha(255-transparency_);
		}

		void SecondaryStructureColorProcessor::setStrandColor(const ColorRGBA& color)
			throw()
		{
			strand_color_ = color;
			strand_color_.setAlpha(255-transparency_);
		}

		void SecondaryStructureColorProcessor::setTurnColor(const ColorRGBA& color)
			throw()
		{
			turn_color_ = color;
			turn_color_.setAlpha(255-transparency_);
		}

		const ColorRGBA& SecondaryStructureColorProcessor::getHelixColor() const
			throw()
		{
			return helix_color_;
		}

		const ColorRGBA& SecondaryStructureColorProcessor::getCoilColor() const
			throw()
		{
			return coil_color_;
		}

		const ColorRGBA& SecondaryStructureColorProcessor::getStrandColor() const
			throw()
		{
			return strand_color_;
		}

		const ColorRGBA& SecondaryStructureColorProcessor::getTurnColor() const
			throw()
		{
			return turn_color_;
		}

#	ifdef BALL_NO_INLINE_FUNCTIONS
#		include <BALL/VIEW/MODELS/standardColorProcessor.iC>
#	endif

	} // namespace VIEW
} // namespace BALL

