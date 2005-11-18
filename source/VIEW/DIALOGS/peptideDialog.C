// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: peptideDialog.C,v 1.9.6.3 2005/11/18 15:21:13 amoll Exp $

#include <BALL/VIEW/DIALOGS/peptideDialog.h>
#include <BALL/COMMON/logStream.h>

#include <qlineedit.h>
#include <qbuttongroup.h>
#include <qradiobutton.h>

namespace BALL
{
	namespace VIEW
	{
		String PeptideDialog::all_amino_acids_ = "ACDEFGHIKLMNPQRSTVWY";

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
			if (sequence->text().length() <= 1)
			{
				sequence->clear();
				return;
			}

			String old = sequence->text().ascii();
			old.truncate(old.size() - 1);
			sequence->setText(old.c_str());
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

		void PeptideDialog::insert_(char a)
			throw()
		{
			sequence->insert(QString(QChar(a)));
		}
		
		bool PeptideDialog::isAminoAcid_(char a) const
		{
			return all_amino_acids_.has(a);
		}

		void PeptideDialog::insert_seq()
		{
			/////////////////////////////////////////////
			// first check sanity
			String aa(sequence->text().ascii());
			aa.toUpper();

			String new_aa;
			for (Position p = 0; p < aa.size(); p++)
			{
				if (isAminoAcid_(aa[p]))
				{
					new_aa += aa[p];
				}
			}

			if (new_aa != sequence->text().ascii())
			{
				sequence->setText(new_aa.c_str());
				return;
			}

			/////////////////////////////////////////////
			// no difference ?
			if (sequence_.size() == sequence->text().length())
			{
				bool equal = true;
				String s = sequence->text().ascii();
				for (Position p = 0; p < sequence_.size(); p++)
				{
					if ((sequence_[p]).getType()[0] != s[p])
					{
						equal = false;
						break;
					}

					if (equal) return;
				}
			}

			/////////////////////////////////////////////
			// empty string?
			Size written_seq_size = sequence->text().length();
			if (written_seq_size == 0)
			{
				sequence->clear();
				sequence_.clear();
				return;
			}

			/////////////////////////////////////////////
			Size old_size = sequence_.size();
			
			//if written_seq < already built sequence => delete amino acids
			if (written_seq_size < old_size)
			{
				sequence_.resize(written_seq_size);

				String s = sequence->text().ascii();
				for (Position p = 0; p < sequence_.size(); p++)
				{
					sequence_[p].setAminoAcidType(String(s[p]));
				}

				if (sequence_.size() != 0)
				{
					Peptides::AminoAcidDescriptor& aad = sequence_[sequence_.size()-1];
					String phis = String(aad.getPhi().toDegree());
					truncString_(phis);

					String psis = String(aad.getPsi().toDegree());
					truncString_(psis);

					String omegas = String(aad.getOmega().toDegree());
					truncString_(omegas);

					if (phis == "-47" && psis == "-58")
					{
						alpha->setChecked(true);
					}
					else
					{
						if (phis == "180" && psis == "180")
						{
							beta->setChecked(true);
						}
						else
						{
							other->setChecked(true);
						}
					}

					phi->setText(phis.c_str());
					omega->setText(omegas.c_str());
					psi->setText(psis.c_str());
				}

				return;
			}

			/////////////////////////////////////////////
			// if written_seq > already built sequence => add amino acid
			if (written_seq_size > old_size)
			{
				Angle a_phi(String(phi->text().ascii()).toFloat(), false);
				Angle a_psi(String(psi->text().ascii()).toFloat(), false);
				Angle a_omega(String(omega->text().ascii()).toFloat(), false);
				const String aa = Substring(sequence->text().ascii(), old_size, written_seq_size - old_size);
				for (Position p = 0; p < aa.size(); p++)
				{
					addAminoAcid(aa[p], a_phi, a_psi, a_omega);
				}
				return;
			}

			/////////////////////////////////////////////
			// same size, but different amino acids
			if (written_seq_size == old_size)
			{
				String s = sequence->text().ascii();
				for (Position p = 0; p < sequence_.size(); p++)
				{
					sequence_[p].setAminoAcidType(String(s[p]));
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
