// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// Phase 999.40 — BALLView Refresh design tokens (Handover Phase 0).
//
// Single source of truth for spacing, type, radii, icon sizes and the
// neutral color palette used by the v1.7 BALLView refresh.
//
// Per maintainer-Q3 (`.planning/MAINTAINER-QUESTIONS-999.1.md`):
//   "Single neutral theme only" — NO Light/Dark/Follow-System modes.
//
// QSS cannot share C++ scope, so the matching neutral .qss in
// `source/VIEW/KERNEL/theme/theme.qrc` hard-codes equivalent values.
// If you change a token here, mirror it in `theme-neutral.qss`.
//

#ifndef BALL_VIEW_KERNEL_THEME_TOKENS_H
#define BALL_VIEW_KERNEL_THEME_TOKENS_H

namespace BALL
{
	namespace VIEW
	{
		namespace Theme
		{

			// ----- Spacing scale — multiples of 4px. -----
			constexpr int kSpace1 = 4;
			constexpr int kSpace2 = 8;
			constexpr int kSpace3 = 12;
			constexpr int kSpace4 = 16;
			constexpr int kSpace5 = 24;
			constexpr int kSpace6 = 32;
			constexpr int kSpace7 = 48;

			// ----- Type scale — pt sizes at 1× DPI. -----
			constexpr int kFontXs    = 10;
			constexpr int kFontSm    = 11;
			constexpr int kFontMd    = 13;  // body default
			constexpr int kFontLg    = 15;
			constexpr int kFontXl    = 18;
			constexpr int kFontTitle = 22;

			// ----- Radii. -----
			constexpr int kRadiusSm = 4;
			constexpr int kRadiusMd = 6;
			constexpr int kRadiusLg = 10;

			// ----- Icon sizes (logical px; engine multiplies by devicePixelRatio). -----
			constexpr int kIconToolbar = 20;
			constexpr int kIconButton  = 16;
			constexpr int kIconTree    = 14;

			// ----- Neutral color palette (single theme, per maintainer-Q3). -----
			//
			// Picked from the "Light" column of the Handover Phase 0 doc; values
			// are intentionally neutral so the palette inherits naturally from
			// the OS chrome (window frame, scroll bars, accent) rather than
			// fighting it. Light + dark variants are explicitly out of v1.7 scope.
			//
			// Format: 0xRRGGBB. Use Theme::asQColor() helpers in widget code if
			// you need a QColor; QSS file uses literal hex.
			//
			constexpr unsigned int kBg        = 0xf7f7f8;
			constexpr unsigned int kBgElev    = 0xffffff;
			constexpr unsigned int kBgSoft    = 0xefeff1;
			constexpr unsigned int kInk       = 0x1a1c1f;
			constexpr unsigned int kInkSoft   = 0x4c5158;
			constexpr unsigned int kInkMuted  = 0x7a7f87;
			constexpr unsigned int kRule      = 0xe1e3e8;
			constexpr unsigned int kAccent    = 0x2a6fdb;
			constexpr unsigned int kAccentBg  = 0xe7efff;
			constexpr unsigned int kWarn      = 0xc2580f;
			constexpr unsigned int kDanger    = 0xb3261e;
			constexpr unsigned int kOk        = 0x1f7a4d;

		} // namespace Theme
	} // namespace VIEW
} // namespace BALL

#endif // BALL_VIEW_KERNEL_THEME_TOKENS_H
