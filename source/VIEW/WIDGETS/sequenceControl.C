/* TRANSLATOR BALL::VIEW::SequenceControl

		Necessary for lupdate.
*/

// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
#include <BALL/VIEW/WIDGETS/sequenceControl.h>
#include <BALL/VIEW/KERNEL/mainControl.h>
#include <BALL/VIEW/KERNEL/message.h>
#include <BALL/VIEW/DIALOGS/compositeProperties.h>
#include <BALL/KERNEL/system.h>
#include <BALL/KERNEL/protein.h>
#include <BALL/KERNEL/nucleicAcid.h>

#include <QtGui/QPushButton>
#include <QtGui/QLineEdit>
#include <QtGui/QMessageBox>
#include <QtGui/QTextEdit>
#include <QtGui/QToolTip>
#include <QtGui/QGridLayout>
#include <QtGui/QCompleter>

#include <set>

namespace BALL
{
	namespace VIEW
	{
		//////////////////////////   MODEL   ////////////////////////////
		SequenceControlModel::SequenceControlModel()
		{
		}

		int SequenceControlModel::rowCount(const QModelIndex& /*parent*/) const
		{
				return sequences_.size();
		}

		int SequenceControlModel::columnCount(const QModelIndex& /*parent*/) const
		{
			// selector / name / sequence
			return 3;
		}

		QVariant SequenceControlModel::data(const QModelIndex& index, int role) const
		{
			if(index.column() == 0) {
				switch(role) {
					case Qt::DisplayRole: {
						return QVariant::fromValue(true);
					}
				}
			} else if (index.column() == 1) {
				switch(role) {
					case Qt::DisplayRole:
						return QVariant::fromValue(QString(sequences_[index.row()]->getName().c_str()));
				}
			} else {
				switch(role) {
					case Qt::DisplayRole:
						return QVariant::fromValue(QString(sequences_[index.row()]->getStringSequence().c_str()));
				}
			}

			return QVariant();
		}

		QVariant SequenceControlModel::headerData(int section, Qt::Orientation orientation, int role) const
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
						return QVariant::fromValue(QString("Selected"));
					}
					else if (section == 1)
					{
						return QVariant::fromValue(QString("Name"));
					}
					else if (section == 2)
					{
						return QVariant::fromValue(QString("Sequence"));
					}
				default:
					return QVariant();
			}
		}

		Qt::ItemFlags SequenceControlModel::flags(const QModelIndex& /*index*/) const
		{
			return Qt::ItemIsSelectable | Qt::ItemIsEnabled;
		}


		SequenceControl::SequenceControl(QWidget* parent, const char* name)
				: DockWidget(parent, name),
				  PreferencesEntry()
		{
		#ifdef BALL_VIEW_DEBUG
			Log.error() << "new SequenceControl " << this << std::endl;
		#endif

			//TODO
			tab_widget_ = new QTabWidget(this);
			ui_.setupUi(tab_widget_);
			setGuest(*tab_widget_);

			setINIFileSectionName("SEQUENCECONTROL");
			setObjectName(name);
			// for preferences	
			registerWidgets_();
			// for actions, icons...
			ModularWidget::registerWidget(this);

			// signals and slots connections


		/*	QGridLayout* glay = getGuestLayout();
			QGridLayout* lay = new QGridLayout();
			glay->addLayout(lay, 2, 0);

			listview->setObjectName("SequenceControlList");
			listview->headerItem()->setText(0, tr("Name") + " ["+ tr("highlight") + "]");
			listview->headerItem()->setText(1, tr("checked"));
			listview->headerItem()->setText(2, tr("Sequence"));
			listview->resizeColumnToContents(0);
			listview->resizeColumnToContents(2);
			lay->addWidget(listview,0, 0, 1, -1);

			resize(200,400);
*/
			registerWidget(this);

			sequences_per_tab_["all_sequences"] = boost::shared_ptr<SequenceControlModel>(new SequenceControlModel());
			tab_widget_->findChild<QColumnView*>("sequence_view")->setModel(sequences_per_tab_["all_sequences"].get());
			tab_indices_per_name_["all_sequences"] = 0;
			//buildContextMenu_();
		}

		SequenceControl::~SequenceControl()
		{
		#ifdef BALL_VIEW_DEBUG
			Log.error() << "Destroying SequenceControl " << this << std::endl;
		#endif
		}

		void SequenceControl::initializeWidget(MainControl& main_control)
		{
			// add specific menu entries
			// TODO

			DockWidget::initializeWidget(main_control);

			registerForHelpSystem(this, "sequenceControl.html");

		}

		void SequenceControl::fetchPreferences(INIFile& inifile)
		{
			//TODO
			DockWidget::fetchPreferences(inifile);
			//TODO
			if (inifile.hasEntry("SEQUENCECONTROL", "RegularExpressions"))
			{
				vector<String> fields;
				Size size = inifile.getValue("SEQUENCECONTROL", "RegularExpressions").split(fields, "|");
				for (Position p = 0; p < size; p++)
				{
				//	selector_edit_->addItem(fields[p].c_str());
				}
			}
		}

		void SequenceControl::writePreferences(INIFile& inifile)
		{
			//TODO
			inifile.appendSection("SEQUENCECONTROL");
			//inifile.insertValue("SEQUENCECONTROL", "ShowSS", show_ss_);

			DockWidget::writePreferences(inifile);
		}

		void SequenceControl::onNotify(Message *message)
		{
			#ifdef BALL_VIEW_DEBUG
				Log.error() << "SequenceControl " << this << " onNotify " << message << std::endl;
			#endif

			if (RTTI::isKindOf<CompositeMessage>(*message))
			{
				CompositeMessage* composite_message = RTTI::castTo<CompositeMessage>(*message);
				Composite* composite = composite_message->getComposite();
		Log.error() << "dooing " << std::endl;

				switch (composite_message->getType())
				{
					case CompositeMessage::NEW_MOLECULE:
					{
						if (RTTI::isKindOf<System>(*composite))
						{
							System* system = RTTI::castTo<System>(*composite);
							for (ProteinIterator p_it = system->beginProtein(); +p_it; ++p_it)
							{
								handleProtein_(&*p_it);
							}
							for (NucleicAcidIterator na_it = system->beginNucleicAcid(); +na_it; ++na_it)
							{
								handleNucleicAcid_(&*na_it);
							}
						}
						else if (RTTI::isKindOf<Protein>(*composite))
						{
							Protein* protein = RTTI::castTo<Protein>(*composite);
							handleProtein_(protein);
						}
						else if (RTTI::isKindOf<NucleicAcid>(*composite))
						{
							NucleicAcid* na = RTTI::castTo<NucleicAcid>(*composite);
							handleNucleicAcid_(na);
						}
					}
					default:
						break;
				}
			}
		}

		void SequenceControl::handleProtein_(Protein* protein)
		{
/*			for (ChainIterator c_it = protein->beginChain(); +c_it; ++c_it)
			{
				sequences_per_tab_["all_sequences"].push_back(boost::shared_ptr<Sequence>(new Sequence(protein->getName() + "_" + c_it->getName(), &*c_it)));
				updateTab_("all_sequences");
			}
*/
				sequences_per_tab_["all_sequences"]->addSequence(boost::shared_ptr<Sequence>(new Sequence(protein->getName(), protein)));
				updateTab_("all_sequences");
		}

		void SequenceControl::handleNucleicAcid_(NucleicAcid* na)
		{
			sequences_per_tab_["all_sequences"]->addSequence(boost::shared_ptr<Sequence>(new Sequence(na->getName(), na)));
			updateTab_("all_sequences");
		}

		// TODO: better use QTs model view delegate for this
		void SequenceControl::updateTab_(const String& tab_name)
		{
			//sequences_per_tab_[tab_name]->invalidate();
		}

	} // namespace VIEW
} // namespace BALL
