#include <hermes/hermes.h>
#include <hermes/cdp/CDPAgent.h>
#include <iostream>

/// JS code to be executed.
static const char *code = R"(
  let test = "foobar";
  print(`Hello, ${test}`);
  throw Error("Surprise!");
)";

int main() {
  // You can Customize the runtime config here.
  auto runtimeConfig =
      hermes::vm::RuntimeConfig::Builder().withIntl(false).build();

  // facebook::hermes::cdp::CDPAgent cdpAgent()

  // Create the Hermes runtime.
  auto runtime = facebook::hermes::makeHermesRuntime(runtimeConfig);

  // Execute some JS.
  int status = 0;
  try {
      runtime->evaluateJavaScript(
          std::make_unique<facebook::jsi::StringBuffer>(code), "main.js");
  } catch (facebook::jsi::JSError &e) {
      // Handle JS exceptions here.
      std::cerr << "JS Exception: " << e.getStack() << std::endl;
      status = 1;
  } catch (facebook::jsi::JSIException &e) {
      // Handle JSI exceptions here.
      std::cerr << "JSI Exception: " << e.what() << std::endl;
      status = 1;
  }

  return status;
}