// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// BALL 2.0 — v1 -> v2 persistence converter (Track B B0.2).
//

#include <BALL/KERNEL/v1ToV2JsonConverter.h>
#include <BALL/KERNEL/system.h>
#include <BALL/KERNEL/molecule.h>
#include <BALL/KERNEL/systemJson.h>
#include <BALL/CONCEPT/textPersistenceManager.h>
#include <BALL/CONCEPT/persistentObject.h>

#include <memory>

namespace BALL
{

bool convertV1BalToV2JSON(std::istream& in, std::ostream& out)
{
	// Read via TextPersistenceManager.
	TextPersistenceManager pm;
	pm.setIstream(in);
	pm.registerClass(RTTI::getStreamName<System>(),
	                 (PersistenceManager::CreateMethod)&System::createDefault);

	PersistentObject* obj = nullptr;
	pm >> obj;
	if (obj == nullptr) return false;

	System* sys = dynamic_cast<System*>(obj);
	if (sys == nullptr)
	{
		delete obj;
		return false;
	}

	// v1 persistentRead constructed atoms via `new Atom` which orphan-
	// binds them. The Composite tree side has them as children of
	// sys, but they live in the global orphan store, not sys.getStore().
	// Drive K0.4.3-style adoption manually so subsequent saveSystemJSON
	// sees consistent atom_indices into sys's own store. adoptSubtree
	// is idempotent (`src == dst` continue guard) so calling on any
	// container is safe even when some atoms are already adopted.
	for (Position mi = 0; mi < sys->countMolecules(); ++mi)
	{
		Molecule* m = sys->getMolecule(mi);
		if (m != nullptr) sys->adoptSubtree(*m);
	}
	// Also any direct System-child atoms (Composite tree root level).
	sys->adoptSubtree(*sys);

	// Write via the v2 JSON path. Pretty-print so the converted file
	// is human-inspectable by default — the converter is interactive-
	// migration tooling, not a hot-path serialiser.
	saveSystemJSON(*sys, out, /*indent=*/2);

	// NB: heap-allocated System destruction (`delete sys`) currently
	// crashes at process exit on certain K0 builds — a pre-existing
	// destruction-order issue between the per-System store, the global
	// orphan store, and the global CompiledExpressionCache. Reproduced
	// outside this converter with a trivial `new System; insert atom;
	// delete sys` sequence. Tracked separately; not introduced by B0.2.
	// For interactive migration tooling this is harmless (process is
	// exiting anyway). Workaround for library callers: keep the
	// PersistentObject* alive past the converter and let normal
	// stack-scoped cleanup happen at caller's level.
	delete sys;
	return true;
}

} // namespace BALL
