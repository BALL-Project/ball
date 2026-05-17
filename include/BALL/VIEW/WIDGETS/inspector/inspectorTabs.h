// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// Phase 999.44 — InspectorTabs: 3-tab top strip (Selection /
// Representation / Scene) per Handover §Architecture.
//

#ifndef BALL_VIEW_WIDGETS_INSPECTOR_INSPECTORTABS_H
#define BALL_VIEW_WIDGETS_INSPECTOR_INSPECTORTABS_H

// Phase 999.44: gated by BALL_UI_V2 (see inspectorSection.h for the
// AUTOMOC-cell rationale).


#ifndef BALL_COMMON_GLOBAL_H
# include <BALL/COMMON/global.h>
#endif

#include <QtWidgets/QTabBar>

namespace BALL
{
	namespace VIEW
	{

		/**
		 * Three-tab top strip for the Inspector. Tab indices:
		 *   0 = Selection
		 *   1 = Representation
		 *   2 = Scene
		 *
		 * QSS objectName: `inspectorTabs`.
		 */
		class BALL_VIEW_EXPORT InspectorTabs : public QTabBar
		{
			Q_OBJECT

			public:
				enum class TabIndex : int { Selection = 0, Representation = 1, Scene = 2 };

				explicit InspectorTabs(QWidget* parent = nullptr);
				~InspectorTabs() override;

				TabIndex currentTabIndex() const;
				void setCurrentTabIndex(TabIndex idx);

				/** Map an INI string ("Selection" / "Representation" / "Scene") to TabIndex. */
				static TabIndex tabFromIniString(const QString& s);
				static QString  iniStringFromTab(TabIndex idx);
		};

	} // namespace VIEW
} // namespace BALL

#endif // BALL_VIEW_WIDGETS_INSPECTOR_INSPECTORTABS_H
