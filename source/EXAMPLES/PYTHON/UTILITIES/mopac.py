# Example script to calculate the energy of the first System by calling Mopac.
S = getSystem(0)

text = "PM3 1SCF MMOK GEO-OK\n\n\n"
for i in atoms(S):
	e = i.getElement().getSymbol()
	p = i.getPosition()
	text += "%s %f 1 %f 1 %f 1\n" % (e, p.x, p.y, p.z)

open("tmp.dat", 'w').write(text)

os.system("run_mopac tmp")
print os.popen('grep "TOTAL ENER" tmp.out').read() 
