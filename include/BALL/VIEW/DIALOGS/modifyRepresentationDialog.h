// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: modifyRepresentationDialog.h,v 1.1.4.1 2007/03/25 21:25:52 oliver Exp $
//

#ifndef BALL_VIEW_DIALOGS_MODIFYSURFACEDIALOG_H
#define BALL_VIEW_DIALOGS_MODIFYSURFACEDIALOG_H

#ifndef BALL_COMMON_GLOBAL_H
# include <BALL/COMMON/global.h>
#endif

#ifdef BALL_COMPILER_MSVC
# include <BALL/VIEW/UIC/ui_modifyRepresentationDialog.h>
#else
# include <BALL/VIEW/UIC/modifyRepresentationDialogData.h>
#endif


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

#ifndef BALL_VIEW_DIALOG_MATERIALSETTINGS_H
# include <BALL/VIEW/DIALOGS/materialSettings.h>
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
		class BALL_VIEW_EXPORT ModifyRepresentationDialog 
			: public QDialog,
				public Ui_ModifyRepresentationDialogData,
				public ModularWidget
		{ 
			Q_OBJECT

			public:

			BALL_EMBEDDABLE(ModifyRepresentationDialog, ModularWidget)

			///
			ModifyRepresentationDialog(QWidget* parent = 0, const char* name = "ModifyRepresentationDialog", 
																 bool modal = FALSE, Qt::WFlags fl = 0);

			///
			~ModifyRepresentationDialog();
					
			///
			virtual void onNotify(Message *message);

			/// 
			void setRepresentation(Representation* rep);

			///
			void setGrid(RegularData3D* grid);
					
			///
			void setMinValue(float value);
					
			///
			void setMaxValue(float value);
					
			///
			void setMidValue(float value);

			///
			void setSplitRadius(float distance);

			///
			void setMode(Position pos);
				
			public slots:
			
			void applyPressed();
			void cancelPressed();
			void maxPressed();
			void midPressed();
			void minPressed();
			void minMinPressed();
			void maxMaxPressed();
			void tabChanged();		
			void autoScale();
			void choosePressed();
			void gridSelected();
			void show();
			void applySplit();

			protected slots:

			void gridTransparencyChanged();
			void customColorTransparencyChanged();
			void changeDrawingModeTransparencyChanged();
			void splitMethodChanged();
			void normalizationChanged();

			protected:

			ModifyRepresentationDialog(const ModifyRepresentationDialog& dialog);

			typedef HashGrid3<const Atom*>  AtomGrid;
			typedef HashGridBox3<const Atom*> AtomBox;
			void colorByCustomColor_();
			bool colorByGrid_();
			bool insertGrid_(RegularData3D& grid, const String& name);
			void removeGrid_(RegularData3D& grid);
			void setColor_(ColorRGBA& color, const QLabel* label, const QSpinBox* box);
			void getColor_(const ColorRGBA& color, QLabel* label, QSpinBox* box);
			void invalidateGrid_();
			void invalidateMesh_();
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

			Representation* rep_;
			std::list<RegularData3D*> grid_list_;
			vector<Vector3> vertices_;
			MaterialSettings* material_settings_;
		};

	} // namespace VIEW
} // namespace BALL

#endif
