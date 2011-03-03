// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/VIEW/WIDGETS/textEditorWidget.h>

#include <QtCore/QSize>
#include <QtGui/QPainter>
#include <QtGui/QTextBlock>

namespace BALL
{
	namespace VIEW
	{
		TextEditorWidget::TextEditorWidget(QWidget *parent) 
			: QPlainTextEdit(parent)
		{
			line_numbers_ = new LineNumbers(this);

			connect(this, SIGNAL(blockCountChanged(int)), 
							this, SLOT(updateLineNumbersWidth(int)));
			
			connect(this, SIGNAL(updateRequest(QRect,int)), 
							this, SLOT(updateLineNumbers(QRect,int)));
			
			connect(this, SIGNAL(cursorPositionChanged()), 
							this, SLOT(highlightCurrentLine()));

			updateLineNumbersWidth(0);
			highlightCurrentLine();
		}

		int TextEditorWidget::lineNumbersWidth()
		{
			int max    = std::max(1, blockCount());
			int digits = ((int)log10((float)max))+1;

			int space = 3 + fontMetrics().width(QLatin1Char('9')) * digits;

			return space;
		}

		void TextEditorWidget::updateLineNumbersWidth(int /* newBlockCount */)
		{
			setViewportMargins(lineNumbersWidth(), 0, 0, 0);
		}

		void TextEditorWidget::updateLineNumbers(const QRect &rect, int dy)
		{
			if (dy)
				line_numbers_->scroll(0, dy);
			else
				line_numbers_->update(0, rect.y(), line_numbers_->width(), rect.height());

			if (rect.contains(viewport()->rect()))
				updateLineNumbersWidth(0);
		}

		void TextEditorWidget::resizeEvent(QResizeEvent *e)
		{
			QPlainTextEdit::resizeEvent(e);

			QRect cr = contentsRect();
			line_numbers_->setGeometry(QRect(cr.left(), cr.top(), 
																 lineNumbersWidth(), cr.height()));
		}

		void TextEditorWidget::highlightCurrentLine()
		{
			QList<QTextEdit::ExtraSelection> extraSelections;

			if (!isReadOnly()) 
			{
				QTextEdit::ExtraSelection selection;

				QColor lineColor = QColor(Qt::yellow).lighter(160);

				selection.format.setBackground(lineColor);
				selection.format.setProperty(QTextFormat::FullWidthSelection, true);
				selection.cursor = textCursor();
				selection.cursor.clearSelection();
				extraSelections.append(selection);
			}

			setExtraSelections(extraSelections);
		}

		void TextEditorWidget::lineNumbersPaintEvent(QPaintEvent *event)
		{
			QPainter painter(line_numbers_);
			painter.fillRect(event->rect(), Qt::lightGray);

			QTextBlock block = firstVisibleBlock();
			int blockNumber = block.blockNumber();
			int top = (int) blockBoundingGeometry(block).translated(contentOffset()).top();
			int bottom = top + (int) blockBoundingRect(block).height();

			while (block.isValid() && top <= event->rect().bottom()) 
			{
				if (block.isVisible() && bottom >= event->rect().top()) 
				{
					QString number = QString::number(blockNumber + 1);
					painter.setPen(Qt::black);
					painter.drawText(0, top+3, line_numbers_->width(), fontMetrics().height(),
					                 Qt::AlignRight, number);
				}

				block = block.next();
				top = bottom;
				bottom = top + (int) blockBoundingRect(block).height();
				++blockNumber;
			}
		}	

		TextEditorWidget::LineNumbers::LineNumbers(TextEditorWidget *editor) 
			: QWidget(editor) 
		{
			editor_ = editor;
		}

		QSize TextEditorWidget::LineNumbers::sizeHint() const 
		{
			return QSize(editor_->lineNumbersWidth(), 0);
		}

		void TextEditorWidget::LineNumbers::paintEvent(QPaintEvent *event) 
		{
			editor_->lineNumbersPaintEvent(event); 
		} 
	}
}
