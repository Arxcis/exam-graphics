
# Exam Graphics IMT 2531 Spring semester NTNU Gjøvik 2018

## General info
* candidate name: Jonas J. Solsvik
* guid: jonasjso
* email: jonasjso@stud.ntnu.no
* start: 12:00, 2018-05-11
* end:   12:00, 208-05-14


# Devlog

## Monday 2018-05-14

#### 10:27 - Rungholt day cycle

![sweeter](https://media.giphy.com/media/57WbJAYEyn3QPwmlgq/giphy.gif)


#### 10:20 - RUngholt gif

![sweet](https://media.giphy.com/media/2ifRiYjkBEORZzTrjX/giphy.gif)


#### 10:10 - Rungholt city model photoshoot

<a href="https://ibb.co/ijN15d"><img src="https://preview.ibb.co/j1XerJ/Screen_Shot_2018_05_14_at_09_57_50.png" alt="Screen_Shot_2018_05_14_at_09_57_50" border="0"></a>
<a href="https://ibb.co/kAC15d"><img src="https://preview.ibb.co/bveg5d/Screen_Shot_2018_05_14_at_10_01_50.png" alt="Screen_Shot_2018_05_14_at_10_01_50" border="0"></a>
<a href="https://ibb.co/djDkWJ"><img src="https://preview.ibb.co/gCYkWJ/Screen_Shot_2018_05_14_at_10_03_26.png" alt="Screen_Shot_2018_05_14_at_10_03_26" border="0"></a>


#### 06:58 - Clouds + moving water + water geometry + terrain zones + wobbling

![cool](https://media.giphy.com/media/A7WjobeQrbMLVkHlfU/giphy.gif)


#### 06:26 - Moving clouds

![moving-clouds](https://media.giphy.com/media/9S5mBWjvrTz1AxHRpx/giphy.gif)

#### 03:45 - Spinning propeller 

![propeller](https://media.giphy.com/media/u45hRBtVbehl87AOro/giphy.gif)


#### 02:20 - Wobbling water specularity

<a href="https://ibb.co/m1ZHdy"><img src="https://preview.ibb.co/jUziJy/Screen_Shot_2018_05_14_at_02_17_17.png" alt="Screen_Shot_2018_05_14_at_02_17_17" border="0"></a>


## Friday 2018-05-11


#### 18:26 - Rendering heightmap texture on terrain

<a href="https://ibb.co/dfHc8y"><img src="https://preview.ibb.co/eDsATy/Screen_Shot_2018_05_11_at_18_25_01.png" alt="Screen_Shot_2018_05_11_at_18_25_01" border="0"></a>

#### 16:29 - Finally some terrain data output

```shell
vertices: 506016
v:  0.000000 72.000000  0.000000    0.000  0.000  0.000    0.000  0.000     0   0   0   0
v:  1.000000 72.000000  0.000000    0.000  0.000  0.000    0.000  0.000     0   0   0   0
v:  2.000000 72.000000  0.000000    0.000  0.000  0.000    0.000  0.000     0   0   0   0
v:  3.000000 71.000000  0.000000    0.000  0.000  0.000    0.000  0.000     0   0   0   0
v:  4.000000 71.000000  0.000000    0.000  0.000  0.000    0.000  0.000     0   0   0   0
v:  5.000000 72.000000  0.000000    0
```

Good reference for exploring the Pillow image class
* http://effbot.org/imagingbook/image.htm

Using Pillow for Python to load and process the heightmap
* http://pillow.readthedocs.io/en/latest/handbook/tutorial.html#using-the-image-class



## Installation guide

```
$git clone <project-repo-url>
$cd <project-folder>

$python3 tools/make-terrain.py

$mkdir build
$cd build
$cmake .. -DCMAKE_BUILD_TYPE=Release
$make
```

## Run program
```
$ ./cube
$ ./cube <path-to-scene-file>
```


## Key bindings

```
//
//////////////////////////// KEY BINDINGS //////////////////////////
//
// 
// --------------------
//    General:
// --------------------
//  ESC: quit app
//
//
// -------------------
//   Time and weather:
//  -------------------
//   1: Pick spring
//   2: Pick Summer
//   3: Pick autumn
//   4: Pick Winter
//   5: Pause season change
//
//   6: Pick morning
//   7: Pick noon
//   8: Pick afternoon
//   9: Pick night
//   0: Pause daycycle
//
//   UP:   Increase water level
//   DOWN: Decrease water level
//
//
// ------------------------
//   Camera:
// ------------------------
//   i: forward
//   k: backward
//   l: right
//   j: left
//   y: up
//   h: down
//   space: toggl camera mode [FREELOOK | ORBITAL]
//   tab: cycle through cameras
//   mouse: look around, change orientation
// 
// -----------------------
//    Glider:
// -----------------------  
//    W: pitch down
//    S: pitch up
//    A: yaw left
//    D: yaw right 
//    F: Jump to either Lillehammer, Gjøvik or Hamar
//    R: Reset glider position
//   
//    comma:  speed down
//    period: speed up
//
//
// -------------------------------
//    Visualizations:
// -------------------------------
//   g: toggle grey scale shader
//   c: toggle countour lines shader
//
//
// --------------------------------------
//    Reload asset files:
// --------------------------------------
//   shift + 1 TO RELOAD Shaders
//   shift + 2 TO RELOAD Materials
//   shift + 3 TO RELOAD Models
//   shift + 4 TO RELOAD Scene + Config file
//
//
//////////////////////////////////////////////////////////////////////////////////////

```