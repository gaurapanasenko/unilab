#include "libgaura.h"

const char* GauraException::what() const noexcept {
  return "Gaura exception";
}
