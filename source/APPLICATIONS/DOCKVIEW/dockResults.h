//   // -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#ifndef DOCKRESULTS_H
#define DOCKRESULTS_H

namespace BALL
{

		class DockResults
		{
			public:
				
				//Constructor
				DockResults()
					throw();

				/// Destructor
				virtual ~DockResults()
					throw();
					
			private:
			
				ConformationSet conformation_set_;
				Options options_;
		}
		
}