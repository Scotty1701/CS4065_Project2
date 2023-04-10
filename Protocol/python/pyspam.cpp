#include <pybind11/pybind11.h>
#include "spam_api.hpp"


PYBIND11_MODULE(pyspam, m) {
    m.def("gen_request_connect", &spam_api::gen_request_connect);
    m.def("gen_respond_connect", &spam_api::gen_respond_connect);
    m.def("gen_request_join", &spam_api::gen_request_join);
    m.def("gen_respond_join", &spam_api::gen_respond_join);
    m.def("gen_request_post", &spam_api::gen_request_post);
    m.def("gen_respond_post", &spam_api::gen_respond_post);
    m.def("gen_request_message", &spam_api::gen_request_message);
    m.def("gen_respond_message", &spam_api::gen_respond_message);
    m.def("gen_request_leave", &spam_api::gen_request_leave);
    m.def("gen_respond_leave", &spam_api::gen_respond_leave);
    m.def("gen_request_getusers", &spam_api::gen_request_getusers);
    m.def("gen_respond_getusers", &spam_api::gen_respond_getusers);
}
