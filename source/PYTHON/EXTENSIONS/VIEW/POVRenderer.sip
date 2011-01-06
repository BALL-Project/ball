// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id$
//

class POVRenderer 
: Renderer
{
%TypeHeaderCode
	#include <BALL/VIEW/RENDERING/RENDERERS/POVRenderer.h>
	using namespace BALL;
	using namespace BALL::VIEW;
	using namespace BALL::Exception;
%End
  public:
  POVRenderer() throw();
  POVRenderer(const String&) throw(FileNotFound);
//  String POVColorRGBA(const ColorRGBA&) throw();
//  String POVFinish(const String&, const ColorRGBA&) throw();
//  String POVVector3(Vector3) throw();
  bool finish() throw();
  void clear() throw();

	virtual bool init(const Stage&, float, float) throw();
  void setFileName(const String&) throw(FileNotFound);
  ~POVRenderer() throw();
};
