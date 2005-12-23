clearAll()
# open the PDB file in BALL/data/structures
# of course you could also use an absolute Path here...
openFile(Path().find("structures/bpti.pdb"))
dp = getDisplayProperties()
dp.selectModel(MODEL_VDW)
dp.createRepresentation(getSystems())
