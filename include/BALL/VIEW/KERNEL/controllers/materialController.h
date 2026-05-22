// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// Phase 999.44 Plan 04 — MaterialController (read-only mirror).
//
// Source dialog: source/VIEW/DIALOGS/materialSettings.C (~280 LOC).
//

#ifndef BALL_VIEW_KERNEL_CONTROLLERS_MATERIALCONTROLLER_H
#define BALL_VIEW_KERNEL_CONTROLLERS_MATERIALCONTROLLER_H


#ifndef BALL_COMMON_GLOBAL_H
# include <BALL/COMMON/global.h>
#endif

#include <QtCore/QObject>
#include <QtGui/QColor>

namespace BALL
{
	namespace VIEW
	{
		class Representation;

		/**
		 * Material settings controller: ambient / diffuse / specular /
		 * shininess factors, the three material colors, and per-rep
		 * transparency.
		 *
		 * Two apply paths, mirroring the legacy MaterialSettings::apply()
		 * split (source/VIEW/DIALOGS/materialSettings.C:51-92):
		 *   - apply(): per-Representation path. When a Representation is
		 *     attached, writes the mirrored fields onto that rep's material
		 *     via Scene::updateMaterialForRepresentation(rep_, material) —
		 *     the dialog's "MaterialSettingsForRepresentation" branch.
		 *   - applyDefaultMaterial(): all-representations default path.
		 *     Writes the mirrored fields onto the Stage default material
		 *     (Scene::getStage()->getMaterial() = material) and refreshes
		 *     every representation via Scene::updateAllMaterials() — the
		 *     dialog's "called from preferences" branch. This is what
		 *     Scene::applyPreferences() now drives (999.58-01 cut-over)
		 *     in place of MaterialSettings::apply().
		 */
		class BALL_VIEW_EXPORT MaterialController : public QObject
		{
			Q_OBJECT

			Q_PROPERTY(float ambientFactor READ ambientFactor WRITE setAmbientFactor NOTIFY ambientFactorChanged)
			Q_PROPERTY(float diffuseFactor READ diffuseFactor WRITE setDiffuseFactor NOTIFY diffuseFactorChanged)
			Q_PROPERTY(float specularFactor READ specularFactor WRITE setSpecularFactor NOTIFY specularFactorChanged)
			Q_PROPERTY(float shininess READ shininess WRITE setShininess NOTIFY shininessChanged)
			// The three Stage::Material colors (ambient / specular / reflective).
			// Mirror members so the controller owns the COMPLETE field set the
			// legacy MaterialSettings dialog wrote — QColor here, ColorRGBA on
			// the Stage::Material (converted on apply via ColorRGBA::set()).
			Q_PROPERTY(QColor ambientColor READ ambientColor WRITE setAmbientColor NOTIFY ambientColorChanged)
			Q_PROPERTY(QColor specularColor READ specularColor WRITE setSpecularColor NOTIFY specularColorChanged)
			Q_PROPERTY(QColor reflectiveColor READ reflectiveColor WRITE setReflectiveColor NOTIFY reflectiveColorChanged)
			// 0–255 per-representation alpha (0 = opaque). Drives the WORKING
			// interactive-GL path (Representation::setTransparency), NOT the dead
			// Stage::Material.transparency field. Shared with the Model section.
			Q_PROPERTY(int transparency READ transparency WRITE setTransparency NOTIFY transparencyChanged)

			public:
				explicit MaterialController(Representation* rep = nullptr,
				                            QObject* parent = nullptr);
				~MaterialController() override;

				Representation* representation() const { return rep_; }
				void setRepresentation(Representation* rep);

				float ambientFactor() const  { return ambient_; }
				float diffuseFactor() const  { return diffuse_; }
				float specularFactor() const { return specular_; }
				float shininess() const      { return shininess_; }
				QColor ambientColor() const    { return ambient_color_; }
				QColor specularColor() const   { return specular_color_; }
				QColor reflectiveColor() const { return reflective_color_; }
				int   transparency() const   { return transparency_; }
				bool isApplying() const      { return applying_; }

			public Q_SLOTS:
				void apply();
				/**
				 * All-representations default-material apply path. Writes the
				 * mirrored material fields (colors + intensities + shininess)
				 * onto the Stage default material, then refreshes every
				 * representation via Scene::updateAllMaterials(). Mirrors the
				 * legacy MaterialSettings::apply() preferences branch
				 * (materialSettings.C:85-90). Used by Scene::applyPreferences().
				 */
				void applyDefaultMaterial();
				void revert();

				void setAmbientFactor(float v);
				void setDiffuseFactor(float v);
				void setSpecularFactor(float v);
				void setShininess(float v);
				void setAmbientColor(const QColor& c);
				void setSpecularColor(const QColor& c);
				void setReflectiveColor(const QColor& c);
				void setTransparency(int v);

			Q_SIGNALS:
				void ambientFactorChanged(float v);
				void diffuseFactorChanged(float v);
				void specularFactorChanged(float v);
				void shininessChanged(float v);
				void ambientColorChanged(const QColor& c);
				void specularColorChanged(const QColor& c);
				void reflectiveColorChanged(const QColor& c);
				void transparencyChanged(int v);
				void appliedStub();

			private:
				Representation* rep_;
				float ambient_;
				float diffuse_;
				float specular_;
				float shininess_;
				QColor ambient_color_;     // mirrors Stage::Material.ambient_color
				QColor specular_color_;    // mirrors Stage::Material.specular_color
				QColor reflective_color_;  // mirrors Stage::Material.reflective_color
				int   transparency_;  // 0–255 per-rep alpha (Representation::setTransparency path).
				bool applying_;  // v1.7.x-24 — re-entrancy shield (see ControllerApplyGuard).
		};

	} // namespace VIEW
} // namespace BALL

#endif // BALL_VIEW_KERNEL_CONTROLLERS_MATERIALCONTROLLER_H
