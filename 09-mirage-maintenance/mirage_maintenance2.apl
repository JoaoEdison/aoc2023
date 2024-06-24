⍝ Código escrito por João Edison Roso Manica
∇Z← Replace line
  line[⍸line='-'] ← '¯'
  Z ← ⍎line
∇
∇Z← Back first
  oddMask ← 2|⍳⍴first
  odd ← +/oddMask⊂first
  even ← +/(~oddMask)⊂first
  →(0=2|⍴first)⍴L0
  Z ← odd-even
  →0
  L0:
  Z ← even-odd
∇
∇Z← Zeros line
  extra ← ⍬
  L0:
    extra ← (1↑line),extra
    line ← {line[⍵+1]-line[⍵]}¨⍳(⍴line)-1
  →(∨/0≠line)⍴L0
  Z ← Back extra
∇
+/Zeros¨Replace¨⎕FIO[49] 'input'
)OFF
