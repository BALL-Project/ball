// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#ifndef BALL_VIEW_WIDGETS_SEQUENCECONTROL_H
#define BALL_VIEW_WIDGETS_SEQUENCECONTROL_H

#ifndef BALL_VIEW_WIDGETS_DOCKWIDGET_H
# include <BALL/VIEW/WIDGETS/dockWidget.h>
#endif

#ifndef BALL_CONCEPT_MOLECULARINFORMATION_H
# include <BALL/CONCEPT/molecularInformation.h>
#endif

#ifndef BALL_VIEW_KERNEL_PREFERENCESENTRY
# include <BALL/VIEW/KERNEL/preferencesEntry.h>
#endif

#ifndef BALL_VIEW_KERNEL_COMMON_H
# include <BALL/VIEW/KERNEL/common.h>
#endif

#ifndef BALL_SEQUENCE_SEQUENCE_H
# include <BALL/SEQUENCE/sequence.h>
#endif

#ifndef BALL_SEQUENCE_ALIGNMENT_H
#include<BALL/SEQUENCE/alignment.h>
#endif

#ifndef BALL_DATATYPE_STRINGHASHMAP_H
# include <BALL/DATATYPE/stringHashMap.h>
#endif

#include <QtCore/QAbstractTableModel>
#include <QtCore/QPoint>
#include <QtGui/QMenu>
#include <QtGui/QTableView>
#include <QtGui/QComboBox>


#include <boost/shared_ptr.hpp>

#include <BALL/VIEW/UIC/ui_sequenceControl.h>


namespace BALL
{
	class Protein;
	class NucleicAcid;
	
	namespace VIEW
	{

		class BALL_VIEW_EXPORT SequenceControlModel
			: public QAbstractTableModel
		{
			Q_OBJECT

		friend class SequenceControl;

			public:
				typedef std::vector<boost::shared_ptr<Sequence> > SequenceVector;

				SequenceControlModel();

				void addSequence(boost::shared_ptr<Sequence> const& sequence)
				{
					beginInsertRows(QModelIndex(), sequences_.size(), sequences_.size());
					sequences_.push_back(sequence);
					selection_.push_back(false);
					endInsertRows();
				};

				int rowCount(const QModelIndex& parent = QModelIndex()) const;
				int columnCount(const QModelIndex& parent = QModelIndex()) const;

				bool hasSequenceFor(AtomContainer const* ac);
				boost::shared_ptr<Sequence> getSequenceFor(AtomContainer const* ac);

				QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const;
				QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
				Qt::ItemFlags flags(const QModelIndex& index) const;

				bool setData(const QModelIndex& index, const QVariant& data, int role = Qt::EditRole);

			

			protected:
				SequenceVector sequences_;
				std::vector<bool> selection_;
		};


class BALL_VIEW_EXPORT AlignmentControlModel
			: public QAbstractTableModel
		{
			Q_OBJECT

		friend class SequenceControl;

			public:
				
				AlignmentControlModel();

				void addSequence(boost::shared_ptr<Sequence> const& sequence)
				{
					beginInsertRows(QModelIndex(), alignment_.rows(), alignment_.rows());
					alignment_.addSequence(*sequence);
					selection_.push_back(false);
					endInsertRows();
				};

				int rowCount(const QModelIndex& parent = QModelIndex()) const;
				int columnCount(const QModelIndex& parent = QModelIndex()) const;

				bool hasSequenceFor(AtomContainer const* ac);
				boost::shared_ptr<Sequence> getSequenceFor(AtomContainer const* ac);

				QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const;
				QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
				Qt::ItemFlags flags(const QModelIndex& index) const;

				bool setData(const QModelIndex& index, const QVariant& data, int role = Qt::EditRole);

			

			protected:
				Alignment alignment_;
				std::vector<bool> selection_;
		};


//TODO
/**	SequenceControl is a widget to display the sequence of Composite objects. 
		This class is derived from the class DockWidget and extends it for showing and modifiying
		sequences. The methods checkMenu() and buildContextMenu() are overridden 
		for performing special sequential tasks.
	  \ingroup ViewWidgets
*/
class BALL_VIEW_EXPORT SequenceControl
	: public DockWidget,
		public PreferencesEntry
{

	Q_OBJECT

	public:
  BALL_EMBEDDABLE(SequenceControl, DockWidget)

	/**	@name	Constructors and Destructor
	*/
	//@{

	/** Default Constructor.
			\param      parent the parent widget 
			\param      name the name of this widget
	*/
	SequenceControl(QWidget* parent = 0, const char* name = "SequenceControl");

	/** Destructor.
	*/
	virtual ~SequenceControl();

	/// Resize Event
	virtual void resizeEvent(QResizeEvent* event);

	//@} 
	/**	@name	Accessors: inspectors and mutators 
	*/
	//@{

	/**	Menu checking method.
			This method is called by MainControl::checkMenus before a popup menu is shown.
			The copy menu entry is only enabled for selected System objects.
			\param main_control the MainControl object whose menus should be checked
			\see   Control::checkMenu
	*/
//	virtual void checkMenu(VIEW::MainControl& main_control);

	/** Insert a Composite object into this Control.
			If <tt>name == ""</tt> than the Information visitor is used for determining
			a name for the Composite.\par
			Calls generateListViewItem_() for the Composite.
			\param   composite a pointer to the Composite to be inserted into the Control
	*/
  //void addComposite(Composite& composite, String given_name = "");

	/** Recursive removal of a Composite from the Control.
	*/
	//Size removeComposite(Composite& composite);

	/** Non-mutable inspection of the selection.
	*/
	//const std::list<Composite*>& getSelection() const;

	/** Message handling.
			Overridden method from ConnectionObject to handle incoming messages.\par
			Calls reactToMessages_().\par
			\param  message a pointer to a Message object
	*/
	virtual void onNotify(Message *message);

	/** Build a context menu for a Composite.
			If the Composite has certain properties a context menu is created.
			This method only creates the necessary interface for derived classes.
			Override this method for creating other context menu entries, but call 
			Control::buildContextMenu at the end of your method to create the standard entries.
			Calls insertContextMenuEntry() for each context menu entry that will be created.
			\param   composite the Composite object for that a context menu should be created
			\see     onContextMenu
	*/
//	virtual void updateContextMenu(Composite& composite);

	/**	Initialize the menu entries:
				- cut 
				- copy 
				- paste 
				- delete
				- clear clipboard
			\par
			This method is called automatically	immediately before the main application 
			is started by MainControl::show.
			\param main_control the MainControl object to be initialized with this ModularWidget
	*/
	virtual void initializeWidget(MainControl& main_control);


	///
	void writePreferences(INIFile& inifile);

	///
	void fetchPreferences(INIFile& inifile);

	///
	void addSequenceTab(String const& name);

	///
	void addAlignmentTab(String const& name);
	
	public slots:

	//@}
	/** @name Public slots
	*/
	//@{
	void alignSelected();


	//@} 
	/** @name Protected members 
	*/
	//@{
	protected slots:

	void showGuestContextMenu(const QPoint& pos);

		void onTabCloseRequested_(int index);

	protected:
		void handleProtein_(Protein* protein);
		void handleNucleicAcid_(NucleicAcid* na);

		Ui_SequenceControlData ui_;

		StringHashMap<boost::shared_ptr<AlignmentControlModel> > alignment_per_tab_;
		StringHashMap<boost::shared_ptr<SequenceControlModel> >  sequences_per_tab_;
		StringHashMap<QTableView*>                               tabs_per_name_;

		QTabWidget* tab_widget_;
	///
		void buildContextMenu_();

	/** Set the selection of the checkboxes and the opening of the tree 
			according to the selection in the MainControl.
			\param open true means, that the item tree is opend and closed according to the changes
			\param force true means, that the item tree is opend and closed
						 also if more than 50 items are selected.
	*/
	//	void setSelection_(bool open, bool force = false);


	// only for Python Interface
	//	SequenceControl(const SequenceControl& mc);

	//	void enableUpdates_(bool state);
	//	void newSelection_(std::list<Composite*>& sel, bool selected);

	//@} 
	/** @name Menu entries ids
	*/
	//@{
	// the context menus
	QMenu               context_menu_;
								//			model_menu_;
//											edit_menu_,
		//									color_menu_[MODEL_LABEL - MODEL_LINES];
QAction* align_selected_action_;

};
}} // namespaces

#endif // BALL_VIEW_WIDGETS_SEQUENCECONTROL_H
