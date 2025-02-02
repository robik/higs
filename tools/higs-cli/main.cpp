#include <iostream>
#include <vector>

#include <fmt/format.h>
#include <hermes/hermes.h>
#include <higs/common.hpp>
#include <higs/runtime.hpp>
#include <boost/program_options.hpp>

namespace po = boost::program_options;

int main(int argc, char** argv)
{
    std::vector<std::string> evalStrings;
    po::options_description general_opts { "General options" };
    general_opts.add_options()
        ("help", "Print help message")
        ("eval,e", po::value<std::vector<std::string>>(&evalStrings), "Evaluate a string of JS code");

    po::positional_options_description args;
    // TODO: support
    args.add("input-file", -1);

    po::variables_map vm;
    auto parser = po::command_line_parser(argc, argv).options(general_opts).positional(args);
    po::store(parser.run(), vm);
    po::notify(vm);

    if (vm.count("help")) {
        std::cout << general_opts << "\n";
        return 1;
    }

    auto rt = higs::Runtime::create();
    auto& env = rt->mainEnvironment();

    // Execute some JS.
    int status = 0;
    try {
        if (!evalStrings.empty()) {
            for (auto& evalStr: evalStrings) {
                auto script = fmt::format("print({});", evalStr);
                env.evaluateScript(script);
            }
        }
    }
    catch (jsi::JSError& e) {
        // Handle JS exceptions here.
        std::cerr << "JS Exception: " << e.getStack() << std::endl;
        status = 1;
    }
    catch (jsi::JSIException& e) {
        // Handle JSI exceptions here.
        std::cerr << "JSI Exception: " << e.what() << std::endl;
        status = 1;
    }

    return status;
}
