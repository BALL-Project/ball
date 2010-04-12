#include "../include/BALLViewPluginConfiguration.h"



namespace BALL
{
	namespace VIEW
	{
		

		// Parse xml file
		// load config parameters
		void CommunicationConfiguration::ParseXmlFile(const char * file_name)
		{
			string input;

			std::ifstream myfile (file_name);



			if (myfile.is_open())
			{
				while (! myfile.eof())
				{
					std::getline (myfile,input,';');

					if (input.find("localip") !=string::npos)
					{

						local_ip_ = input.substr(8,input.length());

					}
					else if (input.find("remoteip")!=string::npos)
					{
						
						remote_ip_ = input.substr(9,input.length());

					}
					else if (input.find("localport")!=string::npos)
					{


						string localport_value = input.substr(10,input.length());

						const char * local_port_val_= localport_value.c_str();

						local_port_ = atoi(local_port_val_);


					}
					else if (input.find("remoteport")!=string::npos)
					{

						string remoteport_value = input.substr(11,input.length());

						const char * remote_port_val_= remoteport_value.c_str();

						remote_port_ =  atoi(remote_port_val_);


					}
			
				}

				myfile.close();
			}
			else 
			{
				std::cout << "Unable to open file"; 
			}

			
			
		}


		CommunicationConfiguration::CommunicationConfiguration()
		{


			
			
		}

	}
}




