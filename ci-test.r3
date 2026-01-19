Rebol [
    title: "Rebol/AGG extension CI test"
]

;print ["Running test on Rebol build:" mold to-block system/build]

;; make sure that we load a fresh extension
try [system/modules/agg: none]
;; use current directory as a modules location
system/options/modules: what-dir

;; import the extension
agg: import 'agg

{
print as-yellow "Content of the module..."
? agg

grid10x10: agg/draw 20x20 [
    fill 215.215.215
    box 0x0 10x10 box 10x10 20x20
]
;quit
img: agg/draw 300x300 [
    pen 255.0.0.128
    line-width 6
    box 20x20 280x280 40

    line-width 10
    pen 10.10.10.100
    line 0x0 300x300
    line 0x300 300x0

    fill 0.200.0.100
    pen off
    circle 150x150 100
    pen on
    circle 150x150 50 20
]
}

img-size: 480x480 
center:  img-size / 2
points:  #(f64! [100 100  100 380  380 380  380 100])
points2: skip points 2
points-blk:  [100x100  100x380  380x380  380x100]
points-in-block: [
    0x200 2x198.056 4x196.544 6x195.464 8x194.816 10x194.6 12x194.816 14x195.464 16x196.544 18x198.056
    20x200 22x196.073 24x192.628 26x189.667 28x187.188 30x185.191 32x183.677 34x182.646 36x182.098 38x182.032
    40x182.449 42x183.348 44x184.73 46x186.595 48x188.943 50x191.773 52x195.086 54x198.881 56x195.296 58x189.142
    60x183.35 62x177.922 64x172.856 66x168.154 68x163.814 70x159.838 72x156.224 74x152.974 76x150.086 78x147.562
    80x145.4 82x143.602 84x142.166 86x141.094 88x140.384 90x140.038 92x140.054 94x140.434 96x141.176 98x142.282
    100x143.75 102x145.582 104x147.776 106x150.334 108x153.254 110x156.538 112x160.184 114x164.194 116x168.566 118x173.302
    120x178.4 122x183.862 124x189.686 126x195.874 128x196.02 130x185.281 132x174.845 134x164.711 136x154.88 138x145.351
    140x136.125 142x127.201 144x118.58 146x110.261 148x102.245 150x94.531 152x87.12 154x80.011 156x73.205 158x66.701
    160x60.5 162x54.601 164x49.005 166x43.711 168x38.72 170x34.031 172x29.645 174x25.561 176x21.78 178x18.301
    180x15.125 182x12.251 184x9.68 186x7.411 188x5.445 190x3.781 192x2.42 194x1.361 196x0.605 198x0.151
    200x0
]
triangles: [50x50 240x240 50x430 430x50 240x240 430x430]
text: "The quick brown fox jumps over the lazy dog. 123456780"
fontname: "/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf" 

gnome:   load %test/assets/gnome.png
fish:    load %test/assets/fish.png
texture: load %test/assets/texture.jpeg

rotate-45-around-center: #(double! [0.707107 0.707107 -0.707107 0.707107 240.000000 -99.411255])

{
img: make image! 480x480

probe dt [agg/draw :img [
    line-width 4
    pen 200.60.60 
    ellipse 10x10  460x460
    ellipse 10x100 460x280
    ellipse 10x200 460x80
    pen 60.60.200 
    ellipse 100x10 280x460
    ellipse 200x10 80x460

    fill off
    line-width 4
    pen green
    polygon 10x10 240x100 470x10 380x240 470x470 240x380 10x470 100x240

    pen off fill 255.0.0.100 
    polygon 10x10 240x200 470x10 280x240 470x470 240x280 10x470 200x240

    fill-pen 220.120.80
    polygon 10x20 100x10 190x20 100x40
    fill-pen 220.150.100
    polygon 10x20 100x40 100x100 10x50
    fill-pen 120.100.40
    polygon 100x40 190x20 190x50 100x100

    ;- TEST: Line join
    line-width 20 pen gray
    line-join miter       line 140x20   40x80  140x80
    line-join round       line 300x20  200x80  300x80
    line-join bevel       line 460x20  360x80  460x80

    line-join miter       line 140x120  40x180 140x180
    line-join miter       line 300x140 200x180 300x180
    line-join miter       line 460x150 360x180 460x180

    line-join miter round line 140x220  40x280 140x280
    line-join miter round line 300x240 200x280 300x280
    line-join miter round line 460x250 360x280 460x280

    line-join miter bevel line 140x320  40x380 140x380
    line-join miter bevel line 300x340 200x380 300x380
    line-join miter bevel line 460x350 360x380 460x380

    line-join miter
    line-width 1 pen black
    line 140x20   40x80 140x80
    line 300x20  200x80 300x80
    line 460x20  360x80 460x80

    line 140x120  40x180 140x180
    line 300x140 200x180 300x180
    line 460x150 360x180 460x180

    line 140x220  40x280 140x280
    line 300x240 200x280 300x280
    line 460x250 360x280 460x280

    line 140x320  40x380 140x380
    line 300x340 200x380 300x380
    line 460x350 360x380 460x380 

]]
}

pentagram: [
    move 0x-150        ;; Top point
    line-by 88x271     ;; To bottom-right
    line-by -231x-167  ;; To left
    line-by 286x0      ;; To right
    line-by -231x167   ;; To bottom-left
    line-by 88x-271    ;; Back to top
]

tests: [
    line-cap: [
        pen 0.0.100
        line-width 20
        line-cap 0 line 20x20 460x20
        line-cap 1 line 20x50 460x50
        line-cap 2 line 20x80 460x80
        line-cap 3 line 20x110 460x110
        line-cap 4 line 20x140 460x140
        line-cap 5 line 20x170 460x170

    ;   AGG cannot set different START/END caps
    ;    pen 0.100.0
    ;    line-cap 5 0 line 20x220 460x220
    ;    line-cap 5 1 line 20x250 460x250
    ;    line-cap 5 2 line 20x280 460x280
    ;    line-cap 5 3 line 20x310 460x310
    ;    line-cap 5 4 line 20x340 460x340
    ;    line-cap 5 5 line 20x370 460x370
    ]
    line-width: [
        pen 0.0.0
        line-width 1  line 10x0   470x0
        line-width 3  line 10x40  470x40
        line-width 5  line 10x80  470x80
        line-width 7  line 10x120 470x120
        line-width 9  line 10x160 470x160
        line-width 11 line 10x200 470x200
        line-width 13 line 10x240 470x240
        line-width 15 line 10x280 470x280
        line-width 17 line 10x320 470x320
        line-width 19 line 10x360 470x360
        line-width 21 line 10x400 470x400
        line-width 23 line 10x440 470x440
        line-width 25 line 10x480 470x480
    ]
    graph: [
        translate 15x20
        scale 2
        ;; axis
        line-width 1
        line 0x0 0x200 210x200  ;; axis
        ;; graph
        line-width 0.5 pen red
        line  :points-in-block
        fill blue
        point-size 3
        point :points-in-block
    ]
    triangle: [
        line-width 4
        ; single triangle
        pen  red
        fill orange
        triangle 100x100 100x380 380x380
        ; two triangles using raw pairs
        pen  off
        fill olive
        triangle 370x100 110x100 240x230  380x110 380x370 250x240

        ; triangle from a vector value
        fill 200.100.100.100
        triangle :points2

        ; triangles from a block
        fill 100.200.100.100
        triangle :triangles
    ]
    arc: [
        pen 200.60.60
        fill 100.200.100.100
        line-width 8 
        arc 240x240 200x200 0 240 'chord
        line-width 6
        arc 240x240 150x150 0 240
        line-width 4
        fill 100.100.200.100
        arc 240x240 100x100 0 240 'pie
    ]
    point: [
        line-width 4
        pen  red
        fill orange
        point-size 30
        point 240x240

        line-width 2
        fill 100.0.0.128
        point-size 40
        point :points-blk

        line-width 0
        fill blue
        point-size 20
        point :points ; multiple points from vector value

        point-size 10
        fill purple
        point 240x350 ; 
        
        point-size 4
        fill black
        point 230x360 240x360 250x360 240x370 ; multiple points
    ]
    polygon: [
        pen 200.60.60 line-width 4
        fill 200.200.200.100
        polygon 10x10 470x10 470x470 10x470 
        fill off
        polygon 10x10 240x100 470x10 380x240 470x470 240x380 10x470 100x240
        fill 255.0.0 
        polygon 10x10 240x200 470x10 280x240 470x470 240x280 10x470 200x240
    ]
    polygon-line-join: [
        pen red fill-pen orange
        line-width 10 line-join round
        translate 10x0
        polygon 50x100 50x200 150x100 150x200
        translate 130x0 line-join miter
        polygon 50x100 50x200 150x100 150x200
        translate 130x0 line-join bevel
        polygon 50x100 50x200 150x100 150x200
    ]
    polygon-translate: [
        line-width 3
        pen red
        reset-matrix
        scale 1.5
        translate 100x100
        polygon 50x5 62x40 95x40 68x60 80x95 50x74 20x95 32x60 5x40 38x40

        reset-matrix        
        translate 100x100
        polygon 50x5 62x40 95x40 68x60 80x95 50x74 20x95 32x60 5x40 38x40

        reset-matrix
        scale .5
        translate 100x100
        polygon 50x5 62x40 95x40 68x60 80x95 50x74 20x95 32x60 5x40 38x40

        reset-matrix
        scale .2
        translate 100x100
        polygon 50x5 62x40 95x40 68x60 80x95 50x74 20x95 32x60 5x40 38x40

        reset-matrix
        pen blue
        polygon 50x5 62x40 95x40 68x60 80x95 50x74 20x95 32x60 5x40 38x40
    ]
    push: [
        ;clip 0x0 300x300
        fill 200.0.0.128
        point-size 480
        point 240x0
        ;point 30x40
        line-width 5
        box 10x10 470x470
        push [
            line-width 2
            box 200x200 280x280 10
            fill green line-width 10
            skew 0.5x0 240x240
            rotate 45 240x240
            box 200x200 280x280 10
        ]
        box 100x100 200x200 20
    ]
    basic-shapes: [
        translate 40x40
        box 0x0 200x200
        fill green
        circle 100x100 50
        translate 200x0
        ellipse 10x50 190x100
        translate 0x200
        box 10x10 190x190
        fill red
        box 50x50 150x150 20
        translate -200x0
        triangle 100x0 200x200 0x200
    ]
    points: [
        alpha 30%
        line-width 4
        pen red
        point-size 200x300
        point :center
        fill orange
        point-size 30
        point :center ; single point
        line-width 0
        ; multiple points from vector value
        point-size 20 fill blue
        point :points 
        point-size 10 fill purple
        point 240x350
        ; multiple points
        point-size 4  fill black
        point 230x360 240x360 250x360 240x370
        ; multiple points from block
        translate 20x20 scale 2
        point :points-in-block
    ]
    text-1: [
        line-width 0 fill black
        font :fontname
        text 1x12  12 :text
        text 1x40  18 :text
        text 1x74  24 :text
        text 1x120 36 :text
        text 1x178 48 :text
        text 1x248 60 :text
        text 1x330 72 :text
        fill red line-width 2
        text 1x444 104 :text
    ]
    cubic: [
        clear-all fill 0.200.0.100
        pen 200.60.60 line-width 4
        cubic 10x470 100x10 380x470 470x10 
        fill off
        pen 60.60.200 line-width 4.0
        cubic 470x10 10x100 470x380 10x470
        box 50x50 430x430 10 50
    ]
    shape: [
        translate 240x240
        pen green fill off line-width 6
        shape :pentagram
        scale 120%
        pen red
        shape :pentagram
    ]
    shape-arc: [
        fill off
        line-width 10
        line-join  'miter-round
        line-cap   2
        pen orange shape [move 100x200 arc 380x200 220 100 0]
        pen green  shape [move 100x200 arc 380x200 220 100 0 'large]
        pen blue   shape [move 100x200 arc 380x200 220  50 0 'sweep]
        pen red    shape [move 100x200 arc 380x200 220  50 0 'sweep 'large]
        ; complex shape:
        pen black
        shape [
            move 0x399
            line 42x357
            arc 84x315 25 20 -45 'sweep
            line 126x273
            arc  168x231 25 40 -45 'sweep
            line 210x189
            arc  252x147 25 60 -45 'sweep
            line 294x105
            arc  336x63 25 80 -45 'sweep
            line 399x0
            move 0x0
        ]
    ]
    shape-curve: [
        fill off
        ; A cubic Bézier curve is defined by a start point, an end point, and two control points.
        line-width 2 pen black
        shape [
            move 0x0   curve 0x480 480x480 480x0
            move 0x480 curve 0x0   480x0   480x480
        ]
        ; and a shape with multiple curves using just one keyword
        
        line-width 10
        line-cap   2
        line-join 'miter-round
        pen red
        shape [
            move  100x50
            curve 100x450 380x450 380x50  300x250 180x250 100x50
        ]
        ; Control points and lines for the red path:
        pen 0.0.0.100
        line-width 2
        line 100x50 100x450
        line 380x50 380x450
        line 100x50 180x250
        line 380x50 300x250
        pen off fill 0.0.200.200
        point-size 16 point 100x50 100x450 380x450 380x50 300x250 180x250
    ]
    shape-qcurve: [
        fill off
        line-width 10
        line-join 'miter-round
        line-cap   2
        pen red
        shape [
            move 100x50 qcurve 240x430 380x50
        ]
        pen orange line-width 3
        shape [
            move 0x240 line 240x240 qcurve 480x240 480x0
        ]
        ; Control points and lines for the red path:
        pen 0.0.0.100
        line-width 2
        line 100x50 240x430 380x50
        pen off fill 0.0.200.200
        point-size 16 point 100x50 240x430 380x50
    ]
    shape-qcurv: [
        fill off
        line-width 10
        pen red
        shape [
            move     0x240
            qcurve 120x450 240x240
            qcurv  480x240
        ]
        pen blue line-width 5
        shape [
            move    0x240
            qcurve 60x-80 120x240
            qcurv 240x240 360x240 480x240 
        ]
        ; Control points:
        pen off fill 0.0.200.200
        point-size 16 point 0x240 240x240 480x240
    ]
    shape-qcurv-relative: [
        fill off
        line-width 10
        pen red
        shape [
            move        0x240
            qcurve-by 120x210 240x0
            qcurv-by  240x0
        ]
        pen blue line-width 5
        shape [
            move    0x240
            qcurve-by 30x-80 60x0
            qcurv-by 60x0 60x0 60x0
            qcurve-by 15x-200 30x0
            qcurv-by 30x0 30x0 30x0 30x0 30x0 30x0 30x0
        ]
        ; Control points:
        pen off fill 0.0.200.200
        point-size 16 point 0x240 240x240 480x240
    ]
    shape-vline-vline: [
        line-width 10
        line-join 'miter-round
        line-cap   2
        pen red
        shape [
            move 100x100 hline 380
            move 100x150 hline 280
            move 100x200 hline 180
        ]
        pen blue
        shape [
            move 380x100 vline 380
            move 280x150 vline 280
            move 180x200 vline 180
        ]
    ]

    anti-alias: [
        line-width 5
        translate 10x0 anti-alias 0.0  line 0x0 200x480
        translate 10x0 anti-alias 0.5  line 0x0 200x480
        translate 10x0 anti-alias 5.0  line 0x0 200x480
        translate 10x0 anti-alias 7.0  line 0x0 200x480
        translate 10x0 anti-alias 9.0  line 0x0 200x480
        translate 10x0 anti-alias 11.0 line 0x0 200x480
        translate 10x0 anti-alias 13.0 line 0x0 200x480
        translate 10x0 anti-alias 15.0 line 0x0 200x480
        translate 10x0 anti-alias 17.0 line 0x0 200x480
        translate 10x0 anti-alias 19.0 line 0x0 200x480
        translate 10x0 anti-alias 21.0 line 0x0 200x480
        translate 10x0 anti-alias 23.0 line 0x0 200x480
        translate 10x0 anti-alias 25.0 line 0x0 200x480
        translate 10x0 anti-alias 27.0 line 0x0 200x480
        translate 10x0 anti-alias 29.0 line 0x0 200x480
        translate 10x0 anti-alias 31.0 line 0x0 200x480
        translate 10x0 anti-alias 33.0 line 0x0 200x480
        translate 10x0 anti-alias 35.0 line 0x0 200x480
        translate 10x0 anti-alias 37.0 line 0x0 200x480
        translate 10x0 anti-alias 39.0 line 0x0 200x480

        reset-matrix
        fill red line-width 10
        anti-alias on  circle 120x120 100
        translate 240x0
        anti-alias 0.1 circle 120x120 100
    ]

    rotate: [
        translate 100x100
        push [
            fill blue
            box 30x30 130x130
            rotate 25
            fill gray
            box 30x30 130x130
        ]
        push [
            fill blue
            box 150x30 250x130

            ;rotate 25 200x80 ;can be written like:
            translate 200x80 rotate 25 translate -200x-80

            fill gray
            box 150x30 250x130
        ]
    ]
    transform: [
        line-width 3
        pen red 
        fill off
        transform 30 1 1 240x240
        box -100x-100 100x100
        push [
            reset-matrix
            pen green
            box 140x140 340x340
            transform 240x240 60 1 1 0x0
            pen blue
            box 140x140 340x340
        ]
        box -50x-50 50x50
        rotate -30 240x240
        fill gold
        box -20x-20 20x20
    ]
    matrix: [
        line-width 3
        pen red
        push [
            matrix :rotate-45-around-center
            fill 100.200.200.50
            box 40x40 440x440
        ]
        fill off
        matrix [0.866025 0.5 -0.5 0.866025 240 240]
        box -100x-100 100x100
        push [
            reset-matrix
            pen green
            box 140x140 340x340
            matrix [0.5 0.866025 -0.866025 0.5 327.846097 -87.846097]
            pen blue
            box 140x140 340x340
        ]
        box -50x-50 50x50
        rotate -30 240x240
        fill gold
        box -20x-20 20x20
    ]

    rectangle: [
        fill 100.200.200.100
        rect 100x100 200x200
        rectangle 100x100 300x300
        fill 200.200.100.100 pen red
        box  100x100 200x200
    ]

    invert-matrix: [
        line-width 5 fill none     ;; Thick outline only (no fill) for clear edges
        pen red                    ;; Red pen: original box
        rectangle 190x190 100x100  ;; Top-left 190x190, size 100x100 (identity matrix → screen 190x190)

        translate 100x100          ;; Post-multiply translate(+100,+100): shifts subsequent drawing right/down
        pen orange
        rectangle 190x190 100x100  ;; Logical top-left 190x190 → screen 290x290

        invert-matrix              ;; Post-multiply inverse of current matrix: applies -100x-100 translation overall
        pen blue
        rectangle 190x190 100x100  ;; Logical top-left 190x190 → screen 90x90 (shifted top-left from original)
    ]


    blend-clear: [
        scale 50%          ;; Apply 50% scaling to test it doesn't affect fill-all
        fill :texture      ;; Use image as fill style
        fill-all           ;; Completely fill the entire drawing target
        reset-matrix       ;; Reset the transformation matrix
        fill gray          ;; Set fill color to gray
        blend 'clear       ;; Apply CLEAR blend mode
        circle 240x240 200 ;; Draw circle (creates transparent hole in textured image)
    ]

    image: [
        image :gnome 0x0
        image :gnome 250x0 50%
        image :gnome 250x125 50x50
    ]

    image-alpha: [
        alpha 30%
        image :texture 0x0
        alpha 60%
        image :texture 40x40 400x400
        alpha 100%
        image :texture 80x80 320x320
        blend 'clear
        fill black
        circle 240x240 200
    ]
    image-fill: [
        alpha 30%
        image :texture 0x0
        alpha 100%
        fill :texture
        ;box 80x80 400x400
        pen red line-width 4
        scale 50%
        circle 240x240 200
    ]

    image-blend-1: [
        clear-all
        scale 0.5
        translate 30x30
        blend 'source-copy      image :fish  0x0 100%
        blend 'source-over      image :gnome 0x0 100%
        translate 140x0
        blend 'source-copy      image :fish  0x0 100%
        blend 'source-copy      image :gnome 0x0 100%
        translate 140x0
        blend 'source-copy      image :fish  0x0 100%
        blend 'source-in        image :gnome 0x0 100%

       translate -280x140
       blend 'source-copy      image :fish  0x0
       blend 'source-out       image :gnome 0x0
       translate 140x0
       blend 'source-copy      image :fish  0x0
       blend 'source-atop      image :gnome 0x0
       translate 140x0
       blend 'source-copy      image :fish  0x0
       blend 'destination-over image :gnome 0x0

;       translate -280x140
;       blend 'source-copy      image :fish  0x0
;       blend 'destination-in   image :gnome 0x0
;       translate 140x0
;       blend 'source-copy      image :fish  0x0
;       blend 'destination-out  image :gnome 0x0
;       translate 140x0
;       blend 'source-copy      image :fish  0x0
;       blend 'destination-atop image :gnome 0x0
    ]
]

;foreach [name spec] tests [
;    img: make image! img-size
;    agg/draw img spec
;    save probe to file! join name %.png img
;]
img: make image! img-size
;agg/draw img tests/line-cap
;agg/draw img tests/line-width
;agg/draw img tests/graph
;agg/draw img tests/triangle
;agg/draw img tests/arc
;agg/draw img tests/point
;agg/draw img tests/polygon
;agg/draw img tests/polygon-line-join
;agg/draw img tests/polygon-translate
;agg/draw img tests/push
;agg/draw img tests/basic-shapes
;agg/draw img tests/points
;agg/draw img tests/text-1
;agg/draw img tests/cubic
agg/draw img tests/shape
;agg/draw img tests/shape-arc
;agg/draw img tests/shape-curve
;agg/draw img tests/shape-qcurve
;agg/draw img tests/shape-qcurv
;agg/draw img tests/shape-qcurv-relative
;agg/draw img tests/shape-vline-vline
;agg/draw img tests/anti-alias
;agg/draw img tests/rotate
;agg/draw img tests/transform
;agg/draw img tests/matrix
;agg/draw img tests/invert-matrix
;agg/draw img tests/blend-clear
;agg/draw img tests/rectangle
;agg/draw img tests/image
;agg/draw img tests/image-alpha
;agg/draw img tests/image-fill
;agg/draw img tests/image-blend-1

;b: [line-width 5 ] tr: 0x0 a: 1.0
;loop 20 [
;    append b compose [
;        translate 10x0 anti-alias (a) line 0x0 100x480
;    ]
;    tr/x: tr/x + 20
;    a: a + 2
;]
;agg/draw img b
;probe b
;
save %test.png img
try [view img]
quit



