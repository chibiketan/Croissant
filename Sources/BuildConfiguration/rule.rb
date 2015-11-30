require 'io/console'

args = ARGF.argv
dFile = args[0]
cppFile = args[1]
oFile = args[2]

# on retire un comportement bizarre de notre ami mingw64 pour windows + ruby
oFile = oFile.sub("C:/MinGW/msys/1.0", "")

if oFile == ''
  oFile = '/'
end

File.open(dFile, "w") { |file|
    index = 0

    while (line = $stdin.gets)
        if index == 0
          line = line.sub!(/(?<basename>.*\.o):/, "$($(TARGET)_LIBDIR)#{oFile}\\k<basename>: #{dFile}")
        end
        
        file.write(line)
        index = index + 1
    end
    file.write("	@echo compilation du fichier objet $@\n")
    #file.write("	if not exist \"$(subst /,\,$(dir $@))\" mkdir \"$(subst /,\,$(dir $@))\"\n")
    file.write("	@ruby \"")
    file.write(File.join(File.expand_path(File.dirname(__FILE__)), "mkdir.rb"))
    file.write("\" \"$(dir $@)\"\n")
    file.write("	@$(CXX) -o $@ -c ")
    file.write(cppFile)
    file.write(" $(DEBUG_CXXFLAGS)")
}
