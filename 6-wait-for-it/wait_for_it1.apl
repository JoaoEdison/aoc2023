⍝ Código escrito por João Edison Roso Manica
GetVec ← {⍎⊃2⌷(⍵≠⍺)⊂⍵}
time ← ':' GetVec ⍞
dist ← ':' GetVec ⍞
∇Z←Measure t
  Z ← +/t[2]<{⍵×1⌷t-⍵}¨1↓⍳(1⌷t-2)
∇
×/Measure¨{time[⍵],dist[⍵]}¨⍳⍴time
)OFF
