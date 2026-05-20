// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: mainControlPreferences.C,v 1.16.16.1 2007/03/25 22:02:03 oliver Exp $
//

#include <BALL/VIEW/DIALOGS/mainControlPreferences.h>
#include <BALL/VIEW/KERNEL/common.h>

#include <QtCore/QDir>

#include <QtWidgets/QCheckBox>
#include <QtWidgets/QStyleFactory>
#include <QtWidgets/QFontDialog>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>

#include <BALL/SYSTEM/path.h>

namespace BALL
{
	namespace VIEW
	{

MainControlPreferences::MainControlPreferences(QWidget* parent, const char* name, Qt::WindowFlags fl)
	: QWidget(parent, fl),
		Ui_MainControlPreferencesData(),
		PreferencesEntry(),
		last_index_(0),
		text_size_combo_(nullptr)
{
	setupUi(this);
	setObjectName(name);
	setINIFileSectionName("GENERAL");

	Path p;
	QStringList dpaths = QString(p.getDataPath().c_str()).split("\n");

	languageComboBox_->addItem("English (default)", QVariant("en_US"));
	Q_FOREACH(QString str, dpaths) {
		QDir dir(str + "BALLView/translations");
		QStringList tList = dir.entryList(QStringList("BALLView-*.qm"));
		Q_FOREACH(QString entry, tList) {
			entry.replace("BALLView-", "");
			entry.replace(".qm", "");
			languageComboBox_->addItem(QLocale::languageToString(QLocale(entry).language()), QVariant(entry));
		}
	}

	style_box_->addItems(QStyleFactory::keys());
	QString prefered_style = "Plastique";
#ifdef BALL_OS_WINDOWS
	prefered_style = "WindowsXP";
#endif
#ifdef BALL_OS_DARWIN
	prefered_style = "Macintosh (aqua)";
#endif

	Index pos = style_box_->findText(prefered_style);
	if (pos != -1) style_box_->setCurrentIndex(pos);

	setWidgetStackName("General");
	registerWidgets_();
	unregisterObject_(style_box_);
	unregisterObject_(languageComboBox_);

	connect( font_button, SIGNAL( clicked() ), this, SLOT( selectFont() ) );

	// v1.7.x-21 — accessibility text-size scale, added under the font row of
	// the font group box. Multiplies the chosen application font's point size
	// in getFont(); applied app-wide via MainControl::applyPreferences().
	text_size_combo_ = new QComboBox(this);
	text_size_combo_->addItem(tr("Normal (100%)"),      QVariant(1.0));
	text_size_combo_->addItem(tr("Large (125%)"),       QVariant(1.25));
	text_size_combo_->addItem(tr("Extra Large (150%)"), QVariant(1.5));
	text_size_combo_->setToolTip(tr("Scale all application text for "
		"readability. Takes effect when you apply preferences."));
	{
		QHBoxLayout* ts_row = new QHBoxLayout();
		ts_row->addWidget(new QLabel(tr("Text size:"), this));
		ts_row->addWidget(text_size_combo_);
		ts_row->addStretch(1);
		// `verticalLayout` is the font group box's layout (see the .ui).
		verticalLayout->addLayout(ts_row);
	}
}

MainControlPreferences::~MainControlPreferences()
{
	#ifdef BALL_VIEW_DEBUG
		Log.error() << "Destructing object " << (void *)this 
								<< " of class MainControlPreferences" << std::endl;
	#endif 
}

QStyle* MainControlPreferences::setStyle()
{
	if(!QApplication::style() || (QApplication::style()->objectName().compare(style_box_->currentText(), Qt::CaseInsensitive) != 0))
	{
		QStyle* new_style = QStyleFactory::create(style_box_->currentText());
		QApplication::setStyle(new_style);
	}

	return QApplication::style();
}

QFont MainControlPreferences::getFont()
{
	// v1.7.x-21 — apply the accessibility text-size multiplier on top of the
	// user's chosen font. The font_label always shows the unscaled base font;
	// the combo scales it app-wide.
	QFont f = font_label->font();
	double m = 1.0;
	if (text_size_combo_ != nullptr)
	{
		bool ok = false;
		const double v = text_size_combo_->currentData().toDouble(&ok);
		if (ok && v > 0.0) m = v;
	}
	if (m != 1.0)
	{
		if (f.pointSizeF() > 0.0)      f.setPointSizeF(f.pointSizeF() * m);
		else if (f.pixelSize() > 0)    f.setPixelSize(qRound(f.pixelSize() * m));
	}
	return f;
}

void MainControlPreferences::enableLoggingToFile(bool state)
{
	if (state) logging_to_file->setChecked(true);
	else       logging_to_file->setChecked(false);
}

bool MainControlPreferences::loggingToFileEnabled() const
{
	return logging_to_file->isChecked();
}

bool MainControlPreferences::getSkipDriverChecks() const
{
	return skip_driver_checks->isChecked();
}

void MainControlPreferences::selectFont()
{
	bool ok = true;
	QFont font = QFontDialog::getFont(&ok, font_, 0);

	if (!ok) return;

	font_label->setFont(font);
	font_ = font;
}

void MainControlPreferences::setFont(QFont font)
{
	font_ = font;
	font_label->setFont(font);
}

void MainControlPreferences::writePreferenceEntries(INIFile& inifile)
{
	PreferencesEntry::writePreferenceEntries(inifile);
	inifile.insertValue(inifile_section_name_, "style", ascii(style_box_->currentText()));
	inifile.insertValue(inifile_section_name_, "language", ascii(languageComboBox_->currentData().toString()));
	if (text_size_combo_ != nullptr)
	{
		inifile.insertValue(inifile_section_name_, "textScale",
			ascii(QString::number(text_size_combo_->currentIndex())));
	}
}

void MainControlPreferences::readPreferenceEntries(const INIFile& inifile)
{
	PreferencesEntry::readPreferenceEntries(inifile);
	if (inifile.hasEntry(inifile_section_name_, "style"))
	{
		String value = inifile.getValue(inifile_section_name_, "style");
		int e = style_box_->findText(value.c_str());
		if (e == -1) return;

		style_box_->setCurrentIndex(e);
	}

	if (inifile.hasEntry(inifile_section_name_, "language"))
	{
		int e = languageComboBox_->findData(QString(inifile.getValue(inifile_section_name_, "language").c_str()));
		if (e == -1) return;

		last_index_ = e;
		languageComboBox_->setCurrentIndex(e);
	}

	if (text_size_combo_ != nullptr
	    && inifile.hasEntry(inifile_section_name_, "textScale"))
	{
		const int idx = String(inifile.getValue(inifile_section_name_, "textScale")).toInt();
		if (idx >= 0 && idx < text_size_combo_->count())
			text_size_combo_->setCurrentIndex(idx);
	}
}

} } // namespaces
