⍝ Código escrito por João Edison Roso Manica
in ← ⎕FIO[49] 'input'
matxs ← {'#'=⊃⍵}¨(,⊃{0≠⍴⍵}¨in)⊂in
aux ← 1
mirrors ← ⍬
∇FindPoint equals
  →(aux<1+⍴equals)⍴L0
    →0
  L0:
  →(1=⍴⊃aux⌷equals)⍴L1
    vec ← (⌈2÷⍨(1+⍴equals)-aux)↑(aux-1)↓equals
    →(⊃∨/{1=⍴⍵}¨vec)⍴L1
      mirroed ← ∧/{((1+⍵-⍨⍴equals)∊⊃vec[⍵])∧(aux+⍵-1)∊⊃vec[⍵]}¨⍳⍴vec
      →(⊃mirroed)⍴L2
        →L1
      L2:
        mirrors ← mirrors,1-⍨aux+⍴vec
        →0
  L1:
    aux ← aux+1
    FindPoint equals
∇
∇Z← Reflections mat
  flag ← 0
  mirrors ← ⍬
  j ← 0
  L0:
    ref ← {(⊖(((1⌷⍴mat)⍴2)⊤2*1-⍨⍵))∨(⍵⍴0),⍵{∧/mat[⍺;]=mat[⍺+⍵;]}¨⍳⍵-⍨1⌷⍴mat}¨⍳1⌷⍴mat
    aux ← 1
    FindPoint {⍸⍵}¨ref
    →(flag)⍴L1
      j ← ⍴mirrors
      flag ← 1
      mat ← ⊖mat
      →L0
  L1:
    Z ← ∊{(mirrors[⍵],mirrors[⍵]-⍨1⌷⍴mat)[1+(⍵>j)]}¨⍳⍴mirrors
∇
∇Z← FixSmudge mat
  cp ← mat
  isT ← 0
  L0:
    isT ← isT+1
    cp ← ⍉cp
    first ← Reflections cp
    →(⍬≢first)⍴L1
  →L0
  L1:
    p1 ← first
  lisT ← isT
  isT ← 0
  L2:
    isT ← isT+1
    mat ← ⍉mat
    indexes ← ⍳1⌷⍴mat
    i ← 1
    L3:
      →(i>⍴indexes)⍴L2
      curr_idx ← indexes[i]
      slices ← ⍳curr_idx-⍨1⌷⍴mat
      ii ← 1 
      i ← i+1
      L4:
        →(ii>⍴slices)⍴L3
        curr_slice ← slices[ii]
        ii ← ii+1
        →(1≠+/mat[curr_idx;]≠mat[curr_idx+curr_slice;])⍴L4
          diff ← ⍸mat[curr_idx;]≠mat[curr_idx+curr_slice;]
          mat[curr_idx;diff] ← ~mat[curr_idx;diff]
          p2 ← Reflections mat
          →(∨/p1≠p2)⍴L5
          →((⍬≢p2)∧isT≠lisT)⍴L6
          mat[curr_idx;diff] ← ~mat[curr_idx;diff]
          mat[curr_idx+curr_slice;diff] ← ~mat[curr_idx+curr_slice;diff]
          p2 ← Reflections mat
          →(∨/p1≠p2)⍴L5
          →((⍬≢p2)∧isT≠lisT)⍴L6
          mat[curr_idx+curr_slice;diff] ← ~mat[curr_idx+curr_slice;diff]
        →L4
  L6:
    ans ← p2
    →L7
  L5:
    ans ← ↑(p1≠p2)⊂p2
  L7:
  →(~2|isT)⍴L8
    Z ← ans 
    →0
  L8:
    Z ← 100×ans
∇
+/FixSmudge¨matxs
)OFF
