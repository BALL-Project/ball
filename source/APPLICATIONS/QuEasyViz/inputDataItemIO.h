/* inputDataItemIO.h
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

#ifndef INPUTDATAITEMIO_H
#define INPUTDATAITEMIO_H

#include <dataItemView.h>
#include <CSVInputDataItem.h>
#include <SDFInputDataItem.h>
#include <partitioningItem.h>
#include <BALL/DATATYPE/string.h>
#include <map>
#include <set>

namespace BALL
{
	namespace VIEW
	{

		/** @class InputDataItemIO
		* @brief Helper class for creation of InputSDFItems and CSVInputItems from config-file sections
		*/
		class InputDataItemIO
		{
			public:
				InputDataItemIO(DataItemView* view);
			
				/** reads the given config-file section, creates all neccessary SDFInputDataItems and CSVInputDataItems, registers them in the view and in the Pipeline-QSets of MainWindow and creates edges between those input items */
				void readConfigSection(String& configfile_section, std::map<String, DataItem*>& filenames_map,list<pair<double,double> >* item_positions, const String& directory);
				
				void writeConfigSection(SDFInputDataItem* sd_item, ofstream& out, ostringstream& item_positions, const String& directory);
				void writeConfigSection(CSVInputDataItem* sd_item, ofstream& out, const String& directory);
				void writeConfigSection(PartitioningItem* item, ofstream& out, ostringstream& item_positions);
						
				
			private:
				
				/** restore a PartitioningItem. This function is called automatically from readConfigSection() if an InputPartitioner-section is observed. */
				void readPartitionerSection(String& configfile_section, map<String, DataItem*>& filenames_map, list<pair<double,double> >* item_positions);
				
				DataItemView* view_;
				
				set<CSVInputDataItem*> saved_csv_;
				set<CSVInputDataItem*> written_csv_;
		};
	}
}

#endif // INPUTDATAITEMIO_H
