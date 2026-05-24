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
			// v1.7.0-rc2 UFG-09 — opaque background. Without this the
			// tab strip's inter-tab spacing + the trailing margin to the
			// right of the last tab passes pixels through to whatever
			// paints underneath the InspectorView. On the empty-state
			// (no document loaded) path the first paint pass races with
			// the dock's resize-to-minimum-width (280 px, see
			// inspectorDock.C), and a width-clipped "Selecti" rendering
			// from the BEFORE-resize pass remains visible underneath
			// the fully-rendered "Selection" of the AFTER-resize pass.
			// User sees `Selecti | Selection | Representation | Scene`.
			// Forcing the tab strip to fill its background covers the
			// stale pixels every time the bar repaints. QTabBar already
			// draws its own tabs opaquely via the style; we only need
			// to fill the inter-tab gaps and the right margin.
			setAutoFillBackground(true);
			// Phase 999.62 — uniform opaque-paint contract marker.
			setProperty("opaqueContainer", true);
			// v1.7.0-rc2 UFG-04 — label elision fix.
			//
			// rc1 painted the tab strip with setExpanding(true) +
			// setUsesScrollButtons(false). Under that combination Qt
			// distributes the available width equally across the 3 tabs;
			// when the InspectorDock's natural width is the rc1 default
			// (~180 px) each tab gets ~60 px and the labels collapse to
			// "Sele...", "Represent...", "Sc...". Per UFG-04 the labels
			// must render in full.
			//
			// Fix: stop expanding (so each tab fits its label), disable
			// eliding outright, and fall back to scroll buttons if the
			// dock is dragged narrower than the natural tab strip. The
			// matching minimumWidth on InspectorDock (see inspectorDock.C)
			// ensures the default-opened dock is wide enough that the
			// scroll buttons never appear in practice.
			setExpanding(false);
			setElideMode(Qt::ElideNone);
			setDrawBase(false);
			setShape(QTabBar::RoundedNorth);
			setUsesScrollButtons(true);

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

