#include <qwt_plot.h>
#include <BALL/APPLICATIONS/QSAR_GUI/dataItem.h>
#include <list>
#include <QString>
#include <QtGui/QDialog>
#include <QMouseEvent>



namespace BALL
{
	namespace VIEW
	{
		class Plotter : public QwtPlot
		{				
			protected:
				/** constructor */
				Plotter(DataItem* item);
			
				~Plotter();
				
				DataItem* item_;
				
				/** contains a name for each plotted data point.\n
				Can be used e.g. for mouse-over effects... */
				std::list<QString> names_;
			
		};
	}
}
