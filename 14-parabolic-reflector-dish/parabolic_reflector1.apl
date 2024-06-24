⍝ Código escrito por João Edison Roso Manica
in ← ⎕FIO[49] 'input'
getIdxOf ← {⍸⊃⍵=⍺}
cubes ← in getIdxOf '#'
cubeMtx ← ⊃cubes
positions ← ⍬
getLRock ← {¯2↑∊((⍺[;2]=2⌷rock)∧⍺[;1]<1⌷rock)⊂⍵}
∇Tilt rock
  →(1<1⌷rock)⍴L0
    positions ← positions,⊂rock
    →0
  L0:
    posMtx ← ((⍴positions),2)⍴⊃positions
    lround ← posMtx getLRock positions
    lcube ← cubeMtx getLRock cubes
    positions ← positions,⊂((1+(1⌷lround)⌈1⌷lcube),2⌷rock)
∇
Tilt ¨ in getIdxOf 'O'
+/{1+(⍴in)-1⌷⍵}¨positions
)OFF
