#ifndef CVS_INPUTDATAITEM_H
#define CVS_INPUTDATAITEM_H

#include <BALL/APPLICATIONS/QSAR_GUI/inputDataItem.h>
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
				CSVInputDataItem(QSARData* data);
				void readData();
				void appendData();

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
				void setAppend(bool append);
				bool append();
				string getSeperator() {return sep_;}
				bool getDescriptorLabels() {return x_labels_;}
				bool getCompoundLabels() {return y_labels_;}
				int getNoResponseVariables() {return no_y_;}
				void setDone(bool b) {done_=b;}

				void addToPipeline();
				void removeFromPipeline();

			private:
				/** @name Attributes
				*/
				int no_y_;
				bool x_labels_;
				bool y_labels_;
				bool append_;
				string sep_;

			protected:
				void mousePressEvent(QGraphicsSceneMouseEvent *event);
				void contextMenuEvent(QGraphicsSceneContextMenuEvent *event);
		
		};
	}
}	

#endif
