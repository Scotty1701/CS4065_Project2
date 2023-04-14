#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include "spam_api.hpp"


PYBIND11_MODULE(pyspam, m) {
    auto m_gen = m.def_submodule("gen");

    auto m_g_request = m_gen.def_submodule("request");
    m_g_request.def("connect", &spam_api::gen::request::connect);
    m_g_request.def("join", &spam_api::gen::request::join);
    m_g_request.def("post", &spam_api::gen::request::post);
    m_g_request.def("message", &spam_api::gen::request::message);
    m_g_request.def("leave", &spam_api::gen::request::leave);
    m_g_request.def("getusers", &spam_api::gen::request::getusers);

    auto m_respond = m_gen.def_submodule("respond");
    m_respond.def("connect", &spam_api::gen::respond::connect);
    m_respond.def("join", &spam_api::gen::respond::join);
    m_respond.def("post", &spam_api::gen::respond::post);
    m_respond.def("message", &spam_api::gen::respond::message);
    m_respond.def("leave", &spam_api::gen::respond::leave);
    m_respond.def("getusers", &spam_api::gen::respond::getusers);

    m.def("parse", &spam_api::parse);
}
