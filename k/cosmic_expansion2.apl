⍝ Código escrito por João Edison Roso Manica
in ← ⎕FIO[49] 'input'
mat ← ⍉⊃in
emptyl ← ⍸{∧/⍵='.'}¨in
emptyc ← ⍸{∧/⍵='.'}¨{mat[⍵;]}¨⍳↑⍴mat
galaxies ← ⍸'.'≠⊃in
∇Z←GetDist gal
  Z ← +/⊃+/|{⍵-⊃gal⌷galaxies}¨gal↓galaxies
∇
times ← 1000000-1
∇Z←Expand gal
  Z ← (gal[1]+times×+/{gal[1]>⍵}¨emptyl),gal[2]+times×+/{gal[2]>⍵}¨emptyc
∇
galaxies ← Expand¨galaxies
+/GetDist¨⍳⍴galaxies
)OFF
