// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: stage.sip,v 1.1 2003/12/11 00:27:03 amoll Exp $
//

class LightSource
{
%TypeHeaderCode
	#include <BALL/VIEW/KERNEL/stage.h>
	using namespace BALL;
	using namespace BALL::VIEW;
%End
  public:

	enum Types
	{
		AMBIENT = 0,
		POSITIONAL,
		DIRECTIONAL
	};

	LightSource() throw();
	LightSource(const LightSource&) throw();
	~LightSource() throw();
	const Vector3& getPosition() const throw();
	void setPosition(const Vector3&) throw();
	const Vector3& getDirection() const throw();
	void setDirection(const Vector3&) throw();
	const Angle& getAngle() const throw();
	void setAngle(const Angle&) throw();
	float getIntensity() const throw();
	void setIntensity(float) throw();
	const ColorRGBA& getColor() const throw();
	void setColor(const ColorRGBA&) throw();
	Index getType() const throw();
	void setType(Types) throw();
	void setRelativeToCamera(bool) throw();
	bool isRelativeToCamera() const throw();
	void translate(const Vector3&) throw();
	void rotate(const Quaternion&) throw();
	//bool operator == (const LightSource&) const throw();
	//virtual void dump(std::ostream& s = std::cout, Size depth = 0) const throw();
};

class Camera
{
%TypeHeaderCode
	#include <BALL/VIEW/KERNEL/stage.h>
	using namespace BALL;
	using namespace BALL::VIEW;
%End
	public:

	Camera() throw();
	Camera(const Camera&) throw();
	Camera(const Vector3& , const Vector3& , const Vector3& ) throw();
	virtual ~Camera() throw();
	const Vector3& getViewPoint() const throw();
	void setViewPoint(const Vector3& ) throw();
	const Vector3& getLookAtPosition() const throw();
	void setLookAtPosition(const Vector3& ) throw();
	const Vector3& getLookUpVector() const throw();
	void setLookUpVector(const Vector3& ) throw() ;
	float getDistance() const throw() ;
	Vector3 getViewVector() const throw() ;
	Vector3 getRightVector() const throw() ;
	void translate(const Vector3& ) throw() ;
	void rotate(const Quaternion& ) throw();
	virtual void clear() throw();
	//bool operator == (const Camera& ) const throw();
	//virtual void dump(std::ostream& s = std::cout, Size depth = 0) const throw();
};

class Stage
{
%TypeHeaderCode
	#include <BALL/VIEW/KERNEL/stage.h>
	using namespace BALL;
	using namespace BALL::VIEW;
%End

	public:

	Stage() throw();
	Stage(const Stage& ) throw();
	~Stage() throw();
	void clear() throw();
//	virtual List<LightSource>& getLightSources() throw();  ?????
//	virtual const List<LightSource>& getLightSources() const throw();  ?????
	void addLightSource(const LightSource& ) throw();
	void removeLightSource(const LightSource& ) throw();
	Camera& getCamera() throw();
	const Camera& getCamera() const throw();
	void setCamera(const Camera& ) throw();
	const ColorRGBA& getBackgroundColor() const throw();
	void setBackgroundColor(const ColorRGBA& ) throw();
	void showCoordinateSystem(bool ) throw();
	bool coordinateSystemEnabled() const throw();
	void translate(const Vector3& ) throw();
	void rotate(const Quaternion& ) throw();
	void moveCameraTo(const Camera& ) throw();
	//bool operator == (const Stage& ) const throw();
	//void dump(std::ostream& s = std::cout, Size depth = 0) const throw();
};

