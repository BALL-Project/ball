// $Id: embeddable.sip,v 1.2 2001/07/25 11:32:32 oliver Exp $

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
