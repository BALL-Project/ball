// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// Phase 999.44 Plan 03 — QuickActionsSection: Selection-tab Inspector
// section with two quick-action buttons (Clear / Invert selection).
//

#ifndef BALL_VIEW_WIDGETS_INSPECTOR_SECTIONS_QUICKACTIONSSECTION_H
#define BALL_VIEW_WIDGETS_INSPECTOR_SECTIONS_QUICKACTIONSSECTION_H

#ifdef BALL_UI_V2

#ifndef BALL_COMMON_GLOBAL_H
# include <BALL/COMMON/global.h>
#endif

#include <BALL/VIEW/WIDGETS/inspector/inspectorSection.h>

class QPushButton;

namespace BALL
{
	namespace VIEW
	{
		class MainControl;

		/**
		 * Selection-tab section with two buttons mirroring the existing
		 * Edit menu actions: "Clear Selection" + "Invert Selection".
		 * The buttons trigger MainControl's existing slots so behaviour
		 * is identical to the menu entries.
		 */
		class BALL_VIEW_EXPORT QuickActionsSection : public InspectorSection
		{
			Q_OBJECT

			public:
				explicit QuickActionsSection(MainControl* main_control,
				                             QWidget* parent = nullptr);
				~QuickActionsSection() override;

			private Q_SLOTS:
				void onClearClicked_();
				void onInvertClicked_();

			private:
				MainControl* main_control_;
				QPushButton* clear_btn_;
				QPushButton* invert_btn_;
		};

	} // namespace VIEW
} // namespace BALL

#endif // BALL_UI_V2
#endif // BALL_VIEW_WIDGETS_INSPECTOR_SECTIONS_QUICKACTIONSSECTION_H
