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
#include <BALL/VIEW/MODELS/representationBuilder.h>
#include <BALL/VIEW/KERNEL/stage.h>
#include <BALL/VIEW/KERNEL/mainControl.h>
#include <BALL/VIEW/WIDGETS/scene.h>
#include <BALL/VIEW/DATATYPE/colorRGBA.h>
#include <BALL/CONCEPT/property.h>
#include <BALL/VIEW/KERNEL/controllers/controllerApplyGuard.h>
#include <BALL/COMMON/logStream.h>
#include <BALL/CONCEPT/timeStamp.h>

#include <sstream>

namespace BALL
{
	namespace VIEW
	{

		namespace
		{
			// Compact owner-state snapshot for the ApplyPayload before/after
			// blobs (§2 step 3). The headless-observable per-rep state is the
			// transparency (the harness OwnerSnapshot also reads only that for
			// material); the material colors live in the Rendering::Material
			// property, summarised here for the capture record.
			String snapshotMaterialState_(Representation* rep)
			{
				if (rep == nullptr) return String("material{null}");
				std::ostringstream s;
				s << "transparency=" << static_cast<int>(rep->getTransparency());
				return String(s.str());
			}
		}

		MaterialController::MaterialController(Representation* rep, QObject* parent)
			: Controller(parent),
				rep_(rep),
				ambient_(0.3f), diffuse_(0.7f),
				specular_(0.2f), shininess_(30.0f),
				// Colors default to white, matching the MaterialSettings dialog's
				// initial color labels (materialSettings.C:30-32). revert() pulls
				// the live values when a Stage/rep material is available.
				ambient_color_(255, 255, 255),
				specular_color_(255, 255, 255),
				reflective_color_(255, 255, 255),
				transparency_(0)  // 0 = opaque, matching Representation/ModelController default.
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

		void MaterialController::reset()
		{
			// §2 reset path (999.64 consumes) — single-pass re-sync from owner.
			revert();
		}

		bool MaterialController::apply()
		{
			// ── 1. Preconditions (§2 step 1) ─────────────────────────────────
			if (rep_ == nullptr)
			{
				Log.warn() << "[MaterialController::apply] no Representation attached — skipping." << std::endl;
				return false;                    // rejected — nothing to mutate
			}

			MainControl* mc = MainControl::getInstance(0);
			if (mc != nullptr && mc->isBusy())
			{
				Log.info() << "[MaterialController::apply] MainControl busy — deferring." << std::endl;
				return false;                    // rejected — busy
			}

			// NOTE (999.59-02): the prior implementation bailed when
			// Scene::getInstance(0) was null — but the per-rep material is
			// OWNER state on the Representation (the Rendering::Material
			// property), not Scene state; the Scene is only the renderer-side
			// refresh. The owner mutation is therefore Scene-INDEPENDENT and
			// runs even headless (so the 999.60 material parity fixture, which
			// has no Scene, can assert apply()==true). The Scene/renderer
			// refresh moves to invalidateDeclared_() (§2a), guarded on a live
			// Scene there.

			// ── 2. Re-entrancy shield (§2 step 2 / §4) — nest-aware drop ─────
			if (applying_depth_ > 0) return false;          // dropped — re-entry
			ControllerApplyGuard apply_guard(applying_depth_);

			// ── 3. Capture reversible intent (§2 step 3) ─────────────────────
			ApplyPayload payload;
			payload.command_id = "material.setMaterial";
			payload.before     = snapshotMaterialState_(rep_);
			payload.target     = rep_;
			payload.t_us       = PreciseTime::now().getMicroSeconds();

			// ── 4. Mutate the single owner (§2 step 4) ───────────────────────
			applyInternal_();

			payload.after = snapshotMaterialState_(rep_);

			// ── 5. Emit ONE typed event (§2 step 5) ──────────────────────────
			Q_EMIT appliedStub();

			// ── 6. Request the DECLARED invalidation (§2 step 6 / §2a) ───────
			invalidateDeclared_();               // soft refresh + Scene refresh

			// ── 7. Record reversible intent (§2 step 7) — capture only ───────
			recordIntent_(payload);

			return true;                          // mutated
		}

		bool MaterialController::applyInternal_()
		{
			// §13 cookbook step 2 — Scene-INDEPENDENT owner mutation, moved
			// out of apply(). Precondition (rep_ != nullptr) guaranteed by
			// apply(). Does NOT notify the Scene/renderer or call update();
			// that is invalidateDeclared_() (§2a).

			// Seed from existing per-rep material (if any) so we keep any color
			// the user picked through the legacy dialog; else fall back to the
			// Stage default material when a Scene exists; else struct defaults.
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
				Scene* scene = Scene::getInstance(0);
				if (scene != nullptr && scene->getStage() != nullptr)
				{
					material = scene->getStage()->getMaterial();
				}
				// else: keep Stage::Material struct defaults (headless path).
			}

			material.ambient_intensity    = ambient_;
			material.reflective_intensity = diffuse_;
			material.specular_intensity   = specular_;
			material.shininess            = std::max(shininess_, 0.1f);
			// The controller owns the three colors too (999.58-01), matching
			// the legacy MaterialSettings::apply() field set.
			material.ambient_color.set(ambient_color_);
			material.specular_color.set(specular_color_);
			material.reflective_color.set(reflective_color_);
			// material.transparency is intentionally NOT written: the
			// interactive GLRenderer never reads it (POVRay/RTfact-only, and
			// RTfact is disabled), so writing it would produce NO visible
			// change — that dead field is the cause of #527. Per-rep
			// transparency is driven through Representation::setTransparency()
			// below.

			// Store the material as the per-rep Rendering::Material property —
			// the SAME owner state Scene::updateMaterialForRepresentation
			// writes (scene.C:3483-3487). Doing it here makes the owner
			// mutation Scene-independent; the renderer notify is the §2a
			// invalidation.
			rep_->clearProperty("Rendering::Material");
			boost::shared_ptr<PersistentObject> p(new Stage::Material(material));
			NamedProperty rt_mat_property("Rendering::Material", p);
			rep_->setProperty(rt_mat_property);

			// Working interactive-GL transparency: push the 0–255 alpha onto
			// the Representation. MaterialController shares this Representation
			// transparency state with the Model section so both stay consistent.
			// §3c — Representation mutation flows through RepresentationBuilder.
			RepresentationBuilder::setTransparency(*rep_, static_cast<Size>(transparency_));

			return true;
		}

		void MaterialController::invalidateDeclared_()
		{
			// §2a Material row — SOFT refresh. A material / transparency change
			// leaves the primitive set intact; update(false) re-draws the
			// existing geometry without a display-list rebuild. When a Scene is
			// live, also notify the renderers so GLRenderer's transparent pass
			// (which keys off rep.getTransparency()) and the material cache pick
			// up the new property. Headless (no Scene), the update(false) is the
			// whole invalidation.
			if (rep_ == nullptr) return;

			Scene* scene = Scene::getInstance(0);
			if (scene != nullptr && scene->getStage() != nullptr
			    && rep_->hasProperty("Rendering::Material"))
			{
				NamedProperty mat_property = rep_->getProperty("Rendering::Material");
				boost::shared_ptr<PersistentObject> mat_ptr = mat_property.getSmartObject();
				Stage::Material* mat_cast = dynamic_cast<Stage::Material*>(mat_ptr.get());
				if (mat_cast != nullptr)
				{
					// Re-uses the existing Scene path so renderer-side material
					// caches + updateGL fire exactly as before; the property is
					// already set, so this is idempotent on the owner state.
					scene->updateMaterialForRepresentation(rep_, *mat_cast);
				}
			}

			rep_->update(false);
		}

		void MaterialController::recordIntent_(const ApplyPayload& payload)
		{
			// v1.7.4 — capture only (no UndoStack yet, §2 step 7).
			last_payload_ = payload;
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

			// v1.7.x-24 / 999.59-02 — re-entrancy shield, now nest-aware on the
			// base depth counter. The updateAllMaterials() refresh below can
			// notify back into the controllers; drop rather than re-running the
			// mutation. RAII guard decrements on every exit.
			if (applying_depth_ > 0) return;
			ControllerApplyGuard apply_guard(applying_depth_);

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
