// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: peptideDialog.C,v 1.9 2004/06/26 15:33:58 amoll Exp $

#include <BALL/VIEW/DIALOGS/peptideDialog.h>
#include <BALL/COMMON/logStream.h>

#include <qlineedit.h>
#include <qbuttongroup.h>
#include <qradiobutton.h>

namespace BALL
{
	namespace VIEW
	{
		String PeptideDialog::all_amino_acids_ = "ACDEFGHIKLMNOPQRSTUVWY";

		PeptideDialog::PeptideDialog( QWidget* parent, const char* name, WFlags fl )
		 : PeptideDialogData( parent, name, fl ),
			 Peptides::PeptideBuilder(),
			 protein_(0)
		{
			sequence->clear();
			show();
		}

		void PeptideDialog::back_pressed()
		{
			//change sequence shown in line-edit
			if (sequence->text().length() > 1)
			{
				String old = sequence->text().ascii();
				old = old.getSubstring(0, old.size()-1);
				sequence->setText(old.c_str());
			}
			else
			{
				sequence->clear();
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
			if (sequence->text() != "" && fragment_db_ != 0)
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

		void PeptideDialog::insert_(String aa)
			throw()
		{
			aa.toUpper();

			String new_aa;
			for (String::iterator it = aa.begin(); it != aa.end(); ++it)
			{
				if (all_amino_acids_.find(*it) != string::npos)
				{
					new_aa.push_back(*it);
				}
			}
			sequence->insert(new_aa.c_str());
		}
		
		bool PeptideDialog::islower_(char ch)
			throw()
		{
			return ( ch>=97 && ch<=122 );
		}

		void PeptideDialog::insert_seq()
		{
			String written_seq = sequence->text().ascii();
			written_seq.toUpper();
			if (written_seq != sequence->text().ascii())
			{
				sequence->setText(written_seq.c_str());
				return;
			}

			Size written_seq_size = written_seq.size();
			if (written_seq_size == 0)
			{
				sequence->clear();
				sequence_.clear();
				return;
			}

			//if written letter is no amino acid, set text to original string and return
			String last_letter = *(written_seq.end()-1);
			if (all_amino_acids_.find(last_letter) == string::npos)
			{
				written_seq = written_seq.getSubstring(0, written_seq.size()-1);
				sequence->setText(written_seq.c_str());
				return;
			}

			// if written_seq > already built sequence => add amino acid
			Size sequence_size = sequence_.size();
			if (written_seq_size > sequence_size)
			{
				written_seq.toLower();
				Angle a_phi(String(phi->text().ascii()).toFloat(), false);
				Angle a_psi(String(psi->text().ascii()).toFloat(), false);
				Angle a_omega(String(omega->text().ascii()).toFloat(), false);
				const String aa = *(written_seq.end()-1);
				addAminoAcid(aa.c_str(), a_phi, a_psi, a_omega);
				return;
			}

			//if written_seq < already built sequence => delete amino acid
			if (written_seq_size < sequence_size)
			{
				sequence_.pop_back();
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
		}

		String PeptideDialog::getSequence()
			throw()
		{
			return sequence->text().ascii();
		}

	} // namespace VIEW
} // namespace BALL
