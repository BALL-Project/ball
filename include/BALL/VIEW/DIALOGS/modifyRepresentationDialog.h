// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#ifndef BALL_VIEW_DIALOGS_MODIFYSURFACEDIALOG_H
#define BALL_VIEW_DIALOGS_MODIFYSURFACEDIALOG_H

#ifndef BALL_COMMON_GLOBAL_H
# include <BALL/COMMON/global.h>
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

#include <BALL/VIEW/UIC/ui_modifyRepresentationDialog.h>

#include <QtWidgets/QSlider>
#include <QtWidgets/QLabel>

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
																 bool modal = false, Qt::WindowFlags fl = Qt::WindowFlags());

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
				
			public Q_SLOTS:
			
			void accept();
			void tabChanged();		
			void autoScale();
			void choosePressed();
			void gridSelected();
			void show();
			void applySplit();

			// Material-tab slots. The dialog hosts its own material controls
			// directly (999.67-02) instead of embedding the legacy
			// MaterialSettings child widget, which has been deleted.
			void materialAmbientFactorChanged_();
			void materialSpecularityFactorChanged_();
			void materialReflectivenessFactorChanged_();
			void materialShininessFactorChanged_();
			void materialTransparencyFactorChanged_();
			void editMaterialAmbientColor_();
			void editMaterialSpecularityColor_();
			void editMaterialReflectivenessColor_();

			protected Q_SLOTS:

			void customColorTransparencyChanged();
			void changeDrawingModeTransparencyChanged();
			void splitMethodChanged();

			protected:

			ModifyRepresentationDialog(const ModifyRepresentationDialog& dialog);

			typedef HashGrid3<const Atom*>  AtomGrid;
			typedef HashGridBox3<const Atom*> AtomBox;
			void colorByCustomColor_();
			bool colorByGrid_();
			bool insertGrid_(RegularData3D& grid, const String& name);
			void removeGrid_(RegularData3D& grid);
			void invalidateGrid_();
			void invalidateMesh_();
			void changeDrawingMode_();
			void checkApplyButton_();

			void calculateIncludedVertices_(vector<bool>& include_vertex, const Mesh& org_mesh, HashSet<const Composite*>& roots);
			inline bool checkInclude_(const AtomGrid& atom_grid, const Vector3& point) const;

			// Build the material-tab controls into the material_setting page and
			// wire their signals (replaces the embedded MaterialSettings widget).
			void setupMaterialControls_();
			// Apply the material-tab values to rep_'s material, reproducing the
			// legacy MaterialSettings::apply() Stage::Material setter sequence.
			void applyMaterial_();
			// Populate the material-tab controls from rep_'s current material,
			// reproducing MaterialSettings::setCurrentRepresentation().
			void updateMaterialControls_();
			// Slider-value -> label helper (mirrors MaterialSettings::setLabel_).
			void setMaterialLabel_(QLabel& label, float value);

			RegularData3D* grid_;
			float square_distance_;

			ColorRGBA	 	selected_color, min_min_color, min_color, mid_color, max_color, max_max_color;

			Representation* rep_;
			std::list<RegularData3D*> grid_list_;
			vector<Vector3> vertices_;

			// Material-tab controls (own widgets, hosted in the material_setting
			// page; replaces the deleted MaterialSettings child widget).
			QSlider* material_ambient_slider_;
			QSlider* material_specularity_slider_;
			QSlider* material_reflectiveness_slider_;
			QSlider* material_shininess_slider_;
			QSlider* material_transparency_slider_;
			QLabel* material_ambient_label_;
			QLabel* material_specularity_label_;
			QLabel* material_reflectiveness_label_;
			QLabel* material_shininess_label_;
			QLabel* material_transparency_label_;
			QLabel* material_ambient_color_label_;
			QLabel* material_specularity_color_label_;
			QLabel* material_reflectiveness_color_label_;
		};

	} // namespace VIEW
} // namespace BALL

#endif
