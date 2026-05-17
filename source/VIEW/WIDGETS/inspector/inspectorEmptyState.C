// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// Phase 999.44 — InspectorEmptyState implementation.
//

#include <BALL/VIEW/WIDGETS/inspector/inspectorEmptyState.h>


#include <BALL/VIEW/KERNEL/theme/iconRegistry.h>

#include <QtGui/QFont>
#include <QtGui/QIcon>
#include <QtGui/QPixmap>
#include <QtWidgets/QLabel>
#include <QtWidgets/QVBoxLayout>

namespace BALL
{
	namespace VIEW
	{

		namespace
		{
			constexpr int ICON_PX  = 48;
			constexpr int TITLE_PT = 16;
			constexpr int BODY_PT  = 12;

			QString noSelectionCopy_title()
			{
				return QObject::tr("Nothing selected");
			}
			QString noSelectionCopy_body()
			{
				return QObject::tr(
					"Pick a residue, chain, or atom in the Project dock to "
					"see its properties here.");
			}
			QString noRepCopy_title()
			{
				return QObject::tr("No representation");
			}
			QString noRepCopy_body()
			{
				return QObject::tr(
					"Add a representation from the Representations dock or via "
					"the Display menu.");
			}
			QString noSceneCopy_title()
			{
				return QObject::tr("No scene yet");
			}
			QString noSceneCopy_body()
			{
				return QObject::tr(
					"Open a PDB file from File › Open to populate the scene.");
			}
		}

		InspectorEmptyState::InspectorEmptyState(const QString& iconKey,
		                                         const QString& titleText,
		                                         const QString& bodyText,
		                                         QWidget* parent)
			: QWidget(parent),
				icon_(nullptr),
				title_(nullptr),
				body_(nullptr)
		{
			setObjectName("inspectorEmptyState");
			setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

			QVBoxLayout* root = new QVBoxLayout(this);
			root->setContentsMargins(24, 32, 24, 32);
			root->setSpacing(12);
			root->addStretch(1);

			icon_ = new QLabel(this);
			icon_->setAlignment(Qt::AlignCenter);
			QIcon ic = Icons::get(iconKey.toUtf8().constData());
			if (!ic.isNull())
				icon_->setPixmap(ic.pixmap(ICON_PX, ICON_PX));
			root->addWidget(icon_, 0, Qt::AlignHCenter);

			title_ = new QLabel(titleText, this);
			title_->setObjectName("inspectorEmptyStateTitle");
			QFont tf = title_->font();
			tf.setPointSize(TITLE_PT);
			tf.setBold(true);
			title_->setFont(tf);
			title_->setAlignment(Qt::AlignCenter);
			title_->setWordWrap(true);
			root->addWidget(title_);

			body_ = new QLabel(bodyText, this);
			body_->setObjectName("inspectorEmptyStateBody");
			QFont bf = body_->font();
			bf.setPointSize(BODY_PT);
			body_->setFont(bf);
			body_->setAlignment(Qt::AlignCenter);
			body_->setWordWrap(true);
			root->addWidget(body_);

			root->addStretch(2);
		}

		InspectorEmptyState::~InspectorEmptyState() = default;

		InspectorEmptyState* InspectorEmptyState::forNoSelection(QWidget* parent)
		{
			// "molecule-add-hydrogens" is the closest atomic-pick icon in
			// the current theme bundle; the icon set is a Phase 999.42
			// followup. If the key fails to resolve the empty state still
			// renders title + body without an icon.
			return new InspectorEmptyState(
				QStringLiteral("actions/molecule-add-hydrogens"),
				noSelectionCopy_title(), noSelectionCopy_body(), parent);
		}

		InspectorEmptyState* InspectorEmptyState::forNoRepresentation(QWidget* parent)
		{
			return new InspectorEmptyState(
				QStringLiteral("actions/list-add"),
				noRepCopy_title(), noRepCopy_body(), parent);
		}

		InspectorEmptyState* InspectorEmptyState::forNoScene(QWidget* parent)
		{
			return new InspectorEmptyState(
				QStringLiteral("actions/quickopen-file"),
				noSceneCopy_title(), noSceneCopy_body(), parent);
		}

	} // namespace VIEW
} // namespace BALL

