// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: modelSettingsDialog.C,v 1.27 2004/09/28 17:36:00 amoll Exp $
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
#include <qradiobutton.h>
#include <qbuttongroup.h>

namespace BALL
{
	namespace VIEW
	{

		ModelSettingsDialog::ModelSettingsDialog( QWidget* parent,  const char* name, WFlags fl )
			: ModelSettingsDialogData( parent, name, fl ),
				PreferencesEntry()
		{
			setINIFileSectionName("MODEL_OPTIONS");
Log.error() << "#~~#   1 "             << " "  << __FILE__ << "  " << __LINE__<< std::endl;

			registerObject_(stick_radius_slider);

			registerObject_(ball_stick_cylinder_radius_slider);
			registerObject_(ball_stick_sphere_radius_slider);
			registerObject_(ball_stick_dashed_bonds);
			
			registerObject_(vdw_radius_factor_slider);
			
			registerObject_(surface_probe_radius_slider);
			
			registerObject_(tube_radius_slider);
			
			registerObject_(cartoon_tube_radius_slider);
			registerObject_(cartoon_helix_radius_slider);
			registerObject_(cartoon_arrow_height_slider);
			registerObject_(cartoon_arrow_width_slider);
			registerObject_(cartoon_dna_helix_radius_slider);
			registerObject_(cartoon_dna_ladder_radius_slider);
			registerObject_(cartoon_dna_base_radius_slider);
			registerObject_(cartoon_dna_wac);
			
			registerObject_(force_scaling_slider);
			registerObject_(force_max_length_slider);

			registerObject_(hbonds_radius_slider);
Log.error() << "#~~#   2 "             << " "  << __FILE__ << "  " << __LINE__<< std::endl;

			setDefaults(true);
		}

		void ModelSettingsDialog::setDefaults(bool all)
			throw()
		{
			if (all || list_box->currentItem() == 0)
			{
				AddBallAndStickModel dummy;
				dummy.enableStickModel();
				getSettings(dummy);
			}
			
			if (all || list_box->currentItem() == 1)
			{
				AddBallAndStickModel dummy;
				getSettings(dummy);
			}
			
			if (all || list_box->currentItem() == 2)
			{
				AddVanDerWaalsModel dummy;
				getSettings(dummy);
			}
			
			if (all || list_box->currentItem() == 3)
			{
				AddSurfaceModel dummy;
				getSettings(dummy);
			}
			
			if (all || list_box->currentItem() == 4)
			{
				AddBackboneModel dummy;
				getSettings(dummy);
			}

			if (all || list_box->currentItem() == 5)
			{
				AddCartoonModel dummy;
				getSettings(dummy);
			}
			
			if (all || list_box->currentItem() == 6)
			{
				HBondModelProcessor dummy;
				getSettings(dummy);
			}
			
			if (all || list_box->currentItem() == 7)
			{
				ForceModel dummy;
				getSettings(dummy);
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

		void ModelSettingsDialog::setDefaultValues()
			throw()
		{
			setDefaults(false);
		}

		void ModelSettingsDialog::showPage_(int nr)
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
				((AddCartoonModel*) &mp)->setDrawDNAAsLadderModel(cartoon_dna_ladder->isChecked());
				((AddCartoonModel*) &mp)->setDNALadderRadius(getDNALadderRadius());
				((AddCartoonModel*) &mp)->setDNABaseRadius(getDNABaseRadius());
				((AddCartoonModel*) &mp)->setDNAHelixRadius(getDNAHelixRadius());
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
				((ForceModel*) &mp)->setScaling((float)(getForceScaling()) / 10.0);
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
					
			if (RTTI::isKindOf<AddCartoonModel>(mp))
			{
				setCartoonTubeRadius(((AddCartoonModel*) &mp)->getTubeRadius());
				setCartoonHelixRadius(((AddCartoonModel*) &mp)->getHelixRadius());
				setCartoonArrowWidth(((AddCartoonModel*) &mp)->getArrowWidth());
				setCartoonArrowHeight(((AddCartoonModel*) &mp)->getArrowHeight());
				setCartoonDNAHelixRadius(((AddCartoonModel*) &mp)->getDNAHelixRadius());
				setCartoonDNALadderRadius(((AddCartoonModel*) &mp)->getDNALadderRadius());
				setCartoonDNABaseRadius(((AddCartoonModel*) &mp)->getDNABaseRadius());
				cartoon_dna_ladder->setChecked(((AddCartoonModel*) &mp)->drawDNAAsLadderModel());
				cartoon_dna_wac->setChecked(!((AddCartoonModel*) &mp)->drawDNAAsLadderModel());
				return;
			}

			// after derived class
			if (RTTI::isKindOf<AddBackboneModel>(mp))
			{
				setTubeRadius(((AddBackboneModel*) &mp)->getTubeRadius());
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

		void ModelSettingsDialog::showPage(ModelType type)
			throw()
		{
			switch (type)
			{
				case MODEL_LINES:
					break;
					
				case MODEL_STICK:
					showPage_(0);
					break;

				case MODEL_BALL_AND_STICK:
					showPage_(1);
					break;
					
				case MODEL_SE_SURFACE:
				case MODEL_SA_SURFACE:
					showPage_(3);
					break;
					
				case MODEL_VDW:
					showPage_(2);
					break;

				case MODEL_BACKBONE:
					showPage_(4);
					break;

				case MODEL_CARTOON:
					showPage_(5);
					break;
					
				case MODEL_HBONDS:
					showPage_(6);
					break;

				case MODEL_FORCES:
					showPage_(7);
					break;

				default:
					break;
			}
		}

	} // namespace VIEW
} // namespace BALL
