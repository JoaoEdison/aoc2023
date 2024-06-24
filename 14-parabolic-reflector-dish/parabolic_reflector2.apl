⍝ Código escrito por João Edison Roso Manica
in ← ⎕FIO[49] 'input'
getIdxOf ← {⍸⊃⍵=⍺}
getLRock ← {¯2↑∊((⍺[;2]=2⌷rock)∧⍺[;1]<1⌷rock)⊂⍵}
sort ← {⍵[⍋⍵]}
∇Z← cubesParams Tilt rock;posMtx;lround;lcube
  →(1<1⌷rock)⍴L0
    positions ← positions,⊂rock
    Z ← 0
    →0
  L0:
    posMtx ← ((⍴positions),2)⍴⊃positions
    lround ← posMtx getLRock positions
    lcube ← (⊃1⌷cubesParams) getLRock (⊃2⌷cubesParams)
    positions ← positions,⊂((1+(1⌷lround)⌈1⌷lcube),2⌷rock)
    Z ← 0
∇
∇Z← cubesParams Turn vec;positions;a
  positions ← ⍬
  a ← {cubesParams Tilt ⍵}¨vec
  Z ← positions
∇
x0 ← in getIdxOf 'O'
mtxT ← {((⊃⍵) ⍵)}
cubesN ← mtxT in getIdxOf '#'
cubesW ← mtxT sort {⊖⍵}¨(⊃2⌷cubesN)
cubesS ← mtxT sort {(1+(⍴in)-2⌷⍵),1⌷⍵}¨(⊃2⌷cubesW)
cubesL ← mtxT sort {(1+(⍴in)-2⌷⍵),1+(⍴in)-1⌷⍵}¨(⊃2⌷cubesS)
∇Z← Cycle map;prev;now
  prev ← cubesN Turn map 
  now ← sort {⊖⍵}¨prev
  prev ← cubesW Turn now 
  prev ← sort {(1+(⍴in)-2⌷⍵),1⌷⍵}¨prev
  now ← cubesS Turn prev
  now ← sort {(1+(⍴in)-2⌷⍵),1+(⍴in)-1⌷⍵}¨now
  prev ← cubesL Turn now
  Z ← sort {(2⌷⍵),1+(⍴in)-1⌷⍵}¨prev
∇
∇Z← Brent ;power;lam;mu;tortoise;hare
  power ← 1
  lam ← 1
  tortoise ← x0 
  hare ← Cycle tortoise
  L0:
  →(tortoise≡hare)⍴L1
    →(power≠lam)⍴L2
      tortoise ← hare
      power ← power×2
      lam ← 0
    L2:
      hare ← Cycle hare
      lam ← lam+1
      lam
    →L0  
  L1:
    tortoise ← hare ← x0
    i ← 0
  L3:
    →(i=lam)⍴L4
      hare ← Cycle hare
      i ← i+1
    →L3
  L4:
  mu ← 0
  L5:
    →(tortoise≡hare)⍴L6
      tortoise ← Cycle tortoise
      hare ← Cycle hare
      mu ← mu+1
  →L5
  L6:
    Z ← lam,mu,⊂hare
∇
∇ Part2 last;cyc;remain;curr
  cyc ← Brent
  cyc
  remain ← cyc[1]|last-2⌷cyc
  curr ← ⊃3⌷cyc 
  L0:
    →(0=remain)⍴L1
      curr ← Cycle curr 
      remain ← remain-1 
  →L0
  L1:
    +/{1+(⍴in)-1⌷⍵}¨curr
∇
Part2 1000000000
)OFF
