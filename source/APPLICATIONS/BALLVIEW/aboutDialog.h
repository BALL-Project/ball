// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// Phase 999.43 — BALLView Refresh: hand-coded About dialog.
//
// Replaces the legacy aboutDialog.ui (deleted in this commit).
// Uses the Phase 999.43 shared widgets (SectionHeader, FormRow)
// and the Phase 999.42 Icons::get façade for the app icon.
// Authors are sourced from data/BALLView/about/authors.json
// (with a hard-coded fallback embedded in aboutDialog.C).
//

#ifndef BALL_APPLICATIONS_BALLVIEW_ABOUTDIALOG_H
#define BALL_APPLICATIONS_BALLVIEW_ABOUTDIALOG_H

#include <QtWidgets/QDialog>

namespace BALL
{
	namespace VIEW
	{
		/**
		 * "About BALLView" dialog. Fixed width 480 px, height grows
		 * with content. Designed for the Refresh design language —
		 * see Claude Design Handover §03 mockups.html "About" board.
		 *
		 * Construct, exec(), let it destruct. No persistent state.
		 */
		class AboutDialog : public QDialog
		{
			Q_OBJECT

			public:
				explicit AboutDialog(QWidget* parent = nullptr);
				~AboutDialog() override;

			private:
				/**
				 * Load author list from data/BALLView/about/authors.json
				 * via BALL::Path lookup. Returns the hardcoded fallback
				 * list when the file is missing/unparseable so the
				 * dialog stays meaningful in untrained dev-trees.
				 */
				static QStringList loadAuthors_();
		};

	} // namespace VIEW
} // namespace BALL

#endif // BALL_APPLICATIONS_BALLVIEW_ABOUTDIALOG_H
