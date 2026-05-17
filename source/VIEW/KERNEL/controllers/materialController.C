// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// Phase 999.44 Plan 04 — MaterialController implementation.
//

#include <BALL/VIEW/KERNEL/controllers/materialController.h>


#include <BALL/VIEW/KERNEL/representation.h>
#include <BALL/COMMON/logStream.h>

namespace BALL
{
	namespace VIEW
	{

		MaterialController::MaterialController(Representation* rep, QObject* parent)
			: QObject(parent),
				rep_(rep),
				ambient_(0.3f), diffuse_(0.7f),
				specular_(0.2f), shininess_(30.0f)
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
			// Material settings live on the Representation's color
			// processor / model processor at varying levels; for the
			// read-only mirror we keep the default values populated in
			// the constructor. The cut-over plan migrates the legacy
			// MaterialSettings::apply() machinery into apply() and
			// gains read access on revert().
		}

		void MaterialController::apply()
		{
			Log.info() << "[MaterialController::apply] STUB — legacy MaterialSettings "
				"owns mutation. Mirrored ambient=" << ambient_
				<< " diffuse=" << diffuse_
				<< " specular=" << specular_
				<< " shininess=" << shininess_ << std::endl;
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

