Author: James Ly

View final proj page at:
https://jamesquietly.github.io/cpe473/web/index.html

OR

in ../web/
images of soft shadows in ../web/images

1. What feature you chose and why
For the final project, I decided to do soft shadows because it sounded do-able and it would make the shadows look nicer instead of just pure darkness ambient color.


2. What resources you used to research and implement the feature
I found a power point slide from the University of North Carolina that had a pretty good explanation of soft shadows.
Link: https://www.cs.unc.edu/~dm/UNC/COMP236/LECTURES/SoftShadows.pdf

3. Explanation/description of your Software Design implementation
The way I implemented was adding on to my exisiting blinn phong shading function because that was where I was doing my shadow checks. The concept of soft shadows was sampling points around the light and averaging them. In order to sample points around the light, I treated the point light like a rectangle and sample the points in a grid like procedure, much like way that we did anti-aliasing. I can then add up the color from those samples and divide them by the square of the number of samples taken. I found that 10 samples worked out great.
You can find the implementation in Shading.cpp in the blinn_phong_soft_shadow function.

4. Refactor for better software design
For the refractoring portion, I chose to refactor my blinn phong shading function. More specifically, the shadow check part. For shadow checking, I had to check if there was an another object in between the current object we are looking at and the light. If the object was behind the light, we also should not cover it in shadow. In both those cases we need add the diffuse and specular color onto the ambient color. I orignally had two if statments that check for both. I moved the shadow check into the shadow_check function that returns a bool. Now the blinn_phong function is much shorter.
You can see the changes in Shading.cpp or this commit:
https://github.com/jamesquietly/cpe473/commit/47828773d44b2b894dc2dd4ff99a4997c0e2bf87



Usage: raytrace render <input_filename> <width> <height> [-fresnel] [-ss=N] [-gi] [-softshadow]
       raytrace sceneinfo <input_filename>
       raytrace pixelray <input_filename> <width> <height> <x> <y>
       raytrace firsthit <input_filename> <width> <height> <x> <y>
       raytrace pixelcolor <input_filename> <width> <height> <x> <y> [-altbrdf]
       raytrace pixeltrace <input_filename> <width> <height> <x> <y> [-altbrdf]
       raytrace printrays <input_filename> <width> <height> <x> <y> [-altbrdf]
