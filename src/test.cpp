#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/functional.h>

#include <fmt/format.h>

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <functional>

namespace py = pybind11;
using namespace pybind11::literals;

struct AdultPerson
{
    size_t age;
    std::string name;

public:
    auto get_age() const
    {
        return age;
    }

    void set_age(size_t new_age)
    {
        if (new_age < 18)
        {
            throw std::runtime_error{"too young"};
        }
        age = new_age;
    }

    auto get_name() const
    {
        return name;
    }

    static void greed(const std::string &name)
    {
        std::cout << "Hello, " << name << "!" << std::endl;
    }

    static void greed()
    {
        std::cout << "Hello!" << std::endl;
    }
};

enum class Color
{
    BLACK = 0,
    WHITE = 1,
};

PYBIND11_MODULE(test, m)
{
    m.doc() = R"doc(
        My test bindings
        ----------------
    )doc";

    /* Basic */
    m.def("hello_world", []
          { std::cout << "Hello, world" << std::endl; });

    m.def(
        "greet", [](const std::string &name)
        { return "Hello, " + name + "!"; },
        "Function that greet person");

    m.def(
        "add_int", [](int a, int b)
        { return a + b; },
        py::arg("a"), py::arg("b") = 0);

    m.def(
        "add_float", [](double a, double b)
        { return a + b; },
        "a"_a, "b"_a = 0.0);

    /* STL */
    m.def(
        "get_n_zeros", [](size_t n)
        { 
            std::vector<double> result(n, 0.0);  
            return result; },
        "n"_a = 10);

    m.def("get_map", []()
          { 
        std::map<std::string, size_t> m;
        m.insert({"a", 1});
        m.insert({"b", 1});
        return m; });

    /* Attributes */
    m.attr("int_number") = 12;
    m.attr("float_number") = 1.2;
    m.attr("ones_list") = std::vector<size_t>(10, 1);

    /* Class */
    py::class_<AdultPerson> adult_person(m, "AdultPerson");
    adult_person.def(py::init<size_t, std::string>(), "age"_a, "name"_a);
    adult_person.def_property("age", &AdultPerson::get_age, &AdultPerson::set_age);
    adult_person.def("get_name", &AdultPerson::get_age);
    adult_person.def_static("greed", py::overload_cast<const std::string &>(&AdultPerson::greed), "name"_a);
    adult_person.def_static("greed", py::overload_cast<>(&AdultPerson::greed));
    adult_person.def("__repr__", [](AdultPerson &self)
                     { return fmt::format("AdultPerson(age={}, name={})", self.age, self.name); });

    /* Enum */
    py::enum_<Color>(m, "Color")
        .value("WHITE", Color::WHITE)
        .value("BLACK", Color::BLACK);

    /* Functional */
    m.def("call_callback", [](const std::function<std::vector<double>(size_t n)>& cb, size_t n) {
        std::cout << "Calling callback..." << std::endl;
        auto res = cb(n);
        return res;
    });

    /* py::object */
    m.def("safe_get_attr", [](py::object obj, const std::string& attr) {
        auto value = obj.attr(attr.c_str());
        py::print(obj, ".attr(", attr, ") = ", value);
        return value;
    });

    /*
    t1 --------------
    t2 --------------
    */

    m.def("unsafe_get_attr", [](py::object obj, const std::string& attr) {
        py::gil_scoped_release release;
        auto value = obj.attr(attr.c_str());
        py::print(obj, ".attr(", attr, ") = ", value);
        return value;
    });
}