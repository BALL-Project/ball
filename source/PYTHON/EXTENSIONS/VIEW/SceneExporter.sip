class SceneExporter /Abstract/
{
%TypeHeaderCode
	#include <BALL/VIEW/RENDERING/sceneExporter.h>
	using namespace BALL;
	using namespace BALL::VIEW;
%End
	public:
		explicit SceneExporter(const String& filename);

		virtual ~SceneExporter();

		virtual bool init(const Stage* stage, float width, float height) = 0;

		virtual bool exportOneRepresentation(const Representation* rep) = 0;

		virtual bool finish() = 0;
};
