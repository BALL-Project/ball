# value for isosurface
start_value = -0.3
end_value   = 0.3
dist = 0.025

# colors:
c1 = ColorRGBA(1.,0,0)
c2 = ColorRGBA(1.,1.,1.)
c3 = ColorRGBA(0,0,1.)

# create a color map:
l = []
l.append(c1)
l.append(c2)
l.append(c3)
cm = ColorMap()
cm.setBaseColors(l)
cm.setNumberOfColors(100)
cm.setRange(start_value,end_value)
cm.createMap()

setMultithreading(false)

rc = RegularData3DController.getInstance(0)
sets = rc.getDatasets()
if len(sets) == 0:
	exit

grid = sets[0]

value = start_value
while value < end_value:
	color = cm.map(value)
	string = String(value)
	if (value >= 0):
		string = String("+") + string
	string.truncate(5)
	getScene().showText(string)
	if (rc.computeIsoContourSurface(grid, color, value) != 0):
		rep = getRepresentations()[len(getRepresentations()) - 1]
		rep.setTransparency(50)
		getMainControl().update(rep)
		getScene().exportPNG()
		getMainControl().remove(rep)
	value += dist

setMultithreading(true)
