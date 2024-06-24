⍝ Código escrito por João Edison Roso Manica
∇Z← Replace line
  line[⍸line='-'] ← '¯'
  Z ← ⍎line
∇
∇Z← Zeros line
  sum ← 0
  L0:
    sum ← sum+(¯1↑line)
    line ← {line[⍵+1]-line[⍵]}¨⍳(⍴line)-1
  →(∨/0≠line)⍴L0
  Z ← sum
∇
+/Zeros¨Replace¨⎕FIO[49] 'input'
)OFF
