import untangle
import cogapp
from typing import *
import py.render as render
import py.loader as loader

model = loader.SchemaLoader('test.xml')
render = render.Renderer(model.schema)
render.generate('./out')