go_line = "30.0 -200.0 400.0 30.0 -200.0 400.0 30.0 -200.0 400.0"

go = go_line.split()
sign_negative = [False, False, False]
sign_negative[0] = True if go[1][0] == '-' else False
sign_negative[1] = True if go[4][0] == '-' else False
sign_negative[2] = True if go[7][0] == '-' else False

print(sign_negative)

ans = "30.0 200.0 30.0 200.0 30.0 200.0"
tmp = ans.split()
if sign_negative[0]:
    tmp[1] = -1 * float(tmp[1])
if sign_negative[1]:
    tmp[3] = -1 * float(tmp[3])
if sign_negative[2]:
    tmp[5] = -1 * float(tmp[5])

ans = ""
for i in range(len(tmp)):
    ans += str(tmp[i]) + " "

ans = ans[:-1]
print(ans.encode())