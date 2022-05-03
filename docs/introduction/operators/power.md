# Power

The power operator raises all the components of a vector, quaternion to the given power:

```cpp
vecmath> a = [3.1,4.1]
vecmath> b = 3.0
vecmath> c = a ^ b
vecmath> print c
```

For the resulting vector, all the components have been raised to the third power:

```cpp
vecmath> c = [29.791,68.921]
```

The power operator also works on a scaler:

```cpp
a = 2
b = 3.0
c = a ^ b
print c
```

Which is off course the following result:

```cpp
c = [8]
```