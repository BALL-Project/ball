#ifndef SDFInputDataItem_H
#define SDFInputDataItem_H

#include <BALL/APPLICATIONS/QSAR_GUI/inputDataItem.h>
#include <BALL/APPLICATIONS/QSAR_GUI/CSVInputDataItem.h>

#include <QtGui/QGraphicsSceneMouseEvent>

using namespace BALL::QSAR;

namespace BALL
{
	namespace VIEW
	{

		/** @class SDFInputDataItem
		* @brief class for the representation of input data read from a SD-file
		*
		* @todo
		*/
		class SDFInputDataItem : public InputDataItem
		{
			public:
				/** @name Constructors and Destructors
				*/
				
				SDFInputDataItem(QString filename,SortedList<int> act, bool cdv, bool crv, DataItemView* view);
				SDFInputDataItem(QString filename, QString name, DataItemView* view);
				SDFInputDataItem(QString filename, DataItemView* view);
				
				~SDFInputDataItem();

				SDFInputDataItem(SDFInputDataItem& item);
				/** @name Accessors
				*/

				enum { Type = UserType + 125 };
		
				/** Returns the type of the item as an int. This type information is used by qgraphicsitem_cast() to distinguish between types. */
				int type() const { return Type; }
				SortedList<int> activityValues();
				void setActivityValues(SortedList<int> act);
				bool checkForDiscreteY();
				void readData();
				void appendCSVDescriptors(CSVInputDataItem* item);
				list<CSVInputDataItem*>* getConnectedCSVItems();

				void addToPipeline();
				void removeFromPipeline();

		
			private:
				/** @name Attributes
				*/
				SortedList<int> activity_values_;
				
				/** Are there additional descriptors that are to be read from a csv-file?!\n
				If yes, center the input data only after the data of the last CSVInputDataItem within this list has been read!! */
				list<CSVInputDataItem*> additional_descriptors_;
				
			protected:
				void mousePressEvent(QGraphicsSceneMouseEvent *event);
				void contextMenuEvent(QGraphicsSceneContextMenuEvent *event);
				
				friend class DataItemScene;
		};
	}
}	

#endif
