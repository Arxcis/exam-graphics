#!/usr/local/bin/python3
# -*- coding:utf-8 -*-

import argparse
from subprocess import call
import os
import platform
import sys
import random
import math

from PIL import Image


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

    def __str__(self):
        return "x:{} y:{} z:{}".format(self.x,self.y,self.z)

class Color:
    __slot__ = ["r", "g", "b", "a"]

    def __init__(self, r,g,b,a):
        self.r = r
        self.g = g
        self.b = b
        self.a = a

class Vertex:
    __slot__ = ["position", "normal", "texcoord", "color"]

    def __init__(self,position,normal=0,texcoord=0,color=0 ):
        self.position = position
        self.normal = normal
        self.texcoord = texcoord
        self.color = color

    def __str__(self):
        return "pos:{}".format(self.position)

class Triangle:
    __slot__ = ["a", "b", "c", "normal"]

    def __init__(self, a,b,c, normal):
        self.a = a
        self.b = b
        self.c = c
        self.normal = normal


vertices  = []
indicies  = []
triangles = []


if __name__ == "__main__":

    in_heightmap_path = "resources/ExamResources/heightmap/height100.png"

    try:
        im = Image.open(in_heightmap_path)
    except IOError:
        print("cannot convert", infile)
        exit(1)



    pixelbuffer = im.getdata()
    width, height = im.size
    print("width:", width, "height:", height)
    
    print(len(pixelbuffer))

    # Iterating all pixels
    count = 0

    for z in range(height):
        for x in range(width):

            y = pixelbuffer[x + z * width]

            vertices.append(
                Vertex(position=Vec3(x,y,z)))


    for v in vertices:

        print(v)

