⍝ Código escrito por João Edison Roso Manica
∇Z←GetCubes game
  x ← (game≠',')⊂game
  cubes ← 0 0 0
  red ← x[⍸(∨/¨({' red'⍷⍵})¨x)]
  green ← x[⍸(∨/¨({' green'⍷⍵})¨x)]
  blue ← x[⍸(∨/¨({' blue'⍷⍵})¨x)]
  →(0=⍴red)⍴L1
    red ← ⊃1⌷red
    cubes[1] ← (⍎⊃1⌷(red≠' ')⊂red)
  L1: 
  →(0=⍴green)⍴L2
    green ← ⊃1⌷green
    cubes[2] ← (⍎⊃1⌷(green≠' ')⊂green)
  L2: 
  →(0=⍴blue)⍴L3
    blue ← ⊃1⌷blue
    cubes[3] ← (⍎⊃1⌷(blue≠' ')⊂blue)
  L3:
  Z ← cubes
∇
∇Z←ReadLines line
  aux ← ⊃1⌷(line≠':')⊂line
  aux2 ← ⊃2⌷(line≠':')⊂line
  aux3 ← (aux2≠';')⊂aux2
  Z ← ×/↑⌈/GetCubes¨aux3
∇
lines ← ⎕FIO[49] 'input'
+/ReadLines¨lines
)OFF
