#include <BALL/VIEW/WIDGETS/hotkeyTable.h>

namespace BALL
{
	namespace VIEW
	{
		HotkeyTable::HotkeyTable(QWidget* parent,  const char*)
			: QTableWidget(parent),
			  delegate_(this)
		{
			setItemDelegate(&delegate_);

			setColumnCount(4);
			setHorizontalHeaderItem(0, new QTableWidgetItem());
			setHorizontalHeaderItem(1, new QTableWidgetItem());
			setHorizontalHeaderItem(2, new QTableWidgetItem());
			setHorizontalHeaderItem(3, new QTableWidgetItem());
			horizontalHeaderItem(0)->setText("Modifier");
			horizontalHeaderItem(1)->setText("Key");
			horizontalHeaderItem(2)->setText("Command");
			horizontalHeaderItem(3)->setText("Comment");

			setRowCount(0);
			setShowGrid(true);

			//setColumnWidth(0, 90);
			//setColumnWidth(1, 60);
			//setColumnWidth(2, 214);
			//setColumnWidth(3, 200);
			//setGeometry(2,2, 610, 342);

			setSelectionBehavior(QAbstractItemView::SelectRows);
			setSelectionMode(QAbstractItemView::SingleSelection);

			setObjectName("PythonHotkeys");

			modifier_ << "" << "Shift";// << "Alt";

			for (Position p = 2; p < 13; p++)
			{
				keys_ << (String("F") + String(p)).c_str();
			}

			// F2 -> runCurrentScript()
			appendHotkey("None", "F2", "runCurrentScript()", "Run current Python script");

			// F3 -> hideAllRepresentations()
			appendHotkey("None", "F3", "hideAllRepresentations()", "Hide all loaded representations");

			// F4 -> removeWater()
			appendHotkey("None", "F4", "removeWater()", "Delete all water molecules");

			// S-F4 -> randomizeAtoms()
			appendHotkey("Shift", "F4", "randomizeAtoms(0.2)", "Randomize all atom positions by max 0.2 A");

			// F5 -> printAtomTypesForHighlighted()
			appendHotkey("None", "F5", "printAtomTypesForHighlighted()",
									 "Print the atom types for the currently highlighted molecular items");

			// S-F5 -> printAtomTypesForLigands()
			appendHotkey("Shift", "F5", "printAtomTypesForLigands()",
									 "Print the atom types for all ligands");

			// F6 -> showCartoonAndLigand()
			appendHotkey("None", "F6", "highlightLigand()",
									 "Highlight the ligand in the molecular control");

			// S-F6 -> showCartoonAndLigand()
			appendHotkey("Shift", "F6", "showCartoonAndLigand()",
									 "Create VDW for all ligands and a cartoon model for the receptor");

			// F7 -> addOptimizedHydrogens()
			appendHotkey("None", "F7", "addOptimizedHydrogens()",
									 "Add hydrogens through FragmentDB and optimize them");

			// S-F7 -> relaxStructure()
			appendHotkey("Shift", "F7", "relaxStructure()",
									 "Quickly optimize a ligand");

			// F8 -> quickSave()
			appendHotkey("None", "F8", "quickSave()",
									 "Quickly store a project file");

			// S-F8 -> quickLoad()
			appendHotkey("Shift", "F8", "quickLoad()",
									 "Quickly load a project file");

			// F12 -> clearRepresentations()
			appendHotkey("None", "F12", "clearRepresentations()",
									 "Delete all loaded representations");

			// S-F12 -> clearAll()
			appendHotkey("Shift", "F12", "clearAll()",
									 "Delete all molecules and representations");
		}

		void HotkeyTable::appendHotkey(const String& mod, const String& F_key,
																	 const String& command, String comment)
		{
			addEmptyRow();
			Index r = rowCount() - 1;

			item(r, 0)->setText(mod.c_str());
			item(r, 1)->setText(F_key.c_str());
			item(r, 2)->setText(command.c_str());
			item(r, 3)->setText(comment.c_str());
			scrollToBottom();
		}

		list<Hotkey> HotkeyTable::getContent() const
		{
			list<Hotkey> result;
			for (Index pos = 0; pos < rowCount(); pos++)
			{
				if (item(pos, 2)->text().isEmpty())
				{
					Log.error() << "Problem reading content of PythonHotkeys" << std::endl;
					continue;
				}

				bool ok;
				Hotkey hotkey = Hotkey::createHotkey(ascii(item(pos, 0)->text()),
																						 ascii(item(pos, 1)->text()),
																						 ascii(item(pos, 2)->text()),
																						 ok,
																						 ascii(item(pos, 3)->text()));
				if (!ok)
				{
					Log.error() << "Problem reading content of PythonHotkeys" << std::endl;
					return result;
				}

				result.push_back(hotkey);
			}

			return result;
		}


		void HotkeyTable::setContent(const list<Hotkey>& hotkeys)
		{
			setRowCount(hotkeys.size());
			setColumnWidth(2, 230);

			list<Hotkey>::const_iterator it = hotkeys.begin();
			Position p = 0;
			for (; it != hotkeys.end(); it++)
			{
				String s;
				QStringList sl;
				switch ((Position)(*it).button_state)
				{
					case (Position)Qt::NoButton:        s = "None"; break;
					case (Position)Qt::ShiftModifier:   s = "Shift"; break;
					case (Position)Qt::ControlModifier: s = "Ctrl"; break;
					default:
						Log.error() << "Invalid button state for Hotkey" << std::endl;
						return;
				}

				setItem(p, 0, new QTableWidgetItem(s.c_str()));

				Position k = (*it).key - Qt::Key_F1 + 1;
				s = String("F") + String(k);
				setItem(p,1, new QTableWidgetItem(s.c_str()));
				setItem(p,2, new QTableWidgetItem((*it).action.c_str()));
				setItem(p,3, new QTableWidgetItem((*it).comment.c_str()));

				p++;
			}
		}

		void HotkeyTable::addEmptyRow()
		{
			Position p = rowCount();
			insertRow(p);
			scrollToBottom();
			setItem(p, 0, new QTableWidgetItem());
			setItem(p, 1, new QTableWidgetItem());
			setItem(p, 2, new QTableWidgetItem());
			setItem(p, 3, new QTableWidgetItem());
			item(p, 0)->setText("None");
			item(p, 1)->setText("F2");
			item(p, 2)->setText("");
			item(p, 3)->setText("");
			emit(cellActivated(p, 2));
		}

		void HotkeyTable::removeSelection()
		{
			QList<QTableWidgetSelectionRange> l = QTableWidget::selectedRanges();
			if (l.size() == 0) return;

			removeRow((*l.begin()).bottomRow());
		}

		bool HotkeyTable::getValue(String& value) const
		{
			value = "";
			for (Position p = 0; p < (Position)rowCount(); p++)
			{
				value += ascii(item(p, 0)->text()) + "°";
				value += ascii(item(p, 1)->text()) + "°";
				value += ascii(item(p, 2)->text()) + "°";
				value += ascii(item(p, 3)->text()) + "°";
				value += '@';
			}
			return true;
		}

		bool HotkeyTable::setValue(const String& value)
		{
			setRowCount(0);

			vector<String> fields;
			vector<String> fields2;
			Size nr = value.split(fields, "@");

			for (Position p = 0; p < nr; p ++)
			{
				Size nr2 = fields[p].split(fields2, "°");
				if (nr2 < 3)
				{
		//   			BALLVIEW_DEBUG;
					continue;
				}

				fields2.resize(4);

				appendHotkey(fields2[0], fields2[1], fields2[2], fields2[3]);
			}

			return true;
		}
	}
}
