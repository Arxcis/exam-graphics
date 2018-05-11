
class Texcoord:
    __slot__ = ["u", "v"]

    def __init__(self, u,v):
        self.u = u
        self.v = v


class Vec3:
    __slot__ = ["x", "y", "z"]

    def __init__(self, x,y,z):
        self.x = x
        self.y = y
        self.z = z

class Color:
    __slot__ = ["r", "g", "b", "a"]

    def __init__(self, r,g,b,a):
        self.r = r
        self.g = g
        self.b = b
        self.a = a

class Vertex:
    __slot__ = ["position", "normal", "texcoord", "color"]

    def __init__(self,position,normal,texcoord,color):
        self.position = position
        self.normal = normal
        self.texcoord = texcoord
        self.color = color

class Triangle:
    __slot__ = ["a", "b", "c", "normal"]

    def __init__(self, a,b,c, normal):
        self.a = a
        self.b = b
        self.c = c
        self.normal = normal
