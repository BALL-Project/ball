// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: coloringSettingsDialog.C,v 1.38.16.3 2007/05/30 21:23:14 amoll Exp $
//

#include <BALL/VIEW/DIALOGS/coloringSettingsDialog.h>
#include <BALL/VIEW/MODELS/standardColorProcessor.h>
#include <BALL/KERNEL/PTE.h>

#include <QtGui/qcolordialog.h>
#include <QtGui/qslider.h>
#include <QtGui/qlabel.h>
#include <QtGui/QStackedWidget>
#include <QtGui/qcheckbox.h>

namespace BALL
{
	namespace VIEW
	{
		ColoringSettingsDialog::ColoringSettingsDialog( QWidget* parent,  const char* name, Qt::WFlags fl )
			: QWidget(parent, fl),
				Ui_ColoringSettingsDialogData(),
				PreferencesEntry()
		{
			setupUi(this);

			element_table_ ->setObjectName("Elements");
			residue_table_ ->setObjectName("Residues");
			chain_table_   ->setObjectName("Chains");
			molecule_table_->setObjectName("Molecules");

			setObjectName(name);
			setINIFileSectionName("COLORING_OPTIONS");
			setDefaultValues_();
			setWidgetStackName("Model Colors");
			setWidgetStack(widget_stack);

			// signals and slots connections
			connect( coil_color_button, SIGNAL( clicked() ), this, SLOT( coilColorPressed() ) );
			connect( first_residue_button, SIGNAL( clicked() ), this, SLOT( firstResidueColorPressed() ) );
			connect( force_max_color_button, SIGNAL( clicked() ), this, SLOT( forceMaxColorPressed() ) );
			connect( force_max_value_slider, SIGNAL( valueChanged(int) ), this, SLOT( forceMaxValueChanged() ) );
			connect( force_min_color_button, SIGNAL( clicked() ), this, SLOT( forceMinColorPressed() ) );
			connect( force_min_value_slider, SIGNAL( valueChanged(int) ), this, SLOT( forceMinValueChanged() ) );
			connect( helix_color_button, SIGNAL( clicked() ), this, SLOT( helixColorPressed() ) );
			connect( max_distance_button, SIGNAL( clicked() ), this, SLOT( maxDistanceColorPressed() ) );
			connect( max_distance_slider, SIGNAL( valueChanged(int) ), this, SLOT( maxDistanceChanged() ) );
			connect( max_tf_slider, SIGNAL( valueChanged(int) ), this, SLOT( maxTFChanged() ) );
			connect( maximum_o_button, SIGNAL( clicked() ), this, SLOT( maximumOccupancyColorPressed() ) );
			connect( maximum_tf_button, SIGNAL( clicked() ), this, SLOT( maximumTFColorPressed() ) );
			connect( middle_residue_button, SIGNAL( clicked() ), this, SLOT( middleResidueColorPressed() ) );
			connect( minimum_o_button, SIGNAL( clicked() ), this, SLOT( minimumOccupancyColorPressed() ) );
			connect( minimum_tf_button, SIGNAL( clicked() ), this, SLOT( minimumTFColorPressed() ) );
			connect( negative_charge_color_button, SIGNAL( clicked() ), this, SLOT( negativeChargeColorPressed() ) );
			connect( neutral_charge_color_button, SIGNAL( clicked() ), this, SLOT( neutralChargeColorPressed() ) );
			connect( null_distance_button, SIGNAL( clicked() ), this, SLOT( nullDistanceColorPressed() ) );
			connect( positive_charge_color_button, SIGNAL( clicked() ), this, SLOT( positiveChargeColorPressed() ) );
			connect( strand_color_button, SIGNAL( clicked() ), this, SLOT( strandColorPressed() ) );
			connect( turn_color_button, SIGNAL( clicked() ), this, SLOT( turnColorPressed() ) );
			connect( unassigned_o_button, SIGNAL( clicked() ), this, SLOT( unassignedOccupancyColorPressed() ) );
			connect( unassigned_tf_button, SIGNAL( clicked() ), this, SLOT( unassignedTFColorPressed() ) );
			connect( last_residue_button, SIGNAL( clicked() ), this, SLOT( lastResidueColorPressed() ) );
			connect( acidic_color_button, SIGNAL( clicked() ), this, SLOT( acidicColorPressed() ) );
			connect( basic_color_button, SIGNAL( clicked() ), this, SLOT( basicColorPressed() ) );
			connect( polar_color_button, SIGNAL( clicked() ), this, SLOT( polarColorPressed() ) );
			connect( hydrophobic_color_button, SIGNAL( clicked() ), this, SLOT( hydrophobicColorPressed() ) );
			connect( aromatic_color_button, SIGNAL( clicked() ), this, SLOT( aromaticColorPressed() ) );
			connect( other_color_button, SIGNAL( clicked() ), this, SLOT( otherColorPressed() ) );

			registerWidgets_();
		}

		void ColoringSettingsDialog::setDefaultValues_()
		{
			vector<String> 		names;
			vector<ColorRGBA> colors;

			// =============================================================
			// setting element colors
			{
				// create a dummy processor to get the default values
				ElementColorProcessor elp;
				const HashMap<Position, ColorRGBA>& color_hash_map = elp.getColorMap();
				HashMap<Position, ColorRGBA>::ConstIterator it = color_hash_map.begin();

				for(; it != color_hash_map.end(); it++)
				{
					if (it->first == 0) continue;
					names.push_back(PTE[it->first].getSymbol());
					colors.push_back(it->second);
				}

				names.push_back(PTE[0].getSymbol());
				colors.push_back(color_hash_map[0]);
				element_table_->setNamesTitle("Element");
				element_table_->setContent(names, colors);
				names.clear();
				colors.clear();
			}

			// =============================================================
			// setting residue name colors
			// create a dummy processor to get the default values
			{
				ResidueNameColorProcessor rcp;
				const StringHashMap<ColorRGBA>& color_map = rcp.getColorMap();
				StringHashMap<ColorRGBA>::ConstIterator it2 = color_map.begin();

				for(; it2 != color_map.end(); it2++)
				{
					names.push_back(it2->first);
					colors.push_back(it2->second);
				}

				residue_table_->setNamesTitle("Residue");
				residue_table_->setContent(names, colors);
			}
				
			// =============================================================
			getSettings(ResidueNumberColorProcessor());
			getSettings(AtomChargeColorProcessor());
			getSettings(AtomDistanceColorProcessor());
			getSettings(TemperatureFactorColorProcessor());
			getSettings(OccupancyColorProcessor());
			getSettings(SecondaryStructureColorProcessor());
			getSettings(ForceColorProcessor());
			getSettings(ResidueTypeColorProcessor());
			getSettings(ChainColorProcessor());
			getSettings(MoleculeColorProcessor());
		}

		vector<ColorRGBA> ColoringSettingsDialog::getColors(ColoringMethod method) const
		{
			vector<ColorRGBA> colors;
			ColorTable* table = 0;
			switch (method)
			{
				case COLORING_ELEMENT:
				{
					table = element_table_; 
					if (table->rowCount() > 0)
					{
						colors.push_back(table->item(table->rowCount() - 1, 1)->backgroundColor());
					}
					break;
				}
				case COLORING_RESIDUE_INDEX: 	table = residue_table_; break;
				case COLORING_CHAIN: 					table = chain_table_  ; break;
				case COLORING_MOLECULE: 			table = molecule_table_; break;
				default: return colors;
			}
					
			for (Position p = 0; p < (Position)table->rowCount(); p++)
			{
				colors.push_back(table->item(p, 1)->backgroundColor());
			}

			return colors;
		}

		void ColoringSettingsDialog::applySettingsTo(ColorProcessor& cp) const
		{
			if (RTTI::isKindOf<CustomColorProcessor>(cp)) return;

			if (RTTI::isKindOf<ElementColorProcessor>(cp))
			{
				vector<ColorRGBA> colors = getColors(COLORING_ELEMENT);
				for (Position p = 0; p < colors.size(); p++)
				{
					(*(ElementColorProcessor*)&cp).getColorMap()[p] = colors[p];
				}
				return;
			}
			
			if (RTTI::isKindOf<ResidueNameColorProcessor>(cp))
			{
				for (Position p = 0; p < (Position)residue_table_->rowCount(); p++)
				{
					(*(ResidueNameColorProcessor*)&cp).getColorMap()[ascii(residue_table_->item(p,0)->text())] = 
						residue_table_->item(p,1)->backgroundColor();
				}
				return;
			}

			if (RTTI::isKindOf<ResidueNumberColorProcessor>(cp))
			{
				ResidueNumberColorProcessor& dp = (*(ResidueNumberColorProcessor*)&cp);
				dp.setFirstColor(getColor(first_residue_label));
				dp.setMiddleColor(getColor(middle_residue_label));
				dp.setLastColor(getColor(last_residue_label));
				return;
			}

			if (RTTI::isKindOf<AtomChargeColorProcessor>(cp))
			{
				AtomChargeColorProcessor& dp = (*(AtomChargeColorProcessor*)&cp);
				dp.getColors()[0] = (getColor(negative_charge_label));
				dp.getColors()[1] = (getColor(neutral_charge_label));
				dp.getColors()[2] = (getColor(positive_charge_label));
				return;
			}

			if (RTTI::isKindOf<AtomDistanceColorProcessor>(cp))
			{
				AtomDistanceColorProcessor& dp = (*(AtomDistanceColorProcessor*)&cp);
				dp.setNullDistanceColor(getColor(null_distance_label));
				dp.setMaxDistanceColor(getColor(max_distance_label));
				dp.setDistance(((float)max_distance_slider->value()) / 10.0);
 				dp.setShowSelected(distance_show_selected->isChecked());
				return;
			}

			if (RTTI::isKindOf<OccupancyColorProcessor>(cp))
			{
				OccupancyColorProcessor& dp = (*(OccupancyColorProcessor*)&cp);
				dp.getColors()[0] = (getColor(minimum_o_label));
				dp.getColors()[1] = (getColor(maximum_o_label));
				return;
			}

			if (RTTI::isKindOf<SecondaryStructureColorProcessor>(cp))
			{
				SecondaryStructureColorProcessor& dp = (*(SecondaryStructureColorProcessor*)&cp);

				dp.setHelixColor(getColor(helix_color_label));
				dp.setCoilColor(getColor(coil_color_label));
				dp.setStrandColor(getColor(strand_color_label));
				dp.setTurnColor(getColor(turn_color_label));

				return;
			}

			if (RTTI::isKindOf<TemperatureFactorColorProcessor>(cp))
			{
				TemperatureFactorColorProcessor& dp = (*(TemperatureFactorColorProcessor*)&cp);
				dp.setMinColor(getColor(unassigned_tf_label));
				dp.getColors()[0] = (getColor(minimum_tf_label));
				dp.getColors()[1] = (getColor(maximum_tf_label));
				dp.setMaxColor(getColor(unassigned_tf_label));
				dp.setMaxValue(((float)max_tf_slider->value()) / 10.0);
				return;
			}

			if (RTTI::isKindOf<ForceColorProcessor>(cp))
			{
				ForceColorProcessor& dp = (*(ForceColorProcessor*)&cp);
				dp.getColors()[0] = (getColor(force_min_color_label));
				dp.getColors()[1] = (getColor(force_max_color_label));
				dp.setMaxValue(((float)force_max_value_slider->value()) / 10.0);
				dp.setMinValue(((float)force_min_value_slider->value()) / 10.0);
				return;
			}

			if (RTTI::isKindOf<ResidueTypeColorProcessor>(cp))
			{
				ResidueTypeColorProcessor& dp = (*(ResidueTypeColorProcessor*)&cp);
				dp.setBasicColor(getColor(basic_color_label));
				dp.setAcidicColor(getColor(acidic_color_label));
				dp.setAromaticColor(getColor(aromatic_color_label));
				dp.setPolarColor(getColor(polar_color_label));
				dp.setHydrophobicColor(getColor(hydrophobic_color_label));
				dp.setOtherColor(getColor(other_color_label));
				return;
			}

			if (RTTI::isKindOf<ChainColorProcessor>(cp))
			{
				((ChainColorProcessor*)&cp)->setColors(getColors(COLORING_CHAIN));
				return;
			}

			if (RTTI::isKindOf<MoleculeColorProcessor>(cp))
			{
				((MoleculeColorProcessor*)&cp)->setColors(getColors(COLORING_MOLECULE));
				return;
			}

		}
			
		void ColoringSettingsDialog::maxDistanceChanged()
		{
			String text = String(((float)max_distance_slider->value()) / 10.0);
			text = text.trimRight("0");
			if (text.hasSuffix(".")) text += "0";
			max_distance_value_label->setText(text.c_str());
		}
			
		void ColoringSettingsDialog::maxTFChanged()
		{
			String text = String(((float)max_tf_slider->value()) / 10.0);
			text = text.trimRight("0");
			if (text.hasSuffix(".")) text += "0";
			max_tf_label->setText(text.c_str());
		}

		void ColoringSettingsDialog::forceMaxValueChanged()
		{
			String text = String(((float)force_max_value_slider->value()) / 10.0);
			text = text.trimRight("0");
			if (text.hasSuffix(".")) text += "0";
			force_max_value_label->setText(text.c_str());
		}

		void ColoringSettingsDialog::forceMinValueChanged()
		{
			String text = String(((float)force_min_value_slider->value()) / 10.0);
			text = text.trimRight("0");
			if (text.hasSuffix(".")) text += "0";
			force_min_value_label->setText(text.c_str());
		}


		ColorProcessor* ColoringSettingsDialog::createColorProcessor(ColoringMethod method) const
			throw(Exception::InvalidOption)
		{
			ColorProcessor* color_processor = 0;

			switch(method)
			{
				case COLORING_ELEMENT:
					color_processor = new ElementColorProcessor;
					break;

				case COLORING_RESIDUE_NAME:
					color_processor = new ResidueNameColorProcessor;
					break;

				case COLORING_RESIDUE_INDEX:
					color_processor = new ResidueNumberColorProcessor;
					break;

				case COLORING_SECONDARY_STRUCTURE:
					color_processor = new SecondaryStructureColorProcessor;
					break;

				case COLORING_ATOM_CHARGE:
					color_processor = new AtomChargeColorProcessor;
					break;

				case COLORING_CUSTOM:
					color_processor = new CustomColorProcessor;
					break;

				case COLORING_DISTANCE:
					color_processor = new AtomDistanceColorProcessor;
					break;

				case COLORING_TEMPERATURE_FACTOR:
					color_processor = new TemperatureFactorColorProcessor;
					break;

				case COLORING_OCCUPANCY:
					color_processor = new OccupancyColorProcessor;
					break;

				case COLORING_FORCES:
					color_processor = new ForceColorProcessor;
					break;

				case COLORING_RESIDUE_TYPE:
					color_processor = new ResidueTypeColorProcessor;
					break;

				case COLORING_CHAIN:
					color_processor = new ChainColorProcessor;
					break;

				case COLORING_MOLECULE:
					color_processor = new MoleculeColorProcessor;
					break;

				default:
					throw(Exception::InvalidOption(__FILE__, __LINE__, method));
			}

			applySettingsTo(*color_processor);

			return color_processor;
		}

		void ColoringSettingsDialog::getSettings(const ColorProcessor& cp)
		{

			if (RTTI::isKindOf<CustomColorProcessor>(cp))
			{
			} else

			if (RTTI::isKindOf<ElementColorProcessor>(cp))
			{
			} else
			
			if (RTTI::isKindOf<ResidueNameColorProcessor>(cp))
			{
			} else

			if (RTTI::isKindOf<ResidueNumberColorProcessor>(cp))
			{
				ResidueNumberColorProcessor& dp = (*(ResidueNumberColorProcessor*)&cp);
				setColor(first_residue_label, dp.getFirstColor());
				setColor(middle_residue_label, dp.getMiddleColor());
				setColor(last_residue_label, dp.getLastColor());
			} else

			if (RTTI::isKindOf<AtomChargeColorProcessor>(cp))
			{
				AtomChargeColorProcessor& dp = (*(AtomChargeColorProcessor*)&cp);
				setColor(negative_charge_label, dp.getColors()[0]);
				setColor(neutral_charge_label, dp.getColors()[1]);
				setColor(positive_charge_label, dp.getColors()[2]);
			} else

			if (RTTI::isKindOf<AtomDistanceColorProcessor>(cp))
			{
				AtomDistanceColorProcessor& dp = (*(AtomDistanceColorProcessor*)&cp);
				setColor(null_distance_label, dp.getNullDistanceColor());
				setColor(max_distance_label, dp.getMaxDistanceColor());
				max_distance_slider->setValue((Size)(dp.getDistance() * 10.0));
 				distance_show_selected->setChecked(dp.showSelected());
			} else

			if (RTTI::isKindOf<OccupancyColorProcessor>(cp))
			{
				OccupancyColorProcessor& dp = (*(OccupancyColorProcessor*)&cp);
				setColor(minimum_o_label, dp.getColors()[0]);
				setColor(maximum_o_label, dp.getColors()[1]);
			} else

			if (RTTI::isKindOf<SecondaryStructureColorProcessor>(cp))
			{
				SecondaryStructureColorProcessor& dp = (*(SecondaryStructureColorProcessor*)&cp);
				setColor(helix_color_label, dp.getHelixColor());
				setColor(coil_color_label, dp.getCoilColor());
				setColor(strand_color_label, dp.getStrandColor());
				setColor(turn_color_label, dp.getTurnColor());
			} else

			if (RTTI::isKindOf<TemperatureFactorColorProcessor>(cp))
			{
				TemperatureFactorColorProcessor& dp = (*(TemperatureFactorColorProcessor*)&cp);
				setColor(unassigned_tf_label, dp.getDefaultColor());
				setColor(minimum_tf_label, dp.getColors()[0]);
				setColor(maximum_tf_label, dp.getColors()[1]);
				max_tf_slider->setValue((Size)(dp.getMaxValue() * 10.0));
			} else

			if (RTTI::isKindOf<ForceColorProcessor>(cp))
			{
				ForceColorProcessor& dp = (*(ForceColorProcessor*)&cp);
				setColor(force_min_color_label, dp.getColors()[0]);
				setColor(force_max_color_label, dp.getColors()[1]);
				force_max_value_slider->setValue((Size)(dp.getMaxValue() * 10.0));
				force_min_value_slider->setValue((Size)(dp.getMinValue() * 10.0));
			} else

			if (RTTI::isKindOf<ResidueTypeColorProcessor>(cp))
			{
 				ResidueTypeColorProcessor& dp = (*(ResidueTypeColorProcessor*)&cp);
				setColor(acidic_color_label, dp.getAcidicColor());
				setColor(aromatic_color_label, dp.getAromaticColor());
				setColor(basic_color_label, dp.getBasicColor());
				setColor(hydrophobic_color_label, dp.getHydrophobicColor());
				setColor(other_color_label, dp.getOtherColor());
				setColor(polar_color_label, dp.getPolarColor());
			} else

			if (RTTI::isKindOf<ChainColorProcessor>(cp))
			{
 				ChainColorProcessor& dp = (*(ChainColorProcessor*)&cp);
				vector<String> 		names;
				vector<ColorRGBA> colors;

				for (Position p = 0; p < dp.getColors().size(); p++)
				{
					colors.push_back(dp.getColors()[p]);
					names.push_back(p);
				}

				chain_table_->setNamesTitle("Chain");
				chain_table_->setContent(names, colors);
			} else

			if (RTTI::isKindOf<MoleculeColorProcessor>(cp))
			{
 				MoleculeColorProcessor& dp = (*(MoleculeColorProcessor*)&cp);
				vector<String> 		names;
				vector<ColorRGBA> colors;

				for (Position p = 0; p < dp.getColors().size(); p++)
				{
					colors.push_back(dp.getColors()[p]);
					names.push_back(p);
				}

				molecule_table_->setNamesTitle("Molecule");
				molecule_table_->setContent(names, colors);
			}

		}

		QWidget* ColoringSettingsDialog::getEntryFor(ColoringMethod method)
		{
			switch (method)
			{
				case COLORING_ELEMENT: 							return widget_stack->widget(0);
				case COLORING_RESIDUE_NAME: 				return widget_stack->widget(2);
				case COLORING_RESIDUE_INDEX: 				return widget_stack->widget(1);
				case COLORING_SECONDARY_STRUCTURE: 	return widget_stack->widget(7);
				case COLORING_ATOM_CHARGE: 					return widget_stack->widget(3);
				case COLORING_DISTANCE: 						return widget_stack->widget(4);
				case COLORING_TEMPERATURE_FACTOR: 	return widget_stack->widget(5);
				case COLORING_OCCUPANCY: 						return widget_stack->widget(6);
				case COLORING_FORCES: 							return widget_stack->widget(8);
				case COLORING_RESIDUE_TYPE: 				return widget_stack->widget(9);
				case COLORING_CHAIN: 								return widget_stack->widget(10);
				case COLORING_MOLECULE: 						return widget_stack->widget(11);
				default: break;
			}

			return 0;
		}

  } // namespace VIEW
} // namespace BALL
