// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: colorMeshDialog.h,v 1.3 2003/09/04 22:17:44 amoll Exp $
//

#ifndef BALL_VIEW_DIALOGS_COLORMESHDIALOG_H
#define BALL_VIEW_DIALOGS_COLORMESHDIALOG_H

#include <BALL/VIEW/UIC/colorMeshDialogData.h>

#ifndef BALL_VIEW_DATATYPE_COLORRGBA_H
# include <BALL/VIEW/DATATYPE/colorRGBA.h>
#endif

#ifndef BALL_VIEW_PRIMITIVES_MESH_H
# include <BALL/VIEW/PRIMITIVES/mesh.h>
#endif

#ifndef BALL_VIEW_KERNEL_REPRESENTATION_H
# include <BALL/VIEW/KERNEL/representation.h>
#endif

#ifndef BALL_DATATYPE_REGULARDATA3D
# include <BALL/DATATYPE/regularData3D.h>
#endif 

namespace BALL
{
	namespace VIEW
	{

		/** Dialog for coloring of surfaces
				\ingroup ViewDialogs
		*/
		class ColorMeshDialog 
			: public ColorMeshDialogData
		{ 
				Q_OBJECT

			public:

				///
				class ColoringConfig
				{
					public:

					ColorRGBA min_min_color, min_color, mid_color, max_color, max_max_color, custom_color;
					float min_value, mid_value, max_value;
					Size number_of_levels;
					Position transparency;
					Position tab;
					String selected_grid;
				};

				enum ColoringMethods
				{
					GRID_FROM_DISTANCE = 0,
					GRID_FROM_FDPD
				};

				ColorMeshDialog( QWidget* parent = 0, const char* name = 0, bool modal = FALSE, WFlags fl = 0 );
				~ColorMeshDialog()
					throw();
						
				void setRepresentation(Representation& rep)
					throw();

				Representation* getRepresentation()
					throw() { return rep_;}

				void setMesh(Mesh& mesh)
					throw() {mesh_ = &mesh;}

				Mesh* getMesh()
					throw() { return mesh_;}
				
		public slots:
				void applyPressed();
				void cancelPressed();
				void loadPressed();
				void colorBoxesChanged();
				void maxPressed();
				void midPressed();
				void minPressed();
				void minMinPressed();
				void maxMaxPressed();
				void tabChanged();		
				void choosePressed();
				void computePressed();
				void deletePressed();
				void gridSelected();
				void gridChoosen();
				void savePressed();
				void autoScalePressed();

		protected:
				QColor setColor(QPushButton* button);
				void loadGrid_(const String& filename);
				void saveGrid_(const String& filename);
				void colorByCustomColor_();
				void colorByGrid_();
				bool insertGrid_(RegularData3D& grid, const String& name);
				void setColor_(ColorRGBA& color, const QPushButton* button, const QSpinBox* box);
				void getColor_(const ColorRGBA& color, QPushButton* button, QSpinBox* box);
				RegularData3D* createGridFromDistance_();
				RegularData3D* createGridFromFPDB_();
				void saveSettings_();
				void loadSettings_();

				vector<RegularData3D*> grids_;
				vector<float> min_values_;
				vector<float> mid_values_;
				vector<float> max_values_;

				ColorRGBA	 	selected_color, min_min_color, min_color, mid_color, max_color, max_max_color;	

				HashMap<Representation*, ColoringConfig> configs_;
				Mesh* mesh_;		
				Representation* rep_;
		};

} } // Namespaces
#endif
