#include <BALL/VIEW/DIALOGS/pluginDialog.h>
#include <BALL/PLUGIN/pluginManager.h>
#include <BALL/PLUGIN/BALLPlugin.h>
#include <BALL/VIEW/PLUGIN/VIEWPlugin.h>
#include <BALL/VIEW/INPUT/inputPlugin.h>
#include <BALL/COMMON/logStream.h>
#include <BALL/VIEW/WIDGETS/scene.h>

Q_DECLARE_METATYPE(BALL::VIEW::VIEWPlugin*)
Q_DECLARE_METATYPE(BALL::BALLPlugin*)
Q_DECLARE_METATYPE(BALL::VIEW::InputPlugin*)

#include <QtGui/QFileDialog>
#include <QtCore/QObject>
#include <QtCore/QDebug>

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
			if(!i.isValid()) {
				return QVariant();
			}

			PluginManager& man = PluginManager::instance();

			if(i.row() >= num_rows_) {
				return QVariant();
			}

			switch(role) {
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
						return QIcon(*(ptr->getIcon()));
					else 
						return QVariant();
				}
				default:
					return QVariant();
			}
		}

		PluginDialog::PluginDialog(QWidget* parent)
			: QDialog(parent)
		{
			setupUi(this);
			pluginView->setModel(&model_);
		}

		void PluginDialog::applyChanges()
		{
		}

		void PluginDialog::revertChanges()
		{
		}

		void PluginDialog::pluginChanged(QModelIndex i)
		{
			active_index_ = i;
			QObject* active_object = qvariant_cast<QObject*>(model_.data(i, Qt::UserRole));

			BALLPlugin* active_plugin;
			if(!(active_plugin = qobject_cast<BALLPlugin*>(active_object))) {
				return;
			}

			togglePluginButton->setEnabled(true);
			if(active_plugin->isActive()) {
				togglePluginButton->setText("Deactivate");
			} else {
				togglePluginButton->setText("Activate");
			}

			VIEWPlugin* active_view_plugin = qvariant_cast<VIEWPlugin*>(active_object);
			if (active_view_plugin)
			{
				QWidget* cfg_dialog = active_view_plugin->getConfigDialog();

				if(cfg_dialog) {
					settingsButton->setEnabled(true);
					connect(settingsButton, SIGNAL(clicked()), cfg_dialog, SLOT(show()));
				} else {
					settingsButton->setEnabled(false);
				}
			}
			else
				settingsButton->setEnabled(false);
		}

		void PluginDialog::addPluginDirectory()
		{
			QString dir = QFileDialog::getExistingDirectory(0, "Select a plugin directory", "/home/daniel/source/BALL");
			PluginManager& man = PluginManager::instance();
			man.setPluginDirectory(dir);
			model_.pluginsLoaded();
		}

		void PluginDialog::togglePluginState()
		{
			if(!active_index_.isValid()) {
				return;
			}

			QObject* active_object = qvariant_cast<QObject*>(model_.data(active_index_, Qt::UserRole));
			BALLPlugin* active_plugin = qobject_cast<BALLPlugin*>(active_object);
			if(active_plugin->isActive()) {
				active_plugin->deactivate();
				togglePluginButton->setText("Activate");
			} else {
				active_plugin->activate();

				// TODO: this should be somewhere else! :-)
				InputPlugin* ptr = qobject_cast<InputPlugin*>(active_object);
				ptr->setReceiver(Scene::getInstance(0));

				togglePluginButton->setText("Deactivate");
			}
		}

	}
}

