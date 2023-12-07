⍝ Código escrito por João Edison Roso Manica
GetVec ← {⊃2⌷(⍵≠⍺)⊂⍵}
GetNum ← {⍎∊((⍵∊'0123456789')⊂⍵)}
time ← GetNum ':' GetVec ⍞
dist ← GetNum ':' GetVec ⍞
∇Z← d Measure t
  aux ← 2
  L0:
  →(d<aux×t-aux)⍴L1
    aux ← aux+1
    →L0
  L1:
  →(t|2≠0)⍴L2
    Z ← t-2×aux
    →0
  L2:
    Z ← 1+t-2×aux
∇
dist Measure time
)OFF
