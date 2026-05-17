// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// Phase 999.43 — BALLView Refresh: hand-coded About dialog.
// See `aboutDialog.h` for design.
//

#include "aboutDialog.h"

#include <BALL/COMMON/version.h>
#include <BALL/SYSTEM/path.h>
#include <BALL/VIEW/KERNEL/theme/iconRegistry.h>
#include <BALL/VIEW/WIDGETS/formRow.h>
#include <BALL/VIEW/WIDGETS/sectionHeader.h>

#include <QtCore/QFile>
#include <QtCore/QJsonArray>
#include <QtCore/QJsonDocument>
#include <QtCore/QJsonObject>
#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtGui/QPixmap>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QVBoxLayout>

#include <QtCore/qglobal.h>
#include <QtGui/QGuiApplication>

namespace BALL
{
	namespace VIEW
	{

		AboutDialog::AboutDialog(QWidget* parent)
			: QDialog(parent)
		{
			setObjectName(QStringLiteral("aboutDialog"));
			setWindowTitle(tr("About BALLView"));
			setFixedWidth(480);

			QVBoxLayout* root = new QVBoxLayout(this);
			root->setContentsMargins(24, 20, 24, 16);
			root->setSpacing(12);

			// ── Header: app icon + product name + version ─────────────────
			QHBoxLayout* header = new QHBoxLayout();
			header->setSpacing(16);

			QLabel* icon_label = new QLabel(this);
			icon_label->setObjectName(QStringLiteral("aboutDialogIcon"));
			QIcon app_icon = Icons::get("ball-app");
			if (!app_icon.isNull())
			{
				icon_label->setPixmap(app_icon.pixmap(64, 64));
			}
			icon_label->setFixedSize(64, 64);
			icon_label->setAlignment(Qt::AlignCenter);
			header->addWidget(icon_label);

			QVBoxLayout* title_col = new QVBoxLayout();
			title_col->setSpacing(2);

			QLabel* product_label = new QLabel(QStringLiteral("BALLView"), this);
			product_label->setObjectName(QStringLiteral("aboutDialogProduct"));
			QFont product_font = product_label->font();
			product_font.setPointSize(22);
			product_font.setBold(true);
			product_label->setFont(product_font);
			title_col->addWidget(product_label);

			QLabel* version_label = new QLabel(
				QStringLiteral("Version ") + QString::fromLatin1(BALL_RELEASE_STRING),
				this);
			version_label->setObjectName(QStringLiteral("aboutDialogVersion"));
			QFont version_font = version_label->font();
			version_font.setStyleHint(QFont::TypeWriter);
			version_font.setFamily(QStringLiteral("Menlo, Consolas, monospace"));
			version_font.setPointSize(12);
			version_label->setFont(version_font);
			title_col->addWidget(version_label);

			header->addLayout(title_col, /*stretch=*/1);
			root->addLayout(header);

			root->addSpacing(4);

			// ── "Built with" section ─────────────────────────────────────
			root->addWidget(new SectionHeader(tr("Built with"), this));

			root->addWidget(new FormRow(tr("Qt"),
				new QLabel(QString::fromLatin1(qVersion()), this), this));

			// Python version (best-effort: BALL is not necessarily linked
			// with embedded Python — we report the runtime detection
			// outcome rather than a compile-time string).
			QString python_str = QStringLiteral("n/a (BALL_PYTHON_SUPPORT off)");
#			ifdef BALL_PYTHON_SUPPORT
				python_str = QStringLiteral("embedded");
#			endif
			root->addWidget(new FormRow(tr("Python"),
				new QLabel(python_str, this), this));

			root->addWidget(new FormRow(tr("OpenGL"),
				new QLabel(QStringLiteral("fixed-function (compat profile)"), this), this));

			root->addSpacing(8);

			// ── Authors section ──────────────────────────────────────────
			root->addWidget(new SectionHeader(tr("Authors"), this));

			QLabel* authors_label = new QLabel(this);
			authors_label->setObjectName(QStringLiteral("aboutDialogAuthors"));
			authors_label->setAlignment(Qt::AlignCenter);
			authors_label->setWordWrap(true);
			authors_label->setText(loadAuthors_().join(QStringLiteral("\n")));
			root->addWidget(authors_label);

			root->addSpacing(8);

			// ── Footer: links + Close button ─────────────────────────────
			QLabel* links = new QLabel(this);
			links->setObjectName(QStringLiteral("aboutDialogLinks"));
			links->setTextFormat(Qt::RichText);
			links->setOpenExternalLinks(true);
			links->setAlignment(Qt::AlignCenter);
			links->setText(QStringLiteral(
				"<a href=\"https://ball-project.org\">Website</a> &nbsp;·&nbsp; "
				"<a href=\"https://github.com/BALL-Project/ball/issues\">Issues</a> &nbsp;·&nbsp; "
				"<a href=\"https://www.gnu.org/licenses/lgpl-2.1.html\">LGPL-2.1</a>"));
			root->addWidget(links);

			root->addSpacing(8);

			QDialogButtonBox* buttons = new QDialogButtonBox(QDialogButtonBox::Close, this);
			buttons->setObjectName(QStringLiteral("aboutDialogButtons"));
			connect(buttons, &QDialogButtonBox::rejected, this, &QDialog::accept);
			connect(buttons, &QDialogButtonBox::accepted, this, &QDialog::accept);
			root->addWidget(buttons);
		}

		AboutDialog::~AboutDialog() = default;

		QStringList AboutDialog::loadAuthors_()
		{
			static const QStringList kFallback = {
				QStringLiteral("Anna Katharina Hildebrandt"),
				QStringLiteral("Thomas Kemmer"),
				QStringLiteral("Stefan Nickels"),
				QStringLiteral("Daniel Stöckel"),
				QStringLiteral("Philipp Thiel"),
				QStringLiteral("Oliver Kohlbacher"),
				QStringLiteral("Hans-Peter Lenhof"),
				QStringLiteral("Andreas Hildebrandt"),
			};

			// Resolve via the same BALL::Path lookup the old code used
			// for graphics/logo.png. Returns "" if the file isn't
			// reachable through BALL_DATA_PATH.
			Path p;
			String resolved = p.find("BALLView/about/authors.json");
			if (resolved == "") return kFallback;

			QFile f(QString::fromStdString(resolved));
			if (!f.open(QIODevice::ReadOnly | QIODevice::Text)) return kFallback;
			QByteArray raw = f.readAll();
			f.close();

			QJsonParseError err;
			QJsonDocument doc = QJsonDocument::fromJson(raw, &err);
			if (err.error != QJsonParseError::NoError || !doc.isObject()) return kFallback;

			QJsonObject obj = doc.object();
			QJsonValue authors_val = obj.value(QStringLiteral("authors"));
			if (!authors_val.isArray()) return kFallback;

			QStringList out;
			const QJsonArray arr = authors_val.toArray();
			for (const QJsonValue& v : arr)
			{
				if (v.isString()) out.append(v.toString());
			}
			return out.isEmpty() ? kFallback : out;
		}

	} // namespace VIEW
} // namespace BALL
