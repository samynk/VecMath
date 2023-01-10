# Functions
Functions are used in a syntax that resembles C++, but with one important distinction that the
function is applied to all the components in a vector or quaternion.

- sin, cos and tan: The sine, cosine and tangent function. These functions work with radians.
- sind, cosd and tand: The sine, cosine and tangent function. These functions work with degrees as input.
- asin,acos and atan: The arcsine, arc cosine and arc tangent functions. These formulas return an
angle in **radians**. Undefined behaviour when the variable is not in the range [-1,1].
- asind,acosd and atand: The arcsine, arc cosine and arc tangent functions. These formulas return an
angle in **degrees**. Undefined behaviour when the variable is not in the range [-1,1].

- sqrt: Calculates the square root of a variable.
- norm : calculate the length of a vector, alternative for the | | notation.
- abs : calculates the absolute value of all the components.
- con : calculates the conjugate of a quaternion or complex number.
- inv : calculates the inverse of a variable.
    For a scalar this is 1/x,
    for a 2d vector [1/x,1/y],
    for a complex number or quaternion this is the actual inverse.
- angle : analyzes the rotation angle of a quaternion, returns the result in radians.
- angled : anaylyzes the rotaiton angle of a quaternion, returns the result in degrees.
- axis : analyzes the rotation axis of a quaternion, returns the result as a 3D vector.
- rad2deg: Converts a variable from radians to degrees.
- deg2rad: Converts a variable from degrees to radians.

## Trigonometric functions

The trigonometric functions work on scalars, but also on 2D and 3D vectors and quaternions. In the
latter case the trigonometric function is applied to all the components in the vector or quaternion, and
in the case of a scalar, just the one value.
The constant Pi,PI or pi is off course defined to be the well known mathematical constant.

## cosine

```cpp
vecmath> a=[Pi/4,Pi/3,Pi/2]
vecmath> ca=cos(a)
vecmath> print ca
```

This results in the application of the cosine function on all the elements of the vector a:

```cpp
ca = [0.707107,0.5,-4.37114e-08]
```

## sine

```cpp
vecmath> a=[Pi/4,Pi/3,Pi/2]
vecmath> sa=sin(a)
vecmath> print sa
```

This results in the application of the cosine function on all the elements of the vector a:

```cpp
sa = [0.707107,0.866025,1]
```

## tangent

```cpp
vecmath> a=[Pi/4,Pi/3,Pi/2]
vecmath> ta=tan(a)
vecmath> print ta
```

This results in the application of the tangent function on all the elements of the vector a. Remark
the undefined behaviour for Pi/2 or 90 degrees.

```cpp
sa = [0.707107,0.866025,1]
```

The sine and cosine functions can now be used to create a quaternion which represents for example
the left part of the ’sandwich’ method:

```cpp
vecmath> q1=[cos(Pi/3),(0,0,sin(Pi/3))]
vecmath> print q1
```

With the following result:

```cpp
q1 = [0.5 ,( 0 , 0 , 0.866025 )]
```

## Arc functions

The arc functions again work on scalars, vectors and quaternions. The function rad2deg can be used to
convert the resulting radians values into degree, which are perhaps more intuitively understandable.

## Arc cosine

The illustrate the arc cosine, we use the well know formula to calculate the angle between two vectors:

```cpp
vecmath> v1=[1,2,3]
vecmath> v2=[4,5,6]
vecmath> angleRad= acos(v1.v2/(|v1|*|v2|))
vecmath> print angleRad
```

The result in radians is:

```cpp
angleRad = [0.225726]
```

To convert to degrees we use the rad2deg function:

```cpp
vecmath> angleDegrees = rad2deg(angleRad)
vecmath> print angleDegrees
```

With the following result:

```cpp
angleDegrees = [12.9332]
```
