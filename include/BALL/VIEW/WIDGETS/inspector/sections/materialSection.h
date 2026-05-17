// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// Phase 999.44 Plan 04 — MaterialSection: Representation-tab section
// bound to MaterialController.
//

#ifndef BALL_VIEW_WIDGETS_INSPECTOR_SECTIONS_MATERIALSECTION_H
#define BALL_VIEW_WIDGETS_INSPECTOR_SECTIONS_MATERIALSECTION_H


#ifndef BALL_COMMON_GLOBAL_H
# include <BALL/COMMON/global.h>
#endif

#include <BALL/VIEW/WIDGETS/inspector/inspectorSection.h>

#include <QtCore/QTimer>

namespace BALL
{
	namespace VIEW
	{
		class MaterialController;
		class LabeledSlider;

		class BALL_VIEW_EXPORT MaterialSection : public InspectorSection
		{
			Q_OBJECT

			public:
				explicit MaterialSection(MaterialController* controller,
				                         QWidget* parent = nullptr);
				~MaterialSection() override;

			private Q_SLOTS:
				void onAmbientChanged_(int v);
				void onDiffuseChanged_(int v);
				void onSpecularChanged_(int v);
				void onShininessChanged_(int v);
				void onControllerAmbientChanged_(float v);
				void onControllerDiffuseChanged_(float v);
				void onControllerSpecularChanged_(float v);
				void onControllerShininessChanged_(float v);
				void onDebounceFire_();

			private:
				MaterialController* controller_;
				QTimer              debounce_;
				LabeledSlider*      ambient_;
				LabeledSlider*      diffuse_;
				LabeledSlider*      specular_;
				LabeledSlider*      shininess_;
		};

	} // namespace VIEW
} // namespace BALL

#endif // BALL_VIEW_WIDGETS_INSPECTOR_SECTIONS_MATERIALSECTION_H
