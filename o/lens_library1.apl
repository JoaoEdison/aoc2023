⍝ Código escrito por João Edison Roso Manica
split ← {(⍵≠⍺)⊂⍵}
∇ GetValue letter
    ans ← 256|17×ans+letter
∇
∇Z← Hash steps;ans
  ans ← 0
  GetValue¨steps
  Z ← ans
∇
+/Hash¨⎕UCS¨',' split ⍞
)OFF
