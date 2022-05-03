# Dot operator

The dot operator has two different meanings:

1. For 2D and 3D vector this operator corresponds to the dot product as you would expect.
2. For a quaternion this operator corresponds to the quaternion multiplication.

## 2D and 3D dot product

```cpp
vecmath> a=[7,-9]
vecmath> b=[2,3]
vecmath> dab=a.b
vecmath> print dab
```

The result of this operator is off course a scalar:

```cpp
dab = [-13]
```

A 3D example:

```cpp
vecmath> a=[7,-9,1]
vecmath> b=[2,3,-3]
vecmath> dab=a.b
vecmath> print dab
```

The result of this operator is again a scalar:

```cpp
dab = [-16]
```

## Quaternion multiplication

The dot operator can be used to perform the quaternion complex multiplication and off course also to
calculate a rotation with the sandwich formula. The result of this operator on quaternions is a new
quaternion:

```cpp
vecmath> q1=[1,(-9,1,2)]
vecmath> q2=[3,(2,-3,4)]
vecmath> q3=q1.q2
vecmath> print q3
```

The resulting quaternion is :

```cpp
q3 = [16 ,( -15 , 40 , 35 )]
```