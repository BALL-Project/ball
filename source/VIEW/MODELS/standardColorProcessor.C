// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: standardColorProcessor.C,v 1.56.18.1 2007/03/25 22:02:31 oliver Exp $
//

#include <BALL/VIEW/MODELS/standardColorProcessor.h>
#include <BALL/VIEW/PRIMITIVES/mesh.h>
#include <BALL/VIEW/DATATYPE/colorExtensions.h>
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

#define BALL_VIEW_NUMBER_ELEMENTS 111

		ElementColorProcessor::ElementColorProcessor()
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
			
			for (Size i = 0; i < BALL_VIEW_NUMBER_ELEMENTS; i++)
			{
				color_map_.insert
					(HashMap<Position, ColorRGBA>::ValueType(i, 
					 ColorRGBA(color_values[i][0], color_values[i][1], color_values[i][2])));
			}
		}

		void ElementColorProcessor::setTransparency(Size value)
		{
			ColorProcessor::setTransparency(value);
			HashMap<Position, ColorRGBA>::Iterator it = color_map_.begin();
			for (;it != color_map_.end(); it++)
			{
				it->second.setAlpha(255 - value);
			}
		}

		void ElementColorProcessor::getColor(const Composite& composite, ColorRGBA& color_to_be_set)
		{
			const Atom* atom = dynamic_cast<const Atom*>(&composite);
			if (atom != 0)
			{
				HashMap<Position, ColorRGBA>::Iterator it(
						color_map_.find(
							atom->getElement().getAtomicNumber()));

				if (it != color_map_.end())
				{
					color_to_be_set.set(it->second);
					return;
				}
			}
			
			color_to_be_set.set(default_color_);
		}

		////////////////////////////////////////////////////////////////////
		ResidueNameColorProcessor::ResidueNameColorProcessor()
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

		void ResidueNameColorProcessor::setTransparency(Size value)
		{
			ColorProcessor::setTransparency(value);
			StringHashMap<ColorRGBA>::Iterator it = color_map_.begin();
			for (;it != color_map_.end(); it++)
			{
				it->second.setAlpha(255 - value);
			}
		}

		bool ResidueNameColorProcessor::canUseMeshShortcut_(const Composite& composite)
		{
			return RTTI::isKindOf<Residue>(composite);
		}

		void ResidueNameColorProcessor::getColor(const Composite& composite, ColorRGBA& color_to_be_set)
		{
			const Residue* residue = dynamic_cast<const Residue*>(&composite);
			if (residue == 0)
			{
				residue = composite.getAncestor(dummy_residue);
				if (residue == 0)
				{
					color_to_be_set.set(default_color_);
					return;
				}
			}
			
			StringHashMap<ColorRGBA>::Iterator it(color_map_.find(residue->getName()));
			if (it != color_map_.end())
			{
				color_to_be_set.set(it->second);
				return;
			}

			color_to_be_set.set(default_color_);
		}

		// ========================================================================
		ResidueNumberColorProcessor::ResidueNumberColorProcessor()
			: ColorProcessor(),
				first_color_("FF0000"),
				middle_color_("00FF00"),
				last_color_("0000FF"),
				dummy_residue_()
		{
		}

		bool ResidueNumberColorProcessor::canUseMeshShortcut_(const Composite& composite)
		{
			return RTTI::isKindOf<Residue>(composite);
		}

		void ResidueNumberColorProcessor::getColor(const Composite& composite, ColorRGBA& color_to_be_set)
		{
			const Residue* residue = dynamic_cast<const Residue*>(&composite);
			if (residue == 0)
			{
				residue = composite.getAncestor(dummy_residue_);
				if (residue == 0)
				{
					color_to_be_set.set(default_color_);
					return;
				}
			}
				
			HashMap<const Residue*, Position>::Iterator it = residue_map_.find(residue);
			if (it == residue_map_.end())
			{
				color_to_be_set.set(default_color_);
				return;
			}

			color_to_be_set.set(table_.map((*it).second));
		}

		bool ResidueNumberColorProcessor::start()
		{
			ColorProcessor::start();
			residue_map_.clear();
			table_.clear();
			table_ = ColorMap(500);
			ColorRGBA base_colors[3];
			base_colors[0] = first_color_;
			base_colors[1] = middle_color_;
			base_colors[2] = last_color_;
			table_.setBaseColors(base_colors, 3);

			if (composites_ == 0) return false;

			list<const Composite*>::const_iterator it = composites_->begin();
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
				else if (RTTI::isKindOf<Atom>(**it))
				{
					const Residue* residue = dynamic_cast<const Residue*>((**it).getParent());
					if (residue == 0) continue;

					residue_map_[residue] = residue_map_.size();
					continue;
				}
				else
				{
					const Residue* residue = dynamic_cast<const Residue*>((*it));
					if (residue == 0) continue;

					residue_map_[residue] = residue_map_.size();
					continue;
				}


				for (; +res_it; ++res_it)
				{
					if ((*res_it).getName() == "HOH") continue;

					residue_map_[&*res_it] = residue_map_.size();
				}
			}

			if (residue_map_.size() == 0) return true;

			table_.setRange(0, residue_map_.size() - 1);
			table_.createMap();

			for (Position p = 0; p < table_.size(); p++)
			{
				table_[p].setAlpha(255 - transparency_);
			}

			return true;
		}

		////////////////////////////////////////////////////////////////////
		AtomChargeColorProcessor::AtomChargeColorProcessor()
			:	InterpolateColorProcessor()
		{
			mode_ = NO_OUTSIDE_COLORS;

			colors_.resize(3);

			min_value_ = -1.0;
			max_value_ =  1.0;

			colors_[0] = "FF0000FF";
			colors_[1] = "FFFFFFFF";
			colors_[2] = "0000FFFF";

			update_always_needed_ = true;
		}


		AtomChargeColorProcessor::AtomChargeColorProcessor(const AtomChargeColorProcessor& color_processor)
			: InterpolateColorProcessor(color_processor)
		{
		}

		void AtomChargeColorProcessor::getColor(const Composite& composite, ColorRGBA& color_to_be_set)
		{
			const Atom* atom = dynamic_cast<const Atom*>(&composite);

			if (atom == 0)
			{
				color_to_be_set.set(default_color_);
				return;
			}

			interpolateColor(atom->getCharge(), color_to_be_set);
		}


		////////////////////////////////////////////////////////////////////
		AtomDistanceColorProcessor::AtomDistanceColorProcessor()
			: ColorProcessor(),
				atom_2_distance_(),
				distance_((float)10),
				show_selection_(false),
				null_distance_color_("FFFF00FF"),
				full_distance_color_("0000FFFF")
		{
			update_always_needed_ = true;
		}

		AtomDistanceColorProcessor::AtomDistanceColorProcessor(const AtomDistanceColorProcessor& color_processor)
			:	ColorProcessor(color_processor),
				atom_2_distance_(),
				distance_(color_processor.distance_),
				show_selection_(color_processor.show_selection_),
				null_distance_color_(color_processor.null_distance_color_),
				full_distance_color_(color_processor.full_distance_color_)
		{
		}

		void AtomDistanceColorProcessor::calculateDistances()
		{
			AtomDistanceHashMap::Iterator it1 = atom_2_distance_.begin();
			AtomDistanceHashMap::Iterator it1_old;
			Molecule dummy;

			// brute force
			for(; it1 != atom_2_distance_.end();)
			{
				const Atom* const atom1 = dynamic_cast<const Atom*>(it1->first);

				it1_old = it1;

				AtomDistanceHashMap::Iterator it2 = ++it1;
				
				for(; it2 != atom_2_distance_.end(); ++it2)
				{
					const Atom* const atom2 = dynamic_cast<const Atom*>(it2->first);

					if (atom1->isSelected() != atom2->isSelected())
					{
						const float distance = (atom2->getPosition() - atom1->getPosition()).getSquareLength();
						
						if (it1_old->second > distance) it1_old->second = distance;
						if (it2->second 		> distance) 	  it2->second = distance;
					}
				}
			}
		}


		void AtomDistanceColorProcessor::addAtom(const Atom& atom)
		{
			AtomDistanceHashMap::Iterator it = atom_2_distance_.find(&atom);

			// atom not in hashmap ? => insert into hashmap with start distance = distance_
			if (it == atom_2_distance_.end())
			{
				atom_2_distance_.insert(AtomDistanceHashMap::ValueType(&atom, distance_ * distance_));
			}		
		}

		void AtomDistanceColorProcessor::getColor(const Composite& composite, ColorRGBA& color_to_be_set)
		{
			const Atom* const atom = dynamic_cast<const Atom*>(&composite);
			if (atom == 0)
			{
				color_to_be_set.set(default_color_);
				return;
			}

			// here we have to consider selection color, unlike as for the other coloring processors
			if (atom->isSelected() && show_selection_)
			{
				color_to_be_set.set(selection_color_);
				return;
			}

			const AtomDistanceHashMap::Iterator it = atom_2_distance_.find(atom);
			float distance = distance_;

			// atom in hashmap ?
			if (it != atom_2_distance_.end())
			{
				// get distance
				distance = sqrt(it->second);
			}

			// clip the distance to  0 -> distance_
			if (distance > distance_) distance = distance_;
			if (distance < 0.0)
			{
				distance = 0.0;
			}

			const float red1   = null_distance_color_.getRed();
			const float green1 = null_distance_color_.getGreen();
			const float blue1  = null_distance_color_.getBlue();

			const float red2   = full_distance_color_.getRed();
			const float green2 = full_distance_color_.getGreen();
			const float blue2  = full_distance_color_.getBlue();

			color_to_be_set.set(red1 + (distance * (red2 - red1)) 			/ distance_,
													 green1 + (distance * (green2 - green1)) 	/ distance_,
													 blue1 + (distance * (blue2 - blue1)) 		/ distance_,
													 255 - transparency_);
		}

		void AtomDistanceColorProcessor::colorGeometricObject_(GeometricObject& object)
		{
			const Composite* composite = object.getComposite();

			Mesh* const mesh = dynamic_cast<Mesh*>(&object);
			if (mesh != 0)
			{
				mesh->colors.clear();
				if (composite == &composite_to_be_ignored_for_colorprocessors_ || composites_ == 0)
				{
					mesh->colors.push_back(default_color_);
					return;
				}

				if (composite == 0 || composite != last_composite_of_grid_)
				{
					createAtomGrid(composite);
				}

				colorMeshFromGrid_(*mesh);
				return;
			}

			ColorExtension2* const two_colored = dynamic_cast<ColorExtension2*>(&object);

			if (composite == 0 ||
					composite == &composite_to_be_ignored_for_colorprocessors_)
			{
				object.setColor(default_color_); 
				if (two_colored != 0)
				{
					two_colored->setColor2(default_color_);
				}
				return;
			}

			if (two_colored == 0)
			{
				if (show_selection_ && composite->isSelected())
				{
					object.setColor(selection_color_);
				}
				else
				{
					getColor(*composite, object.getColor()); 
				}
				return;
			}

			// ok, we have a two colored object
			const Bond* const bond = dynamic_cast<const Bond*>(composite);
			if (bond != 0)
			{
				const Atom* atom = bond->getFirstAtom();
				if (!atom->isSelected() ||
						!show_selection_)
				{
					getColor(*atom, object.getColor());
				}
				else
				{
					object.setColor(selection_color_);
				}

				const Atom* atom2 = bond->getSecondAtom();
				if (!atom2->isSelected() ||
						!show_selection_)
				{
					getColor(*atom2, two_colored->getColor2());
				}
				else
				{
					two_colored->setColor2(selection_color_);
				}
			}
			else
			{
				if (composite->isSelected() && 
						show_selection_)
				{
					object.setColor(selection_color_);
					two_colored->setColor2(selection_color_);
				}
				else
				{
 					getColor(*composite, object.getColor());
 					two_colored->setColor2(object.getColor());
				}
			}
		}

		bool AtomDistanceColorProcessor::finish()
		{
			calculateDistances();
			GeometricObjectList::iterator it = list_.begin();
			for(; it != list_.end(); it++)
			{
				colorGeometricObject_(**it);
			}

			atom_2_distance_.clear();
			list_.clear();
			
			return true;
		}

		Processor::Result AtomDistanceColorProcessor::operator() (GeometricObject*& object)
		{
			if (RTTI::isKindOf<Mesh>(*object))
			{
				if (last_composite_of_grid_ == 0)
				{ 
					createAtomGrid();
				}
				list<const Composite*>::const_iterator it = composites_->begin();
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
						addAtom(*dynamic_cast<const Atom*> (*it));
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
			
		void AtomDistanceColorProcessor::colorMeshFromGrid_(Mesh& mesh)
		{
			if (atom_grid_.isEmpty()) return;
			
			mesh.colors.resize(mesh.vertex.size());
			
			for (Position p = 0; p < mesh.vertex.size(); p++)
			{
				// make sure we found an atom
				const Atom* atom = getClosestItem(mesh.vertex[p]);

				if (atom == 0)
				{
 					mesh.colors[p] = default_color_;
				}
				else
				{
					if (show_selection_ && atom->isSelected())
					{
						mesh.colors[p] = selection_color_;
					}
					else
					{
 						getColor(*atom, mesh.colors[p]);
					}
				}
			}
		}

		////////////////////////////////////////////////////////////////////
		TemperatureFactorColorProcessor::TemperatureFactorColorProcessor()
			: InterpolateColorProcessor()
		{
			mode_ = DEFAULT_COLOR_FOR_OUTSIDE_COLORS;

			colors_.resize(2);
			default_color_ = ColorRGBA(1.0,1.0,1.0);
			colors_[0].set(0,0,1.0),
			colors_[1].set(1.0,1.0,0),
			min_value_ = 0.0001;
			max_value_ = 50;
		}

		void TemperatureFactorColorProcessor::getColor(const Composite& composite, ColorRGBA& color_to_be_set)
		{
			const PDBAtom* const atom = dynamic_cast<const PDBAtom*>(&composite);
			if (atom == 0)
			{
				color_to_be_set.set(default_color_);
				return;
			}

			interpolateColor(atom->getTemperatureFactor(), color_to_be_set);
		}

		////////////////////////////////////////////////////////////////////
		OccupancyColorProcessor::OccupancyColorProcessor()
			: InterpolateColorProcessor()
		{
			mode_ = DEFAULT_COLOR_FOR_OUTSIDE_COLORS;

			colors_.resize(2);

			default_color_ = ColorRGBA(1.0, 1.0, 1.0);
			colors_[0].set(0, 0, 1.0),
			colors_[1].set(1.0,1.0,0),
			min_value_ = 0.0;
			max_value_ = 1.0;
		}

		void OccupancyColorProcessor::getColor(const Composite& composite, ColorRGBA& color_to_be_set)
		{
			const PDBAtom* atom = dynamic_cast<const PDBAtom*>(&composite);
			if (atom == 0)			
			{
				color_to_be_set.set(default_color_);
			}
			else
			{
				interpolateColor(atom->getOccupancy(), color_to_be_set);
			}
		}
		
		////////////////////////////////////////////////////////////////////
		ForceColorProcessor::ForceColorProcessor()
			: InterpolateColorProcessor()
		{
			mode_ = NO_OUTSIDE_COLORS;

			colors_.resize(2);

			default_color_ = ColorRGBA(1.0, 1.0, 1.0);

			colors_[0].set(0, 0, 1.0),
			colors_[1].set(1.0, 0, 0),
			min_value_ = 0;
			max_value_ = 10;

			update_always_needed_ = true;
		}

		void ForceColorProcessor::getColor(const Composite& composite, ColorRGBA& color_to_be_set)
		{
			const Atom* atom = dynamic_cast<const Atom*>(&composite);
			if (atom == 0)			
			{
				color_to_be_set.set(default_color_);
				return;
			}

			Vector3 force = atom->getForce();
			if (force.getSquareLength() == 0) 
			{
				color_to_be_set.set(min_color_);
				return;
			}

			force *= pow((float)10.0, (float)12.0);

			interpolateColor(log(force.getLength()), color_to_be_set);
		}

		////////////////////////////////////////////////////////////////////
		SecondaryStructureColorProcessor::SecondaryStructureColorProcessor()
			: ColorProcessor(),
			  helix_color_(0,0,255),
				coil_color_(0,155,155),
				strand_color_(255,0,0),
				turn_color_(255,255,0),
				dummy_ss_()
		{
		}

		bool SecondaryStructureColorProcessor::canUseMeshShortcut_(const Composite& composite)
		{
			return RTTI::isKindOf<SecondaryStructure>(composite) ||
				     composite.getAncestor(dummy_ss_) != 0;
		}

		void SecondaryStructureColorProcessor::getColor(const Composite& composite, ColorRGBA& color_to_be_set)
		{
			const SecondaryStructure* ss = dynamic_cast<const SecondaryStructure*>(&composite);
			if (ss == 0)
			{
				ss = dynamic_cast<const SecondaryStructure*>(composite.getAncestor(dummy_ss_));
				if (ss == 0)
				{
					color_to_be_set.set(default_color_);
					return;
				}
			}

			const SecondaryStructure::Type type = ss->getType();
			if (type == SecondaryStructure::HELIX)
			{
				color_to_be_set.set(helix_color_);
			}
			else if (type == SecondaryStructure::COIL)
			{
				color_to_be_set.set(coil_color_);
			}
			else if (type == SecondaryStructure::STRAND)
			{
				color_to_be_set.set(strand_color_);
			}
			else if (type == SecondaryStructure::TURN)
			{
				color_to_be_set.set(turn_color_);
			}
		}

		void SecondaryStructureColorProcessor::setTransparency(Size t)
		{
			ColorProcessor::setTransparency(t);
			helix_color_.setAlpha(255 - t);
			coil_color_.setAlpha(255 - t);
			strand_color_.setAlpha(255 - t);
			turn_color_.setAlpha(255 - t);
		}

		void SecondaryStructureColorProcessor::setHelixColor(const ColorRGBA& color)
		{
			helix_color_ = color;
			helix_color_.setAlpha(255 - transparency_);
		}

		void SecondaryStructureColorProcessor::setCoilColor(const ColorRGBA& color)
		{
			coil_color_ = color;
			coil_color_.setAlpha(255 - transparency_);
		}

		void SecondaryStructureColorProcessor::setStrandColor(const ColorRGBA& color)
		{
			strand_color_ = color;
			strand_color_.setAlpha(255 - transparency_);
		}

		void SecondaryStructureColorProcessor::setTurnColor(const ColorRGBA& color)
		{
			turn_color_ = color;
			turn_color_.setAlpha(255 - transparency_);
		}

		const ColorRGBA& SecondaryStructureColorProcessor::getHelixColor() const
		{
			return helix_color_;
		}

		const ColorRGBA& SecondaryStructureColorProcessor::getCoilColor() const
		{
			return coil_color_;
		}

		const ColorRGBA& SecondaryStructureColorProcessor::getStrandColor() const
		{
			return strand_color_;
		}

		const ColorRGBA& SecondaryStructureColorProcessor::getTurnColor() const
		{
			return turn_color_;
		}

		////////////////////////////////////////////////////////////////////
		
		ResidueTypeColorProcessor::ResidueTypeColorProcessor()
			: ColorProcessor(),
				basic_color_(ColorRGBA(255,255,0)),
				acidic_color_(ColorRGBA(0,0,255)),
				polar_color_(ColorRGBA(255,0,255)),
				hydrophobic_color_(ColorRGBA(0,255,0)),
				aromatic_color_(ColorRGBA(255,0,0)),
				other_color_(ColorRGBA(125,125,125)),
				dummy_residue_()
		{
		}

		bool ResidueTypeColorProcessor::canUseMeshShortcut_(const Composite& composite)
		{
			return RTTI::isKindOf<Residue>(composite);
		}

		void ResidueTypeColorProcessor::getColor(const Composite& composite, ColorRGBA& color_to_be_set)
		{
			const Residue* residue = dynamic_cast<const Residue*>(&composite);
			if (residue == 0)
			{
				residue = dynamic_cast<const Residue*>(composite.getAncestor(dummy_residue_));
				if (residue == 0)
				{
					color_to_be_set.set(default_color_);
					return;
				}
			}

			const String name = residue->getName();
			if (name == "LYS" || 
					name == "ARG" || 
					name == "HIS") 
			{
				color_to_be_set.set(basic_color_);
				return;
			}
			
			if (name == "PHE" || 
					name == "TYR" || 
					name == "TRP") 
			{
				color_to_be_set.set(aromatic_color_);
				return;
			}
			
			if (name == "VAL" || 
					name == "LEU" || 
					name == "MET" || 
					name == "ILE")
			{
				color_to_be_set.set(hydrophobic_color_);
				return;
			}
			
			if (name == "ASP" || 
					name == "GLU" || 
					name == "GLN" || 
					name == "ASN")
			{
				color_to_be_set.set(acidic_color_);
				return;
			}

			if (name == "ALA" || 
					name == "GLY" || 
					name == "SER" ||
					name == "THR" || 
					name == "PRO")
			{
				color_to_be_set.set(polar_color_);
				return;
			}

			color_to_be_set.set(other_color_);
		}

		void ResidueTypeColorProcessor::setBasicColor(const ColorRGBA& color)
		{
			basic_color_ = color;
			basic_color_.setAlpha(255 - transparency_);
		}

		void ResidueTypeColorProcessor::setAcidicColor(const ColorRGBA& color)
		{
			acidic_color_ = color;
			acidic_color_.setAlpha(255 - transparency_);
		}

		void ResidueTypeColorProcessor::setPolarColor(const ColorRGBA& color)
		{
			polar_color_ = color;
			polar_color_.setAlpha(255 - transparency_);
		}
		
		void ResidueTypeColorProcessor::setHydrophobicColor(const ColorRGBA& color)
		{
			hydrophobic_color_ = color;
			hydrophobic_color_.setAlpha(255 - transparency_);
		}

		void ResidueTypeColorProcessor::setAromaticColor(const ColorRGBA& color)
		{
			aromatic_color_ = color;
			aromatic_color_.setAlpha(255 - transparency_);
		}

		void ResidueTypeColorProcessor::setOtherColor(const ColorRGBA& color)
		{
			other_color_ = color;
			other_color_.setAlpha(255 - transparency_);
		}

		const ColorRGBA& ResidueTypeColorProcessor::getBasicColor() const
		{
			return basic_color_;
		}

		const ColorRGBA& ResidueTypeColorProcessor::getAcidicColor() const
		{
			return acidic_color_;
		}

		const ColorRGBA& ResidueTypeColorProcessor::getPolarColor() const
		{
			return polar_color_;
		}

		const ColorRGBA& ResidueTypeColorProcessor::getHydrophobicColor() const
		{
			return hydrophobic_color_;
		}

		const ColorRGBA& ResidueTypeColorProcessor::getAromaticColor() const
		{
			return aromatic_color_;
		}

		const ColorRGBA& ResidueTypeColorProcessor::getOtherColor() const
		{
			return other_color_;
		}

		void ResidueTypeColorProcessor::setTransparency(Size t)
		{
			basic_color_.setAlpha(255 - t);
			acidic_color_.setAlpha(255 - t);
			polar_color_.setAlpha(255 - t);
			hydrophobic_color_.setAlpha(255 - t);
			aromatic_color_.setAlpha(255 - t);
			other_color_.setAlpha(255 - t);
		}

		////////////////////////////////////////////////////////////////////
		PositionColorProcessor::PositionColorProcessor()
			: ColorProcessor()
		{
			colors_.resize(20);
			colors_[ 0].set(1.0, 0.0, 0.0);
			colors_[ 1].set(0.0, 1.0, 0.0);
			colors_[ 2].set(0.0, 0.0, 1.0);
			colors_[ 3].set(1.0, 1.0, 0.0);
			colors_[ 4].set(0.0, 1.0, 1.0);
			colors_[ 5].set(1.0, 0.0, 1.0);
			colors_[ 6].set(0.5, 0.5, 0.5);
			colors_[ 7].set(1.0, 0.5, 0.5);
			colors_[ 8].set(1.0, 1.0, 1.0);
			colors_[ 9].set(0.5, 0.5, 0.0);
			colors_[10].set(1.0, 0.2, 0.2);
			colors_[11].set(0.9, 0.1, 0.9);
			colors_[12].set(0.0, 0.9, 0.0);
			colors_[13].set(0.9, 0.0, 0.2);
			colors_[14].set(1.0, 1.0, 0.5);
			colors_[15].set(0.5, 1.0, 1.0);
			colors_[16].set(1.0, 0.5, 1.0);
			colors_[17].set(0.7, 0.2, 0.7);
			colors_[18].set(0.2, 0.7, 0.7);
			colors_[19].set(0.7, 0.7, 0.2);
		}
		
		void PositionColorProcessor::getColor(const Composite& composite, ColorRGBA& color_to_be_set)
		{
			const Composite* c = getAncestor_(composite);
			if (c == 0)
			{
				color_to_be_set.set(default_color_);
				return;
			}

			HashMap<const Composite*, Position>::Iterator it = composite_to_position_.find(c);
			if (it != composite_to_position_.end())
			{
				color_to_be_set.set(colors_[it->second]);
				return;
			}

 			const Composite* parent = c->getParent();
			if (parent == 0) 
			{
				composite_to_position_[c] = 0;
				color_to_be_set.set(colors_[0]);
				return;
			}

		 	const Composite* child = parent->getFirstChild();
		 	Position pos = 0;
		 	while (child != 0)
		 	{
				if (isOK_(*child))
				{
				 	composite_to_position_[child] = pos;
			 	}

			 	child = child->getSibling(1);
				pos++;
				if (pos >= colors_.size() - 1) pos -= (colors_.size() - 1);
		 	}

			color_to_be_set.set(colors_[composite_to_position_[c]]);
		}

		bool PositionColorProcessor::start() 
		{
			if (!ColorProcessor::start()) return false;

			if (colors_.size() < 2) return false;

			for (Position p = 0; p < colors_.size(); p++)
			{
				colors_[p].setAlpha(255 - transparency_);
			}

			return true;
		}
		
		////////////////////////////////////////////////////////////////////
		
		ChainColorProcessor::ChainColorProcessor()
			: PositionColorProcessor()
		{
		}

		bool ChainColorProcessor::canUseMeshShortcut_(const Composite& composite)
		{
			return RTTI::isKindOf<Chain>(composite) ||
						 composite.getAncestor(dummy_chain_) != 0;
		}


		////////////////////////////////////////////////////////////////////
		
		MoleculeColorProcessor::MoleculeColorProcessor()
			: PositionColorProcessor()
		{
		}

		bool MoleculeColorProcessor::canUseMeshShortcut_(const Composite& composite)
		{
			return RTTI::isKindOf<Molecule>(composite) ||
						 composite.getAncestor(dummy_molecule_) != 0;
		}


#	ifdef BALL_NO_INLINE_FUNCTIONS
#		include <BALL/VIEW/MODELS/standardColorProcessor.iC>
#	endif

	} // namespace VIEW
} // namespace BALL

