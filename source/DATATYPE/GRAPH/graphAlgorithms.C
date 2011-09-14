#include <BALL/DATATYPE/GRAPH/graphAlgorithms.h>
#include <BALL/DATATYPE/string.h>

namespace BALL
{
	namespace GRAPH
	{
		UnconnectedGraphException::UnconnectedGraphException(const char * file, int line) 
			: Exception::GeneralException(file, line, String("UnconnectedGraphException"),
					String("This operation can't be performed on unconnected graphs!")) 
		{
		}

		UnconnectedGraphException::UnconnectedGraphException(const char * file, int line, BALL::String computation) 
			: Exception::GeneralException(file, line, String("UnconnectedGraphException"),
					String("Cannot compute ") + computation +
					String(" on unconnected graphs!")) 
		{
		}

		IllegalStateException::IllegalStateException(const char* file, int line, String message) 
			: Exception::GeneralException(file, line, "IllegalStateException", message)
		{
		} 

	}
}
