// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id$
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
	//LightSource& operator = (const LightSource&) throw();
	//bool operator < (const LightSource&) const throw();
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
	~Camera() throw();
	//Camera& operator = (const Camera&) throw();
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
	void rotate(const Quaternion&, const Vector3&) throw();
	virtual void clear() throw();
	String toString() const	throw();
	bool readFromString(const String&) throw();
	//bool operator == (const Camera& ) const throw();
	//bool operator < (const Camera& camera) const throw();
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
	virtual void clear() throw();
//	virtual List<LightSource>& getLightSources() throw();  ?????
//	virtual const List<LightSource>& getLightSources() const throw();  ?????
	virtual void addLightSource(const LightSource& ) throw();
	virtual void removeLightSource(const LightSource& ) throw();
	void clearLightSources() throw();

	virtual Camera& getCamera() throw();
	// virtual const Camera& getCamera() const throw();

	virtual void setCamera(const Camera& ) throw();
	virtual const ColorRGBA& getBackgroundColor() const throw();
	virtual void setBackgroundColor(const ColorRGBA& ) throw();
	virtual void showCoordinateSystem(bool ) throw();
	bool coordinateSystemEnabled() const throw();
	void setEyeDistance(float) throw();
	float getEyeDistance() const throw();				
	void setFocalDistance(float) throw();
	float getFocalDistance() const throw();
	void setSwapSideBySideStereo(bool) throw();
	bool swapSideBySideStereo() const throw();
	float getFogIntensity() const	throw();
	void setFogIntensity(float) throw();
	float getSpecularIntensity() const throw();
	void setSpecularIntensity(float) throw();
	float getDiffuseIntensity() const throw();
	void setDiffuseIntensity(float)	throw();		
	float getAmbientIntensity() const throw();
	void setAmbientIntensity(float) throw();			
	float getShininess() const throw();
	void setShininess(float)throw();
	//bool operator == (const Stage& ) const throw();
	Vector3 calculateRelativeCoordinates(Vector3) const;
	Vector3 calculateAbsoluteCoordinates(Vector3) const;
	//void dump(std::ostream& s = std::cout, Size depth = 0) const throw();
};

