// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#ifndef BALL_VIEW_DIALOGS_CLIPPINGDIALOG_H
#define BALL_VIEW_DIALOGS_CLIPPINGDIALOG_H

#ifndef BALL_COMMON_GLOBAL_H
# include <BALL/COMMON/global.h>
#endif

#ifndef  BALL_VIEW_KERNEL_CLIPPING_PLANE_H
# include <BALL/VIEW/KERNEL/clippingPlane.h>
#endif

#include <BALL/VIEW/UIC/ui_clippingDialog.h>

#include <QtWidgets/QDialog>

namespace BALL
{
	namespace VIEW
	{
		/**	Dialog for selcting which Representations are clipped by one clipping plane.
    		\ingroup  ViewDialogs
		*/
		class BALL_VIEW_EXPORT ClippingDialog 
			: public QDialog,
				public Ui_ClippingDialogData
		{
			Q_OBJECT
				
			public:
			
			/**	@name	Constructors
			*/	
			//@{

			/** Default Constructor.
			*/
			ClippingDialog(QWidget *parent = NULL, const char *name = "ClippingDialog" );

			//@}
			/** @name Destructors 
			*/
			//@{

			/** Destructor.
			*/
			virtual ~ClippingDialog();

			///
			void setClippingPlane(ClippingPlane* plane) { clipping_plane_ = plane;}

			///
			const ClippingPlane* getClippingPlane() const { return clipping_plane_;}
			
			//@}
			
			public slots:
				
  		/** @name Public slots
	  	*/
			//@{
				
			/** Show and raise the dialog
			*/
			int exec();
			
			///
			void accept();

			//@}
			/** @name Protected slots
			*/
			//@{
				
			protected:

			ClippingPlane*  clipping_plane_;
		};

} } // namespaces

#endif // BALL_VIEW_DIALOGS_CLIPPINGDIALOG_H
