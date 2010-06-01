# create a BS model from a HINFile through POVRay export
import BALL
from BALL import *
s = System()
HINFile("test.hin").read(s)
r = Representation()
r.setComposites([s])
r.setModelProcessor(AddBallAndStickModel())
r.setColorProcessor(ElementColorProcessor())
r.update(1)
camera = focusCamera(s)
stage = Stage()
stage.setCamera(camera)
stage.addLightSource(LightSource())
p = POVRenderer("test.pov")
p.init(stage, 1024, 800)
p.render(r)
p.finish()
