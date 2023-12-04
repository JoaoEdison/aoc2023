⍝ Código escrito por João Edison Roso Manica
in ← ⊃(⎕FIO[49] 'input')
∇Z ← mat OutOfBounds idx
  Z ← ∨/(idx=0)∨(idx=1+⍴mat) 
∇
∇Z ← mat Neighbors pos
  hasNeigh ← 8⍴0
  aux ← (1+(pos[1],pos[2]))
  →(mat OutOfBounds aux)⍴L1
    hasNeigh[1] ← aux⌷mat
  L1:
  aux ← ((pos[1],pos[2])-1)
  →(mat OutOfBounds aux)⍴L2
    hasNeigh[2] ← aux⌷mat
  L2:
  aux ← ((1+pos[1]),pos[2])
  →(mat OutOfBounds aux)⍴L3
    hasNeigh[3] ← aux⌷mat
  L3:
  aux ← ((1+pos[1]),(pos[2]-1))
  →(mat OutOfBounds aux)⍴L4
    hasNeigh[4] ← aux⌷mat
  L4:
  aux ← (pos[1],(pos[2]+1))
  →(mat OutOfBounds aux)⍴L5
    hasNeigh[5] ← aux⌷mat
  L5:
  aux ← (pos[1],(pos[2]-1))
  →(mat OutOfBounds aux)⍴L6
    hasNeigh[6] ← aux⌷mat
  L6:
  aux ← ((pos[1]-1),(pos[2]+1))
  →(mat OutOfBounds aux)⍴L7
    hasNeigh[7] ← aux⌷mat
  L7:
  aux ← ((pos[1]-1),pos[2])
  →(mat OutOfBounds aux)⍴L8
    hasNeigh[8] ← aux⌷mat
  L8:
  Z ← ∨/hasNeigh
∇
symbols ← ~in∊'.0123456789'
mat ← in∊'0123456789'
has ← {symbols Neighbors ⍵}¨⍸in∊'0123456789'
has ← has⊂(⍸in∊'0123456789')
has ← ,/(((⍴∊has)÷2),2)⍴(∊has)
{mat[⍵[1];⍵[2]] ← 0}¨has
digits ← in∊'0123456789'
nums ← ⍎¨(,digits)⊂(,in)
+/(⊃,/((~⌊/¨(,digits)⊂(,mat))⊂nums))
)OFF
