// $Id: residueChecker.sip,v 1.1 2001/09/03 16:53:16 oliver Exp $

class ResidueChecker
//  : public UnaryProcessor<Residue>
	: ResidueProcessor
{
%HeaderCode
	#include <BALL/STRUCTURE/residueChecker.h>
	using namespace BALL;
%End
  public:
  // BALL_CREATE(ResidueChecker)
  ResidueChecker();
  ResidueChecker(FragmentDB& /fragment_db/);
  ResidueChecker(const ResidueChecker& /residue_checker/, bool /deep/ = true);
  ~ResidueChecker();
  bool getStatus() const;
  Processor::Result operator () (Residue& /residue/);
  bool start();
  bool finish();
}; 
