// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: datasetControl.h,v 1.19.16.1 2007/03/25 21:26:18 oliver Exp $
//

#ifndef BALL_VIEW_WIDGETS_DATASETCONTROL_H
#define BALL_VIEW_WIDGETS_DATASETCONTROL_H

#ifndef BALL_KERNEL_SYSTEM_H
# include <BALL/KERNEL/system.h>
#endif

#ifndef BALL_VIEW_WIDGETS_GENERICCONTROL_H
# include <BALL/VIEW/WIDGETS/genericControl.h>
#endif

#include <QtGui/QTreeWidgetItem>

namespace BALL
{
	namespace VIEW
	{
		class Dataset;
		class DatasetController;
		class Message;

		/**	DatasetControl is a widget to manage and manipulate arbitrary data types.
		 		For each type a derived DatasetController class must be registered.
				The DatasetControl has three columns: Name, from (Composite) and Type.
				@see DatasetControl
				@see Dataset
				@see DatasetMessage
				\ingroup ViewWidgets
		*/
		class BALL_VIEW_EXPORT DatasetControl
			: public GenericControl
		{
			Q_OBJECT

			public:

			BALL_EMBEDDABLE(DatasetControl,GenericControl)

			/** Default Constructor.
					Calls registerWidget().
					\param      parent the parent widget of the DatasetControl 
					\param      name the name of the DatasetControl 
					\see        ModularWidget
			*/
			DatasetControl(QWidget* parent = 0, const char* name = 0);
			
			/** Destructor.
			*/
			virtual ~DatasetControl();

			/** Overload this method to react to further messages
			 		Reacts to DatasetMessage, it will be passed on to
					the corresponding DatasetController.
			*/
			virtual void onNotify(Message *message);

			/**	Initialize the widget.
			*/
			virtual void initializeWidget(MainControl& main_control);

			/** Check the menu entries.
			 		Calls DatasetController::checkMenu
			*/
			virtual void checkMenu(MainControl& main_control);

			/** Test if this ModularWidget can handle a given file format.
					(Overloaded from ModularWidget)
					Calls DatasetControl::getSupportedFileFormats.
			 		@param fileform short string with the file extension (e.g. PDB)
					@see openFile
			*/
			virtual bool canHandle(const String& fileformat) const;

			/** Tell this ModularWidget to open a given file.
					(Overloaded from ModularWidget)
					Will call DatasetController::openFile.
			 		@see canHandle
			*/
			virtual bool openFile(const String& filename);

			/** Register a DatasetController.
			 		The DatasetController must be created on the heap and
					will be deleted together with the DatasetControl.
			*/
			virtual bool registerController(DatasetController* con);

			/** Return the registered DatasetController with the given type.
			*/
			DatasetController* getController(const String& type);

			/** Get the DatasetController, that is responsible for
			 		the item.
			*/
			DatasetController* getController(QTreeWidgetItem* item);

			/** Get all Datasets with one type
			 		@see Datset::getType
					@see DatasetController::getType
			*/
			vector<Dataset*> getDatasets(const String& type);

			/// Return the number of selected entries
			Size getSelectionSize();

			/// Get the type for one QTreeWidgetItem
			String getType(QTreeWidgetItem* item);

			/// Deletes the currently selected items
			virtual void deleteCurrentItems();

			public slots:
				
			// Overloaded from GenericControl 
//   			virtual void deleteCurrentItems();

		  protected slots:

			virtual void showGuestContextMenu(const QPoint& pos);
			
			/** Called when the item selection changes.
			 		Sends a DatasetMessage with type DatasetMessage::SELECTED.
			*/
			void updateSelection();

		  protected:
			
			// only for Python Interface
			DatasetControl(const DatasetControl& control);

			vector<DatasetController*> controllers_;
		};
		
} } // namespaces

#endif // BALL_VIEW_WIDGETS_DATASETCONTROL_H
