// $Id: embeddable.sip,v 1.1 2000/07/18 17:02:22 oliver Exp $

class Embeddable
{
%HeaderCode
	#include <BALL/CONCEPT/embeddable.h>
	using namespace BALL;
%End
  public:
  // typedef List<const Embeddable*> EmbeddableList;
  ~Embeddable();

  void unregisterThis();
  virtual void registerThis();

  protected:
};
