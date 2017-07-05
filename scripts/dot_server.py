#!/usr/bin/env python
import sys
from livereload import Server, shell
import pydot
server = Server()

def make_dots():
	for fname in glob.glob(sys.argv[1] + "/*.dot"):
		(graph,) = pydot.graph_from_dot_file(fname)
		graph.write_png(fname + ".png")

server.watch(sys.argv[1] + "/*.dot", make_dots)
server.serve(root=sys.argv[1])