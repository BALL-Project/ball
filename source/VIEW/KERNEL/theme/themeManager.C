// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// Phase 999.40 — BALLView Refresh ThemeManager implementation
// (Handover Phase 0). Single-theme per maintainer-Q3.
// Phase 999.48 — adds text-size preference per Handover §8.3.
//

#include <BALL/VIEW/KERNEL/theme/themeManager.h>

#include <QtCore/QFile>
#include <QtCore/QIODevice>
#include <QtCore/QSettings>
#include <QtCore/QTextStream>
#include <QtGui/QFont>
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
			  initialised_(false),
			  text_scale_(ScaleNormal),
			  base_point_size_(0)
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

			// Phase 999.48 §8.3 — text-size preference. Cache the base
			// point size (so we can rescale relative to it on subsequent
			// changes), load the persisted scale, and apply it. Default
			// scale is ScaleNormal (100%) — first run is a no-op.
			QFont base = app_->font();
			base_point_size_ = base.pointSize() > 0 ? base.pointSize() : 13;

			QSettings settings;
			text_scale_ = settings.value(QStringLiteral("Appearance/textScale"),
			                              int(ScaleNormal)).toInt();
			// Clamp to the supported set; out-of-range falls back to Normal.
			if (text_scale_ != ScaleNormal && text_scale_ != ScaleLarge
			    && text_scale_ != ScaleExtraLarge)
			{
				text_scale_ = ScaleNormal;
			}
			if (text_scale_ != ScaleNormal)
			{
				applyTextScale_();
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

		void ThemeManager::setTextScale(int scale)
		{
			// Validate the value against the public enum. Silently no-op
			// on out-of-range / no-change.
			if (scale != ScaleNormal && scale != ScaleLarge && scale != ScaleExtraLarge)
			{
				return;
			}
			if (scale == text_scale_) return;

			text_scale_ = scale;

			QSettings settings;
			settings.setValue(QStringLiteral("Appearance/textScale"), text_scale_);

			applyTextScale_();
			Q_EMIT textScaleChanged(text_scale_);
		}

		void ThemeManager::applyTextScale_()
		{
			if (app_ == nullptr || base_point_size_ <= 0) return;

			QFont f = app_->font();
			// Compute the new point size from the cached base × scale/100.
			// Use float math then round to nearest int to avoid drift on
			// repeated rescales. setFont() triggers a full UI relayout.
			const int new_pt = qRound(base_point_size_ * (text_scale_ / 100.0));
			if (new_pt <= 0) return;
			f.setPointSize(new_pt);
			app_->setFont(f);
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
