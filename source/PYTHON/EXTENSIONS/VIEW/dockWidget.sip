class DockWidget
	: ModularWidget
{
%TypeHeaderCode
	#include <BALL/VIEW/WIDGETS/dockWidget.h>
%End
	public:

	static Size countInstances();
	static DockWidget* getInstance(Position);

	DockWidget();
	~DockWidget() throw();

	void applyPreferences() throw();
	void setWidgetVisible(bool);

	private:
	DockWidget(const DockWidget&);
};
