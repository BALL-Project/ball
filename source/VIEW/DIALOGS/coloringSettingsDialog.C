// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: coloringSettingsDialog.C,v 1.34.2.2 2005/01/13 01:18:24 amoll Exp $
//

#include <BALL/VIEW/DIALOGS/coloringSettingsDialog.h>
#include <BALL/VIEW/MODELS/standardColorProcessor.h>
#include <BALL/FORMAT/INIFile.h>
#include <BALL/KERNEL/PTE.h>

#include <qcolordialog.h>
#include <qslider.h>
#include <qlabel.h>
#include <qwidgetstack.h>
#include <qcheckbox.h>

namespace BALL
{
	namespace VIEW
	{

		QColorTableItem::QColorTableItem(QTable *t, EditType et, const ColorRGBA& color)
			: QTableItem(t,et,""),
				color_rgba_(color)
		{ 
		}

		void QColorTableItem::paint(QPainter *p, const QColorGroup &cg, const QRect &cr, bool selected)
		{
			QColorGroup g( cg );
			g.setColor( QColorGroup::Base, QColor (color_rgba_.getQColor()));
			QTableItem::paint( p, g, cr, selected );
		}

		// ==============================================================================================
		QColorTable::QColorTable(QWidget* parent) 
			throw()
			: QTable(parent),
				setting_content_(false)
		{
			setNumCols(2);
			horizontalHeader()->setLabel(1, "Color");
			setGeometry(5,5, 400, 388);
			setColumnWidth(1, 230);
			setSelectionMode(NoSelection);
		}

		void QColorTable::setNamesTitle(const String& name)
			throw()
		{
			horizontalHeader()->setLabel(0, name.c_str());
		}
			
		void QColorTable::setContent(const vector<String>& names, const vector<ColorRGBA>& colors)
			throw()
		{
			setting_content_ = true;
			colors_ = colors;
			names_ = names;

			setNumRows(colors_.size());
			for (Position p = 0; p < names_.size(); p++)
			{
				QColorTableItem* c2 = new QColorTableItem(this, QTableItem::WhenCurrent, colors_[p]);
				setText(p, 0, names_[p].c_str());
				setItem(p, 1, c2 );
			}
			setting_content_ = false;
		}

		void QColorTable::setColors(const vector<ColorRGBA>& colors)
			throw()
		{
			colors_ = colors;

			setNumRows(colors_.size());
			for (Position p = 0; p < names_.size(); p++)
			{
				QColorTableItem* c2 = new QColorTableItem(this, QTableItem::WhenCurrent, colors_[p]);
				setItem(p, 1, c2 );
			}
		}

		QWidget* QColorTable::beginEdit(int row, int col, bool)
		{
			if (col == 0 || setting_content_) return 0;
			ColorRGBA old_rgba(((QColorTableItem*)item(row,col))->getColor());
			QColor qcolor = QColorDialog::getColor(old_rgba.getQColor());
			if (!qcolor.isValid()) return 0;

			ColorRGBA new_color(qcolor);
			((QColorTableItem*)item(row,col))->setColor(new_color);
			updateCell(row, col);
			colors_[row] = new_color;
			return 0;
		}
						
		// =========================================================================================
		ColoringSettingsDialog::ColoringSettingsDialog( QWidget* parent,  const char* name, WFlags fl )
			: ColoringSettingsDialogData(parent, name, fl),
				PreferencesEntry()
		{
			setINIFileSectionName("COLORING_OPTIONS");

			element_table_ = new QColorTable(widget_stack->widget(0));
			residue_table_ = new QColorTable(widget_stack->widget(2));
			setDefaultValues(true);

			registerObject_(first_residue_label);
			registerObject_(middle_residue_label);
			registerObject_(last_residue_label);
			
			registerObject_(negative_charge_label);
			registerObject_(neutral_charge_label);
			registerObject_(positive_charge_label);

			registerObject_(null_distance_label);
			registerObject_(max_distance_label);
			registerObject_(max_distance_slider);
			registerObject_(distance_show_selected);

			registerObject_(minimum_tf_label);
			registerObject_(maximum_tf_label);
			registerObject_(unassigned_tf_label);
			registerObject_(max_tf_slider);

			registerObject_(minimum_o_label);
			registerObject_(unassigned_o_label);
			registerObject_(maximum_o_label);

			registerObject_(helix_color_label);
			registerObject_(coil_color_label);
			registerObject_(strand_color_label);
			registerObject_(turn_color_label);

			registerObject_(force_max_color_label);
			registerObject_(force_min_color_label);
			registerObject_(force_max_value_slider);
			registerObject_(force_min_value_slider);

			insertEntry(this, "Colors");
			setWidgetStack(widget_stack);
		}

		void ColoringSettingsDialog::writePreferenceEntries(INIFile& inifile)
		{
			PreferencesEntry::writePreferenceEntries(inifile);

			for (Position p = 0; p < element_table_->getColors().size(); p ++)
			{
				inifile.insertValue(inifile_section_name_, String(p).c_str(), element_table_->getColors()[p]);
			}

			for (Position p = 0; p < residue_table_->getColors().size(); p ++)
			{
				inifile.insertValue(inifile_section_name_, residue_table_->getNames()[p].c_str(), 
												 residue_table_->getColors()[p]);
			}
		}

		void ColoringSettingsDialog::readPreferenceEntries(const INIFile& inifile)
		{
			PreferencesEntry::readPreferenceEntries(inifile);

			vector<ColorRGBA> colors;
			for (Position p = 0; p < 112; p ++)
			{
				ColorRGBA color;
				if (!fetchPreference_(inifile, String(p), color)) break;
				colors.push_back(color);
			}
			if (colors.size() == element_table_->getNames().size())
			{
				element_table_->setColors(colors);
			}

			colors.clear();
			for (Position p = 0; p < residue_table_->getColors().size(); p ++)
			{
				ColorRGBA color;
				if (!fetchPreference_(inifile, residue_table_->getNames()[p], color)) break;
				colors.push_back(color);
			}
			if (colors.size() == residue_table_->getColors().size())
			{
				residue_table_->setColors(colors);
			}
		}

		void ColoringSettingsDialog::setDefaultValues(bool all)
			throw()
		{
			vector<String> 		names;
			vector<ColorRGBA> colors;

			Position current = widget_stack->id(widget_stack->visibleWidget());

			// =============================================================
			// setting element colors
			if (all || current == 0)
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
			if (all || current == 2)
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
			// setting residue number colors
			if (all || current == 1)
			{
				ResidueNumberColorProcessor dummy;
				getSettings(dummy);
			}
			// =============================================================
			// setting charge colors
			if (all || current == 3)
			{
				AtomChargeColorProcessor dummy;
				getSettings(dummy);
			}
			// =============================================================
			// setting distance colors
			if (all || current == 4)
			{
				AtomDistanceColorProcessor dummy;
				getSettings(dummy);
			}
			// =============================================================
			// setting temperature factor colors
			if (all || current == 5)
			{
				TemperatureFactorColorProcessor dummy;
				getSettings(dummy);
			}
			// =============================================================
			// setting occupancy colors
			if (all || current == 6) 
			{
				OccupancyColorProcessor dummy;
				getSettings(dummy);
			}
			// =============================================================
			// setting secondary structure colors
			if (all || current == 7)
			{
				SecondaryStructureColorProcessor dummy;
				getSettings(dummy);
			}
			// =============================================================
			// setting force colors
			if (all || current == 8)
			{
				ForceColorProcessor dummy;
				getSettings(dummy);
			}
		}

		vector<ColorRGBA> ColoringSettingsDialog::getElementColors() const
			throw()
		{
			vector<ColorRGBA> colors;
			colors.push_back(((QColorTableItem*)element_table_->item(element_table_->numRows()-1, 1))->getColor());
			for (Position p = 0; p < (Position)element_table_->numRows(); p++)
			{
				colors.push_back(((QColorTableItem*)element_table_->item(p, 1))->getColor());
			}

			return colors;
		}

		vector<ColorRGBA> ColoringSettingsDialog::getResidueColors() const
			throw()
		{
			vector<ColorRGBA> colors;
			colors.push_back(((QColorTableItem*)residue_table_->item(residue_table_->numRows()-1, 1))->getColor());
			for (Position p = 0; p < (Position)residue_table_->numRows(); p++)
			{
				colors.push_back(((QColorTableItem*)residue_table_->item(p, 1))->getColor());
			}

			return colors;
		}

		void ColoringSettingsDialog::applySettingsTo(ColorProcessor& cp) const
			throw()
		{
			if (RTTI::isKindOf<CustomColorProcessor>(cp))
			{
				return;
			}

			if (RTTI::isKindOf<ElementColorProcessor>(cp))
			{
				vector<ColorRGBA> colors = getElementColors();
				for (Position p = 0; p < colors.size(); p++)
				{
					(*(ElementColorProcessor*)&cp).getColorMap()[p] = colors[p];
				}
				return;
			}
			
			if (RTTI::isKindOf<ResidueNameColorProcessor>(cp))
			{
				for (Position p = 0; p < (Position)residue_table_->numRows(); p++)
				{
					(*(ResidueNameColorProcessor*)&cp).getColorMap()[residue_table_->item(p,0)->text().ascii()] = 
						((QColorTableItem*)residue_table_->item(p,1))->getColor();
				}
				return;
			}

			if (RTTI::isKindOf<ResidueNumberColorProcessor>(cp))
			{
				ResidueNumberColorProcessor& dp = (*(ResidueNumberColorProcessor*)&cp);
				dp.setFirstColor(getLabelColor_(first_residue_label));
				dp.setMiddleColor(getLabelColor_(middle_residue_label));
				dp.setLastColor(getLabelColor_(last_residue_label));
				return;
			}

			if (RTTI::isKindOf<AtomChargeColorProcessor>(cp))
			{
				AtomChargeColorProcessor& dp = (*(AtomChargeColorProcessor*)&cp);
				dp.setPositiveColor(getLabelColor_(positive_charge_label));
				dp.setNegativeColor(getLabelColor_(negative_charge_label));
				dp.setNeutralColor(getLabelColor_(neutral_charge_label));
				return;
			}

			if (RTTI::isKindOf<AtomDistanceColorProcessor>(cp))
			{
				AtomDistanceColorProcessor& dp = (*(AtomDistanceColorProcessor*)&cp);
				dp.setNullDistanceColor(getLabelColor_(null_distance_label));
				dp.setMaxDistanceColor(getLabelColor_(max_distance_label));
				dp.setDistance(((float)max_distance_slider->value()) / 10.0);
 				dp.setShowSelected(distance_show_selected->isChecked());
				return;
			}

			if (RTTI::isKindOf<OccupancyColorProcessor>(cp))
			{
				OccupancyColorProcessor& dp = (*(OccupancyColorProcessor*)&cp);
				dp.setMinColor(getLabelColor_(minimum_o_label));
				dp.setMaxColor(getLabelColor_(maximum_o_label));
				return;
			}

			if (RTTI::isKindOf<SecondaryStructureColorProcessor>(cp))
			{
				SecondaryStructureColorProcessor& dp = (*(SecondaryStructureColorProcessor*)&cp);

				dp.setHelixColor(getLabelColor_(helix_color_label));
				dp.setCoilColor(getLabelColor_(coil_color_label));
				dp.setStrandColor(getLabelColor_(strand_color_label));
				dp.setTurnColor(getLabelColor_(turn_color_label));

				return;
			}

			if (RTTI::isKindOf<TemperatureFactorColorProcessor>(cp))
			{
				TemperatureFactorColorProcessor& dp = (*(TemperatureFactorColorProcessor*)&cp);
				dp.setMinMinColor(getLabelColor_(unassigned_tf_label));
				dp.setMinColor(getLabelColor_(minimum_tf_label));
				dp.setMaxColor(getLabelColor_(maximum_tf_label));
				dp.setMaxMaxColor(getLabelColor_(unassigned_tf_label));
				dp.setMaxValue(((float)max_tf_slider->value()) / 10.0);
				return;
			}

			if (RTTI::isKindOf<ForceColorProcessor>(cp))
			{
				ForceColorProcessor& dp = (*(ForceColorProcessor*)&cp);
				dp.setMinColor(getLabelColor_(force_min_color_label));
				dp.setMaxColor(getLabelColor_(force_max_color_label));
				dp.setMaxValue(((float)force_max_value_slider->value()) / 10.0);
				dp.setMinValue(((float)force_min_value_slider->value()) / 10.0);
				return;
			}
		}
			

		void ColoringSettingsDialog::minimumOccupancyColorPressed()
		{
			chooseColor_(minimum_o_label);
		}
			
		void ColoringSettingsDialog::middleResidueColorPressed()
		{
			chooseColor_(middle_residue_label);
		}
			
		void ColoringSettingsDialog::lastResidueColorPressed()
		{
			chooseColor_(last_residue_label);
		}
			
		void ColoringSettingsDialog::negativeChargeColorPressed()
		{
			chooseColor_(negative_charge_label);
		}
			
		void ColoringSettingsDialog::neutralChargeColorPressed()
		{
			chooseColor_(neutral_charge_label);
		}
			
		void ColoringSettingsDialog::positiveChargeColorPressed()
		{
			chooseColor_(positive_charge_label);
		}
			
		void ColoringSettingsDialog::nullDistanceColorPressed()
		{
			chooseColor_(null_distance_label);
		}
			
		void ColoringSettingsDialog::maxDistanceColorPressed()
		{
			chooseColor_(max_distance_label);
		}

		void ColoringSettingsDialog::minimumTFColorPressed()
		{
			chooseColor_(minimum_tf_label);
		}
			
		void ColoringSettingsDialog::maximumTFColorPressed()
		{
			chooseColor_(maximum_tf_label);
		}
			
		void ColoringSettingsDialog::unassignedTFColorPressed()
		{
			chooseColor_(unassigned_tf_label);
		}
			
		void ColoringSettingsDialog::maximumOccupancyColorPressed()
		{
			chooseColor_(maximum_o_label);
		}
			
		void ColoringSettingsDialog::unassignedOccupancyColorPressed()
		{
			chooseColor_(unassigned_o_label);
		}

		void ColoringSettingsDialog::firstResidueColorPressed()
		{
			chooseColor_(first_residue_label);
		}

		void ColoringSettingsDialog::helixColorPressed()
		{
			chooseColor_(helix_color_label);
		}

		void ColoringSettingsDialog::turnColorPressed()
		{
			chooseColor_(turn_color_label);
		}

		void ColoringSettingsDialog::strandColorPressed()
		{
			chooseColor_(strand_color_label);
		}

		void ColoringSettingsDialog::coilColorPressed()
		{
			chooseColor_(coil_color_label);
		}

		void ColoringSettingsDialog::forceMaxColorPressed()
		{
			chooseColor_(force_max_color_label);
		}

		void ColoringSettingsDialog::forceMinColorPressed()
		{
			chooseColor_(force_min_color_label);
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

				default:
					throw(Exception::InvalidOption(__FILE__, __LINE__, method));
			}

			applySettingsTo(*color_processor);

			return color_processor;
		}

		void ColoringSettingsDialog::getSettings(const ColorProcessor& cp)
			throw()
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
				setLabelColor_(first_residue_label, dp.getFirstColor());
				setLabelColor_(middle_residue_label, dp.getMiddleColor());
				setLabelColor_(last_residue_label, dp.getLastColor());
			} else

			if (RTTI::isKindOf<AtomChargeColorProcessor>(cp))
			{
				AtomChargeColorProcessor& dp = (*(AtomChargeColorProcessor*)&cp);
				setLabelColor_(positive_charge_label, dp.getPositiveColor());
				setLabelColor_(negative_charge_label, dp.getNegativeColor());
				setLabelColor_(neutral_charge_label, dp.getNeutralColor());
			} else

			if (RTTI::isKindOf<AtomDistanceColorProcessor>(cp))
			{
				AtomDistanceColorProcessor& dp = (*(AtomDistanceColorProcessor*)&cp);
				setLabelColor_(null_distance_label, dp.getNullDistanceColor());
				setLabelColor_(max_distance_label, dp.getMaxDistanceColor());
				max_distance_slider->setValue((Size)(dp.getDistance() * 10.0));
 				distance_show_selected->setChecked(dp.showSelected());
			} else

			if (RTTI::isKindOf<OccupancyColorProcessor>(cp))
			{
				OccupancyColorProcessor& dp = (*(OccupancyColorProcessor*)&cp);
				setLabelColor_(minimum_o_label, dp.getMinColor());
				setLabelColor_(maximum_o_label, dp.getMaxColor());
			} else

			if (RTTI::isKindOf<SecondaryStructureColorProcessor>(cp))
			{
				SecondaryStructureColorProcessor& dp = (*(SecondaryStructureColorProcessor*)&cp);
				setLabelColor_(helix_color_label, dp.getHelixColor());
				setLabelColor_(coil_color_label, dp.getCoilColor());
				setLabelColor_(strand_color_label, dp.getStrandColor());
				setLabelColor_(turn_color_label, dp.getTurnColor());
			} else

			if (RTTI::isKindOf<TemperatureFactorColorProcessor>(cp))
			{
				TemperatureFactorColorProcessor& dp = (*(TemperatureFactorColorProcessor*)&cp);

				setLabelColor_(unassigned_tf_label, dp.getMinMinColor());
				setLabelColor_(minimum_tf_label, dp.getMinColor());
				setLabelColor_(maximum_tf_label, dp.getMaxColor());
				max_tf_slider->setValue((Size)(dp.getMaxValue() * 10.0));
			} else

			if (RTTI::isKindOf<ForceColorProcessor>(cp))
			{
				ForceColorProcessor& dp = (*(ForceColorProcessor*)&cp);

				setLabelColor_(force_min_color_label, dp.getMinColor());
				setLabelColor_(force_max_color_label, dp.getMaxColor());
				force_max_value_slider->setValue((Size)(dp.getMaxValue() * 10.0));
				force_min_value_slider->setValue((Size)(dp.getMinValue() * 10.0));
			}
		}

		QWidget* ColoringSettingsDialog::getEntryFor(ColoringMethod method)
			throw()
		{
			switch (method)
			{
				case COLORING_ELEMENT:
					return widget_stack->widget(0);

				case COLORING_RESIDUE_NAME:
					return widget_stack->widget(2);

				case COLORING_RESIDUE_INDEX:
					return widget_stack->widget(1);

				case COLORING_SECONDARY_STRUCTURE:
					return widget_stack->widget(7);

				case COLORING_ATOM_CHARGE:
					return widget_stack->widget(3);

				case COLORING_DISTANCE:
					return widget_stack->widget(4);

				case COLORING_TEMPERATURE_FACTOR:
					return widget_stack->widget(5);

				case COLORING_OCCUPANCY:
					return widget_stack->widget(6);

				case COLORING_FORCES:
					return widget_stack->widget(8);

				default:
					break;
			}

			return 0;
		}

  } // namespace VIEW
} // namespace BALL
