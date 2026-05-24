// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// Phase 999.60 (v1.7.x-25) — Contract-test harness implementation.
//
// See contractTestHarness.h for the design rationale and the harness-first
// sequencing note (Codex MEDIUM #7).
//

#include "contractTestHarness.h"

#include <BALLTestConfig.h>   // BALL_TEST_DATA_PATH (sample location)

#include <BALL/KERNEL/system.h>
#include <BALL/KERNEL/atom.h>
#include <BALL/KERNEL/atomIterator.h>
#include <BALL/FORMAT/PDBFile.h>

#include <BALL/VIEW/KERNEL/stage.h>
#include <BALL/VIEW/KERNEL/representation.h>
#include <BALL/VIEW/RENDERING/camera.h>
#include <BALL/VIEW/DATATYPE/colorRGBA.h>

#include <sstream>
#include <iomanip>

using namespace BALL;

namespace BALL
{
	namespace VIEW
	{
		namespace Contract
		{

			const char* surfaceName(SurfaceKind kind)
			{
				switch (kind)
				{
					case SurfaceKind::Inspector: return "Inspector";
					case SurfaceKind::Toolbar:   return "Toolbar";
					case SurfaceKind::Menu:      return "Menu";
					case SurfaceKind::Python:    return "Python";
				}
				return "Unknown";
			}

			bool surfaceAvailable(SurfaceKind kind)
			{
				switch (kind)
				{
					case SurfaceKind::Inspector:
						// Controllers exist in-tree — the Inspector binding
						// path is always available.
						return true;

					case SurfaceKind::Toolbar:
					case SurfaceKind::Menu:
						// The §8 command-body migration (999.59-03) has not
						// landed and CommandRegistry is compiled out today
						// (BALL_UI_V2 was removed). The toolbar/menu helpers
						// MUST invoke the registered command body, not
						// action->trigger() indirection (plan acceptance
						// criterion), so until that body exists these
						// surfaces are unavailable.
						return false;

					case SurfaceKind::Python:
						// controllers.sip (999.59-06) drives this surface
						// when BALL_HAS_PYBALL is ON. PyBALL is disabled in
						// the modernization build (CLAUDE.md), so this is
						// unavailable today.
#if defined(BALL_HAS_PYBALL) && defined(BALL_VIEW_HAS_CONTROLLER_SIP)
						return true;
#else
						return false;
#endif
				}
				return false;
			}

			ContractTestHarness::ContractTestHarness()
				: stage_(nullptr),
				  representation_(nullptr),
				  system_(nullptr),
				  sample_loaded_(false)
			{
			}

			ContractTestHarness::~ContractTestHarness()
			{
				tearDown();
			}

			void ContractTestHarness::setUp()
			{
				tearDown();

				// The headless owners the Controllers mutate (§1). No
				// Mainframe/MainControl — see the header rationale.
				stage_          = new Stage();
				representation_ = new Representation();

				// Best-effort load of a CI-stable sample for the selection
				// surface. If BALL_DATA_PATH / the test data is unavailable
				// (some headless runners) the harness still works for the
				// Stage/Representation postcondition assertions; only the
				// selection-consumer surface needs the system.
				system_ = new System();
				try
				{
					PDBFile pdb(BALL_TEST_DATA_PATH(1BNA.pdb));
					// File::isValid() — qualified to avoid the ambiguous-name
					// lookup against PropertyManager::isValid() (PDBFile inherits
					// both File and a PropertyManager-bearing base).
					if (pdb.File::isValid())
					{
						pdb >> *system_;
						pdb.close();
						sample_loaded_ = (system_->countAtoms() > 0);
					}
				}
				catch (...)
				{
					// Sample load is best-effort; a missing sample is not a
					// harness failure (the fixtures that need it skip).
					sample_loaded_ = false;
				}
			}

			void ContractTestHarness::tearDown()
			{
				delete representation_;
				representation_ = nullptr;
				delete stage_;
				stage_ = nullptr;
				delete system_;
				system_ = nullptr;
				sample_loaded_ = false;
			}

			OwnerSnapshot ContractTestHarness::snapshotOwner() const
			{
				OwnerSnapshot snap;

				// ---- Stage scene state -----------------------------------
				if (stage_ != nullptr)
				{
					std::ostringstream s;
					s << std::fixed << std::setprecision(6);

					const ColorRGBA& bg = stage_->getBackgroundColor();
					s << "bg=" << static_cast<float>(bg.getRed())
					  << ',' << static_cast<float>(bg.getGreen())
					  << ',' << static_cast<float>(bg.getBlue()) << ';';
					s << "fog=" << stage_->getFogIntensity() << ';';
					s << "eye=" << stage_->getEyeDistance() << ';';
					s << "focal=" << stage_->getFocalDistance() << ';';
					s << "coord=" << (stage_->coordinateSystemEnabled() ? 1 : 0) << ';';
					s << "swap=" << (stage_->swapSideBySideStereo() ? 1 : 0) << ';';

					const Camera& cam = stage_->getCamera();
					const Vector3& vp = cam.getViewPoint();
					const Vector3& la = cam.getLookAtPosition();
					s << "cam=" << vp.x << ',' << vp.y << ',' << vp.z
					  << '/' << la.x << ',' << la.y << ',' << la.z << ';';

					snap.stage_blob = s.str();
				}

				// ---- Representation per-rep state -------------------------
				if (representation_ != nullptr)
				{
					std::ostringstream r;
					r << "model=" << static_cast<int>(representation_->getModelType()) << ';';
					r << "coloring=" << static_cast<int>(representation_->getColoringMethod()) << ';';
					r << "mode=" << static_cast<int>(representation_->getDrawingMode()) << ';';
					r << "precision=" << static_cast<int>(representation_->getDrawingPrecision()) << ';';
					r << "transparency=" << static_cast<int>(representation_->getTransparency()) << ';';
					snap.representation_blob = r.str();
				}

				// ---- Composite selection set ------------------------------
				// Canonicalise the selected-atom count from the loaded
				// system. A future 999.59 cut-over extends this to the full
				// four-consumer selection set (§7); the count is the stable
				// scalar the harness compares today.
				if (system_ != nullptr)
				{
					std::ostringstream sel;
					Size selected = 0;
					for (AtomIterator it = system_->beginAtom(); +it; ++it)
					{
						if (it->isSelected()) ++selected;
					}
					sel << "selected=" << selected;
					snap.selection_blob = sel.str();
				}

				return snap;
			}

		} // namespace Contract
	} // namespace VIEW
} // namespace BALL
