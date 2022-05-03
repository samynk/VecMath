# Magnitude

The magnitude operation is defined for 1D, 2D, 3D vectors and quaternions:

```cpp
vecmath> a = 5.0
vecmath> ma = |a|
vecmath> print ma
```

The (logical) output is:

```cpp
ma = [5]
```

For a 2d vector:

```cpp
vecmath> b = [5,12]
vecmath> mb = |b|
vecmath> print mb
```

the output is:

```cpp
vecmath> mb = [13]
```

For a 3d vector:

```cpp
vecmath> c = [3,8,-2]
vecmath> mc = |c|
vecmath> print mc
```

the output is:

```cpp
vecmath> mc = [8.77496]
```

For a quaternion:

```cpp
vecmath> q = [3,(1,-2,4)]
vecmath> mq = |q|
vecmath> print mq
```

the output is:

```cpp
vecmath> mq = [5.47723]
```