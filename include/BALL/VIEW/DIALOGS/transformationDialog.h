

#include <BALL/VIEW/UIC/transformationDialogData.h>

#ifndef BALL_VIEW_KERNEL_MODULARWIDGET_H
#include <BALL/VIEW/KERNEL/modularWidget.h>
#endif

#ifndef BALL_STRUCTURE_GEOMETRICTRANSFORMATIONS_H
#include <BALL/STRUCTURE/geometricTransformations.h>
#endif

#ifndef BALL_STRUCTURE_GEOMETRICPROPERIES_H
#include <BALL/STRUCTURE/geometricProperties.h>
#endif

namespace BALL
{
	namespace VIEW
	{

		/** Dialog for moving molecular objects.
				\ingroup ViewDialogs
		*/
		class BALL_EXPORT TransformationDialog 
			:	public TransformationDialogData,
				public ModularWidget
		{
			Q_OBJECT

			public:	

			BALL_EMBEDDABLE(TransformationDialog, ModularWidget)

			 
			/** A Constructor */
			TransformationDialog(QWidget* parent = 0, const char* name = 0, bool modal = FALSE, WFlags fl = 0 ) ;

			/** Destructor */
			~TransformationDialog() throw();
			
			/** This function sets the  Composite */
			void  setComposite (Composite* composite);
			
			///This function gets the Composite
			Composite* getComposite() { return (composite_);}
			
			/** This function is used to translate a molecule in 3D dimensional space */
			bool translate(float x, float y, float z);

			/** This function rotates a molecule around the x-axis */
			bool rotateX(float angle, bool radian);

			/** This function rotates a molecule around the y-axis */
			bool rotateY(float angle, bool radian);

			/** This function rotates a molecule around the z-axis */
			bool rotateZ(float angle, bool radian);


		public slots:

			///This function moves the selected molecule in the analogous direction
			virtual void zNegRotationClicked();
			
			///This function moves the selected molecule in the analogous direction
			virtual void xNegRotationClicked();
			
			///This function moves the selected molecule in the analogous direction
			virtual void xNegTranslationClicked();
			
			///This function moves the selected molecule in the analogous direction
			virtual void xRotationClicked();
			
			///This function moves the selected molecule in the analogous direction
			virtual void xTranslationClicked();
			
			///This function moves the selected molecule in the analogous direction
			virtual void yNegRotationClicked();
			
			///This function moves the selected molecule in the analogous direction
			virtual void yNegTranslationClicked();
			
			///This function moves the selected molecule in the analogous direction
			virtual void yRotationClicked();
			
			///This function moves the selected molecule in the analogous direction
			virtual void yTranslationClicked();
			
			///This function moves the selected molecule in the analogous direction
			virtual void zNegTranslationClicked();
			
			///This function moves the selected molecule in the analogous direction
			virtual void zRotationClicked();
			
			///This function moves the selected molecule in the analogous direction
			virtual void zTranslationClicked();
			
			/** This function catches the MaxRotation in the dialogfield */
			float getMaxRotation() const;

			/** This function catches the MaxTrans in the dialogfield */
			float getMaxTrans() const;
			
			protected:

			virtual void onNotify(Message *message)
				throw();

			void update_();
			
			/** Its a pointer of a selected ligand */
			Composite* composite_;
			
			/** Its a pointer of the TransformationProcessor */
			TransformationProcessor my_processor_;
			
			/** Its a pointer of the GeometricCenterProcessor */
			GeometricCenterProcessor center_processor_;
			
			/** Initialization of a Vector3 for calculating the molecule's balance point */ 
			Vector3 composite_center_;

			/** Initialization of a Matrix4x4 for the transformation of the molecule */
			Matrix4x4 matrix_;
			
			/** Initialization of a Angle for the rotation */
			Angle angle_;
		};

	} // namespace VIEW

} // namespace BALL
