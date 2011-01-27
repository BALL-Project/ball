// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#ifndef BALL_VIEW_DATATYPE_DATASET_H
#define BALL_VIEW_DATATYPE_DATASET_H

#ifndef BALL_DATATYPE_STRING_H
# include <BALL/DATATYPE/string.h>
#endif

#ifndef BALL_DATATYPE_HASHMAP_H
# include <BALL/DATATYPE/hashMap.h>
#endif

#ifndef BALL_DATATYPE_HASHSET_H
# include <BALL/DATATYPE/hashSet.h>
#endif

#ifndef BALL_CONCEPT_EMBEDDABLE_H
#	include <BALL/CONCEPT/embeddable.h>
#endif

#ifndef BALL_VIEW_KERNEL_UIOPERATIONMODE_H
# include <BALL/VIEW/KERNEL/UIOperationMode.h>
#endif

#include <QtGui/QMenu>
#include <QtGui/QTreeWidgetItem>

namespace BALL
{
	class Composite;

	namespace VIEW
	{
		class DatasetControl;
		class DatasetMessage;
		class MainControl;
		class DataMessage;

		/** Baseclass for datasets.
		 		Datasets are used to incorporate any kind of data and make it accessible through the
				DatasetControl. The data is stored as a void pointer and casted to its real type in the
				corresponding Contoller class.
				A dataset has the following attributes:
				<ul>
				<li> a type, used for the cast 
				<li> a name, used for the entry in the DatasetControl
				<li> a Composite pointer to know its corresponding molecular entity
				<li> the void pointer for access to the data
				</ul>
				<bf>NOTE:</bf> The data will not be deleted, when the Dataset is deleted.
				This has to be done in the DatasetController, since only it knows what kind of object
				is contained!
				@see DatasetControl
				@see DatasetController
				\ingroup ViewDatatype
		*/
		class BALL_VIEW_EXPORT Dataset
		{
			public:
	
			/**	@name	Constructors and Destructors
			*/	
			//@{

			/** Default Constructor.
			*/
			Dataset();

			/** Copy constructor.
			*/
			Dataset(const Dataset& dataset);

			/** Destructor.
			*/
			virtual ~Dataset();

			/** Explicit default initialization.
			*/
			virtual void clear();

			//@}
			/**	@name	Assignment methods
			*/
			//@{

			/** Assignment.
			*/
			void set(const Dataset& v);

			/** Assignment operator.
					Calls set.
			*/
			const Dataset& operator = (const Dataset& v);

			///
			void setName(String name) { name_ = name;}

			///
			String getName() const { return name_;}
			
			///
			void setType(String type) { type_ = type;}

			///
			String getType() const { return type_;}
			
			///
			void setComposite(Composite* composite) { composite_ = composite;}

			///
			Composite* getComposite() const { return composite_;}

			///
			void dump(std::ostream& s, Size depth) const;
			//
			//@}
			
			protected:
		
			Composite* 			composite_;
			String     			name_;
			String 					type_;
		};


		/** Macro to create derived Dataset classes
		*/
#define BALL_CREATE_DATASET(TYPE)\
		class BALL_VIEW_EXPORT TYPE##Dataset\
			: public Dataset\
		{\
			public :\
		\
			TYPE##Dataset(TYPE* data = 0)\
				: Dataset(),\
					data_(data)\
			{}\
			\
			TYPE##Dataset(const TYPE##Dataset& set)\
				: Dataset(set),\
					data_(set.data_)\
			{}\
			\
			TYPE* getData() { return data_;}\
			\
			void setData(TYPE* data) { data_ = data;}\
			\
			TYPE* data_;\
		};


		/** Controller base class for one type of Dataset
		 		A DatasetController provides all neccessary means to manage and manipulate
				a kind of data in the DatasetControl.
				The class has the following features:
				<ul>
				<li> It can be registered at a DatasetControl with the method DatasetControl::registerController.
				<li> It knows about its supported file formats, and can open them when requested.
						 (see getSupportedFileFormats())
				<li> It will then automatically call createMenuEntries and thus create the menu entries.
						 Derived classes can add further entries, by overloading this method.
				<li> The DatasetControl provides a context menu for the Datasets. Its entries are 
						 created in DatasetController::buildContextMenu.
				<li> Since DatasetController is derived from Embeddable, access to individual instances 
						 (also to derived classes) is possible through getInstance().
				<li> When a DatasetController is deleted, it deletes all Datasets and their data.
				</ul>
				To use this class, derive a new class from it and overload/create the following methods:
				<ul>
				<li> getData
				<li> buildContextMenu
				<li> creatMenuEntries
				<li> open
				<li> write
				<li> deleteDataset_
				</ul>
				Furthermore 
				<ul>
				<li> add a static String member type
				<li> assign file_formats_ in the derived classes constructor
				</ul>
				@see DatasetControl
				@see Dataset
				@see DatasetMessage
				@see Embeddable
		*/
		class BALL_VIEW_EXPORT DatasetController
			: public QObject,
			  public Embeddable
		{
			Q_OBJECT

			friend class DatasetControl;

			public:

			BALL_EMBEDDABLE(DatasetController,Embeddable)

			///
			DatasetController();

			///
			DatasetController(DatasetController& dc);

			///
			virtual ~DatasetController();

			///
			void setDatasetControl(DatasetControl* dc) { control_ = dc;}

			///
			DatasetControl* getDatasetControl() { return control_;}

			/** Message handling.
			 		If a DatasetControl receives a DatasetMessage, it 
					queries the data type and calls this method in the
					corresponding DatasetController.
					This method has intial support for DatasetMessage::ADD and
					DatasetMessage::REMOVE. For these cases, it
					calls insertDataset or respectively deleteDataset.
			*/
			virtual bool handle(DatasetMessage* msg);

			///
			virtual bool insertDataset(Dataset* set);

			///
			virtual bool deleteDataset(Dataset* set);

			/** Create menu entries for this data type.
			 		Initial support for open file menu entry is included.
					Overload this method for providing further entries.
			*/
			virtual bool createMenuEntries();

			/** Create a context menu for this data type.
			 		Initial support for deleting and saving entries is included.
					Overload this method for providing further entries.
			*/
			virtual QMenu* buildContextMenu(QTreeWidgetItem* item);

			/// Get all Datasets of this type in the DatasetControl.
			vector<Dataset*> getDatasets();

			/// Get all selected Datasets of this type in the DatasetControl.
			vector<Dataset*> getSelectedDatasets();

			/** Get one selected Datasets of this type in the DatasetControl.
			 		If multiple entries are selected in the DatasetControl, 0 is returned.
			*/
			Dataset* getSelectedDataset();

			/** Test if a Dataset is registered in this controller.
			*/
			bool hasDataset(Dataset* set);

			/// Test if a given QTreeWidgetItem corresponds to a registered Dataset.
			bool hasItem(QTreeWidgetItem* item);

			/// Get the Dataset for a QTreeWidgetItem
			Dataset* getDataset(QTreeWidgetItem* item);

			/** Get the String type identifier for this data type.
			*/
			String getType() { return type_;}

			/** Check the menu entries for this data type.
			 		If the MainControl is currently busy, all entries are disabled.
					If multiple entries are selected, it is tested, if an QAction is included in
					actions_for_one_set_. If true, this QAction is disabled.
			 		Overload this method for more sophisticated behaviour.
			*/
			virtual void checkMenu(MainControl& mc);

			/** Return a vector with all supported file formats.
			 		Set the member file_formats_ in the constructor of the derived class.
					The methods open(...) and write(...) must be overloaded such that they know
					how to handle the supported file formats.
			*/
			vector<String> getSupportedFileFormats() { return file_formats_;}

			public slots:

			/// Show a file dialog for writing the selected Dataset.
			virtual bool write();

			/** Show a file dialog to open a data file.
			 		Calls open(String, String).
					Overload this method to support multiple file formats.
			*/
			bool open();

			/// Delete all Datasets
			virtual bool deleteDatasets();

			/// Delete the currently highlighted Dataset
			virtual bool deleteDataset();

			protected:
			
			///
			void setStatusbarText(const String& text, bool important = false);
			void setStatusbarText(const QString& text, bool important = false);

			virtual bool write(Dataset* /*set*/, String /*filetype*/, String /*filename*/);
			virtual Dataset* open(String /*filetype*/, String /*filename*/);

			String getFileTypes_();
			String getNameFromFileName_(String filename);
			QAction* insertMenuEntry_(Position pid, const QString& name, const char* slot,
																const String& description = "", QKeySequence accel = QKeySequence(),
																const QString& menu_hint = "", 
																UIOperationMode::OperationMode initial_mode = UIOperationMode::MODE_ALL);
			
			/** In the derived class: cast to concrete type and delete the data!
			*/
			virtual void deleteDataset_(Dataset*) {};

			String 						type_;
			vector<String> 		file_formats_;
			DatasetControl* 	control_;
			vector<QAction*> 	actions_;
			HashSet<QAction*> actions_for_one_set_;
			HashMap<QTreeWidgetItem*, Dataset*> item_to_dataset_;
			HashMap<Dataset*, QTreeWidgetItem*> dataset_to_item_;
		};

	} // namespace VIEW
} // namespace BALL

#endif // BALL_VIEW_DATATYPE_DATASET_H
