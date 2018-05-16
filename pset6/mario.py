import cs50

while True:
    print("Height: ", end="")
    height = cs50.get_int()

    if height > 0 and height < 23:
        break
 
for i in range(height):
    spaces = height - (i + 1) 
    hashes = i + 1
    gap = 2
     
    # print spaces for left pyramid
    print(spaces * ' ', end="")
    # print hashes for left pyramid
    print(hashes * '#', end="")
    # print gap
    print(gap * ' ', end="")
    # print hashes for right pyramid
    print(hashes * '#', end="")
    # print new line
    print()

    
