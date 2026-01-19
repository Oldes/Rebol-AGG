Rebol [
    title: "AGG Path DSL commands specification"
    version: 0.1.0
]

move: [
    "Move path pen to absolute position"
    position [pair! "X/Y coordinates"]
]
move-by: [
    "Move path pen by relative offset"
    offset [pair! "Delta (dx, dy)"]
]
line: [
    "Add one or more line segments to current path"
    points [pair! + "Absolute endpoint coordinates"]
]
line-by: [
    "Add one or more relative line segments to current path"
    deltas [pair! + "Relative deltas from current position"]
]
arc: [
    "Add elliptical arc to path"
    end-point [pair! "Arc endpoint coordinates"]
    rx [number! "X-axis radius"]
    ry [number! "Y-axis radius"]
    rotation [number! "Ellipse rotation angle in degrees"]
    /sweep "Use positive sweep direction"
    /large "Use large-arc (>180°) instead of small-arc"
]
arc-by: [
    "Add elliptical arc with relative endpoint"
    end-delta [pair! "Relative arc endpoint delta"]
    rx [number! "X-axis radius"]
    ry [number! "Y-axis radius"]
    rotation [number! "Ellipse rotation angle in degrees"]
    /sweep "Use positive sweep direction"
    /large "Use large-arc (>180°) instead of small-arc"
]
curve: [
    "Add one or more cubic Bézier curve4 segments"
    control1  [pair! "First control point"]
    control2  [pair! "Second control point"] 
    end-point [pair! "Endpoint coordinates"]
]
curve-by: [
    "Add one or more relative cubic Bézier curve4 segments"
    control1  [pair! "Relative first control point"]
    control2  [pair! "Relative second control point"]
    end-delta [pair! "Relative endpoint delta"]
]
curv: [
    "Add one or more smooth cubic Bézier curve4 segments (relative controls)"
    control1 [pair! "First control point (relative)"]
    end-point [pair! "Endpoint coordinates"]
]
curv-by: [
    "Add one or more smooth cubic Bézier curve4 segments (all relative)"
    control1 [pair! "Relative first control point"]
    end-delta [pair! "Relative endpoint delta"]
]
qcurve: [
    "Add one or more quadratic Bézier curve3 segments"
    control [pair! "Control point"]
    end-point [pair! "Endpoint coordinates"]
]
qcurve-by: [
    "Add one or more relative quadratic Bézier curve3 segments"
    control [pair! "Relative control point"]
    end-delta [pair! "Relative endpoint delta"]
]
qcurv: [
    "Add one or more smooth quadratic Bézier curve3 segments"
    control [pair! "Control point (relative/reflected)"]
]
qcurv-by: [
    "Add one or more smooth quadratic Bézier curve3 segments (relative)"
    end-delta [pair! "Relative endpoint delta"]
]
hline: [
    "Add horizontal line to path end X coordinate"
    x [number! "X position (Y stays same)"]
]
hline-by: [
    "Add horizontal line by relative delta X"
    dx [number! "Delta X (Y stays same)"]
]
vline: [
    "Add vertical line to path end Y coordinate"
    y [number! "Y position (X stays same)"]
]
vline-by: [
    "Add vertical line by relative delta Y"
    dy [number! "Delta Y (X stays same)"]
]
close: [
    "Close current path subpath"
]
