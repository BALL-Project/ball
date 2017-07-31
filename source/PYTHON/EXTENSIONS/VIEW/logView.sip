class LogView
	: DockWidget
{
%TypeHeaderCode
	#include <BALL/VIEW/WIDGETS/logView.h>
%End
	public:
	static Size countInstances();
	static LogView* getInstance(Position);
	LogView() throw();
	~LogView() throw();
};
