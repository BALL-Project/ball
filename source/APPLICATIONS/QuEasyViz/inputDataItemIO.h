
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
				void readConfigSection(String& configfile_section, std::map<String, DataItem*>& filenames_map, std::list<std::pair<double,double> >* item_positions, const String& directory);
				
				void writeConfigSection(SDFInputDataItem* sd_item, std::ofstream& out, std::ostringstream& item_positions, const String& directory);
				void writeConfigSection(CSVInputDataItem* sd_item, std::ofstream& out, const String& directory);
				void writeConfigSection(PartitioningItem* item, std::ofstream& out, std::ostringstream& item_positions);
						
				
			private:
				
				/** restore a PartitioningItem. This function is called automatically from readConfigSection() if an InputPartitioner-section is observed. */
				void readPartitionerSection(String& configfile_section, std::map<String, DataItem*>& filenames_map, std::list<std::pair<double,double> >* item_positions);
				
				DataItemView* view_;
				
				std::set<CSVInputDataItem*> saved_csv_;
				std::set<CSVInputDataItem*> written_csv_;
		};
	}
}

#endif // INPUTDATAITEMIO_H
