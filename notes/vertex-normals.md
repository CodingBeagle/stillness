# Vertex Normals

Vertex normals are unit vectors associated with vertices of a 3D model.

They represent the direction the surface is facing at that vertex.

They are perpendicular to the surface at that point.

They are, amongst other things, used for lighting calculations.

## Vertex Normal Matrix

When transforming a vertex normal, it is important not to simply use the model matrix.

This is because:

- Normals are directions, not positions. They should not be translated.
- Non-uniform scaling on the model will distort the normals, making them no longer perpdendicular to the surface.
  - Rotations and shearing can also distort normals.
  - This would result in incorrect lighting calculations, if used for that.

Because of this, the vertex normal should be transformed using:
```glsl
mat3 normalMatrix = transpose(inverse(mat3(modelMatrix)));
```

The reason you first convert the 4x4 model matrix to a 3x3 matrix is because you remove the translation component (bottom row and righmost column).

This leaves us with the rotation, scaling, and shearing.

The inverse part counteracts non-uniform scaling effects and ensures normals remain perpendicular to the surfaces.

The inverse is used to correct for non-uniform scaling, and the transpose is used to convert from a 4x4 matrix to a 3x3 matrix.