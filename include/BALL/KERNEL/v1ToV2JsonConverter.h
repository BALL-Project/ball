// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// BALL 2.0 — K0.6.4 / Track B B0.2 v1 → v2 persistence converter.
//
// Reads a v1.x System persistence stream (the text format emitted by
// TextPersistenceManager) and writes the equivalent v2.0 JSON
// document via saveSystemJSON.
//
// Lives in CONCEPT-not-FORMAT for build-dependency reasons:
// TextPersistenceManager is in CONCEPT (already core), so this
// converter ships without the FORMAT module re-enable. The PDB/HIN
// reader side that the original K0.6.4 spec wanted is separate work
// — and only relevant if a downstream user has v1 binary blobs they
// want to migrate, not v1 PDB/HIN files (those already round-trip
// through any v2 PDB/HIN reader).
//

#ifndef BALL_KERNEL_V1TOV2JSONCONVERTER_H
#define BALL_KERNEL_V1TOV2JSONCONVERTER_H

#ifndef BALL_COMMON_GLOBAL_H
# include <BALL/COMMON/global.h>
#endif

#include <iosfwd>

namespace BALL
{
	/** Read a v1.x System persistence stream from `in` and write the
			equivalent v2 JSON document to `out` via saveSystemJSON.

			Returns true on success, false on read failure (input stream
			didn't yield a System*, or downstream cast failed). Any
			Exception::ParseError from the JSON writer side propagates
			to the caller.

			Ownership: the constructed System is owned by this function
			and destroyed before return.
	*/
	BALL_EXPORT bool convertV1BalToV2JSON(std::istream& in, std::ostream& out);

} // namespace BALL

#endif // BALL_KERNEL_V1TOV2JSONCONVERTER_H
