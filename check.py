actions = []

with open('tmp.out', 'r') as f:
    for line in f:

        line = line.strip()
        #print(line)
        time_range = line[line.find('[') + 1 : line.find(')')]
        time_range = tuple(map(int, time_range.split(',')))
        #print(time_range)
        fr = line[line.find('from ') + 5 : line.find(' to')]
        fr = int(fr)
        to = line[line.find('to ') + 3 :]
        to = int(to)
        #print(fr)
        #print(to)
        actions.append((*time_range, fr, to))
print(len(actions))
for i1, a1 in enumerate(actions):
    for i2, a2 in enumerate(actions):
        if i1 != i2:
            if a1[2] == a2[2] or a1[3] == a2[2]:
                if a2[2] != -1:
                    if not (a1[1] <= a2[0] or a2[1] <= a1[0]):
                        print(a1, a2)
            if a1[2] == a2[3] or a1[3] == a2[3]:
                if a2[3] != -1:
                    if not (a1[1] <= a2[0] or a2[1] <= a1[0]):
                        print(a1, a2)
                    #assert(a1[1] <= a2[0] or a2[1] <= a1[0])
                
