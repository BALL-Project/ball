// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// Phase 999.44 Plan 04 — RepHeaderSection: Representation-tab header
// showing the current Representation + a picker to switch between
// representations on the active selection.
//

#ifndef BALL_VIEW_WIDGETS_INSPECTOR_SECTIONS_REPHEADERSECTION_H
#define BALL_VIEW_WIDGETS_INSPECTOR_SECTIONS_REPHEADERSECTION_H

#ifdef BALL_UI_V2

#ifndef BALL_COMMON_GLOBAL_H
# include <BALL/COMMON/global.h>
#endif

#include <BALL/VIEW/WIDGETS/inspector/inspectorSection.h>

#include <list>

class QComboBox;
class QLabel;

namespace BALL
{
	namespace VIEW
	{
		class Representation;

		/**
		 * Representation-tab header: name of the current
		 * Representation + a QComboBox picker that lets the user
		 * switch which Representation the rest of the tab edits.
		 * Emits representationPicked when the user changes the
		 * combo selection.
		 */
		class BALL_VIEW_EXPORT RepHeaderSection : public InspectorSection
		{
			Q_OBJECT

			public:
				explicit RepHeaderSection(QWidget* parent = nullptr);
				~RepHeaderSection() override;

			public Q_SLOTS:
				/**
				 * Refresh the combo with the given list of
				 * Representations and select @p active if non-null.
				 */
				void updateRepresentations(const std::list<Representation*>& reps,
				                           Representation* active);

			Q_SIGNALS:
				/** Emitted when the user picks a different Representation. */
				void representationPicked(Representation* rep);

			private Q_SLOTS:
				void onComboChanged_(int index);

			private:
				QComboBox* combo_;
				QLabel*    summary_;
				std::list<Representation*> last_reps_;
		};

	} // namespace VIEW
} // namespace BALL

#endif // BALL_UI_V2
#endif // BALL_VIEW_WIDGETS_INSPECTOR_SECTIONS_REPHEADERSECTION_H
