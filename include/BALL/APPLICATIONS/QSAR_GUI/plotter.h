#include <qwt_plot.h>
#include <BALL/APPLICATIONS/QSAR_GUI/dataItem.h>
#include <list>
#include <QString>
#include <QtGui/QDialog>
#include <QMouseEvent>
#include <QPushButton>
#include <QtGui/QDialogButtonBox>
#include <QHBoxLayout>

#include <qwt_symbol.h>




namespace BALL
{
	namespace VIEW
	{
		class Plotter : public QwtPlot
		{				
			public: 
				
				/** the symbol to be used for each data point within the plots */
				QwtSymbol data_symbol;
				
				/** the font to to be used for each data point within the plots */
				QFont data_label_font;
				
				/** determines how each label is aligned with respect to its data point */
				Qt::Alignment data_label_alignment;
				
				/** determines whether or not labels for each data point are to be drawn */
				bool show_data_labels;
			
			
			
			
			protected:
				/** constructor */
				Plotter(DataItem* item);
			
				~Plotter();
				
				DataItem* item_;
				
				/** contains a name for each plotted data point.\n
				Can be used e.g. for mouse-over effects... */
				std::list<QString> names_;
				
				QDialogButtonBox* buttons_;
				QPushButton* okButton_;
				QHBoxLayout* buttonsLayout_;
	
			
		};
	}
}
