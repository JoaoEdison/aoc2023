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
∇Z← Result idx
  has ← symbols Neighbors idx
  →(2≠≢has⊂(12⍴1))⍴END
  mapa ← (⍴in)⍴0
  mapa[idx[1];idx[2]] ← 1
  mat ← in∊'0123456789'
  has ← {mapa Neighbors ⍵}¨⍸mat
  has ← {∨/⍵}¨has
  has ← has⊂(⍸in∊'0123456789')
  has ← ,/(((⍴∊has)÷2),2)⍴(∊has)
  {mat[⍵[1];⍵[2]] ← 0}¨has
  res ← ⊃,/((~⌊/¨(,symbols)⊂(,mat))⊂nums)
  Z ← res
  →0
  END:
  Z ← 0 0
∇
mata ← in∊'*'
res ← ({Result ⍵}¨⍸mata)
+/×/¨res
)OFF
