// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// Phase 999.44 Plan 03 — PropertiesSection: Selection-tab Inspector
// section showing per-atom details when a single Atom is selected.
//

#ifndef BALL_VIEW_WIDGETS_INSPECTOR_SECTIONS_PROPERTIESSECTION_H
#define BALL_VIEW_WIDGETS_INSPECTOR_SECTIONS_PROPERTIESSECTION_H

#ifdef BALL_UI_V2

#ifndef BALL_COMMON_GLOBAL_H
# include <BALL/COMMON/global.h>
#endif

#include <BALL/VIEW/WIDGETS/inspector/inspectorSection.h>

#include <list>

class QLabel;
class QStackedWidget;

namespace BALL
{
	class Composite;

	namespace VIEW
	{

		/**
		 * Per-atom property panel. When the selection is exactly one
		 * Atom the section shows element / position / partial charge;
		 * otherwise a placeholder QLabel ("Select a single atom for
		 * details") replaces the property panel.
		 */
		class BALL_VIEW_EXPORT PropertiesSection : public InspectorSection
		{
			Q_OBJECT

			public:
				explicit PropertiesSection(QWidget* parent = nullptr);
				~PropertiesSection() override;

			public Q_SLOTS:
				void updateForSelection(const std::list<Composite*>& selection);

			private:
				QStackedWidget* stack_;
				QLabel*         placeholder_;
				QLabel*         element_label_;
				QLabel*         position_label_;
				QLabel*         charge_label_;
				QLabel*         radius_label_;
		};

	} // namespace VIEW
} // namespace BALL

#endif // BALL_UI_V2
#endif // BALL_VIEW_WIDGETS_INSPECTOR_SECTIONS_PROPERTIESSECTION_H
