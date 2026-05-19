// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/VIEW/WIDGETS/colorTable.h>

namespace BALL
{
	namespace VIEW
	{
		ColorTable::ColorTable(QWidget* parent, const char* name)
			: QTableWidget(0, 2, parent),
				setting_content_(false)
		{
			setObjectName(name);
			setColumnCount(2);
			setHorizontalHeaderItem(0, new QTableWidgetItem());
			setHorizontalHeaderItem(1, new QTableWidgetItem());
			horizontalHeaderItem(1)->setText("Color");
			setGeometry(5,5, 400, 388);
			setColumnWidth(1, 230);
			setSelectionMode(NoSelection);
		}

		void ColorTable::setNamesTitle(const String& name)
		{
			horizontalHeaderItem(0)->setText(name.c_str());
		}

		String ColorTable::getNamesTitle() const
		{
			return ascii(horizontalHeaderItem(0)->text());
		}

		void ColorTable::setContent(const vector<String>& names, const vector<ColorRGBA>& colors)
		{
			setting_content_ = true;
			colors_ = colors;
			names_ = names;

			setRowCount(colors_.size());
			for (Position p = 0; p < names_.size(); p++)
			{
				QTableWidgetItem* item = new QTableWidgetItem(names_[p].c_str());
				setItem(p,0,item);
				item->setFlags(Qt::ItemIsEnabled);
				item = new QTableWidgetItem();
				item->setBackground(colors_[p].getQColor());
				setItem(p,1,item);
			}
			setting_content_ = false;
		}

		void ColorTable::setColors(const vector<ColorRGBA>& colors)
		{
			setting_content_ = true;
			colors_ = colors;

			setRowCount(colors_.size());
			for (Position p = 0; p < names_.size(); p++)
			{
				QTableWidgetItem* item = new QTableWidgetItem();
				item->setBackground(colors_[p].getQColor());
				setItem(p, 1, item);
			}
			setting_content_ = false;
		}

		void ColorTable::beginEdit(int row, int col)
		{
			if (col == 0 || setting_content_) return;
			ColorRGBA old_rgba(item(row,col)->background().color());
			QColor qcolor = QColorDialog::getColor(old_rgba.getQColor());
			if (!qcolor.isValid()) return;

			ColorRGBA new_color(qcolor);
			item(row,col)->setBackground(new_color.getQColor());
			colors_[row] = new_color;
		}

		bool ColorTable::getValue(String& value) const
		{
			value.clear();

			vector<ColorRGBA> colors = getColors();

			for (Position p = 0; p < colors.size(); p ++)
			{
				value += colors[p];
				value += ";";
			}

			return true;
		}

		bool ColorTable::setValue(const String& value)
		{
			vector<String> fields;
			vector<ColorRGBA> colors;

			Size nr = value.split(fields, ";");

			ColorRGBA color;

			for (Position p = 0; p < nr; p ++)
			{
				color = fields[p];
				colors.push_back(color);
			}

			if (colors.size() != getNames().size())
			{
				// v1.7.x (2026-05-19) — was BALLVIEW_DEBUG (alarming
				// "Please notify on GitHub" message). The size mismatch
				// fires PREDICTABLY for every user with a pre-v1.7 BALLView
				// INI config: the Phase 4.1 ColoringSettingsDialog migration
				// changed the color-table entry count, and stale INI blocks
				// with the old count flow through colorTable's parse path
				// before the dialog's "dropping legacy Elements= block"
				// migration kicks in. The code already returns false here
				// and the caller falls back to compiled defaults — the
				// behaviour is correct; only the log noise is wrong.
				// Downgrade to Log.info so the diagnostic value is preserved
				// without scaring users into filing GitHub issues.
				Log.info() << "ColorTable: dropping stale config block — "
				           << colors.size() << " colors found, "
				           << getNames().size() << " expected (likely pre-v1.7 BALLView config; "
				           << "compiled defaults will be used)" << std::endl;
				return false;
			}

			setColors(colors);

			return true;
		}

		void ColorTable::mousePressEvent(QMouseEvent* event)
		{
			Index c = columnAt(event->pos().x());
			Index r = rowAt(event->pos().y());
			if (c == 0) return;
			if (c != 1 || r < 0)
			{
				QTableWidget::mousePressEvent(event);
				return;
			}

			beginEdit(r, c);
		}
	}
}
