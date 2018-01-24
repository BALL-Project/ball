#include <GL/glew.h>

#include <Camera.h>
#include <Global.h>


Camera::Camera()
{
	init();
}

Camera::Camera ( const Camera& camera )
	: QObject()
{
	if (this != &camera) // protect against invalid self-assignment
	{
		setProjectionMatrix_(camera.projection_matrix_);
		setViewMatrix(camera.view_matrix_);
		setFOV(camera.fovy_);
		setNearDistance(camera.near_);
		setFarDistance(camera.far_);
		setViewport(camera.viewport_.width, camera.viewport_.height, camera.viewport_.x, camera.viewport_.y);
		activeProjectionMatrix_ = camera.activeProjectionMatrix_;
		setClipRange_(camera.clipRange_);
	}
}

Camera::~Camera()
{
	//nothing to do
}

Camera& Camera::operator= ( const Camera& camera )
{
	if (this != &camera) // protect against invalid self-assignment
	{
		setProjectionMatrix_(camera.projection_matrix_);
		setViewMatrix(camera.view_matrix_);
		setFOV(camera.fovy_);
		setNearDistance(camera.near_);
		setFarDistance(camera.far_);
		setViewport(camera.viewport_.width, camera.viewport_.height, camera.viewport_.x, camera.viewport_.y);
		activeProjectionMatrix_ = camera.activeProjectionMatrix_;
		setClipRange_(camera.clipRange_);
	}

	// by convention, always return *this
	return *this;
}

void Camera::dump()
{
	std::cout << "[Camera]: dump:" << std::endl;
	std::cout << "\t fov: " << fovy_ << std::endl;
	std::cout << "\t near plane distance: " << near_ << std::endl;
	std::cout << "\t far plane distance: " << far_ << std::endl;
	std::cout << "\t zoom: " << zoom_ << std::endl;
	std::cout << "\t viewport (w,h,x,y): (" << viewport_.width << ", " << viewport_.height << "," << viewport_.x << ", " << viewport_.y << ")" << std::endl;
	std::cout << "\t clip range: (" << clipRange_.near << ", " << clipRange_.x_range << ", " << clipRange_.y_range << ", " << clipRange_.far << ")" << std::endl;
	std::cout << "\t active projection matrix: ";

	switch(activeProjectionMatrix_)
	{
		case PERSPECTIVE:
			std::cout << "Perspective";
			break;
		case ORTHOGRAPHIC:
			std::cout << "Orthographic";
			break;
		default:
			std::cout << "NONE";
	}

	std::cout << std::endl;
	std::cout << "\t projection matrix:\n";
	std::cout << projection_matrix_.matrix() << std::endl;
	std::cout << "\t projection matrix inverse:\n";
	std::cout << projection_matrix_inv_.matrix() << std::endl;
	std::cout << "\t projection matrix transpose:\n";
	std::cout << projection_matrix_transp_.matrix() << std::endl;

	std::cout << "\t view matrix:\n";
	std::cout << view_matrix_.matrix() << std::endl;
	std::cout << "\t view matrix inverse:\n";
	std::cout << view_matrix_inv_.matrix() << std::endl;
	std::cout << "\t view matrix transpose:\n";
	std::cout << view_matrix_transp_.matrix() << std::endl;
}

void Camera::init()
{
	setFOV(45);
	setNearDistance(1.5);
	setFarDistance(2000);
	setViewport(800, 600, 0, 0);

	activeProjectionMatrix_ = PERSPECTIVE;
	createProjectionMatrix(activeProjectionMatrix_);
	setViewMatrix(Eigen::Affine3f::Identity());
	setClipRange_(calculateClipRange_());
}

void Camera::createProjectionMatrix ( ProjectionMatrixType matrix )
{
	switch (matrix)
	{

		case ORTHOGRAPHIC:
			setProjectionMatrix_(createOrthographicMatrix_());
			break;
		case PERSPECTIVE:
			setProjectionMatrix_(createPerspectiveMatrix_());
			break;
		default:
			setProjectionMatrix_(createPerspectiveMatrix_());
			break;
	}

	activeProjectionMatrix_ = matrix;
}

void Camera::recreateProjectionMatrix ()
{
	switch (activeProjectionMatrix_)
	{

		case ORTHOGRAPHIC:
			setProjectionMatrix_(createOrthographicMatrix_());
			break;
		case PERSPECTIVE:
			setProjectionMatrix_(createPerspectiveMatrix_());
			break;
		default:
			setProjectionMatrix_(createPerspectiveMatrix_());
			break;
	}
}

void Camera::createViewMatrix (const BALL::VIEW::Camera* camera)
{
	BALL::Vector3 eye = camera->getViewPoint() * Global::getScaleFactor();
	BALL::Vector3 target = camera->getLookAtPosition() * Global::getScaleFactor();
	BALL::Vector3 up = camera->getLookUpVector();


	Eigen::Affine3f view_matrix(Eigen::Affine3f::Identity());
	Eigen::Matrix4f m(Eigen::Matrix4f::Identity());

	BALL::Vector3 f = target - eye;
	f.normalize();
	up.normalize();
	BALL::Vector3 s = f % up;
	s.normalize();
	BALL::Vector3 u = s % f;

	m(0,0) = s.x;
	m(1,0) = u.x;
	m(2,0) = -f.x;
	m(3,0) = 0;

	m(0,1) = s.y;
	m(1,1) = u.y;
	m(2,1) = -f.y;
	m(3,1) = 0;

	m(0,2) = s.z;
	m(1,2) = u.z;
	m(2,2) = -f.z;
	m(3,2) = 0;

	view_matrix = m;
	view_matrix.translate(Eigen::Vector3f(-eye.x, -eye.y, -eye.z));

	setViewMatrix(view_matrix);
}

ClipRange Camera::getClipRange()
{
	return clipRange_;
}

Eigen::Matrix4f& Camera::getProjectionMatrix()
{
	return projection_matrix_;
}

Eigen::Matrix4f& Camera::getInverseProjectionMatrix()
{
	return projection_matrix_inv_;
}

Eigen::Matrix4f& Camera::getTransposeProjectionMatrix()
{
	return projection_matrix_transp_;
}

Eigen::Matrix4f& Camera::getViewProjectionMatrix()
{
	return view_projection_matrix_;
}

Eigen::Matrix4f& Camera::getTransposeViewProjectionMatrix()
{
	return view_projection_matrix_transp_;
}

void Camera::setViewMatrix(Eigen::Affine3f view_matrix)
{
	if(view_matrix_.matrix() != view_matrix.matrix())
	{
		view_matrix_ = view_matrix;
		view_matrix_inv_ = view_matrix_.inverse();

		//transpose is not available for affine matrices
		Eigen::Matrix4f mat(view_matrix_.data());
		view_matrix_transp_ = mat.transpose();

		view_projection_matrix_ = Eigen::Matrix4f(projection_matrix_.matrix() * view_matrix_.matrix());
		view_projection_matrix_transp_ = view_projection_matrix_.transpose();

		emit viewMatrixChanged(view_matrix);
		emit viewMatrixInverseChanged(view_matrix_inv_);
		emit viewMatrixTransposeChanged(view_matrix_transp_);
		emit viewProjectionMatrixChanged(view_projection_matrix_);
		emit viewProjectionMatrixTransposeChanged(view_projection_matrix_transp_);
	}
}

Eigen::Affine3f& Camera::getViewMatrix()
{
	return view_matrix_;
}

Eigen::Affine3f& Camera::getInverseViewMatrix()
{
	return view_matrix_inv_;
}

Eigen::Matrix4f& Camera::getTransposeViewMatrix()
{
	return view_matrix_transp_;
}

void Camera::setFOV ( float fov )
{
	if(fovy_ != fov)
	{
		fovy_ = fov;
		emit FOVChanged(fov);
		recreateProjectionMatrix();
	}
}

float Camera::getFOV()
{
	return fovy_;
}


void Camera::setViewport (unsigned int width, unsigned int height, unsigned int x, unsigned int y)
{
	if(viewport_.x != x || viewport_.y != y || viewport_.width != width || viewport_.height != height)
	{
		glViewport(x, y, width, height);
		viewport_.x = x;
		viewport_.y = y;
		viewport_.height = height;
		viewport_.width = width;

		if (width > height)
		{
			x_scale_ = (float) width / (float) height;
			y_scale_ = 1;
		}
		else
		{
			x_scale_ = 1;
			y_scale_ = (float) height / (float) width;
		}

		emit viewportChanged(width, height, x, y);
		emit frustumChanged(-x_scale_, x_scale_, -y_scale_, y_scale_);
		recreateProjectionMatrix();
	}
}

void Camera::setViewport ( Viewport& viewport )
{
	setViewport(viewport_.width, viewport.height, viewport_.x, viewport_.y);
}

Viewport& Camera::getViewport()
{
	return viewport_;
}

void Camera::setNearDistance ( float near )
{
	if(near != near_)
	{
		near_ = near;
		emit nearDistanceChanged(near);
		recreateProjectionMatrix();
	}
}

float Camera::getNearDistance()
{
	return near_;
}

void Camera::setFarDistance ( float far )
{
	if(far != far_)
	{
		far_ = far;
		emit farDistanceChanged(far);
		recreateProjectionMatrix();
	}
}

float Camera::getFarDistance()
{
	return far_;
}

void Camera::setZoom ( float zoom )
{
	zoom_ = zoom;
}

/**
 * @brief Return the current zoom level.
 *
 * @return float
 **/
float Camera::getZoom()
{
	return zoom_;
}


void Camera::setClipRange_ ( ClipRange clipRange )
{
	clipRange_ = clipRange;
	emit clipRangeChanged(clipRange_);
}

ClipRange Camera::calculateClipRange_()
{
	float y_range = tan(fovy_/2 * BALL::Constants::PI/180) * far_ * 2;

	const float aspect = (float) viewport_.width/ (float)viewport_.height;
	float x_range = y_range * aspect;

	ClipRange clipRange;
	clipRange.near = near_;
	clipRange.x_range = x_range;
	clipRange.y_range = y_range;
	clipRange.far = far_;
	return clipRange;

}

void Camera::setProjectionMatrix_ ( Eigen::Matrix4f projection_matrix )
{
	if(projection_matrix.matrix() != projection_matrix_.matrix())
	{
		projection_matrix_ = projection_matrix;
		projection_matrix_inv_ = projection_matrix_.inverse();
		projection_matrix_transp_=projection_matrix_.transpose();

		view_projection_matrix_ = Eigen::Matrix4f(projection_matrix_.matrix() * view_matrix_.matrix());
		view_projection_matrix_transp_ = view_projection_matrix_.transpose();

		emit projectionMatrixChanged(projection_matrix_);
		emit projectionMatrixInverseChanged(projection_matrix_inv_);
		emit projectionMatrixTransposeChanged(projection_matrix_transp_);
		emit viewProjectionMatrixChanged(view_projection_matrix_);
		emit viewProjectionMatrixTransposeChanged(view_projection_matrix_transp_);
		setClipRange_(calculateClipRange_());
	}
}

Eigen::Matrix4f Camera::createOrthographicMatrix_()
{
	Eigen::Matrix4f projection(Eigen::Matrix4f::Zero());

	int w = viewport_.width;
	int h = viewport_.height;

	int side = std::min(w, h);

	const float left    = -(float)w /(float)side * -zoom_*0.4;
	const float right   =  (float)w /(float)side * -zoom_*0.4;
	const float bottom  = -(float)h/(float)side * -zoom_*0.4;
	const float top     =  (float)h/(float)side * -zoom_*0.4;
	const float near = near_;
	const float far  = far_;

	projection(0,0) =  2.0f / (right - left);
	projection(1,1) =  2.0f / (top - bottom);
	projection(2,2) = -2.0f / (far - near);
	projection(3,3) = 1.0f;
	projection(0,3) = -(right + left) / (right - left);
	projection(1,3) = -(top + bottom) / (top - bottom);
	projection(2,3) = -(far + near) / (far - near);

	return projection;
}


Eigen::Matrix4f Camera::createPerspectiveMatrix_()
{
	Eigen::Matrix4f projection(Eigen::Matrix4f::Zero());

//  const float aspect = (float) viewport_.width/ (float)viewport_.height;   //aspect ratio that determines the field of view in the x direction.
//  const float f = 1 / tan(fovy_/2  * BALL::Constants::PI/180);
//
//  //Fill the diagonal entries
//  projection(0,0) = f/aspect;
//  projection(1,1) = f;
//  projection(2,2) = (far_ + near_) / (near_ - far_);
//
//  //Fill the off diagonals
//  projection(3,2) = -1.0f;
//  projection(2,3) = 2*(far_ * near_) / (near_ - far_);
//
//  return projection;


	const float left   = -x_scale_;
	const float right  =  x_scale_;
	const float bottom = -y_scale_;
	const float top    =  y_scale_;
	const float near = near_;
	const float far  = far_;

	//Fill the diagonal entries
	projection(0,0) = 2*near / (right - left);
	projection(1,1) = 2*near / (top - bottom);
	projection(2,2) = -(far + near) / (far - near);

	//Fill the off diagonals
	projection(3,2) = -1.0f;
	projection(2,3) = -2*(far * near_) / (far - near);

	return projection;
}



void Camera::emitAll()
{
	emit viewMatrixChanged(view_matrix_);
	emit viewMatrixInverseChanged(view_matrix_inv_);
	emit viewMatrixTransposeChanged(view_matrix_transp_);
	emit projectionMatrixChanged(projection_matrix_);
	emit projectionMatrixInverseChanged(projection_matrix_inv_);
	emit projectionMatrixTransposeChanged(projection_matrix_transp_);
	emit viewProjectionMatrixChanged(view_projection_matrix_);
	emit viewProjectionMatrixTransposeChanged(view_projection_matrix_transp_);
	emit FOVChanged(fovy_);
	emit viewportChanged(viewport_.width, viewport_.height, viewport_.x, viewport_.y);
	emit nearDistanceChanged(near_);
	emit farDistanceChanged(far_);
	emit clipRangeChanged(clipRange_);
	emit frustumChanged(-x_scale_, x_scale_, -y_scale_, y_scale_);
}

void Camera::getFrustum ( float& near_f, float& far_f, float& left_f, float& right_f, float& top_f, float& bottom_f )
{
	left_f   = -x_scale_;
	right_f  =  x_scale_;
	bottom_f = -y_scale_;
	top_f    =  y_scale_;
	near_f   = near_;
	far_f    = far_;
}

void Camera::registerViewMatrix ( const QObject* slot )
{
	connect(this, SIGNAL(viewMatrixChanged(Eigen::Affine3f)),
	        slot, SLOT(setView(Eigen::Affine3f)));
}

void Camera::unregisterViewMatrix ( const QObject* slot )
{
	disconnect(this, SIGNAL(viewMatrixChanged(Eigen::Affine3f)),
	           slot, SLOT(setView(Eigen::Affine3f)));
}

void Camera::registerViewMatrixInverse ( const QObject* slot )
{
	connect(this, SIGNAL(viewMatrixInverseChanged(Eigen::Affine3f)),
	        slot, SLOT(setViewInverse(Eigen::Affine3f)));
}

void Camera::unregisterViewMatrixInverse ( const QObject* slot )
{
	disconnect(this, SIGNAL(viewMatrixInverseChanged(Eigen::Affine3f)),
	           slot, SLOT(setViewInverse(Eigen::Affine3f)));
}

void Camera::registerViewMatrixTranspose ( const QObject* slot )
{
	connect(this, SIGNAL(viewMatrixTransposeChanged(Eigen::Matrix4f)),
	        slot, SLOT(setViewTranspose(Eigen::Matrix4f)));
}

void Camera::unregisterViewMatrixTranspose ( const QObject* slot )
{
	disconnect(this, SIGNAL(viewMatrixTransposeChanged(Eigen::Matrix4f)),
	           slot, SLOT(setViewTranspose(Eigen::Matrix4f)));
}

void Camera::registerProjectionMatrix ( const QObject* slot )
{
	connect(this, SIGNAL(projectionMatrixChanged(Eigen::Matrix4f)),
	        slot, SLOT(setProjection(Eigen::Matrix4f)));
}

void Camera::unregisterProjectionMatrix ( const QObject* slot )
{
	disconnect(this, SIGNAL(projectionMatrixChanged(Eigen::Matrix4f)),
	           slot, SLOT(setProjection(Eigen::Matrix4f)));
}

void Camera::registerProjectionMatrixInverse ( const QObject* slot )
{
	connect(this, SIGNAL(projectionMatrixInverseChanged(Eigen::Matrix4f)),
	        slot, SLOT(setProjectionInverse(Eigen::Matrix4f)));
}

void Camera::unregisterProjectionMatrixInverse ( const QObject* slot )
{
	disconnect(this, SIGNAL(projectionMatrixInverseChanged(Eigen::Matrix4f)),
	           slot, SLOT(setProjectionInverse(Eigen::Matrix4f)));
}

void Camera::registerProjectionMatrixTranspose ( const QObject* slot )
{
	connect(this, SIGNAL(projectionMatrixTransposeChanged(Eigen::Matrix4f)),
	        slot, SLOT(setProjectionTranspose(Eigen::Matrix4f)));
}

void Camera::unregisterProjectionMatrixTranspose ( const QObject* slot )
{
	disconnect(this, SIGNAL(projectionMatrixTransposeChanged(Eigen::Matrix4f)),
	           slot, SLOT(setProjectionTranspose(Eigen::Matrix4f)));
}

void Camera::registerViewProjectionMatrix ( const QObject* slot )
{
	connect(this, SIGNAL(viewProjectionMatrixChanged(Eigen::Matrix4f)),
	        slot, SLOT(setViewProjection(Eigen::Matrix4f)));
}

void Camera::unregisterViewProjectionMatrix ( const QObject* slot )
{
	disconnect(this, SIGNAL(viewProjectionMatrixChanged(Eigen::Matrix4f)),
	           slot, SLOT(setViewProjection(Eigen::Matrix4f)));
}

void Camera::registerViewProjectionMatrixTranspose ( const QObject* slot )
{
	connect(this, SIGNAL(viewProjectionMatrixTransposeChanged(Eigen::Matrix4f)),
	        slot, SLOT(setViewProjectionTranspose(Eigen::Matrix4f)));
}

void Camera::unregisterViewProjectionMatrixTranspose ( const QObject* slot )
{
	disconnect(this, SIGNAL(viewProjectionMatrixTransposeChanged(Eigen::Matrix4f)),
	           slot, SLOT(setViewProjectionTranspose(Eigen::Matrix4f)));
}

void Camera::registerFOV ( const QObject* slot )
{
	connect(this, SIGNAL(FOVChanged(float)),
	        slot, SLOT(setFOV(float)));
}

void Camera::unregisterFOV ( const QObject* slot )
{
	disconnect(this, SIGNAL(FOVChanged(float)),
	           slot, SLOT(setFOV(float)));
}

void Camera::registerViewport ( const QObject* slot )
{
	connect(this, SIGNAL(viewportChanged(unsigned int, unsigned int, unsigned int, unsigned int)),
	        slot, SLOT(setViewport(unsigned int, unsigned int, unsigned int, unsigned int)));
}

void Camera::unregisterViewport ( const QObject* slot )
{
	disconnect(this, SIGNAL(viewportChanged(unsigned int, unsigned int, unsigned int, unsigned int)),
	           slot, SLOT(setViewport(unsigned int, unsigned int, unsigned int, unsigned int)));
}

void Camera::registerFrustum ( const QObject* slot )
{
	connect(this, SIGNAL(frustumChanged( float, float, float, float)),
	        slot, SLOT(setFrustum( float, float, float, float)));
}

void Camera::unregisterFrustum ( const QObject* slot )
{
	disconnect(this, SIGNAL(frustumChanged(float, float, float, float)),
	           slot, SLOT(setFrustum(float, float, float, float)));
}

void Camera::registerFarDistance ( const QObject* slot )
{
	connect(this, SIGNAL(farDistanceChanged(float)),
	        slot, SLOT(setFarDistance(float)));
}

void Camera::unregisterFarDistance ( const QObject* slot )
{
	disconnect(this, SIGNAL(farDistanceChanged(float)),
	           slot, SLOT(setFarDistance(float)));
}

void Camera::registerNearDistance ( const QObject* slot )
{
	connect(this, SIGNAL(nearDistanceChanged(float)),
	        slot, SLOT(setNearDistance(float)));
}

void Camera::unregisterNearDistance ( const QObject* slot )
{
	disconnect(this, SIGNAL(nearDistanceChanged(float)),
	           slot, SLOT(setNearDistance(float)));
}

void Camera::registerClipRange ( const QObject* slot )
{
	connect(this, SIGNAL(clipRangeChanged(ClipRange)),
	        slot, SLOT(setClipRange(ClipRange)));
}

void Camera::unregisterClipRange ( const QObject* slot )
{
	disconnect(this, SIGNAL(clipRangeChanged(ClipRange)),
	           slot, SLOT(setClipRange(ClipRange)));
}

