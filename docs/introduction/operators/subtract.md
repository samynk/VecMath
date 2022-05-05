# Subtract

A scalar (a in the example) will be promoted to a 2d vector:

```cpp
vecmath> a = 7.0
vecmath> b = [3.1,4.1]
vecmath> c = a - b
vecmath> printAll
```

The result is :

```cpp
a = [7]
b = [3.1,4.1]
c = [3.9,2.9]
```

Subtracting a 3d vector from a 2d vector leads to a different type of promotion of the 2d vector:

```cpp
vecmath> a=[-2.3,4]
vecmath> b=[5,6,7]
vecmath> c=a-b
vecmath> printAll
```

The result is :

```cpp
a = [-2.3,4]
b = [5,6,7]
c = [-7.3,-2,-7]
```

It is possible to use a quaternion to subtract from a 1d,2d or 3d vector but this has no special meaning
so it is not discussed here.