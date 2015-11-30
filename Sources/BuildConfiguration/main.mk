
ROOT_MAIN_MAKEFILE := $(realpath $(lastword $(MAKEFILE_LIST)))
ROOT_MAIN := $(realpath $(dir $(lastword $(MAKEFILE_LIST))))

LIBDIR := $(ROOT_MAIN)/../../Obj/$(EXE_NAME).$(MACHINE)_$(ARCH)$(EXE_EXT)

CXXFLAGS += $(patsubst %,-I%,$(EXTRA_INCLUDE))

# ---------------------------------------- Debug Config

DEBUG_SUFFIX := d
DEBUG_EXE := $(EXE_NAME)$(DEBUG_SUFFIX).$(MACHINE)_$(ARCH)$(EXE_EXT)
DEBUG_OUTPUT_DIR := $(ROOT_MAIN)/../../Tests$(OUTPUT_DIR)
DEBUG_CXXFLAGS := $(CXXFLAGS) -g -ggdb
DEBUG_LIBDIR := $(LIBDIR)/Debug
DEBUG_LDLIBS := $(LDFLAGS) -L$(ROOT_MAIN)/../../Tests $(LDLIBS)
DEBUG_OBJS := $(SOURCES:$(SRCDIR)/%.cpp=$(DEBUG_LIBDIR)/%.o)

ifneq ($(DEBUG_MEMORY),NO)
DEBUG_CXXFLAGS += -DCROISSANT_MEMORY_DEBUG
endif
# ---------------------------------------- Release Config

RELEASE_SUFFIX :=
RELEASE_EXE := $(EXE_NAME)$(RELEASE_SUFFIX).$(MACHINE)_$(ARCH)$(EXE_EXT)
RELEASE_OUTPUT_DIR := $(ROOT_MAIN)/../../Bin$(OUTPUT_DIR)
RELEASE_CXXFLAGS := $(CXXFLAGS) -O2
RELEASE_LIBDIR := $(LIBDIR)/Release
RELEASE_LDLIBS := $(LDFLAGS) $(LDLIBS) -L$(ROOT_MAIN)/../../Bin
RELEASE_OBJS := $(SOURCES:$(SRCDIR)/%.cpp=$(RELEASE_LIBDIR)/%.o)

include $(SOURCES:$(SRCDIR)/%.cpp=$($(TARGET)_LIBDIR)/Makefiles/%.d)

# ----------------------------------------- TARGET Rules

all: $(TARGET)

clean: $(TARGET)_clean

fclean: $(TARGET)_fclean

$(TARGET): $($(TARGET)_OUTPUT_DIR)/$($(TARGET)_EXE)

$($(TARGET)_OUTPUT_DIR)/$($(TARGET)_EXE): $($(TARGET)_OBJS)
	@ruby "$(ROOT_MAIN)/mkdir.rb" "$(dir $@)"
	@echo creation du fichier $@
	@$(CXX) -o $@ $^ $(EXE_CXXFLAGS) $($(TARGET)_CXXFLAGS) $($(TARGET)_LDLIBS)

$($(TARGET)_LIBDIR)/Makefiles/%.d: $(SRCDIR)/%.cpp
	@echo creation des regles de compilation pour le fichier $*.cpp
	@ruby "$(ROOT_MAIN)/mkdir.rb" "$(dir $@)"
	@$(CXX) -MM $(CXXFLAGS) $< | ruby "$(ROOT_MAIN)/rule.rb" "$@" "$<" "$(dir $(subst $(SRCDIR),,$<))" "$(CXX)" "$(CXXFLAGS)"
#	ruby "$(ROOT_MAIN)/rule.rb" "$@" "$<" "$(dir $(subst $(SRCDIR),,$<))" "$(CXX)" "$(CXXFLAGS)"


$(TARGET)_clean:
	@echo suppression des fichiers temporaires
	@ruby "$(ROOT_MAIN)/rm.rb" "$($(TARGET)_LIBDIR)"

$(TARGET)_fclean: $(TARGET)_clean
	@echo suppression du fichier généré
	@ruby "$(ROOT_MAIN)/rm.rb" "$($(TARGET)_OUTPUT_DIR)/$($(TARGET)_EXE)"

