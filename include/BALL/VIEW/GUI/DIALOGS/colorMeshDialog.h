#ifndef BALL_VIEW_GUI_COLORMESHDIALOG_H
#define BALL_VIEW_GUI_COLORMESHDIALOG_H

#ifndef COLORMESHDIALOGDATA_H
#include "colorMeshDialogData.h"
#endif

#ifndef BALL_VIEW_PRIMITIV_MESH_H
# include <BALL/VIEW/PRIMITIV/mesh.h>
#endif

namespace BALL
{
	namespace VIEW
	{

		class ColorMeshDialog 
			: public colorMeshDialogData
		{ 
			Q_OBJECT

			public:
				ColorMeshDialog( QWidget* parent = 0, const char* name = 0, bool modal = FALSE, WFlags fl = 0 );
								
				~ColorMeshDialog();

				Mesh *mesh;
								
			public slots:
							
				/** @name Public slots
				 */
				//@{
				
				/** Color the mesh.
				 */
				void colorMesh();

				/** Browse file names.
				 */
				void browseLoadFiles();
		};

	}
}

#endif // COLORMESHDIALOG_H
