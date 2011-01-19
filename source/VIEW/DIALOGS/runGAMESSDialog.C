// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/VIEW/DIALOGS/runGAMESSDialog.h>
#include <BALL/VIEW/KERNEL/mainControl.h>
#include <BALL/VIEW/WIDGETS/datasetControl.h>

#include <QtGui/QFileDialog>
#include <QtGui/QComboBox>
#include <QtGui/QCheckBox>
#include <QtGui/QSpinBox>
#include <QtGui/QRadioButton>
#include <QtGui/QLineEdit>

namespace BALL
{
	namespace VIEW
	{
		RunGAMESSDialog::RunGAMESSDialog(QWidget* parent, const char* name)
			: RunGAMESSDialogData(parent, name)
		{
		}

		RunGAMESSDialog::~RunGAMESSDialog()
		{
		}

		String RunGAMESSDialog::extractValue_(const QComboBox* box)
		{
			String tmp(box->currentText().latin1());
			return (tmp.getField(1, "()"));
		}

		void RunGAMESSDialog::setParameters_()
		{
			outfile_.clearParameters();

			/** Set the Control - Group Parameters **/
			outfile_.inBlock("$contrl");
			outfile_.insertBlockedData("SCFTYP", extractValue_(SCFTYP));
			outfile_.insertBlockedData("RUNTYP", extractValue_(RUNTYP));
			outfile_.insertBlockedData("EXETYP", extractValue_(EXETYP));
			outfile_.insertBlockedData("MAXIT",  String( MAXIT->value()));
			outfile_.insertBlockedData("ICHARG", String(ICHARG->value()));
			outfile_.insertBlockedData("MULT",   String(  MULT->value()));

			if (DO_CI->isChecked())
				outfile_.insertBlockedData("CITYP",  extractValue_( CCTYP));
			if (DO_MP->isChecked())
				outfile_.insertBlockedData("MPLEVL", extractValue_(MPLEVL));
			if (DO_CC->isChecked())
				outfile_.insertBlockedData("CCTYP",  extractValue_( CCTYP));

			/** Insert BASIS group data **/
			outfile_.inBlock("$basis");
			outfile_.insertBlockedData("GBASIS", extractValue_(GBASIS));
			outfile_.insertBlockedData("NGAUSS", String(NGAUSS->value()));
			outfile_.insertBlockedData("NDFUNC", String(NDFUNC->value()));
			outfile_.insertBlockedData("NFFUNC", String(NFFUNC->value()));

			if (DIFFSP->isChecked())
				outfile_.insertBlockedData("DIFFSP", "TRUE");
			if ( DIFFS->isChecked())
				outfile_.insertBlockedData( "DIFFS", "TRUE");
		}
		
		void RunGAMESSDialog::accept()
		{
			String filename;
			File::createTemporaryFilename(filename);

			outfile_.open(filename+".inp", std::ios::out);
			setParameters_();
			outfile_.write(*(MainControl::getInstance(0)->getSelectedSystem()));
			outfile_.close();

			String GAMESSCommand = GAMESSPath->text().latin1();
			GAMESSCommand += " " + filename;
			Log.info() << (String)tr("Executing GAMESS") << std::endl;

			system(GAMESSCommand.c_str());
			Log.info() << (String)tr("Done! ") << std::endl;

			File::remove(filename+".inp");

			DatasetControl::getInstance(0)->addGAMESSData();
		}

		void RunGAMESSDialog::saveGAMESSInput()
		{
			QString filename = QFileDialog::getSaveFileName(MainControl::getInstance(0)->getWorkingDir().c_str());
			
			if (filename == "")
				return;

			outfile_.open(filename.latin1(), std::ios::out);
			setParameters_();
			outfile_.write(*(MainControl::getInstance(0)->getSelectedSystem()));
			outfile_.close();
		}
	}
}
