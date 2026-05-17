// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// Phase 999.42 — BALLView Refresh: IconRegistry / Icons::get façade.
//
// Single entry point for icon lookups from BALLView call sites
// (replaces the older `IconLoader::instance().getIcon(...)` idiom).
//
// Behavior depends on BALL_UI_V2:
//   - ON  → construct a QIcon backed by ThemedIconEngine (per-DPR
//           state-tinted SVG renderer; see themedIconEngine.h).
//   - OFF → delegate to IconLoader::instance().getIcon(name) so the
//           legacy PNG-from-disk lookup keeps working unchanged.
//
// This way every BALLView call site can use `Icons::get("foo")`
// unconditionally — the build flag picks the implementation.
//

#ifndef BALL_VIEW_KERNEL_THEME_ICONREGISTRY_H
#define BALL_VIEW_KERNEL_THEME_ICONREGISTRY_H

#include <BALL/COMMON/global.h>

#include <QtGui/QIcon>

namespace BALL
{
	namespace VIEW
	{
		/**
		 * BALLView Refresh icon-lookup façade. Always available
		 * regardless of BALL_UI_V2 — the underlying mechanism switches
		 * but the API contract does not.
		 *
		 * Usage:
		 *   button->setIcon(BALL::VIEW::Icons::get("actions/quicksave"));
		 *
		 * Naming convention: same key namespace BALLView has always used
		 * (`actions/<name>`, `categories/<name>`, etc.). Under BALL_UI_V2
		 * the key resolves to `:/icons/<name>.svg` in theme.qrc; under the
		 * OFF path it resolves via IconLoader's BALL_DATA_PATH walk.
		 */
		namespace Icons
		{
			/**
			 * Primary entry point — return a QIcon for the given key.
			 * Cheap to call repeatedly; under BALL_UI_V2 each call
			 * returns a fresh QIcon-backed-by-ThemedIconEngine (engines
			 * carry their own cache so repeated paint calls are fast).
			 *
			 * Under the OFF path returns a copy of the IconLoader
			 * cached QIcon (already pointer-cheap inside Qt's QIcon
			 * shared-data).
			 */
			BALL_VIEW_EXPORT QIcon get(const char* name);
		} // namespace Icons
	} // namespace VIEW
} // namespace BALL

#endif // BALL_VIEW_KERNEL_THEME_ICONREGISTRY_H
