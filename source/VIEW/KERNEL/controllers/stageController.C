// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// Phase 999.44 — StageController implementation.
//

#include <BALL/VIEW/KERNEL/controllers/stageController.h>

#ifdef BALL_UI_V2

#include <BALL/VIEW/KERNEL/stage.h>
#include <BALL/VIEW/DATATYPE/colorRGBA.h>
#include <BALL/COMMON/logStream.h>

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
			// TODO(999.44-RC-patch): mirror StageSettings::saveSettingsToStage_().
			// During the migration window the legacy StageSettings dialog
			// still owns the actual mutation path; this controller is a
			// read-only mirror so the Inspector StageSection can show
			// live values while we wait for the cut-over plan.
			Log.info()
				<< "[StageController::apply] STUB — legacy StageSettings owns the "
				   "actual mutation until the 999.44-RC-patch cut-over plan lands. "
				   "Mirrored values: bg=("
				<< background_color_.red() << ","
				<< background_color_.green() << ","
				<< background_color_.blue() << ") coordSys="
				<< (show_coordinate_system_ ? "on" : "off")
				<< " fog=" << fog_intensity_
				<< std::endl;
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

#endif // BALL_UI_V2
