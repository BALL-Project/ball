// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// Phase 999.47 (Handover §7.1) — WelcomeScreen widget.
//
// In-place panel (NOT a modal dialog) shown when no document is
// open under BALL_UI_V2. Whole class is gated behind BALL_UI_V2;
// mirrors the commandPalette.h whole-TU gating pattern so the OFF
// cell produces an empty translation unit (no AUTOMOC dangling
// symbols, no BALL_VIEW_EXPORT entries in the OFF VIEW DLL).

#ifndef BALL_VIEW_WIDGETS_WELCOMESCREEN_H
#define BALL_VIEW_WIDGETS_WELCOMESCREEN_H

#ifndef BALL_COMMON_GLOBAL_H
#	include <BALL/COMMON/global.h>
#endif


#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtWidgets/QWidget>

class QLabel;
class QPushButton;
class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QTimer;

namespace BALL
{
	namespace VIEW
	{

		/** Phase 999.47 §7.1 — WelcomeScreen panel.
		 *
		 * Displays the new-user / no-document state with three
		 * primary action buttons (Open file, Open from PDB/PubChem,
		 * Recent files), a "Sample structures" 6-card grid, an
		 * optional "What's new in 1.7" card surfaced after a
		 * version bump, and a "Skip this screen on startup" toggle
		 * in the footer.
		 *
		 * Lifecycle:
		 *  - Mainframe owns one WelcomeScreen and one Scene; flips
		 *    them via setCentralWidget(...) when the composite
		 *    count crosses 0 ↔ ≥1.
		 *  - WelcomeScreen never mutates document state directly;
		 *    it emits signals consumed by mainframe slots that
		 *    invoke the existing MolecularFileDialog / PubChemDialog
		 *    / recent-files plumbing.
		 */
		class BALL_VIEW_EXPORT WelcomeScreen : public QWidget
		{
			Q_OBJECT

			public:

				explicit WelcomeScreen(QWidget* parent = nullptr);
				~WelcomeScreen() override;

				/** Populate the "Recent" column from the application
				 *  QSettings file-history list. Pass an empty list to
				 *  clear. Truncates to 5 entries.
				 */
				void setRecentFiles(const QStringList& files);

				/** Show the "What's new in 1.7" card. Card auto-
				 *  dismisses after 2 minutes via internal QTimer.
				 *  Pass an absolute path to a Markdown file; renders
				 *  via QTextBrowser::setMarkdown.
				 */
				void showWhatsNew(const QString& markdownFile);

				/** True if the user toggled "skip on startup". The
				 *  caller is responsible for persisting + honouring
				 *  the preference.
				 */
				bool isSkipOnStartupChecked() const;

			Q_SIGNALS:

				/** "Open file…" primary button. */
				void openFileRequested();

				/** "Open from PDB / PubChem…" primary button. */
				void openFromPdbRequested();

				/** A recent-file row was activated. */
				void recentFileRequested(const QString& path);

				/** A sample-structure card was activated. The path is
				 *  the absolute path to the bundled file under the
				 *  BALL data dir, resolved via Path::find().
				 */
				void sampleRequested(const QString& absolutePath);

				/** The "Skip this screen on startup" toggle changed.
				 *  Mainframe persists in QSettings.
				 */
				void skipOnStartupToggled(bool skip);

			private Q_SLOTS:

				void onWhatsNewDismiss_();
				void onWhatsNewAutoDismiss_();
				void onSampleClicked_();
				void onRecentClicked_();
				void onSkipToggled_(bool checked);

			private:

				void buildHeader_(QVBoxLayout* root);
				void buildActions_(QVBoxLayout* root);
				void buildRecentColumn_(QHBoxLayout* twoCol);
				void buildSamplesStrip_(QVBoxLayout* root);
				void buildFooter_(QVBoxLayout* root);

				QLabel*       header_logo_;
				QLabel*       header_title_;
				QLabel*       header_tagline_;
				QLabel*       header_version_;

				QPushButton*  open_file_btn_;
				QPushButton*  open_pdb_btn_;
				QPushButton*  recent_btn_;

				QVBoxLayout*  recent_list_layout_;
				QLabel*       recent_empty_label_;

				QGridLayout*  samples_grid_;
				QWidget*      whats_new_card_;
				QTimer*       whats_new_auto_dismiss_;

				QWidget*      footer_;
				QPushButton*  skip_toggle_;

				QStringList   recent_files_;
		};

	} // namespace VIEW
} // namespace BALL

#endif // BALL_VIEW_WIDGETS_WELCOMESCREEN_H
