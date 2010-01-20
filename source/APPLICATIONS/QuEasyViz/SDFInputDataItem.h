/* SDFInputDataItem.h
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

#ifndef SDFInputDataItem_H
#define SDFInputDataItem_H

#include <BALL/APPLICATIONS/QuEasyViz/inputDataItem.h>
#include <BALL/APPLICATIONS/QuEasyViz/CSVInputDataItem.h>

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
				
				/** specifies whether or not properties of the SD-file should be used as descriptors */
				void useSDProperties(bool b);
				
				bool useSDProperties();
				
				/** read the input data */
				bool execute();
				
				void appendCSVDescriptors(CSVInputDataItem* item);
				list<CSVInputDataItem*>* getConnectedCSVItems();
				
				void addToPipeline();
				void removeFromPipeline();
				
				/** Replace the given InputDataItem.\n
				Thus all edges are transferred from the old item to this item, the latter one is inserted into the correct pipeline at the same position than the old item, which is then deleted.*/
				void replaceItem(InputDataItem* old_item);
				
				void loadFromFile(String file);

		
			private:
				
				/** @name Attributes
				*/
				SortedList<int> activity_values_;
				
				bool use_SD_properties_;
				
				/** Are there additional descriptors that are to be read from a csv-file?!\n
				If yes, center the input data only after the data of the last CSVInputDataItem within this list has been read!! */
				list<CSVInputDataItem*> additional_descriptors_;
				
			protected:
				void contextMenuEvent(QGraphicsSceneContextMenuEvent *event);
				
				friend class DataItemScene;
		};
	}
}	

#endif
