Rebol [
    title: "Rebol/AGG extension CI test"
]

print ["Running test on Rebol build:" mold to-block system/build]

;; make sure that we load a fresh extension
try [system/modules/agg: none]
;; use current directory as a modules location
system/options/modules: what-dir

;; import the extension
agg: import 'agg

? agg

texture: load %test/assets/texture.jpeg
pentagram: [
    move 0x-150        ;; Top point
    line-by 88x271     ;; To bottom-right
    line-by -231x-167  ;; To left
    line-by 286x0      ;; To right
    line-by -231x167   ;; To bottom-left
    line-by 88x-271    ;; Back to top
]

img: agg/draw 480x480 [
    alpha 30%
    fill :texture fill-all
    alpha 100%
    translate 240x240
    pen green fill off line-width 6
    shape :pentagram
    scale 120%
    pen red
    shape :pentagram
]

save %test.png img

