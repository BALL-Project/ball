#ifndef CONTOURSURFACEDIALOG_H
#define CONTOURSURFACEDIALOG_H
#include "contourSurfaceDialogData.h"

#ifndef BALL_VIEW_DATATYPE_COLORRGBA_H
# include <BALL/VIEW/DATATYPE/colorRGBA.h>
#endif

namespace BALL
{

	using VIEW::ColorRGBA;

	namespace MOLVIEW
	{

class ContourSurfaceDialog : public ContourSurfaceDialogData
{ 
    Q_OBJECT

public:
    ContourSurfaceDialog( QWidget* parent = 0, const char* name = 0, bool modal = FALSE, WFlags fl = 0 );
    ~ContourSurfaceDialog();
				
		ColorRGBA	 	color;
		String 			location;		
		ColorRGBA		min_min_color;
		ColorRGBA 	min_color;
		ColorRGBA   mid_color;
		ColorRGBA		max_color;
		ColorRGBA		max_max_color;	

		float minimum;
		float maximum;
		Size nr_of_levels;		
		
public slots:
    void apply_clicked();
    void browse_clicked();
    void cancel_clicked();
    void choose_clicked();
    void color_boxes_changed();
    void location_changed();
    void max_clicked();
    void mid_clicked();
    void min_clicked();
    void min_min_clicked();
    void max_max_clicked();
    void tab_changed();		
};

	}
}

#endif // CONTOURSURFACEDIALOG_H
