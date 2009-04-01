#include <BALL/VIEW/WIDGETS/shortcutTableView.h>

#ifndef BALL_VIEW_KERNEL_SHORTCUTREGISTRY_H
# include <BALL/VIEW/KERNEL/shortcutRegistry.h>
#endif

#include <QtCore/QAbstractTableModel>

#include <BALL/VIEW/WIDGETS/scene.h>

Q_DECLARE_METATYPE(BALL::String)

namespace BALL
{
	namespace VIEW
	{

		//////////////////  INSERTER  ////////////////////////////

		ShortcutInserter::ShortcutInserter(QWidget* parent)
			: EditSingleShortcut(parent), 
			  modifiers_(0), 
				key_(0)
		{
			setShortcutText("Please insert your shortcut now.");
			setErrorText("");

			grabKeyboard();
		}

		ShortcutInserter::~ShortcutInserter()
		{
		}

		void ShortcutInserter::keyPressEvent(QKeyEvent* evt)
		{
			// Qt delivered a garbage keycode; Ignore it!
			if (evt->key() == -1) 
			{
				return;
			}

			// If AltGr is pressed there is a problem with unicode
			// So just bail out...

			evt->accept();

			switch (evt->key()) 
			{
				case Qt::Key_AltGr: //or else we get unicode salad
					return;
		    case Qt::Key_Shift:
				case Qt::Key_Control:
		    case Qt::Key_Alt:
		    case Qt::Key_Meta:
		    case Qt::Key_Menu: //unused (yes, but why?)
					break;
				default:
					key_ = evt->key();
			}

			if ((key_ == 0) || (key_ == Qt::Key_unknown)) 
			{
				// Filter the interesting modifiers
				modifiers_ = evt->modifiers() & (Qt::ControlModifier | Qt::AltModifier |
				                                 Qt::ShiftModifier | Qt::MetaModifier);
			}

			new_sequence_ = QKeySequence(key_ | modifiers_);
			edited_ = true;
			updateText_();
		}

		void ShortcutInserter::keyReleaseEvent(QKeyEvent* evt)
		{
			// Qt delivered a garbage keycode; Ignore it!
			if (evt->key() == -1) {
				return;
			}

			bool released_key = false;

			switch (evt->key()) {
				case Qt::Key_AltGr: //or else we get unicode salad
					return;
		    case Qt::Key_Shift:
				case Qt::Key_Control:
		    case Qt::Key_Alt:
		    case Qt::Key_Meta:
		    case Qt::Key_Menu: //unused (yes, but why?)
					break;
				default:
					released_key = true;
					key_ = evt->key();
			}

			if ((key_ == 0) || (key_ == Qt::Key_unknown)) 
			{
				// Filter the interesting modifiers
				modifiers_ = evt->modifiers() & (Qt::ControlModifier | Qt::AltModifier |
					                               Qt::ShiftModifier | Qt::MetaModifier);
			}

			new_sequence_ = QKeySequence(key_ | modifiers_);
			evt->accept();

			updateText_();
			//key_=0;
			//modifiers_=0;
		}

		void ShortcutInserter::updateText_()
		{
			QString s = new_sequence_.toString(QKeySequence::NativeText);
			s.replace('&', QLatin1String("&&"));

			setShortcutText(s);
			
			if (ShortcutRegistry::getInstance(0)->hasKey(new_sequence_))
			{
				setErrorText("Shortcut already in use.");
			}
			else
			{
				setErrorText("");
			}
		}

		
		//////////////////////////   MODEL   ////////////////////////////

		class ShortcutTableModel : public QAbstractTableModel
		{
			public:
				ShortcutTableModel(ShortcutRegistry* reg);
				int rowCount(const QModelIndex& parent = QModelIndex()) const;
				int columnCount(const QModelIndex& parent = QModelIndex()) const;

				QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const;
				QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
				void sort ( int column, Qt::SortOrder order = Qt::AscendingOrder);
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
			switch (role) 
			{
				case Qt::DisplayRole:
					if (index.column() == 0) 
					{
						String desc = (*registry_)[index.row()].first;
						desc.substitute("Shortcut|","");
						while (desc.substitute("|","->") != String::EndPos) ;
						while (desc.substitute("_"," ") != String::EndPos) ;

						return QVariant::fromValue(QString(desc.c_str()));
					} 
					else 
					{
						return QVariant::fromValue((*registry_)[index.row()].second->shortcut().toString());
					}
					break;
				default:
					return QVariant();
			}
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
		
		void ShortcutTableModel::sort ( int column, Qt::SortOrder order)
		{
			std::cout << "TODO sort" << std::endl;
			//TODO connect the QHeaderView::sectionClicked() signal to the QTableView::sortByColumn() slot
		}

		Qt::ItemFlags ShortcutTableModel::flags(const QModelIndex& index) const
		{
			if (index.column() == 1) 
			{
				return Qt::ItemIsEditable | Qt::ItemIsSelectable | Qt::ItemIsEnabled;
			}

			return Qt::ItemIsSelectable | Qt::ItemIsEnabled;
		}

		bool ShortcutTableModel::isValid(const QKeySequence& seq) const
		{
			return !registry_->hasKey(seq.toString());
		}

		bool ShortcutTableModel::setData(const QModelIndex& index, const QVariant& data,
		                                 int role)
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

		////////////////////////////  DELEGATE  ///////////////////////////////////

		QWidget* ShortcutTableDelegate::createEditor(QWidget* parent,
		                                             const QStyleOptionViewItem&,
		                                             const QModelIndex& index) const
		{
			if (index.column() == 0) 
			{
				return NULL;
			}

			ShortcutInserter* editor = new ShortcutInserter(parent);

			return editor;
		}

		void ShortcutTableDelegate::setEditorData(QWidget* editor, const QModelIndex& index) const
		{
			if (index.column() == 1) 
			{
				((EditSingleShortcut*)editor)->setShortcutText(qvariant_cast<QString>(index.model()->data(index, Qt::DisplayRole)));
			}
		}

		// Returns updated data to the model
		void ShortcutTableDelegate::setModelData(QWidget* editor,
		                                         QAbstractItemModel* model,
		                                         const QModelIndex& index) const
		{
			if (((ShortcutInserter*)editor)->wasEdited())
			{
				if (((ShortcutInserter*)editor)->wasCustomMode())
					model->setData(index, QVariant::fromValue(((ShortcutInserter*)editor)->getKeySequence()));
				else
					model->setData(index, QVariant::fromValue(QKeySequence()));
			}
		}
/*
		QSize ShortcutTableDelegate::sizeHint ( const QStyleOptionViewItem& option, const QModelIndex& index ) const
		{
			std::cout << " TODO implement ShortcutTableDelegate::sizeHint! " << std::endl;
			return QSize();
		}
		
		void ShortcutTableDelegate::paint (QPainter* painter, const QStyleOptionViewItem& option, 
																					 const QModelIndex& index ) const
		{	
			std::cout << " TODO implement ShortcutTableDelegate::paint()! " << std::endl;
		}

*/
		void ShortcutTableDelegate::updateEditorGeometry(QWidget* editor,
		                                                 const QStyleOptionViewItem& style,
		                                                 const QModelIndex& index) const
		{
			QRect new_rect(style.rect.topLeft(),
					           style.rect.topLeft() + QPoint(editor->minimumWidth(), editor->minimumHeight()));

			editor->setGeometry(new_rect);

			((QTableView*)(editor->parent()))->setRowHeight(index.row(), editor->minimumHeight());
		}

		/////////////////////////  ShortcutTableView  ///////////////////////////////

		ShortcutTableView::ShortcutTableView(QWidget* parent)
			: QTableView(parent)
		{
			// Create a new model
			ShortcutTableModel* model = new ShortcutTableModel(ShortcutRegistry::getInstance(0));

			// Setting up a view to display the items in the model
			setModel(model);
			
			// Setting up the Delegate
			setItemDelegate(new ShortcutTableDelegate());
		}

	}
}
