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
			: QAbstractTableModel()
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

		bool SequenceControlModel::hasSequenceFor(AtomContainer const* ac)
		{
			for (Position i=0; i<sequences_.size(); ++i)
			{
				if (sequences_[i]->getOrigin() == ac)
					return true;
			}

			return false;
		}

		boost::shared_ptr<Sequence> SequenceControlModel::getSequenceFor(AtomContainer const* ac)
		{
			boost::shared_ptr<Sequence> result;
			for (Position i=0; i<sequences_.size(); ++i)
			{
				if (sequences_[i]->getOrigin() == ac)
					result = sequences_[i];
			}

			return result;
		}

		QVariant SequenceControlModel::data(const QModelIndex& index, int role) const
		{
			if(index.column() == 0) {
				switch(role) {
					case Qt::CheckStateRole: {
						return selection_[index.row()] ? Qt::Checked : Qt::Unchecked;
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
					case Qt::FontRole:
				  {
						QFont font("Monospace");
						font.setStyleHint(QFont::TypeWriter);
						font.setLetterSpacing(QFont::PercentageSpacing, 150);

						return font;
					}
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
						return QVariant::fromValue(QString("")); //Selected"));
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

		Qt::ItemFlags SequenceControlModel::flags(const QModelIndex& index) const
		{
			if (index.column() == 0)
				return Qt::ItemIsSelectable | Qt::ItemIsUserCheckable | Qt::ItemIsEnabled;
			else
				return Qt::ItemIsSelectable | Qt::ItemIsEnabled;
		}

		bool SequenceControlModel::setData(const QModelIndex& index, const QVariant& data, int role)
		{
			if (index.column() == 0 && role == Qt::CheckStateRole && index.row() < (int)selection_.size())
			{
				selection_[index.row()] = !selection_[index.row()];
				emit dataChanged(index, index);

				return true;
			}

			return false;
		}

		// --------------------- SequenceControl ----------------------------------------
		SequenceControl::SequenceControl(QWidget* parent, const char* name)
				: DockWidget(parent, name),
				  PreferencesEntry()
		{
		#ifdef BALL_VIEW_DEBUG
			Log.error() << "new SequenceControl " << this << std::endl;
		#endif

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
			registerWidget(this);

			addSequenceTab("all_sequences");

			// allow closing of tabs
			tab_widget_->setTabsClosable(true);
			connect(tab_widget_, SIGNAL(tabCloseRequested(int)), this, SLOT(onTabCloseRequested_(int)));

			buildContextMenu_();
		}

		SequenceControl::~SequenceControl()
		{
		#ifdef BALL_VIEW_DEBUG
			Log.error() << "Destroying SequenceControl " << this << std::endl;
		#endif
		}

		void SequenceControl::resizeEvent(QResizeEvent* event)
		{
			//QTableView* sequence_view = tab_widget_->findChild<QTableView*>("sequence_view");
			//sequence_view->setColumnWidth(2, event->size().width() - sequence_view->columnWidth(0) - sequence_view->columnWidth(1)-27);
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
			if (inifile.hasEntry("SEQUENCECONTROL", "LetterSpacing"))
			{
				vector<String> fields;
				Size size = inifile.getValue("SEQUENCECONTROL", "LetterSpacing").split(fields, "|");
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
			//inifile.insertValue("SEQUENCECONTROL", "LetterSpacing", show_ss_);

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

				switch (composite_message->getType())
				{
					case CompositeMessage::NEW_MOLECULE:
					{
						// TODO: make nicer
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
					case CompositeMessage::CHANGED_COMPOSITE_HIERARCHY:
					{
						for (StringHashMap<boost::shared_ptr<SequenceControlModel> >::Iterator model_it = sequences_per_tab_.begin(); model_it != sequences_per_tab_.end(); ++model_it)
						{
							boost::shared_ptr<SequenceControlModel> current_model = model_it->second;

							if (RTTI::isKindOf<System>(*composite))
							{
								System* system = RTTI::castTo<System>(*composite);
								for (ProteinIterator p_it = system->beginProtein(); +p_it; ++p_it)
								{
									if (current_model->hasSequenceFor(&*p_it))
									{
										current_model->getSequenceFor(&*p_it)->setName(p_it->getName());
									}
								}
								for (NucleicAcidIterator na_it = system->beginNucleicAcid(); +na_it; ++na_it)
								{
									if (current_model->hasSequenceFor(&*na_it))
									{
										current_model->getSequenceFor(&*na_it)->setName(na_it->getName());
									}
								}
							}
							else if (RTTI::isKindOf<Protein>(*composite))
							{
								AtomContainer* ac = RTTI::castTo<AtomContainer>(*composite);
								if (current_model->hasSequenceFor(ac))
								{
									current_model->getSequenceFor(ac)->setName(ac->getName());
								}
							}
							else if (RTTI::isKindOf<NucleicAcid>(*composite))
							{
								AtomContainer* ac = RTTI::castTo<AtomContainer>(*composite);
								if (current_model->hasSequenceFor(RTTI::castTo<AtomContainer>(*composite)))
								{
									current_model->getSequenceFor(ac)->setName(ac->getName());
								}
							}
						}
					}
					default:
						break;
				}
			}

		}

		void SequenceControl::onTabCloseRequested_(int index)
		{
			String tab_name = ascii(tab_widget_->tabText(index));
			if (tab_name != "all_sequences")
			{
				sequences_per_tab_.remove(tab_name);
				tabs_per_name_.remove(tab_name);

				tab_widget_->removeTab(index);
			}
		}

		void SequenceControl::handleProtein_(Protein* protein)
		{
/*			for (ChainIterator c_it = protein->beginChain(); +c_it; ++c_it)
			{
				sequences_per_tab_["all_sequences"].push_back(boost::shared_ptr<Sequence>(new Sequence(protein->getName() + "_" + c_it->getName(), &*c_it)));
			}
*/
			sequences_per_tab_["all_sequences"]->addSequence(boost::shared_ptr<Sequence>(new Sequence(protein->getName(), protein)));

			tabs_per_name_["all_sequences"]->resizeColumnToContents(2);

			/// TEST for Nikola: TODO add a tab for an alignment! 
			addSequenceTab(protein->getName());
			sequences_per_tab_[protein->getName()]->addSequence(boost::shared_ptr<Sequence>(new Sequence(protein->getName(), protein)));
			tabs_per_name_[protein->getName()]->resizeColumnToContents(2);
			/// END TEST Nikola
		}

				void SequenceControl::buildContextMenu_()
			{
			// ===============================================================
			// create sequences context menu:
			// ===============================================================
			context_menu_.setObjectName("ContextMenu");
			model_menu_.setObjectName("ModelMenu");
/*
			model_menu_.addAction(tr("Custom"), this, SLOT(align())); 

			const ModelInformation& mi = getMainControl()->getModelInformation();
			QAction* action =	0;
			Position p = 0;
			for (Position pos = MODEL_LINES; pos < MODEL_LABEL; pos++)
			{
				if (!mi.modelMuteableByDisplayProperties((ModelType)pos)) break;
				action = model_menu_.addMenu(&color_menu_[p]);
				color_menu_[p].setObjectName((String("ColorMenu") + String(p)).c_str());
				action->setText(mi.getModelName((ModelType)pos).c_str());
				action->setObjectName(String(pos).c_str());
				p++;
			}
*/
			
		}
//TODO NUR GRADE ZUM LINKEN
void SequenceControl::align()
{
}


		
		void SequenceControl::handleNucleicAcid_(NucleicAcid* na)
		{
			sequences_per_tab_["all_sequences"]->addSequence(boost::shared_ptr<Sequence>(new Sequence(na->getName(), na)));
		}

		void SequenceControl::addSequenceTab(String const& name)
		{
			// create a new tab 
			sequences_per_tab_[name] = boost::shared_ptr<SequenceControlModel>(new SequenceControlModel());

			QTableView* sequence_view = new QTableView();
			sequence_view->setModel(sequences_per_tab_[name].get());
			sequence_view->horizontalHeader()->setStretchLastSection(true);
			sequence_view->horizontalHeader()->setResizeMode(2, QHeaderView::ResizeToContents);
			sequence_view->horizontalHeader()->setDefaultAlignment(Qt::AlignLeft);

			sequence_view->resizeColumnToContents(0);

			//sequence_view->setColumnWidth(2, sequence_view->width() - sequence_view->columnWidth(0) - sequence_view->columnWidth(1));

			tabs_per_name_[name] = sequence_view;
			tab_widget_->addTab(sequence_view, name.c_str());
		}


				action->setText(mi.getModelName((ModelType)pos).c_str());
				action->setObjectName(String(pos).c_str());
				p++;
			}
*/
			
		}
//TODO NUR GRADE ZUM LINKEN
void SequenceControl::align()
{
}

	} // namespace VIEW
} // namespace BALL
