#ifndef CVS_INPUTDATAITEM_H
#define CVS_INPUTDATAITEM_H

#include <BALL/APPLICATIONS/QuEasyViz/inputDataItem.h>
#include <BALL/QSAR/QSARData.h>

#include <QtGui/QGraphicsSceneMouseEvent>

using namespace BALL::QSAR;

namespace BALL
{
	namespace VIEW
	{
		class CSVInputDataItem : public InputDataItem
		{
			public:
				/** @name Constructors and Destructors
				*/
				//CSVInputDataItem(QString file, int no_y, bool xlabels, bool ylabels, const char* sep);	
				CSVInputDataItem(QString filename, DataItemView* view);	
				~CSVInputDataItem();
				CSVInputDataItem(CSVInputDataItem& item);
				CSVInputDataItem(QSARData* data, DataItemView* view);
				
				/** read the input data */
				bool execute();
				

				/** @name Accessors
				*/
				
				enum { Type = UserType + 124 };
		
				/** Returns the type of the item as an int. This type information is used by qgraphicsitem_cast() to distinguish between types. */
				int type() const { return Type; }
				void setXLabelFlag(bool x);
				void setYLabelFlag(bool y);
				void setNumOfActivities(int num);
				void setSeperator(string sep);
				bool checkForDiscreteY();
				string getSeperator() {return sep_;}
				bool getDescriptorLabels() {return x_labels_;}
				bool getCompoundLabels() {return y_labels_;}
				int getNoResponseVariables() {return no_y_;}
				void addToPipeline();
				void removeFromPipeline();
				
				/** Replace the given InputDataItem.\n
				Thus all edges are transferred from the old item to this item, the latter one is inserted into the correct pipeline at the same position than the old item, which is then deleted.*/
				void replaceItem(InputDataItem* old_item);

				
			private:
				void readData();
				void appendData();
				
				/** @name Attributes
				*/
				int no_y_;
				bool x_labels_;
				bool y_labels_;
				string sep_;

			protected:
				void contextMenuEvent(QGraphicsSceneContextMenuEvent *event);
		
		};
	}
}	

#endif
