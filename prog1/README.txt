Author: James Ly

Global Illumination somewhat working. I am getting some green and red on the simple-gi
but it only looks like it went through one bounce?
All previous requirements working except for SDS.


Usage: raytrace render <input_filename> <width> <height> [-fresnel] [-ss=N] [-gi]
       raytrace sceneinfo <input_filename>
       raytrace pixelray <input_filename> <width> <height> <x> <y>
       raytrace firsthit <input_filename> <width> <height> <x> <y>
       raytrace pixelcolor <input_filename> <width> <height> <x> <y> [-altbrdf]
       raytrace pixeltrace <input_filename> <width> <height> <x> <y> [-altbrdf]
       raytrace printrays <input_filename> <width> <height> <x> <y> [-altbrdf]
