#ifndef INPUTDATAITEM_H
#define INPUTDATAITEM_H

#include <BALL/APPLICATIONS/QuEasyViz/dataItem.h>
#include <BALL/QSAR/QSARData.h>

#include <QtGui/QGraphicsSceneMouseEvent>



namespace BALL
{
	namespace VIEW
	{
		using namespace BALL::QSAR;
		
		class InputPlotter;
		
		/** @class InputDataItem
		* @brief class for the representation of input data
		*
		* @todo
		*/
		class InputDataItem : public DataItem
		{
			Q_OBJECT
					
			public:
				/** @name Constructors and Destructors
				*/
				virtual ~InputDataItem();

				InputDataItem(QString filename, bool center_data, bool center_y, DataItemView* view);
				//InputDataItem(QString filename, QString name, DataItemView* view);
				InputDataItem(QString filename, DataItemView* view);
				InputDataItem(InputDataItem& item);
				InputDataItem();

				/** @name Accessors
				*/
				
				/** Returns a pointer to the data saved in the item*/
				QSAR::QSARData* data();
				enum { Type = UserType + 2 };
		
				/** Returns the type of the item as an int. This type information is used by qgraphicsitem_cast() to distinguish between types. */
				int type() const { return Type; }

				QString filename();
				bool centerData();
				bool centerY();
				void setCenterDataFlag(bool cd);
				void setCenterResponseFlag(bool cr);
				void setData(QSAR::QSARData* data);
				void setFilename(String filename) { filename_=filename.c_str();}
				void setAppend(bool append);
				bool append();
				void setNonNumericClassNames(bool b) {nonnumeric_class_names_ = b;}
				bool getNonNumericClassNames() {return nonnumeric_class_names_;}
			
				/** load preprocessed input data from a file */
				virtual void loadFromFile(String file);

				virtual bool checkForDiscreteY() = 0;
			
			public slots:
				void showPlotter();
				

			protected:
				/** @name Attributes
				*/
		
				/**pointer to an instance of QSARData that holds the input data read from a file */
				QSAR::QSARData* data_;
				QString filename_;
				bool center_data_;
				bool center_y_;
				
				/** is the data of this InputDataItem append to another InputDataItem?? */
				bool append_;
				
				/** has a check for discrete response variables already been done? */
				bool checked_for_discrete_y_;
				
				/** do all response variables contain only discrete values? */
				bool discrete_y_;
				
				/** determines whether class names are to be convertig to IDs while reading the input data */
				bool nonnumeric_class_names_;
				
				/** the IDs of PartitioningItems that are connected to this item */
				SortedList<int> partitioner_IDs_;
				
				InputPlotter* input_plotter_;
				
				void createActions();
				
				
			private:
				BALL::String getMouseOverText();
				
				friend class InputPartitionItem;
				friend class PartitioningItem;
		};
	}
}	

#endif
