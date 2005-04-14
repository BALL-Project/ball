// $Id: dockProgressDialog.C,v 1.1.2.5 2005/04/14 16:32:12 leonhardt Exp $
//

#include "dockProgressDialog.h"

# include <BALL/STRUCTURE/DOCKING/geometricFit.h>
#include <BALL/VIEW/KERNEL/message.h>

#include <qprogressbar.h>
#include <qtextedit.h>
#include <qpushbutton.h>
#include <qmessagebox.h>

namespace BALL
{
	namespace VIEW
	{
		
		// Constructor
		DockProgressDialog::DockProgressDialog(QWidget* parent,  const char* name, bool modal, WFlags fl)
			throw()
			: DockProgressDialogData(parent, name, modal, fl),
				ModularWidget(name),
				alg_(0)
		{
			#ifdef BALL_VIEW_DEBUG
				Log.error() << "new DockProgressDialog " << this << std::endl;
			#endif
		
			// register the widget with the MainControl
			ModularWidget::registerWidget(this);
			
			connect(&timer_, SIGNAL(timeout()), SLOT(updateProgress_()));
		}
		
		// Destructor	
		DockProgressDialog::~DockProgressDialog()
			throw()
		{
			#ifdef BALL_VIEW_DEBUG
				Log.info() << "Destructing object " << this << " of class DockProgressDialog" << std::endl;
			#endif 
		}
		
		
		void DockProgressDialog::setDockingAlgorithm(DockingAlgorithm* alg)
			throw()
		{
			 alg_ = alg;
		}	
				
		const DockingAlgorithm* DockProgressDialog::getDockingAlgorithm() const
			throw()
		{
		  return alg_;
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
		void DockProgressDialog::show()
		{
			timer_.start(1000, true);
			//show dialog to user
			DockProgressDialogData::show();
		}
		
		//
		void DockProgressDialog::pauseClicked()
		{
			if (alg_->wasPaused())
			{
				pause_button->setText("Continue");
				alg_->pause();
			}
			else 
			{
				pause_button->setText("Pause");
				alg_->proceed();
			}
		}	
		
		//
		void DockProgressDialog::abortClicked()
		{
			alg_->abort();
			close();
		}
		
		
		//
		void DockProgressDialog::updateProgress_()
		{
			if (alg_->wasAborted()) return;
			Log.error() << "in DockProgressDialog::updateProgress_" << std::endl;
			progress_bar->setProgress((int)(alg_->getProgress() * 100.0), 100);
			
			//if (!alg_->hasFinished() && !alg_->wasAborted())
			if (!alg_->hasFinished())
			{
			 	timer_.start(1000, true);
			}
			else
			{
			 	close();
			}
		}
		
	}
}
