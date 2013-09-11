#ifndef CAMERA_H
#define CAMERA_H

#include <GL/glew.h>

#include <Eigen/Geometry>
#include <iostream>
#include <QObject>
#include <BALL/VIEW/KERNEL/stage.h>


/**
 * @brief The viewport consits of width and height and lower left corner position (Initially (0,0)).
 **/
struct Viewport
{
	unsigned int x;
	unsigned int y;
	unsigned int width;
	unsigned int height;
};

/**
 * @brief Stores the clip range data.
 *
 * near: Distance from camera to near plane.
 * x_range: Distance from left to right clipping plane on the far plane (Wfar).
 * y_range: Distance from the top to the bottom clipping plane on the far plane (Hfar).
 * far: Distance fromv camera to far plane.
 **/
struct ClipRange
{
	float near;
	float x_range;
	float y_range;
	float far;
};

/**
 * @brief Enumerates the projection matrices.
 **/
enum ProjectionMatrixType
{
	PERSPECTIVE,
	ORTHOGRAPHIC
};

/**
 * @brief Manages and stores camera information.
 *
 * This class manages and stores the camera information for the renderer. It uses signals and slots
 * of Qt to be able to notify objects when data of the camera change. The objects need to register
 * by calling a register function (for the different data the camera stores, different functions are
 * available.).
 *
 * To make use of this class one can create a projection matrix with
 * createProjectionMatrix(ProjectionMatrixType matrix). By default this is a perspective projection
 * matrix. Additionally on can specify an view matrix by
 * setViewMatrix(Eigen::Affine3f view_matrix) and set FOV, far and near plane distance, and the
 * viewport.
 *
 * All the data can be querried with getters.
 **/
class Camera : public QObject
{
		Q_OBJECT

	public:
		EIGEN_MAKE_ALIGNED_OPERATOR_NEW

		/**
		 * @brief Default constructor.
		 *
		 * Initializes the camera.
		 *
		 **/
		Camera();

		/**
		 * @brief Copy constructor.
		 *
		 * @param camera The camera to copy.
		 **/
		Camera(const Camera& camera);


		/**
		 * @brief Destructor.
		 *
		 **/
		virtual ~Camera();

		/**
		 * @brief Assignment operator.
		 *
		 * @param camera The camera to assign.
		 * @return Camera& Returns this.
		 **/
		Camera& operator = (const Camera& camera);

		/**
		 * @brief Prints the current state of the camera to std::cout.
		 *
		 * @return void
		 **/
		void dump();

		/**
		 * @brief Resets all data.
		 *
		 * All data will be reset to default values.
		 *
		 * FOV: 45
		 * near dist: 2
		 * far dist: 150
		 * viewport: 800x600 (0,0)
		 * projection matrix: perspective
		 * viewmatrix: identity
		 *
		 * @return void
		 **/
		void init();


		/**
		 * @brief Creates the projection matrix.
		 *
		 * Creates the projection matrix depening on the matrix type to use. This can either bei
		 * PERSPECTIVE or ORTHOGRAPHIC.
		 *
		 * This method also creates the inverse and the transpose view matrix.
		 *
		 * @param matrix Matrixtype to contruct.
		 * @return void
		 **/
		void createProjectionMatrix(ProjectionMatrixType matrix);

		/**
		 * @brief Recreates the projection matrix.
		 *
		 * This should be done if viewport, near or far planes have changed.
		 *
		 * @return void
		 **/
		void recreateProjectionMatrix();

		void createViewMatrix(const BALL::VIEW::Camera* camera);

		/**
		 * @brief Returns the clip range.
		 *
		 * @return ClipRange
		 **/
		ClipRange getClipRange();

		/**
		 * @brief Returns the projection matrix.
		 *
		 * @return :Matrix4f& The projection matrix.
		 **/
		Eigen::Matrix4f& getProjectionMatrix();

		/**
		 * @brief Returns the inverse of the projection matrix.
		 *
		 * @return :Matrix4f& The inverse of the projection matrix.
		 **/
		Eigen::Matrix4f& getInverseProjectionMatrix();

		/**
		 * @brief Returns the transpose of the projection matrix.
		 *
		 * @return :Matrix4f& The transpose of the projection matrix.
		 **/
		Eigen::Matrix4f& getTransposeProjectionMatrix();

		/**
		 * @brief Sets the view matrix.
		 *
		 * This method also sets the inverse and the transpose view matrix.
		 *
		 * @param view_matrix The view matrix to be set.
		 * @return void
		 **/
		void setViewMatrix(Eigen::Affine3f view_matrix);

		/**
		 * @brief Returns the view matrix.
		 *
		 * @return :Affine3f& The view matrix to return.
		 **/
		Eigen::Affine3f& getViewMatrix();

		/**
		 * @brief Returns the inverse of the view matrix.
		 *
		 * @return :Affine3f& The inverse of the view matrix.
		 **/
		Eigen::Affine3f& getInverseViewMatrix();

		/**
		 * @brief Returns the transpose of the view matrix.
		 *
		 * @return :Matrix4f& The transpose of the view matrix.
		 **/
		Eigen::Matrix4f& getTransposeViewMatrix();

		/**
		 * @brief Returns the view projection matrix.
		 *
		 * @return :Matrix4f& The view projection matrix.
		 **/
		Eigen::Matrix4f& getViewProjectionMatrix();

		/**
		 * @brief Returns the transpose of the view projection matrix.
		 *
		 * @return :Matrix4f& The transpose of the view projection matrix.
		 **/
		Eigen::Matrix4f& getTransposeViewProjectionMatrix();

		/**
		 * @brief Sets the vertical field of view (fov) angle in degrees.
		 *
		 * Sets the vertical field of view (fov) angle in degrees. This is the angle between top plane of
		 * the view frustum and bottom plane.
		 *
		 * Note: the projection matrix will be rebuild.
		 *
		 * @param fov The field of view to be set.
		 * @return void
		 **/
		void setFOV(float fov);

		/**
		 * @brief Returns the vertical field of view angle in degrees.
		 *
		 * Returns the vertical field of view angle in degrees. This is the angle between top plane of the
		 * view frustum and bottom plane.
		 *
		 * @return float
		 **/
		float getFOV();

		/**
		 * @brief Sets the viewport.
		 *
		 * The viewport consists of a width and a height value.
		 * Note: the projection matrix will be rebuild.
		 *
		 * @param width The width of the viewport.
		 * @param height The height of the viewport.
		 * @param x X position of the lower left corner of the viewport rectangle in pixels.
		 * @param y Y position of the lower left corner of the viewport rectangle in pixels.

		 * @return void
		 **/
		void setViewport(unsigned int width, unsigned int height, unsigned int x=0, unsigned int y=0);

		/**
		 * @brief Sets the viewport.
		 *
		 * Note: the projection matrix will be rebuild (and the clipRange).
		 *
		 * @param viewport The viewport to be set.
		 * @return void
		 **/
		void setViewport(Viewport& viewport);

		/**
		 * @brief Returns the viewport.
		 *
		 * @return Viewport& The viewport.
		 **/
		Viewport& getViewport();

		/**
		 * @brief Sets the near distance of the viewport.
		 *
		 * The near distance is the distance between camera and near plane of the view frustum.
		 *
		 * Note: the projection matrix will be rebuild (and the clipRange).
		 *
		 * @param near The near distance.
		 * @return void
		 **/
		void setNearDistance(float near);

		/**
		 * @brief Returns the near distance.
		 *
		 * @return float The near distance.
		 **/
		float getNearDistance();

		/**
		 * @brief Sets the far distance of the viewport.
		 *
		 * The far distance is the distance between camera and far plane of the view frustum.
		 *
		 * Note: the projection matrix will be rebuild (and the clipRange).
		 *
		 * @param far The far disance.
		 * @return void
		 **/
		void setFarDistance(float far);

		/**
		 * @brief Returns the far distance of the viewport.
		 *
		 * @return float The far distance.
		 **/
		float getFarDistance();

		/**
		 * @brief Set the zoom level.
		 *
		 * @param zoom Zoom to be set.
		 * @return void
		 **/
		void setZoom(float zoom);

		float getZoom();

		/**
		 * @brief Emits all currently stored data to all slots.
		 *
		 * Use it only to be sure that slots receive data initially.
		 *
		 * @return void
		 **/
		void emitAll();

		void getFrustum(float& near_f, float& far_f, float& left_f, float& right_f, float& top_f, float& bottom_f);

		/**
		 * @brief Connects the Camera with the given <code>slot</code> object.
		 *
		 * Every time the view matrix is changed, setView of the receiver is called. The receiver
		 * needs to implement the function setView(Eigen::Affine3f).
		 *
		 * @param slot The slot to be notified.
		 * @return void
		 **/
		void registerViewMatrix(const QObject* slot);

		/**
		 * @brief Disconnects the Camera from the given <code>slot</code> object.
		 *
		 * The slot will no longer be notified on view matrix changes.
		 *
		 * @param slot The slot to disconnect.
		 * @return void
		 **/
		void unregisterViewMatrix(const QObject* slot);

		/**
		 * @brief Connects the Camera with the given <code>slot</code> object.
		 *
		 * Every time the inverse of the view matrix is changed, setViewInverse of the receiver is
		 * called. The receiver needs to implement the function setViewInverse(Eigen::Affine3f).
		 *
		 * @param slot The slot to be notified.
		 * @return void
		 **/
		void registerViewMatrixInverse(const QObject* slot);

		/**
		 * @brief Disconnects the Camera from the given <code>slot</code> object.
		 *
		 * The slot will no longer be notified on inverse view matrix changes.
		 *
		 * @param slot The slot to disconnect.
		 * @return void
		 **/
		void unregisterViewMatrixInverse(const QObject* slot);

		/**
		 * @brief Connects the Camera with the given <code>slot</code> object.
		 *
		 * Every time the transpose of the view matrix is changed, setViewTranspose of the receiver
		 * is called. The receiver needs to implement the function
		 * setViewMatrixTranspose(Eigen::Matrix4f).
		 *
		 * @param slot The slot to be notified.
		 * @return void
		 **/
		void registerViewMatrixTranspose(const QObject* slot);

		/**
		 * @brief Disconnects the Camera from the given <code>slot</code> object.
		 *
		 * The slot will no longer be notified on transpose view matrix changes.
		 *
		 * @param slot The slot to disconnect.
		 * @return void
		 **/
		void unregisterViewMatrixTranspose(const QObject* slot);

		/**
		 * @brief Connects the Camera with the given <code>slot</code> object.
		 *
		 * Every time the projection matrix is changed, setProjection of the receiver is called. The
		 * receiver needs to implement the function setProjection(Eigen::Matrix4f).
		 *
		 * @param slot The slot to be notified.
		 * @return void
		 **/
		void registerProjectionMatrix(const QObject* slot);

		/**
		 * @brief Disconnects the Camera from the given <code>slot</code> object.
		 *
		 * The slot will no longer be notified on projection matrix changes.
		 *
		 * @param slot The slot to disconnect.
		 * @return void
		 **/
		void unregisterProjectionMatrix(const QObject* slot);

		/**
		 * @brief Connects the Camera with the given <code>slot</code> object.
		 *
		 * Every time the inverse of the projection matrix is changed, setProjectionInverse of the
		 * receiver is called. The receiver needs to implement the function
		 * setProjectionInverse(Eigen::Matrix4f).
		 *
		 * @param slot The slot to be notified.
		 * @return void
		 **/
		void registerProjectionMatrixInverse(const QObject* slot);

		/**
		 * @brief Disconnects the Camera from the given <code>slot</code> object.
		 *
		 * The slot will no longer be notified on inverse projection matrix changes.
		 *
		 * @param slot The slot to disconnect.
		 * @return void
		 **/
		void unregisterProjectionMatrixInverse(const QObject* slot);

		/**
		 * @brief Connects the Camera with the given <code>slot</code> object.
		 *
		 * Every time the transpose of the projection matrix is changed, setProjectionMatrixTranspose of
		 * the receiver is called. The receiver needs to implement the function
		 * setProjectionMatrixTranspose(Eigen::Matrix4f).
		 *
		 * @param slot The slot to be notified.
		 * @return void
		 **/
		void registerProjectionMatrixTranspose(const QObject* slot);

		/**
		 * @brief Disconnects the Camera from the given <code>slot</code> object.
		 *
		 * The slot will no longer be notified on transpose projection matrix changes.
		 *
		 * @param slot The slot to disconnect.
		 * @return void
		 **/
		void unregisterProjectionMatrixTranspose(const QObject* slot);

		/**
		 * @brief Connects the Camera with the given <code>slot</code> object.
		 *
		 * Every time the view projection matrix is changed, setViewProjectionMatrix of
		 * the receiver is called. The receiver needs to implement the function
		 * setViewProjectionMatrix(Eigen::Matrix4f).
		 *
		 * @param slot The slot to be notified.
		 * @return void
		 **/
		void registerViewProjectionMatrix(const QObject* slot);

		/**
		 * @brief Disconnects the Camera from the given <code>slot</code> object.
		 *
		 * The slot will no longer be notified on view projection matrix changes.
		 *
		 * @param slot The slot to disconnect.
		 * @return void
		 **/
		void unregisterViewProjectionMatrix(const QObject* slot);

		/**
		 * @brief Connects the Camera with the given <code>slot</code> object.
		 *
		 * Every time the transpose of the view projection matrix is changed, setViewProjectionMatrixTranspose of
		 * the receiver is called. The receiver needs to implement the function
		 * setViewProjectionMatrixTranspose(Eigen::Matrix4f).
		 *
		 * @param slot The slot to be notified.
		 * @return void
		 **/
		void registerViewProjectionMatrixTranspose(const QObject* slot);

		/**
		 * @brief Disconnects the Camera from the given <code>slot</code> object.
		 *
		 * The slot will no longer be notified on transpose view projection matrix changes.
		 *
		 * @param slot The slot to disconnect.
		 * @return void
		 **/
		void unregisterViewProjectionMatrixTranspose(const QObject* slot);

		/**
		 * @brief Connects the Camera with the given <code>slot</code> object.
		 *
		 *
		 * Every time the field of view is changed, setFOV of the receiver is called. The
		 * receiver needs to implement the function setFOV(float).
		 *
		 * @param slot The slot to be notified.
		 * @return void
		 **/
		void registerFOV(const QObject* slot);

		/**
		 * @brief Disconnects the Camera from the given <code>slot</code> object.
		 *
		 * The slot will no longer be notified on field of view changes.
		 *
		 * @param slot The slot to disconnect.
		 * @return void
		 **/
		void unregisterFOV(const QObject* slot);

		/**
		 * @brief Connects the Camera with the given <code>slot</code> object.
		 *
		 *
		 * Every time the viewport is changed, setViewport of the receiver is called. The
		 * receiver needs to implement the function setViewport(int, int, int, int).
		 *
		 * @param slot The slot to be notified.
		 * @return void
		 **/
		void registerViewport(const QObject* slot);

		/**
		 * @brief Disconnects the Camera from the given <code>slot</code> object.
		 *
		 * The slot will no longer be notified on viewport changes.
		 *
		 * @param slot The slot to disconnect.
		 * @return void
		 **/
		void unregisterViewport(const QObject* slot);

		/**
		 * @brief Connects the Camera with the given <code>slot</code> object.
		 *
		 * Every time the frustum is changed, setFrustum of the receiver is called. The
		 * receiver needs to implement the function setFrustum(float, float, float, float).
		 * Only left, right, top, bottom frustum coordinates are registered. There are special register
		 * functions for near and far distance.
		 *
		 * @see registerNearDistance
		 * @see registerFarDistance
		 * @see registerClipRange
		 *
		 * @param slot The slot to be notified.
		 * @return void
		 **/
		void registerFrustum(const QObject* slot);

		/**
		 * @brief Disconnects the Camera from the given <code>slot</code> object.
		 *
		 * The slot will no longer be notified on frustum changes.
		 *
		 * @param slot The slot to disconnect.
		 * @return void
		 **/
		void unregisterFrustum(const QObject* slot);

		/**
		 * @brief Connects the Camera with the given <code>slot</code> object.
		 *
		 *
		 * Every time the near distance is changed, setNearDistance of the receiver is called. The
		 * receiver needs to implement the function setNearDistance(float).
		 *
		 * @param slot The slot to be notified.
		 * @return void
		 **/
		void registerNearDistance(const QObject* slot);

		/**
		 * @brief Disconnects the Camera from the given <code>slot</code> object.
		 *
		 * The slot will no longer be notified on near distance changes.
		 *
		 * @param slot The slot to disconnect.
		 * @return void
		 **/
		void unregisterNearDistance(const QObject* slot);

		/**
		 * @brief Connects the Camera with the given <code>slot</code> object.
		 *
		 *
		 * Every time the far distance is changed, setFarDistance of the receiver is called. The
		 * receiver needs to implement the function setFarDistance(float).
		 *
		 * @param slot The slot to be notified.
		 * @return void
		 **/
		void registerFarDistance(const QObject* slot);

		/**
		 * @brief Disconnects the Camera from the given <code>slot</code> object.
		 *
		 * The slot will no longer be notified on far distance changes.
		 *
		 * @param slot The slot to disconnect.
		 * @return void
		 **/
		void unregisterFarDistance(const QObject* slot);

		/**
		 * @brief Connects the Camera with the given <code>slot</code> object.
		 *
		 *
		 * Every time the clip range is changed, setClipRange of the receiver is called. The
		 * receiver needs to implement the function setClipRange(ClipRange).
		 *
		 * @param slot The slot to be notified.
		 * @return void
		 **/
		void registerClipRange(const QObject* slot);

		/**
		 * @brief Disconnects the Camera from the given <code>slot</code> object.
		 *
		 * The slot will no longer be notified on clip range changes.
		 *
		 * @param slot The slot to disconnect.
		 * @return void
		 **/
		void unregisterClipRange(const QObject* slot);

	signals:

		/**
		 * @brief Signal for changed view matrix.
		 *
		 * @param view_matrix The changed view matrix.
		 * @return void
		 **/
		void viewMatrixChanged(const Eigen::Affine3f& view_matrix);

		/**
		 * @brief Signal for changed inverse view matrix.
		 *
		 * @param view_matrix_inv The changed inverse view matrix.
		 * @return void
		 **/
		void viewMatrixInverseChanged(const Eigen::Affine3f& view_matrix_inv);

		/**
		 * @brief Signal for changed transpose view matrix.
		 *
		 * @param view_matrix_transp The changed transpose view matrix.
		 * @return void
		 **/
		void viewMatrixTransposeChanged(const Eigen::Matrix4f& view_matrix_transp);

		/**
		 * @brief Signal for changed projection matrix.
		 *
		 * @param projection_matrix The changed projection matrix.
		 * @return void
		 **/
		void projectionMatrixChanged(const Eigen::Matrix4f& projection_matrix);

		/**
		 * @brief Signal for changed inverse projection matrix.
		 *
		 * @param projection_matrix The changed inverse projection matrix.
		 * @return void
		 **/
		void projectionMatrixInverseChanged(const Eigen::Matrix4f& projection_matrix_inv);

		/**
		 * @brief Signal for changed transpose projection matrix.
		 *
		 * @param projection_matrix The changed transpose projection matrix.
		 * @return void
		 **/
		void projectionMatrixTransposeChanged(const Eigen::Matrix4f& projection_matrix_transp);

		/**
		 * @brief Signal for changed view projection matrix.
		 *
		 * @param projection_matrix The changed view projection matrix.
		 * @return void
		 **/
		void viewProjectionMatrixChanged(const Eigen::Matrix4f& view_projection_matrix);

		/**
		 * @brief Signal for changed transpose view projection matrix.
		 *
		 * @param projection_matrix The changed transpose view projection matrix.
		 * @return void
		 **/
		void viewProjectionMatrixTransposeChanged(const Eigen::Matrix4f& view_projection_matrix_transp);

		/**
		 * @brief Signal for changed field of view (fov).
		 *
		 * @param fov The changed fov.
		 * @return void
		 **/
		void FOVChanged(float fov);

		/**
		 * @brief Signal for changed viewport.
		 *
		 * @param width Width of the changed viewport.
		 * @param height Height of the changed viewport.
		 * @param x X position of the lower left corner position of the viewport rectangle.
		 * @param y Y position of the lower left corner position of the viewport rectangle.
		 * @return void
		 **/
		void viewportChanged(unsigned int width, unsigned int height, unsigned int x=0, unsigned int y=0);

		/**
		 * @brief Signal for changed frustum.
		 *
		 * @param left Coordinate for the left vertical clipping plane.
		 * @param right Coordinate for the right vertical clipping plane.
		 * @param bottom Coordinate for the bottom horizontal clipping plane.
		 * @param top Coordinate for the top horizontal clipping plane.
		 * @return void
		 **/
		void frustumChanged(float left, float right, float bottom, float top);

		/**
		 * @brief Signal for changed near distance.
		 *
		 * @param near The changed near distance.
		 * @return void
		 **/
		void nearDistanceChanged(float near);

		/**
		 * @brief Signal for changed far distance.
		 *
		 * @param far The changed far distance.
		 * @return void
		 **/
		void farDistanceChanged(float far);

		/**
		 * @brief Signal for changed clip range.
		 *
		 * @param clipRange The changed clip range.
		 * @return void
		 **/
		void clipRangeChanged(ClipRange clipRange);


	private:

		/**
		 * @brief Calculates the clip range.
		 * @return The calculated clip range.
		 *
		 **/
		ClipRange calculateClipRange_();

		/**
		 * @brief Sets the clip range.
		 *
		 * @param clipRange The cliprange to be set.
		 * @return void
		 **/
		void setClipRange_(ClipRange clipRange);

		/**
		 * @brief Creates the orthographic projection matrix.
		 *
		 * @return :Matrix4f The created orthographic projection matrix.
		 **/
		Eigen::Matrix4f createOrthographicMatrix_();

		/**
		 * @brief Creates the perspective projection matrix.
		 *
		 * @return :Matrix4f The created perspective projection matrix.
		 **/
		Eigen::Matrix4f createPerspectiveMatrix_();

		/**
		 * @brief Set the projection matrix.
		 *
		 * Note: this also recreates the clipRange.
		 *
		 * @param projection_matrix The projection matrix to set.
		 * @return void
		 **/
		void setProjectionMatrix_(Eigen::Matrix4f projection_matrix);

		/**
		 * @brief The projection matrix.
		 *
		 * The projection matrix transforms data to normalized device space (projects points to a two
		 * dimensional plane). The projection matrix could for example be a orthogonal (parallel projection)
		 * where parallel lines do not converge at infinity or a perspective projection which mimics the
		 * real-life visual effect of forshortening.
		 **/
		Eigen::Matrix4f projection_matrix_;

		/**
		 * @brief The inverse projection matrix.
		 *
		 * The inverse projection matrix is always be updated when the projection_matrix_ is updated.
		 **/
		Eigen::Matrix4f projection_matrix_inv_;

		/**
		 * @brief The transpose projection matrix.
		 *
		 * The transpose projection matrix is always be updated when the projection_matrix_ is updated.
		 **/
		Eigen::Matrix4f projection_matrix_transp_;

		/**
		 * @brief The view matrix.
		 *
		 * The view matrix is used to position the world relative to the viewers position / camera
		 * position. The created space is called eye space.
		 **/
		Eigen::Affine3f view_matrix_;

		/**
		 * @brief The inverse view matrix.
		 *
		 * The inverse view matrix is always be updated when the view_matrix_ is updated.
		 **/
		Eigen::Affine3f view_matrix_inv_;

		/**
		 * @brief The transpose view matrix.
		 *
		 * The transpose view matrix is always be updated when the view_matrix_ is updated.
		 **/
		Eigen::Matrix4f view_matrix_transp_;

		/**
		 * @brief The view projection matrix.
		 *
		 * The view projection matrix is always be updated when the view_matrix_ or
		 * projection_matrix_ is updated.
		 **/
		Eigen::Matrix4f view_projection_matrix_;

		/**
		 * @brief The transpose view projection matrix.
		 *
		 * The transpose view projection matrix is always be updated when the view_matrix_ or
		 * projection_matrix_ is updated.
		 **/
		Eigen::Matrix4f view_projection_matrix_transp_;

		/**
		 * @brief Field of view.
		 *
		 * Field of view angle in y direction, in degrees.
		 **/
		float fovy_;

		/**
		 * @brief Near distance.
		 *
		 * Distance from the viewer to the near clipping plane.
		 **/
		float near_;

		/**
		 * @brief Far distance.
		 *
		 * Distance from the viewer to the far clipping plane.
		 **/
		float far_;

		/**
		 * @brief Stores the width and height of view.
		 **/
		Viewport viewport_;

		/**
		 * @brief Stores which projection matrix is active.
		 **/
		ProjectionMatrixType activeProjectionMatrix_;

		/**
		 * @brief Stores the clip range.
		 **/
		ClipRange clipRange_;

		/**
		 * @brief Zoom.
		 **/
		float zoom_;

		/**
		 * @brief Maximal distance from left to right plane.
		 *
		 * The maximal distance is on the far plane.
		 **/
		float x_scale_;

		/**
		 * @brief Maximal distance from top to bottom plane.
		 *
		 * The maximal distance is on the far plane.
		 **/
		float y_scale_;

};

#endif //CAMERA_H