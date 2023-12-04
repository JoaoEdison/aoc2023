⍝ Código escrito por João Edison Roso Manica
in ← ⊃(⎕FIO[49] 'input')
∇Z ← mat OutOfBounds idx
  Z ← ∨/(idx=0)∨(idx=1+⍴mat) 
∇
∇Z ← mat Neighbors pos
  hasNeigh ← 12⍴0
  aux ← (pos[1],pos[2])-1
  →(mat OutOfBounds aux)⍴L1
    hasNeigh[1] ← aux⌷mat
  L1:
  aux ← (pos[1]-1),pos[2]
  →(mat OutOfBounds aux)⍴L2
    hasNeigh[2] ← aux⌷mat
  L2:
  aux ← (pos[1]-1),(pos[2]+1)
  →(mat OutOfBounds aux)⍴L3
    hasNeigh[3] ← aux⌷mat
  L3:
  aux ← pos[1],(pos[2]-1)
  →(mat OutOfBounds aux)⍴L4
    hasNeigh[5] ← aux⌷mat
  L4:
  aux ← pos[1],(pos[2]+1)
  →(mat OutOfBounds aux)⍴L5
    hasNeigh[7] ← aux⌷mat
  L5:
  aux ← (1+pos[1]),(pos[2]-1)
  →(mat OutOfBounds aux)⍴L6
    hasNeigh[9] ← aux⌷mat
  L6:
  aux ← (1+pos[1]),pos[2]
  →(mat OutOfBounds aux)⍴L7
    hasNeigh[10] ← aux⌷mat
  L7:
  aux ← 1+(pos[1],pos[2])
  →(mat OutOfBounds aux)⍴L8
    hasNeigh[11] ← aux⌷mat
  L8:
  Z ← hasNeigh
∇
symbols ← in∊'0123456789'
nums ← ⍎¨(,symbols)⊂(,in)
∇Z← GetGear idx
  has ← symbols Neighbors idx
  →(2≠≢has⊂(12⍴1))⍴END
  map ← (⍴in)⍴0
  map[idx[1];idx[2]] ← 1
  has ← {map Neighbors ⍵}¨⍸symbols
  has ← {∨/⍵}¨has
  has ← has⊂(⍸symbols)
  has ← ,/(((⍴∊has)÷2),2)⍴(∊has)
  matrix ← symbols
  {matrix[⍵[1];⍵[2]] ← 0}¨has
  Z ← ⊃,/((~⌊/¨(,symbols)⊂(,matrix))⊂nums)
  →0
  END:
  Z ← 0 0
∇
+/×/¨({GetGear ⍵}¨⍸in∊'*')
)OFF
