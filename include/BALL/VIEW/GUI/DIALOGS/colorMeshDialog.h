#ifndef BALL_VIEW_GUI_COLORMESHDIALOG_H
#define BALL_VIEW_GUI_COLORMESHDIALOG_H

#include "colorMeshDialogData.h"

#ifndef BALL_VIEW_DATATYPE_COLORRGBA_H
# include <BALL/VIEW/DATATYPE/colorRGBA.h>
#endif

#ifndef BALL_VIEW_PRIMITIV_MESH_H
# include <BALL/VIEW/PRIMITIV/mesh.h>
#endif

namespace BALL
{
	namespace VIEW
	{

class ColorMeshDialog 
	: public ColorMeshDialogData
{ 
    Q_OBJECT

	public:
    ColorMeshDialog( QWidget* parent = 0, const char* name = 0, bool modal = FALSE, WFlags fl = 0 );
    ~ColorMeshDialog();
				
		ColorRGBA	 	selected_color;
		ColorRGBA		min_min_color;
		ColorRGBA 	min_color;
		ColorRGBA   mid_color;
		ColorRGBA		max_color;
		ColorRGBA		max_max_color;	

		Mesh* mesh;		
		
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
protected:
		QColor setColor(QPushButton* button);
};

	}
}

#endif
