//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.
//
#include <gtest/gtest.h>
#include <higs/runtime.hpp>
#include <jsrt/conv.hpp>

using namespace higs;
namespace conv = jsrt::conv;

TEST(TestConvToJS, NullToJS)
{
    auto host = Runtime::create();
    auto&& ctx = host->mainEnvironment();
    auto&& jsVal = conv::toJS(nullptr, ctx);

    EXPECT_EQ(jsVal.isNull(), true);
}

TEST(TestConvToJS, BoolToJS)
{
    auto host = Runtime::create();
    auto&& ctx = host->mainEnvironment();
    auto&& jsVal = conv::toJS(true, ctx);

    EXPECT_EQ(jsVal.isBool(), true);
    EXPECT_EQ(jsVal.asBool(), true);
}

TEST(TestConvToJS, Int32ToJS)
{
    auto host = Runtime::create();
    auto&& ctx = host->mainEnvironment();
    int32_t num = -123456;

    auto&& jsVal = conv::toJS(num, ctx);

    EXPECT_EQ(jsVal.isNumber(), true);
    EXPECT_EQ(jsVal.asNumber(), num);
}

TEST(TestConvToJS, UInt32ToJS)
{
    auto host = Runtime::create();
    auto&& ctx = host->mainEnvironment();
    uint32_t num = 9999100;

    auto&& jsVal = conv::toJS(num, ctx);

    EXPECT_EQ(jsVal.isNumber(), true);
    EXPECT_EQ(jsVal.asNumber(), num);
}

//
// ------------------------------------------------------------------------------------------------- Floating
//
TEST(TestConvToJS, FloatToJS)
{
    auto host = Runtime::create();
    auto&& ctx = host->mainEnvironment();
    float num = 9999100.12321f;

    auto&& jsVal = conv::toJS(num, ctx);

    EXPECT_EQ(jsVal.isNumber(), true);
    EXPECT_EQ(jsVal.asNumber(), num);
}

TEST(TestConvToJS, DoubleToJS)
{
    auto host = Runtime::create();
    auto&& ctx = host->mainEnvironment();
    double num = 9999100.12321211;

    auto&& jsVal = conv::toJS(num, ctx);

    EXPECT_EQ(jsVal.isNumber(), true);
    EXPECT_EQ(jsVal.asNumber(), num);
}

//
// ------------------------------------------------------------------------------------------------- String
//
TEST(TestConvToJS, StdStringToJS)
{
    auto host = Runtime::create();
    auto&& ctx = host->mainEnvironment();
    std::string val = "testą🚀";

    auto jsVal = conv::toJS(val, ctx);

    EXPECT_EQ(jsVal.isString(), true);
    EXPECT_EQ(std::move(jsVal).asString(ctx).utf8(ctx), val);
}

TEST(TestConvToJS, StdU16StringToJS)
{
    auto host = Runtime::create();
    auto&& ctx = host->mainEnvironment();
    std::u16string val = u"testą🚀";

    auto jsVal = conv::toJS(val, ctx);

    EXPECT_EQ(jsVal.isString(), true);
    EXPECT_EQ(std::move(jsVal).asString(ctx).utf16(ctx), val);
}

//
// ------------------------------------------------------------------------------------------------- Functions
//
TEST(TestConvToJS, Function_void_void_ToJS)
{
    auto host = Runtime::create();
    auto&& ctx = host->mainEnvironment();
    int called = 0;
    std::function func = [&]() { called += 1; };

    auto jsFunc = conv::toJS(func, ctx);
    EXPECT_EQ(called, 0);

    EXPECT_EQ(jsFunc.isObject(), true);
    jsFunc.asObject(ctx).asFunction(ctx).call(ctx);
    EXPECT_EQ(called, 1);
}

TEST(TestConvToJS, Function_int_void_ToJS)
{
    auto host = Runtime::create();
    auto&& ctx = host->mainEnvironment();
    int called = 0;
    std::function func = [&]() { called += 1; return 5; };

    auto jsFunc = conv::toJS(func, ctx);
    EXPECT_EQ(called, 0);

    EXPECT_EQ(jsFunc.isObject(), true);
    jsFunc.asObject(ctx).asFunction(ctx).call(ctx);
    EXPECT_EQ(called, 1);
}
