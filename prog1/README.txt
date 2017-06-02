Author: James Ly

It looks like the box intersections are working. I am still working on
the BVH tree. I can construct the BVH tree. I am still working on traversing
and finding intersections using the tree. You can bvh tree code in BVHNode.cpp


Usage: raytrace render <input_filename> <width> <height>
       raytrace sceneinfo <input_filename>
       raytrace pixelray <input_filename> <width> <height> <x> <y>
       raytrace firsthit <input_filename> <width> <height> <x> <y>
       raytrace pixelcolor <input_filename> <width> <height> <x> <y> [-altbrdf]
       raytrace pixeltrace <input_filename> <width> <height> <x> <y> [-altbrdf]
       raytrace printrays <input_filename> <width> <height> <x> <y> [-altbrdf]
