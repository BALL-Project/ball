// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// Phase 999.44 — InspectorSection implementation.
//

#ifdef BALL_UI_V2

#include <BALL/VIEW/WIDGETS/inspector/inspectorSection.h>
#include <BALL/VIEW/WIDGETS/sectionHeader.h>

#include <QtCore/QPropertyAnimation>
#include <QtCore/QEasingCurve>
#include <QtWidgets/QVBoxLayout>

namespace BALL
{
	namespace VIEW
	{

		InspectorSection::InspectorSection(const QString& title,
		                                   const QString& stateKey,
		                                   QWidget* parent)
			: QWidget(parent),
				state_key_(stateKey),
				header_(nullptr),
				content_(nullptr),
				root_layout_(nullptr),
				anim_(nullptr),
				expanded_max_height_(0)
		{
			setObjectName("inspectorSection");
			root_layout_ = new QVBoxLayout(this);
			root_layout_->setContentsMargins(0, 0, 0, 0);
			root_layout_->setSpacing(0);

			header_ = new SectionHeader(title, this);
			root_layout_->addWidget(header_);

			connect(header_, &SectionHeader::expandedChanged,
			        this, &InspectorSection::onHeaderToggled_);
		}

		InspectorSection::~InspectorSection() = default;

		QString InspectorSection::title() const
		{
			return header_ ? header_->title() : QString();
		}

		bool InspectorSection::isExpanded() const
		{
			return header_ ? header_->isExpanded() : false;
		}

		void InspectorSection::setContent(QWidget* content)
		{
			if (!content || content_)
				return;
			content_ = content;
			content_->setParent(this);
			root_layout_->addWidget(content_);

			// Lazy-create the animation now that we have a target.
			anim_ = new QPropertyAnimation(content_, "maximumHeight", this);
			anim_->setDuration(160);
			anim_->setEasingCurve(QEasingCurve::OutCubic);

			// Cache the natural height after first show. Use the size hint
			// as the initial expanded max — Qt expands it on first paint.
			expanded_max_height_ = content_->sizeHint().height();
			if (expanded_max_height_ <= 0)
				expanded_max_height_ = 1000;  // sane fallback for an as-yet-unrendered widget
		}

		void InspectorSection::setExpanded(bool expanded)
		{
			if (header_)
				header_->setExpanded(expanded);
		}

		void InspectorSection::onHeaderToggled_(bool expanded)
		{
			if (!content_ || !anim_)
			{
				Q_EMIT expandedChanged(expanded);
				return;
			}

			// Refresh the natural height before each expand so dynamic
			// content (e.g. populated empty state) animates to the right
			// target.
			if (expanded)
				expanded_max_height_ = std::max(content_->sizeHint().height(), 1);

			anim_->stop();
			anim_->setStartValue(content_->maximumHeight());
			anim_->setEndValue(expanded ? expanded_max_height_ : 0);
			anim_->start();

			Q_EMIT expandedChanged(expanded);
		}

	} // namespace VIEW
} // namespace BALL

#endif // BALL_UI_V2
