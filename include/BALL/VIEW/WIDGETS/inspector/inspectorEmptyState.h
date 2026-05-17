// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// Phase 999.44 — InspectorEmptyState: centred illustrated message
// shown when an Inspector tab has nothing useful to display
// (no selection / no representation / no scene). Three preset
// constructors bake the Handover §4.1.4 copy.
//

#ifndef BALL_VIEW_WIDGETS_INSPECTOR_INSPECTOREMPTYSTATE_H
#define BALL_VIEW_WIDGETS_INSPECTOR_INSPECTOREMPTYSTATE_H

// Phase 999.44: header unconditional for AUTOMOC; implementation
// wraps body with #ifdef BALL_UI_V2.

#ifndef BALL_COMMON_GLOBAL_H
# include <BALL/COMMON/global.h>
#endif

#include <QtCore/QString>
#include <QtWidgets/QWidget>

class QLabel;

namespace BALL
{
	namespace VIEW
	{

		/**
		 * Centred illustrated empty-state widget. Three preset
		 * factories (forNoSelection / forNoRepresentation / forNoScene)
		 * bake the Handover §4.1.4 copy and icon choices so callers
		 * don't reinvent the strings.
		 *
		 * Layout: vertically centred column with [48px icon] +
		 * [16pt semibold title] + [12pt dimmed body]. The widget
		 * expands to fill its parent so the content stays centred
		 * regardless of dock height.
		 *
		 * QSS objectName: `inspectorEmptyState`.
		 */
		class BALL_VIEW_EXPORT InspectorEmptyState : public QWidget
		{
			Q_OBJECT

			public:
				/**
				 * @param iconKey  Icon registry key (e.g. "actions/help-about");
				 *   if the icon registry can't resolve it the empty state
				 *   simply has no icon (no error).
				 * @param title    Section title — bold 16pt.
				 * @param body     Body line — 12pt dimmed.
				 * @param parent   Parent widget.
				 */
				InspectorEmptyState(const QString& iconKey,
				                    const QString& title,
				                    const QString& body,
				                    QWidget* parent = nullptr);
				~InspectorEmptyState() override;

				static InspectorEmptyState* forNoSelection(QWidget* parent = nullptr);
				static InspectorEmptyState* forNoRepresentation(QWidget* parent = nullptr);
				static InspectorEmptyState* forNoScene(QWidget* parent = nullptr);

			private:
				QLabel* icon_;
				QLabel* title_;
				QLabel* body_;
		};

	} // namespace VIEW
} // namespace BALL

#endif // BALL_VIEW_WIDGETS_INSPECTOR_INSPECTOREMPTYSTATE_H
