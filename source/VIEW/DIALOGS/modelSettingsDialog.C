// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: modelSettingsDialog.C,v 1.20 2004/09/02 12:45:54 amoll Exp $
//

#include <BALL/VIEW/DIALOGS/modelSettingsDialog.h>


#include <BALL/VIEW/MODELS/backboneModel.h>
#include <BALL/VIEW/MODELS/cartoonModel.h>
#include <BALL/VIEW/MODELS/ballAndStickModel.h>
#include <BALL/VIEW/MODELS/lineModel.h>
#include <BALL/VIEW/MODELS/surfaceModel.h>
#include <BALL/VIEW/MODELS/vanDerWaalsModel.h>
#include <BALL/VIEW/MODELS/HBondModel.h>
#include <BALL/VIEW/MODELS/forceModel.h>
#include <BALL/VIEW/MODELS/standardColorProcessor.h>

#include <BALL/DATATYPE/string.h>
#include <BALL/FORMAT/INIFile.h>

#include <qslider.h>
#include <qlabel.h>
#include <qlistbox.h>
#include <qwidgetstack.h>

namespace BALL
{
	namespace VIEW
	{

		ModelSettingsDialog::ModelSettingsDialog( QWidget* parent,  const char* name, WFlags fl )
				: ModelSettingsDialogData( parent, name, fl )
		{
			setDefaults();
		}

		void ModelSettingsDialog::setDefaults(bool all)
			throw()
		{
			if (all || list_box->currentItem() == 0)
			{
				stick_radius_slider->setValue(2);
			}
			
			if (all || list_box->currentItem() == 1)
			{
				ball_stick_cylinder_radius_slider->setValue(2);
				ball_stick_sphere_radius_slider->setValue(4);
				ball_stick_dashed_bonds->setChecked(true);
			}
			
			if (all || list_box->currentItem() == 2)
			{
				vdw_radius_factor_slider->setValue(10);
			}
			
			if (all || list_box->currentItem() == 3)
			{
				surface_probe_radius_slider->setValue(15);
			}
			
			if (all || list_box->currentItem() == 4)
			{
				tube_radius_slider->setValue(4);
			}
			
			if (all || list_box->currentItem() == 5)
			{
				cartoon_tube_radius_slider->setValue(4);
				cartoon_helix_radius_slider->setValue(20);
				cartoon_arrow_width_slider->setValue(20);
				cartoon_arrow_height_slider->setValue(4);
			}
			
			if (all || list_box->currentItem() == 6)
			{
				hbonds_radius_slider->setValue(3);
			}

			if (all || list_box->currentItem() == 7)
			{
				force_max_length_slider->setValue((Size)(10 * 10.0));
				force_scaling_slider->setValue(Size(11 * 10.0));
			}
		}

		float ModelSettingsDialog::getFloatValue_(const QSlider* const& slider) const
			throw()
		{
			return (float(slider->value())) / 10.0;
		}

		void ModelSettingsDialog::setValue_(QSlider* le, float value)
			throw()
		{
			le->setValue((Size)(value * 10.0));
		}

		void ModelSettingsDialog::setLabelText_(QLabel* label, const QSlider* const from)
			throw()
		{
			String s((float)from->value() / 10.0);
			s.trimRight("0");
			if (s.hasSuffix(".")) s+= "0";
			label->setText(s.c_str());
		}

		void ModelSettingsDialog::fetchPreference_(const INIFile& inifile, const String& entry, QSlider& slider)
			throw()
		{
			if (!inifile.hasEntry("MODEL_OPTIONS", entry)) return;
			setValue_(&slider, inifile.getValue("MODEL_OPTIONS", entry).toFloat());
		}

		void ModelSettingsDialog::writePreference_(INIFile& inifile, const String& entry, const QSlider& slider) const
			throw()
		{
			inifile.insertValue("MODEL_OPTIONS", entry, getFloatValue_(&slider));
		}


		void ModelSettingsDialog::writePreferences(INIFile& file)
			throw()
		{
			file.appendSection("MODEL_OPTIONS");
			writePreference_(file, "stick_radius", *stick_radius_slider);
			writePreference_(file, "ball_stick_cylinder_radius", *ball_stick_cylinder_radius_slider);
			writePreference_(file, "ball_stick_sphere_radius", *ball_stick_sphere_radius_slider);
			if (ball_stick_dashed_bonds->isChecked())
			{
				file.insertValue("MODEL_OPTIONS", "ball_stick_dashed_bonds", true);
			}

			writePreference_(file, "vdw_radius_factor", *vdw_radius_factor_slider);
			writePreference_(file, "surface_probe_radius", *surface_probe_radius_slider);
			writePreference_(file, "tube_radius", *tube_radius_slider);
			writePreference_(file, "cartoon_tube_radius", *cartoon_tube_radius_slider);
			writePreference_(file, "cartoon_helix_radius", *cartoon_helix_radius_slider);
			writePreference_(file, "cartoon_arrow_height", *cartoon_arrow_height_slider);
			writePreference_(file, "cartoon_arrow_width", *cartoon_arrow_width_slider);
			
			writePreference_(file, "force_scaling", *force_scaling_slider);
			writePreference_(file, "force_max_length", *force_max_length_slider);
		
			writePreference_(file, "hbonds_radius", *hbonds_radius_slider);
		}

		void ModelSettingsDialog::fetchPreferences(const INIFile& file)
			throw()
		{
			fetchPreference_(file, "stick_radius", *stick_radius_slider);
			
			fetchPreference_(file, "ball_stick_cylinder_radius", *ball_stick_cylinder_radius_slider);
			fetchPreference_(file, "ball_stick_sphere_radius", *ball_stick_sphere_radius_slider);
			ball_stick_dashed_bonds->setChecked(
				(file.hasEntry("MODEL_OPTIONS", "ball_stick_dashed_bonds")));
			
			fetchPreference_(file, "vdw_radius_factor", *vdw_radius_factor_slider);
			fetchPreference_(file, "surface_probe_radius", *surface_probe_radius_slider);
			fetchPreference_(file, "tube_radius", *tube_radius_slider);
			fetchPreference_(file, "cartoon_tube_radius", *cartoon_tube_radius_slider);
			fetchPreference_(file, "cartoon_helix_radius", *cartoon_helix_radius_slider);
			fetchPreference_(file, "cartoon_arrow_height", *cartoon_arrow_height_slider);
			fetchPreference_(file, "cartoon_arrow_width", *cartoon_arrow_width_slider);

			fetchPreference_(file, "force_max_length", *force_max_length_slider);
			fetchPreference_(file, "force_scaling", *force_scaling_slider);

			fetchPreference_(file, "hbonds_radius", *hbonds_radius_slider);
		}

		void ModelSettingsDialog::setDefaultValues()
			throw()
		{
			setDefaults(false);
		}

		void ModelSettingsDialog::showPage(int nr)
		{
			if (widget_stack->widget(nr) == 0)
			{
				return;
			}

			if (list_box->currentItem() != nr)
			{
				list_box->setCurrentItem(nr);
			}
			widget_stack->raiseWidget(nr);
		}


		void ModelSettingsDialog::applySettingsTo(ModelProcessor& mp) const
			throw()
		{
			if (RTTI::isKindOf<AddLineModel>(mp))
			{
				return;
			}
					
			if (RTTI::isKindOf<AddBallAndStickModel>(mp))
			{
				((AddBallAndStickModel*)&mp)->setStickRadius(getBallAndStickStickRadius());
				((AddBallAndStickModel*)&mp)->setBallRadius(getBallRadius());
				((AddBallAndStickModel*)&mp)->enableDashedBonds(
												((AddBallAndStickModel*)&mp)->isBallAndStickModel() &&
												ballAndStickDashedBondsEnabled());
				return;
			}
					
			if (RTTI::isKindOf<AddSurfaceModel>(mp))
			{
				((AddSurfaceModel*)&mp)->setProbeRadius(getSurfaceProbeRadius());
				return;
			}
					
			if (RTTI::isKindOf<AddVanDerWaalsModel>(mp))
			{
				((AddVanDerWaalsModel*) &mp)->setVDWRadiusFactor(getVDWRadiusFactor());
				return;
			}

			if (RTTI::isKindOf<AddVanDerWaalsModel>(mp))
			{
				((AddBackboneModel*) &mp)->setTubeRadius(getTubeRadius());
				return;
			}
					
			if (RTTI::isKindOf<AddCartoonModel>(mp))
			{
				((AddCartoonModel*) &mp)->setTubeRadius(getCartoonTubeRadius());
				((AddCartoonModel*) &mp)->setHelixRadius(getCartoonHelixRadius());
				((AddCartoonModel*) &mp)->setArrowWidth(getCartoonArrowWidth());
				((AddCartoonModel*) &mp)->setArrowHeight(getCartoonArrowHeight());
				return;
			}
					
			if (RTTI::isKindOf<HBondModelProcessor>(mp))
			{
				((HBondModelProcessor*) &mp)->setRadius(getHBondsRadius());
				return;
			}
					
			if (RTTI::isKindOf<ForceModel>(mp))
			{
				((ForceModel*) &mp)->setMaxLength((float)(getForceMaxLength()) / 10.0);
				((ForceModel*) &mp)->setScaling((float)(getForceScaling()) / 1000.0);
				return;
			}
		}


		ModelProcessor* ModelSettingsDialog::createModelProcessor(ModelType type) const
			throw(Exception::InvalidOption)
		{
			ModelProcessor* model_processor = 0;

			switch (type)
			{
				case MODEL_LINES:
					model_processor = new AddLineModel;
					break;
					
				case MODEL_STICK:
					model_processor = new AddBallAndStickModel;
					((AddBallAndStickModel*)model_processor)->enableStickModel();
					break;
					
				case MODEL_BALL_AND_STICK:
					model_processor = new AddBallAndStickModel;
					((AddBallAndStickModel*)model_processor)->enableBallAndStickModel();
					break;
					
				case MODEL_SE_SURFACE:
					model_processor = new AddSurfaceModel;
					((AddSurfaceModel*)model_processor)->setType(SurfaceProcessor::SOLVENT_EXCLUDED_SURFACE);	
					break;
					
				case MODEL_SA_SURFACE:
					model_processor = new AddSurfaceModel;
					((AddSurfaceModel*)model_processor)->setType(SurfaceProcessor::SOLVENT_ACCESSIBLE_SURFACE);	
					break;
					
				case MODEL_VDW:
					model_processor = new AddVanDerWaalsModel;
					((AddVanDerWaalsModel*) model_processor)->setVDWRadiusFactor(getVDWRadiusFactor());
					break;

				case MODEL_BACKBONE:
					model_processor = new AddBackboneModel;
					((AddBackboneModel*) model_processor)->setTubeRadius(getTubeRadius());
					break;

				case MODEL_CARTOON:
					model_processor = new AddCartoonModel;
					break;
					
				case MODEL_HBONDS:
					model_processor = new HBondModelProcessor;
					break;

				case MODEL_FORCES:
					model_processor = new ForceModel;
					break;
					
				default:
					throw(Exception::InvalidOption(__FILE__, __LINE__, type));
			}

			applySettingsTo(*model_processor);

			return model_processor;
		}


		void ModelSettingsDialog::getSettings(const ModelProcessor& mp)
			throw()
		{
			if (RTTI::isKindOf<AddBallAndStickModel>(mp))
			{
				if (((AddBallAndStickModel*) &mp)->isStickModel())
				{
					setStickStickRadius(((AddBallAndStickModel*)&mp)->getStickRadius());
				}
				else
				{
					setBallAndStickStickRadius(((AddBallAndStickModel*)&mp)->getStickRadius());
					setBallRadius(((AddBallAndStickModel*)&mp)->getBallRadius());
				}
				return;
			}
					
			if (RTTI::isKindOf<AddSurfaceModel>(mp))
			{
				setSurfaceProbeRadius(((AddSurfaceModel*)&mp)->getProbeRadius());
				return;
			}
					
			if (RTTI::isKindOf<AddVanDerWaalsModel>(mp))
			{
				setVDWRadiusFactor(((AddVanDerWaalsModel*) &mp)->getVDWRadiusFactor());
				return;
			}
					
			if (RTTI::isKindOf<AddBackboneModel>(mp))
			{
				setTubeRadius(((AddBackboneModel*) &mp)->getTubeRadius());
				return;
			}
					
			if (RTTI::isKindOf<AddCartoonModel>(mp))
			{
				setCartoonTubeRadius(((AddCartoonModel*) &mp)->getTubeRadius());
				setCartoonHelixRadius(((AddCartoonModel*) &mp)->getHelixRadius());
				setCartoonArrowWidth(((AddCartoonModel*) &mp)->getArrowWidth());
				setCartoonArrowHeight(((AddCartoonModel*) &mp)->getArrowHeight());
				return;
			}
					
			if (RTTI::isKindOf<HBondModelProcessor>(mp))
			{
				setHBondRadius(((HBondModelProcessor*) &mp)->getRadius());
				return;
			}
					
			if (RTTI::isKindOf<ForceModel>(mp))
			{
				setForceScaling(((ForceModel*) &mp)->getScaling());
				setForceMaxLenght(((ForceModel*) &mp)->getMaxLength());
				return;
			}

		}

	} // namespace VIEW
} // namespace BALL
