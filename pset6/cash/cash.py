from cs50 import get_float

while True:
    c = get_float("change owed?: ")
    if c > 0:
        break

#transform change into cents
c = c * 100

quarters = 0
dimes = 0
nickels = 0
pennies = 0

if c >= 25:
    rq = c % 25
    w = c - rq
    quarters = quarters + w / 25
    c = rq
    
if c >= 10:
    rd = c % 10
    w = c - rd
    dimes = dimes + w / 10
    c = rd
    
if c >= 5:
    rn = c % 5
    w = c - rn
    nickels = nickels + w / 5
    c = rn
    
if c >= 1:
    rp = c % 1
    w = c - rp
    pennies = pennies + w / 1
    c = rp

print(f"{int(quarters + dimes + nickels + pennies)}")
