// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// Phase 999.42 — BALLView Refresh: ThemedIconEngine implementation.
// See `include/BALL/VIEW/KERNEL/theme/themedIconEngine.h` for design.
//

#include <BALL/VIEW/KERNEL/theme/themedIconEngine.h>

#ifdef BALL_UI_V2

#include <BALL/VIEW/KERNEL/theme/tokens.h>

#include <QtCore/QFile>
#include <QtGui/QColor>
#include <QtGui/QGuiApplication>
#include <QtGui/QImage>
#include <QtGui/QPainter>
#include <QtGui/QScreen>
#include <QtSvg/QSvgRenderer>

namespace BALL
{
	namespace VIEW
	{
		namespace Theme
		{

			ThemedIconEngine::ThemedIconEngine(const QString& name)
				: QIconEngine(),
				  name_(name)
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

				const QString key = cacheKey_(size, mode, dpr);
				const auto hit = cache_.find(key);
				if (hit != cache_.end())
				{
					return hit.value();
				}
				QPixmap rendered = render_(size, mode, dpr);
				cache_.insert(key, rendered);
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

#endif // BALL_UI_V2
