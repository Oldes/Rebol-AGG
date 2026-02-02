![rebol-agg](https://github.com/user-attachments/assets/e3364886-9bf7-4974-9f41-a713d73a4a90)

[![Rebol-AGG CI](https://github.com/Oldes/Rebol-AGG/actions/workflows/main.yml/badge.svg)](https://github.com/Oldes/Rebol-AGG/actions/workflows/main.yml)
[![Gitter](https://badges.gitter.im/rebol3/community.svg)](https://app.gitter.im/#/room/#Rebol3:gitter.im)
[![Zulip](https://img.shields.io/badge/zulip-join_chat-brightgreen.svg)](https://rebol.zulipchat.com/)

# Rebol/AGG

AGG (Anti-Grain Geometry) extension for [Rebol3](https://github.com/Oldes/Rebol3) (version 3.14.1 and higher)  
**(This is still work in progress!)**

## Usage
```rebol
agg: import agg
img: agg/draw 600x600 [pen red line-width 5 triangle 0x0 600x600 300x600]
save %temp.png img
```

## Extension commands:

#### `draw` `:image` `:commands`
Draws scalable vector graphics to an image
* `image` `[image! pair!]`
* `commands` `[block!]`

# AGG Draw Dialect Commands Reference

The AGG Draw dialect provides comprehensive 2D vector graphics capabilities through Rebol's block-based syntax. Each command configures state or immediately renders geometry with flexible coordinate input.

## State Attributes

### `fill` / `fill-pen` / `fill-color`

Sets fill color or toggles filling for subsequent path rendering. Supports solid colors and linear gradients.

**Parameters:**


| Type | Description |
| :-- | :-- |
| `tuple!` | RGBA color value (e.g. `255.0.0.128`) |
| `logic!` | `true`=enabled, `false`=disabled |
| `linear` | Linear gradient: `tuple! number!` (color, position) |

**Example:**

```rebol
fill 255.128.0.255    ; Semi-transparent orange fill
fill false            ; Disable filling
```


### `pen` / `line-color`

Sets stroke color or toggles stroking for path outlines.

**Parameters:**


| Type | Description |
| :-- | :-- |
| `tuple!` | RGBA stroke color |
| `logic!` | `true`=enabled, `false`=disabled |

**Example:**

```rebol
pen 0.0.255.255       ; Blue stroke
pen false             ; No outline
```


### `line-width`

Sets stroke thickness for subsequent paths.

**Parameters:**


| Type | Description |
| :-- | :-- |
| `number!` | Line thickness in pixels/world units |

**Example:**

```rebol
line-width 3.0
```


### `line-join`

Controls corner join style between path segments.

**Parameters:**


| Mode | Description |
| :-- | :-- |
| `'miter` | Sharp corners (extends until limit) |
| `'bevel` | Cut-off corners |
| `'round` | Circular corners |
| `'miter-round` | Miter with rounded truncation |
| `'miter-revert` | Miter with bevel fallback |

### `line-cap`

Defines appearance of path endpoints.

**Parameters:**


| Style | Description |
| :-- | :-- |
| `'butt` | Flat end perpendicular to direction |
| `'square` | Square extension beyond endpoint |
| `'round` | Circular semicircle cap |
| `'round-in` | Inner circular cap |
| `'triangle` | Outward-pointing triangle |
| `'triangle-in` | Inward-pointing triangle |
| `integer!` | Raw cap enum value |

### `alpha`

Global transparency multiplier for all operations.

**Parameters:**


| Type | Description |
| :-- | :-- |
| `number!` | 0.0=transparent, 1.0=opaque |

### `anti-alias`

Controls anti-aliasing and gamma correction.

**Parameters:**


| Type | Description |
| :-- | :-- |
| `number!` | Gamma value (0.5-4.0 typical) |
| `logic!` | `true`=normal (1.0), `false`=off |
| `none!` | Disable anti-aliasing |

### `point-size`

Size for subsequent point drawing commands.

**Parameters:**


| Type | Description |
| :-- | :-- |
| `number!` | Point radius |
| `pair!` | Diameter/extent (non-uniform) |

### `blend`

Sets Porter-Duff/SVG compositing mode.

**Modes:** `'clear`, `'source-copy`, `'source-over`, `'destination-over`, `'source-in`, `'destination-in`, `'source-out`, `'destination-out`, `'source-atop`, `'destination-atop`, `'xor`, `'add`, `'multiply`, `'screen`, `'overlay`, `'darken`, `'lighten`, `'color-dodge`, `'color-burn`, `'hard-light`, `'soft-light`, `'difference`, `'exclusion`

## Basic Shapes

### `line`

Draws open polyline (stroke only) from point sequence.

**Parameters (`coords`):**


| Type | Description |
| :-- | :-- |
| `pair! +` | Sequential pairs (first=moveTo, rest=lineTo) |
| `vector!` | X/Y coordinates (2 values per point) |
| `block!` | Block of pair coordinates |

**Example:**

```rebol
line 10x10 50x20 80x40 120x10
```


### `polygon`

Renders closed polygon from vertices.

**Parameters (`coords`):**


| Type | Description |
| :-- | :-- |
| `pair! 3+` | 3+ individual pairs |
| `vector!` | X/Y coordinates (2 per point) |
| `block!` | Block of pairs |

### `triangle`

Renders one or more triangles from vertex triplets.

**Parameters (`coords`):**


| Type | Description |
| :-- | :-- |
| `pair! 3+` | Sequential triplets per triangle |
| `vector!` | Flattened: x1 y1 x2 y2 x3 y3... |
| `block!` | Block of 3-pair triplets |

### `point`

Draws points using current `point-size`.

**Parameters (`coords`):**


| Type | Description |
| :-- | :-- |
| `pair! +` | Individual coordinates |
| `vector!` | X/Y pairs |
| `block!` | Block of pairs |

### `box` / `rect`

Draws rectangle (optionally rounded).

**Parameters:**


| Param | Type | Description |
| :-- | :-- | :-- |
| `top-left` | `pair!` | Top-left corner |
| `bottom-right`/`size` | `pair!` | Opposite corner/size |
| `radius-x` | `number!` | Horizontal radius (opt) |
| `radius-y` | `number!` | Vertical radius (opt) |

### `circle`

Draws ellipse/circle by center and radii.

**Parameters:**


| Param | Type | Description |
| :-- | :-- | :-- |
| `center` | `pair!` | Ellipse center |
| `radius-x` | `number!` | Horizontal radius |
| `radius-y` | `number!` | Vertical radius (opt) |

### `ellipse`

Ellipse from bounding rectangle.

**Parameters:**


| Param | Type | Description |
| :-- | :-- | :-- |
| `top-left` | `pair!` | Bounding box corner |
| `size` | `pair!` | Width/height |

### `arc`

Elliptical arc with closure options.

**Parameters:**


| Param | Type | Description |
| :-- | :-- | :-- |
| `center` | `pair!` | Ellipse center |
| `radius` | `pair!` | X/Y radii |
| `begin` | `number!` | Start angle (degrees) |
| `sweep` | `number!` | Sweep angle (degrees) |
| `type` | `'open`/`'pie`/`'chord` | Closure style (opt) |

## Curves

### `cubic`

Cubic Bézier spline segments.

**Parameters:**


| Param | Type | Description |
| :-- | :-- | :-- |
| `start` | `pair!` | Initial position (moveTo) |
| `segments` | `pair! 3+` | control1 control2 end (per segment) |

**Example:**

```rebol
cubic 10x10 30x50 70x50 90x10
```


## Transformations

### `translate`

Shifts coordinate origin.

**Parameters:** `offset pair!` (dx, dy)

### `scale`

Scales coordinate system.

**Parameters:** `number!` (uniform) or `pair!` (sx, sy)

### `skew`

Applies shear transformation.

**Parameters:** `angles pair!/number!`, optional `pivot pair!`

### `rotate`

Rotates around center.

**Parameters:** `angle number!` (degrees), optional `center pair!`

### `transform`

Compound affine transform.

**Parameters:** `center pair!` (opt), then `rotate/scale-x/scale-y/translate`

### `matrix`

Full 2D affine matrix.

**Parameters:** `vector!`/`block!` (6 values: sx shy shx sy tx ty)

### `reset-matrix`

Returns identity transform.

## Clipping \& Clearing

### `clip`

Rectangular clipping region.

**Parameters:** `top-left pair!`, `bottom-right pair!`

### `clear-all`

Clears entire surface to transparent black.

### `clear-clip`

Clears clip region (optional fill color).

**Parameters:** `tuple!` color or `unset!` (transparent)

## Scoping

### `push`

State isolation context.

**Parameters:** `block!` of drawing commands

**Example:**

```rebol
push [pen red line 10x10 100x100]
; Original pen restored
```


## Text

### `font`

Loads font resource.

**Parameters:** `file!` path or `string!` name

### `text`

Renders text string.

**Parameters:**


| Param | Type | Description |
| :-- | :-- | :-- |
| `position` | `pair!` | Baseline position |
| `height` | `number!` | Font height (opt) |
| `content` | `string!` | Text to render |

## Paths

### `shape`

Renders complex path from sub-dialect.

**Parameters:** `path-data block!` with path commands (`move`, `line`, `arc`, `curve`, `close`, etc.)

## Images

### `image`

Blits image with optional scaling.

**Parameters:**


| Param | Type | Description |
| :-- | :-- | :-- |
| `img` | `image!` | Source image |
| `position` | `pair!` | Destination top-left |
| `scale` | `pair!/percent!` | Size or scale factor (opt) |


# AGG Path Dialect Reference

The AGG Path DSL defines a sub-dialect for constructing complex vector paths within the `shape` command of the draw dialect. Paths are built sequentially from a current "pen" position, supporting absolute and relative coordinates for maximum flexibility.

Paths support filling, stroking, or both based on the outer draw state's `fill`/`pen` settings. Multiple subpaths can be defined before closing.

## Path Movement

### `move`

Moves the path pen to an absolute position (starts new subpath).

**Parameters:**


| Type | Description |
| :-- | :-- |
| `position` | `pair!` XY coordinates |

**Example:**

```rebol
shape [move 10x10 ...]
```


### `move-by`

Moves pen by relative offset (starts new subpath).

**Parameters:**


| Type | Description |
| :-- | :-- |
| `offset` | `pair!` Delta (dx, dy) |

## Line Commands

### `line`

Adds one or more absolute line segments from current position.

**Parameters:**


| Type | Description |
| :-- | :-- |
| `points` | `pair! +` Absolute endpoint coordinates (batched) |

**Example:**

```rebol
shape [move 0x0 line 50x0 50x50 0x50]
```


### `line-by`

Adds relative line segments.

**Parameters:**


| Type | Description |
| :-- | :-- |
| `deltas` | `pair! +` Relative deltas from current position |

### `hline`

Horizontal line to absolute X (Y unchanged).

**Parameters:** `x number!`

### `hline-by`

Horizontal relative delta X.

**Parameters:** `dx number!`

### `vline`

Vertical line to absolute Y (X unchanged).

**Parameters:** `y number!`

### `vline-by`

Vertical relative delta Y.

**Parameters:** `dy number!`

## Arcs

### `arc`

Adds elliptical arc (SVG-style parameters).

**Parameters:**


| Param | Type | Description |
| :-- | :-- | :-- |
| `end-point` | `pair!` | Arc endpoint coordinates |
| `rx` | `number!` | X-axis radius |
| `ry` | `number!` | Y-axis radius |
| `rotation` | `number!` | Ellipse rotation (degrees) |
| `sweep` | `logic!` | `true`=positive sweep direction |
| `large` | `logic!` | `true`=large-arc (>180°) |

### `arc-by`

Relative endpoint version.

**Parameters:**


| Param | Type | Description |
| :-- | :-- | :-- |
| `end-delta` | `pair!` | Relative endpoint delta |

## Cubic Bézier Curves

### `curve`

Absolute cubic Bézier (C1, C2, end).

**Parameters:**


| Param | Type | Description |
| :-- | :-- | :-- |
| `control1` | `pair!` | First control point |
| `control2` | `pair!` | Second control point |
| `end-point` | `pair!` | Endpoint coordinates |

**Example:**

```rebol
shape [move 10x10 curve 30x50 70x50 90x10]
```


### `curve-by`

All-relative cubic Bézier.

**Parameters:** Relative `control1`, `control2`, `end-delta pair!`

### `curv`

Smooth cubic (first control relative/reflected).

**Parameters:** `control1 pair!` (relative), `end-point pair!`

### `curv-by`

Fully relative smooth cubic.

**Parameters:** `control1 pair!` (relative), `end-delta pair!`

## Quadratic Bézier Curves

### `qcurve`

Absolute quadratic Bézier (single control).

**Parameters:**


| Param | Type | Description |
| :-- | :-- | :-- |
| `control` | `pair!` | Control point |
| `end-point` | `pair!` | Endpoint |

### `qcurve-by`

Relative quadratic.

**Parameters:** Relative `control`, `end-delta pair!`

### `qcurv`

Smooth quadratic (reflected control).

**Parameters:** `control pair!` (relative/reflected)

### `qcurv-by`

Relative smooth quadratic.

**Parameters:** `end-delta pair!`

## Path Closure

### `close`

Closes current subpath to its starting point with a straight line.

**Parameters:** None

**Example (Closed Rectangle with Curve):**

```rebol
shape [
    move 10x10
    line 90x10 90x50 10x50
    curv 30x30 50x70
    close
]
```

**Usage in Draw:** Paths are consumed by `shape` in the main draw dialect, inheriting fill/pen state. All commands support batching multiple segments for efficiency.

