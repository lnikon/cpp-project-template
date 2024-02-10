#include "ExampleIncludeDir/ExampleHeader.h"

std::string SayHelloTo(const std::string& whom) noexcept
{
  return std::string("Hello, ") + whom + std::string("!");
}
