∇Z←Check cube
  b ← (cube≠',')⊂cube
  bool ← 1
  red ← b[⍸(∨/¨({' red'⍷⍵})¨b)]
  green ← b[⍸(∨/¨({' green'⍷⍵})¨b)]
  blue ← b[⍸(∨/¨({' blue'⍷⍵})¨b)]
  →(0=⍴red)⍴L1
    red ← ⊃1⌷red
    bool ← bool ∧ ((⍎⊃1⌷(red≠' ')⊂red) < 13)
  L1: 
  →(0=⍴green)⍴L2
    green ← ⊃1⌷green
    bool ← bool ∧ ((⍎⊃1⌷(green≠' ')⊂green) < 14)
  L2: 
  →(0=⍴blue)⍴L3
    blue ← ⊃1⌷blue
    bool ← bool ∧ ((⍎⊃1⌷(blue≠' ')⊂blue) < 15)
  L3:
  Z ← bool
∇
∇Z←ReadLines line
  aux ← ⊃1⌷(line≠':')⊂line
  aux2 ← ⊃2⌷(line≠':')⊂line
  aux3 ← (aux2≠';')⊂aux2
  →(0=∧/Check¨aux3)⍴LF
    Z ← ⍎⊃2⌷(aux≠' ')⊂aux
    →0
  LF:
    Z ← 0
∇
lines ← ⎕FIO[49] 'input'
+/ ReadLines¨lines
)OFF
