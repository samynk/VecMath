# Add

A scalar (a in the example) will be promoted to a 2d vector:

```cpp
vecmath> a = 7.0
vecmath> b = [3.1,4.1]
vecmath> c = a + b
vecmath> printAll
```

The result is :

```cpp
a = [7]
b = [3.1,4.1]
c = [10.1,11.1]
```

Adding a 2d vector to a 3d vector leads to a different type of promotion of the 2d vector:

```cpp
vecmath> a = [4.2,8.3,-5]
vecmath> b = [3.1,4.1]
vecmath> c = a + b
vecmath> printAll
```

The result is :

```cpp
a = [4.2,8.3,-5]
b = [3.1,4.1]
c = [7.3,12.4,-5]
```

It is possible to use a quaternion to a 1d,2d or 3d vector but this has no special meaning so it is not
discussed here.