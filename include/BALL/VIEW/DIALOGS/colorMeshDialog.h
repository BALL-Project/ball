// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: colorMeshDialog.h,v 1.16 2005/02/28 20:25:49 amoll Exp $
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

#ifndef BALL_VIEW_KERNEL_MODULARWIDGET_H
# include <BALL/VIEW/KERNEL/modularWidget.h>
#endif

#ifndef BALL_DATATYPE_REGULARDATA3D
# include <BALL/DATATYPE/regularData3D.h>
#endif 

namespace BALL
{
	namespace VIEW
	{
		/** Dialog for coloring of surfaces
		 		The surfaces can be colored by a given color, or from the values, contained 
				in a RegularData3D grid. You can also set the transparency of the surface.
				\ingroup ViewDialogs
		*/
		class BALL_EXPORT ColorMeshDialog 
			: public ColorMeshDialogData,
				public ModularWidget
		{ 
			Q_OBJECT

			public:

			BALL_EMBEDDABLE(ColorMeshDialog,ModularWidget)

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

			///
			ColorMeshDialog(QWidget* parent = 0, const char* name = 0, bool modal = FALSE, WFlags fl = 0);

			///
			~ColorMeshDialog()
				throw();
					
			///
			virtual void onNotify(Message *message)
				throw();

			/// 
			void setMesh(Mesh* mesh, Representation* rep)
				throw();

			void setGrid(RegularData3D* grid)
				throw();
				
			public slots:
			
			void applyPressed();
			void cancelPressed();
			void colorBoxesChanged();
			void maxPressed();
			void midPressed();
			void minPressed();
			void minMinPressed();
			void maxMaxPressed();
			void tabChanged();		
			void autoScalePressed();
			void choosePressed();
			void gridSelected();
			void show();

			protected slots:

			void gridTransparencyChanged();
			void customColorTransparencyChanged();

			protected:
			void colorByCustomColor_();
			bool colorByGrid_();
			bool insertGrid_(RegularData3D& grid, const String& name);
			void removeGrid_(RegularData3D& grid);
			void setColor_(ColorRGBA& color, const QLabel* label, const QSpinBox* box, const QRadioButton* rbutton);
			void getColor_(const ColorRGBA& color, QLabel* label, QSpinBox* box);
			void saveSettings_();
			void loadSettings_();
			void invalidateGrid_() throw();
			void invalidateMesh_() throw();
			void calculateValues_();

			RegularData3D* grid_;
			float min_value_;
			float mid_value_;
			float max_value_;

			ColorRGBA	 	selected_color, min_min_color, min_color, mid_color, max_color, max_max_color;	

			HashMap<Representation*, ColoringConfig> configs_;
			Mesh* mesh_;		
			Representation* rep_;
			List<RegularData3D*> grid_list_;
		};

	} // namespace VIEW
} // namespace BALL

#endif
