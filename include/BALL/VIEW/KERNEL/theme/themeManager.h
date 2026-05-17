// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// Phase 999.40 — BALLView Refresh ThemeManager (Handover Phase 0).
// Phase 999.48 — adds TextScale enum + applyTextScale() per Handover §8.3.
//
// Singleton that loads the neutral .qss stylesheet at startup. Per
// maintainer-Q3 (`.planning/MAINTAINER-QUESTIONS-999.1.md`) the v1.7
// refresh ships a SINGLE NEUTRAL THEME — no light/dark/follow-system
// modes. The `QStyleHints::colorSchemeChanged` reactor is wired but
// currently a no-op stub so a future reversal of Q3 can flip it on
// without touching call sites.
//
// Gated on the BALL_UI_V2 CMake flag (default OFF). When OFF, this class
// is built but never instantiated — `main.C` only calls `init()` inside
// an `#ifdef BALL_UI_V2` block, so OFF builds are bit-for-bit identical
// to the pre-refresh baseline.
//

#ifndef BALL_VIEW_KERNEL_THEME_THEMEMANAGER_H
#define BALL_VIEW_KERNEL_THEME_THEMEMANAGER_H

#include <BALL/COMMON/global.h>

#include <QtCore/QObject>
#include <QtCore/QString>

class QApplication;

namespace BALL
{
	namespace VIEW
	{

		/**
		 * Singleton that owns the BALLView v1.7 refresh stylesheet.
		 *
		 * Usage (from `source/APPLICATIONS/BALLVIEW/main.C` only):
		 * @code
		 *   QApplication app(argc, argv);
		 *   #ifdef BALL_UI_V2
		 *     BALL::VIEW::ThemeManager::instance().init(&app);
		 *   #endif
		 * @endcode
		 */
		class BALL_VIEW_EXPORT ThemeManager : public QObject
		{
			Q_OBJECT

			public:

			/// Access the singleton.
			static ThemeManager& instance();

			/**
			 * Load the neutral stylesheet from the Qt resource system and
			 * apply it to @p app. Safe to call multiple times — second and
			 * subsequent calls re-apply the current stylesheet.
			 *
			 * Wires the `QStyleHints::colorSchemeChanged` signal so a future
			 * reversal of maintainer-Q3 (re-introducing light/dark/follow)
			 * has a hook to dispatch on. The handler is currently a no-op.
			 *
			 * Phase 999.48 §8.3 — also loads the persisted text scale from
			 * QSettings `[Appearance]/textScale` (default 100) and applies
			 * it to qApp's base font via setFont().
			 */
			void init(QApplication* app);

			/**
			 * Re-apply the current stylesheet to qApp. Useful if a later
			 * phase introduces user-driven re-theming. Emits themeChanged().
			 */
			void applyTheme();

			/// Text-size preference per Handover §8.3.
			enum TextScale
			{
				ScaleNormal     = 100,   ///< 100 % — default
				ScaleLarge      = 115,   ///< 115 %
				ScaleExtraLarge = 130    ///< 130 %
			};

			/// Current text scale (percent, default 100).
			int textScale() const { return text_scale_; }

			/**
			 * Set the text scale (Normal / Large / Extra Large, expressed
			 * as a percent). Applies via qApp->setFont() on the base font
			 * and persists to QSettings under [Appearance]/textScale.
			 * No-op if @p scale equals the current value or is out of range.
			 */
			void setTextScale(int scale);

			Q_SIGNALS:

			/// Emitted after a successful applyTheme().
			void themeChanged();

			/// Emitted after a successful setTextScale().
			void textScaleChanged(int newScale);

			private Q_SLOTS:

			/**
			 * No-op stub for the OS-level light/dark toggle.
			 *
			 * Q3 future-proofing: per maintainer-Q3 v1.7 ships single-theme
			 * only, so we ignore the signal. If Q3 is reversed in a later
			 * release, replace this body with a stylesheet swap and the
			 * call-site / signal wiring already exists.
			 */
			void onSystemColorSchemeChanged();

			private:

			ThemeManager();
			~ThemeManager() override = default;
			ThemeManager(const ThemeManager&) = delete;
			ThemeManager& operator=(const ThemeManager&) = delete;

			/// Read the neutral QSS bytes out of the compiled-in resource.
			static QString loadStylesheet_();

			/// Apply text_scale_ to qApp's font; cached base point-size used.
			void applyTextScale_();

			QApplication* app_;
			bool          initialised_;
			int           text_scale_;        ///< current scale (percent)
			int           base_point_size_;   ///< qApp base font pt size cached at init
		};

	} // namespace VIEW
} // namespace BALL

#endif // BALL_VIEW_KERNEL_THEME_THEMEMANAGER_H
