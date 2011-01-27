#include <BALL/VIEW/DIALOGS/pluginDialog.h>
#include <BALL/PLUGIN/pluginManager.h>
#include <BALL/PLUGIN/BALLPlugin.h>
#include <BALL/VIEW/PLUGIN/VIEWPlugin.h>
#include <BALL/COMMON/logStream.h>
#include <BALL/VIEW/WIDGETS/scene.h>
#include <BALL/VIEW/KERNEL/mainControl.h>

#include <BALL/VIEW/PLUGIN/inputDevPluginHandler.h>

Q_DECLARE_METATYPE(BALL::VIEW::VIEWPlugin*)

#include <QtGui/QFileDialog>
#include <QtCore/QObject>
#include <QtCore/QDebug>
#include <QtCore/QLibraryInfo>

namespace BALL
{
	namespace VIEW
	{

/*		PluginItemDelegate::sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const
		{

		}

		void PluginItemDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index ) const
		{
			option.decorationSize = QSize(64, 64);
			drawBackground(painter, option, index);
			drawDecoration(painter, option, index);

			painter->drawText(option.decorationSize().width() + )
		}
*/
		PluginModel::PluginModel()
			: num_rows_(0)
		{
			PluginManager& man = PluginManager::instance();
			man.registerHandler(new InputDevPluginHandler());
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
					return ptr->getName();
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

		
		/////////////////////////////////////////////////////////////////


		PluginDialog::PluginDialog(QWidget* parent, const char* name)
			: QDialog(parent),	
				Ui_PluginDialogData(),
				ModularWidget(name)
		{
			setupUi(this);
			
			// define the dialogs section name in the INIFile
			setINIFileSectionName("BALLVIEW_PLUGIN_DIRECTORIES");

			setObjectName(name);

			// register all visible childs
			registerWidgets_();	
			// register the PluginManager explicitly; it is not a QWidget
			registerObject_(&(PluginManager::instance()));
			
			//TODO
			//registerWidgetForHelpSystem_(widget_stack->widget(0), "plugins.html#plugins");

			plugin_view->setModel(&model_);

//			QString plugin_path(BALL_PATH);
//			plugin_path += "/plugins";
			QString plugin_path(QLibraryInfo::location(QLibraryInfo::PluginsPath));

			PluginManager& man = PluginManager::instance();
			man.addPluginDirectory(plugin_path);
			model_.pluginsLoaded();

			setObjectName(name);
			ModularWidget::registerWidget(this);
			hide();	
			
			connect(plugin_dir_button_add, SIGNAL(clicked()), this, SLOT(addPluginDirectory()));
			connect(plugin_dir_button_remove, SIGNAL(clicked()), this, SLOT(removePluginDirectory()));
			connect(plugin_directories_view, SIGNAL(itemSelectionChanged()), this, SLOT(directorySelectionChanged()));
		}

		void PluginDialog::initializeWidget(MainControl&)
		{	
			insertMenuEntry(MainControl::TOOLS, tr("Load Plugin"), this, 
			                SLOT(show()), "Shortcut|Tools|Load_Plugin", QKeySequence(),
											tr(""), UIOperationMode::MODE_ADVANCED);	
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
		}

		void PluginDialog::readPreferenceEntries(const INIFile& inifile)
		{
			PreferencesEntry::readPreferenceEntries(inifile);
			model_.pluginsLoaded();

			vector<QString> plugin_dirs = PluginManager::instance().getPluginDirectories();

			for (Size i=0; i<plugin_dirs.size(); i++)
			{
				plugin_directories_view->addItem(plugin_dirs[i]);
			}

			for (std::list<PreferencesEntry*>::iterator child_it = child_entries_.begin(); 
					 child_it != child_entries_.end(); 
					 ++child_it)
			{
				(*child_it)->readPreferenceEntries(inifile);
			}

			if (activate_all_plugins_check_box->isChecked())
			{
				PluginManager& pm = PluginManager::instance();

				for (int i=0; i<pm.getPluginCount(); ++i)
				{
					pm.startPlugin(i);
				}
			}
		}

/*		void PluginDialog::close()
		{
			hide();
		}

		void PluginDialog::reject()
		{
			hide();
		}
*/
		
		void PluginDialog::pluginChanged(QModelIndex i)
		{
			active_index_ = i;
			QObject* active_object = qvariant_cast<QObject*>(model_.data(i, Qt::UserRole));

			BALLPlugin* active_plugin;
			if (!(active_plugin = qobject_cast<BALLPlugin*>(active_object))) 
			{
				return;
			}
			
			plugin_toggle_button->setEnabled(true);
			if (active_plugin->isActive()) 
			{
				plugin_toggle_button->setText(tr("Deactivate"));
			} 
			else 
			{
				plugin_toggle_button->setText(tr("Activate"));
			}

			VIEWPlugin* active_view_plugin = qobject_cast<VIEWPlugin*>(active_object);
			if (active_view_plugin)
			{
				QWidget* cfg_dialog = active_view_plugin->getConfigDialog();

				if (cfg_dialog) 
				{
					settings_button->setEnabled(true);
					connect(settings_button, SIGNAL(clicked()), cfg_dialog, SLOT(show()));
				} 
				else 
				{
					settings_button->setEnabled(false);
				}
			}
			else
				settings_button->setEnabled(false);
		}

		void PluginDialog::addPluginDirectory()
		{
			QString plugin_path(BALL_PATH);
			plugin_path += "/plugins";

			QString dir = QFileDialog::getExistingDirectory(0, tr("Select a plugin directory"), plugin_path);
			PluginManager& man = PluginManager::instance();
			man.addPluginDirectory(dir, activate_all_plugins_check_box->isChecked());
			model_.pluginsLoaded();

			plugin_directories_view->addItem(dir);
		}

		void PluginDialog::removePluginDirectory()
		{
			// TODO: disable remove button if no item selected
			if (!plugin_directories_view->currentItem())
			{
				return;
			}

			QListWidgetItem* item = plugin_directories_view->takeItem(plugin_directories_view->currentRow());

			PluginManager& man = PluginManager::instance();
			man.removePluginDirectory(item->text());

			model_.pluginsLoaded();

			delete(item);
		}

		void PluginDialog::directorySelectionChanged()
		{
			if (plugin_directories_view->selectedItems().size() > 0)
				plugin_dir_button_remove->setEnabled(true);
			else
				plugin_dir_button_remove->setEnabled(false);
		}

		void PluginDialog::togglePluginState()
		{
			if (!active_index_.isValid()) 
			{
				return;
			}

			QObject* active_object = qvariant_cast<QObject*>(model_.data(active_index_, Qt::UserRole));
			BALLPlugin* active_plugin = qobject_cast<BALLPlugin*>(active_object);
			if (active_plugin->isActive()) 
			{
				PluginManager::instance().stopPlugin(active_plugin);
				plugin_toggle_button->setText(tr("Activate"));
			} 
			else 
			{
				PluginManager::instance().startPlugin(active_plugin);
				plugin_toggle_button->setText(tr("Deactivate"));
			}
		}

		void PluginDialog::registerChildEntry(PreferencesEntry* child)
		{
			child_entries_.push_back(child);
		}

		void PluginDialog::unregisterChildEntry(PreferencesEntry* child)
		{
			std::list<PreferencesEntry*>::iterator child_it = std::find(child_entries_.begin(), child_entries_.end(), child);
			child_entries_.erase(child_it);
		}

	}
}

