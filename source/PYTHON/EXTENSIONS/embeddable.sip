// $Id: embeddable.sip,v 1.1 2001/09/03 16:53:07 oliver Exp $

class Embeddable
{
%HeaderCode
	#include <BALL/CONCEPT/embeddable.h>
	using namespace BALL;
%End
  public:
  // typedef List<const Embeddable*> EmbeddableList;
  ~Embeddable() throw();

  void unregisterThis() throw();
  virtual void registerThis() throw();
};
