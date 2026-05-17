// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// Phase 999.44 Plan 04 — MaterialController (read-only mirror).
//
// Source dialog: source/VIEW/DIALOGS/materialSettings.C (~280 LOC).
//

#ifndef BALL_VIEW_KERNEL_CONTROLLERS_MATERIALCONTROLLER_H
#define BALL_VIEW_KERNEL_CONTROLLERS_MATERIALCONTROLLER_H

#ifdef BALL_UI_V2

#ifndef BALL_COMMON_GLOBAL_H
# include <BALL/COMMON/global.h>
#endif

#include <QtCore/QObject>

namespace BALL
{
	namespace VIEW
	{
		class Representation;

		/**
		 * Per-Representation Material settings: ambient / diffuse /
		 * specular / shininess factors + transparency.
		 *
		 * Read-only mirror; the legacy MaterialSettings dialog still
		 * owns mutation until the cut-over plan.
		 */
		class BALL_VIEW_EXPORT MaterialController : public QObject
		{
			Q_OBJECT

			Q_PROPERTY(float ambientFactor READ ambientFactor WRITE setAmbientFactor NOTIFY ambientFactorChanged)
			Q_PROPERTY(float diffuseFactor READ diffuseFactor WRITE setDiffuseFactor NOTIFY diffuseFactorChanged)
			Q_PROPERTY(float specularFactor READ specularFactor WRITE setSpecularFactor NOTIFY specularFactorChanged)
			Q_PROPERTY(float shininess READ shininess WRITE setShininess NOTIFY shininessChanged)

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

			public Q_SLOTS:
				void apply();
				void revert();

				void setAmbientFactor(float v);
				void setDiffuseFactor(float v);
				void setSpecularFactor(float v);
				void setShininess(float v);

			Q_SIGNALS:
				void ambientFactorChanged(float v);
				void diffuseFactorChanged(float v);
				void specularFactorChanged(float v);
				void shininessChanged(float v);
				void appliedStub();

			private:
				Representation* rep_;
				float ambient_;
				float diffuse_;
				float specular_;
				float shininess_;
		};

	} // namespace VIEW
} // namespace BALL

#endif // BALL_UI_V2
#endif // BALL_VIEW_KERNEL_CONTROLLERS_MATERIALCONTROLLER_H
