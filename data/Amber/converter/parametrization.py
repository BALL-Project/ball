class Atom:
	pass

class Torsion:
	def __init__(self):
		self.params = []

class HBond:
	pass

class ResidueParams:
	def __init__(self):
		self.atoms = {}
		self.impropers = []

	def write(self, output):
		self.__writeImpropers(output)
		self.__writeCharges(output)

	def getAtom(self, name):
		if not self.atoms.has_key(name):
			self.atoms[name] = Atom()
		return self.atoms[name]

	def __writeImpropers(self, output):
		output.write("[ResidueImproperTorsions]\n")
		output.write("key:name\n")
		output.write(";\n;\n;    res:atom\n;  -------------\n")

		for i in self.impropers:
			output.write("   " + i + "\n")

		output.write("\n")

	def __writeCharges(self, output):
		output.write("[ChargesAndTypeNames]\n")
		output.write("ver:version key:name value:q value:type\n")
		output.write("@unit_q=e0\n")

		for k, v in self.atoms.iteritems():
			output.write("   1.0 %s     %3.5f %s\n" % (k, v.charge, v.typename))
		output.write("\n")

class Parametrization:
	atoms = {}
	torsions = {}
	improper_torsions = {}
	bond_stretch = {}
	angle_bends = {}
	hydrophilic_atoms = []
	h_bonds = []
	lj_potential = {}

	def getAtom(self, name):
		if not self.atoms.has_key(name):
			self.atoms[name] = Atom()
		return self.atoms[name]

	def getTorsion(self, partners):
		if not self.torsions.has_key(partners):
			self.torsions[partners] = Torsion()
		return self.torsions[partners]

	def getImproperTorsion(self, partners):
		if not self.improper_torsions.has_key(partners):
			self.improper_torsions[partners] = Torsion()
		return self.improper_torsions[partners]

	def write(self, outFile):
		self.__writeAtomTypes(outFile)
		self.__writeQuadraticBondStretch(outFile)
		self.__writeTorsions(outFile)
		self.__writeAngleBend(outFile)
		self.__writeHydrophilicAtoms(outFile)
		self.__writeLennardJones(outFile)

	def __writeAtomTypes(self, output):
		output.write("[AtomTypes]\n")
		output.write("value:ver key:type value:mass value:comment\n")
		output.write("@unit=g/mol")
		output.write(";\n;\n;   Rev Type    mass      comment\n")
		output.write(";   --- ---- ---------- ----------------------------------\n")

		for k, v in self.atoms.iteritems():
			output.write("    1.0  %-2s  %4.5f   \"\"\n" % (k, v.mass))

		output.write("\n")

	def __writeQuadraticBondStretch(self, output):
		output.write("[QuadraticBondStretch]\n")
		output.write("ver:version key:I key:J value:k value:r0 value:comment\n")
		output.write("@unit_k=kcal/mol\n")
		output.write("@unit_r0=Angstrom\n")
		output.write(";\n;\n;  Rev  I   J       k         r0      comment\n")
		output.write(";  --- --- --- ---------- ---------- -----------------------\n")

		for k, v in self.bond_stretch.iteritems():
			output.write("   1.0  %2s  %2s    " % k)
			output.write("%.3f  %.5f    \"\"\n" % v)

		output.write("\n")

	def __writeTorsions(self, output):
		output.write("[Torsions]\n")
		output.write("ver:version key:I key:J key:K key:L key:n value:div value:V value:phi0 value:f value:comment\n")
		output.write("@unit_V=kcal/mol\n")
		output.write("@unit_phi0=degree\n")
		output.write("@unit_div=1\n")
		output.write("@unit_f=1\n")
		output.write(";\n; energy is calculated as follows:\n;\n")
		output.write(";   E = (V / div) * (1 + cos(f * phi + phi0))\n;\n;\n")
		output.write(";  Rev  I   J   K   L   n  div     V         phi0     f    comment\n")
		output.write(";  --- --- --- --- --- --- --- ---------- ---------- --- -----------------------\n")

		for k, v in self.torsions.iteritems():
			output.write("   1.0  %-2s  %-2s  %-2s  %-2s  " % k)
			output.write("N  %d  %f  %f  %d \"\"\n" % (len(v.params), 0, 0, 0) )
			t = 1
			for i in v.params:
				output.write("   1.0  %-2s  %-2s  %-2s  %-2s  " % k)
				output.write("%d  " % t)
				output.write("%d  %f  %f  %d   \"\"\n" % i)
				t += 1
		output.write("\n")

	def __writeAngleBend(self, output):
		output.write("[QuadraticAngleBend]\n")
		output.write("ver:version key:I key:J key:K value:k value:theta0 value:comment\n")
		output.write("@unit_k=kcal/mol\n")
		output.write("@unit_theta0=degree\n")
		output.write(";\n;\n;  Rev  I   J   K       k         r0      comment\n")
		output.write(";  --- --- --- --- ---------- ---------- -----------------------\n")

		for k, v in self.angle_bends.iteritems():
			output.write("   1.0  %2s %2s %2s    " % k )
			output.write("%.5f  %.5f   \"\"\n"     % v )

		output.write("\n")

	def __writeHydrophilicAtoms(self, output):
		output.write("[HydrophilicTypes]\n")
		output.write(reduce(lambda x, y: x + " " + y, self.hydrophilic_atoms))
		output.write("\n\n")

	def __writeHydrogenBonds(self, output):
		output.write("[HydrogenBonds]\n")
		output.write("ver:version key:I key:J value:A value:B")
		output.write("@unit_A=kcal/mol*A^12")
		output.write("@unit_B=kcal/mol*A^10")
		output.write(";\n;\n;  Rev  I   J      A          B")
		output.write(";  --- --- --- ---------- ----------")
		for i in h_bonds:
			output.write("   1.0  %s %s     %f     %f" % (i.i, i.j, i.a, i.b))

	def __writeLennardJones(self, output):
		output.write("[LennardJones]\n")
		output.write("ver:version key:I value:R value:epsilon\n")
		output.write("@unit_R=Angstrom\n")
		output.write("@unit_epsilon=kcal/mol\n")
		output.write("@format=" + self.lj_format+"\n")
		output.write(";\n;\n;  Rev  I      R       epsilon    comment\n")
		output.write(";  --- --- ---------- ---------- -------------------------\n")

		#Currently this handles only one of the known amber formats, namely RE
		for k, v in self.lj_potential.iteritems():
			output.write("   1.0 %s" % k)
			output.write("    %f   %f  \"\"\n" % v)
		output.write("\n")

