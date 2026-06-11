// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// BALL 2.0 — JSON (de)serialization of PropertyManager bags (K0.6.5b).
// Used by both moleculeStoreJson (per-atom) and systemJson (per-molecule
// and per-system). Free functions returning / taking nlohmann::json as
// type-erased void* so this header doesn't drag the 900 KB JSON header
// into every TU that includes it.
//
// Coverage:
//   * Named properties of scalar types BOOL, INT, UNSIGNED_INT, FLOAT,
//     DOUBLE, STRING — fully round-tripped.
//   * OBJECT / SMART_OBJECT / NONE — emitted as {"type":"OBJECT", ...}
//     with no value payload; load-side reconstructs a NONE-typed
//     placeholder so the key survives but the pointer doesn't. Logged
//     at write time so callers know to migrate stateful Object
//     properties to typed scalars.
//   * Indexed (BitVector) properties — set-bit indices serialised as
//     a sparse array.
//

#ifndef BALL_KERNEL_PROPERTYJSON_H
#define BALL_KERNEL_PROPERTYJSON_H

#ifndef BALL_COMMON_GLOBAL_H
# include <BALL/COMMON/global.h>
#endif

namespace BALL
{
	class PropertyManager;
	class ContainerHandleBase;        // v2.2 H4 commit 6.b
	class AtomHandle;                 // v2.2 H4 commit 6.b

	namespace detail {
		/** Emit a PropertyManager into an existing nlohmann::json object
				(passed as `void*` to avoid the json include).
				Layout under `out_json`:
				{
					"named":   [ {"name":"...", "type":"INT", "value":42}, ... ],
					"indexed": { "size": N, "bits": [i, j, ...] }   // optional; omitted if empty
				}
				If the PropertyManager has neither named nor any set bits,
				out_json is left as an empty object {}.

				v2.2 H4 commit 6.b note: the PropertyManager& overload
				below is the v0/dual-existence form. The ContainerHandle /
				AtomHandle overloads in the same header are the post-flip
				path; both dispatch into the same json layout for wire-
				format equivalence. At H4 commit 8 the v0 PropertyManager
				overload is REMOVED (per D-H4.6 R2 (b) -- the persistence
				API break). The wire format is unchanged (H6b owns the
				schema freeze).
		*/
		BALL_EXPORT void properties_to_json(const PropertyManager& pm, void* out_json);

		/** Populate `pm` from a json object emitted by properties_to_json.
				Existing properties on `pm` are first cleared. Unknown
				named-property type strings are silently skipped (forward-
				compat with newer minor versions). */
		BALL_EXPORT void json_to_properties(PropertyManager& pm, const void* in_json);

		/** v2.2 H4 commit 6.b (D-H4.6 R2): ContainerHandleBase overloads.
				Same wire format as the PropertyManager& path; the
				implementation dispatches through the container_back_ptr
				bridge (commit 6.a, d0772f204) so the v0 + handle paths
				produce byte-identical JSON. */
		BALL_EXPORT void properties_to_json(const ContainerHandleBase& h, void* out_json);
		BALL_EXPORT void json_to_properties(ContainerHandleBase& h, const void* in_json);

		/** v2.2 H4 commit 6.b (D-H4.6 R2): AtomHandle overloads. Same
				contract as above but for atom-row properties; dispatches
				through AtomHandle::getAtom() during dual existence. */
		BALL_EXPORT void properties_to_json(const AtomHandle& h, void* out_json);
		BALL_EXPORT void json_to_properties(AtomHandle& h, const void* in_json);
	}
} // namespace BALL

#endif // BALL_KERNEL_PROPERTYJSON_H
