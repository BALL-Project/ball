class Renderer /Abstract/
{
%TypeHeaderCode
	#include <BALL/VIEW/RENDERING/RENDERERS/renderer.h>
%End
	public:
	Renderer() throw();
	Renderer(const Renderer&) throw();
	bool hasStage() const throw();
	bool operator == (const Renderer&) const throw();
	const Stage& getStage() const throw();
	float getHeight() const throw();
	float getWidth() const throw();
	void clear() throw();
	void setSize(float, float) throw();
	void setStage(const Stage&) throw();
	void bufferRepresentation(const Representation&) throw();
	~Renderer() throw();
};
