// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id:

#ifndef BALL_VIEW_DIALOGS_CLIPPINGDIALOG_H
#define BALL_VIEW_DIALOGS_CLIPPINGDIALOG_H

#include <BALL/VIEW/UIC/clippingDialogData.h>

#ifndef  BALL_VIEW_KERNEL_CLIPPING_PLANE_H
# include <BALL/VIEW/KERNEL/clippingPlane.h>
#endif

#include <QtGui/QDialog>

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
			ClippingDialog(QWidget *parent = NULL, const char *name = "ClippingDialog" )
				throw();

			//@}
			/** @name Destructors 
			*/
			//@{

			/** Destructor.
			*/
			virtual ~ClippingDialog()
				throw();

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
			void exec();
			
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
