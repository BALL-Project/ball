// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// Phase 999.44 Plan 04 — ModelSection implementation.
//

#include <BALL/VIEW/WIDGETS/inspector/sections/modelSection.h>


#include <BALL/VIEW/KERNEL/controllers/modelController.h>
#include <BALL/VIEW/WIDGETS/formRow.h>
#include <BALL/VIEW/WIDGETS/labeledSlider.h>
#include <BALL/VIEW/KERNEL/common.h>

#include <QtCore/QSignalBlocker>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

namespace BALL
{
	namespace VIEW
	{

		namespace
		{
			constexpr int DEBOUNCE_MS = 100;

			void populateModelTypes_(QComboBox* c)
			{
				c->addItem(QStringLiteral("Lines"),         MODEL_LINES);
				c->addItem(QStringLiteral("Stick"),         MODEL_STICK);
				c->addItem(QStringLiteral("Ball and Stick"),MODEL_BALL_AND_STICK);
				c->addItem(QStringLiteral("VdW"),           MODEL_VDW);
				c->addItem(QStringLiteral("SE Surface"),    MODEL_SE_SURFACE);
				c->addItem(QStringLiteral("SA Surface"),    MODEL_SA_SURFACE);
				c->addItem(QStringLiteral("Backbone"),      MODEL_BACKBONE);
				c->addItem(QStringLiteral("Cartoon"),       MODEL_CARTOON);
				c->addItem(QStringLiteral("Ribbon"),        MODEL_RIBBON);
				c->addItem(QStringLiteral("HBonds"),        MODEL_HBONDS);
				c->addItem(QStringLiteral("Forces"),        MODEL_FORCES);
			}

			void populateDrawingModes_(QComboBox* c)
			{
				c->addItem(QStringLiteral("Dots"),     DRAWING_MODE_DOTS);
				c->addItem(QStringLiteral("Wireframe"),DRAWING_MODE_WIREFRAME);
				c->addItem(QStringLiteral("Solid"),    DRAWING_MODE_SOLID);
				c->addItem(QStringLiteral("Toon"),     DRAWING_MODE_TOON);
			}

			void populatePrecision_(QComboBox* c)
			{
				c->addItem(QStringLiteral("Low"),    DRAWING_PRECISION_LOW);
				c->addItem(QStringLiteral("Medium"), DRAWING_PRECISION_MEDIUM);
				c->addItem(QStringLiteral("High"),   DRAWING_PRECISION_HIGH);
				c->addItem(QStringLiteral("Ultra"),  DRAWING_PRECISION_ULTRA);
			}

			int findIndexForData_(QComboBox* c, int value)
			{
				for (int i = 0; i < c->count(); ++i)
					if (c->itemData(i).toInt() == value) return i;
				return -1;
			}

			// Radii are mapped to the int-based LabeledSlider by *100
			// (0.00–3.00 Å -> 0..300) and unmapped by /100.0f.
			LabeledSlider* makeRadiusSlider_(QWidget* parent, float init)
			{
				return new LabeledSlider(0, 300, static_cast<int>(init * 100.0f),
				                         QStringLiteral("Å"), parent);
			}
		}

		ModelSection::ModelSection(ModelController* controller, QWidget* parent)
			: InspectorSection(tr("Model"),
			                   QStringLiteral("Representation/Model"),
			                   parent),
				controller_(controller),
				model_type_(nullptr), drawing_mode_(nullptr),
				precision_(nullptr), transparency_(nullptr),
				type_options_(nullptr),
				bs_sphere_radius_(nullptr), bs_bond_radius_(nullptr),
				bs_dashed_bonds_(nullptr), stick_radius_(nullptr),
				surface_probe_radius_(nullptr), cartoon_tube_radius_(nullptr),
				page_empty_(0), page_ball_and_stick_(0), page_stick_(0),
				page_surface_(0), page_cartoon_(0)
		{
			QWidget* content = new QWidget(this);
			QVBoxLayout* col = new QVBoxLayout(content);
			col->setContentsMargins(8, 4, 8, 8);
			col->setSpacing(6);

			model_type_   = new QComboBox(content);  populateModelTypes_(model_type_);
			drawing_mode_ = new QComboBox(content);  populateDrawingModes_(drawing_mode_);
			precision_    = new QComboBox(content);  populatePrecision_(precision_);
			transparency_ = new LabeledSlider(0, 255, 0, QStringLiteral(""), content);

			col->addWidget(new FormRow(tr("Model"),       model_type_,   content));
			col->addWidget(new FormRow(tr("Drawing"),     drawing_mode_, content));
			col->addWidget(new FormRow(tr("Precision"),   precision_,    content));
			col->addWidget(new FormRow(tr("Transparency"),transparency_, content));

			// v1.7.x-16 — type-specific controls. One QStackedWidget page
			// per model-type group; an empty page for types without extra
			// options. The visible page tracks the selected model type.
			type_options_ = new QStackedWidget(content);

			// Empty page (default for types without extra options).
			QWidget* empty_page = new QWidget(type_options_);
			page_empty_ = type_options_->addWidget(empty_page);

			// Ball & Stick page.
			{
				QWidget* page = new QWidget(type_options_);
				QVBoxLayout* pcol = new QVBoxLayout(page);
				pcol->setContentsMargins(0, 0, 0, 0);
				pcol->setSpacing(6);
				bs_sphere_radius_ = makeRadiusSlider_(page,
					controller_ ? controller_->ballRadius() : 0.4f);
				bs_bond_radius_   = makeRadiusSlider_(page,
					controller_ ? controller_->ballStickBondRadius() : 0.2f);
				bs_dashed_bonds_  = new QCheckBox(page);
				bs_dashed_bonds_->setChecked(controller_ ? controller_->dashedBonds() : false);
				pcol->addWidget(new FormRow(tr("Sphere radius"), bs_sphere_radius_, page));
				pcol->addWidget(new FormRow(tr("Bond radius"),   bs_bond_radius_,   page));
				pcol->addWidget(new FormRow(tr("Dashed bonds"),  bs_dashed_bonds_,  page));
				page_ball_and_stick_ = type_options_->addWidget(page);
			}

			// Stick page.
			{
				QWidget* page = new QWidget(type_options_);
				QVBoxLayout* pcol = new QVBoxLayout(page);
				pcol->setContentsMargins(0, 0, 0, 0);
				pcol->setSpacing(6);
				stick_radius_ = makeRadiusSlider_(page,
					controller_ ? controller_->stickRadius() : 0.2f);
				pcol->addWidget(new FormRow(tr("Stick radius"), stick_radius_, page));
				page_stick_ = type_options_->addWidget(page);
			}

			// Surface page (shared by SE and SA surfaces).
			{
				QWidget* page = new QWidget(type_options_);
				QVBoxLayout* pcol = new QVBoxLayout(page);
				pcol->setContentsMargins(0, 0, 0, 0);
				pcol->setSpacing(6);
				surface_probe_radius_ = makeRadiusSlider_(page,
					controller_ ? controller_->surfaceProbeRadius() : 1.5f);
				pcol->addWidget(new FormRow(tr("Probe radius"), surface_probe_radius_, page));
				page_surface_ = type_options_->addWidget(page);
			}

			// Cartoon page.
			{
				QWidget* page = new QWidget(type_options_);
				QVBoxLayout* pcol = new QVBoxLayout(page);
				pcol->setContentsMargins(0, 0, 0, 0);
				pcol->setSpacing(6);
				cartoon_tube_radius_ = makeRadiusSlider_(page,
					controller_ ? controller_->cartoonTubeRadius() : 0.4f);
				pcol->addWidget(new FormRow(tr("Tube radius"), cartoon_tube_radius_, page));
				page_cartoon_ = type_options_->addWidget(page);
			}

			col->addWidget(type_options_);

			setContent(content);

			debounce_.setSingleShot(true);
			debounce_.setInterval(DEBOUNCE_MS);
			connect(&debounce_, &QTimer::timeout, this, &ModelSection::onDebounceFire_);

			if (controller_)
			{
				int mi = findIndexForData_(model_type_, controller_->modelType());
				if (mi >= 0) model_type_->setCurrentIndex(mi);
				int di = findIndexForData_(drawing_mode_, controller_->drawingMode());
				if (di >= 0) drawing_mode_->setCurrentIndex(di);
				int pi = findIndexForData_(precision_, controller_->drawingPrecision());
				if (pi >= 0) precision_->setCurrentIndex(pi);
				transparency_->setValue(controller_->transparency());
			}

			connect(model_type_,
			        QOverload<int>::of(&QComboBox::currentIndexChanged),
			        this, &ModelSection::onModelTypeChosen_);
			connect(drawing_mode_,
			        QOverload<int>::of(&QComboBox::currentIndexChanged),
			        this, &ModelSection::onDrawingModeChosen_);
			connect(precision_,
			        QOverload<int>::of(&QComboBox::currentIndexChanged),
			        this, &ModelSection::onPrecisionChosen_);
			connect(transparency_, &LabeledSlider::valueChanged,
			        this, &ModelSection::onTransparencyChanged_);

			connect(bs_sphere_radius_, &LabeledSlider::valueChanged,
			        this, &ModelSection::onBallRadiusChanged_);
			connect(bs_bond_radius_, &LabeledSlider::valueChanged,
			        this, &ModelSection::onBallStickBondRadiusChanged_);
			connect(bs_dashed_bonds_, &QCheckBox::toggled,
			        this, &ModelSection::onDashedBondsToggled_);
			connect(stick_radius_, &LabeledSlider::valueChanged,
			        this, &ModelSection::onStickRadiusChanged_);
			connect(surface_probe_radius_, &LabeledSlider::valueChanged,
			        this, &ModelSection::onSurfaceProbeRadiusChanged_);
			connect(cartoon_tube_radius_, &LabeledSlider::valueChanged,
			        this, &ModelSection::onCartoonTubeRadiusChanged_);

			// Show the page matching the initial model type.
			showPageForModelType_(controller_ ? controller_->modelType()
			                                   : model_type_->currentData().toInt());

			if (controller_)
			{
				connect(controller_, &ModelController::modelTypeChanged,
				        this, &ModelSection::onControllerModelTypeChanged_);
				connect(controller_, &ModelController::drawingModeChanged,
				        this, &ModelSection::onControllerDrawingModeChanged_);
				connect(controller_, &ModelController::drawingPrecisionChanged,
				        this, &ModelSection::onControllerPrecisionChanged_);
				connect(controller_, &ModelController::transparencyChanged,
				        this, &ModelSection::onControllerTransparencyChanged_);
				connect(controller_, &ModelController::ballRadiusChanged,
				        this, &ModelSection::onControllerBallRadiusChanged_);
				connect(controller_, &ModelController::ballStickBondRadiusChanged,
				        this, &ModelSection::onControllerBallStickBondRadiusChanged_);
				connect(controller_, &ModelController::dashedBondsChanged,
				        this, &ModelSection::onControllerDashedBondsChanged_);
				connect(controller_, &ModelController::stickRadiusChanged,
				        this, &ModelSection::onControllerStickRadiusChanged_);
				connect(controller_, &ModelController::surfaceProbeRadiusChanged,
				        this, &ModelSection::onControllerSurfaceProbeRadiusChanged_);
				connect(controller_, &ModelController::cartoonTubeRadiusChanged,
				        this, &ModelSection::onControllerCartoonTubeRadiusChanged_);

				// v1.7.x-18 — per-section reset.
				setResettable(true, tr("Reset Model settings to the "
				                       "representation's current values?"));
				connect(this, &InspectorSection::resetRequested, this, [this]() {
					if (controller_) { controller_->revert(); controller_->apply(); }
				});
			}
		}

		ModelSection::~ModelSection()
		{
			// v1.7-RC1 I-1 — flush any pending debounced edit before
			// destruction so a final user change isn't silently dropped
			// (e.g. user adjusts transparency then closes the dock
			// within DEBOUNCE_MS).
			if (debounce_.isActive())
			{
				debounce_.stop();
				onDebounceFire_();
			}
		}

		void ModelSection::scheduleApply_() { debounce_.start(); }

		void ModelSection::showPageForModelType_(int t)
		{
			if (!type_options_) return;
			int page = page_empty_;
			switch (t)
			{
				case MODEL_BALL_AND_STICK: page = page_ball_and_stick_; break;
				case MODEL_STICK:          page = page_stick_;          break;
				case MODEL_SE_SURFACE:
				case MODEL_SA_SURFACE:     page = page_surface_;        break;
				case MODEL_CARTOON:        page = page_cartoon_;        break;
				default:                   page = page_empty_;          break;
			}
			type_options_->setCurrentIndex(page);
		}

		void ModelSection::onModelTypeChosen_(int idx)
		{
			if (idx < 0 || !controller_) return;
			int t = model_type_->itemData(idx).toInt();
			controller_->setModelType(t);
			showPageForModelType_(t);
			scheduleApply_();
		}

		void ModelSection::onDrawingModeChosen_(int idx)
		{
			if (idx < 0 || !controller_) return;
			controller_->setDrawingMode(drawing_mode_->itemData(idx).toInt());
			scheduleApply_();
		}

		void ModelSection::onPrecisionChosen_(int idx)
		{
			if (idx < 0 || !controller_) return;
			controller_->setDrawingPrecision(precision_->itemData(idx).toInt());
			scheduleApply_();
		}

		void ModelSection::onTransparencyChanged_(int v)
		{
			if (!controller_) return;
			controller_->setTransparency(v);
			scheduleApply_();
		}

		void ModelSection::onBallRadiusChanged_(int v)
		{
			if (!controller_) return;
			controller_->setBallRadius(v / 100.0f);
			scheduleApply_();
		}

		void ModelSection::onBallStickBondRadiusChanged_(int v)
		{
			if (!controller_) return;
			controller_->setBallStickBondRadius(v / 100.0f);
			scheduleApply_();
		}

		void ModelSection::onDashedBondsToggled_(bool on)
		{
			if (!controller_) return;
			controller_->setDashedBonds(on);
			scheduleApply_();
		}

		void ModelSection::onStickRadiusChanged_(int v)
		{
			if (!controller_) return;
			controller_->setStickRadius(v / 100.0f);
			scheduleApply_();
		}

		void ModelSection::onSurfaceProbeRadiusChanged_(int v)
		{
			if (!controller_) return;
			controller_->setSurfaceProbeRadius(v / 100.0f);
			scheduleApply_();
		}

		void ModelSection::onCartoonTubeRadiusChanged_(int v)
		{
			if (!controller_) return;
			controller_->setCartoonTubeRadius(v / 100.0f);
			scheduleApply_();
		}

		void ModelSection::onControllerModelTypeChanged_(int t)
		{
			int i = findIndexForData_(model_type_, t);
			if (i >= 0 && model_type_->currentIndex() != i)
			{
				QSignalBlocker b(model_type_);
				model_type_->setCurrentIndex(i);
			}
			showPageForModelType_(t);
		}

		void ModelSection::onControllerDrawingModeChanged_(int m)
		{
			int i = findIndexForData_(drawing_mode_, m);
			if (i >= 0 && drawing_mode_->currentIndex() != i)
			{
				QSignalBlocker b(drawing_mode_);
				drawing_mode_->setCurrentIndex(i);
			}
		}

		void ModelSection::onControllerPrecisionChanged_(int p)
		{
			int i = findIndexForData_(precision_, p);
			if (i >= 0 && precision_->currentIndex() != i)
			{
				QSignalBlocker b(precision_);
				precision_->setCurrentIndex(i);
			}
		}

		void ModelSection::onControllerTransparencyChanged_(int t)
		{
			if (transparency_->value() != t)
				transparency_->setValue(t);
		}

		void ModelSection::onControllerBallRadiusChanged_(float v)
		{
			int iv = static_cast<int>(v * 100.0f);
			if (bs_sphere_radius_->value() != iv)
			{
				QSignalBlocker b(bs_sphere_radius_);
				bs_sphere_radius_->setValue(iv);
			}
		}

		void ModelSection::onControllerBallStickBondRadiusChanged_(float v)
		{
			int iv = static_cast<int>(v * 100.0f);
			if (bs_bond_radius_->value() != iv)
			{
				QSignalBlocker b(bs_bond_radius_);
				bs_bond_radius_->setValue(iv);
			}
		}

		void ModelSection::onControllerDashedBondsChanged_(bool v)
		{
			if (bs_dashed_bonds_->isChecked() != v)
			{
				QSignalBlocker b(bs_dashed_bonds_);
				bs_dashed_bonds_->setChecked(v);
			}
		}

		void ModelSection::onControllerStickRadiusChanged_(float v)
		{
			int iv = static_cast<int>(v * 100.0f);
			if (stick_radius_->value() != iv)
			{
				QSignalBlocker b(stick_radius_);
				stick_radius_->setValue(iv);
			}
		}

		void ModelSection::onControllerSurfaceProbeRadiusChanged_(float v)
		{
			int iv = static_cast<int>(v * 100.0f);
			if (surface_probe_radius_->value() != iv)
			{
				QSignalBlocker b(surface_probe_radius_);
				surface_probe_radius_->setValue(iv);
			}
		}

		void ModelSection::onControllerCartoonTubeRadiusChanged_(float v)
		{
			int iv = static_cast<int>(v * 100.0f);
			if (cartoon_tube_radius_->value() != iv)
			{
				QSignalBlocker b(cartoon_tube_radius_);
				cartoon_tube_radius_->setValue(iv);
			}
		}

		void ModelSection::onDebounceFire_()
		{
			if (controller_) controller_->apply();
		}

	} // namespace VIEW
} // namespace BALL

