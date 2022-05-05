# Variables

For now, only floating point values are supported in the VecMath shell. Vector, matrix and quaternion
types can be entered with a syntax that is similar to C/C++.

For now there are only four types of variables in the vecmathtool:

- Scalar: This variable is a single float value. This type can also be entered as an array with one
element. This is off course perfectly valid because you can see scalars as one dimensional vectors.

- 2D vector: This variable must be entered as an array with two elements.

- 3D vector: This variable must be entered as an array with three elements.

- Quaternion: This type is again an array, but the complex parts have to be defined between parentheses

## Scalar

A scalar is a simple float which can be assigned to a variable with the equals (=) operator.
To enter the example type in the code line per line, and press enter for each line.
The print command prints out the the contents of a variable.

```cpp
vecmath> a = 7.0
vecmath> print a
```

The result is :
```cpp 
a = [7.0]
```

A scalar can also be defined as an array with a single element:

```cpp
vecmath> b = [9.3]
vecmath> print b
```

The result is :
```cpp
b = [9.3]
```

## 2D Vector

A 2d vector is defined as an array of two floats (or two operations that result in a float). The REPL
shell does not make a distinction between points (place vectors) or vectors. The user (you) has to make
that distinction theirself:

```cpp
vecmath> v = [7.0,2.3]
vecmath> print v
```

The result is :
```cpp
v = [7.0,2.3]
```

## 3D Vector
A 3d vector is defined as an array of two floats (or two operations that result in a float).
Again, the REPL shell does not make a distinction between points (place vectors) or vectors. The
user (you) has to make that distinction theirself:

```cpp
vecmath> v = [7.0,2.3,5.0]
vecmath> print v
```
The result is :

```cpp
v = [7,2.3,5]
```

## Quaternions

To create a quaternion, the following syntax is used:

```cpp
vecmath> q1=[1,(2,3,4)]
vecmath> print q1
```

The output is:

```cpp
vecmath>q1 = [1 ,( 2 , 3 , 4 )]
```


#### This corresponds to the quaternion: 1 + <span style="color:red">2i</span> + <span style="color:green">3j</span> + <span style="color:blue">4k</span>