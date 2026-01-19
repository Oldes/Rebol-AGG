Rebol [
    title: "AGG Draw DSL commands specification"
    version: 0.1.0
]

;- State Attributes (setup before drawing) -------------------------------------
fill:
fill-pen:
fill-color: [
    "Set fill color or toggle for subsequent path rendering"
    color [
        tuple!  "RGBA color value"
        logic!  "true=enabled false=disabled"
        [
            'linear "Linear gradient"
            tuple! number! +
        ]

    ]
]

pen:
line-color: [
    "Set stroke color or toggle for subsequent path rendering"
    color [
        tuple!  "RGBA color value" 
        logic!  "true=enabled false=disabled"
    ]
]

line-width: [
    "Set line width for subsequent path rendering"
    width [number! "Line thickness in pixels/world units"]
]

line-join: [
    "Set line join style for subsequent path rendering"
    mode [
        'miter        "Sharp corners"
        'bevel        "Bevel (cut-off corners)"
        'round        "Round corners"
        'miter-round  "Miter with rounded truncation"
        'miter-revert "Miter with revert-to-bevel fallback"
    ]
]

line-cap: [
    "Set line cap style for path endpoints"
    cap [
        integer!      "Raw numeric cap enum value"
        'butt         "Flat end perpendicular to line direction"
        'square       "Square extension beyond endpoint"
        'round        "Circular semicircle at endpoint"
        'round-in     "Inner circular semicircle"
        'triangle     "Triangle pointing away from endpoint"
        'triangle-in  "Inner triangle"
    ]
]

alpha: [
    "Set master alpha for subsequent drawing operations"
    value [number! "Alpha multiplier (0.0=transparent, 1.0=opaque)"]
]

anti-alias: [
    "Set anti-aliasing gamma control"
    gamma [
        number! "Gamma value (0.5-4.0 typical)"
        logic!  "TRUE=normal (1.0), FALSE=off (1000.0)"
        none!   "Disable anti-aliasing"
    ]
]

point-size: [
    "Set point size for subsequent point drawing operations"
    size [
        number! "Point radius"
        pair!   "Diameter/extent"
    ]
]

blend: [
    "Set Porter-Duff/SVG compositing mode for subsequent drawing"
    mode [
        'clear            "No output (dest cleared)"
        'source-copy      "Source only (Src)"
        'destination-copy "Destination only (Dst)" 
        'source-over      "Source over destination (SrcOver)"
        'destination-over "Destination over source (DstOver)"
        'source-in        "Source clipped by destination (SrcIn)"
        'destination-in   "Destination clipped by source (DstIn)"
        'source-out       "Source where dest is transparent (SrcOut)"
        'destination-out  "Destination where source is transparent (DstOut)"
        'source-atop      "Source clipped to dest, dest behind (SrcAtop)"
        'destination-atop "Dest clipped to source, source behind (DstAtop)"
        'xor              "Exclusive OR (Src XOR Dst)"
        'add              "Pixel addition"
        'multiply         "Multiply source * destination"
        'screen           "Screen blend (1 - (1-S)*(1-D))"
        'overlay          "Overlay (brighter/darker regions)"
        'darken           "Darker of source/destination"
        'lighten          "Lighter of source/destination"
        'color-dodge      "Color dodge"
        'color-burn       "Color burn" 
        'hard-light       "Hard light"
        'soft-light       "Soft light"
        'difference       "Absolute difference"
        'exclusion        "Exclusion blend"
    ]
]




;- Basic Shapes ----------------------------------------------------------------
line: [
    "Draw open polyline from sequence of points (stroke only)"
    coords [
        "Flexible coordinate input supporting multiple points"
        pair! +   "Sequential pairs: moveTo first, lineTo rest"
        vector!   "Vector of x/y coordinates (2 values per point)"
        block!    "Block of pair coordinates"
    ]
]

polygon: [
    "Draw closed polygon from sequence of points"
    coords [
        "Flexible coordinate input supporting multiple points"
        pair! 3 + "Three or more individual pair coordinates"
        vector!   "Vector of x/y coordinates (2 values per point)"
        block!    "Block of pair coordinates"
    ]
]

triangle: [
    "Draw one or more triangles from vertex coordinates"
    coords [
        "Flexible triangle vertex input (groups of 3 points)"
        pair! 3 + "Sequential triplets: triangle(p0,p1,p2) repeated" 
        vector!   "Flattened x1 y1 x2 y2 x2 y3 ... (6 values per triangle)"
        block!    "Block of 3-pair triplets"
    ]
]

point: [
    "Draw one or more points at specified coordinates"
    "Point size set by preceding point-size command"
    coords [
        "Flexible coordinate input supporting multiple points"
        pair! +   "One or more individual pair coordinates"
        vector!   "Vector of x/y coordinates (2 values per point)"
        block!    "Block of pair coordinates"
    ]
]

box: [
    "Draw rectangle (optionally rounded)"
    top-left     [pair!  "Top-left corner"]
    bottom-right [pair!  "Bottom-right corner"]
    radius-x     [number! "Horizontal corner radius" optional]
    radius-y     [number! "Vertical corner radius (defaults to radius-x)" optional]
]

rect: [
    "Draw rectangle (optionally rounded)"
    top-left     [pair!  "Top-left corner"]
    size         [pair!  "Width/height"]
    radius-x     [number! "Horizontal corner radius" optional]
    radius-y     [number! "Vertical corner radius (defaults to radius-x)" optional]
]

circle: [
    "Draw ellipse/circle with optional elliptical radius"
    center [pair!  "Ellipse center (x,y)"]
    radius-x [number! "Horizontal radius"]
    radius-y [number! "Vertical radius (defaults to radius-x)" optional]
]

ellipse: [
    "Draw ellipse defined by top-left corner and size"
    top-left [pair!  "Top-left bounding box corner"]
    size     [pair!  "Width/height of ellipse bounding box"]
]
arc: [
    "Draw elliptical arc (pie/chord capable)" 
    center [pair!   "Center point of the ellipse"]
    radius [pair!   "Elliptical radius"]
    begin  [number! "Starting angle in degrees"]
    sweep  [number! "Sweep angle in degrees"]
    type   [
        'open  "Open arc (no fill closure)"
        'pie   "Pie slice (radial lines to center)"
        'chord "Chord (connects arc ends)"
        optional
    ]
]

;- Curves ----------------------------------------------------------------------
cubic: [
    "Draw cubic Bézier spline (moveTo + cubicCurveTo segments)"
    start [pair! "Initial position (moveTo)"]
    segments [
        "Sets of 3 pairs per cubic segment: control1 control2 end"
        pair! 3 + "Sequential cubic Bézier segments"
    ]
]

;- Transformations -------------------------------------------------------------
translate: [
    "Translate coordinate system by offset vector"
    offset [pair! "Translation vector (dx, dy)"]
]
scale: [
    "Scale coordinate system"
    factors [
        number! "Uniform scaling (sx=sy)"
        pair!   "Non-uniform scaling (sx, sy)"
    ]
]
skew: [
    "Apply skew/shear transformation"
    angles [
        pair!   "Skew angles sx,sy in degrees"
        number! "Uniform skew angle s in degrees"
    ]
    pivot [pair! "Optional pivot point for skew" optional]
]
rotate: [
    "Apply rotation transformation"
    angle  [number! "Rotation angle in degrees (auto-converted to radians)"]
    center [pair!   "Optional rotation center/pivot" optional]
]
transform: [
    "Apply affine transformation relative to optional center"
    center    [pair!   "Rotation/scale pivot point" optional]
    rotate    [number! "Angle in degrees"]
    scale-x   [number! "X scale factor"]
    scale-y   [number! "Y scale factor"]
    translate [pair!   "Translation offset"]
]

matrix: [
    "Set full 2D affine transformation matrix"
    matrix [
        vector! "At least 6 float64 values: [sx shy shx sy tx ty] (uses first 6)"
        block!  "6 numbers: [sx shy shx sy tx ty]"
        ;@@TODO: matrix as a handle!
    ]
]

invert-matrix: [
    "Invert current transformation matrix"
]
reset-matrix: [
    "Reset transformation matrix to identity"
]

;- Clipping and Clearing -------------------------------------------------------
clip: [
    "Set rectangular clipping region"
    top-left     [pair! "Top-left clip corner"]
    bottom-right [pair! "Bottom-right clip corner"]
]
clear-all: [
    "Clear entire surface to transparent black"
]
clear-clip: [
    "Clear clipping region with optional fill color"
    color [
        tuple!  "RGBA background color to fill before clearing"
        unset!  "Clear to transparent without fill (full surface)"
    ]
]

;- Scoping ---------------------------------------------------------------------
push: [
    "Push state, execute block, then pop state (scoping group)"
    block [block! "Drawing commands executed in isolated state"]
]

;- Text ------------------------------------------------------------------------
font: [
    "Load font from file path or string"
    source [
        file!   "Font file path"
        string! "Font name"
    ]
]
text: [
    "Draw text at position with optional height"
    position [pair! "X/Y coordinates"]
    height [number! "Font height in pixels" optional]
    content [string! "Text to render"]
]

;- Paths -----------------------------------------------------------------------
shape: [
    "Define shape path from block of commands"
    path-data [block! "Path commands: move line curve close etc."]
]

;- Image -----------------------------------------------------------------------
image: [
    "Blit image at position or scaled to destination rect"
    img      [image! "Source image"]
    position [pair!  "Top-left destination position"]
    scale [
        optional
        percent! "Relative scale factor"
        pair!    "Absolute destination image size"
    ]
]
