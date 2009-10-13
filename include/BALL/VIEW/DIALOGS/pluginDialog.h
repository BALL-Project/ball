#ifndef BALL_VIEW_PLUGINDIALOG_H
#define BALL_VIEW_PLUGINDIALOG_H

#ifndef BALL_VIEW_KERNEL_PREFERENCESENTRY
# include <BALL/VIEW/KERNEL/preferencesEntry.h>
#endif

#ifndef BALL_VIEW_KERNEL_MODULARWIDGET_H
# include <BALL/VIEW/KERNEL/modularWidget.h>
#endif

#include <BALL/VIEW/UIC/ui_pluginDialog.h>

#include <QtCore/QModelIndex>
#include <QtGui/QItemDelegate>
#include <QtGui/QDialog>

namespace BALL
{
	namespace VIEW
	{
		class VIEWPlugin;

/*		class PluginItemDelegate : public QItemDelegate
		{
			Q_OBJECT

			public:
				QSize sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const;
				void paint ( QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index ) const;
		};
*/
		class PluginModel : 
			public QAbstractListModel	
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


		
		/** Dialog for handling the BALLView plugins
		*/
		class PluginDialog 
			: public QDialog, 
				private Ui_PluginDialogData,
				public ModularWidget,
				public PreferencesEntry
		{
			Q_OBJECT

			BALL_EMBEDDABLE(PluginDialog, ModularWidget)
			
			public:
				PluginDialog(QWidget* parent, const char *name = "PluginDialog");
				virtual ~PluginDialog() {}

			/** Initialization. 
			 		This method is called automatically before the main 
					application is started. 
					It adds the	dialog's menu entries and connections.
			*/
			virtual void initializeWidget(MainControl& main_control);

			/** Finalization 
			 		This method is called automatically before the main 
					application is closed. 
			*/
			virtual void finalizeWidget(MainControl& main_control);

			virtual void readPreferenceEntries(const INIFile& inifile);

			protected slots:
				virtual void addPluginDirectory();
				virtual void removePluginDirectory();
				virtual void directorySelectionChanged();
				//virtual void close();
				//virtual void reject();
				virtual void pluginChanged(QModelIndex i);
				virtual void togglePluginState();

			private:
				QModelIndex active_index_;
				PluginModel model_;
		};

	}
}

#endif

