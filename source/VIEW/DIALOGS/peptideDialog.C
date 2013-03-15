// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/VIEW/DIALOGS/peptideDialog.h>
#include <BALL/VIEW/KERNEL/common.h>
#include <BALL/COMMON/logStream.h>
#include <BALL/STRUCTURE/peptideCapProcessor.h>

#include <QtWidgets/QLineEdit>
#include <QtWidgets/QRadioButton>

namespace BALL
{
	namespace VIEW
	{
		String PeptideDialog::all_amino_acids_ = "ACDEFGHIKLMNPQRSTVWY";

		PeptideDialog::PeptideDialog( QWidget* parent, const char* name, Qt::WindowFlags fl )
		 : QDialog(parent, fl),
		 	 Ui_PeptideDialogData(),
			 Peptides::PeptideBuilder(),
			 protein_(0)
		{
			setupUi(this);
			// signals and slots connections
			connect( ala, SIGNAL( pressed() ), this, SLOT( ala_pressed() ) );
			connect( arg, SIGNAL( pressed() ), this, SLOT( arg_pressed() ) );
			connect( asn, SIGNAL( pressed() ), this, SLOT( asn_pressed() ) );
			connect( cys, SIGNAL( pressed() ), this, SLOT( cys_pressed() ) );
			connect( gln, SIGNAL( pressed() ), this, SLOT( gln_pressed() ) );
			connect( glu, SIGNAL( pressed() ), this, SLOT( glu_pressed() ) );
			connect( gly, SIGNAL( pressed() ), this, SLOT( gly_pressed() ) );
			connect( his, SIGNAL( pressed() ), this, SLOT( his_pressed() ) );
			connect( ile, SIGNAL( pressed() ), this, SLOT( ile_pressed() ) );
			connect( leu, SIGNAL( pressed() ), this, SLOT( leu_pressed() ) );
			connect( lys, SIGNAL( pressed() ), this, SLOT( lys_pressed() ) );
			connect( met, SIGNAL( pressed() ), this, SLOT( met_pressed() ) );
			connect( phe, SIGNAL( pressed() ), this, SLOT( phe_pressed() ) );
			connect( pro, SIGNAL( pressed() ), this, SLOT( pro_pressed() ) );
			connect( ser, SIGNAL( pressed() ), this, SLOT( ser_pressed() ) );
			connect( thr, SIGNAL( pressed() ), this, SLOT( thr_pressed() ) );
			connect( trp, SIGNAL( pressed() ), this, SLOT( trp_pressed() ) );
			connect( tyr, SIGNAL( pressed() ), this, SLOT( tyr_pressed() ) );
			connect( val, SIGNAL( pressed() ), this, SLOT( val_pressed() ) );
			connect( close_button, SIGNAL( pressed() ), this, SLOT( close_pressed() ) );
			connect( phi, SIGNAL( returnPressed() ), this, SLOT( angle_changed() ) );
			connect( psi, SIGNAL( returnPressed() ), this, SLOT( angle_changed() ) );
			connect( omega, SIGNAL( returnPressed() ), this, SLOT( angle_changed() ) );
			connect( asp, SIGNAL( pressed() ), this, SLOT( asp_pressed() ) );
			connect( alpha, SIGNAL( toggled(bool) ), this, SLOT( angle_changed() ) );
			connect( beta, SIGNAL( toggled(bool) ), this, SLOT( angle_changed() ) );
			connect( other, SIGNAL( toggled(bool) ), this, SLOT( angle_changed() ) );
			connect( sequence, SIGNAL( textChanged(const QString&) ), this, SLOT( insert_seq() ) );
			connect( cancel_button, SIGNAL( pressed() ), this, SLOT( close() ) );

			pro->installEventFilter(this);

			setObjectName(name);
			sequence->clear();
			show();
		}

		bool PeptideDialog::eventFilter(QObject* object, QEvent* event)
		{
			if (static_cast<QPushButton*>(object) == pro)
			{
				switch (event->type())
				{
					case QEvent::Enter:
					 	prolineActivated();
						break;
					case QEvent::Leave:
						prolineDeactivated();
						break;
					default: // do nothing
						break;
				}
			}

			return QDialog::eventFilter(object, event);
		}

		void PeptideDialog::back_pressed()
		{
			//change sequence shown in line-edit
			if (sequence->text().length() <= 1)
			{
				sequence->clear();
				return;
			}

			String old = ascii(sequence->text());
			old.truncate(old.size() - 1);
			sequence->setText(old.c_str());
		}

		void PeptideDialog::truncString_(String& data)
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

			if (build_endcaps->isChecked())
			{
				PeptideCapProcessor pcp;
				protein_->apply(pcp);
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
		
		void PeptideDialog::prolineActivated() 
		{	
			if (alpha->isChecked())
			{
				phi->setEnabled(false);
				psi->setEnabled(false);
				phi->setText("-60");
				psi->setText("-45");
			}
			else if (beta->isChecked())
			{
				phi->setEnabled(false);
				psi->setEnabled(false);
				phi->setText("-60");
				psi->setText("135");
			}
			else // other
			{
				phi->setEnabled(true);
				psi->setEnabled(true);
				phi->setText("-60");
				psi->setText("-45");
			}
		}
      
		void PeptideDialog::prolineDeactivated()
		{
			angle_changed();
		}


		void PeptideDialog::insert_(char a)
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
			String aa(ascii(sequence->text()));
			aa.toUpper();

			String new_aa;
			for (Position p = 0; p < aa.size(); p++)
			{
				if (isAminoAcid_(aa[p]))
				{
					new_aa += aa[p];
				}
			}

			if (new_aa != ascii(sequence->text()))
			{
				sequence->setText(new_aa.c_str());
				return;
			}

			/////////////////////////////////////////////
			// no difference ?
			if (sequence_.size() == (Position)sequence->text().length())
			{
				bool equal = true;
				String s = ascii(sequence->text());
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

				String s = ascii(sequence->text());
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
				Angle a_phi(ascii(phi->text()).toFloat(), false);
				Angle a_psi(ascii(psi->text()).toFloat(), false);
				Angle a_omega(ascii(omega->text()).toFloat(), false);
				const String aa = Substring(ascii(sequence->text()), old_size, written_seq_size - old_size);
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
				String s = ascii(sequence->text());
				for (Position p = 0; p < sequence_.size(); p++)
				{
					sequence_[p].setAminoAcidType(String(s[p]));
				}
			}
		}

		String PeptideDialog::getSequence()
		{
			return ascii(sequence->text());
		}

	} // namespace VIEW
} // namespace BALL
