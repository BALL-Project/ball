// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// Phase 999.46 (Handover §6.1) — CommandRegistry: canonical list of
// every BALLView action with a stable id, human-readable title,
// category, optional description / shortcut / icon / trigger / enabled
// predicate, used by the command palette (Cmd/Ctrl+K) and as a
// re-routable target for plugin-contributed commands.
//
// Entire implementation is gated behind BALL_UI_V2 at the source
// level. The header keeps a small compile-only surface in the OFF
// cell so callers don't need to #ifdef around includes.

#ifndef BALL_VIEW_KERNEL_COMMANDREGISTRY_H
#define BALL_VIEW_KERNEL_COMMANDREGISTRY_H

#ifndef BALL_COMMON_GLOBAL_H
#	include <BALL/COMMON/global.h>
#endif

// Whole class is BALL_UI_V2-only — gating the header (not just the
// impl) keeps AUTOMOC from generating moc symbols in the OFF cell
// that would dangling-link against an empty TU. The OFF cell sees
// no class at all, and no v2-only call-site compiles in the OFF
// cell anyway because they're all under #ifdef BALL_UI_V2 themselves.

#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtCore/QList>
#include <QtCore/QPointer>
#include <QtCore/QHash>
#include <QtGui/QIcon>
#include <QtGui/QKeySequence>

#include <functional>

class QAction;

namespace BALL
{
	namespace VIEW
	{
		/** Phase 999.46 §6.1 — single record describing one BALLView
		 *  command. Lifetime is owned by CommandRegistry; the
		 *  QAction* back-pointer is non-owning (QPointer auto-nulls
		 *  when the QAction goes away under its parent QMenu).
		 *
		 *  Fields:
		 *   - id: stable identifier ("view.fullscreen"). Used by the
		 *     palette MRU and for plugin override lookup.
		 *   - title: human-readable label ("Enter Fullscreen").
		 *   - category: top-level bucket ("View" / "Compute" / ...).
		 *     Default = parent-menu title at registration time.
		 *   - description: optional one-line help text (palette `?`
		 *     mode renders this prominently).
		 *   - shortcut / icon: mirrors QAction at registration time.
		 *   - trigger: invoked by the palette on Enter; defaults to
		 *     `action->trigger()` when synthesized via
		 *     registerFromAction().
		 *   - isEnabled: optional gating predicate; defaults to
		 *     `action->isEnabled()`.
		 */
		// Phase 999.46 / CI 26000130897 fix — Command struct is a
		// pure POD (only Qt + STL members; no virtual). Marking it
		// BALL_VIEW_EXPORT on MSVC made consumers see all special
		// members as __declspec(dllimport) and the link failed
		// because the optimizer elided the defaulted definitions
		// in libVIEW.dll. Solution: don't dllexport the POD at all
		// — let each TU instantiate its own inline implicit
		// special members (which is what Qt's QList<T> expects
		// anyway: T must be CopyConstructible + MoveConstructible
		// + Destructible at the use site, not across DLL boundary).
		// CommandRegistry (the *class* with public API) keeps its
		// BALL_VIEW_EXPORT — its methods do need to cross the DLL
		// boundary, and they're non-defaulted so the optimizer
		// can't elide.
		struct Command
		{
			QString id;
			QString title;
			QString category;
			QString description;
			QKeySequence shortcut;
			QIcon icon;
			std::function<void()> trigger;
			std::function<bool()> isEnabled;
			QPointer<QAction> action;
		};

		/** Phase 999.46 §6.1 — process-singleton registry that
		 *  every menu-registered QAction lands in (via the
		 *  insertMenuEntry hook in MainControl), plus any
		 *  plugin-contributed or palette-synthesized commands.
		 *
		 *  Thread-affinity: created lazily on the GUI thread on
		 *  first access; not safe to access from non-GUI threads.
		 */
		class BALL_VIEW_EXPORT CommandRegistry : public QObject
		{
			Q_OBJECT

			public:
				/** Lazy singleton. */
				static CommandRegistry& instance();

				/** Register a fully-described Command. Idempotent on
				 *  `id`: a second register with the same id replaces
				 *  the previous record.
				 */
				void registerCommand(Command cmd);

				/** Convenience the menu hook calls — synthesizes a
				 *  Command record from a freshly created QAction.
				 *  The category defaults to the QAction's parent
				 *  menu title (with leading `&` stripped); the
				 *  `trigger` is action->trigger() and `isEnabled` is
				 *  action->isEnabled().
				 */
				void registerFromAction(QAction* action, const QString& category);

				/** Plugin / late-binding override of the category
				 *  bucket for a previously-registered QAction
				 *  (Handover §Risks: plugins want to influence
				 *  bucketing without rerouting their own
				 *  insertMenuEntry calls).
				 */
				void overrideCategory(QAction* action, const QString& category);

				/** All registered commands, in insertion order. */
				QList<Command> all() const;

				/** Fuzzy-match against title + id. Returns ranked
				 *  results: exact-prefix > word-boundary >
				 *  subsequence. Empty query returns recent().
				 */
				QList<Command> search(const QString& query) const;

				/** MRU list (capped at 16), most recent first. */
				QList<Command> recent() const;

				/** Notify the registry that a command was triggered
				 *  (palette calls this after invoking
				 *  Command::trigger). Promotes the id to head of
				 *  the MRU list.
				 */
				void noteTriggered(const QString& id);

				/** Test hook — wipes the registry. Intended for
				 *  menuMapping_test.C; production code should never
				 *  call this.
				 */
				void clearForTest();

			private:
				CommandRegistry();
				~CommandRegistry() override;
				CommandRegistry(const CommandRegistry&) = delete;
				CommandRegistry& operator=(const CommandRegistry&) = delete;

				QString synthesizeId_(QAction* action, const QString& category) const;
				int score_(const QString& query, const Command& cmd) const;

				QList<Command> commands_;
				QHash<QString, int> id_to_index_;
				QList<QString> recent_ids_;
		};
	}
}


#endif // BALL_VIEW_KERNEL_COMMANDREGISTRY_H
