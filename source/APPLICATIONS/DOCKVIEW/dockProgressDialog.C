// $Id: dockProgressDialog.C,v 1.1.2.4 2005/04/12 11:50:44 haid Exp $
//

#include "dockProgressDialog.h"

# include <BALL/STRUCTURE/DOCKING/geometricFit.h>

#include <qprogressbar.h>
#include <qtextedit.h>
#include <BALL/VIEW/KERNEL/message.h>

namespace BALL
{
	namespace VIEW
	{
		
		// Constructor
		DockProgressDialog::DockProgressDialog(QWidget* parent,  const char* name, bool modal, WFlags fl)
			throw()
			: DockProgressDialogData(parent, name, modal, fl),
				ModularWidget(name)
		{
			#ifdef BALL_VIEW_DEBUG
				Log.error() << "new DockProgressDialog " << this << std::endl;
			#endif
		
			// register the widget with the MainControl
			ModularWidget::registerWidget(this);
		}
		
		// Destructor	
		DockProgressDialog::~DockProgressDialog()
			throw()
		{
			#ifdef BALL_VIEW_DEBUG
				Log.info() << "Destructing object " << this << " of class DockProgressDialog" << std::endl;
			#endif 
		}
		
		//
		void DockProgressDialog::fillDialog(QString p1, QString p2, QString alg, QString sf, Options& alg_opt, Options& sf_opt)
			throw()
		{
			QString s = "Docking partner 1: ";
			general_params->append(s.append(p1));
			s = "Docking partner 2: ";
			general_params->append(s.append(p2));
			s = "Algorithm: ";
			general_params->append(s.append(alg));
			s = "Scoring function: ";
			general_params->append(s.append(sf));
			
			options->append("*** Options of algorithm ***");
			s = "number of best docked structures: ";
			options->append(s.append(alg_opt.get(GeometricFit::Option::BEST_NUM)));
			Options::Iterator it = alg_opt.begin();
			for(; +it; ++it)
			{
				s = it->first;
				s.append(" : ");
				options->append(s.append(it->second));
			}
		}
		
		//
		void DockProgressDialog::onNotify(Message *message)
			throw()
		{
			if (RTTI::isKindOf<DockingProgressMessage>(*message))
			{
				DockingProgressMessage* dpm = RTTI::castTo<DockingProgressMessage>(*message);
				progress_bar->setProgress((int)dpm->getProgress() * 100, 100);
			}
		}
		
		//
		void DockProgressDialog::pauseClicked()
		{
			unsigned int total = 1000;
			progress_bar->setTotalSteps(total);
			for(unsigned int i = 0; i <= total; i++)
			{
				progress_bar->setProgress(i);
			}
		}	
		
		//
		void DockProgressDialog::abortClicked()
		{
		
		}
		
	}
}
