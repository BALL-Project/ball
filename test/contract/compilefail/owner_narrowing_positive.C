// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// Phase 999.60 (v1.7.x-25) — Owner-narrowing POSITIVE compile test.
//
// ARCHITECTURE-CONTRACT.md §3 / §11c: the legal mutation surfaces
// (RepresentationBuilder friend §3c, StageMutation facade §3b) MUST
// compile. This is the companion to the negative tests — it proves the
// narrowing leaves a working legal path, not just a wall.
//
// Today this references RepresentationBuilder (already in-tree from
// 999.65), which is the legal builder/file-IO mutation surface. When
// 999.59-04 lands StageMutation, this file is extended to mutate the Stage
// through the facade as well. This file MUST compile in every phase.
//

#include <BALL/VIEW/MODELS/representationBuilder.h>

using namespace BALL;
using namespace BALL::VIEW;

// Proves the legal mutation surface (RepresentationBuilder) is reachable
// and callable from a builder TU. Taking the address of the static
// mutation entry point is a compile-only assertion that the surface
// exists with the expected signature; it does not run.
void legal_mutation_surface_compiles()
{
	// The builder's model-settings entry point is the §3c legal surface.
	using ApplyModelFn = void (*)(Representation&,
	                              const RepresentationSpec&,
	                              const ModelInformation&);
	ApplyModelFn fn = &RepresentationBuilder::applyModelSettings;
	(void)fn;
}
