#ifndef BALL_VIEW_DIALOGS_EXPORTGEOMETRYDIALOG_H
#define BALL_VIEW_DIALOGS_EXPORTGEOMETRYDIALOG_H

#ifndef BALL_COMMON_GLOBAL_H
# include <BALL/COMMON/global.h>
#endif

#ifndef  BALL_VIEW_KERNEL_CLIPPING_PLANE_H
# include <BALL/VIEW/KERNEL/clippingPlane.h>
#endif

#include <BALL/VIEW/UIC/ui_exportGeometryDialog.h>

#include <QtWidgets/QDialog>

namespace BALL
{
	namespace VIEW
	{

		class BALL_VIEW_EXPORT ExportGeometryDialog
			: public QDialog,
				public Ui_ExportGeometryDialogData
		{
			Q_OBJECT

			public:

			//Constructor
			ExportGeometryDialog(QWidget *parent = NULL, const char *name = "ExportGeometryDialog" );

			//Destructor
			virtual ~ExportGeometryDialog();

			/**
			for transfer of continous filenumber from scene to dialog
			*/
			void setFilename(QString filename)	{filename_ = filename;}

			QString getFilename() { return filename_; }

			/** for information whether vrml export was chosen
			*/
			bool export_vrml();

			/** for information whether stl export was chosen
			*/
			bool export_stl();

			/** for information whether representations have to be split into several files
			*/
			bool split();

			/** for information which representations in scene have to be exported
			*/
//			bool* reps() {return reps;}

			/** for the information which reps have to be restored for the basestate
			*/
//			bool* basestats() {return basestats;}

			bool reps[100];

			bool basestats[100];

			public slots:

			/** Show and raise the dialog
			*/
			int exec();

			///
			void accept();

			//@}
			/** @name Protected slots
			*/
			//@{

			protected slots:

			/**	Opens the file decision dialog
			*/
			void browseFiles();

			/** If unprintable files should be not checkable
			*/
			void protomode();

			/** Normal mode without test on prototype-ability
			*/
			void viewmode();


			private:
			/**	Makes it possible to use the filename changed by the browseFiles() method 
			*/
			QString filename_;

			bool vrml_;

			bool stl_;

			bool split_;

		};

} } // namespaces

#endif // BALL_VIEW_DIALOGS_EXPORTGEOMETRYDIALOG_H
