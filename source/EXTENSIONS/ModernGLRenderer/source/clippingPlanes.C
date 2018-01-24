#include <clippingPlanes.h>

#include <iostream>


ClippingPlanes::ClippingPlanes()
{
	clear();
}

ClippingPlanes::ClippingPlanes ( const ClippingPlanes& clipping_planes )
{
	if (this != &clipping_planes) // protect against invalid self-assignment
	{
		for(unsigned int i = 0; i < MAX_CLIPPING_PLANES+4; i++)
		{
			planes_[i] = clipping_planes.planes_[i];
		}

		num_planes_ = clipping_planes.num_planes_;
		frustum_planes_set_ = clipping_planes.frustum_planes_set_;
	}
}

ClippingPlanes::~ClippingPlanes()
{
}

ClippingPlanes& ClippingPlanes::operator= ( const ClippingPlanes& clipping_planes )
{
	if (this != &clipping_planes) // protect against invalid self-assignment
	{
		for(unsigned int i = 0; i < MAX_CLIPPING_PLANES+4; i++)
		{
			planes_[i] = clipping_planes.planes_[i];
		}

		num_planes_ = clipping_planes.num_planes_;
		frustum_planes_set_ = clipping_planes.frustum_planes_set_;
	}

	// by convention, always return *this
	return *this;
}

void ClippingPlanes::dump()
{
	std::cout << "[Clipping Plane]: dump:" << std::endl;
	std::cout << "\t MAX_CLIPPING_PLANES: " << MAX_CLIPPING_PLANES << std::endl;
	std::cout << "\t num planes: " << num_planes_ << std::endl;
	std::cout << "\t frustum_planes_set_: " << frustum_planes_set_ << std:: endl;

	for(unsigned int i = 0; i < MAX_CLIPPING_PLANES; i++)
	{
		std::string s;

		if(frustum_planes_set_)
		{
			switch (i)
			{
				case 0:
					s = "near: ";
					break;
				case 1:
					s = "far: ";
					break;
				case 2:
					s = "left: ";
					break;
				case 3:
					s = "right: ";
					break;
				case 4:
					s = "top: ";
					break;
				case 5:
					s = "bottom: ";
					break;
				default:
					std::stringstream sstm;
					sstm << i << ": ";
					s = sstm.str();
					break;
			}
		}
		else
		{
			std::stringstream sstm;
			sstm << i << ": ";
			s = sstm.str();
		}

		std::cout << "\t\t" << s << planes_[i*4 + 0] << "x + "
		          << planes_[i*4 + 1] << "y + "
		          << planes_[i*4 + 2] << "z + "
		          << planes_[i*4 + 3] << " = 0"<< std:: endl;
	}
}

bool ClippingPlanes::addClippingPlane (const Eigen::Vector4f& plane )
{
	return addClippingPlane(plane[0], plane[1], plane[2], plane[3]);
}

bool ClippingPlanes::addClippingPlane ( float a, float b, float c, float d )
{
	return setClippingPlane(std::max(6, num_planes_), a, b, c, d);
}

bool ClippingPlanes::addClippingPlanes ( const std::vector< Eigen::Vector4f >& clippingPlanes )
{
	bool r = true;

	for(unsigned int i = 0; i < clippingPlanes.size(); i++)
	{
		const Eigen::Vector4f plane = clippingPlanes.at(i);
		r = r && setClippingPlane(std::max(6, num_planes_), plane);

		if(!r) return false;
	}

	return r;
}

bool ClippingPlanes::setClippingPlane ( unsigned int i, const Eigen::Vector4f& plane)
{
	return setClippingPlane(i, plane[0], plane[1], plane[2], plane[3]);
}

bool ClippingPlanes::setClippingPlane ( unsigned int i, float a, float b, float c, float d )
{
	if(i < MAX_CLIPPING_PLANES)
	{
		planes_[4*i + 0] = a;
		planes_[4*i + 1] = b;
		planes_[4*i + 2] = c;
		planes_[4*i + 3] = d;

		normalize_(planes_ + i*4);

		num_planes_ = std::max((int) i + 1, num_planes_);
		return true;
	}
	else
	{
		std::cerr << "[Clipping Plane]: [ERROR]: Could not add clipping plane " << i
		          << "(" << a <<"x + " << b << "y + " << c << "z + " << d << "): "
		          << "Maximum number of clipping planes is " << MAX_CLIPPING_PLANES << std::endl;
		return false;
	}
}

int ClippingPlanes::countClippingPlanes()
{
	return num_planes_;
}

void ClippingPlanes::createFrustumClippingPlanes ( const Eigen::Affine3f& view_matrix, const Eigen::Matrix4f& projection_matrix )
{

	Eigen::Matrix4f mvp = (projection_matrix * Eigen::Matrix4f(view_matrix.data()));

	//Eigen::Vector4f near   = Eigen::Vector4f(mvp(2,0)+mvp(3,0), mvp(2,1)+mvp(3,1), (mvp(2,2)+mvp(3,2)), mvp(2,3)+mvp(3,3));
	Eigen::Vector4f near     =Eigen::Vector4f(1,0,0,42.); //TODO use upper one. However, clipping with this plane does not work
	setClippingPlane(0, near);
	Eigen::Vector4f far    = Eigen::Vector4f(-mvp(2,0)+mvp(3,0), -mvp(2,1)+mvp(3,1), -mvp(2,2)+mvp(3,2), -mvp(2,3)+mvp(3,3));
	setClippingPlane(1, far);
	Eigen::Vector4f left   = Eigen::Vector4f(mvp(0,0)+mvp(3,0), mvp(0,1)+mvp(3,1), mvp(0,2)+mvp(3,2), (mvp(0,3)+mvp(3,3)));
	setClippingPlane(2, left);
	Eigen::Vector4f right  = Eigen::Vector4f(-mvp(0,0)+mvp(3,0), -mvp(0,1)+mvp(3,1), -mvp(0,2)+mvp(3,2), -mvp(0,3)+mvp(3,3));
	setClippingPlane(3, right);
	Eigen::Vector4f top    = Eigen::Vector4f(-mvp(1,0)+mvp(3,0), -mvp(1,1)+mvp(3,1), -mvp(1,2)+mvp(3,2), -mvp(1,3)+mvp(3,3));
	setClippingPlane(4, top);
	Eigen::Vector4f bottom = Eigen::Vector4f(mvp(1,0)+mvp(3,0), mvp(1,1)+mvp(3,1), mvp(1,2)+mvp(3,2), mvp(1,3)+mvp(3,3));
	setClippingPlane(5, bottom);

	num_planes_ = std::max(num_planes_, 6);
	frustum_planes_set_ = true;
}

bool ClippingPlanes::clear()
{
	num_planes_ = 0;
	frustum_planes_set_ = false;

	for(unsigned int i = 0; i < MAX_CLIPPING_PLANES; i++)
	{
		planes_[4*i + 0] = 0.;
		planes_[4*i + 1] = 0.;
		planes_[4*i + 2] = 1.;
		planes_[4*i + 3] = 100.;
	}

	return true;
}

float* ClippingPlanes::getClippingPlane ( int i )
{
	return planes_ + i*4;
}

float* ClippingPlanes::getClippingPlanes()
{
	return planes_;
}

void ClippingPlanes::normalize_ ( float* plane )
{
	float normalize = sqrt(plane[0] * plane[0] + plane[1] * plane[1] + plane[2] * plane[2]);
	plane[0] = plane[0] / normalize;
	plane[1] = plane[1] / normalize;
	plane[2] = plane[2] / normalize;
	plane[3] = plane[3] / normalize;
}

