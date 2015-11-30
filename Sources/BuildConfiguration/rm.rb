require 'fileutils'

args = ARGF.argv
folderName = args[0]

FileUtils.rm_rf(folderName)
