// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: peptideDialog.C,v 1.3 2003/08/28 13:23:12 oliver Exp $
//

#include <BALL/VIEW/DIALOGS/peptideDialog.h>
#include <BALL/COMMON/logStream.h>

#include <qlineedit.h>
#include <qbuttongroup.h>
#include <qradiobutton.h>

namespace BALL
{

	namespace VIEW
	{

		PeptideDialog::PeptideDialog( QWidget* parent,  const char* name, WFlags fl )
		 : PeptideDialogData( parent, name, fl ),
			 Peptides::PeptideBuilder(),
			 protein_(0)
		{
			show();
		}

		void PeptideDialog::back_pressed()
		{
			if (sequence_.size() > 1)
			{
				String old = sequence->text().ascii();
				old = old.getSubstring(0, old.size()-1);
				sequence->setText(old.c_str());
				sequence_.pop_back();
			}
			else 
			{
				sequence->setText("");
			}

			if (sequence_.size() != 0)
			{
				Peptides::AminoAcidDescriptor& aad = sequence_[sequence_.size()-1];
				String data = String(aad.getPhi().toDegree());
				truncString_(data);
				phi->setText(data.c_str());

				data = String(aad.getPsi().toDegree());
				truncString_(data);
				psi->setText(data.c_str());

				data = String(aad.getOmega().toDegree());
				truncString_(data);
				omega->setText(data.c_str());
			}
		}

		void PeptideDialog::truncString_(String& data)
			throw()
		{
			if (!data.has('.')) return;
			data.trimRight("0");
			if (data.hasSuffix(".")) data.trimRight(".");
		}

		void PeptideDialog::close_pressed()
		{
			protein_ = 0;
			if (sequence->text() != "")
			{
				protein_ = construct();
			}
			hide();
		}

		void PeptideDialog::angle_changed()
		{
			if (alpha->isChecked())
			{
				phi->setEnabled(false);
				psi->setEnabled(false);
				phi->setText("-47");
				psi->setText("-58");
			}
			else if (beta->isChecked())
			{
				phi->setEnabled(false);
				psi->setEnabled(false);
				phi->setText("180");
				psi->setText("180");
			}
			else // other
			{
				phi->setEnabled(true);
				psi->setEnabled(true);
			}
		}

		void PeptideDialog::insert_(const String& aa)
			throw()
		{
			Angle a_phi(String(phi->text().ascii()).toFloat(), false);
			Angle a_psi(String(psi->text().ascii()).toFloat(), false);
			Angle a_omega(String(omega->text().ascii()).toFloat(), false);
			addAminoAcid(aa.c_str(), a_phi, a_psi, a_omega);
			sequence->insert(aa.c_str());
		}


		String PeptideDialog::getSequence()
			throw()
		{
			return sequence->text().ascii();
		}

	} // namespace VIEW

} // namespace BALL
