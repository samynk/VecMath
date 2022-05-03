# Multiply

The value of the scalar (a in the example) will be multiplied with every component of the 2D vector (b
in the example):

```cpp
vecmath> a=3.0
vecmath> b=[3.1,4.1]
vecmath> c=a*b
vecmath> printAll
```

The result is :

```cpp
a = [3]
b = [3.1,4.1]
c = [9.3,12.3]
```

Multiplying a 2d vector to a 3d vector means that the z-component of the 2d vector will be set to
zero, as a 2d vector can be seen to be a 3d vector on the XY-plane. Remark that multiplication here is
not the same as the dot product!

```cpp
vecmath> a = [4.2,8.3,-5]
vecmath> b = [3.1,4.1]
vecmath> c = a * b
vecmath> printAll
```

The result is :

```cpp
a = [4.2,8.3,-5]
b = [3.1,4.1]
c = [13.02,34.03,-0]
```

The multiplication of two quaternions could be used to more easily implement the axis-angle formula,by storing the axis information in one quaternion, and the pure rotation in the other quaternion:

```cpp
vecmath> qaxis= [1,(0,0,1)]
vecmath> qangle = [0.707,(0.707,0.707,0.707]]
vecmath> q = qangle*qaxis
vecmath> print q
```

The result is :

```cpp
q = [0.707 ,( 0 , 0 , 0.707 )]
```

:::danger
Be very careful with the multiplication. Within the vecmath REPL shell this is always a
component wise multiplication and not to be confused with the dot product or complex
multiplication.
:::