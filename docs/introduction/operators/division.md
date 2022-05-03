# Division

With the division operator we can already provide a nice example of normalizing a vector.

```cpp
vecmath> a=[7,24]
vecmath> ma=|a|
vecmath> print ma
```

The result of the length of the vector is:

```cpp
ma = [25]
```

We can now normalize by deviding the vector a with its length:

```cpp
vecmath> an=a/ma
vecmath> print an
```

Giving the result:

```cpp
an = [0.28,0.96]
```

The normalization can off course be done in one step:

```cpp
vecmath> a=[20,21]
vecmath> an=a/|a|
vecmath> print an
```

The same methodology can be used to normalize a 3D vector.
