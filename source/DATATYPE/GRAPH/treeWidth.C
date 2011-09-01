#include <BALL/DATATYPE/GRAPH/treeWidth.h>
#include <BALL/DATATYPE/string.h>

using namespace std;

namespace BALL
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

}
