#include <BALL/PLUGIN/pluginManager.h>
#include <BALL/PLUGIN/BALLPlugin.h>
#include <BALL/COMMON/logStream.h>

#include <BALL/VIEW/WIDGETS/scene.h>
#include <BALL/VIEW/KERNEL/mainControl.h>
#include <BALL/VIEW/KERNEL/iconLoader.h>
#include <BALL/VIEW/DIALOGS/preferences.h>
#include <BALL/VIEW/DIALOGS/displayProperties.h>
#include <BALL/VIEW/DIALOGS/pluginDialog.h>
#include <BALL/VIEW/PLUGIN/VIEWPlugin.h>
#include <BALL/STRUCTURE/bindingPocketProcessor.h>

#include <BALL/VIEW/UIC/ui_pluginDialog.h>

Q_DECLARE_METATYPE(BALL::VIEW::VIEWPlugin*)

#include <QtGui/QFileDialog>
#include <QtCore/QObject>
#include <QtCore/QDebug>
#include <QtCore/QLibraryInfo>

namespace BALL
{
	namespace VIEW
	{
		/*
		 * Implementation of PluginModel
		 */
		PluginModel::PluginModel(QObject* parent)
			: QAbstractListModel(parent),
			  num_rows_(0)
		{
		}

		int PluginModel::rowCount(const QModelIndex& parent) const
		{
			Q_UNUSED(parent)

			return num_rows_;
		}

		void PluginModel::pluginsLoaded()
		{
			int nrow = num_rows_;
			num_rows_ = PluginManager::instance().getPluginCount();
			beginInsertRows(QModelIndex(), nrow, num_rows_);
			endInsertRows();
		}

		QVariant PluginModel::data(const QModelIndex& i, int role) const
		{
			if(!i.isValid())
			{
				return QVariant();
			}

			PluginManager& man = PluginManager::instance();

			if(i.row() >= num_rows_)
			{
				return QVariant();
			}

			switch(role)
			{
				case Qt::UserRole:
					return qVariantFromValue(man.getPluginInstance(i.row()));
				case Qt::DisplayRole:
				{
					BALLPlugin* ptr = qobject_cast<BALLPlugin*>(man.getPluginInstance(i.row()));

					// we *know* that this is a BALLPlugin (the loader takes care of that), so we don't need
					// to test if the cast succeeded
					return ptr->getName() + " (" + (ptr->isActive() ? tr("active") : tr("inactive")) + ")";
				}
				case Qt::DecorationRole:
				{
					VIEWPlugin* ptr = qobject_cast<VIEWPlugin*>(man.getPluginInstance(i.row()));

					if (ptr)
						return *(ptr->getIcon());
					else
						return QVariant();
				}
				default:
					return QVariant();
			}
		}

		/*
		 * Implementation of PluginDirectoryModel
		 */

		PluginDirectoryModel::PluginDirectoryModel(QObject* parent)
			: QAbstractListModel(parent)
		{
		}

		int PluginDirectoryModel::rowCount(const QModelIndex& parent) const
		{
			Q_UNUSED(parent);

			PluginManager& man = PluginManager::instance();
			return man.getPluginDirectories().size();
		}

		QVariant PluginDirectoryModel::data(const QModelIndex& i, int role) const
		{
			if(!i.isValid()) {
				return QVariant();
			}

			PluginManager& man = PluginManager::instance();

			if((size_t)i.row() >= man.getPluginDirectories().size())
			{
				return QVariant();
			}

			switch(role)
			{
				case Qt::DisplayRole:
				{
					return man.getPluginDirectories()[i.row()];
				}

				default:
					return QVariant();
			}
		}

		void PluginDirectoryModel::addDirectory(const QString& dir)
		{
			PluginManager& man = PluginManager::instance();

			if(man.addPluginDirectory(dir)) {
				int idx = man.getPluginDirectories().size() - 1;

				beginInsertRows(QModelIndex(), idx, idx);
				endInsertRows();
			}
		}

		void PluginDirectoryModel::removeDirectory(const QModelIndex& idx)
		{
			PluginManager& man = PluginManager::instance();

			if(man.removePluginDirectory(idx.data(Qt::DisplayRole).toString())) {
				beginRemoveRows(QModelIndex(), idx.row(), idx.row());
				endRemoveRows();
			}
		}

		/*
		 * Implementation of PluginDialog
		 */
		PluginDialog::PluginDialog(Preferences* preferences, QWidget* parent, const char* name)
			: QWidget(parent),
			  ModularWidget(name),
			  preferences_(preferences)
		{
			ui_ = new Ui::PluginDialogData;
			ui_->setupUi(this);

			// define the dialogs section name in the INIFile
			setINIFileSectionName("PLUGINS");
			stack_pages_.push_back(std::make_pair(this, tr("Plugins").toStdString()));

			preferences_->insertEntry(this);

			setObjectName(name);

			IconLoader& icon_loader = IconLoader::instance();

			ui_->plugin_dir_button_add->setIcon(icon_loader.getIcon("actions/list-add"));
			ui_->plugin_dir_button_remove->setIcon(icon_loader.getIcon("actions/edit-delete"));

			// register all visible childs
			registerWidgets_();

			//TODO
			//registerWidgetForHelpSystem_(widget_stack->widget(0), "plugins.html#plugins");

			ui_->plugin_view->setModel(&plugin_model_);
			ui_->plugin_directories_view->setModel(&plugin_dir_model_);

//			QString plugin_path(BALL_PATH);
//			plugin_path += "/plugins";
			QString plugin_path(QLibraryInfo::location(QLibraryInfo::PluginsPath));

			plugin_dir_model_.addDirectory(plugin_path);
			plugin_model_.pluginsLoaded();

			setObjectName(name);
			ModularWidget::registerWidget(this);

			connect(ui_->plugin_dir_button_add, SIGNAL(clicked()), this, SLOT(addPluginDirectory()));
			connect(ui_->plugin_dir_button_remove, SIGNAL(clicked()), this, SLOT(removePluginDirectory()));
			connect(ui_->plugin_directories_view, SIGNAL(activated(const QModelIndex&)), this, SLOT(directorySelectionChanged(const QModelIndex& )));
		}

		PluginDialog::~PluginDialog()
		{
			PluginManager::instance().unregisterHandler(this);
			delete ui_;
		}

		void PluginDialog::initializeWidget(MainControl&)
		{
			PreferencesEntry::restoreValues();
		}

		void PluginDialog::finalizeWidget(MainControl& mc)
		{
			ModularWidget::finalizeWidget(mc);
			PluginManager::instance().unloadAllPlugins();
		}

		void PluginDialog::writePreferenceEntries(INIFile& inifile)
		{
			PreferencesEntry::writePreferenceEntries(inifile);

			for (std::list<PreferencesEntry*>::iterator child_it = child_entries_.begin(); child_it != child_entries_.end(); ++child_it)
			{
				(*child_it)->writePreferenceEntries(inifile);
			}

			PluginManager& man = PluginManager::instance();

			if(!inifile.hasSection(getINIFileSectionName())) {
				inifile.appendSection(getINIFileSectionName());
			}

			String value;
			man.getPluginDirectories(value);

			inifile.insertValue(getINIFileSectionName(), "PluginDirectories", value);
			inifile.insertValue(getINIFileSectionName(), "ActivePlugins", man.getAutoActivatePlugins().toStdString());
		}

		void PluginDialog::readPreferenceEntries(const INIFile& inifile)
		{
			PreferencesEntry::readPreferenceEntries(inifile);

			plugin_model_.pluginsLoaded();

			for (std::list<PreferencesEntry*>::iterator child_it = child_entries_.begin(); 
					 child_it != child_entries_.end(); 
					 ++child_it)
			{
				(*child_it)->readPreferenceEntries(inifile);
			}

			PluginManager& man = PluginManager::instance();

			if(inifile.hasEntry(getINIFileSectionName(), "ActivePlugins")) {
				String active_plugins = inifile.getValue(getINIFileSectionName(), "ActivePlugins");
				man.setAutoActivatePlugins(active_plugins.c_str());
			}

			if(inifile.hasEntry(getINIFileSectionName(), "PluginDirectories")) {
				String plugin_directories = inifile.getValue(getINIFileSectionName(), "PluginDirectories");
				man.setPluginDirectories(plugin_directories);
			}
		}

		void PluginDialog::pluginChanged(QModelIndex i)
		{
			//Cleanup the old config dialog
			active_index_ = i;
			QObject* active_object = qvariant_cast<QObject*>(plugin_model_.data(i, Qt::UserRole));

			BALLPlugin* active_plugin = qobject_cast<BALLPlugin*>(active_object);
			if (!active_plugin)
			{
				return;
			}

			ui_->plugin_toggle_button->setEnabled(true);
			if (active_plugin->isActive())
			{
				ui_->plugin_toggle_button->setText(tr("Deactivate"));
			}
			else
			{
				ui_->plugin_toggle_button->setText(tr("Activate"));
			}
		}

		void PluginDialog::addPluginDirectory()
		{
			QString plugin_path(BALL_PATH);
			plugin_path += "/plugins";

			QString dir = QFileDialog::getExistingDirectory(0, tr("Select a plugin directory"), plugin_path);
			plugin_dir_model_.addDirectory(dir);
			plugin_model_.pluginsLoaded();
		}

		void PluginDialog::removePluginDirectory()
		{
			// TODO: disable remove button if no item selected
			if (!ui_->plugin_directories_view->currentIndex().isValid())
			{
				return;
			}

			plugin_dir_model_.removeDirectory(ui_->plugin_directories_view->currentIndex());
			plugin_model_.pluginsLoaded();
		}

		void PluginDialog::directorySelectionChanged(const QModelIndex& idx)
		{
			ui_->plugin_dir_button_remove->setEnabled(idx.isValid());
		}

		void PluginDialog::togglePluginState()
		{
			if (!active_index_.isValid())
			{
				return;
			}

			QObject* active_object = qvariant_cast<QObject*>(plugin_model_.data(active_index_, Qt::UserRole));
			BALLPlugin* active_plugin = qobject_cast<BALLPlugin*>(active_object);
			if (active_plugin->isActive())
			{
				PluginManager::instance().doNotAutoActivatePlugin(active_plugin->getName());
				PluginManager::instance().stopPlugin(active_plugin);
				ui_->plugin_toggle_button->setText(tr("Activate"));
			}
			else
			{
				PluginManager::instance().autoActivatePlugin(active_plugin->getName());
				PluginManager::instance().startPlugin(active_plugin);
				ui_->plugin_toggle_button->setText(tr("Deactivate"));
			}

			ui_->plugin_view->update(active_index_);
		}

		void PluginDialog::registerChildEntry(PreferencesEntry* child)
		{
			if(!child) return;

			preferences_->insertChildEntry("PLUGINS", child);
			child_entries_.push_back(child);
		}

		void PluginDialog::unregisterChildEntry(PreferencesEntry* child)
		{
			if(!child) return;

			std::list<PreferencesEntry*>::iterator child_it = std::find(child_entries_.begin(), child_entries_.end(), child);
			child_entries_.erase(child_it);
			preferences_->removeEntry(child);
		}

		bool PluginDialog::canHandle(BALLPlugin* plugin) const
		{
			return dynamic_cast<VIEWPlugin*>(plugin) != 0;
		}

		bool PluginDialog::specificSetup_(BALLPlugin* plugin)
		{
			VIEWPlugin* view_plugin = dynamic_cast<VIEWPlugin*>(plugin);

			if(view_plugin->getConfigDialog() != 0 && preferences_ != 0)
			{
				registerChildEntry(view_plugin->getConfigDialog());
			}

			return true;
		}

		bool PluginDialog::specificShutdown_(BALLPlugin* plugin)
		{
			VIEWPlugin* view_plugin = dynamic_cast<VIEWPlugin*>(plugin);

			if(view_plugin->getConfigDialog() != 0 && preferences_ != 0)
			{
				unregisterChildEntry(view_plugin->getConfigDialog());
				view_plugin->getConfigDialog()->setParent(0);
			}

			return true;
		}

	}
}
