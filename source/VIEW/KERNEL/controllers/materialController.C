// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// Phase 999.44 Plan 04 — MaterialController implementation.
// UFG-03 cut-over (v1.7-modernization) — apply() now mutates the
// attached Representation's material through the Scene's per-rep
// material path (Scene::updateMaterialForRepresentation), which is
// the same backend the legacy MaterialSettings dialog calls. Material
// is per-rep state on the Scene/Renderer, not on the Representation
// itself, so the StageController-style "apply to rep_" pattern routes
// through Scene::getInstance(0) here.
//

#include <BALL/VIEW/KERNEL/controllers/materialController.h>


#include <BALL/VIEW/KERNEL/representation.h>
#include <BALL/VIEW/KERNEL/stage.h>
#include <BALL/VIEW/KERNEL/mainControl.h>
#include <BALL/VIEW/WIDGETS/scene.h>
#include <BALL/CONCEPT/property.h>
#include <BALL/VIEW/KERNEL/controllers/controllerApplyGuard.h>
#include <BALL/COMMON/logStream.h>

namespace BALL
{
	namespace VIEW
	{

		MaterialController::MaterialController(Representation* rep, QObject* parent)
			: QObject(parent),
				rep_(rep),
				ambient_(0.3f), diffuse_(0.7f),
				specular_(0.2f), shininess_(30.0f),
				applying_(false)
		{
			revert();
		}

		MaterialController::~MaterialController() = default;

		void MaterialController::setRepresentation(Representation* rep)
		{
			if (rep_ == rep) return;
			rep_ = rep;
			revert();
		}

		void MaterialController::revert()
		{
			// Pull per-rep material if present, else fall back to the
			// Stage's default material so the inspector mirrors what the
			// scene will actually render. Colors (ambient/specular/
			// reflective) are not part of the v1.7 controller's
			// Q_PROPERTY surface — preserved on apply() by reading
			// existing values.
			if (rep_ == nullptr) return;

			Stage::Material material;
			bool have_material = false;

			if (rep_->hasProperty("Rendering::Material"))
			{
				NamedProperty mat_property = rep_->getProperty("Rendering::Material");
				boost::shared_ptr<PersistentObject> mat_ptr = mat_property.getSmartObject();
				Stage::Material* mat_cast = dynamic_cast<Stage::Material*>(mat_ptr.get());
				if (mat_cast != nullptr)
				{
					material = *mat_cast;
					have_material = true;
				}
			}

			if (!have_material)
			{
				Scene* scene = Scene::getInstance(0);
				if (scene != nullptr && scene->getStage() != nullptr)
				{
					material = scene->getStage()->getMaterial();
					have_material = true;
				}
			}

			if (!have_material) return;

			if (ambient_ != material.ambient_intensity)
			{
				ambient_ = material.ambient_intensity;
				Q_EMIT ambientFactorChanged(ambient_);
			}
			if (diffuse_ != material.reflective_intensity)
			{
				// "Diffuse" in the controller maps to the legacy
				// dialog's "Reflectiveness" intensity — same field,
				// renamed in the inspector copy. Keep the mapping
				// consistent on both revert() and apply().
				diffuse_ = material.reflective_intensity;
				Q_EMIT diffuseFactorChanged(diffuse_);
			}
			if (specular_ != material.specular_intensity)
			{
				specular_ = material.specular_intensity;
				Q_EMIT specularFactorChanged(specular_);
			}
			if (shininess_ != material.shininess)
			{
				shininess_ = material.shininess;
				Q_EMIT shininessChanged(shininess_);
			}
		}

		void MaterialController::apply()
		{
			if (rep_ == nullptr)
			{
				Log.warn() << "[MaterialController::apply] no Representation attached — skipping." << std::endl;
				return;
			}

			MainControl* mc = MainControl::getInstance(0);
			if (mc != nullptr && mc->isBusy())
			{
				Log.info() << "[MaterialController::apply] MainControl busy — deferring." << std::endl;
				return;
			}

			Scene* scene = Scene::getInstance(0);
			if (scene == nullptr || scene->getStage() == nullptr)
			{
				Log.warn() << "[MaterialController::apply] no Scene/Stage available — skipping." << std::endl;
				return;
			}

			// v1.7.x-24 — re-entrancy shield. If a notification triggered by
			// this apply() (e.g. the scene material refresh below)
			// synchronously re-enters apply(), bail rather than re-running the
			// mutation — this is the cascade class behind the v1.7.x-13 freeze.
			// The RAII guard clears the flag on every exit path.
			if (applying_) return;
			ControllerApplyGuard apply_guard(applying_);

			// Seed from existing per-rep material (if any) so we keep
			// any color the user picked through the legacy dialog;
			// only overwrite the intensity / shininess fields that
			// the inspector controller mirrors.
			Stage::Material material;
			bool seeded = false;

			if (rep_->hasProperty("Rendering::Material"))
			{
				NamedProperty mat_property = rep_->getProperty("Rendering::Material");
				boost::shared_ptr<PersistentObject> mat_ptr = mat_property.getSmartObject();
				Stage::Material* mat_cast = dynamic_cast<Stage::Material*>(mat_ptr.get());
				if (mat_cast != nullptr)
				{
					material = *mat_cast;
					seeded = true;
				}
			}
			if (!seeded)
			{
				material = scene->getStage()->getMaterial();
			}

			material.ambient_intensity    = ambient_;
			material.reflective_intensity = diffuse_;
			material.specular_intensity   = specular_;
			material.shininess            = std::max(shininess_, 0.1f);
			// transparency stays as-is; per-rep transparency is owned
			// by the ModelController (transparency_slider on the
			// legacy modal mirrors to Representation::setTransparency).

			scene->updateMaterialForRepresentation(rep_, material);

			Q_EMIT appliedStub();
		}

		void MaterialController::setAmbientFactor(float v)
		{
			if (v == ambient_) return;
			ambient_ = v;
			Q_EMIT ambientFactorChanged(v);
		}

		void MaterialController::setDiffuseFactor(float v)
		{
			if (v == diffuse_) return;
			diffuse_ = v;
			Q_EMIT diffuseFactorChanged(v);
		}

		void MaterialController::setSpecularFactor(float v)
		{
			if (v == specular_) return;
			specular_ = v;
			Q_EMIT specularFactorChanged(v);
		}

		void MaterialController::setShininess(float v)
		{
			if (v == shininess_) return;
			shininess_ = v;
			Q_EMIT shininessChanged(v);
		}

	} // namespace VIEW
} // namespace BALL
