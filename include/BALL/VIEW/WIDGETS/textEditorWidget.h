// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#ifndef BALL_VIEW_WIDGETS_TEXTEDITORWIDGET_H
#define BALL_VIEW_WIDGETS_TEXTEDITORWIDGET_H

#ifndef BALL_VIEW_KERNEL_COMMON_H
# include <BALL/VIEW/KERNEL/common.h>
#endif

#ifndef BALL_VIEW_WIDGETS_DOCKWIDGET_H
# include <BALL/VIEW/WIDGETS/dockWidget.h>
#endif

#include <QPlainTextEdit>
#include <QObject>

class QPaintEvent;
class QResizeEvent;
class QSize;
class QWidget;

namespace BALL
{
	namespace VIEW
	{
		/** This widget can be used for displaying or editing text.
		 *  This class is heavily based on the Qt CodeEditor example.
		 */
		class BALL_VIEW_EXPORT TextEditorWidget
			: public QPlainTextEdit
		{
			Q_OBJECT

			public:
			
				TextEditorWidget(QWidget *parent = 0);

				void lineNumbersPaintEvent(QPaintEvent *event);
				int  lineNumbersWidth();

			protected:
				
				class LineNumbers 
					: public QWidget
				{
					public:
						LineNumbers(TextEditorWidget *editor); 
						QSize sizeHint() const;

					protected:
						void paintEvent(QPaintEvent *event);
					
					private:
						TextEditorWidget *editor_;
				};
			 
			 void resizeEvent(QResizeEvent *event);

			private slots:
				void updateLineNumbersWidth(int newBlockCount);
				void highlightCurrentLine();
				void updateLineNumbers(const QRect &, int);

			private:
				QWidget *line_numbers_;
		};

	}
}

#endif // BALL_VIEW_WIDGETS_TEXTEDITORWIDGET_H
