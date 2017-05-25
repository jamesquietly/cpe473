Author: James Ly

Transformations is now working. I need to use object space ray for
shadows instead of world space ray (transformed by inverse matrix).
Everything like schlick's and anit aliasing seems to be working.
I also made a pov and rendered image: p4.pov and p4.png in resources folder


Usage: raytrace render <input_filename> <width> <height>
       raytrace sceneinfo <input_filename>
       raytrace pixelray <input_filename> <width> <height> <x> <y>
       raytrace firsthit <input_filename> <width> <height> <x> <y>
       raytrace pixelcolor <input_filename> <width> <height> <x> <y> [-altbrdf]
       raytrace pixeltrace <input_filename> <width> <height> <x> <y> [-altbrdf]
       raytrace printrays <input_filename> <width> <height> <x> <y> [-altbrdf]
