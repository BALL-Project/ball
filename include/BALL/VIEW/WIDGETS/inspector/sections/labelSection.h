// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// Phase 999.51 Plan 02 — LabelSection: Representation-tab section
// bound to LabelController.
//

#ifndef BALL_VIEW_WIDGETS_INSPECTOR_SECTIONS_LABELSECTION_H
#define BALL_VIEW_WIDGETS_INSPECTOR_SECTIONS_LABELSECTION_H


#ifndef BALL_COMMON_GLOBAL_H
# include <BALL/COMMON/global.h>
#endif

#include <BALL/VIEW/WIDGETS/inspector/inspectorSection.h>

#include <QtCore/QTimer>

class QLineEdit;
class QComboBox;

namespace BALL
{
	namespace VIEW
	{
		class LabelController;
		class LabeledSlider;

		class BALL_VIEW_EXPORT LabelSection : public InspectorSection
		{
			Q_OBJECT

			public:
				explicit LabelSection(LabelController* controller,
				                      QWidget* parent = nullptr);
				~LabelSection() override;

			private Q_SLOTS:
				void onTextChanged_(const QString& t);
				void onModeChanged_(int index);
				void onFontSizeChanged_(int v);
				void onControllerTextChanged_(const QString& t);
				void onControllerModeChanged_(int t);
				void onControllerFontSizeChanged_(int s);
				void onDebounceFire_();

			private:
				LabelController* controller_;
				QTimer           debounce_;
				QLineEdit*       text_;
				QComboBox*       mode_;
				LabeledSlider*   font_size_;
		};

	} // namespace VIEW
} // namespace BALL

#endif // BALL_VIEW_WIDGETS_INSPECTOR_SECTIONS_LABELSECTION_H
