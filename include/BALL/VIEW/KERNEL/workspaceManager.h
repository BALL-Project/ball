// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#ifndef BALL_VIEW_KERNEL_WORKSPACEMANAGER_H
#define BALL_VIEW_KERNEL_WORKSPACEMANAGER_H

// Phase 999.45 — BALLView Refresh: Workspace consolidation
// =========================================================
//
// WorkspaceManager owns the named-preset dock-layout API. Three built-in
// presets ship with the application:
//
//   - Default  — ProjectDock (left) + InspectorDock (right) +
//                BottomDrawer (bottom, collapsed). The refreshed default.
//   - Classic  — legacy 5-dock layout (MolecularControl / GeometricControl
//                left, DatasetControl top hidden, LogView / FileObserver
//                bottom). Retained for the v1.7 RC cycle; Phase 999.49
//                deletes Classic after the BALL_UI_V2 flag flip in 999.48.
//   - Focused  — single-canvas, BottomDrawer collapsed, no side docks,
//                toolbars hidden. For distraction-free work.
//
// Plus a UserDefined slot for user-saved layouts.
//
// Built-in presets are described as JSON in data/BALLView/workspaces/
// (default.layout / classic.layout / focused.layout). The JSON is a
// portable, Qt-version-agnostic description of dock area + visibility +
// size + toolbars. apply(Preset, QMainWindow*) walks the JSON and
// programmatically reconstructs the layout — this is more robust than
// committing Qt-version-bound saveState() binary blobs to the data
// tree, which would break across Qt 6.x point releases.
//
// User-defined layouts use Qt's native QMainWindow::saveState() blob
// (stored base64 in the [Workspace] INI section + a sidecar JSON for
// the display name).
//
// Persistence lives in BALLView's INI file (post-999.45-Task-6 migration
// path: ~/.config/BALLView/BALLView.ini on Linux, equivalents on
// macOS/Windows; pre-migration: ~/.BALLView). Section [Workspace]:
//   currentPreset       = Default | Classic | Focused | UserDefined
//   firstRunPromptSeen  = 1
//   userDefined.<name>  = <base64 saveState payload>
//
// All public API is safe to call with BALL_UI_V2 OFF — the class
// compiles in both branches; Mainframe only wires it in under the
// BALL_UI_V2 ifdef so OFF builds get the legacy layout for free
// (which is, by definition, the Classic preset).

#include <BALL/COMMON/global.h>

#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtCore/QStringList>

class QMainWindow;
class QLabel;
class QMouseEvent;

namespace BALL
{
	namespace VIEW
	{
		/**
		 *  Named-preset workspace dock-layout manager.
		 *
		 *  Singleton attached to QApplication. Mainframe::show() (under
		 *  BALL_UI_V2) calls WorkspaceManager::instance().apply(Default, this)
		 *  after constructing ProjectDock + BottomDrawer + (via 999.44)
		 *  InspectorDock.
		 *
		 *  @see ProjectDock, BottomDrawer, ConfigMigration
		 */
		class BALL_EXPORT WorkspaceManager : public QObject
		{
			Q_OBJECT

			public:
				/// Built-in + user-defined preset identifiers.
				enum Preset
				{
					Default = 0,
					Classic = 1,
					Focused = 2,
					UserDefined = 3
				};

				/// Returns the process-wide singleton.
				static WorkspaceManager& instance();

				/// Currently-applied preset (defaults to Default).
				Preset currentPreset() const;

				/// Set the active preset; emits presetChanged. Does NOT call apply.
				void setCurrentPreset(Preset preset);

				/// Localized display name for a preset (uses tr()).
				QString presetDisplayName(Preset preset) const;

				/**
				 *  Apply a built-in preset to the given QMainWindow.
				 *
				 *  Walks data/BALLView/workspaces/<preset>.layout, looks up
				 *  registered QDockWidget instances by objectName, and
				 *  programmatically sets their dock area + visibility +
				 *  size. Toolbars are shown/hidden per the JSON
				 *  toolbarsVisible flag.
				 *
				 *  Docks that the JSON references but aren't found in the
				 *  main window (e.g., InspectorDock during the v1.7 RC
				 *  period before 999.44 fully lands) are silently skipped
				 *  with a debug log — apply() returns true if at least the
				 *  preset descriptor was parsed.
				 *
				 *  Emits presetChanged on success.
				 *
				 *  @param preset which built-in preset to apply (UserDefined
				 *                is not valid here — use load() instead)
				 *  @param main   target QMainWindow (typically Mainframe);
				 *                must not be null
				 *  @return true if the preset descriptor was successfully
				 *          parsed and at least one dock was placed
				 */
				bool apply(Preset preset, QMainWindow* main);

				/**
				 *  Save the current QMainWindow dock state as a named
				 *  user-defined preset.
				 *
				 *  Persists QMainWindow::saveState() as a base64 string
				 *  under [Workspace] userDefined.<name> in the BALLView
				 *  INI file. The name must be non-empty; existing names
				 *  are overwritten.
				 *
				 *  @return true on success
				 */
				bool save(QMainWindow* main, const QString& name);

				/**
				 *  Load a named user-defined preset and restore the
				 *  QMainWindow dock state.
				 *
				 *  @return true if the preset existed and was applied
				 */
				bool load(QMainWindow* main, const QString& name);

				/// List of user-defined preset names available for load().
				QStringList userPresets() const;

				/**
				 *  Resolves the absolute path to a built-in preset's
				 *  .layout descriptor via BALL::Path lookup of
				 *  "BALLView/workspaces/<preset>.layout".
				 *
				 *  Returns an empty string if not found (e.g., BALL_DATA_PATH
				 *  misconfigured in a dev build).
				 */
				QString builtInPresetPath(Preset preset) const;

			Q_SIGNALS:
				/// Emitted when setCurrentPreset or a successful apply() / load() changes the preset.
				void presetChanged(BALL::VIEW::WorkspaceManager::Preset preset);

			private:
				WorkspaceManager();
				~WorkspaceManager() override;
				WorkspaceManager(const WorkspaceManager&) = delete;
				WorkspaceManager& operator=(const WorkspaceManager&) = delete;

				Preset current_preset_;
		};

		/**
		 *  Status-bar workspace label — a clickable QLabel that shows the
		 *  active preset name and opens a popup menu on click to switch
		 *  presets / save current as a user-defined preset.
		 *
		 *  Added to the right end of Mainframe's status bar under
		 *  BALL_UI_V2 (Phase 999.45 Handover §5.5).
		 */
		class BALL_EXPORT WorkspaceStatusLabel : public QObject
		{
			Q_OBJECT

			public:
				explicit WorkspaceStatusLabel(QMainWindow* mainWindow);
				~WorkspaceStatusLabel() override;

				/// The underlying QLabel — addPermanentWidget(label()) into the status bar.
				QLabel* label() const { return label_; }

			private Q_SLOTS:
				void onPresetChanged(BALL::VIEW::WorkspaceManager::Preset preset);
				void onLabelClicked();

			private:
				QMainWindow* main_window_;
				QLabel* label_;
				bool event_filter_installed_;

				// Intercept QEvent::MouseButtonPress on the QLabel to fire the popup.
				bool eventFilter(QObject* watched, QEvent* event) override;
		};

	} // namespace VIEW
} // namespace BALL

#endif // BALL_VIEW_KERNEL_WORKSPACEMANAGER_H
