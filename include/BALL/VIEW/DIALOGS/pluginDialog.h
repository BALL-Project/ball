#ifndef BALL_VIEW_PLUGINDIALOG_H
#define BALL_VIEW_PLUGINDIALOG_H

#ifndef BALL_VIEW_KERNEL_PREFERENCESENTRY
# include <BALL/VIEW/KERNEL/preferencesEntry.h>
#endif

#ifndef BALL_VIEW_KERNEL_MODULARWIDGET_H
# include <BALL/VIEW/KERNEL/modularWidget.h>
#endif

#ifndef BALL_PLUGIN_PLUGINHANDLER_H
# include <BALL/PLUGIN/pluginHandler.h>
#endif

#include <QtCore/QModelIndex>
#include <QtWidgets/QItemDelegate>
#include <QtWidgets/QDialog>

namespace Ui
{
	class PluginDialogData;
}

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

		/**
		 * A wrapper around the plugin manager that provides
		 * a model on the currently loaded plugins
		 */
		class PluginModel : public QAbstractListModel
		{
			Q_OBJECT

			public:
				explicit PluginModel ( QObject* parent = 0 );

				int rowCount(const QModelIndex& parent = QModelIndex()) const;
				QVariant data(const QModelIndex& i, int role) const;
				void pluginsLoaded();
			private:
				int num_rows_;
		};

		/**
		 * A wrapper around the plugin manager that provides a
		 * model on the current plugin directories
		 */
		class PluginDirectoryModel : public QAbstractListModel
		{
			Q_OBJECT

			public:
				explicit PluginDirectoryModel ( QObject* parent = 0 );

				int rowCount(const QModelIndex& parent = QModelIndex()) const;
				QVariant data(const QModelIndex& i, int role) const;

				void addDirectory(const QString& dir);
				void removeDirectory(const QModelIndex& index);
		};

		/**
		 * Dialog for handling the BALL plugins
		 *
		 * This dialog shows all available plugins in a QListView. The user can opt to activate
		 * or deactivate any plugin and manipulate the plugin search path. Also this dialog acts as
		 * a plugin handler for VIEWPlugins. It is responsible for registering the ConfigDialog of a
		 * VIEWPlugin with the preferences system.
		 */
		class BALL_VIEW_EXPORT PluginDialog 
			: public QWidget,
			  public ModularWidget,
			  public PreferencesEntry,
			  public PluginHandler
		{
			Q_OBJECT

			public:
				BALL_EMBEDDABLE(PluginDialog, ModularWidget)

				PluginDialog(Preferences* preferences, QWidget* parent, const char *name = "PluginDialog");
				virtual ~PluginDialog();

				/**
				 * Initialization.
				 * This method is called automatically before the main
				 * application is started.
				 * It adds the dialog's menu entries and connections.
				 */
				virtual void initializeWidget(MainControl& main_control);

				/**
				 * Finalization
				 * This method is called automatically before the main
				 * application is closed.
				 */
				 virtual void finalizeWidget(MainControl& main_control);

				 virtual void writePreferenceEntries(INIFile& inifile);
				 virtual void readPreferenceEntries(const INIFile& inifile);

				 virtual void registerChildEntry(PreferencesEntry* child);
				 virtual void unregisterChildEntry(PreferencesEntry* child);

				 bool canHandle(BALLPlugin* plugin) const;
				 bool specificSetup_(BALLPlugin* plugin);
				 bool specificShutdown_(BALLPlugin* plugin);

			protected slots:
				virtual void addPluginDirectory();
				virtual void removePluginDirectory();
				virtual void directorySelectionChanged(const QModelIndex&);
				virtual void pluginChanged(QModelIndex i);
				virtual void togglePluginState();

			private:
				QModelIndex active_index_;
				PluginModel plugin_model_;
				PluginDirectoryModel plugin_dir_model_;
				Preferences* preferences_;

				Ui::PluginDialogData* ui_;
				std::list<PreferencesEntry*> child_entries_;
		};
	}
}

#endif

