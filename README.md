![rebol-agg](https://github.com/user-attachments/assets/e3364886-9bf7-4974-9f41-a713d73a4a90)

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
