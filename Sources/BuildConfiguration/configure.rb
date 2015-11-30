require 'io/console'

args = ARGF.argv
#dFile = args[0]
#cppFile = args[1]
#oFile = args[2]

class DefineSimple
	@symbol
	
	def initialize(symbol)
		@symbol = symbol
	end
	
	def to_s
		"-D#{@symbol}"
	end
end

class LoadFlag
  @symbol
  
  def initialize(symbol)
    @symbol = symbol
  end
  
  def to_s
    "-l#{@symbol}"
  end
end

class CppInclude
	def initialize(path, isSystem = true)
		@path = path
		@isSystem = isSystem
	end
	
	def to_s
		if @isSystem
			"<#{@path}>"
		else 
			"\"#{@path}\""
		end
	end
end

module CppTest
	def write
		File.open("#{fileName}.cpp", "w") { |file|
			includeList { |include|
				file.write "#include #{include}\n"
			}
		
			file.write("int main(int argc, char** argv) {\n")
			mainContent { |line|
			  file.write(line)
		  }
			file.write("return 0;\n")
			file.write("}")
		}
	end
	
	def run
		write
		result = system("g++ -std=c++11 #{compilationFlags} -o #{fileName}.exe #{fileName}.cpp", { :out => "out.log", :err => "out.log"})
		clean
		
		result
	end
	
	def clean
		if (File.exists?("#{fileName}.exe"))
			File.delete("#{fileName}.exe")
        end

		if (File.exists?("#{fileName}.cpp"))
			File.delete("#{fileName}.cpp")
		end

		if (File.exists?("out.log"))
			File.delete("out.log")
        end
	end
end

class TestThreadStd
    include CppTest
	#
	def initialize
	end
	
	def fileName
		"TestThreadStd"
	end
	
	def compilationFlags
	  ""
	end
	
	def includeList
		yield CppInclude.new("thread", true)
	end
	
	def mainContent
	  yield "std::thread t([]() {});"
	  yield "t.join();"
	end
	
	def writeConfig(config)
		config.addDefine DefineSimple.new(:CROISSANT_THREAD_STD)
		config.addLoadFlag LoadFlag.new(:pthread)
	end
end

class ThreadCapability
	@test
	@result
	
	def initialize
	  puts "Detection des capacités de threading"
		@test = TestThreadStd.new

		@result = @test.run
		if (!@result)
			puts "thread standard c++11 [KO]"
		else
		  puts "thread standard c++11 [OK]"
		end
	end
	
	def writeConfig(config)
		if (@result)
			config.addDefine DefineSimple.new(:CROISSANT_HAS_THREAD)
			@test.writeConfig config
		end
	end
end

class TestMemoryDebug

  def initialize
    puts "Activation du debug mémoire [OK]"
  end
  
  def writeConfig(config)
    config.addDefine DefineSimple.new(:CROISSANT_MEMORY_DEBUG)
  end
end


class Configuration
	@ldflags
	@cflags
	def initialize
		@ldflags = []
		@cflags = []

		# configuration des threads
		capa = ThreadCapability.new
		capa.writeConfig self
		
		# configuration du debugde la mémoire
    memDeb = TestMemoryDebug.new
    memDeb.writeConfig self
		
	end

	def addDefine(define)
		@cflags << define
	end
	
	def addLoadFlag(loadFlag)
	  @ldflags << loadFlag
	end
	
	def write
		File.open("configure.mk", "w") { |file|
			file.write("CXXFLAGS+=")
			@cflags.each { |flag|
				file.write " "
				file.write flag
			}
			file.write "\n"
			
			file.write "LDFLAGS+="
			@ldflags.each { |flag|
				file.write " "
				file.write flag
			}
			file.write "\n"
		}
	end
end

#test = ThreadCapability.new
#puts test.
#
config = Configuration.new
config.write
puts "end"
