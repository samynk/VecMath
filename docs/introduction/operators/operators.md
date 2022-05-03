# Operators

The set of operators is a bit bigger than the operators in C++, and one main difference is that the
XOR symbol (^) has been repurposed as the power operator.

All but one operator are so called Hadamard operators:
(https://en.wikipedia.org/wiki/Hadamard_product_(matrices)).

An important detail is that variables can be promoted to other types, for example when a scalar is
multiplied with a 2D vector, the x and y component of the 2D vector will be multiplied with the value
of the scalar.

In contrast, when a 2D vector is added to a 3D vector, the z-component of the 2D vector will be
considered to be zero.The operators are (in order of precedence):

- | | : calculates the magnitude of the variable inside.

- \* : multiplies two variables component wise.

- / : divides two variables by each other.

- . : a special product. For 1d, 2d and 3d vectors this is the sum of the products of the components.
For a quaternion, this is the complex multiplication of two quaternions.

- ^ : raises the first variable to the power of the second. The second variable is assumed the be a
scalar. (this might be subject to review later).

- \+ : adds two variables together component wise.

- \- : subtracts two variables from each other component wise, or negates all the components of one
variable.

Finally, parentheses can be used in the same way as in C++ to group operations together if the order
of operations must be different than the default operator precedence