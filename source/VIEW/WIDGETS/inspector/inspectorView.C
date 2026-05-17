// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// Phase 999.44 — InspectorView implementation.
//

#ifdef BALL_UI_V2

#include <BALL/VIEW/WIDGETS/inspector/inspectorView.h>
#include <BALL/VIEW/WIDGETS/inspector/inspectorBody.h>
#include <BALL/VIEW/WIDGETS/inspector/inspectorSection.h>

#include <QtCore/QSettings>
#include <QtCore/QStandardPaths>
#include <QtCore/QDir>
#include <QtWidgets/QVBoxLayout>

namespace BALL
{
	namespace VIEW
	{

		namespace
		{
			// INI file path — matches MainControl(".BALLView") convention
			// established in mainframe.C. We use QSettings with explicit
			// IniFormat at $HOME/.BALLView so we share the same file the
			// legacy Preferences system writes to.
			QString iniFilePath_()
			{
				return QDir::homePath() + QStringLiteral("/.BALLView");
			}

			QSettings makeSettings_()
			{
				return QSettings(iniFilePath_(), QSettings::IniFormat);
			}
		}

		InspectorView::InspectorView(QWidget* parent)
			: QWidget(parent),
				tabs_(nullptr),
				body_(nullptr),
				loading_(false)
		{
			setObjectName("inspectorView");

			QVBoxLayout* root = new QVBoxLayout(this);
			root->setContentsMargins(0, 0, 0, 0);
			root->setSpacing(0);

			tabs_ = new InspectorTabs(this);
			body_ = new InspectorBody(this);

			root->addWidget(tabs_);
			root->addWidget(body_, 1);

			connect(tabs_, &InspectorTabs::currentChanged,
			        this, &InspectorView::onTabChanged_);

			state_writer_.setSingleShot(true);
			state_writer_.setInterval(250);
			connect(&state_writer_, &QTimer::timeout,
			        this, &InspectorView::flushStateWrite_);

			// Initial sync of tab → body.
			body_->setCurrentTab(tabs_->currentTabIndex());
		}

		InspectorView::~InspectorView()
		{
			// Flush any pending state write so we don't lose the last
			// change on app exit.
			if (state_writer_.isActive())
			{
				state_writer_.stop();
				flushStateWrite_();
			}
		}

		void InspectorView::addSection(InspectorTabs::TabIndex idx, InspectorSection* section)
		{
			if (!section)
				return;
			body_->addSection(idx, section);
			connect(section, &InspectorSection::expandedChanged,
			        this, &InspectorView::onSectionToggled_);

			// Restore persisted expanded state if any.
			QSettings s = makeSettings_();
			s.beginGroup(QStringLiteral("Inspector"));
			QString key = section->stateKey();
			if (s.contains(key))
			{
				QString v = s.value(key).toString();
				bool expanded = (v.compare(QStringLiteral("expanded"), Qt::CaseInsensitive) == 0);
				section->setExpanded(expanded);
			}
			s.endGroup();
		}

		void InspectorView::setEmptyState(InspectorTabs::TabIndex idx, QWidget* emptyState)
		{
			body_->setEmptyState(idx, emptyState);
		}

		void InspectorView::loadState()
		{
			loading_ = true;
			QSettings s = makeSettings_();
			s.beginGroup(QStringLiteral("Inspector"));

			QString tabStr = s.value(QStringLiteral("tab"),
			                          QStringLiteral("Representation")).toString();
			tabs_->setCurrentTabIndex(InspectorTabs::tabFromIniString(tabStr));

			// Per-section state is applied lazily as sections are added via
			// addSection() — see the QSettings read in that method.

			s.endGroup();
			loading_ = false;
		}

		void InspectorView::scheduleStateWrite()
		{
			if (loading_)
				return;
			state_writer_.start();
		}

		void InspectorView::onTabChanged_(int index)
		{
			if (index < 0 || index > 2) return;
			body_->setCurrentTab(static_cast<InspectorTabs::TabIndex>(index));
			scheduleStateWrite();
		}

		void InspectorView::onSectionToggled_(bool /*expanded*/)
		{
			scheduleStateWrite();
		}

		void InspectorView::flushStateWrite_()
		{
			QSettings s = makeSettings_();
			s.beginGroup(QStringLiteral("Inspector"));
			s.setValue(QStringLiteral("tab"),
			           InspectorTabs::iniStringFromTab(tabs_->currentTabIndex()));

			// Iterate over all InspectorSection children of body_ and
			// persist their expanded/collapsed state by their stateKey().
			const auto sections = body_->findChildren<InspectorSection*>();
			for (InspectorSection* sec : sections)
			{
				s.setValue(sec->stateKey(),
				           sec->isExpanded() ? QStringLiteral("expanded")
				                             : QStringLiteral("collapsed"));
			}
			s.endGroup();
			s.sync();
		}

	} // namespace VIEW
} // namespace BALL

#endif // BALL_UI_V2
