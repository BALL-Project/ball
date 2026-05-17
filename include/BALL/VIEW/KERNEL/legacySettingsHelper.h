// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// Phase 999.44 — LegacySettingsHelper: table of the legacy
// Preferences stack names that need to surface under
// `Tools › Legacy Settings` during the BALLView Refresh migration
// window.
//
// The submenu itself is wired in mainframe.C (deferred to a
// follow-up plan due to parallel-agent activity in mainframe at
// the time 999.44 Plan 01 ran). This helper holds the canonical
// list + the menu-label / migration-notice strings so the
// mainframe edit is mechanical.
//

#ifndef BALL_VIEW_KERNEL_LEGACYSETTINGSHELPER_H
#define BALL_VIEW_KERNEL_LEGACYSETTINGSHELPER_H

#ifndef BALL_COMMON_GLOBAL_H
# include <BALL/COMMON/global.h>
#endif

#include <QtCore/QString>
#include <QtCore/QStringList>

namespace BALL
{
	namespace VIEW
	{

		/**
		 * Static-data helper for the BALLView Refresh `Tools › Legacy
		 * Settings` interim home (Handover §sub-PR 4.6). Lists the
		 * preferences-stack names — the strings each legacy dialog
		 * passes to `setWidgetStackName(...)` — so a mainframe loop
		 * can build the submenu mechanically.
		 *
		 * Removal pointer: Phase 999.48 §8.8 removes the entire
		 * submenu when the legacy dialogs are deleted.
		 */
		class BALL_VIEW_EXPORT LegacySettingsHelper
		{
			public:
				/**
				 * The 12 known legacy preferences-stack names inventoried
				 * at the start of Phase 999.44 (grep for setWidgetStackName
				 * across source/VIEW/DIALOGS/). Order is the submenu order
				 * — design / display / model / appearance / lighting / etc.
				 *
				 * Each entry is the EXACT string passed to
				 * `setWidgetStackName(...)`; mainframe's `Tools › Legacy
				 * Settings ▸ X` action handler should look the entry up
				 * in `Preferences::entries_` by `setWidgetStackName`
				 * value and call `Preferences::showEntry(...)`.
				 */
				static QStringList legacyStackNames();

				/**
				 * Localised display name for a stack — e.g. "Display"
				 * stays as "Display"; "Model Colors" stays as "Model
				 * Colors". Provided so future translations can adjust
				 * the submenu labels without touching the dialog code.
				 */
				static QString displayName(const QString& stackName);

				/**
				 * First-launch migration notice copy. Shown once with a
				 * QMessageBox::information() the first time BALLView
				 * starts with BALL_UI_V2 enabled (gate flag stored in
				 * [Inspector] firstRunMigrationNoticeShown=true).
				 */
				static QString migrationNoticeTitle();
				static QString migrationNoticeBody();
		};

	} // namespace VIEW
} // namespace BALL

#endif // BALL_VIEW_KERNEL_LEGACYSETTINGSHELPER_H
