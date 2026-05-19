// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// Phase 999.44 — StageController implementation.
//

#include <BALL/VIEW/KERNEL/controllers/stageController.h>


#include <BALL/VIEW/KERNEL/stage.h>
#include <BALL/VIEW/WIDGETS/scene.h>
#include <BALL/VIEW/DATATYPE/colorRGBA.h>
#include <BALL/COMMON/logStream.h>

// std::clamp is used by colorRGBAToQColor_ below.
#include <algorithm>

namespace BALL
{
	namespace VIEW
	{

		namespace
		{
			QColor colorRGBAToQColor_(const ColorRGBA& c)
			{
				// ColorRGBA channels are ColorUnits (0..1); QColor wants 0..255.
				// Explicit float casts disambiguate ColorUnit's many built-in
				// conversion operators (operator char/int/float/...).
				float r = static_cast<float>(c.getRed());
				float g = static_cast<float>(c.getGreen());
				float b = static_cast<float>(c.getBlue());
				float a = static_cast<float>(c.getAlpha());
				int ri = static_cast<int>(r * 255.0f);
				int gi = static_cast<int>(g * 255.0f);
				int bi = static_cast<int>(b * 255.0f);
				int ai = static_cast<int>(a * 255.0f);
				return QColor(std::clamp(ri, 0, 255),
				              std::clamp(gi, 0, 255),
				              std::clamp(bi, 0, 255),
				              std::clamp(ai, 0, 255));
			}
		}

		StageController::StageController(Stage* stage, Scene* scene, QObject* parent)
			: QObject(parent),
				stage_(stage),
				scene_(scene),
				background_color_(Qt::black),
				show_coordinate_system_(false),
				fog_intensity_(0.0f),
				eye_distance_(0.0f),
				focal_distance_(0.0f)
		{
			revert();
		}

		StageController::~StageController() = default;

		void StageController::setStage(Stage* stage)
		{
			if (stage_ == stage) return;
			stage_ = stage;
			revert();
		}

		void StageController::setScene(Scene* scene)
		{
			scene_ = scene;
		}

		void StageController::revert()
		{
			if (!stage_)
				return;
			QColor bg = colorRGBAToQColor_(stage_->getBackgroundColor());
			if (bg != background_color_)
			{
				background_color_ = bg;
				Q_EMIT backgroundColorChanged(background_color_);
			}
			bool cs = stage_->coordinateSystemEnabled();
			if (cs != show_coordinate_system_)
			{
				show_coordinate_system_ = cs;
				Q_EMIT showCoordinateSystemChanged(cs);
			}
			float fi = stage_->getFogIntensity();
			if (fi != fog_intensity_)
			{
				fog_intensity_ = fi;
				Q_EMIT fogIntensityChanged(fi);
			}
			float ed = stage_->getEyeDistance();
			if (ed != eye_distance_)
			{
				eye_distance_ = ed;
				Q_EMIT eyeDistanceChanged(ed);
			}
			float fd = stage_->getFocalDistance();
			if (fd != focal_distance_)
			{
				focal_distance_ = fd;
				Q_EMIT focalDistanceChanged(fd);
			}
		}

		void StageController::apply()
		{
			// Phase 999.44 Plan 06 — cut-over complete. Push the
			// mirrored fields to the live Stage. Both legacy
			// StageSettings::apply() and the Inspector StageSection /
			// BackgroundSection now mutate the Stage through this
			// single method — consistency-by-construction per Handover
			// §risks. The legacy dialog continues to call its own
			// apply() body for the fields this controller does NOT
			// yet mirror (vertex-buffer toggle, downsampling, etc.);
			// those follow the same migration recipe as new Q_PROPERTY
			// surface lands on the controller.
			if (stage_ == nullptr)
			{
				Log.warn() << "[StageController::apply] no Stage attached — skipping." << std::endl;
				return;
			}

			// background_color_ is QColor; Stage stores ColorRGBA.
			// ColorRGBA::set(const QColor&) handles the 0..255 →
			// 0..1 (ColorUnit) conversion + alpha.
			ColorRGBA c;
			c.set(background_color_);
			stage_->setBackgroundColor(c);

			stage_->showCoordinateSystem(show_coordinate_system_);
			stage_->setFogIntensity(fog_intensity_);
			stage_->setEyeDistance(eye_distance_);
			stage_->setFocalDistance(focal_distance_);

			// UFG-25 (rc4 → rc5 → rc5-followup) — Stage::setBackgroundColor()
			// only stores the new value in the Stage struct. The
			// actual `glClearColor()` requires a separate call to
			// `RenderSetup::updateBackgroundColor()` per renderer,
			// and the fog intensity needs `setFogIntensity()` on the
			// GL renderer (these are what the legacy Stage Preferences
			// dialog does in `Scene::applyPreferences` lines
			// 1142-1155). Just calling `updateGL()` repaints with the
			// OLD glClearColor cached on the renderer.
			//
			// `Scene::refreshSceneRenderState()` is the new public
			// helper that does the renderers loop + updateGL — mirroring
			// the applyPreferences path. Covers background + fog;
			// coordinate-system toggle propagates via the existing
			// stage_->showCoordinateSystem() above.
			if (scene_ != nullptr)
				scene_->refreshSceneRenderState();

			// Emit appliedStub so callers wired during the migration
			// window keep observing the apply signal. (Renamed in the
			// 999.48 cleanup to applied().)
			Q_EMIT appliedStub();
		}

		void StageController::setBackgroundColor(const QColor& c)
		{
			if (c == background_color_) return;
			background_color_ = c;
			Q_EMIT backgroundColorChanged(c);
		}

		void StageController::setShowCoordinateSystem(bool b)
		{
			if (b == show_coordinate_system_) return;
			show_coordinate_system_ = b;
			Q_EMIT showCoordinateSystemChanged(b);
		}

		void StageController::setFogIntensity(float v)
		{
			if (v == fog_intensity_) return;
			fog_intensity_ = v;
			Q_EMIT fogIntensityChanged(v);
		}

		void StageController::setEyeDistance(float v)
		{
			if (v == eye_distance_) return;
			eye_distance_ = v;
			Q_EMIT eyeDistanceChanged(v);
		}

		void StageController::setFocalDistance(float v)
		{
			if (v == focal_distance_) return;
			focal_distance_ = v;
			Q_EMIT focalDistanceChanged(v);
		}

	} // namespace VIEW
} // namespace BALL

