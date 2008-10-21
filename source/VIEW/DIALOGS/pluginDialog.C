#include <BALL/VIEW/DIALOGS/pluginDialog.h>
#include <BALL/VIEW/PLUGIN/pluginManager.h>
#include <BALL/VIEW/PLUGIN/VIEWPlugin.h>

Q_DECLARE_METATYPE(BALL::VIEW::VIEWPlugin*)

#include <QtGui/QFileDialog>

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
					return man.getPluginInstance(i.row());
				case Qt::DisplayRole:
					return man.getPluginInstance(i.row())->getName();
				case Qt::DecorationRole:
					return QIcon(*man.getPluginInstance(i.row())->getIcon());
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
			if(!(active_plugin_ = qvariant_cast<VIEWPlugin*>(model_.data(i, Qt::UserRole)))) {
				return;
			}

			if(active_plugin_->isActive()) {
				togglePluginButton->setText("Deactivate");
			} else {
				togglePluginButton->setText("Activate");
			}

			QWidget* cfg_dialog = active_plugin_->getConfigDialog();

			if(cfg_dialog) {
				settingsButton->setEnabled(true);
				connect(settingsButton, SIGNAL(clicked()), cfg_dialog, SLOT(show()));
			} else {
				settingsButton->setEnabled(false);
			}
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
			if(!active_plugin_) {
				return;
			}

			if(active_plugin_->isActive()) {
				active_plugin_->deactivate();
				togglePluginButton->setText("Activate");
			} else {
				active_plugin_->activate();
				togglePluginButton->setText("Deactivate");
			}
		}

	}
}

