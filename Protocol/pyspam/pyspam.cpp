#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include "spam_api.hpp"

namespace py = pybind11;

PYBIND11_MODULE(pyspam, m) {
    auto m_gen = m.def_submodule("gen");

    auto m_g_request = m_gen.def_submodule("request");
    m_g_request.def("connect", &spam_api::gen::request::connect);
    m_g_request.def("join", &spam_api::gen::request::join);
    m_g_request.def("post", &spam_api::gen::request::post);
    m_g_request.def("message", &spam_api::gen::request::message);
    m_g_request.def("leave", &spam_api::gen::request::leave);
    m_g_request.def("getusers", &spam_api::gen::request::getusers);
    m_g_request.def("getgroups", &spam_api::gen::request::getgroups);

    auto m_respond = m_gen.def_submodule("respond");
    m_respond.def("connect", &spam_api::gen::respond::connect);
    m_respond.def("join", &spam_api::gen::respond::join);
    m_respond.def("post", &spam_api::gen::respond::post);
    m_respond.def("message", py::overload_cast<std::string, std::string, std::string, std::string, std::string, std::string>(&spam_api::gen::respond::message));
    m_respond.def("message", py::overload_cast<bool, std::string>(&spam_api::gen::respond::message));
    m_respond.def("leave", &spam_api::gen::respond::leave);
    m_respond.def("getusers", py::overload_cast<std::vector<std::string>>(&spam_api::gen::respond::getusers));
    m_respond.def("getusers", py::overload_cast<bool, std::string>(&spam_api::gen::respond::getusers));
    m_respond.def("getgroups", py::overload_cast<std::vector<std::string>>(&spam_api::gen::respond::getgroups));
    m_respond.def("getgroups", py::overload_cast<bool, std::string>(&spam_api::gen::respond::getgroups));

    m.def("parse", &spam_api::parse);
}
