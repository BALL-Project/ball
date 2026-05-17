// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// Phase 999.44 — InspectorTabs implementation.
//

#include <BALL/VIEW/WIDGETS/inspector/inspectorTabs.h>


#include <QtCore/QObject>

namespace BALL
{
	namespace VIEW
	{

		InspectorTabs::InspectorTabs(QWidget* parent)
			: QTabBar(parent)
		{
			setObjectName("inspectorTabs");
			setExpanding(true);
			setDrawBase(false);
			setShape(QTabBar::RoundedNorth);
			setUsesScrollButtons(false);

			addTab(tr("Selection"));
			addTab(tr("Representation"));
			addTab(tr("Scene"));

			setCurrentIndex(static_cast<int>(TabIndex::Representation));
		}

		InspectorTabs::~InspectorTabs() = default;

		InspectorTabs::TabIndex InspectorTabs::currentTabIndex() const
		{
			int i = currentIndex();
			if (i < 0) i = 0;
			if (i > 2) i = 2;
			return static_cast<TabIndex>(i);
		}

		void InspectorTabs::setCurrentTabIndex(TabIndex idx)
		{
			setCurrentIndex(static_cast<int>(idx));
		}

		InspectorTabs::TabIndex InspectorTabs::tabFromIniString(const QString& s)
		{
			if (s == "Selection")     return TabIndex::Selection;
			if (s == "Scene")         return TabIndex::Scene;
			// Default + "Representation" both land here per Handover §4.1.3.
			return TabIndex::Representation;
		}

		QString InspectorTabs::iniStringFromTab(TabIndex idx)
		{
			switch (idx)
			{
				case TabIndex::Selection:      return QStringLiteral("Selection");
				case TabIndex::Representation: return QStringLiteral("Representation");
				case TabIndex::Scene:          return QStringLiteral("Scene");
			}
			return QStringLiteral("Representation");
		}

	} // namespace VIEW
} // namespace BALL

