// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
#include <BALL/VIEW/DIALOGS/dockProgressDialog.h>

#include <QtWidgets/QProgressBar>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QMessageBox>
#include <QtWidgets/QLabel>

//#define BALL_VIEW_DEBUG

namespace BALL
{
  namespace VIEW
  {
    // Constructor
    DockProgressDialog::DockProgressDialog(QWidget* parent,  const char* name)
      : QDialog(parent),
				Ui_DockProgressDialogData(),
				alg_(0)
    {
#ifdef BALL_VIEW_DEBUG
      Log.info() << "new DockProgressDialog " << this << std::endl;
#endif

			setupUi(this);
			setWindowTitle(name);
			timer_.setSingleShot(true);
			progress_bar->setMaximum(100);
      
      connect(&timer_, SIGNAL(timeout()), SLOT(updateProgress_()));
    }
    
    // Copy constructor.
    DockProgressDialog::DockProgressDialog(const DockProgressDialog& dock_prog_dialog)
			: QDialog(),
				Ui_DockProgressDialogData(),
				alg_(dock_prog_dialog.alg_),
				//timer_(dock_prog_dialog.timer_),
				start_time_(dock_prog_dialog.start_time_)
    {}
    
    // Destructor	
    DockProgressDialog::~DockProgressDialog()
    {
#ifdef BALL_VIEW_DEBUG
      Log.info() << "Destructing object " << this << " of class DockProgressDialog" << std::endl;
#endif 
    }
    
    // Assignment operator
    const DockProgressDialog& DockProgressDialog::operator =(const DockProgressDialog& dock_prog_dialog)
    {
      if (&dock_prog_dialog != this)
	{
	  alg_ = dock_prog_dialog.alg_;
	  //timer_ = dock_prog_dialog.timer_;           // QTimer::operator=(const QTimer&)' is private !
	  start_time_ = dock_prog_dialog.start_time_;
	}
      return *this;
    }
    
    void DockProgressDialog::setDockingAlgorithm(DockingAlgorithm* alg)
    {
      alg_ = alg;
    }
    
    const DockingAlgorithm* DockProgressDialog::getDockingAlgorithm() const
    {
      return alg_;
    }
    
    // Fill ProgressDialog with information about the chosen
    // docking partners, algorithm, scoring function and options
    void DockProgressDialog::fillDialog(const QString& p1, const QString& p2, const QString& alg, const QString& sf,
					const Options& alg_opt, const Options& sf_opt)
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
      Options::ConstIterator it = alg_opt.begin();
      for (; it != alg_opt.end(); ++it)
			{
				s = it->first.c_str();
				s.append(" : ");
				options->append(s.append(it->second.c_str()));
			}
      
      if (sf_opt.isEmpty())
			{
				options->append("\nThere are no options for this scoring function.");
			}
      else
			{
				options->append("\n*** Options of scoring function ***");
				it = sf_opt.begin();
				for (; it != sf_opt.end(); ++it)
					{
						s = it->first.c_str();
						s.append(" : ");
						options->append(s.append(it->second.c_str()));
					}
			}
    }
    
    //
    void DockProgressDialog::show()
    {
      // start timer, true -> it is a single shot
      timer_.start(1000);
      
      // remember start time
      start_time_ = QDateTime::currentDateTime();
      
      // show dialog to user
			QDialog::show();
    }
    
    // TODO: pause algorithm!!!
    void DockProgressDialog::pauseClicked()
    {
      if(pause_button->text() == "Pause")
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
		}
		
		//
		void DockProgressDialog::updateProgress_()
		{
			if (alg_->wasAborted()) return;
			
			float progress = alg_->getProgress();
			// set progress
			progress_bar->setValue((int)(progress * 100.0));
			// calculate remaining time
			int run_time = start_time_.secsTo(QDateTime::currentDateTime());
			int remain_time, hours, min, sec;
			QString s;
			if (progress > 0.01)
			{
				remain_time = (int)(((1.0 - progress)/progress) * run_time);
				hours = remain_time / 3600;
				min = (remain_time % 3600) / 60;
				sec = (remain_time % 3600) % 60;
				QString convert;
				s.setNum(hours);
				s.append(":");
				if (!(min/10))
				{
				 	s.append("0");
				}
				s.append(convert.setNum(min));
				s.append(":");
				if (!(sec/10))
				{
				 	s.append("0");
				}
				s.append(convert.setNum(sec));
			}
			else
			{
			 	s.append("--:--:--");
			}
			remaining_time->setText(s);
			
			// if docking has not finished restart timer
			// remark: dialog is closed by the docking controller since otherwise the time between 
			// closing the dialog and showing the DockResult in the dataset widget would be too long 
			if (!alg_->hasFinished())
			{
			 	timer_.start(1000);
			}
		}
		
	} // end of namespace View
} // end of namespace BALL
