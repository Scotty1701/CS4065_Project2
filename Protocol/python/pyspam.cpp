#include <pybind11/pybind11.h>
#include "spam_api.hpp"

namespace py = pybind11;



PYBIND11_MODULE(pyspam, m) {
    m.def("gen_request_connect", &spam_api::gen_request_connect);
}
