#!/opt/gnu/bin/python
from string import *

infile=open("aliases.list")
lines=infile.readlines();


amber_to_pdb=[]
discover_to_pdb=[]

number_of_residues = len(lines) / 5
for i in range(number_of_residues):
	name = strip(lines[5 * i])
	pdb=split(strip(lines[5 * i + 1]), ' ')	
	amber=split(strip(lines[5 * i + 2]), ' ')
	discover=split(strip(lines[5 * i + 3]), ' ')
	number=atoi(pdb[2])
	for j in range(number):
		amber_to_pdb = amber_to_pdb + [ '('+name+':'+amber[3+j]+' '+name+':'+pdb[3+j]+')' ]
		discover_to_pdb = discover_to_pdb + [ '('+name+':'+discover[3+j]+' '+name+':'+pdb[3+j]+')' ]


print '(Names'
print '  (Discover PDB'
for i in discover_to_pdb:
	print '    '+i
print '  )'
print '  (Amber PDB'
for i in amber_to_pdb:
	print '    '+i
print '  )'

	
