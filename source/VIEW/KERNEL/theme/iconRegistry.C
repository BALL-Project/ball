// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// Phase 999.42 — BALLView Refresh: IconRegistry implementation.
// See `include/BALL/VIEW/KERNEL/theme/iconRegistry.h` for design.
//

#include <BALL/VIEW/KERNEL/theme/iconRegistry.h>

#	include <BALL/VIEW/KERNEL/theme/themedIconEngine.h>

namespace BALL
{
	namespace VIEW
	{
		namespace Icons
		{

			QIcon get(const char* name)
			{
				// QIcon takes ownership of the engine pointer (Qt convention).
				// Each call returns a fresh QIcon; the per-engine pixmap cache
				// lives for the lifetime of that QIcon. Call sites typically
				// keep the QIcon on a long-lived QAction so cache hit rate is
				// high. If a caller burns through QIcons it pays a fresh-render
				// cost which is still <1 ms per icon at Lucide-subset sizes.
				return QIcon(new Theme::ThemedIconEngine(QString::fromLatin1(name)));
			}

		} // namespace Icons
	} // namespace VIEW
} // namespace BALL
