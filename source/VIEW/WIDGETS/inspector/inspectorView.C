// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// Phase 999.44 — InspectorView implementation.
//

#include <BALL/VIEW/WIDGETS/inspector/inspectorView.h>


#include <BALL/VIEW/WIDGETS/inspector/inspectorBody.h>
#include <BALL/VIEW/WIDGETS/inspector/inspectorSection.h>
#include <BALL/VIEW/WIDGETS/inspector/inspectorEmptyState.h>
#include <BALL/VIEW/WIDGETS/inspector/sections/selectionSummarySection.h>
#include <BALL/VIEW/WIDGETS/inspector/sections/propertiesSection.h>
#include <BALL/VIEW/WIDGETS/inspector/sections/quickActionsSection.h>
#include <BALL/VIEW/WIDGETS/inspector/sections/repHeaderSection.h>
#include <BALL/VIEW/WIDGETS/inspector/sections/modelSection.h>
#include <BALL/VIEW/WIDGETS/inspector/sections/coloringSection.h>
#include <BALL/VIEW/WIDGETS/inspector/sections/materialSection.h>
#include <BALL/VIEW/KERNEL/controllers/modelController.h>
#include <BALL/VIEW/KERNEL/controllers/coloringController.h>
#include <BALL/VIEW/KERNEL/controllers/materialController.h>
#include <BALL/VIEW/KERNEL/controllers/stageController.h>
#include <BALL/VIEW/KERNEL/controllers/cameraController.h>
#include <BALL/VIEW/KERNEL/controllers/lightController.h>
#include <BALL/VIEW/KERNEL/controllers/stereoController.h>
#include <BALL/VIEW/WIDGETS/inspector/sections/stageSection.h>
#include <BALL/VIEW/WIDGETS/inspector/sections/cameraSection.h>
#include <BALL/VIEW/WIDGETS/inspector/sections/lightsSection.h>
#include <BALL/VIEW/WIDGETS/inspector/sections/stereoSection.h>
#include <BALL/VIEW/WIDGETS/inspector/sections/backgroundSection.h>

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
				selection_sections_added_(false),
				rep_header_(nullptr),
				model_section_(nullptr),
				coloring_section_(nullptr),
				material_section_(nullptr),
				model_controller_(nullptr),
				coloring_controller_(nullptr),
				material_controller_(nullptr),
				representation_sections_added_(false),
				stage_controller_(nullptr),
				camera_controller_(nullptr),
				light_controller_(nullptr),
				stereo_controller_(nullptr),
				stage_section_(nullptr),
				camera_section_(nullptr),
				lights_section_(nullptr),
				stereo_section_(nullptr),
				background_section_(nullptr),
				scene_sections_added_(false)
		{
			setObjectName("inspectorView");
			// v1.7.0-rc2 UFG-09 + UFG-10 — opaque background. The
			// InspectorView is the root container holding the tab strip
			// + the QStackedWidget body. UFG-05 fixed the SectionHeader
			// glyphs but left the area BETWEEN sections (and behind the
			// QUICK ACTIONS section title + the per-tab title strip in
			// the Representation tab) transparent. The result is the
			// `QUICK AC` truncation + `REPRESENTATION` / `ENTATION`
			// ghosting reported in UFG-10, and the same family of
			// duplication ghost reported in UFG-09 around the tab strip.
			// Mirror the SectionHeader UFG-05 pattern at the root
			// container level so every paint pass clears the entire
			// Inspector area before children re-paint on top.
			setAutoFillBackground(true);

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

			// UFG-22 fix — force a full-rect repaint of the body widget
			// after a tab switch. QStackedWidget switches pages by
			// hide/show but does NOT issue a paint event on the body
			// area between sections — and on macOS Qt 6.8 + Metal
			// backing, the framebuffer keeps the previous frame's
			// pixels until something explicitly overwrites them. UFG-10
			// added `setAutoFillBackground(true)` on InspectorBody,
			// which fills the body's background on its OWN paint event,
			// but the body widget never GETS that event on a tab switch
			// because its bounding rect is fully covered by its
			// children's covered rects. Result: stale pixels from the
			// outgoing page (e.g. the Selection page's QuickActionsSection
			// header) ghost through where the incoming page's children
			// don't cover ("QUICK AC" overlapping "REPRESENTATION").
			//
			// Forcing `body_->repaint()` (synchronous, not `update()`)
			// after `setCurrentTab` guarantees the body fills its full
			// rect with QPalette::Window before the incoming page's
			// children paint on top. The QScrollArea viewport update_
			// below is belt-and-braces in case the scroll area's own
			// viewport rect needs an explicit invalidation.
			if (body_)
			{
				body_->repaint();
			}

			// UFG-12 fix — when the user switches *to* the Scene tab,
			// resync the Scene-tab Controllers from live Stage/Scene
			// state so the widgets show current values instead of stale
			// defaults left from construction. Without this hook, the
			// Background colour swatch, Ambient slider, eye/focal
			// distances etc. show whatever the controllers were last
			// asked about (often the post-construct defaults: black
			// background, ambient 0.3, eye/focal 0). The Representation
			// tab has the same potential staleness; we resync those
			// controllers too when their tab is selected.
			InspectorTabs::TabIndex tab =
				static_cast<InspectorTabs::TabIndex>(index);
			if (tab == InspectorTabs::TabIndex::Scene)
			{
				if (stage_controller_)  stage_controller_->revert();
				if (camera_controller_) camera_controller_->revert();
				if (light_controller_)  light_controller_->revert();
				if (stereo_controller_) stereo_controller_->revert();
			}
			else if (tab == InspectorTabs::TabIndex::Representation)
			{
				if (model_controller_)    model_controller_->revert();
				if (coloring_controller_) coloring_controller_->revert();
				if (material_controller_) material_controller_->revert();
			}

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

		void InspectorView::attachRepresentationTab()
		{
			// Phase 999.44 Plan 04 — construct read-only mirror
			// Controllers + sections for the Representation tab. The
			// Controllers are owned by InspectorView and re-pointed at
			// the active Representation via setActiveRepresentation().
			if (model_controller_ != nullptr) return;

			model_controller_    = new ModelController(nullptr, this);
			coloring_controller_ = new ColoringController(nullptr, this);
			material_controller_ = new MaterialController(nullptr, this);

			rep_header_       = new RepHeaderSection(body_);
			model_section_    = new ModelSection(model_controller_, body_);
			coloring_section_ = new ColoringSection(coloring_controller_, body_);
			material_section_ = new MaterialSection(material_controller_, body_);

			// The header's picker drives setActiveRepresentation.
			connect(rep_header_, &RepHeaderSection::representationPicked,
			        this, &InspectorView::setActiveRepresentation);
		}

		void InspectorView::setRepresentations(const std::list<Representation*>& reps,
		                                       Representation* active)
		{
			if (model_controller_ == nullptr) attachRepresentationTab();

			if (reps.empty())
			{
				if (representation_sections_added_)
				{
					body_->setEmptyState(InspectorTabs::TabIndex::Representation,
					                     InspectorEmptyState::forNoRepresentation(body_));
					representation_sections_added_ = false;
				}
				setActiveRepresentation(nullptr);
				return;
			}

			if (!representation_sections_added_)
			{
				addSection(InspectorTabs::TabIndex::Representation, rep_header_);
				addSection(InspectorTabs::TabIndex::Representation, model_section_);
				addSection(InspectorTabs::TabIndex::Representation, coloring_section_);
				addSection(InspectorTabs::TabIndex::Representation, material_section_);
				representation_sections_added_ = true;
			}

			rep_header_->updateRepresentations(reps, active);
			setActiveRepresentation(active);
		}

		void InspectorView::setActiveRepresentation(Representation* rep)
		{
			if (model_controller_)    model_controller_->setRepresentation(rep);
			if (coloring_controller_) coloring_controller_->setRepresentation(rep);
			if (material_controller_) material_controller_->setRepresentation(rep);
		}

		void InspectorView::attachSceneTab(Stage* stage, Scene* scene)
		{
			// Phase 999.44 Plan 05 — Scene-tab construction.
			// Idempotent: do nothing on re-attach (Controllers stay
			// pointed at the original stage/scene; cut-over plan adds
			// setStage/setScene swapping).
			if (scene_sections_added_) return;

			stage_controller_  = new StageController(stage, scene, this);
			camera_controller_ = new CameraController(stage, this);
			light_controller_  = new LightController(stage, this);
			stereo_controller_ = new StereoController(stage, this);

			stage_section_      = new StageSection(stage_controller_, body_);
			camera_section_     = new CameraSection(camera_controller_, body_);
			lights_section_     = new LightsSection(light_controller_, body_);
			stereo_section_     = new StereoSection(stereo_controller_, body_);
			background_section_ = new BackgroundSection(stage_controller_, body_);

			// Section order per Handover §04 Scene tab:
			// Camera → Lights → Stage → Stereo → Background.
			addSection(InspectorTabs::TabIndex::Scene, camera_section_);
			addSection(InspectorTabs::TabIndex::Scene, lights_section_);
			addSection(InspectorTabs::TabIndex::Scene, stage_section_);
			addSection(InspectorTabs::TabIndex::Scene, stereo_section_);
			addSection(InspectorTabs::TabIndex::Scene, background_section_);
			scene_sections_added_ = true;
		}

	} // namespace VIEW
} // namespace BALL

