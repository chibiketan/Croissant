#pragma once
#ifndef COMMANDLINE_H_
#define COMMANDLINE_H_

#include <memory>
#include <stdio.h>
#include <stdint.h>
#include <cstring>
#include <list>
#include "Core/CommandLineElement.hpp"
#include "Engine.hpp"
#include "Core/Pimpl.hpp"

#include "Core/String.hpp"

namespace Croissant
{
namespace Core
{
class ENGINE_API CommandLine : public Pimpl<CommandLine>::Detail
{
public:
    CommandLine(std::unique_ptr<CommandLineElement> elements[], std::size_t elementsSize);
    CommandLine(std::unique_ptr<CommandLineElement> elements[], std::size_t elementsSize, const char commmandPrefix);
    CommandLine(const CommandLine& source) = delete;
    CommandLine& operator=(const CommandLine& source) = delete;
    bool Initialize(size_t argc, const char** argv);
    const String& ProgramName(void) const;
    bool Bool(const char* commandName) const;

};
}
}

#endif
