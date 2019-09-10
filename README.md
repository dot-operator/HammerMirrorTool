# HammerMirrorTool
## Mirrors objective-based levels in the Source engine (for symmetrical KOTH, 5cp in tf2)

Drag and drop a VMF file onto the executable to automatically mirror it across the X axis.
Use a "no_mirror" visgroup to prevent parts of the map from being mirrored.
  Mirrored entities will be renamed such that team names match the opposing side.
  Any game logic will be duplicated, and the relevant teams will be flipped or reassigned as necessary.
  
UVs of materials applied to brushes and displacements will automatically be flipped. Brush faces on overlays will be reassigned. IO will be retargetted where applicable.

To do:
- Mirror across an arbitrary axis
- Support arbitrary gamemode logic mirroring
