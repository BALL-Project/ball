// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: POVRenderer.sip,v 1.1 2004/02/12 16:18:10 amoll Exp $
//

class POVRenderer 
: Renderer
{
%TypeHeaderCode
	#include <BALL/VIEW/RENDERING/POVRenderer.h>
	using namespace BALL;
	using namespace BALL::VIEW;
%End
  public:
  POVRenderer() throw();
  POVRenderer(const String&) throw(FileNotFound);
//  String POVColorRGBA(const ColorRGBA&) throw();
//  String POVFinish(const String&, const ColorRGBA&) throw();
//  String POVVector3(Vector3) throw();
//  bool finish() throw();
//  void clear() throw();
  void setFileName(const String&) throw(FileNotFound);
  ~POVRenderer() throw();
};
