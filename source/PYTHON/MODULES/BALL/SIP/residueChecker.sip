// $Id: residueChecker.sip,v 1.1 2000/08/07 17:46:20 oliver Exp $

class ResidueChecker
//  : public UnaryProcessor<Residue>
	: ResidueProcessor
{
%HeaderCode
	#include <BALL/source/STRUCTURE/residueChecker.h>
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
