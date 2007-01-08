class VRMLRenderer 
	: Renderer
{
%TypeHeaderCode
	#include <BALL/VIEW/RENDERING/VRMLRenderer.h>
	using namespace BALL;
	using namespace BALL::VIEW;
%End
  public:

	VRMLRenderer() throw();
	VRMLRenderer(const String&); //throw(Exception::FileNotFound);
	~VRMLRenderer() throw();
	virtual void clear() throw();
	void setFileName(const String&); //throw(Exception::FileNotFound);
	String VRMLColorRGBA(const ColorRGBA&) throw();
	String VRMLVector3(Vector3)	throw();
	void VRMLColor(const ColorRGBA&) throw();
	virtual bool init(const Stage&) throw();
	virtual bool finish() throw();
	void renderSphere_(const Sphere&) throw();
	void renderMesh_(const Mesh&) throw();
	void renderTube_(const Tube&) throw();
	void renderTwoColoredTube_(const TwoColoredTube&) throw();
	void out_(const String&) throw();
	void outheader_(const String&) throw();
	void outfinish_(const String&) throw();
};
