s = Scene.getInstance(0)
s.exportPNG()
pov = POVRenderer("test.pov")
s.exportScene(pov)
