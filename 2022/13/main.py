import functools
import json

def compare_int(l, r):
    if l < r:
        return -1
    elif l == r:
        return 0
    else:
        return 1


def compare(l, r):
    if isinstance(l, int) and isinstance(r, int):
        return compare_int(l, r)

    if isinstance(l, list) and isinstance(r, list):
        ls = len(l)
        rs = len(r)
        for i in range(min(ls, rs)):
            res = compare(l[i], r[i])
            if res != 0:
                return res
        return compare_int(ls, rs)

    if isinstance(l, int):
        return compare([l], r)
    if isinstance(r, int):
        return compare(l, [r])

    return 0


path = '/home/riverope/Downloads/AoC2022/13/input'

with open(path, 'r') as content_file:
    content = content_file.read()

inOrderIndexes = 0
packets = []
messages = content.split('\n\n')
for index in range(len(messages)):
    messagePair = messages[index]
    messageList = messagePair.split('\n')

    listL = json.loads(messageList[0])
    listR = json.loads(messageList[1])
    
    packets.append(listL)
    packets.append(listR)

    res = compare(listL, listR)
    if res == -1:
        inOrderIndexes += index + 1

    

print(f'Part1: {inOrderIndexes}')

packets.append([[2]])
packets.append([[6]])

sorted_packets = sorted(packets, key=functools.cmp_to_key(compare))
firstIndex = sorted_packets.index([[2]]) + 1
secondIndex = sorted_packets.index([[6]]) + 1

print(f'Part2: {firstIndex * secondIndex}')