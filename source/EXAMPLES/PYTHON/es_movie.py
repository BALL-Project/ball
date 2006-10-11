# value for isosurface
start_value = -0.6
end_value   = 0.6
dist = 0.1
steps = (end_value - start_value) / dist
cd = 1. / steps
setMultithreading(false)

if len(getDatasetControl().get3DGrids()) == 0:
	exit

grid = getDatasetControl().get3DGrids()[0]
current = start_value

run  = 0
while (current < end_value):
	color = ColorRGBA(1. - run * cd, 0, run * cd)
	value = start_value + run * dist
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

