/* CSVInputDataItem.h
 * 
 * Copyright (C) 2009 Marcel Schumann
 * 
 * This file is part of QuEasy -- A Toolbox for Automated QSAR Model
 * Construction and Validation.
 * QuEasy is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or (at
 * your option) any later version.
 * 
 * QuEasy is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, see <http://www.gnu.org/licenses/>.
 */

#ifndef CVS_INPUTDATAITEM_H
#define CVS_INPUTDATAITEM_H

#include <inputDataItem.h>
#include <BALL/QSAR/QSARData.h>

#include <QtGui/QGraphicsSceneMouseEvent>

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
				CSVInputDataItem(BALL::QSAR::QSARData* data, DataItemView* view);
				
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
