#include <BALL/VIEW/WIDGETS/propertyEditor.h>

#ifndef BALL_CONCEPT_PROPERTY_H
# include <BALL/CONCEPT/property.h>
#endif

#ifndef BALL_VIEW_KERNEL_ICONLOADER_H
# include <BALL/VIEW/KERNEL/iconLoader.h>
#endif

#include <QtWidgets/QMenu>
#include <QtWidgets/QAction>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMessageBox>
#include <QtWidgets/QInputDialog>
#include <QtGui/QDoubleValidator>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QScrollBar>

#include <limits>

namespace BALL
{
	namespace VIEW
	{
		PropertyEditor::PropertyEditor(QWidget* parent, bool editable)
			: QWidget(parent),
			  has_changes_(false),
			  is_editable_(editable),
			  manager_(0)
		{
			setupUi_();
		}

		PropertyEditor::PropertyEditor(PropertyManager* man, QWidget* parent, bool editable)
			: QWidget(parent),
			  has_changes_(false),
			  is_editable_(editable)
		{
			setupUi_();
			setPropertyManager(man);
		}

		void PropertyEditor::setEditable(bool mode)
		{
			if(is_editable_ != mode)
			{
				is_editable_ = mode;

				QObject* child;
				foreach(child, ui_.editors->children())
				{
					PropEditorWidget* editor = qobject_cast<PropEditorWidget*>(child);
					if(editor)
					{
						editor->setEditable(is_editable_);
					}
				}
			}
		}

		bool PropertyEditor::isEditable() const
		{
			return is_editable_;
		}

		void PropertyEditor::setupUi_()
		{
			ui_.setupUi(this);

			//Insert a layout for the editors
			QBoxLayout* layout = new QBoxLayout(QBoxLayout::TopToBottom, ui_.editors);
			ui_.editors->setLayout(layout);
			layout->addStretch();

			//Setup the add_button
			ui_.add_button->setVisible(is_editable_);
			ui_.add_button->setIcon(IconLoader::instance().getIcon("actions/list-add"));

			//Create the menu which is used for the addition of new properties
			//Ideally this should be replaced by some kind of registry
			QMenu* menu = new QMenu(this);
			QAction* action = menu->addAction("Bool");
			connect(action, SIGNAL(triggered()), this, SLOT(createNewBool_()));
			action = menu->addAction("Integer");
			connect(action, SIGNAL(triggered()), this, SLOT(createNewInt_()));
			action = menu->addAction("Unsigned Integer");
			connect(action, SIGNAL(triggered()), this, SLOT(createNewUInt_()));
			action = menu->addAction("Float");
			connect(action, SIGNAL(triggered()), this, SLOT(createNewFloat_()));
			action = menu->addAction("Double");
			connect(action, SIGNAL(triggered()), this, SLOT(createNewDouble_()));
			action = menu->addAction("String");
			connect(action, SIGNAL(triggered()), this, SLOT(createNewString_()));

			ui_.add_button->setMenu(menu);
		}

		void PropertyEditor::setPropertyManager(PropertyManager* man)
		{
			manager_ = man;
			size_t num_properties = man->countNamedProperties();

			new_properties_.clear();
			deleted_properties_.clear();

			setUpdatesEnabled(false);

			//First we need to remove all
			QObject* child;
			foreach(child, ui_.editors->children())
			{
				PropEditorWidget* editor = qobject_cast<PropEditorWidget*>(child);
				if(editor)
				{
					child->deleteLater();
				}
			}

			//And then add the properties of the new manager
			for(size_t i = 0; i < num_properties; ++i)
			{
				addProperty_(man->getNamedProperty(i));
			}

			setUpdatesEnabled(true);
		}

		void PropertyEditor::apply()
		{
			if(!has_changes_)
			{
				return;
			}

			setUpdatesEnabled(false);

			//Delete all properties which have been scheduled for deletion
			PropEditorWidget* prop;
			foreach(prop, deleted_properties_)
			{
				deleteProperty_(prop);
			}

			//Apply all other properties
			QObject* child;
			foreach(child, ui_.editors->children())
			{
				PropEditorWidget* editor = qobject_cast<PropEditorWidget*>(child);
				if(editor)
				{
					editor->apply(manager_, true);
				}
			}

			//Clear the lists of changes
			new_properties_.clear();
			deleted_properties_.clear();

			has_changes_ = false;

			setUpdatesEnabled(true);
		}

		void PropertyEditor::reset()
		{
			if(!has_changes_)
			{
				return;
			}

			setUpdatesEnabled(false);

			//The user discarded the newly added properties
			PropEditorWidget* prop;
			foreach(prop, new_properties_)
			{
				deleteProperty_(prop);
			}

			//And wants us to reread the old property values
			QObject* child;
			foreach(child, ui_.editors->children())
			{
				PropEditorWidget* editor = qobject_cast<PropEditorWidget*>(child);
				if(editor)
				{
					editor->show();
					//TODO: handle the case a property does no longer exist
					editor->reset(manager_);
				}
			}

			//Clear the lists of changes
			new_properties_.clear();
			deleted_properties_.clear();

			has_changes_ = false;

			setUpdatesEnabled(true);
		}

		/*
		 * Helpers for the creation of new editors/properties
		 */
		void PropertyEditor::createNewBool_()
		{
			QString name = chooseName_("New Bool Property");
			if(!name.isNull()) {
				NamedProperty p(name.toStdString(), false);
				new_properties_.append(addProperty_(new BoolEditorWidget(p, ui_.editors)));

				valueChanged_();
			}
		}

		void PropertyEditor::createNewInt_()
		{
			QString name = chooseName_("New Integer Property");
			if(!name.isNull()) {
				NamedProperty p(name.toStdString(), (int)0);
				new_properties_.append(addProperty_(new IntEditorWidget(p, ui_.editors)));

				valueChanged_();
			}
		}

		void PropertyEditor::createNewUInt_()
		{
			QString name = chooseName_("New Unsigned Integer Property");
			if(!name.isNull()) {
				NamedProperty p(name.toStdString(), (unsigned int)0);
				new_properties_.append(addProperty_(new UIntEditorWidget(p, ui_.editors)));

				valueChanged_();
			}
		}

		void PropertyEditor::createNewFloat_()
		{
			QString name = chooseName_("New Float Property");
			if(!name.isNull()) {
				NamedProperty p(name.toStdString(), 0.0f);
				new_properties_.append(addProperty_(new FloatEditorWidget(p, ui_.editors)));

				valueChanged_();
			}
		}

		void PropertyEditor::createNewDouble_()
		{
			QString name = chooseName_("New Double Property");
			if(!name.isNull()) {
				NamedProperty p(name.toStdString(), 0.0);
				new_properties_.append(addProperty_(new DoubleEditorWidget(p, ui_.editors)));

				valueChanged_();
			}
		}

		void PropertyEditor::createNewString_()
		{
			QString name = chooseName_("New String Property");
			if(!name.isNull()) {
				NamedProperty p(name.toStdString(), "");
				new_properties_.append(addProperty_(new StringEditorWidget(p, ui_.editors)));

				valueChanged_();
			}
		}

		PropEditorWidget* PropertyEditor::addProperty_(const NamedProperty& prop)
		{
			return addProperty_(createEditorWidget_(prop));
		}

		PropEditorWidget* PropertyEditor::addProperty_(PropEditorWidget* w)
		{
			if(w)
			{
				connect(w, SIGNAL(deleteProperty()), this, SLOT(scheduleDelete_()));
				connect(w, SIGNAL(duplicateProperty()), this, SLOT(scheduleDuplicate_()));
				connect(w, SIGNAL(valueChanged()), this, SLOT(valueChanged_()));

				w->setEditable(is_editable_);
				static_cast<QBoxLayout*>(ui_.editors->layout())->insertWidget(ui_.editors->layout()->count() - 1, w);
			}

			return w;
		}

		PropEditorWidget* PropertyEditor::createEditorWidget_(const NamedProperty& prop)
		{
			//Ideally this should be replaced by some kind of registry
			switch(prop.getType())
			{
				case NamedProperty::BOOL:
					return new BoolEditorWidget  (prop, ui_.editors);
				case NamedProperty::INT:
					return new IntEditorWidget   (prop, ui_.editors);
				case NamedProperty::UNSIGNED_INT:
					return new UIntEditorWidget  (prop, ui_.editors);
				case NamedProperty::FLOAT:
					return new FloatEditorWidget (prop, ui_.editors);
				case NamedProperty::DOUBLE:
					return new FloatEditorWidget (prop, ui_.editors);
				case NamedProperty::STRING:
					return new StringEditorWidget(prop, ui_.editors);
				case NamedProperty::SMART_OBJECT:
					if (dynamic_cast<PDBInfo*>(prop.getSmartObject().get()) != NULL)
					{
						return new PDBInfoEditorWidget(* (PDBInfo*)prop.getSmartObject().get(), ui_.editors);
					}
					else
					{
						return new PropDeleteWidget(prop, ui_.editors);
					}
				default:
					return NULL;
			}
		}

		void PropertyEditor::deleteProperty_(PropEditorWidget* editor)
		{
			//Thoroughly delete a property
			manager_->clearProperty(String(editor->getName()));
			layout()->removeWidget(editor);
			editor->setParent(0);
			editor->deleteLater();
		}

		void PropertyEditor::scheduleDelete_()
		{
			PropEditorWidget* editor = qobject_cast<PropEditorWidget*>(sender());

			if(!editor)
			{
				return;
			}

			//If the user wants to delete a newly created property this can happen right away
			//otherwise we schedule the property for release
			int index = new_properties_.indexOf(editor);
			if(index == - 1)
			{
				deleted_properties_.append(editor);
				editor->hide();
			} else {
				new_properties_.removeAt(index);
				deleteProperty_(editor);
			}

			valueChanged_();
		}

		void PropertyEditor::scheduleDuplicate_()
		{
			PropEditorWidget* editor = qobject_cast<PropEditorWidget*>(sender());

			if(!editor)
			{
				return;
			}

			//Get a name and create a new property editor
			QString name = chooseName_(editor->getName());
			if(!name.isNull()) {
				new_properties_.append(addProperty_(editor->clone(name.toStdString(), ui_.editors)));

				valueChanged_();
			}
		}

		QString PropertyEditor::chooseName_(const QString& initial)
		{
			bool ok = true;
			do {
				QString new_name = QInputDialog::getText(this, tr("Insert name"), tr("Name"),
				                                         QLineEdit::Normal, initial, &ok);

				if(!ok) {
					return QString::null;
				}

				//We need to check the inserted name against all properties and properties scheduled for creation
				bool not_used = true;
				PropEditorWidget* e;
				foreach(e, new_properties_)
				{
					if(e->getName() == new_name)
					{
						not_used = false;
						break;
					}
				}

				if(!manager_->hasProperty(String(new_name)) && not_used) {
					return new_name;
				} else {
					QMessageBox::warning(this, tr("Property already exists"),
					                           tr("A property with given name already exists, please enter a new name."));
				}
			} while(true);
		}

		void PropertyEditor::valueChanged_()
		{
			//We only want to emit this signal once
			if(!has_changes_)
			{
				emit valueChanged();

				has_changes_ = true;
			}
		}

		bool PropertyEditor::hasChanges() const
		{
			return has_changes_;
		}

		/*
		 * Editors
		 */
		PropEditorWidget::PropEditorWidget(const std::string& name, QWidget* parent)
			: QWidget(parent),
			  name_(name.c_str())
		{
			setupUi_();
			ui_.label_->setText(getName());
		}

		const QString& PropEditorWidget::getName() const
		{
			return name_;
		}

		void PropEditorWidget::setEditable(bool mode)
		{
			ui_.delete_button->setVisible(mode);
			ui_.duplicate_button->setVisible(mode);

			setEnabled(mode);
		}

		bool PropEditorWidget::isEditable() const
		{
			return ui_.delete_button->isVisible();
		}

		void PropEditorWidget::apply(PropertyManager* man, bool create)
		{
			if(!man) {
				return;
			}

			if(create || man->hasProperty(String(getName())))
			{
				apply_(man);
			}
		}

		bool PropEditorWidget::addWidget_(int i, QWidget* w)
		{
			QBoxLayout* l = qobject_cast<QBoxLayout*>(layout());
			if(!l) {
				return false;
			}

			l->insertWidget(i, w);
			return true;
		}

		void PropEditorWidget::setupUi_()
		{
			ui_.setupUi(this);
			IconLoader& loader= IconLoader::instance();
			ui_.delete_button->setIcon(loader.getIcon("actions/edit-delete"));
			ui_.duplicate_button->setIcon(loader.getIcon("actions/edit-copy"));
			connect(ui_.delete_button, SIGNAL(clicked()), this, SIGNAL(deleteProperty()));
			connect(ui_.duplicate_button, SIGNAL(clicked()), this, SIGNAL(duplicateProperty()));
		}

		void PropEditorWidget::reset(PropertyManager* man)
		{
			if(!man) {
				return;
			}

			String name(getName());
			if(man->hasProperty(name))
			{
				reset_(man->getProperty(name));
			}
		}

		//BOOL
		BoolEditorWidget::BoolEditorWidget(const NamedProperty& prop, QWidget* parent)
			: PropEditorWidget(prop.getName(), parent)
		{
			box_ = new QCheckBox("", this);
			box_->setChecked(prop.getBool());

			connect(box_, SIGNAL(toggled(bool)), this, SIGNAL(valueChanged()));
			addWidget_(1, box_);
		}

		void BoolEditorWidget::apply_(PropertyManager* man)
		{
			man->setProperty(String(getName()), box_->isChecked());
		}

		void BoolEditorWidget::reset_(const NamedProperty& prop)
		{
			box_->setChecked(prop.getBool());
		}

		BoolEditorWidget* BoolEditorWidget::clone(const std::string& name, QWidget* parent)
		{
			NamedProperty prop(name, box_->isChecked());
			return new BoolEditorWidget(prop, parent);
		}

		//INT
		IntEditorWidget::IntEditorWidget(const NamedProperty& prop, QWidget* parent)
			: PropEditorWidget(prop.getName(), parent)
		{
			box_   = new QSpinBox(this);

			box_->setValue(prop.getInt());
			box_->setMinimum(std::numeric_limits<int>::min());
			box_->setMaximum(std::numeric_limits<int>::max());

			connect(box_, SIGNAL(valueChanged(int)), this, SIGNAL(valueChanged()));

			addWidget_(1, box_);
		}

		void IntEditorWidget::apply_(PropertyManager* man)
		{
			man->setProperty(String(getName()), (int)box_->value());
		}

		void IntEditorWidget::reset_(const NamedProperty& prop)
		{
			box_->setValue(prop.getInt());
		}

		IntEditorWidget* IntEditorWidget::clone(const std::string& name, QWidget* parent)
		{
			NamedProperty prop(name, (int)box_->value());
			return new IntEditorWidget(prop, parent);
		}

		//UNSIGNED INT
		UIntEditorWidget::UIntEditorWidget(const NamedProperty& prop, QWidget* parent)
			: PropEditorWidget(prop.getName(), parent)
		{
			box_   = new QSpinBox(this);

			box_->setMinimum(0);
			box_->setMaximum(std::numeric_limits<int>::max());
			box_->setValue(prop.getUnsignedInt());

			connect(box_, SIGNAL(valueChanged(int)), this, SIGNAL(valueChanged()));

			addWidget_(1, box_);
		}

		UIntEditorWidget* UIntEditorWidget::clone(const std::string& name, QWidget* parent)
		{
			NamedProperty prop(name, (unsigned int)box_->value());
			return new UIntEditorWidget(prop, parent);
		}

		void UIntEditorWidget::apply_(PropertyManager* man)
		{
			man->setProperty(String(getName()), (unsigned int)box_->value());
		}

		void UIntEditorWidget::reset_(const NamedProperty& prop)
		{
			box_->setValue(prop.getUnsignedInt());
		}

		//FLOAT
		FloatEditorWidget::FloatEditorWidget(const NamedProperty& prop, QWidget* parent)
			: PropEditorWidget(prop.getName(), parent)
		{
			edit_  = new QLineEdit(this);

			edit_->setText(QString::number(prop.getFloat()));
			edit_->setValidator(new QDoubleValidator(std::numeric_limits<float>::min(), std::numeric_limits<float>::max(), 20, this));

			connect(edit_, SIGNAL(textEdited(const QString&)), this, SIGNAL(valueChanged()));

			addWidget_(1, edit_);
		}

		FloatEditorWidget* FloatEditorWidget::clone(const std::string& name, QWidget* parent)
		{
			NamedProperty prop(name, edit_->text().toFloat());
			return new FloatEditorWidget(prop, parent);
		}

		void FloatEditorWidget::apply_(PropertyManager* man)
		{
			man->setProperty(String(getName()), edit_->text().toFloat());
		}

		void FloatEditorWidget::reset_(const NamedProperty& prop)
		{
			edit_->setText(QString::number(prop.getFloat()));
		}

		//DOUBLE
		DoubleEditorWidget::DoubleEditorWidget(const NamedProperty& prop, QWidget* parent)
			: PropEditorWidget(prop.getName(), parent)
		{
			edit_  = new QLineEdit(this);

			edit_->setText(QString::number(prop.getDouble()));
			edit_->setValidator(new QDoubleValidator(this));

			connect(edit_, SIGNAL(textEdited(const QString&)), this, SIGNAL(valueChanged()));

			addWidget_(1, edit_);
		}

		DoubleEditorWidget* DoubleEditorWidget::clone(const std::string& name, QWidget* parent)
		{
			NamedProperty prop(name, edit_->text().toDouble());
			return new DoubleEditorWidget(prop, parent);
		}

		void DoubleEditorWidget::apply_(PropertyManager* man)
		{
			man->setProperty(String(getName()), edit_->text().toDouble());
		}

		void DoubleEditorWidget::reset_(const NamedProperty& prop)
		{
			edit_->setText(QString::number(prop.getDouble()));
		}

		//STRING
		StringEditorWidget::StringEditorWidget(const NamedProperty& prop, QWidget* parent)
			: PropEditorWidget(prop.getName(), parent)
		{
			edit_  = new QLineEdit(prop.getString().c_str(), this);

			connect(edit_, SIGNAL(textEdited(const QString&)), this, SIGNAL(valueChanged()));

			addWidget_(1, edit_);
		}

		StringEditorWidget* StringEditorWidget::clone(const std::string& name, QWidget* parent)
		{
			NamedProperty prop(name, edit_->text().toStdString());
			return new StringEditorWidget(prop, parent);
		}

		void StringEditorWidget::apply_(PropertyManager* man)
		{
			man->setProperty(String(getName()), edit_->text().toStdString());
		}

		void StringEditorWidget::reset_(const NamedProperty& prop)
		{
			edit_->setText(prop.getString().c_str());
		}

		// SMART_OBJECTs

		PDBInfoEditorWidget::PDBInfoEditorWidget(const PDBInfo &prop, QWidget *parent)
			: PropEditorWidget("PDBINFO", parent)
		{
			editorDialog_ = new EditorPDBInfoDialog(this);
			localCopy_ = prop;
			ui_.duplicate_button->setEnabled(false);
			QPushButton* launchEditor = new QPushButton(tr("Edit..."),this);
			connect(launchEditor,SIGNAL(clicked()),this,SLOT(startEditorDialog()));
			addWidget_(1,launchEditor);
		}

		PDBInfoEditorWidget* PDBInfoEditorWidget::clone(const std::string &, QWidget *parent)
		{
			return new PDBInfoEditorWidget(localCopy_, parent);
		}

		void PDBInfoEditorWidget::startEditorDialog()
		{
			PDBRecords& skipped = localCopy_.getSkippedRecords();
			// copy the text over to the editor component
			editorDialog_->ui_.textEditor->clear();
			for (PDBRecords::iterator it = skipped.begin();
				 it != skipped.end();
				 ++it)
			{
				editorDialog_->ui_.textEditor->appendPlainText((*it).c_str());
			}
			// then fire up the editor
			if (editorDialog_->exec() == QDialog::Accepted) {
				QStringList lines = editorDialog_->ui_.textEditor->toPlainText().split("\n");

				int n = 0;
				skipped.clear();
				foreach(QString line, lines) {
					skipped.push_back(String(line));
					n++;
				}
				// don't forget to emit this -- else changes won't be written back!
				emit valueChanged();
			}
		}

		void PDBInfoEditorWidget::apply_(PropertyManager *man)
		{
			boost::shared_ptr<PersistentObject> ptr(new PDBInfo(localCopy_));
			NamedProperty prop(String(getName()), ptr);
			man->setProperty(prop);
		}

		void PDBInfoEditorWidget::reset_(const NamedProperty &prop)
		{
			if (dynamic_cast<PDBInfo*>(prop.getSmartObject().get()) != NULL)
			{
				localCopy_ = * (PDBInfo*) (prop.getSmartObject().get());
			}
		}

	}
}
