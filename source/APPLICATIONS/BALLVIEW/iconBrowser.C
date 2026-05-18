// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// Phase 999.43 — BALLView Refresh: IconBrowser implementation.
// See `iconBrowser.h` for design.
//

#include "iconBrowser.h"


#include <BALL/VIEW/KERNEL/theme/iconRegistry.h>

#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtGui/QPixmap>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QListWidgetItem>
#include <QtWidgets/QVBoxLayout>

namespace BALL
{
	namespace VIEW
	{

		// Keep in sync with source/VIEW/KERNEL/theme/theme.qrc aliases.
		// When new aliases land in the qrc file (any phase after 999.43),
		// append the key here so the browser surfaces it. The future
		// "scan theme.qrc at build time" enhancement is deferred to 999.46.
		static const char* const kIconKeys[] = {
			// actions/* — chronological order of addition.
			"actions/quickopen-file",
			"actions/quicksave",
			"actions/document-save",
			"actions/view-fullscreen",
			"actions/view-restore",
			"actions/process-stop",
			"actions/measure",
			"actions/list-add",
			"actions/edit-delete",
			"actions/edit-copy",
			"actions/molecule-add-hydrogens",
			"actions/molecule-minimize",
			"actions/molecule-assign-bond-orders",
			"actions/help-hint",
			"actions/simulation-running",
			"actions/raytracing",
			// Phase 999.43 additions:
			"actions/chevron-down",
			"actions/chevron-right",
			"actions/reset",
			// Phase 999.45 additions:
			"actions/chevron-up",
			// v1.7.0-rc2 UFG-01: toolbar-visible additions.
			"actions/document-open",
			"actions/download-pdb",
			"actions/download-electrondensity",
			"actions/download-pubchem",
			"actions/object-focus",
			"actions/mode-edit",
			"actions/molecule-set-element",
			"actions/create-bond",
			"actions/continuous-loop",
			"actions/screenshot",
			// categories/* —
			"categories/preferences",
			// v1.7.0-rc2 UFG-01:
			"categories/preferences-display",
			// app icon —
			"ball-app",
		};
		static constexpr int kIconKeyCount = sizeof(kIconKeys) / sizeof(kIconKeys[0]);

		IconBrowser::IconBrowser(QWidget* parent)
			: QDialog(parent),
				grid_(nullptr)
		{
			setObjectName(QStringLiteral("iconBrowser"));
			setWindowTitle(tr("Icon Browser — Developer Tool"));
			resize(640, 480);

			QVBoxLayout* root = new QVBoxLayout(this);
			root->setContentsMargins(12, 12, 12, 12);
			root->setSpacing(8);

			grid_ = new QListWidget(this);
			grid_->setObjectName(QStringLiteral("iconBrowserGrid"));
			grid_->setViewMode(QListView::IconMode);
			grid_->setIconSize(QSize(32, 32));
			grid_->setGridSize(QSize(120, 80));
			grid_->setResizeMode(QListView::Adjust);
			grid_->setWordWrap(true);
			grid_->setMovement(QListView::Static);
			root->addWidget(grid_);

			QDialogButtonBox* buttons = new QDialogButtonBox(QDialogButtonBox::Close, this);
			connect(buttons, &QDialogButtonBox::rejected, this, &QDialog::accept);
			connect(buttons, &QDialogButtonBox::accepted, this, &QDialog::accept);
			root->addWidget(buttons);

			populate_();
		}

		IconBrowser::~IconBrowser() = default;

		void IconBrowser::populate_()
		{
			if (!grid_) return;
			grid_->clear();

			for (int i = 0; i < kIconKeyCount; ++i)
			{
				const char* key = kIconKeys[i];
				QIcon icon = Icons::get(key);

				QListWidgetItem* item = new QListWidgetItem(grid_);
				item->setIcon(icon);
				item->setText(QString::fromLatin1(key));
				item->setTextAlignment(Qt::AlignHCenter | Qt::AlignBottom);

				// Tooltip: 4 state tints side-by-side as inline images via
				// rich-text. Saves writing a custom delegate.
				QString tip = QStringLiteral("<b>%1</b><br>")
				                  .arg(QString::fromLatin1(key));
				if (icon.isNull())
				{
					tip += tr("&lt;icon resolved to null QIcon&gt;");
				}
				else
				{
					// Inline-base64 would be ideal but expensive for a
					// dev tool; instead just enumerate the four modes by
					// name. Visual diff is one click away (open browser
					// → see 32×32 thumbnail at the cell-icon position).
					tip += tr("Modes available: Normal · Active · Selected · Disabled");
				}
				item->setToolTip(tip);
			}
		}

	} // namespace VIEW
} // namespace BALL

