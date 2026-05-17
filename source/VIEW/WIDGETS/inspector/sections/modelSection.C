// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// Phase 999.44 Plan 04 — ModelSection implementation.
//

#include <BALL/VIEW/WIDGETS/inspector/sections/modelSection.h>

#ifdef BALL_UI_V2

#include <BALL/VIEW/KERNEL/controllers/modelController.h>
#include <BALL/VIEW/WIDGETS/formRow.h>
#include <BALL/VIEW/WIDGETS/labeledSlider.h>
#include <BALL/VIEW/KERNEL/common.h>

#include <QtCore/QSignalBlocker>
#include <QtWidgets/QComboBox>
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
		}

		ModelSection::ModelSection(ModelController* controller, QWidget* parent)
			: InspectorSection(tr("Model"),
			                   QStringLiteral("Representation/Model"),
			                   parent),
				controller_(controller),
				model_type_(nullptr), drawing_mode_(nullptr),
				precision_(nullptr), transparency_(nullptr)
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
			}
		}

		ModelSection::~ModelSection() = default;

		void ModelSection::scheduleApply_() { debounce_.start(); }

		void ModelSection::onModelTypeChosen_(int idx)
		{
			if (idx < 0 || !controller_) return;
			controller_->setModelType(model_type_->itemData(idx).toInt());
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

		void ModelSection::onControllerModelTypeChanged_(int t)
		{
			int i = findIndexForData_(model_type_, t);
			if (i >= 0 && model_type_->currentIndex() != i)
			{
				QSignalBlocker b(model_type_);
				model_type_->setCurrentIndex(i);
			}
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

		void ModelSection::onDebounceFire_()
		{
			if (controller_) controller_->apply();
		}

	} // namespace VIEW
} // namespace BALL

#endif // BALL_UI_V2
