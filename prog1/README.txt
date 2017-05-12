Author: James Ly

I wasn't able to get beer's law or the pixeltrace to work as the expected program.
However, I do still have both implemented. Beer's law is in the ReflectRefract.cpp.
The recursive raytrace is also in the the ReflectRefract.cpp
I seem to have reflection, refraction, and schlick's approximation working. 
Colors might be slightly brighter.


Usage: raytrace render <input_filename> <width> <height>
       raytrace sceneinfo <input_filename>
       raytrace pixelray <input_filename> <width> <height> <x> <y>
       raytrace firsthit <input_filename> <width> <height> <x> <y>
       raytrace pixelcolor <input_filename> <width> <height> <x> <y> [-altbrdf]
       raytrace pixeltrace <input_filename> <width> <height> <x> <y> [-altbrdf]
