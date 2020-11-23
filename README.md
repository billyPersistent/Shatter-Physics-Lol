This is a sort of failed experiment.  I wanted to implement fast destruction physics by slicing meshes by planes.  I aimed to do this in the fragment shader by
discarding all fragments that are on the outer sides of any triangle-defined plane in the mesh.  However, this ended up being very slow and forced all "Shatter Meshes"
to be convex.  Read this code if you dare.
