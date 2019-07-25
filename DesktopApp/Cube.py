from Vertex import Vertex
from OpenGL.GL import *


class Cube:
    def __init__(self):
        self.__vertices = (
            Vertex(1, -1, -1),
            Vertex(1, 1, -1),
            Vertex(-1, 1, -1),
            Vertex(-1, -1, -1),
            Vertex(1, -1, 1),
            Vertex(1, 1, 1),
            Vertex(-1, -1, 1),
            Vertex(-1, 1, 1)
        )
        self.__edges = (
                (0, 1),
                (0, 3),
                (0, 4),
                (2, 1),
                (2, 3),
                (2, 7),
                (6, 3),
                (6, 4),
                (6, 7),
                (5, 1),
                (5, 4),
                (5, 7)
        )

    def get_vertices(self):
        return self.__vertices

    def set_vertices(self, v):
        self.__vertices = v

    def __str__(self):
        s = ""
        for v in self.__vertices:
            s += str(v)+"\n"
        return s

    def draw_cube(self):
        glBegin(GL_LINES)
        for edge in self.__edges:
            for vertex in edge:
                glVertex3fv(self.__vertices[vertex].get_coordinates())
        glEnd()


