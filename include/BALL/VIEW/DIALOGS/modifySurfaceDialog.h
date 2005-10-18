// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: modifySurfaceDialog.h,v 1.1.2.9 2005/10/18 11:11:17 amoll Exp $
//

#ifndef BALL_VIEW_DIALOGS_modifySurfaceDIALOG_H
#define BALL_VIEW_DIALOGS_modifySurfaceDIALOG_H

#include <BALL/VIEW/UIC/modifySurfaceDialogData.h>

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

#ifndef BALL_DATATYPE_HASHGRID_H
# include <BALL/DATATYPE/hashGrid.h>
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
		class BALL_VIEW_EXPORT ModifySurfaceDialog 
			: public ModifySurfaceDialogData,
				public ModularWidget
		{ 
			Q_OBJECT

			public:

			BALL_EMBEDDABLE(ModifySurfaceDialog, ModularWidget)

			///
			ModifySurfaceDialog(QWidget* parent = 0, const char* name = 0, bool modal = FALSE, WFlags fl = 0);

			///
			~ModifySurfaceDialog()
				throw();
					
			///
			virtual void onNotify(Message *message)
				throw();

			/// 
			void setRepresentation(Representation* rep)
				throw();

			///
			void setGrid(RegularData3D* grid)
				throw();
					
			///
			void setMinValue(float value);
					
			///
			void setMaxValue(float value);
					
			///
			void setMidValue(float value);
				
			public slots:
			
			void applyPressed();
			void cancelPressed();
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
			void changeDrawingModeTransparencyChanged();
			void splitMethodChanged();

			protected:

			typedef HashGrid3<const Atom*>  AtomGrid;
			typedef HashGridBox3<const Atom*> AtomBox;
			void colorByCustomColor_();
			bool colorByGrid_();
			bool insertGrid_(RegularData3D& grid, const String& name);
			void removeGrid_(RegularData3D& grid);
			void setColor_(ColorRGBA& color, const QLabel* label, const QSpinBox* box, 
										 const QRadioButton* rbutton);
			void getColor_(const ColorRGBA& color, QLabel* label, QSpinBox* box);
			void invalidateGrid_() throw();
			void invalidateMesh_() throw();
			void calculateValues_();
			void split_();
			void changeDrawingMode_();
			void checkApplyButton_();

			void calculateIncludedVertices_(vector<bool>& include_vertex, const Mesh& org_mesh, HashSet<const Composite*>& roots);
			inline bool checkInclude_(const AtomGrid& atom_grid, const Vector3& point) const;

			RegularData3D* grid_;
			float min_value_;
			float mid_value_;
			float max_value_;
			float square_distance_;

			ColorRGBA	 	selected_color, min_min_color, min_color, mid_color, max_color, max_max_color;	

			Mesh* mesh_;		
			Representation* rep_;
			List<RegularData3D*> grid_list_;
		};

	} // namespace VIEW
} // namespace BALL

#endif
