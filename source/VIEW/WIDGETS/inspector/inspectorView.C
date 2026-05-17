// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// Phase 999.44 — InspectorView implementation.
//

#include <BALL/VIEW/WIDGETS/inspector/inspectorView.h>

#ifdef BALL_UI_V2

#include <BALL/VIEW/WIDGETS/inspector/inspectorBody.h>
#include <BALL/VIEW/WIDGETS/inspector/inspectorSection.h>
#include <BALL/VIEW/WIDGETS/inspector/inspectorEmptyState.h>
#include <BALL/VIEW/WIDGETS/inspector/sections/selectionSummarySection.h>
#include <BALL/VIEW/WIDGETS/inspector/sections/propertiesSection.h>
#include <BALL/VIEW/WIDGETS/inspector/sections/quickActionsSection.h>

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
				loading_(false),
				selection_summary_(nullptr),
				selection_properties_(nullptr),
				selection_actions_(nullptr),
				selection_sections_added_(false)
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

			// Default empty states for all three tabs. Section binding
			// (Task 5) replaces these with real sections when the model
			// has something to show.
			body_->setEmptyState(InspectorTabs::TabIndex::Selection,
			                     InspectorEmptyState::forNoSelection(body_));
			body_->setEmptyState(InspectorTabs::TabIndex::Representation,
			                     InspectorEmptyState::forNoRepresentation(body_));
			body_->setEmptyState(InspectorTabs::TabIndex::Scene,
			                     InspectorEmptyState::forNoScene(body_));
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

		void InspectorView::attachSelectionTab(MainControl* main_control)
		{
			// Phase 999.44 Plan 03 — construct the 3 Selection-tab
			// sections. They sit on the tab page initially as the empty
			// state placeholder; setSelection swaps them in on the
			// first non-empty selection.
			if (selection_summary_ == nullptr)
			{
				selection_summary_    = new SelectionSummarySection(body_);
				selection_properties_ = new PropertiesSection(body_);
				selection_actions_    = new QuickActionsSection(main_control, body_);
			}
		}

		void InspectorView::setSelection(const std::list<Composite*>& selection)
		{
			// Lazy-create on first call if the caller forgot to
			// attachSelectionTab (defensive — no MainControl* available
			// here, so QuickActionsSection won't have one and its
			// buttons will be no-ops).
			if (selection_summary_ == nullptr)
				attachSelectionTab(nullptr);

			if (selection.empty())
			{
				// Restore the empty-state placeholder. The empty state
				// owns the tab again until the next non-empty selection.
				if (selection_sections_added_)
				{
					body_->setEmptyState(InspectorTabs::TabIndex::Selection,
					                     InspectorEmptyState::forNoSelection(body_));
					selection_sections_added_ = false;
				}
				return;
			}

			// Non-empty selection: swap sections in (idempotent — addSection
			// uses Body's append path; we install once and re-use across
			// updates).
			if (!selection_sections_added_)
			{
				addSection(InspectorTabs::TabIndex::Selection, selection_summary_);
				addSection(InspectorTabs::TabIndex::Selection, selection_properties_);
				addSection(InspectorTabs::TabIndex::Selection, selection_actions_);
				selection_sections_added_ = true;
			}

			selection_summary_->updateForSelection(selection);
			selection_properties_->updateForSelection(selection);
		}

	} // namespace VIEW
} // namespace BALL

#endif // BALL_UI_V2
