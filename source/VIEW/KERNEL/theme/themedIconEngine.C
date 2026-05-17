// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// Phase 999.42 — BALLView Refresh: ThemedIconEngine implementation.
// See `include/BALL/VIEW/KERNEL/theme/themedIconEngine.h` for design.
//

#include <BALL/VIEW/KERNEL/theme/themedIconEngine.h>


#include <BALL/VIEW/KERNEL/theme/tokens.h>

#include <QtCore/QFile>
#include <QtCore/QMutexLocker>
#include <QtGui/QColor>
#include <QtGui/QGuiApplication>
#include <QtGui/QImage>
#include <QtGui/QPainter>
#include <QtGui/QScreen>
#include <QtSvg/QSvgRenderer>

#include <cmath>

namespace
{
	/// v1.7-RC1 C-7 — DPR bucket quantizer. System DPRs are typically
	/// {1.0, 1.25, 1.5, 1.75, 2.0, 2.5, 3.0}; the visual difference
	/// between adjacent buckets is below human-perceptible for icon
	/// rasterization, but each unique DPR creates a separate cache
	/// entry. Collapse onto a small fixed bucket set to bound cache
	/// growth.
	qreal quantize_dpr(qreal dpr)
	{
		static const qreal kBuckets[] = { 1.0, 1.5, 2.0, 3.0 };
		qreal best = kBuckets[0];
		for (qreal b : kBuckets)
		{
			if (std::abs(dpr - b) < std::abs(dpr - best))
			{
				best = b;
			}
		}
		return best;
	}
} // anonymous

namespace BALL
{
	namespace VIEW
	{
		namespace Theme
		{

			ThemedIconEngine::ThemedIconEngine(const QString& name)
				: QIconEngine(),
				  name_(name),
				  cache_(64)  // v1.7-RC1 C-7 — bound per-engine cache to 64 entries (LRU)
			{
				// Allow callers to pass either the bare key ("actions/foo") or
				// the full resource path (":/icons/actions/foo.svg"). Normalize
				// to the latter so the QSvgRenderer constructor finds the file
				// in either case.
				if (!name_.startsWith(QLatin1String(":/icons/")))
				{
					name_ = QStringLiteral(":/icons/") + name_;
				}
				if (!name_.endsWith(QLatin1String(".svg")))
				{
					name_ += QStringLiteral(".svg");
				}
			}

			unsigned int ThemedIconEngine::tintForMode_(QIcon::Mode mode)
			{
				// Per Handover §2.2 step 2:
				//   Normal   → --ink-soft  (kInkSoft)
				//   Active   → --ink       (kInk)
				//   Selected → --accent    (kAccent)
				//   Disabled → --ink-muted (kInkMuted)
				switch (mode)
				{
					case QIcon::Active:   return kInk;
					case QIcon::Selected: return kAccent;
					case QIcon::Disabled: return kInkMuted;
					case QIcon::Normal:
					default:              return kInkSoft;
				}
			}

			QString ThemedIconEngine::cacheKey_(const QSize& size, QIcon::Mode mode, qreal dpr)
			{
				return QStringLiteral("%1x%2@%3x%4")
					.arg(size.width())
					.arg(size.height())
					.arg(dpr, 0, 'f', 2)
					.arg(static_cast<int>(mode));
			}

			QPixmap ThemedIconEngine::render_(const QSize& logical_size,
			                                  QIcon::Mode mode,
			                                  qreal dpr) const
			{
				// 1. Open the SVG. If the resource is missing return a null
				//    pixmap — the caller (QIcon) will fall back to whatever
				//    default it has registered. We intentionally don't log
				//    per-paint to avoid spamming Log.error() inside the GUI
				//    event loop; missing-icon detection should happen via
				//    the IconRegistry validator at startup.
				if (!QFile::exists(name_))
				{
					return QPixmap();
				}
				QSvgRenderer renderer(name_);
				if (!renderer.isValid())
				{
					return QPixmap();
				}

				// 2. Allocate an ARGB QImage at PHYSICAL pixel size.
				const QSize physical = logical_size * dpr;
				QImage img(physical, QImage::Format_ARGB32_Premultiplied);
				img.fill(Qt::transparent);

				// 3. Paint the SVG (Lucide stroke=currentColor — but we
				//    composite our own tint over it via SourceIn below,
				//    so the SVG's nominal color is irrelevant; what matters
				//    is the alpha mask the renderer produces).
				{
					QPainter p(&img);
					p.setRenderHint(QPainter::Antialiasing, true);
					p.setRenderHint(QPainter::SmoothPixmapTransform, true);
					renderer.render(&p, QRectF(QPointF(0, 0), QSizeF(physical)));
				}

				// 4. Tint composite: paint a solid rect of the chosen color
				//    using CompositionMode_SourceIn so only the existing
				//    (already-rasterized) alpha mask survives, tinted to
				//    the target color. This is the standard Qt idiom for
				//    re-coloring an SVG-rendered mask.
				{
					QPainter p(&img);
					p.setCompositionMode(QPainter::CompositionMode_SourceIn);
					const unsigned int rgb = tintForMode_(mode);
					const QColor tint(
						static_cast<int>((rgb >> 16) & 0xFF),
						static_cast<int>((rgb >>  8) & 0xFF),
						static_cast<int>(rgb & 0xFF));
					p.fillRect(img.rect(), tint);
				}

				// 5. Wrap as a QPixmap that knows its own DPR — Qt will
				//    then place it at logical_size on screen automatically.
				QPixmap pm = QPixmap::fromImage(img);
				pm.setDevicePixelRatio(dpr);
				return pm;
			}

			QPixmap ThemedIconEngine::pixmap(const QSize& size, QIcon::Mode mode, QIcon::State /*state*/)
			{
				// We ignore QIcon::State (On / Off) — there's no on/off variant
				// in the Lucide subset we ship. If a future glyph needs it,
				// extend cache_key_ + tintForMode_ together.

				// Pick the current screen's DPR — fall back to 1.0 if no
				// QGuiApplication primary screen (headless / batch).
				qreal dpr = 1.0;
				if (QGuiApplication::primaryScreen() != nullptr)
				{
					dpr = QGuiApplication::primaryScreen()->devicePixelRatio();
				}
				// v1.7-RC1 C-7 — quantize DPR to a small bucket set BEFORE
				// key construction so non-integral system DPRs collapse
				// (1.25 → 1.5, 1.75 → 2.0, 2.5 → 2.0, …). Visual diff
				// below human-perceptible for icon sizes.
				const qreal bucketed = quantize_dpr(dpr);

				const QString key = cacheKey_(size, mode, bucketed);

				// v1.7-RC1 C-8 — single mutex guards both lookup and insert.
				// QCache::object returns a pointer that QCache OWNS — we
				// copy the QPixmap before releasing the lock so the
				// returned value survives any concurrent eviction.
				{
					QMutexLocker locker(&cache_mutex_);
					if (QPixmap* hit = cache_.object(key))
					{
						return *hit;  // copy under lock
					}
				}

				// Render OUTSIDE the lock — render_() does no cache_
				// access and can be costly (QSvgRenderer + raster).
				QPixmap rendered = render_(size, mode, bucketed);

				{
					QMutexLocker locker(&cache_mutex_);
					// QCache::insert takes ownership of the QPixmap*;
					// cost defaults to 1 so 64 entries fit our 64 capacity.
					cache_.insert(key, new QPixmap(rendered));
				}
				return rendered;
			}

			void ThemedIconEngine::paint(QPainter* painter,
			                             const QRect& rect,
			                             QIcon::Mode mode,
			                             QIcon::State state)
			{
				if (painter == nullptr) return;
				const QPixmap pm = pixmap(rect.size(), mode, state);
				if (pm.isNull()) return;
				painter->drawPixmap(rect, pm);
			}

			QString ThemedIconEngine::key() const
			{
				return name_;
			}

			QIconEngine* ThemedIconEngine::clone() const
			{
				// Fresh engine with the same name; the per-instance cache is
				// intentionally NOT copied — clones may be on different DPR
				// screens.
				return new ThemedIconEngine(name_);
			}

		} // namespace Theme
	} // namespace VIEW
} // namespace BALL

