⍝ Código escrito por João Edison Roso Manica
in ← ⎕FIO[49] 'input'
∇Z← Points line
  numbers ← (⊃1↓(line≠':')⊂line)[1;]
  numbers ← (numbers≠'|')⊂numbers
  exponent ← (≢(⍎⊃1⌷numbers)∩(⍎⊃2⌷numbers))-1
  →(exponent<0)⍴L0
  Z ← 2*exponent
  →0
  L0:
  Z ← 0 
∇
+/{Points ⍵}¨in
)OFF
