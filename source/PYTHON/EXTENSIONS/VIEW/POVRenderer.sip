class POVRenderer
	: SceneExporter
{
%TypeHeaderCode
	#include <BALL/VIEW/RENDERING/RENDERERS/POVRenderer.h>
%End
	public:
	POVRenderer(const String&);
	void clear();

	virtual bool init(const Stage*, float, float) throw();
	~POVRenderer() throw();
};
