⍝ Código escrito por João Edison Roso Manica
in ← ⎕FIO[49] 'input'
matxs ← {⊃⍵}¨(,⊃{0≠⍴⍵}¨in)⊂in
aux ← 1
∇Z← FindPoint equals
  →(aux<1+⍴equals)⍴L1
    Z ← 0
    →0
  L1:
  →(1=⍴⊃aux⌷equals)⍴L0
    vec ← (⌈2÷⍨(1+⍴equals)-aux)↑(aux-1)↓equals
    →(⊃∨/{1=⍴⍵}¨vec)⍴L0    
      mirroed ← ∧/{((1+⍵-⍨⍴equals)∊⊃vec[⍵])∧(aux+⍵-1)∊⊃vec[⍵]}¨⍳⍴vec
      →(⊃mirroed)⍴L2
        →L0
      L2:
        Z ← 1-⍨aux+⍴vec
        →0
  L0:
    aux ← aux+1
    Z ← FindPoint equals
∇
∇Z← Reflection mat
  flag ← 0
  ans ← 100
  →L2
  L1:
    flag ← 0
    ans ← 1
    mat ← ⍉mat
  L2:
    ref ← {(⊖(((1⌷⍴mat)⍴2)⊤2*1-⍨⍵))∨(⍵⍴0),⍵{∧/mat[⍺;]=mat[⍺+⍵;]}¨⍳⍵-⍨1⌷⍴mat}¨⍳1⌷⍴mat
    aux ← 1
    res ← FindPoint {⍸⍵}¨ref
    →(res)⍴L0
    →(flag)⍴L3
      flag ← 1
      mat ← ⊖mat
      →L2
  L3:
    →(100=ans)⍴L1
  L0:
    →(flag)⍴L4
      Z ← ans×res
      →0
    L4:
      Z ← ans×res-⍨1⌷⍴mat
∇
+/Reflection¨matxs
)OFF
