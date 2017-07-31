class POVRenderer
	: Renderer
{
%TypeHeaderCode
	#include <BALL/VIEW/RENDERING/RENDERERS/POVRenderer.h>
%End
	public:
	POVRenderer() throw();
	POVRenderer(const String&) throw(FileNotFound);
	bool finish() throw();
	void clear() throw();

	virtual bool init(const Stage&, float, float) throw();
	void setFileName(const String&) throw(FileNotFound);
	~POVRenderer() throw();
};
