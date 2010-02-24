#ifndef BALL_VIEW_WIDGETS_COLOR_TABLE_H
#include <QtGui/QTableWidget>

#ifndef BALL_VIEW_KERNEL_PREFERENCESENTRY
# include <BALL/VIEW/KERNEL/preferencesEntry.h>
#endif

#ifndef BALL_VIEW_KERNEL_COMMON_H
# include <BALL/VIEW/KERNEL/common.h>
#endif

#ifndef BALL_VIEW_DATATYPE_COLORRGBA_H
# include <BALL/VIEW/DATATYPE/colorRGBA.h>
#endif

namespace BALL
{
	namespace VIEW
	{
		/** Class for storing colors in a GUI table
				\ingroup ViewDialogs
		*/
		class BALL_VIEW_EXPORT ColorTable
			:	public QTableWidget,
				public PreferencesEntry::ExtendedPreferencesObject
		{
				Q_OBJECT

			public:
				ColorTable(QWidget* parent = 0, const char* name = 0);

				void setNamesTitle(const String& string);

				String getNamesTitle() const;

				void setContent(const vector<String>& names, const vector<ColorRGBA>& colors);

				void setColors(const vector<ColorRGBA>& colors);

				const vector<ColorRGBA>& getColors() const
					{ return colors_;}

				const vector<String>& getNames() const
					{ return names_;}

				///
				virtual bool getValue(String&) const;

				///
				virtual bool setValue(const String& value);

			private slots:

				void beginEdit(int row, int col);

				virtual void mousePressEvent(QMouseEvent* event);

			private:
				vector<ColorRGBA> colors_;
				vector<String>    names_;
				bool              setting_content_;
		};
	}
}

#endif //BALL_VIEW_WIDGETS_COLOR_TABLE_H
