#ifndef BALL_VIEW_WIDGETS_PROPERTYEDITOR_H
#define BALL_VIEW_WIDGETS_PROPERTYEDITOR_H

#ifndef BALL_COMMON_GLOBAL_H
# include <BALL/COMMON/global.h>
#endif

#ifndef BALL_DATATYPE_HASHMAP_H
# include <BALL/DATATYPE/hashMap.h>
#endif

#include <BALL/VIEW/UIC/ui_propertyEditor.h>
#include <BALL/VIEW/UIC/ui_propEditorWidget.h>
#include <BALL/VIEW/UIC/ui_editorPDBInfoDialog.h>

#ifndef BALL_FORMAT_PDBINFO_H
# include <BALL/FORMAT/PDBInfo.h>
#endif

#include <QtGui/QWidget>

class QLabel;
class QSpinBox;
class QLineEdit;
class QCheckBox;

namespace BALL
{
	class NamedProperty;
	class PropertyManager;

	namespace VIEW
	{
		/**
		 * Abstract baseclass for widgets providing editor capability for a single NamedProperty
		 * If you wish to provide you own property editor you need to inherit from this class.
		 */
		class PropEditorWidget : public QWidget
		{
			Q_OBJECT

			public:
				/**
				 * Construct a PropEditorWidget for the property name with parent 'parent'
				 */
				PropEditorWidget(const std::string& name, QWidget* parent);

				/**
				 * Return the name of the property
				 */
				virtual const QString& getName() const;

				/**
				 * Toggles the editablitiy of the widget
				 */
				void setEditable(bool mode);

				/**
				 * Returns wheter the widget is editable
				 */
				bool isEditable() const;

				/**
				 * Apply the changes in the widget to man
				 *
				 * @param man the Property manager the changes are applied to
				 * @param create should a new named property be created if the property this
				 *               widget represents could not be found?
				 */
				void apply(PropertyManager* man, bool create = false);

				/**
				 * Reread the properties values from man
				 */
				void reset(PropertyManager* man);

				/**
				 * Create a copy of the widget with a different name, but with the same property type.
				 * This method must be reimplemented by subclasses
				 */
				virtual PropEditorWidget* clone(const std::string& name, QWidget* parent) = 0;

			signals:
				/**
				 * This signal is triggered when the user requested to delete this property
				 */
				void deleteProperty();

				/**
				 * This signal is triggered when the user requested to duplicate this property
				 */
				void duplicateProperty();

				/**
				 * This signal is triggered when the user changed the value of this widget
				 */
				void valueChanged();

			protected:
				void setupUi_();

				/**
				 * Utility function which adds the widget w to the internal layout
				 */
				bool addWidget_(int i, QWidget* w);

				/**
				 * This function must be reimplemented by subclasses.
				 * It writes the value of the widget to man
				 */
				virtual void apply_(PropertyManager* man) = 0;

				/**
				 * This function must be reimplemented by subclasses.
				 * It rereads the widgets values from prop
				 */
				virtual void reset_(const NamedProperty& prop) = 0;

				QString name_;
				Ui::PropEditorWidget ui_;
		};

		class BoolEditorWidget : public PropEditorWidget
		{
			Q_OBJECT

			public:
				BoolEditorWidget(const NamedProperty& prop, QWidget* parent);
				virtual BoolEditorWidget* clone(const std::string& name, QWidget* parent);

			protected:
				virtual void apply_(PropertyManager* man);
				virtual void reset_(const NamedProperty& prop);

			private:
				QCheckBox* box_;
		};

		class IntEditorWidget : public PropEditorWidget
		{
			Q_OBJECT

			public:
				IntEditorWidget(const NamedProperty& prop, QWidget* parent);
				virtual IntEditorWidget* clone(const std::string& name, QWidget* parent);

			protected:
				virtual void apply_(PropertyManager* man);
				virtual void reset_(const NamedProperty& prop);


			private:
				QSpinBox* box_;
		};

		class UIntEditorWidget : public PropEditorWidget
		{
			Q_OBJECT

			public:
				UIntEditorWidget(const NamedProperty& prop, QWidget* parent);
				virtual UIntEditorWidget* clone(const std::string& name, QWidget* parent);

			protected:
				virtual void apply_(PropertyManager* man);
				virtual void reset_(const NamedProperty& prop);

			private:
				QSpinBox* box_;
		};

		class FloatEditorWidget : public PropEditorWidget
		{
			Q_OBJECT

			public:
				FloatEditorWidget(const NamedProperty& prop, QWidget* parent);
				virtual FloatEditorWidget* clone(const std::string& name, QWidget* parent);

			protected:
				virtual void apply_(PropertyManager* man);
				virtual void reset_(const NamedProperty& prop);

			private:
				QLineEdit* edit_;
		};

		class DoubleEditorWidget : public PropEditorWidget
		{
			Q_OBJECT

			public:
				DoubleEditorWidget(const NamedProperty& prop, QWidget* parent);
				virtual DoubleEditorWidget* clone(const std::string& name, QWidget* parent);

			protected:
				virtual void apply_(PropertyManager* man);
				virtual void reset_(const NamedProperty& prop);

			private:
				QLineEdit* edit_;
		};

		class StringEditorWidget : public PropEditorWidget
		{
			Q_OBJECT

			public:
				StringEditorWidget(const NamedProperty& prop, QWidget* parent);
				virtual StringEditorWidget* clone(const std::string& name, QWidget* parent);

			protected:
				virtual void apply_(PropertyManager* man);
				virtual void reset_(const NamedProperty& prop);

			private:
				QLineEdit* edit_;
		};

		/**
		 * A simple Widget for arbitrary properties that just allows deleting them.
		 */
		class PropDeleteWidget : public PropEditorWidget
		{
			public:
				PropDeleteWidget(const NamedProperty& prop, QWidget* parent)
					: PropEditorWidget(prop.getName(), parent)
				{
					addWidget_(1,new QLabel(tr("(not editable)"),this));
					ui_.duplicate_button->setEnabled(false);
				}
				virtual PropEditorWidget* clone(const std::string&, QWidget*)
				{
					return NULL;
				}
			protected:
				virtual void apply_(PropertyManager *) {}
				virtual void reset_(const NamedProperty &) {}
		};

		class PDBInfoEditorWidget : public PropEditorWidget
		{
			Q_OBJECT

			public:
				PDBInfoEditorWidget(const PDBInfo& info, QWidget* parent);
				virtual PDBInfoEditorWidget* clone(const std::string& name, QWidget* parent);

			protected slots:
				void startEditorDialog();

			protected:
				virtual void apply_(PropertyManager* man);
				virtual void reset_(const NamedProperty& prop);

			private:
				// TODO: This should maybe become a generic TextEditor and not be as hackish.
				class EditorPDBInfoDialog : public QDialog {
				public:
					EditorPDBInfoDialog(QWidget* parent) : QDialog(parent)
					{
						ui_.setupUi(this);
						QFont mono("Monaco"); mono.setStyleHint(QFont::TypeWriter);
						ui_.textEditor->setFont(mono);
					}
				protected:
					friend class PDBInfoEditorWidget;
					Ui::editorPDBInfoDialog ui_;
				};

				PDBInfo localCopy_;
				EditorPDBInfoDialog* editorDialog_;
		};

		/**
		 * A widget providing editor capabilities for arbitrary PropertyManager objects.
		 * The widget provides a read-only mode as well as controls to add, delete and duplicate
		 * properties.
		 * Currently only NamedProperties are supported.
		 */
		class BALL_VIEW_EXPORT PropertyEditor : public QWidget
		{
			Q_OBJECT

			public:
				/**
				 * Construct a PropertyEditor
				 *
				 * @param parent the parent widget of this widget.
				 * @param editable should the properties be editable?
				 */
				explicit PropertyEditor(QWidget* parent = 0, bool editable = true);

				/**
				 * Construct a PropertyEditor
				 *
				 * @param manger a pointer to the PropertyManager object which should be edited
				 * @param parent the parent widget of this widget.
				 * @param editable should the properties be editable?
				 */
				PropertyEditor(PropertyManager* manager, QWidget* parent, bool editable = true);

				/**
				 * Set a different PropertyManager object to display/edit
				 * This function can also be used to explicitly update the view on
				 * the current PropertyManager.
				 */
				void setPropertyManager(PropertyManager* man);

				/**
				 * Control the editablity of the widget.
				 */
				void setEditable(bool mode);

				/**
				 * Are the displayed Properties user-editable
				 */
				bool isEditable() const;

				/**
				 * Are there changes to the properties available
				 */
				bool hasChanges() const;

			public slots:
				/**
				 * Apply all available changes
				 */
				void apply();

				/**
				 * Reread all property values from the PropertyManager
				 */
				void reset();

			signals:
				/**
				 * Emitted upon the first user made change to the editors
				 */
				void valueChanged();

			protected:
				void setupUi_();

				PropEditorWidget* addProperty_(const NamedProperty& prop);
				PropEditorWidget* addProperty_(PropEditorWidget* editor);
				PropEditorWidget* createEditorWidget_(const NamedProperty& prop);
				QString chooseName_(const QString& initial);
				void deleteProperty_(PropEditorWidget* editor);

			protected slots:
				void scheduleDelete_();
				void scheduleDuplicate_();
				void valueChanged_();

				void createNewBool_();
				void createNewInt_();
				void createNewUInt_();
				void createNewFloat_();
				void createNewDouble_();
				void createNewString_();

			private:
				bool has_changes_;
				bool is_editable_;
				PropertyManager* manager_;
				QList<PropEditorWidget*> new_properties_;
				QList<PropEditorWidget*> deleted_properties_;

				Ui::PropertyEditor ui_;
		};

	}
}

#endif //BALL_VIEW_WIDGETS_PROPERTYEDITOR_H
