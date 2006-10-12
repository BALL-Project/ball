# value for isosurface
start_value = -0.6
end_value   = 0.6
dist = 0.1

# colors:
c1 = ColorRGBA(1.,0,0)
c2 = ColorRGBA(1.,1.,1.)
c3 = ColorRGBA(0,0,1.)

# create a color map:
cm = ColorMap()
l = []
l.append(c1)
l.append(c2)
l.append(c3)
cm.setNumberOfColors(100)
cm.setBaseColors(l)
cm.setRange(start_value,end_value)
cm.createMap()

steps = (end_value - start_value) / dist
cd = 1. / steps
setMultithreading(false)

if len(getDatasetControl().get3DGrids()) == 0:
	exit

grid = getDatasetControl().get3DGrids()[0]
current = start_value

run  = 0
while (current < end_value):
	value = start_value + run * dist
	color = cm.map(value)
	string = String(value)
	if (value >= 0):
		string = String("+") + string
	string.truncate(4)
	getScene().showText(string)
	log(String(value))
	if (getDatasetControl().computeIsoContourSurface(grid, color, value) != 0):
		getScene().exportPNG()
		l = len(getRepresentations()) - 1
		print getMainControl().remove(getRepresentations()[l])
	current += dist
	run += 1

setMultithreading(true)

