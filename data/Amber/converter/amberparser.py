import re
import BALL
from parametrization import *

class AmberParser:
	def __init__(self):
		self.LJLineParser = {
			'RE' : self.__parseLennardJonesRE,
			'AC' : self.__parseLennardJonesAC,
			'SK' : self.__parseLennardJonesSK
		}

	def parse(self, input):
		self.params = Parametrization()
		inFile = open(input)

		inFile.readline()

		self.__parseAtomTypes(inFile)
		self.__parseHydrophilicAtoms(inFile)
		self.__parseQuadraticBondStretch(inFile)
		self.__parseQuadraticAngleBend(inFile)
		self.__parseTorsions(inFile)
		self.__parseImproperTorsions(inFile)
		self.__parseHydrogenBonds(inFile)
		self.__parseEquivalencyLists(inFile)

		self.__parseLennardJones(inFile)

		return self.params


	def __readPartners(self, str, num):
		""" This method parses strings of the form: A - B - C ...
		    Where num is the number of parts seperated by '-' and
	      str is the number of input strings. It returns a pair
	      consisting of the splitted A, B, C and the remaining string
		"""
		parts = str.split("-", num - 1)
		next = parts[num - 1].strip().split(" ", 1)

		out = [ x.strip() for x in parts[:-1]]
		out.append(next[0].strip())

		return (out, next[1])

	def __parseAtomTypes(self, input):
		fields = input.readline().split()
		while fields != []:
			atom = self.params.getAtom(fields[0])
			atom.mass = float(fields[1])
			fields = input.readline().split()

	def __parseHydrophilicAtoms(self, input):
		self.params.hydrophilic_atoms += input.readline().split()

	def __parseQuadraticBondStretch(self, input):
		line = input.readline()
		while line.strip() != "":
			partners, str = self.__readPartners(line, 2)
			fields = str.split()
			self.params.bond_stretch[tuple(partners)] = tuple(map(float, fields[:2]))
			line = input.readline()

	def __parseQuadraticAngleBend(self, input):
		line = input.readline()
		while line.strip() != "":
			partners, str = self.__readPartners(line, 3)
			fields = str.split()
			self.params.angle_bends[tuple(partners)] = tuple(map(float, fields[:2]))
			line = input.readline()

	def __parseTorsions(self, input):
		line = input.readline()
		while line.strip() != "":
			partners, str = self.__readPartners(line, 4)
			partners = tuple([ x == "X" and "*" or x for x in partners ])
			fields = str.split()
			tors = self.params.getTorsion(partners)
			tors.params.append((int  (fields[0]),
                          float(fields[1]),
                          float(fields[2]),
                          int(float(fields[3]))
                        ))
			line = input.readline()

	def __parseImproperTorsions(self, input):
		line = input.readline()
		while line.strip():
			partners, str = self.__readPartners(line, 4)
			partners = tuple([ x == "X" and "*" or x for x in partners ])
			fields = str.split()
			tors = self.params.getImproperTorsion(partners)
			tors.params.append((float(fields[0]),
                          float(fields[1]),
                          int(float(fields[2]))
                        ))
			line = input.readline()

	def __parseHydrogenBonds(self, input):
		line = input.readline()
		while line.strip():
			self.params.h_bonds.append(HBond(line.split()[:4]))
			line = input.readline()

	def __parseEquivalencyLists(self, input):
		line = input.readline()
		while line.strip():
			fields = line.split()
			self.params.equivalency_lists[fields[0]] = fields[1:]
			line = input.readline()

	def __parseLennardJones(self, input):
		line = input.readline()
		#Store the format

		format = line.split()[1]

		self.params.lj_format = format

		parseLine = self.LJLineParser[format]
		line = input.readline();
		while line.strip():
			parseLine(line)
			line = input.readline()

	def __parseLennardJonesRE(self, line):
		line = line.split()
		self.params.lj_potential[line[0]] = (float(line[1]), float(line[2]))

	def __parseLennardJonesSK(self, line):
		pass

	def __parseLennardJonesAC(self, line):
		pass

class InParser:
	def __init__(self):
		self.params = ResidueParams()
		self.fragDB = BALL.FragmentDB("")
		self.rename = self.fragDB.getNamingStandard("Amber-PDB")

	def __lookup(self, res, name, suffix):
		try:
			res = str(self.rename[BALL.String(res + ":" + name)]).split(":")
			return res[0] + suffix + ":" +res[1]
		except KeyError:
			pass

		try:
			return res + suffix + ":" + str(self.rename[BALL.String("*:" + name)]).split(":")[1]
		except KeyError:
			pass

		try:
			return str(self.rename[BALL.String(res + ":*")]).split(":")[0]  + suffix + ":" + name
		except KeyError:
			return None

	def __parseAtoms(self, i, lines, residue, suffix):
		while i < len(lines) and lines[i].strip():
			fields = lines[i].split()
			name = fields[1]
			i += 1
			ball_name = self.__lookup(residue, name, suffix)

			if ball_name is None:
				print "Unknown atom: " + residue + suffix + ":" + name
				continue

			atom = self.params.getAtom(ball_name)
			atom.typename = fields[2]
			atom.charge = float(fields[10])

		return i + 1

	def __parseImpropers(self, i, lines, residue, suffix):
		while i < len(lines) and not lines[i].startswith("IMPROPER"):
			i += 1

		i += 1
		while i < len(lines) and lines[i].strip():
			name = lines[i].split()[2]
			i += 1

			ball_name = self.__lookup(residue, name, suffix)
			if ball_name is None:
				print "Unknown atom for IMPROPER torsions: " + residue + suffix + ":" + name
				continue

			self.params.impropers.append(ball_name)
		while i < len(lines) and not lines[i].startswith("DONE"):
			i+=1

		return i + 1

	def parse(self, files):
		for file in files:
			try:
				f = open(file)
			except:
				print "Could not open %s for reading" % file
				exit(-1)

			lines = f.readlines()

			mode = int(lines[0].split()[2])

			if mode == 100 or mode == 200:
				suffix = "-N"
			elif mode == 101 or mode == 201:
				suffix = "-C"
			else:
				suffix = ""

			i = 2

			while i < len(lines):
				if lines[i].startswith("STOP"):
					break
				resname = lines[i + 2].split()[0]
				i += 8
				i = self.__parseAtoms(i, lines, resname, suffix)
				i = self.__parseImpropers(i, lines, resname, suffix)

		return self.params

