// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: peptideDialog.C,v 1.7 2004/05/10 12:12:02 amoll Exp $
//
#include <iostream>

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
			//change sequence shown in line-edit
			if (sequence_.size() > 1)
			{
				String old = sequence->text().ascii();
				old = old.getSubstring(0, old.size()-1);
				sequence->setText(old.c_str());
			}
			else
				sequence->setText("");
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

		void PeptideDialog::insert_(const String& aa)
			throw()
		{
			String tmp = aa;
			tmp.toUpper();

			static const string all_amino_acids = "ACDEFGHIKLMNOPQRSTUVWY";
			string new_aa="";
			for (String::iterator it = tmp.begin(); it != tmp.end(); ++it)
			{
				if (all_amino_acids.find(*it) != string::npos)
					new_aa.push_back(*it);
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
			int written_seq_size = written_seq.size();
			int sequence_size = sequence_.size();

			String last_letter = *(written_seq.end()-1);
			last_letter.toUpper();
			static const string all_amino_acids = "ACDEFGHIKLMNPQRSTVWY";
			
			if (all_amino_acids.find(last_letter) != string::npos || written_seq == "")
			{
				char ch = *(written_seq.end()-1);
				if (islower_(ch) == true) //if last_letter is lower case, change to upper case
				{
					//delete last written letter and write it as upper case letter
					if(written_seq > 1)
						written_seq = written_seq.getSubstring(0, written_seq.size()-1);
					else
						written_seq = "";
					written_seq = written_seq + last_letter;
					sequence->setText(written_seq.c_str());
					return;	
				}
				else 
				{
					// if written_seq > already built sequence => add amino acid
					if (written_seq_size > sequence_size)
					{
						written_seq.toLower();
						const String aa = *(written_seq.end()-1);
						Angle a_phi(String(phi->text().ascii()).toFloat(), false);
						Angle a_psi(String(psi->text().ascii()).toFloat(), false);
						Angle a_omega(String(omega->text().ascii()).toFloat(), false);
						addAminoAcid(aa.c_str(), a_phi, a_psi, a_omega);
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
					if (written_seq_size == sequence_size)// do nothing
						return;	
				}
			}
			else//if written letter is no amino acid, set text to original string and return
			{
				if(written_seq > 1)
						written_seq = written_seq.getSubstring(0, written_seq.size()-1);
					else
						written_seq = "";
				sequence->setText(written_seq.c_str());
			}
		}

		String PeptideDialog::getSequence()
			throw()
		{
			return sequence->text().ascii();
		}

	} // namespace VIEW
} // namespace BALL
