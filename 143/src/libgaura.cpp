#include "libgaura.h"


const char * GauraException::what() const noexcept {
  return "Gaura exception";
}

const char * GauraExceptionMemfail::what() const noexcept {
  return "Memory writing exception";
}

const char * GauraExceptionAccess::what() const noexcept {
 return "Memory access exception";
}
