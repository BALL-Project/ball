// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/VIEW/MODELS/modelProcessorFactory.h>

#include <BALL/VIEW/MODELS/backboneModel.h>
#include <BALL/VIEW/MODELS/cartoonModel.h>
#include <BALL/VIEW/MODELS/ballAndStickModel.h>
#include <BALL/VIEW/MODELS/lineModel.h>
#include <BALL/VIEW/MODELS/surfaceModel.h>
#include <BALL/VIEW/MODELS/vanDerWaalsModel.h>
#include <BALL/VIEW/MODELS/HBondModel.h>
#include <BALL/VIEW/MODELS/forceModel.h>

#include <BALL/COMMON/logStream.h>

namespace BALL
{
	namespace VIEW
	{

		namespace
		{
			// Per-type settings application, relocated verbatim from
			// ModelSettingsDialog::applySettingsTo (modelSettingsDialog.C:114-186),
			// with every dialog getter / checkbox read replaced by the matching
			// ModelProcessorParams field. Behavior is preserved exactly.
			void applySettingsTo_(ModelProcessor& mp, const ModelProcessorParams& params)
			{
				if (RTTI::isKindOf<AddLineModel>(&mp))
				{
					return;
				}

				if (RTTI::isKindOf<AddBallAndStickModel>(&mp))
				{
					AddBallAndStickModel& bsm = *((AddBallAndStickModel*)&mp);
					if (bsm.isStickModel())
					{
						bsm.setStickRadius(params.stick_stick_radius);
					}
					else
					{
						bsm.setStickRadius(params.ball_and_stick_stick_radius);
						bsm.setBallRadius(params.ball_radius);
					}
					bsm.enableDashedBonds(bsm.isBallAndStickModel() && params.ball_and_stick_dashed_bonds_enabled);
					return;
				}

				if (RTTI::isKindOf<AddSurfaceModel>(&mp))
				{
					((AddSurfaceModel*)&mp)->setProbeRadius(params.surface_probe_radius);
					return;
				}

				if (RTTI::isKindOf<AddVanDerWaalsModel>(&mp))
				{
					((AddVanDerWaalsModel*) &mp)->setVDWRadiusFactor(params.vdw_radius_factor);
					return;
				}

				if (RTTI::isKindOf<AddCartoonModel>(&mp))
				{
					AddCartoonModel& cm = *dynamic_cast<AddCartoonModel*>(&mp);
					cm.setTubeRadius(params.cartoon_tube_radius);
					cm.setHelixRadius(params.cartoon_helix_radius);
					cm.setArrowWidth(params.cartoon_arrow_width);
					cm.setStrandHeight(params.cartoon_strand_height);
					cm.setStrandWidth(params.cartoon_strand_width);
					cm.setDrawDNAAsLadderModel(params.cartoon_dna_ladder_enabled);
					cm.setDNALadderRadius(params.dna_ladder_radius);
					cm.setDNABaseRadius(params.dna_base_radius);
					cm.setDNAHelixRadius(params.dna_helix_radius);
					cm.enableRibbons(params.ribbons_enabled);
					return;
				}

				// backbone model after cartoon model !!!
				if (RTTI::isKindOf<AddBackboneModel>(&mp))
				{
					((AddBackboneModel*) &mp)->setTubeRadius(params.tube_radius);
					return;
				}

				if (RTTI::isKindOf<HBondModelProcessor>(&mp))
				{
					((HBondModelProcessor*) &mp)->setRadius(params.hbonds_radius);
					return;
				}

				if (RTTI::isKindOf<ForceModel>(&mp))
				{
					((ForceModel*) &mp)->setMaxLength(params.force_max_length);
					((ForceModel*) &mp)->setScaling(params.force_scaling);
					((ForceModel*) &mp)->setOffset(params.force_offset);
					((ForceModel*) &mp)->setBaseSize(params.force_base);
					return;
				}
			}
		} // anonymous namespace

		// Relocated verbatim from ModelSettingsDialog::createModelProcessor
		// (modelSettingsDialog.C:189-255), with the only-getter MODEL_VDW /
		// MODEL_BACKBONE reads replaced by the matching ModelProcessorParams fields.
		ModelProcessor* ModelProcessorFactory::create(ModelType type, const ModelProcessorParams& params)
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
					((AddVanDerWaalsModel*) model_processor)->setVDWRadiusFactor(params.vdw_radius_factor);
					break;

				case MODEL_BACKBONE:
					model_processor = new AddBackboneModel;
					((AddBackboneModel*) model_processor)->setTubeRadius(params.tube_radius);
					break;

				case MODEL_RIBBON:
					model_processor = new AddBackboneModel;
					((AddBackboneModel*) model_processor)->setRibbonMode(true);
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
					Log.error() << "ModelProcessorFactory::create: unknown ModelType: " << type << std::endl;
					return NULL;
			}

			applySettingsTo_(*model_processor, params);

			return model_processor;
		}

	} // namespace VIEW
} // namespace BALL
