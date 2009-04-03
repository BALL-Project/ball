#include <BALL/VIEW/WIDGETS/shortcutTableView.h>

#ifndef BALL_VIEW_KERNEL_SHORTCUTREGISTRY_H
# include <BALL/VIEW/KERNEL/shortcutRegistry.h>
#endif

#ifndef BALL_VIEW_DIALOGS_EDITSINGLESHORTCUT_H
# include <BALL/VIEW/DIALOGS/editSingleShortcut.h>
#endif

#include <QtCore/QAbstractTableModel>
#include <QtGui/QFontMetrics>
#include <QtGui/QSortFilterProxyModel>

#include <BALL/VIEW/WIDGETS/scene.h>

Q_DECLARE_METATYPE(BALL::String)

namespace BALL
{
	namespace VIEW
	{

		//////////////////////////   MODEL   ////////////////////////////

		class ShortcutTableModel : public QAbstractTableModel
		{
			public:
				ShortcutTableModel(ShortcutRegistry* reg);
				int rowCount(const QModelIndex& parent = QModelIndex()) const;
				int columnCount(const QModelIndex& parent = QModelIndex()) const;

				QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const;
				QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
				Qt::ItemFlags flags(const QModelIndex& index) const;
				bool isValid(const QKeySequence& seq) const;
				bool setData(const QModelIndex& index, const QVariant& data, int role = Qt::EditRole);
			private:
				ShortcutRegistry* registry_;
		};

		ShortcutTableModel::ShortcutTableModel(ShortcutRegistry* reg)
			: registry_(reg)
		{
			if (!registry_)
			{
				throw Exception::NullPointer(__FILE__, __LINE__);
			}
		}

		int ShortcutTableModel::rowCount(const QModelIndex& parent) const
		{
				return registry_->size();
		}

		int ShortcutTableModel::columnCount(const QModelIndex& parent) const
		{
			return 2;
		}

		QVariant ShortcutTableModel::data(const QModelIndex& index, int role) const
		{
			if(index.column() == 0) {
				switch(role) {
					case Qt::DisplayRole: {
						String desc = (*registry_)[index.row()].first;
						desc.substitute("Shortcut|","");
						while (desc.substitute("|","->") != String::EndPos) ;
						while (desc.substitute("_"," ") != String::EndPos) ;

						return QVariant::fromValue(QString(desc.c_str()));
					}
					case Qt::DecorationRole:
						return QVariant::fromValue((*registry_)[index.row()].second->icon());
				}
			} else {
				switch(role) {
					case Qt::DisplayRole:
						return QVariant::fromValue((*registry_)[index.row()].second->shortcut().toString());
				}
			}

			return QVariant();
		}

		QVariant ShortcutTableModel::headerData(int section, Qt::Orientation orientation, int role) const
		{
			switch (role)
			{
				case Qt::DisplayRole:
					if (orientation == Qt::Vertical)
					{
						return QVariant();
					}

					if (section == 0)
					{
						return QVariant::fromValue(QString("Description"));
					}
					else
					{
						return QVariant::fromValue(QString("Key Sequence"));
					}
				default:
					return QVariant();
			}
		}

		Qt::ItemFlags ShortcutTableModel::flags(const QModelIndex& index) const
		{
			return Qt::ItemIsSelectable | Qt::ItemIsEnabled;
		}

		bool ShortcutTableModel::isValid(const QKeySequence& seq) const
		{
			return !registry_->hasKey(seq.toString());
		}

		bool ShortcutTableModel::setData(const QModelIndex& index, const QVariant& data, int /*role*/)
		{
			QKeySequence seq = qvariant_cast<QKeySequence>(data);
			Log.error() << "Setting Data to " << ascii(seq.toString())  << "." << std::endl;

			if ((index.column() == 1) && isValid(seq))
			{
				(*registry_)[index.row()].second->setShortcut(seq);

				// TODO: getDescription(QKeySequence) in shortcutRegistry to provide more helpful message.
				Scene::getInstance(0)->setStatusbarText("Shortcut " + ascii(seq.toString()) + " successfully set.");
				emit dataChanged(index, index);
				return true;
			}

			// TODO: getDescription(QKeySequence) in shortcutRegistry to provide more helpful message.
			Scene::getInstance(0)->setStatusbarText("Shortcut " + ascii(seq.toString()) + " already associated!");

			return false;
		}

		/////////////////////////  ShortcutTableView  ///////////////////////////////

		ShortcutTableView::ShortcutTableView(QWidget* parent)
			: QTableView(parent), edited_row_(-1)
		{
			// Create a new model
			ShortcutTableModel* model = new ShortcutTableModel(ShortcutRegistry::getInstance(0));
			proxy_model_ = new QSortFilterProxyModel(this);
			proxy_model_->setDynamicSortFilter(true);
			proxy_model_->setSourceModel(model);
			proxy_model_->setFilterCaseSensitivity(Qt::CaseInsensitive);

			// Setting up a view to display the items in the model
			setModel(proxy_model_);

			editor_ = new EditSingleShortcut(this);
			connect(editor_, SIGNAL(accepted()), this, SLOT(editSuccess_()));

			connect(this, SIGNAL(doubleClicked(const QModelIndex&)),
			        this, SLOT(  onDoubleClick(const QModelIndex&)));
		}

		void ShortcutTableView::onDoubleClick(const QModelIndex& index)
		{
			if(!index.isValid() || editor_->isVisible()) {
				return;
			}

			QModelIndex sc_index = index.model()->index(index.row(), 1, QModelIndex());

			edited_row_ = sc_index.row();
			editor_->setShortcutText(sc_index.data().toString());
			editor_->show();
		}

		void ShortcutTableView::setFilter(const QString& filter)
		{
			proxy_model_->invalidate();
			proxy_model_->setFilterFixedString(filter);
		}

		void ShortcutTableView::editSuccess_()
		{
			if(edited_row_ < 0) {
				return;
			}

			model()->setData(model()->index(edited_row_, 1, QModelIndex()),
			                 QVariant::fromValue(editor_->getKeySequence()));
		}
	}
}

