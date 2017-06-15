// csc 473 basic box
camera {
location  <0, 0, 14>
up        <0,  1,  0>
right     <1.5, 0,  0>
look_at   <0, 0, 0>}

light_source {<-10, 10, 10> color rgb <1.0, 1.0, 1.0>}

// floor
plane {<0, 1, 0>, -5
pigment {color rgb <0.2, 0.2, 0.8>}
finish {ambient 0.4 diffuse 0.2}
}

//legs
box { <-1, -5, -1>, <0, -3, 0>
  pigment { color rgb <1.0, 0.2, 1.0>}
  finish {ambient 0.2 diffuse 0.8}
  rotate <0, -45, 0>
  translate <4, 0, 0>
}

box { <-1, -5, -1>, <0, -3, 0>
  pigment { color rgb <1.0, 0.2, 1.0>}
  finish {ambient 0.2 diffuse 0.8}
  rotate <0, -45, 0>
  translate <2, 0, 0>
}

//body
box { <-1, -5, -1>, <0, -2, 0>
  pigment { color rgb <1.0, 0.2, 1.0>}
  finish {ambient 0.2 diffuse 0.8}
  rotate <0, -45, 0>
  translate <3, 2, 0>
}

//left arm
box { <-1, -5, -1>, <0, -2, 0>
  pigment { color rgb <1.0, 0.2, 1.0>}
  finish {ambient 0.2 diffuse 0.8}
  rotate <0, -45, 0>
  translate <4, 2, 0>
  rotate <-60, 0, 0>
}

//right arm
box { <-1, -5, -1>, <0, -2, 0>
  pigment { color rgb <1.0, 0.2, 1.0>}
  finish {ambient 0.2 diffuse 0.8}
  rotate <0, -45, 0>
  translate <2, 2, 0>
  rotate <-60, 0, 0>
}

//head
box { <-1, -5, -1>, <1, -3, 1>
  pigment { color rgb <1.0, 0.2, 1.0>}
  finish {ambient 0.2 diffuse 0.8}
  rotate <0, -20, 0>
  translate <3, 5, 0>
}

//building
box { <-2, -5, -2>, <2, 5, 2>
  pigment { color rgb <1.0, 0.2, 1.0>}
  finish {ambient 0.2 diffuse 0.8}
  rotate <0, -45, 0>
  translate <-3, 0, 0>
  rotate <20, 20, 0>
}