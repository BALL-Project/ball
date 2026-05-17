// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// Phase 999.42 — BALLView Refresh: ThemedIconEngine (Handover §2.2).
//
// State-aware (Normal / Active / Selected / Disabled) SVG icon engine
// with a per-DPR pixmap cache. Per maintainer-Q3 (single neutral theme)
// we tint by INTERACTION state, not by theme — the 4 tint colors are
// constants pulled from `tokens.h` (kInkSoft / kInk / kAccent /
// kInkMuted), not theme-switched.
//
// Engine input:  SVG resource path under `:/icons/<name>.svg` (the
//                aliases set up in `theme.qrc` by 999.41).
// Engine output: a QIcon that paints crisply at any DPR, with each
//                QIcon::Mode getting its own pre-tinted pixmap.
//
// The whole file is `#ifdef BALL_UI_V2`-gated — with the flag OFF the
// TU compiles to nothing and the linker omits the symbols entirely.
//

#ifndef BALL_VIEW_KERNEL_THEME_THEMEDICONENGINE_H
#define BALL_VIEW_KERNEL_THEME_THEMEDICONENGINE_H

#include <BALL/COMMON/global.h>


#include <QtCore/QHash>
#include <QtCore/QString>
#include <QtGui/QIconEngine>
#include <QtGui/QPixmap>

namespace BALL
{
	namespace VIEW
	{
		namespace Theme
		{

			/**
			 * QIconEngine subclass that loads an SVG from the
			 * `:/icons/<name>.svg` resource bundle (see theme.qrc),
			 * tints it according to QIcon::Mode (Normal / Active /
			 * Selected / Disabled), and caches the resulting pixmap
			 * per (size, mode, devicePixelRatio).
			 *
			 * Per maintainer-Q3 (`single neutral theme` for v1.7), the
			 * tint colors are constants — there's no theme switching
			 * key in the cache. If/when light/dark variants return, add
			 * a themeRevision integer to the cache key.
			 *
			 * Ownership: pass to a QIcon via
			 *   `QIcon(new BALL::VIEW::Theme::ThemedIconEngine("name"))`.
			 * QIcon takes ownership of the engine pointer (Qt convention).
			 */
			class BALL_VIEW_EXPORT ThemedIconEngine : public QIconEngine
			{
				public:
					/**
					 * Construct an engine that renders `:/icons/<name>.svg`.
					 * The name is stored verbatim; lookup happens lazily on
					 * the first paint to keep construction cheap (call sites
					 * may instantiate many icons before any get painted).
					 */
					explicit ThemedIconEngine(const QString& name);

					~ThemedIconEngine() override = default;

					/// QIconEngine — render directly to the painter (Qt forwards from
					/// QIcon::paint). Delegates to pixmap() + drawPixmap.
					void paint(QPainter* painter,
					           const QRect& rect,
					           QIcon::Mode mode,
					           QIcon::State state) override;

					/// QIconEngine — return a pixmap at the requested logical size.
					/// Internally scales by devicePixelRatio() and caches the result.
					QPixmap pixmap(const QSize& size,
					               QIcon::Mode mode,
					               QIcon::State state) override;

					/// QIconEngine — required for QVariant/QIcon round-trip; returns
					/// the SVG name we were constructed with.
					QString key() const override;

					/// QIconEngine — clone for QIcon copy semantics.
					QIconEngine* clone() const override;

				private:
					/// SVG resource path, e.g. `:/icons/actions/quicksave.svg`.
					QString name_;

					/// Cache key = "{size.w}x{size.h}@{dpr}x{mode}".
					/// Mode is encoded as a single int per QIcon::Mode.
					/// Cache lives for the lifetime of the engine instance; engines
					/// themselves are pooled inside QIcons constructed via
					/// IconRegistry, so cache hit rate is high in practice.
					mutable QHash<QString, QPixmap> cache_;

					/// Convert a QIcon::Mode to the matching tint color from
					/// tokens.h (kInkSoft / kInk / kAccent / kInkMuted).
					static unsigned int tintForMode_(QIcon::Mode mode);

					/// Build the cache key for (size, mode, dpr).
					static QString cacheKey_(const QSize& size, QIcon::Mode mode, qreal dpr);

					/// Render the SVG at physical pixels, then composite the
					/// chosen tint via CompositionMode_SourceIn. Returns a
					/// devicePixelRatio-aware QPixmap.
					QPixmap render_(const QSize& logical_size,
					                QIcon::Mode mode,
					                qreal dpr) const;
			};

		} // namespace Theme
	} // namespace VIEW
} // namespace BALL


#endif // BALL_VIEW_KERNEL_THEME_THEMEDICONENGINE_H
