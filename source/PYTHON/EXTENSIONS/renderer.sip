// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: renderer.sip,v 1.2 2004/04/16 13:18:40 amoll Exp $
//

class Renderer 
{
%TypeHeaderCode
	#include <BALL/VIEW/RENDERING/renderer.h>
	using namespace BALL;
	using namespace BALL::VIEW;
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
	bool render(const Representation&) throw();
  ~Renderer() throw();
};
