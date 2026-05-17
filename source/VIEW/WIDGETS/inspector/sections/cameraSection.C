// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// Phase 999.44 Plan 05 — CameraSection implementation.
//

#include <BALL/VIEW/WIDGETS/inspector/sections/cameraSection.h>


#include <BALL/VIEW/KERNEL/controllers/cameraController.h>
#include <BALL/VIEW/WIDGETS/formRow.h>

#include <QtGui/QVector3D>
#include <QtWidgets/QLabel>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

namespace BALL
{
	namespace VIEW
	{

		namespace
		{
			QString fmt_(const QVector3D& v)
			{
				return QStringLiteral("(%1, %2, %3)")
					.arg(v.x(), 0, 'f', 3)
					.arg(v.y(), 0, 'f', 3)
					.arg(v.z(), 0, 'f', 3);
			}
		}

		CameraSection::CameraSection(CameraController* controller, QWidget* parent)
			: InspectorSection(tr("Camera"),
			                   QStringLiteral("Scene/Camera"),
			                   parent),
				controller_(controller),
				position_label_(nullptr),
				lookat_label_(nullptr)
		{
			QWidget* content = new QWidget(this);
			QVBoxLayout* col = new QVBoxLayout(content);
			col->setContentsMargins(8, 4, 8, 8);
			col->setSpacing(4);

			position_label_ = new QLabel(QStringLiteral("—"), content);
			lookat_label_   = new QLabel(QStringLiteral("—"), content);

			col->addWidget(new FormRow(tr("View point"), position_label_, content));
			col->addWidget(new FormRow(tr("Look at"),    lookat_label_,   content));

			setContent(content);

			if (controller_)
			{
				position_label_->setText(fmt_(controller_->position()));
				lookat_label_->setText(fmt_(controller_->lookAt()));
				connect(controller_, &CameraController::positionChanged,
				        this, &CameraSection::onControllerPositionChanged_);
				connect(controller_, &CameraController::lookAtChanged,
				        this, &CameraSection::onControllerLookAtChanged_);
			}
		}

		CameraSection::~CameraSection() = default;

		void CameraSection::onControllerPositionChanged_(const QVector3D& p)
		{
			position_label_->setText(fmt_(p));
		}

		void CameraSection::onControllerLookAtChanged_(const QVector3D& p)
		{
			lookat_label_->setText(fmt_(p));
		}

	} // namespace VIEW
} // namespace BALL

