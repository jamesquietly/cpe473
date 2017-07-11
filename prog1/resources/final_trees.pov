// cpe473, triangle (RIGHT HANDED)

camera {
  location  <0, 0, 7>
  up        <0,  1,  0>
  right     <1.33333, 0,  0>
  look_at   <0, 0, 0>
}

light_source {<-10, 10, 10> color rgb <1.5, 1.5, 1.5>}

//left tree
triangle {
  <-.001,1, 0 >,
  <-.001, -.3,3 >,
  <1,-.3, 0 >
  pigment {color rgb <0.1, 0.8, 0.4>}
  finish {ambient 0.3 diffuse 0.4}
  scale <1.5, 4, 1.5>
  rotate <0, 21, 0>
  translate <-4.2, 0, -5>
}

triangle {
  <0,1, 0 >,
  <-1, -.3,0 >,
  <0,-.3, 3 >
  pigment {color rgb <0.1, 0.8, 0.4>}
  finish {ambient 0.3 diffuse 0.4}
  scale <1.5, 4, 1.5>
  rotate <0, 21, 0>
  translate <-4.2, 0, -5>
}

box { <-1, -5, -1>, <0, 0, 0>
  pigment { color rgb <0.65, 0.4, 0.4>}
  finish {ambient 0.2 diffuse 0.8}
  translate <-3, 0, -2>
}

//right tree
triangle {
  <-.001,1, 0 >,
  <-.001, -.3,3 >,
  <1,-.3, 0 >
  pigment {color rgb <0.1, 0.8, 0.4>}
  finish {ambient 0.3 diffuse 0.4}
  scale <1.5, 4, 1.5>
  rotate <0, -10, 0>
  translate <3.5, 0, -5>
}

triangle {
  <0,1, 0 >,
  <-1, -.3,0 >,
  <0,-.3, 3 >
  pigment {color rgb <0.1, 0.8, 0.4>}
  finish {ambient 0.3 diffuse 0.4}
  scale <1.5, 4, 1.5>
  rotate <0, -10, 0>
  translate <3.5, 0, -5>
}

box { <-1, -5, -1>, <0, 0, 0>
  pigment { color rgb <0.65, 0.4, 0.4>}
  finish {ambient 0.2 diffuse 0.8}
  translate <3, 0, -2>
}


//house
triangle {
  <0,1, 0 >,
  <-1, -.3,0 >,
  <0,-.3, 3 >
  pigment {color rgb <0.9, 0.1, 0.1>}
  finish {ambient 0.3 diffuse 0.4}
  scale <6, 2, 1>
  rotate <0, 30, 0>
  translate <2, 0.5, -11>
}

box { <-3, -5, -2>, <3, 0, 2>
  pigment { color rgb <0.6, 0.4, 0.9>}
  finish {ambient 0.2 diffuse 0.8}
  translate <0, 0, -10>
}

plane {<0, 1, 0>, -4
  pigment {color rgb <0.2, 0.2, 0.8>}
  finish {ambient 0.3 diffuse 0.8}
}
