import matplotlib.pyplot as plt

num_strings = 100 * 1000

strings = []

with open("instruction.txt") as file:
    for i in range(num_strings):
        strings.append(file.readline())

for i in range(num_strings):
    strings[i] = (strings[i].split('\''))[1] # necessary string in middle

one_line_pattern = {} # {label : counter}
for i in range(num_strings):
    counter = one_line_pattern.get(strings[i])
    if counter == None:
        one_line_pattern[strings[i]] = 1
    else:
        one_line_pattern[strings[i]] = counter + 1

two_line_pattern = {} # {label + label : counter}
for i in range(num_strings - 1):
    pattern = strings[i] + " + " + strings[i + 1]
    counter = two_line_pattern.get(pattern)
    if counter == None:
        two_line_pattern[pattern] = 1
    else:
        two_line_pattern[pattern] = counter + 1

three_line_pattern = {} # {label + label + label : counter}
for i in range(num_strings - 2):
    pattern = strings[i] + " + " + strings[i + 1] + " + " + strings[i + 2]
    counter = three_line_pattern.get(pattern)
    if counter == None:
        three_line_pattern[pattern] = 1
    else:
        three_line_pattern[pattern] = counter + 1

def print_pattern_hist(num_line_pattern, num_most_common):

    sorted_pattern = dict(sorted(num_line_pattern.items(), key = lambda item: item[1])[::-1][:num_most_common])

    num_pattern = list(sorted_pattern.keys())
    num_counter = list(sorted_pattern.values())

    fig, ax = plt.subplots(figsize = (16, 16))
    plt.barh(num_pattern, num_counter)
    plt.show()

print_pattern_hist(one_line_pattern, 10)
print_pattern_hist(two_line_pattern, 10)
print_pattern_hist(three_line_pattern, 10)