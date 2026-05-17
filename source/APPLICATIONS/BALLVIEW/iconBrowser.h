// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// Phase 999.43 — BALLView Refresh: IconBrowser dev tool.
//
// Handover §02-phase-icons.md §2.7. Optional debug dialog shown
// from Tools › Developer › Icon Browser when BALL_UI_V2 is ON
// AND the build is a debug build (!NDEBUG). Renders all icons
// from the theme.qrc bundle in a grid with name + 4 state tints,
// so the design team can spot-check what every key resolves to.
//
// The whole file compiles to an empty translation unit when
// BALL_UI_V2 is OFF — see the #ifdef wrapping in iconBrowser.C —
// so the matrix's UI-V2-OFF cells link the same TU set.
//

#ifndef BALL_APPLICATIONS_BALLVIEW_ICONBROWSER_H
#define BALL_APPLICATIONS_BALLVIEW_ICONBROWSER_H

#ifdef BALL_UI_V2

#include <QtWidgets/QDialog>

class QListWidget;

namespace BALL
{
	namespace VIEW
	{
		/**
		 * Dev-only icon browser. Constructs against a static list
		 * of icon keys (kept in sync with `theme.qrc` aliases); each
		 * cell shows the icon at 32×32 plus the key as the text.
		 * Tooltip shows the four QIcon::Mode tints (Normal, Active,
		 * Selected, Disabled) as a horizontal pixmap strip.
		 *
		 * Constructor is parent-only (no exotic params) — just `new
		 * IconBrowser(parent); dlg->show();`.
		 */
		class IconBrowser : public QDialog
		{
			Q_OBJECT

			public:
				explicit IconBrowser(QWidget* parent = nullptr);
				~IconBrowser() override;

			private:
				QListWidget* grid_;

				void populate_();
		};

	} // namespace VIEW
} // namespace BALL

#endif // BALL_UI_V2

#endif // BALL_APPLICATIONS_BALLVIEW_ICONBROWSER_H
