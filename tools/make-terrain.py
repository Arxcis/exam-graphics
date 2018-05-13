#!/usr/local/bin/python3
# -*- coding:utf-8 -*-

import argparse
from subprocess import call
import os
import platform
import sys
import random
import math

import numpy as np


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

    def __init__(self,position,
                      normal  = Vec3(0,0,0),
                      texcoord = Texcoord(0,0),
                      color    = Color(0,0,0,0)):

        self.position = position
        self.normal = normal
        self.texcoord = texcoord
        self.color = color

    def __str__(self):
        return "pos:{}".format(self.position)

class Triangle:
    __slot__ = ["a", "b", "c", "normal"]

    def __init__(self, a,b,c, normal=Vec3(0,0,0)):
        self.a = a
        self.b = b
        self.c = c
        self.normal = normal






def make_height_vertices(vertices, pixelbuffer, width, height):

    for z in range(height):
        for x in range(width):

            y = pixelbuffer[x + z * width]

            vertices.append(
                Vertex(position=Vec3(x=x/float(width), 
                                     y=y/255.0,
                                     z=z/float(height)),

                       texcoord=Texcoord(
                                     u=x/float(width), 
                                     v=z/float(height))))



def make_pixelbuffer(image):

    pixelbuffer = image.getdata()
    width, height = image.size

    return pixelbuffer, width, height


def cross(v1, v2):
    return Vec3(v1.y*v2.z - v1.z*v2.y,
                v1.z*v2.x - v1.x*v2.z,
                v1.x*v2.y - v1.y*v2.x)

def lenvec3(v):
    return (v.x*v.x + v.y*v.y + v.z*v.z)**0.5

def normalize(v):
    length = lenvec3(v)
    return Vec3(v.x/length, v.y/length, v.z/length)


def subvec3(v1,v2):
    return Vec3(v1.position.x - v2.position.x,
                v1.position.y - v2.position.y,
                v1.position.z - v2.position.z)


def make_vertex_normals(vertices, columns, rows, meshoffset=0):


    for y in range(rows):
        for x in range(columns):

            

            if x == 0:       
                left  = (1)       + y * columns
            else:
                left  = (x-1)     + y * columns

            
            if x == columns-1: 
                right = (columns-2) + y * columns
            else: 
                right = (x+1)     + y * columns



            if y == 0:
                up =     x + 1     * columns
            else:
                up =     x + (y-1) * columns



            if y == rows-1:
                down =   x + (rows-2) * columns
            else:
                down =   x + (y+1)    * columns

            center = x + y * columns


            left_to_right_vector = Vec3( 
                x = 2/columns, 
                y = vertices[right].position.y - vertices[left].position.y, 
                z = 0
            )

            up_to_down_vector = Vec3(
                x = 0, 
                y = vertices[down].position.y - vertices[up].position.y,  
                z = 2/rows
            )


            crossproduct = cross(up_to_down_vector, left_to_right_vector)
            normalized_crossproduct = normalize(crossproduct)

            vertices[center].normal = normalized_crossproduct





def make_triangles_plus_normals(vertices, columns, rows, meshoffset=0):


    triangles = []
    for y in range(rows-1):
        for x in range(columns-1):

            # ____
            # |  /
            # | /
            # |/
            a = (y+1) * columns + x + 0 + meshoffset
            b = (y+0) * columns + x + 1 + meshoffset
            c = (y+0) * columns + x + 0 + meshoffset

            triangles.append(Triangle(a,b,c))
            """
                        # normal - set up cross product
                        v1 = subvec3(vertices[b], vertices[a])
                        v2 = subvec3(vertices[c], vertices[a])
                        
                        crossp = cross(v1, v2)
                        normal = normalize(crossp)
                        vertices[a].normal = normal
                        vertices[b].normal = normal
                        vertices[c].normal = normal
            """
            # 
            #    /|
            #   / |
            #  /__|
            # 
            a = (y+1) * columns + x + 0 + meshoffset
            b = (y+1) * columns + x + 1 + meshoffset
            c = (y+0) * columns + x + 1 + meshoffset

            triangles.append(Triangle(a,b,c))
            """
                        # normal - set up cross product
                        v1 = subvec3(vertices[b], vertices[a])
                        v2 = subvec3(vertices[c], vertices[a])
                        
                        crossp = cross(v1, v2)
                        normal = normalize(crossp)
                        vertices[a].normal = normal
                        vertices[b].normal = normal
                        vertices[c].normal = normal
            """

    return triangles







def write_vertices(outfile, vertices):
    # write vertices
    outfile.write("vertices: {}\n".format(len(vertices)))
    for vert in vertices:
        outfile.write("v: {:9.6f} {:9.6f} {:9.6f}   {:6.3f} {:6.3f} {:6.3f}   {:6.3f} {:6.3f}   {:3} {:3} {:3} {:3}\n".format(
                        vert.position.x,
                        vert.position.y,
                        vert.position.z,
                        vert.normal.x,
                        vert.normal.y,
                        vert.normal.z,
                        vert.texcoord.u,
                        vert.texcoord.v, # remember to flip y axis
                        vert.color.r,
                        vert.color.g,
                        vert.color.b,
                        vert.color.a))

# Example:
"""
triangles: 5566
t: 1 2 3
t: 2 3 1
t: 6 5 4
.......
"""
def write_mesh(outfile, triangles, name):
    outfile.write("mesh: {}\n".format(name))
    outfile.write("material: {}\n".format(name))
    outfile.write("shader: {}\n".format(name))
    outfile.write("triangles: {}\n".format(len(triangles)))
    
    for tri in triangles:
        outfile.write("t: {} {} {}\n".format(
            tri.a,
            tri.b,
            tri.c
        ))





def make_water_vertices(vertices, columns, rows, startheight, alpha):

    for z in range(rows):
        for x in range(columns):
            vertices.append(
                Vertex(position=Vec3(x = x/float(columns), 
                                     y = startheight, 
                                     z = z/float(rows)),

                       texcoord=Texcoord(u = x/float(columns),
                                         v = z/float(rows)),

                       color=Color(r = 0,
                                   g = 0,
                                   b = 255, 
                                   a = alpha)))




if __name__ == "__main__":


    in_heightmap_path = "resources/ExamResources/heightmap/height100.png"
    in_assets_path = "assets"

    try:
        image = Image.open(in_heightmap_path)
    except IOError:
        print("cannot convert", infile)
        exit(1)


    vertices  = []

    #
    # Generate base terrain
    #
    pixelbuffer, width, height = make_pixelbuffer(image)

    make_height_vertices(vertices, pixelbuffer, width, height)

    make_vertex_normals(vertices=vertices, columns=width, rows=height)

    base_triangles = make_triangles_plus_normals(vertices=vertices,
                                                 columns=width, 
                                                 rows=height)

    #
    # Generate water
    # 
    meshoffset = len(vertices)
    make_water_vertices(vertices=vertices, 
                        columns=100, 
                        rows=100, 
                        startheight=0.1,
                        alpha=100)

    water_triangle = make_triangles_plus_normals(vertices=vertices, 
                                         columns=100, 
                                         rows=100, 
                                         meshoffset=meshoffset)



    #
    # Write to file
    #
    with open(in_assets_path + "/models/terrain.yml", "w") as terrainfile:

        write_vertices(terrainfile, vertices)
        terrainfile.write("meshes: 2\n")
        write_mesh(terrainfile, base_triangles, "terrain")
        write_mesh(terrainfile, water_triangle, "water")
