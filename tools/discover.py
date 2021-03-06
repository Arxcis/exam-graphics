import sys
import glob
import os
import time


def printToPipe(event, collection, tag, extension):
    print(event, collection, tag, extension)
    sys.stdout.flush()

def scan_collection(collection, fileregex):

    filenames = glob.glob("assets/" + collection + "/"+ fileregex)
    for fname in filenames:

        fname = os.path.basename(fname)
        if "." not in fname:
            continue

        fname = os.path.basename(fname)
        printToPipe("discovered", 
             collection,
             ".".join(fname.split(".")[:-1]),
             ".".join(fname.split(".")[-1:]))



if __name__ == "__main__":
    scan_collection("textures", "*.jpg");
    scan_collection("textures", "*.png");
    scan_collection("shaders", "*.glsl");
    scan_collection("materials", "*.yml");
    scan_collection("models", "*.yml");
    scan_collection("scenes", "*.yml");
