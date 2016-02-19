from sys import argv

script, filename, *args = argv

print("Creating file {0}".format(filename))

# open file to write it
file = open(filename, 'w')
# truncate file
file.truncate()

file.write("#ifndef ENGINE_DEFINES_HPP_INC\n")
file.write("#  define ENGINE_DEFINES_HPP_INC\n")

for fullDefine in args:
    fullDefine = fullDefine[2:]
    define = None
    value = None
    equalIndex = fullDefine.find('=')

    file.write("#  define ")
    if equalIndex == -1:
        define = fullDefine
        value = None
    else:
        define = fullDefine[0:equalIndex]
        value = fullDefine[equalIndex+1:]

    file.write(define)
    if equalIndex != -1:
        file.write(" ")
        file.write(value)

    file.write("\n")

file.write('#endif\n')
# close file
file.close()

