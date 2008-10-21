#ifndef PLUGINDIALOG_H
#define PLUGINDIALOG_H

#include <BALL/VIEW/UIC/pluginDialogData.h>

#include <QtCore/QModelIndex>
#include <QtGui/QItemDelegate>

namespace BALL
{
	namespace VIEW
	{
		class VIEWPlugin;

		class PluginItemDelegate : public QItemDelegate
		{
			Q_OBJECT

			public:
				QSize sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const;
				void paint ( QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index ) const;
		};

		class PluginModel : public QAbstractListModel
		{
			Q_OBJECT

			public:
				PluginModel();
				int rowCount(const QModelIndex& parent = QModelIndex()) const;
				QVariant data(const QModelIndex& i, int role) const;
				void pluginsLoaded();
			private:
				int num_rows_;
		};

		class PluginDialog : public QDialog, private Ui_PluginDialog
		{
			Q_OBJECT

			public:
				PluginDialog(QWidget* parent);
				virtual ~PluginDialog() {}

			protected slots:
				virtual void addPluginDirectory();
				virtual void applyChanges();
				virtual void pluginChanged(QModelIndex i);
				virtual void revertChanges();
				virtual void togglePluginState();

			private:
				VIEWPlugin* active_plugin_;
				PluginModel model_;
		};

	}
}

#endif

