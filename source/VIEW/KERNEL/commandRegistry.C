// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// Phase 999.46 (Handover §6.1) — CommandRegistry impl. Whole TU
// gated behind BALL_UI_V2: in the OFF cell only an empty translation
// unit is produced (the header's class is still declared because
// other v2-only code includes it, but no symbols are exported).

#include <BALL/VIEW/KERNEL/commandRegistry.h>

#ifdef BALL_UI_V2

#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtCore/QRegularExpression>
#include <QtGui/QAction>

#include <algorithm>

namespace BALL
{
	namespace VIEW
	{
		CommandRegistry& CommandRegistry::instance()
		{
			static CommandRegistry s_instance;
			return s_instance;
		}

		CommandRegistry::CommandRegistry()
			: QObject(nullptr)
		{
		}

		CommandRegistry::~CommandRegistry() = default;

		void CommandRegistry::registerCommand(Command cmd)
		{
			if (cmd.id.isEmpty()) return;

			auto it = id_to_index_.find(cmd.id);
			if (it != id_to_index_.end())
			{
				commands_[it.value()] = std::move(cmd);
				return;
			}

			id_to_index_.insert(cmd.id, commands_.size());
			commands_.append(std::move(cmd));
		}

		void CommandRegistry::registerFromAction(QAction* action, const QString& category)
		{
			if (action == nullptr) return;

			Command cmd;
			cmd.action = action;
			cmd.title = action->text();
			// Strip Qt mnemonic ampersand (`&File` → `File`).
			cmd.title.remove(QLatin1Char('&'));
			cmd.category = category;
			cmd.description = action->toolTip();
			cmd.shortcut = action->shortcut();
			cmd.icon = action->icon();
			cmd.id = synthesizeId_(action, category);

			QPointer<QAction> guarded = action;
			cmd.trigger = [guarded]() {
				if (guarded) guarded->trigger();
			};
			cmd.isEnabled = [guarded]() {
				return guarded && guarded->isEnabled();
			};

			registerCommand(std::move(cmd));
		}

		void CommandRegistry::overrideCategory(QAction* action, const QString& category)
		{
			if (action == nullptr) return;

			for (Command& cmd : commands_)
			{
				if (cmd.action == action)
				{
					cmd.category = category;
					return;
				}
			}
		}

		QList<Command> CommandRegistry::all() const
		{
			return commands_;
		}

		QList<Command> CommandRegistry::recent() const
		{
			QList<Command> out;
			out.reserve(recent_ids_.size());
			for (const QString& id : recent_ids_)
			{
				auto it = id_to_index_.find(id);
				if (it != id_to_index_.end())
					out.append(commands_[it.value()]);
			}
			return out;
		}

		void CommandRegistry::noteTriggered(const QString& id)
		{
			if (id.isEmpty()) return;
			recent_ids_.removeAll(id);
			recent_ids_.prepend(id);
			while (recent_ids_.size() > 16)
				recent_ids_.removeLast();
		}

		void CommandRegistry::clearForTest()
		{
			commands_.clear();
			id_to_index_.clear();
			recent_ids_.clear();
		}

		QString CommandRegistry::synthesizeId_(QAction* action, const QString& category) const
		{
			// Build a stable id "category.token". Prefer objectName
			// when set (insertMenuEntry sets it to the action name);
			// otherwise fall back to the action text.
			QString base = action->objectName();
			if (base.isEmpty()) base = action->text();
			base.remove(QLatin1Char('&'));
			base = base.trimmed().toLower();
			base.replace(QLatin1Char(' '), QLatin1Char('-'));
			base.replace(QLatin1Char('\t'), QLatin1Char('-'));

			QString cat = category.trimmed().toLower();
			cat.remove(QLatin1Char('&'));
			cat.replace(QLatin1Char(' '), QLatin1Char('-'));
			if (cat.isEmpty()) cat = QStringLiteral("uncategorized");

			QString id = cat + QLatin1Char('.') + base;

			// Disambiguate collisions (e.g. two "Open" entries under
			// different parents) by suffixing #N.
			if (id_to_index_.contains(id))
			{
				int n = 2;
				QString candidate;
				do
				{
					candidate = id + QStringLiteral("#") + QString::number(n++);
				}
				while (id_to_index_.contains(candidate));
				id = candidate;
			}
			return id;
		}

		int CommandRegistry::score_(const QString& query, const Command& cmd) const
		{
			if (query.isEmpty()) return 1;

			const QString q = query.toLower();
			const QString title = cmd.title.toLower();
			const QString id = cmd.id.toLower();

			// Exact prefix on title is the strongest signal.
			if (title.startsWith(q)) return 1000 - title.size();
			if (id.startsWith(q))    return 800  - id.size();

			// Word-boundary match. Qt6: QRegExp removed; use QRegularExpression.
			static const QRegularExpression kWordBoundary(QStringLiteral("[\\s\\-_/.]+"));
			const QStringList words = title.split(kWordBoundary, Qt::SkipEmptyParts);
			for (const QString& w : words)
			{
				if (w.startsWith(q)) return 600 - title.size();
			}

			// Subsequence match: every character of q appears in
			// order somewhere inside title. Score grows with how
			// densely the chars cluster (shorter run = better).
			int qi = 0;
			int last_hit = -1;
			int gap_sum = 0;
			for (int i = 0; i < title.size() && qi < q.size(); ++i)
			{
				if (title[i] == q[qi])
				{
					if (last_hit >= 0) gap_sum += (i - last_hit - 1);
					last_hit = i;
					++qi;
				}
			}
			if (qi == q.size()) return 400 - gap_sum - title.size();

			return -1;
		}

		QList<Command> CommandRegistry::search(const QString& query) const
		{
			if (query.isEmpty()) return recent();

			struct Hit { int score; const Command* cmd; };
			QList<Hit> hits;
			hits.reserve(commands_.size());

			for (const Command& cmd : commands_)
			{
				int s = score_(query, cmd);
				if (s > 0) hits.append({s, &cmd});
			}

			std::sort(hits.begin(), hits.end(),
			          [](const Hit& a, const Hit& b) { return a.score > b.score; });

			QList<Command> out;
			out.reserve(hits.size());
			for (const Hit& h : hits) out.append(*h.cmd);
			return out;
		}
	}
}

#endif // BALL_UI_V2
