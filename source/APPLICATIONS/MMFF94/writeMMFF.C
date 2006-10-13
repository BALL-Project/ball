
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
	sprintf(buf, "%.6f", atom.getPosition().x);
	tmp = buf;
	tmp.trimRight();
	enlarge(tmp, 10);
	file << tmp;

	sprintf(buf, "%.6f", atom.getPosition().y);
	tmp = buf;
	enlarge(tmp, 11);
	file << tmp;

	sprintf(buf, "%.6f", atom.getPosition().z);
	tmp = buf;
	enlarge(tmp, 11);
	file << tmp;

	tmp = String(atom.getElement().getAtomicNumber());
	enlarge(tmp, 6);
	file << tmp;

	tmp = String(atom.getElement().getAtomicNumber());
	enlarge(tmp, 3);
	file << tmp;

	tmp = String(atom.getFormalCharge());
	enlarge(tmp, 2);
	file << tmp;

	tmp = String(at_nr);
	enlarge(tmp, 6);
	file << tmp;

	tmp = String(at_nr);
	enlarge(tmp, 3);
	file << tmp;

	file << "    XXX";

	tmp = String("0.0000");
	enlarge(tmp, 12);
	file << tmp << std::endl;

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

