class VRMLRenderer 
	: Renderer
{
%TypeHeaderCode
	#include <BALL/VIEW/RENDERING/RENDERERS/VRMLRenderer.h>
	using namespace BALL;
	using namespace BALL::VIEW;
%End
  public:

	VRMLRenderer();
	VRMLRenderer(const String&); //throw(Exception::FileNotFound);
	~VRMLRenderer();
	virtual void clear();
	void setFileName(const String&); //throw(Exception::FileNotFound);
	String VRMLColorRGBA(const ColorRGBA&);
	String VRMLVector3(Vector3);
	void VRMLColor(const ColorRGBA&);
	virtual bool init(const Stage&);
	virtual bool finish();
	void renderSphere_(const Sphere&);
	void renderMesh_(const Mesh&);
	void renderTube_(const Tube&);
	void renderTwoColoredTube_(const TwoColoredTube&);
	void out_(const String&);
	void outheader_(const String&);
	void outfinish_(const String&);

	private:
	VRMLRenderer(const VRMLRenderer&);
};
