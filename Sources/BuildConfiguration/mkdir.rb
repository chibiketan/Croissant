args = ARGF.argv
folderName = args[0]

if !Dir.exist?(folderName)
    parts = []
    while !Dir.exist?(folderName)
        tmp = File.split(folderName)
        parts.push(tmp[1])
        folderName = tmp[0]
    end

    while !parts.empty?
        folderName = File.join(folderName, parts.pop)
        #puts folderName
        Dir.mkdir(folderName)
    end

    #Dir.mkdir(folderName)
end
