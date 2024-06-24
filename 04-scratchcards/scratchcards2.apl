⍝ Código escrito por João Edison Roso Manica
cards ← {≢(⊃⍵[1])∩(⊃⍵[2])}¨{⍎¨⍵}¨{(⍵≠'|')⊂⍵}¨{(⊃1↓(⍵≠':')⊂⍵)[1;]}¨⎕FIO[49] 'input'
indexes ← ⍳≢cards
aux ← 0
∇Z←cards Wins card
  aux ← aux+1
  a ← {cards Wins ⍵}¨cards[card]↑card↓indexes
  Z ← 0
∇
{cards Wins ⍵}¨indexes
aux
)OFF
