mc = MainControl.getInstance(0)
s = System()
s.setName("My little system");
m = Molecule()
o = Atom()
m.insert(o)
s.insert(m)
mc.insert(s)
