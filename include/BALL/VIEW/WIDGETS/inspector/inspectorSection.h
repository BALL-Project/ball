// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// Phase 999.44 — BALLView Refresh: Inspector section base class.
//
// Per-section host inside the Inspector dock: a SectionHeader (from
// 999.43) atop a content QWidget, with a QPropertyAnimation on the
// content's maximumHeight for the 160ms OutCubic collapse/expand
// animation per Handover §4.1.2.
//
// Subclasses (StageSection, ModelSection, ...) provide the content
// widget. The base class owns the header, the animation, and the
// expanded/collapsed state.
//

#ifndef BALL_VIEW_WIDGETS_INSPECTOR_INSPECTORSECTION_H
#define BALL_VIEW_WIDGETS_INSPECTOR_INSPECTORSECTION_H

// Phase 999.44: Inspector classes are only used inside BALL_UI_V2
// code paths. The whole class declaration is gated so AUTOMOC's
// OFF-cell preprocessor scan finds no Q_OBJECT (no moc generated;
// no dangling slot/vtable references at OFF-cell link time). The
// ON-cell preprocessor IS told about BALL_UI_V2 via the VIEW
// target's PUBLIC compile definition (CMakeLists.txt:654) and
// AUTOMOC's MOC_DEFINITIONS list includes BALL_UI_V2 — verified
// by inspecting VIEW_autogen/AutogenInfo.json after configure.

#ifdef BALL_UI_V2

#ifndef BALL_COMMON_GLOBAL_H
# include <BALL/COMMON/global.h>
#endif

#include <QtCore/QString>
#include <QtWidgets/QWidget>

class QVBoxLayout;
class QPropertyAnimation;

namespace BALL
{
	namespace VIEW
	{
		class SectionHeader;

		/**
		 * Base class for Inspector sections. Composes a SectionHeader on
		 * top of a content widget supplied by the subclass via
		 * setContent(). Toggling the header animates the content's
		 * maximumHeight (160ms, OutCubic).
		 *
		 * The base class also serializes its expanded/collapsed state
		 * under the INI key returned by stateKey(). InspectorView reads
		 * and writes the [Inspector] group on construction / change.
		 *
		 * QSS objectName: `inspectorSection` (root).
		 */
		class BALL_VIEW_EXPORT InspectorSection : public QWidget
		{
			Q_OBJECT

			public:
				/**
				 * @param title  Section title (displayed in the header).
				 * @param stateKey  INI key used to persist collapsed state
				 *   under the [Inspector] group (e.g. "Scene/Stage").
				 * @param parent  Parent widget.
				 */
				InspectorSection(const QString& title,
				                 const QString& stateKey,
				                 QWidget* parent = nullptr);
				~InspectorSection() override;

				/** Section title. */
				QString title() const;

				/** INI key (e.g. "Scene/Stage") used for state persistence. */
				QString stateKey() const { return state_key_; }

				/** Current expanded state. */
				bool isExpanded() const;

				/** Section header (for InspectorView to wire up signals). */
				SectionHeader* header() const { return header_; }

				/** Content widget (subclass plumbed via setContent()). */
				QWidget* content() const { return content_; }

			public Q_SLOTS:
				/**
				 * Expand or collapse the section. Animates the content
				 * widget's maximumHeight over 160ms with OutCubic easing.
				 * No-op if the state already matches.
				 */
				void setExpanded(bool expanded);

			Q_SIGNALS:
				/** Emitted at the end of an animated collapse/expand cycle. */
				void expandedChanged(bool expanded);

			protected:
				/**
				 * Subclasses call this once in their constructor to install
				 * the section content widget. Takes ownership.
				 */
				void setContent(QWidget* content);

			private Q_SLOTS:
				void onHeaderToggled_(bool expanded);

			private:
				QString state_key_;
				SectionHeader* header_;
				QWidget* content_;
				QVBoxLayout* root_layout_;
				QPropertyAnimation* anim_;
				int expanded_max_height_;   // cached natural content height
		};

	} // namespace VIEW
} // namespace BALL

#endif // BALL_UI_V2
#endif // BALL_VIEW_WIDGETS_INSPECTOR_INSPECTORSECTION_H
