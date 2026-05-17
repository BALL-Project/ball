// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// Phase 999.44 Plan 04 — LabelController (read-only mirror).
//
// Source dialog: source/VIEW/DIALOGS/labelDialog.C (~227 LOC).
//

#ifndef BALL_VIEW_KERNEL_CONTROLLERS_LABELCONTROLLER_H
#define BALL_VIEW_KERNEL_CONTROLLERS_LABELCONTROLLER_H


#ifndef BALL_COMMON_GLOBAL_H
# include <BALL/COMMON/global.h>
#endif

#include <QtCore/QObject>
#include <QtCore/QString>

namespace BALL
{
	namespace VIEW
	{
		class Representation;

		/**
		 * Per-Representation Label settings (text content + font +
		 * placement). Read-only mirror; the legacy LabelDialog still
		 * owns mutation until the cut-over plan.
		 */
		class BALL_VIEW_EXPORT LabelController : public QObject
		{
			Q_OBJECT

			Q_PROPERTY(QString labelText READ labelText WRITE setLabelText NOTIFY labelTextChanged)
			Q_PROPERTY(int labelType READ labelType WRITE setLabelType NOTIFY labelTypeChanged)
			Q_PROPERTY(int fontSize READ fontSize WRITE setFontSize NOTIFY fontSizeChanged)

			public:
				explicit LabelController(Representation* rep = nullptr,
				                         QObject* parent = nullptr);
				~LabelController() override;

				Representation* representation() const { return rep_; }
				void setRepresentation(Representation* rep);

				QString labelText() const { return label_text_; }
				int labelType() const     { return label_type_; }
				int fontSize() const      { return font_size_; }

			public Q_SLOTS:
				void apply();
				void revert();

				void setLabelText(const QString& t);
				void setLabelType(int t);
				void setFontSize(int s);

			Q_SIGNALS:
				void labelTextChanged(const QString& t);
				void labelTypeChanged(int t);
				void fontSizeChanged(int s);
				void appliedStub();

			private:
				Representation* rep_;
				QString label_text_;
				int     label_type_;
				int     font_size_;
		};

	} // namespace VIEW
} // namespace BALL

#endif // BALL_VIEW_KERNEL_CONTROLLERS_LABELCONTROLLER_H
