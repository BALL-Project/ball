// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// Phase 999.44 — InspectorBody implementation.
//

#include <BALL/VIEW/WIDGETS/inspector/inspectorBody.h>


#include <BALL/VIEW/WIDGETS/inspector/inspectorSection.h>

#include <QtGui/QPainter>
#include <QtGui/QPaintEvent>
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
			// v1.7.0-rc2 UFG-10 — opaque background. InspectorBody
			// hosts the per-tab QScrollArea + the InspectorSection
			// columns. The gap between the section-collapse animation
			// (InspectorSection's QPropertyAnimation on maximumHeight)
			// and the SectionHeader's UFG-05 fix leaves transient
			// transparent strips above the QUICK ACTIONS section header
			// and inside the per-tab content area — visible as the
			// `QUICK AC` truncation and `REPRESENTATION` / `ENTATION`
			// ghosts reported in UFG-10. Filling the body background
			// closes the gap so any animation in-between frames cannot
			// show stale pixels.
			setAutoFillBackground(true);
			// v1.7.0-rc5 UFG-22 — strengthen UFG-10 opaque-paint
			// contract. On macOS Qt 6.8 + Metal backing,
			// `setAutoFillBackground` alone only fills the body
			// background on its OWN paintEvent — but the body never
			// receives its own paintEvent because the QStackedWidget
			// child fully covers it, so the framebuffer keeps stale
			// pixels from outgoing tabs (e.g. Selection-tab
			// QuickActionsSection header text bleeds through onto the
			// Representation tab as "QUICK AC"). `WA_OpaquePaintEvent`
			// + custom `paintEvent` below force an explicit fillRect
			// across the entire body bounding rect on every paint pass.
			setAttribute(Qt::WA_OpaquePaintEvent, true);

			QVBoxLayout* outer = new QVBoxLayout(this);
			outer->setContentsMargins(0, 0, 0, 0);
			outer->setSpacing(0);

			stack_ = new QStackedWidget(this);
			outer->addWidget(stack_);

			for (int i = 0; i < 3; ++i)
				stack_->addWidget(makePage_(pages_[i]));
		}

		InspectorBody::~InspectorBody() = default;

		void InspectorBody::paintEvent(QPaintEvent* event)
		{
			// UFG-22 — explicit opaque fill across the entire body
			// bounding rect. With `WA_OpaquePaintEvent` set, Qt expects
			// us to fully cover the widget's rect ourselves. We fill
			// with the palette Window colour so the area BETWEEN the
			// QStackedWidget's hidden-page leftover pixels and the
			// visible-page children's covered rects gets overwritten
			// instead of inheriting whatever last painted underneath
			// (which on macOS Metal-backing is the previously-shown
			// page's section header text).
			QPainter p(this);
			p.fillRect(event->rect(), palette().color(QPalette::Window));
		}

		QWidget* InspectorBody::makePage_(TabPage& tp)
		{
			QScrollArea* scroll = new QScrollArea(this);
			scroll->setWidgetResizable(true);
			scroll->setFrameShape(QFrame::NoFrame);
			scroll->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
			// v1.7.0-rc2 UFG-10 — opaque scroll-area viewport.
			// QScrollArea defaults to a transparent viewport on macOS;
			// without this fill, the per-tab content area's 8-px outer
			// margin (set in `lay->setContentsMargins(8, 8, 8, 8)` below)
			// shows through to whatever painted last under the body and
			// leaves the QUICK ACTIONS section title ghosted as
			// "QUICK AC" across the dirty rect boundary.
			scroll->setAutoFillBackground(true);
			if (QWidget* vp = scroll->viewport()) vp->setAutoFillBackground(true);

			QWidget* contents = new QWidget(scroll);
			QVBoxLayout* lay = new QVBoxLayout(contents);
			lay->setContentsMargins(8, 8, 8, 8);
			lay->setSpacing(8);
			lay->addStretch(1);
			// v1.7.0-rc2 UFG-10 — opaque inner contents widget. Same
			// rationale as the scroll viewport fill above: the column
			// between sections must clear stale pixels on every paint.
			contents->setAutoFillBackground(true);

			scroll->setWidget(contents);

			tp.page = scroll;
			tp.layout = lay;
			tp.emptyState = nullptr;
			return scroll;
		}

		void InspectorBody::setCurrentTab(InspectorTabs::TabIndex idx)
		{
			stack_->setCurrentIndex(static_cast<int>(idx));

			// UFG-22 belt-and-braces — invalidate the incoming page's
			// QScrollArea viewport so its full-rect repaint happens on
			// the next paint cycle. InspectorView::onTabChanged_ also
			// calls `body_->repaint()` synchronously, but doing both
			// covers the case where the QScrollArea's viewport rect
			// caches its own dirty region independent of the body.
			const int i = static_cast<int>(idx);
			if (i >= 0 && i < 3 && pages_[i].page)
			{
				if (QScrollArea* sa = qobject_cast<QScrollArea*>(pages_[i].page))
				{
					if (QWidget* vp = sa->viewport()) vp->update();
				}
			}
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
			// UFG-22 (rc5 follow-up) — sections are constructed with
			// `hide()` in InspectorView so they don't float as
			// children of body_ at (0,0) before being attached to a
			// tab. Now that the section IS in a layout, show it. Qt's
			// `setParent()` during `insertWidget()` does NOT
			// auto-show, so this explicit call is required.
			section->show();
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

