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

    auto m_parse = m.def_submodule("parse");

    auto m_p_request = m_parse.def_submodule("request");
    m_p_request.def("connect", &spam_api::parse::request::connect);
    m_p_request.def("join", &spam_api::parse::request::join);
    m_p_request.def("post", &spam_api::parse::request::post);
    m_p_request.def("message", &spam_api::parse::request::message);
    m_p_request.def("leave", &spam_api::parse::request::leave);
    m_p_request.def("getusers", &spam_api::parse::request::getusers);
}
