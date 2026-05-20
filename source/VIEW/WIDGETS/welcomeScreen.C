// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// Phase 999.47 (Handover §7.1) — WelcomeScreen impl.

#include <BALL/VIEW/WIDGETS/welcomeScreen.h>


#include <BALL/SYSTEM/path.h>
#include <BALL/VIEW/KERNEL/theme/iconRegistry.h>

#include <QtCore/QFileInfo>
#include <QtCore/QStandardPaths>
#include <QtCore/QTimer>
#include <QtCore/QUrl>
#include <QtGui/QFont>
#include <QtGui/QPainter>
#include <QtGui/QPaintEvent>
#include <QtGui/QPixmap>
#include <QtWidgets/QApplication>
#include <QtWidgets/QFileIconProvider>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTextBrowser>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QVBoxLayout>

#include <QtCore/QFile>
#include <QtCore/QTextStream>

namespace BALL
{
	namespace VIEW
	{

		namespace
		{
			// Phase 999.47 §7.2 — six curated samples. The path
			// resolution happens at click time via Path::find so the
			// installed and run-from-source layouts both work.
			struct SampleSpec
			{
				const char* relative_path; // under data/BALLView/samples/
				const char* thumbnail;     // 320x200 PNG
				const char* caption;
				const char* tooltip;
			};

			const SampleSpec SAMPLES[6] = {
				{ "BALLView/samples/1bna.pdb",
				  "BALLView/samples/thumbnails/1bna.png",
				  "DNA double helix (1BNA)",
				  "B-form DNA duplex — classic, loads fast, looks good" },
				{ "BALLView/samples/1ubq.pdb",
				  "BALLView/samples/thumbnails/1ubq.png",
				  "Ubiquitin (1UBQ)",
				  "Small protein — pedagogical reference" },
				{ "BALLView/samples/1stp.pdb",
				  "BALLView/samples/thumbnails/1stp.png",
				  "Streptavidin–biotin (1STP)",
				  "Enzyme + ligand — shows ligand handling" },
				{ "BALLView/samples/1ymg.pdb",
				  "BALLView/samples/thumbnails/1ymg.png",
				  "Aquaporin-0 (1YMG)",
				  "Membrane channel — surface + transparency demo" },
				{ "BALLView/samples/caffeine.mol2",
				  "BALLView/samples/thumbnails/caffeine.png",
				  "Caffeine",
				  "Small-molecule path (mol2)" },
				{ "BALLView/samples/dialanine.pdb",
				  "BALLView/samples/thumbnails/dialanine.png",
				  "Dialanine",
				  "Dipeptide — minimal MD trajectory candidate" }
			};
		}

		WelcomeScreen::WelcomeScreen(QWidget* parent)
			: QWidget(parent),
				header_logo_(nullptr),
				header_title_(nullptr),
				header_tagline_(nullptr),
				header_version_(nullptr),
				open_file_btn_(nullptr),
				open_pdb_btn_(nullptr),
				recent_btn_(nullptr),
				recent_list_layout_(nullptr),
				recent_empty_label_(nullptr),
				samples_grid_(nullptr),
				whats_new_card_(nullptr),
				whats_new_auto_dismiss_(nullptr),
				footer_(nullptr),
				skip_toggle_(nullptr)
		{
			setObjectName(QStringLiteral("welcomeScreen"));

			// v1.7.0-rc2 UFG-08 — opaque paint. See paintEvent() below
			// and the matching design note in welcomeScreen.h. The pair
			// (WA_OpaquePaintEvent + setAutoFillBackground + explicit
			// fillRect in paintEvent) guarantees the entire central area
			// is cleared on every paint pass so the previous Scene's
			// OpenGL backing surface cannot bleed through. Same pattern
			// as SectionHeader's UFG-05 fix.
			setAttribute(Qt::WA_OpaquePaintEvent, true);
			setAutoFillBackground(true);

			// Outer layout: vertically centered 720px-max content
			// column. We nest a fixed-width inner widget inside an
			// outer stretched layout to achieve the centering.
			QHBoxLayout* outer = new QHBoxLayout(this);
			outer->setContentsMargins(24, 24, 24, 24);
			outer->addStretch(1);

			QWidget* center = new QWidget(this);
			center->setMaximumWidth(720);
			outer->addWidget(center, 0, Qt::AlignTop);
			outer->addStretch(1);

			QVBoxLayout* root = new QVBoxLayout(center);
			root->setContentsMargins(0, 0, 0, 0);
			root->setSpacing(20);

			buildHeader_(root);
			buildActions_(root);

			// Two-column: recent files | what's new placeholder. We
			// build recent into a single column inside a horizontal
			// strip so a future "What's New" card can slot in beside.
			QHBoxLayout* twoCol = new QHBoxLayout();
			twoCol->setSpacing(16);
			buildRecentColumn_(twoCol);
			root->addLayout(twoCol);

			buildSamplesStrip_(root);

			root->addStretch(1);
			buildFooter_(root);

			// Cosmetics — keep visually neutral; the theme.qss in
			// 999.42 already covers QPushButton + QLabel sizing.
		}

		WelcomeScreen::~WelcomeScreen() = default;

		void WelcomeScreen::paintEvent(QPaintEvent* event)
		{
			// v1.7.0-rc2 UFG-08 — explicit opaque fill, mirrors
			// SectionHeader::paintEvent (UFG-05). WA_OpaquePaintEvent
			// tells Qt to skip the parent-background fill; we honour the
			// promise by filling the dirty rect with the Window palette
			// colour ourselves. setAutoFillBackground alone is not enough
			// once WA_OpaquePaintEvent is set — Qt disables the
			// auto-background step on opaque widgets to avoid
			// double-paint. The 720-px-max inner column then paints its
			// children on top.
			QPainter p(this);
			p.fillRect(event->rect(), palette().color(QPalette::Window));
		}

		void WelcomeScreen::buildHeader_(QVBoxLayout* root)
		{
			QWidget* header = new QWidget(this);
			QHBoxLayout* hl = new QHBoxLayout(header);
			hl->setContentsMargins(0, 0, 0, 0);
			hl->setSpacing(16);

			header_logo_ = new QLabel(header);
			QPixmap logo;
			// BALLView.icns/ico is the canonical app icon; fall back
			// to the Qt-standard application icon.
			QIcon appIcon = qApp->windowIcon();
			if (!appIcon.isNull())
				logo.convertFromImage(appIcon.pixmap(96, 96).toImage());
			if (logo.isNull())
				logo = QPixmap(96, 96);
			header_logo_->setPixmap(logo);
			header_logo_->setFixedSize(96, 96);
			hl->addWidget(header_logo_, 0, Qt::AlignTop);

			QVBoxLayout* tv = new QVBoxLayout();
			tv->setSpacing(2);
			header_title_ = new QLabel(tr("BALLView"), header);
			QFont tf = header_title_->font();
			tf.setPointSizeF(tf.pointSizeF() * 1.8);
			tf.setBold(true);
			header_title_->setFont(tf);
			tv->addWidget(header_title_);

			header_tagline_ = new QLabel(
				tr("Interactive molecular visualization"), header);
			tv->addWidget(header_tagline_);

			header_version_ = new QLabel(header);
			// Mainframe sets the version string via setText later;
			// leave blank-but-present so the layout reserves space.
			header_version_->setObjectName(QStringLiteral("welcomeVersion"));
			tv->addWidget(header_version_);
			tv->addStretch(1);

			hl->addLayout(tv, 1);
			root->addWidget(header);
		}

		void WelcomeScreen::buildActions_(QVBoxLayout* root)
		{
			QWidget* row = new QWidget(this);
			QHBoxLayout* hl = new QHBoxLayout(row);
			hl->setContentsMargins(0, 0, 0, 0);
			hl->setSpacing(12);

			auto makeBtn = [row](const QString& text) {
				QPushButton* b = new QPushButton(text, row);
				b->setMinimumHeight(48);
				b->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
				return b;
			};

			open_file_btn_ = makeBtn(tr("Open file…"));
			open_pdb_btn_  = makeBtn(tr("Open from PDB / PubChem…"));
			recent_btn_    = makeBtn(tr("Recent…"));

			hl->addWidget(open_file_btn_);
			hl->addWidget(open_pdb_btn_);
			hl->addWidget(recent_btn_);

			connect(open_file_btn_, &QPushButton::clicked,
			        this, &WelcomeScreen::openFileRequested);
			connect(open_pdb_btn_, &QPushButton::clicked,
			        this, &WelcomeScreen::openFromPdbRequested);
			// "Recent" primary button surfaces the first recent file
			// if any; mainframe receives the same signal as a row
			// click and shows a chooser otherwise.
			connect(recent_btn_, &QPushButton::clicked, this, [this]() {
				if (!recent_files_.isEmpty())
					Q_EMIT recentFileRequested(recent_files_.first());
			});

			root->addWidget(row);
		}

		void WelcomeScreen::buildRecentColumn_(QHBoxLayout* twoCol)
		{
			QWidget* col = new QWidget(this);
			QVBoxLayout* cv = new QVBoxLayout(col);
			cv->setContentsMargins(0, 0, 0, 0);
			cv->setSpacing(6);

			QLabel* heading = new QLabel(tr("Recent files"), col);
			QFont hf = heading->font();
			hf.setBold(true);
			heading->setFont(hf);
			cv->addWidget(heading);

			recent_list_layout_ = new QVBoxLayout();
			recent_list_layout_->setContentsMargins(0, 0, 0, 0);
			recent_list_layout_->setSpacing(2);
			cv->addLayout(recent_list_layout_);

			recent_empty_label_ = new QLabel(
				tr("No recently opened files yet."), col);
			recent_empty_label_->setEnabled(false);
			cv->addWidget(recent_empty_label_);

			cv->addStretch(1);
			twoCol->addWidget(col, 1);
		}

		void WelcomeScreen::buildSamplesStrip_(QVBoxLayout* root)
		{
			QLabel* heading = new QLabel(tr("Sample structures"), this);
			QFont hf = heading->font();
			hf.setBold(true);
			heading->setFont(hf);
			root->addWidget(heading);

			QWidget* gridHost = new QWidget(this);
			samples_grid_ = new QGridLayout(gridHost);
			samples_grid_->setContentsMargins(0, 0, 0, 0);
			samples_grid_->setSpacing(12);

			for (int i = 0; i < 6; ++i)
			{
				const SampleSpec& s = SAMPLES[i];

				QToolButton* card = new QToolButton(gridHost);
				card->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
				card->setAutoRaise(false);
				card->setIconSize(QSize(160, 100));
				card->setMinimumSize(176, 148);
				card->setText(QString::fromUtf8(s.caption));
				card->setToolTip(QString::fromUtf8(s.tooltip));
				card->setProperty("sampleRelative",
				                  QString::fromUtf8(s.relative_path));

				// Resolve thumbnail via Path::find. If missing (likely
				// at first land before the asset pipeline catches up)
				// fall back to a flat neutral pixmap so layout stays
				// stable.
				try
				{
					BALL::Path p;
					BALL::String resolved = p.find(s.thumbnail);
					if (!resolved.isEmpty())
					{
						QPixmap pm(QString::fromStdString(std::string(resolved)));
						if (!pm.isNull())
							card->setIcon(QIcon(pm));
					}
				}
				catch (...) { /* swallow; placeholder pixmap below */ }

				if (card->icon().isNull())
				{
					QPixmap placeholder(160, 100);
					placeholder.fill(palette().window().color().darker(108));
					card->setIcon(QIcon(placeholder));
				}

				connect(card, &QToolButton::clicked,
				        this, &WelcomeScreen::onSampleClicked_);

				samples_grid_->addWidget(card, i / 3, i % 3);
			}

			root->addWidget(gridHost);
		}

		void WelcomeScreen::buildFooter_(QVBoxLayout* root)
		{
			footer_ = new QWidget(this);
			QHBoxLayout* hl = new QHBoxLayout(footer_);
			hl->setContentsMargins(0, 0, 0, 0);
			hl->setSpacing(8);

			hl->addStretch(1);

			skip_toggle_ = new QPushButton(tr("Skip this screen on startup"), footer_);
			skip_toggle_->setCheckable(true);
			skip_toggle_->setFlat(true);
			connect(skip_toggle_, &QPushButton::toggled,
			        this, &WelcomeScreen::onSkipToggled_);
			hl->addWidget(skip_toggle_);

			root->addWidget(footer_);
		}

		void WelcomeScreen::setRecentFiles(const QStringList& files)
		{
			recent_files_ = files.mid(0, 5);

			// Clear existing rows.
			QLayoutItem* item;
			while ((item = recent_list_layout_->takeAt(0)) != nullptr)
			{
				if (QWidget* w = item->widget())
					w->deleteLater();
				delete item;
			}

			if (recent_files_.isEmpty())
			{
				recent_empty_label_->setVisible(true);
				return;
			}
			recent_empty_label_->setVisible(false);

			QFileIconProvider iconProvider;
			for (const QString& path : recent_files_)
			{
				QFileInfo fi(path);
				QToolButton* row = new QToolButton(this);
				row->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
				row->setAutoRaise(true);
				row->setIcon(iconProvider.icon(fi));
				row->setText(fi.fileName().isEmpty()
				             ? path : fi.fileName() + QStringLiteral("  —  ") + fi.absolutePath());
				row->setToolTip(path);
				row->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
				row->setProperty("recentPath", path);
				connect(row, &QToolButton::clicked,
				        this, &WelcomeScreen::onRecentClicked_);
				recent_list_layout_->addWidget(row);
			}
		}

		void WelcomeScreen::showWhatsNew(const QString& markdownFile)
		{
			// v1.7.x-26 — null immediately after deleteLater(): the early
			// return below (file-open failure) would otherwise leave
			// whats_new_card_ pointing at an object already scheduled for
			// deletion, which onWhatsNewDismiss_() could then dereference.
			if (whats_new_card_)
			{
				whats_new_card_->deleteLater();
				whats_new_card_ = nullptr;
			}

			QFile f(markdownFile);
			if (!f.open(QIODevice::ReadOnly | QIODevice::Text))
				return;
			QTextStream in(&f);
			const QString md = in.readAll();
			f.close();

			whats_new_card_ = new QFrame(this);
			QFrame* frame = qobject_cast<QFrame*>(whats_new_card_);
			frame->setFrameShape(QFrame::StyledPanel);
			QVBoxLayout* vl = new QVBoxLayout(whats_new_card_);
			vl->setContentsMargins(12, 12, 12, 12);
			vl->setSpacing(8);

			QHBoxLayout* hl = new QHBoxLayout();
			QLabel* heading = new QLabel(tr("What's new"), whats_new_card_);
			QFont hf = heading->font(); hf.setBold(true);
			heading->setFont(hf);
			hl->addWidget(heading);
			hl->addStretch(1);
			QPushButton* dismiss = new QPushButton(tr("Dismiss"), whats_new_card_);
			dismiss->setFlat(true);
			connect(dismiss, &QPushButton::clicked,
			        this, &WelcomeScreen::onWhatsNewDismiss_);
			hl->addWidget(dismiss);
			vl->addLayout(hl);

			QTextBrowser* tb = new QTextBrowser(whats_new_card_);
			tb->setMarkdown(md);
			tb->setOpenExternalLinks(true);
			tb->setMinimumHeight(160);
			vl->addWidget(tb);

			// Insert above the samples strip if possible — append at
			// end as a safe fallback.
			QVBoxLayout* root = qobject_cast<QVBoxLayout*>(layout()
				? (layout()->itemAt(0)
					? (layout()->itemAt(0)->widget()
						? layout()->itemAt(0)->widget()->layout() // unused
						: nullptr)
					: nullptr)
				: nullptr);
			(void) root; // simplification — append to the center column directly

			// The center column is the first widget inside the outer
			// QHBoxLayout. Insert at index 2 (after header + actions).
			QHBoxLayout* outer = qobject_cast<QHBoxLayout*>(layout());
			if (outer && outer->count() >= 2)
			{
				QWidget* center = outer->itemAt(1)->widget();
				if (center)
				{
					QVBoxLayout* col = qobject_cast<QVBoxLayout*>(center->layout());
					if (col)
						col->insertWidget(2, whats_new_card_);
				}
			}

			whats_new_auto_dismiss_ = new QTimer(this);
			whats_new_auto_dismiss_->setSingleShot(true);
			whats_new_auto_dismiss_->setInterval(2 * 60 * 1000); // 2 minutes
			connect(whats_new_auto_dismiss_, &QTimer::timeout,
			        this, &WelcomeScreen::onWhatsNewAutoDismiss_);
			whats_new_auto_dismiss_->start();
		}

		bool WelcomeScreen::isSkipOnStartupChecked() const
		{
			return skip_toggle_ ? skip_toggle_->isChecked() : false;
		}

		void WelcomeScreen::onWhatsNewDismiss_()
		{
			if (whats_new_auto_dismiss_) whats_new_auto_dismiss_->stop();
			if (whats_new_card_)
			{
				whats_new_card_->hide();
				whats_new_card_->deleteLater();
				whats_new_card_ = nullptr;
			}
		}

		void WelcomeScreen::onWhatsNewAutoDismiss_()
		{
			onWhatsNewDismiss_();
		}

		void WelcomeScreen::onSampleClicked_()
		{
			QObject* s = sender();
			if (!s) return;
			const QString rel = s->property("sampleRelative").toString();
			if (rel.isEmpty()) return;

			try
			{
				BALL::Path p;
				BALL::String resolved = p.find(rel.toUtf8().constData());
				if (!resolved.isEmpty())
					Q_EMIT sampleRequested(QString::fromStdString(std::string(resolved)));
			}
			catch (...) { /* silent — caller logs if it cares */ }
		}

		void WelcomeScreen::onRecentClicked_()
		{
			QObject* s = sender();
			if (!s) return;
			const QString path = s->property("recentPath").toString();
			if (!path.isEmpty())
				Q_EMIT recentFileRequested(path);
		}

		void WelcomeScreen::onSkipToggled_(bool checked)
		{
			Q_EMIT skipOnStartupToggled(checked);
		}

	} // namespace VIEW
} // namespace BALL

