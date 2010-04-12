#include <string>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <sstream>


#include <BALL/DATATYPE/string.h>

namespace BALL
{
	namespace VIEW
	{
		 class CommunicationConfiguration
		 {

			 friend class BALLViewOpenSimPlugin;

			 protected:

					String local_ip_;
					
					String remote_ip_;

					Size remote_port_;

					Size local_port_;

					
					

			 public:

					CommunicationConfiguration();

		    		String getRemoteIp();

					String getLocalIp();
						
					Size getRemotePort();

					Size getLocalPort();


					void setRemoteIp();

					void setLocalIp();
						
					void setRemotePort();

					void setLocalPort();

					void ParseXmlFile(const char * file_name);



		 };
	}
}