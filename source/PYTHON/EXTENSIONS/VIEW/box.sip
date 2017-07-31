class Box
	: GeometricObject,
	  Box3
{
%TypeHeaderCode
	#include <BALL/VIEW/PRIMITIVES/box.h>
%End
	public:

	Box();
	Box(const Box&);
	Box(const Vector3&, const Vector3&, const Vector3&, float);
	Box(const Vector3&, const Vector3& = Vector3( 0, 1, 0),
		const Vector3& = Vector3(1, 0, 0), const Vector3& = Vector3(0, 0, 1));
	bool isValid() const;
	void clear();
	void set(const Box&);
	~Box();

	void getVertices(vector<Vector3>& vertices) const;

	void setColor(const ColorRGBA& color);

	ColorRGBA& getColor();

	void setWidth(float w);
	float getWidth() const;

	void setHeight(float h);
	float getHeight() const;

	void setDepth(float d);
	float getDepth() const;

	void setPoint(const Vector3& p);
	Vector3 getPoint() const;
};
