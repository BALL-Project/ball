#ifndef INPUTDATAITEMIO_H
#define INPUTDATAITEMIO_H

#include <BALL/APPLICATIONS/QuEasyViz/dataItemView.h>
#include <BALL/APPLICATIONS/QuEasyViz/CSVInputDataItem.h>
#include <BALL/APPLICATIONS/QuEasyViz/SDFInputDataItem.h>
#include <BALL/APPLICATIONS/QuEasyViz/partitioningItem.h>
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
