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
#include <BALL/VIEW/DATATYPE/colorRGBA.h>
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
				// Colors default to white, matching the MaterialSettings dialog's
				// initial color labels (materialSettings.C:30-32). revert() pulls
				// the live values when a Stage/rep material is available.
				ambient_color_(255, 255, 255),
				specular_color_(255, 255, 255),
				reflective_color_(255, 255, 255),
				transparency_(0),  // 0 = opaque, matching Representation/ModelController default.
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
			// scene will actually render. The controller now owns the
			// COMPLETE Stage::Material field set the legacy dialog wrote
			// (intensities + shininess + the three colors), so revert()
			// mirrors the colors too (999.58-01).
			//
			// rep_ == nullptr is the all-representations / default-material
			// case (the path applyDefaultMaterial() drives): fall back to
			// the Stage default material rather than bailing, so the
			// controller mirrors the live default the preferences path used.
			Stage::Material material;
			bool have_material = false;

			if (rep_ != nullptr && rep_->hasProperty("Rendering::Material"))
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

			// Mirror the three material colors (ColorRGBA -> QColor).
			QColor amb_c = material.ambient_color.getQColor();
			if (amb_c != ambient_color_)
			{
				ambient_color_ = amb_c;
				Q_EMIT ambientColorChanged(amb_c);
			}
			QColor spec_c = material.specular_color.getQColor();
			if (spec_c != specular_color_)
			{
				specular_color_ = spec_c;
				Q_EMIT specularColorChanged(spec_c);
			}
			QColor refl_c = material.reflective_color.getQColor();
			if (refl_c != reflective_color_)
			{
				reflective_color_ = refl_c;
				Q_EMIT reflectiveColorChanged(refl_c);
			}

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

			// Transparency mirrors the LIVE Representation, not the
			// Stage::Material — the interactive GLRenderer keys its
			// transparent pass off rep_->getTransparency() (0–255), so
			// that is the value we read back. Same shared state the Model
			// section reads, keeping the two controls consistent.
			if (rep_ != nullptr)
			{
				int tr = static_cast<int>(rep_->getTransparency());
				if (tr != transparency_)
				{
					transparency_ = tr;
					Q_EMIT transparencyChanged(tr);
				}
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
			// The controller now owns the three colors too (999.58-01),
			// matching the legacy MaterialSettings::apply() field set.
			material.ambient_color.set(ambient_color_);
			material.specular_color.set(specular_color_);
			material.reflective_color.set(reflective_color_);
			// material.transparency is intentionally NOT written: the
			// interactive GLRenderer never reads it (POVRay/RTfact-only,
			// and RTfact is disabled), so writing it would produce NO
			// visible change — that dead field is the cause of #527.
			// Per-rep transparency is driven below through the WORKING
			// Representation::setTransparency() path. MaterialController now
			// ALSO drives that path (in addition to the Model section),
			// sharing the same Representation transparency state so both
			// controls stay consistent.

			scene->updateMaterialForRepresentation(rep_, material);

			// Working interactive-GL transparency: push the 0–255 alpha onto
			// the Representation (rebuilds per-vertex alpha / color processor),
			// then update() so GLRenderer's transparent pass — which keys off
			// rep.getTransparency() — picks it up.
			rep_->setTransparency(static_cast<Size>(transparency_));
			// rebuild=false: transparency-only refresh rebuilds the per-vertex
			// alpha / color processor without re-walking the composites
			// (mirrors ModelController's cheap transparency path).
			rep_->update(false);

			Q_EMIT appliedStub();
		}

		void MaterialController::applyDefaultMaterial()
		{
			// 999.58-01 cut-over — the all-representations default-material
			// path the legacy MaterialSettings::apply() preferences branch
			// drove (materialSettings.C:85-90): write the mirrored material
			// fields onto the Stage default material, then refresh every
			// representation via Scene::updateAllMaterials(). This is what
			// Scene::applyPreferences() now calls in place of
			// material_settings_->apply(). No Representation is required
			// (this is the default, not a per-rep edit), so rep_ is ignored.
			MainControl* mc = MainControl::getInstance(0);
			if (mc != nullptr && mc->isBusy())
			{
				Log.info() << "[MaterialController::applyDefaultMaterial] MainControl busy — deferring." << std::endl;
				return;
			}

			Scene* scene = Scene::getInstance(0);
			if (scene == nullptr || scene->getStage() == nullptr)
			{
				Log.warn() << "[MaterialController::applyDefaultMaterial] no Scene/Stage available — skipping." << std::endl;
				return;
			}

			// v1.7.x-24 — re-entrancy shield. The updateAllMaterials() refresh
			// below can notify back into the controllers; bail rather than
			// re-running the mutation. RAII guard clears the flag on every exit.
			if (applying_) return;
			ControllerApplyGuard apply_guard(applying_);

			// Seed from the current Stage default so any field the controller
			// does not mirror is preserved, then overwrite the mirrored set —
			// exactly the Stage::Material the legacy preferences branch built.
			Stage::Material material = scene->getStage()->getMaterial();

			material.ambient_intensity    = ambient_;
			material.reflective_intensity = diffuse_;
			material.specular_intensity   = specular_;
			material.shininess            = std::max(shininess_, 0.1f);
			material.ambient_color.set(ambient_color_);
			material.specular_color.set(specular_color_);
			material.reflective_color.set(reflective_color_);
			// material.transparency intentionally NOT written — see apply()
			// (dead Stage::Material.transparency field, the cause of #527).

			// The legacy preferences branch: set the Stage default + refresh
			// ALL representations (materialSettings.C:88-89).
			scene->getStage()->getMaterial() = material;
			scene->updateAllMaterials();

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

		void MaterialController::setAmbientColor(const QColor& c)
		{
			if (c == ambient_color_) return;
			ambient_color_ = c;
			Q_EMIT ambientColorChanged(c);
		}

		void MaterialController::setSpecularColor(const QColor& c)
		{
			if (c == specular_color_) return;
			specular_color_ = c;
			Q_EMIT specularColorChanged(c);
		}

		void MaterialController::setReflectiveColor(const QColor& c)
		{
			if (c == reflective_color_) return;
			reflective_color_ = c;
			Q_EMIT reflectiveColorChanged(c);
		}

		void MaterialController::setTransparency(int v)
		{
			// 0–255 per-rep alpha; mirrors ModelController::setTransparency.
			if (v == transparency_) return;
			transparency_ = v;
			Q_EMIT transparencyChanged(v);
		}

	} // namespace VIEW
} // namespace BALL
