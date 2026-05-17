// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// Phase 999.40 — BALLView Refresh ThemeManager implementation
// (Handover Phase 0). Single-theme per maintainer-Q3.
//

#include <BALL/VIEW/KERNEL/theme/themeManager.h>

#include <QtCore/QFile>
#include <QtCore/QIODevice>
#include <QtCore/QTextStream>
#include <QtGui/QGuiApplication>
#include <QtGui/QStyleHints>
#include <QtWidgets/QApplication>

// Qt auto-registers resources via static initializers in the generated
// qrc_theme.cpp the moment libVIEW is loaded — no manual
// Q_INIT_RESOURCE(theme) needed because qrc_theme.cpp lives in the same
// translation-unit set as this file (both compile into libVIEW.dylib).
//
// The Q_INIT_RESOURCE / Q_CLEANUP_RESOURCE macros expand to a global
// extern "C" symbol that Qt6 wraps inside an anonymous namespace when
// QT_NO_KEYWORDS is set together with certain build flags on macOS arm64
// — the resulting symbol is unreachable from this TU and the link fails.
// Relying on Qt's automatic static-initializer registration sidesteps
// that entirely.

namespace BALL
{
	namespace VIEW
	{

		ThemeManager& ThemeManager::instance()
		{
			static ThemeManager s_instance;
			return s_instance;
		}

		ThemeManager::ThemeManager()
			: QObject(nullptr),
			  app_(nullptr),
			  initialised_(false)
		{
		}

		void ThemeManager::init(QApplication* app)
		{
			if (app == nullptr) return;
			app_ = app;

			applyTheme();

			// Q3 future-proofing: wire the OS color-scheme reactor even
			// though the current handler is a no-op. If maintainer-Q3 is
			// later reversed (re-introducing light/dark/follow-system),
			// the dispatch path already exists — only the handler body
			// changes.
			if (!initialised_)
			{
				if (QStyleHints* hints = QGuiApplication::styleHints())
				{
					QObject::connect(hints, &QStyleHints::colorSchemeChanged,
					                 this, [this](Qt::ColorScheme) {
					                   onSystemColorSchemeChanged();
					                 });
				}
				initialised_ = true;
			}
		}

		void ThemeManager::applyTheme()
		{
			if (app_ == nullptr) return;

			const QString sheet = loadStylesheet_();
			if (!sheet.isEmpty())
			{
				app_->setStyleSheet(sheet);
				Q_EMIT themeChanged();
			}
		}

		void ThemeManager::onSystemColorSchemeChanged()
		{
			// No-op per maintainer-Q3 (single neutral theme only).
			// See themeManager.h for the future-proofing rationale.
		}

		QString ThemeManager::loadStylesheet_()
		{
			QFile f(QStringLiteral(":/theme/theme-neutral.qss"));
			if (!f.open(QIODevice::ReadOnly | QIODevice::Text))
			{
				return QString();
			}
			QTextStream ts(&f);
			return ts.readAll();
		}

	} // namespace VIEW
} // namespace BALL
