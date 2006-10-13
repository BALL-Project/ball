
	void enlarge(String& text, Size size)
	{
		if (text.size() > size)
		{
			text.truncate(size);
			return;
		}
		while (text.size() < size) text = String(" ") + text;
	}

	void write()
	{
		File file("out.mrk", std::ios::out);
		System& system = *(System*) *getCompositeManager().begin();
		file << "***" << std::endl;
		file << "***" << std::endl;

		String tmp = String(system.countAtoms());
		enlarge(tmp, 5);
		file << tmp;

		tmp = String(system.countBonds());
		enlarge(tmp, 6);
		file << tmp << std::endl;
		
		HashMap<Atom*, Position> map;
		char buf[40];

		Position at_nr = 1;
		AtomIterator ait = system.beginAtom();
		for(; +ait; ++ait)
		{
			Atom& atom = *ait;
			sprintf(buf, "%.4f", atom.getPosition().x);
			tmp = buf;
			tmp.trimRight();
			enlarge(tmp, 10);
			file << tmp;

			sprintf(buf, "%.4f", atom.getPosition().y);
			tmp = buf;
			enlarge(tmp, 11);
			file << tmp;

			sprintf(buf, "%.4f", atom.getPosition().z);
			tmp = buf;
			enlarge(tmp, 11);
			file << tmp;

			tmp = String(atom.getElement().getAtomicNumber());
			enlarge(tmp, 6);
			file << tmp;

			tmp = String(atom.getType());
			enlarge(tmp, 3);
			file << tmp;

			tmp = String(atom.getFormalCharge());
			enlarge(tmp, 2);
			file << tmp;

			tmp = String(at_nr);
			enlarge(tmp, 6);
			file << tmp;

			tmp = String(atom.getName());
			enlarge(tmp, 2);
			enlarge(tmp, 3);
			file << tmp;

			file << "    XXX";

			sprintf(buf, "%.4f", atom.getCharge());
			tmp = buf;
			enlarge(tmp, 11);
			file << tmp;
			
			file << "      MMFF" << std::endl;
			

			map[&atom] = at_nr;
			at_nr++;
		}

		Size max_bond = system.countBonds();
		Size bond_nr = 0;
		AtomBondIterator abit;
		BALL_FOREACH_BOND(system, ait, abit)
		{
			Atom* first = (Atom*)abit->getFirstAtom();
			Atom* second = (Atom*)abit->getSecondAtom();
			Size order = abit->getOrder();

			tmp = map[first];
			enlarge(tmp, 5);
			file << tmp;

			tmp = map[second];
			enlarge(tmp, 6);
			file << tmp;

			tmp = String(order);
			enlarge(tmp, 3);
			file << tmp;

			bond_nr ++;
			if (bond_nr % 5 == 0) 
			{
				file << std::endl;
			}
			else
			{
				if (bond_nr < max_bond) file << "  ";
			}
		}
		file.close();


		// Display about dialog
		QDialog w;
 		Ui_AboutDialog about;
		about.setupUi(&w);
		QString version = QString("QT ") + qVersion();
#ifdef BALL_QT_HAS_THREADS
		version += "(mt)";
#endif
		about.qt_version_label->setText(version);
		QFont font = about.BALLView_version_label->font();
		about.BALLView_version_label->setText(QString("BALLView ") + BALL_RELEASE_STRING);
		font.setPixelSize(18);
		about.BALLView_version_label->setFont(font);
		about.BALL_version_label->setText(__DATE__);
		w.exec(); 
	}

