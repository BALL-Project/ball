// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// Phase 999.44 — InspectorBody implementation.
//

#ifdef BALL_UI_V2

#include <BALL/VIEW/WIDGETS/inspector/inspectorBody.h>
#include <BALL/VIEW/WIDGETS/inspector/inspectorSection.h>

#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QScrollArea>
#include <QtWidgets/QVBoxLayout>

namespace BALL
{
	namespace VIEW
	{

		InspectorBody::InspectorBody(QWidget* parent)
			: QWidget(parent),
				stack_(nullptr)
		{
			setObjectName("inspectorBody");

			QVBoxLayout* outer = new QVBoxLayout(this);
			outer->setContentsMargins(0, 0, 0, 0);
			outer->setSpacing(0);

			stack_ = new QStackedWidget(this);
			outer->addWidget(stack_);

			for (int i = 0; i < 3; ++i)
				stack_->addWidget(makePage_(pages_[i]));
		}

		InspectorBody::~InspectorBody() = default;

		QWidget* InspectorBody::makePage_(TabPage& tp)
		{
			QScrollArea* scroll = new QScrollArea(this);
			scroll->setWidgetResizable(true);
			scroll->setFrameShape(QFrame::NoFrame);
			scroll->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

			QWidget* contents = new QWidget(scroll);
			QVBoxLayout* lay = new QVBoxLayout(contents);
			lay->setContentsMargins(8, 8, 8, 8);
			lay->setSpacing(8);
			lay->addStretch(1);

			scroll->setWidget(contents);

			tp.page = scroll;
			tp.layout = lay;
			tp.emptyState = nullptr;
			return scroll;
		}

		void InspectorBody::setCurrentTab(InspectorTabs::TabIndex idx)
		{
			stack_->setCurrentIndex(static_cast<int>(idx));
		}

		void InspectorBody::addSection(InspectorTabs::TabIndex idx, InspectorSection* section)
		{
			if (!section)
				return;
			TabPage& tp = pages_[static_cast<int>(idx)];

			// Drop an empty state if it's still in the page when a real
			// section is added.
			if (tp.emptyState)
			{
				tp.layout->removeWidget(tp.emptyState);
				tp.emptyState->deleteLater();
				tp.emptyState = nullptr;
			}

			// Insert before the trailing stretch (always the last layout item).
			int insertAt = tp.layout->count() > 0 ? tp.layout->count() - 1 : 0;
			tp.layout->insertWidget(insertAt, section);
		}

		void InspectorBody::setEmptyState(InspectorTabs::TabIndex idx, QWidget* emptyState)
		{
			TabPage& tp = pages_[static_cast<int>(idx)];

			// Drop existing sections + existing empty state.
			clearSections(idx);
			if (tp.emptyState)
			{
				tp.layout->removeWidget(tp.emptyState);
				tp.emptyState->deleteLater();
				tp.emptyState = nullptr;
			}

			if (emptyState)
			{
				tp.emptyState = emptyState;
				emptyState->setParent(tp.page);
				int insertAt = tp.layout->count() > 0 ? tp.layout->count() - 1 : 0;
				tp.layout->insertWidget(insertAt, emptyState);
			}
		}

		void InspectorBody::clearSections(InspectorTabs::TabIndex idx)
		{
			TabPage& tp = pages_[static_cast<int>(idx)];
			// Walk items; remove all InspectorSection children. Keep the
			// trailing stretch and any empty state.
			for (int i = tp.layout->count() - 1; i >= 0; --i)
			{
				QLayoutItem* item = tp.layout->itemAt(i);
				if (!item) continue;
				QWidget* w = item->widget();
				if (!w) continue;
				if (qobject_cast<InspectorSection*>(w))
				{
					tp.layout->removeWidget(w);
					w->deleteLater();
				}
			}
		}

	} // namespace VIEW
} // namespace BALL

#endif // BALL_UI_V2
