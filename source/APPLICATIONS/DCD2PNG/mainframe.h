// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#ifndef BALL_APPLICATIONS_DCD2PNG_MAINFRAME_H
#define BALL_APPLICATIONS_DCD2PNG_MAINFRAME_H

#ifndef BALL_VIEW_KERNEL_MAINCONTROL_H
# include <BALL/VIEW/KERNEL/mainControl.h>
#endif

#ifndef BALL_VIEW_DIALOGS_DISPLAYPROPERTIES_H
# include <BALL/VIEW/DIALOGS/displayProperties.h>
#endif

#ifndef BALL_VIEW_WIDGETS_DATASETCONTROL_H
# include <BALL/VIEW/WIDGETS/datasetControl.h>
#endif

#ifndef BALL_VIEW_WIDGETS_GEOMETRICCONTROL_H
# include <BALL/VIEW/WIDGETS/geometricControl.h>
#endif

#ifndef BALL_VIEW_WIDGETS_MOLECULARSTRUCTURE_H
# include <BALL/VIEW/WIDGETS/molecularStructure.h>
#endif

#ifndef BALL_VIEW_DIALOGS_MOLECULARFILEDIALOG_H
# include <BALL/VIEW/DIALOGS/molecularFileDialog.h>
#endif

#ifndef BALL_VIEW_WIDGETS_SCENE_H
# include <BALL/VIEW/WIDGETS/scene.h>
#endif

#ifndef BALL_VIEW_WIDGETS_MOLECULARCONTROL_H
# include <BALL/VIEW/WIDGETS/molecularControl.h>
#endif

#include <QtWidgets/QWidget>

namespace BALL
{
	using namespace BALL::VIEW;

	class Mainframe	
		: public BALL::VIEW::MainControl
	{
		Q_OBJECT

		public:

		BALL_EMBEDDABLE(Mainframe, MainControl)

		Mainframe(QWidget* parent = 0, const char* name = 0);

		virtual ~Mainframe() throw();

		protected:

		Scene*										scene_;
		MolecularControl*					control_;
		DatasetControl* 					dataset_control_;
		GeometricControl*					geometric_control_;
		DisplayProperties*    		display_properties_;
		MolecularStructure*  			molecular_structure_;
		MolecularFileDialog*  		file_dialog_;
	};

} // namespace BALL

#endif // BALL_APPLICATIONS_DCD2PNG_MAINFRAME_H
