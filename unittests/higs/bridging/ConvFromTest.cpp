//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.
//
#include <gtest/gtest.h>
#include <higs/runtime.hpp>
#include <jsrt/conv.hpp>

using namespace higs;
namespace conv = jsrt::conv;

TEST(TestConvFromJS, JSToBool)
{
    auto host = Runtime::create();
    auto&& ctx = host->mainEnvironment();

    jsi::Value jsValue { false };
    auto&& val = conv::fromJS<bool>(jsValue, ctx);

    EXPECT_EQ(val, false);
}

TEST(TestConvFromJS, JSToInt32)
{
    auto host = Runtime::create();
    auto&& ctx = host->mainEnvironment();

    jsi::Value jsValue { 150 };
    auto&& val = conv::fromJS<int32_t>(jsValue, ctx);

    EXPECT_EQ(val, 150);
}

TEST(TestConvFromJS, FromUInt32)
{
    auto host = Runtime::create();
    auto&& ctx = host->mainEnvironment();

    jsi::Value jsValue { 150 };
    auto&& val = conv::fromJS<uint32_t>(jsValue, ctx);

    EXPECT_EQ(val, 150);
}

//
// ------------------------------------------------------------------------------------------------- String
//
TEST(TestConvFromJS, FromStdString)
{
    auto host = Runtime::create();
    auto&& ctx = host->mainEnvironment();

    jsi::Value jsValue { jsi::String::createFromUtf8(ctx, "testąą🚀") };
    auto&& val = conv::fromJS<std::string>(jsValue, ctx);

    EXPECT_EQ(val, "testąą🚀");
}

TEST(TestConvFromJS, FromStdU16String)
{
    auto host = Runtime::create();
    auto&& ctx = host->mainEnvironment();

    jsi::Value jsValue { jsi::String::createFromUtf16(ctx, u"testąą🚀") };
    auto&& val = conv::fromJS<std::u16string>(jsValue, ctx);

    EXPECT_EQ(val, u"testąą🚀");
}

//
// ------------------------------------------------------------------------------------------------- Functions
//
// TEST(TestConvFromJS, FromFunction_void_void)
// {
//     auto host = std::make_unique<Host>();
//     auto&& ctx = host->defaultContext();
//     int called = 0;
//     std::function func = [&]() { called += 1; };
//
//     auto jsFunc = bridging::toJS(func, ctx);
//     EXPECT_EQ(called, 0);
//
//     EXPECT_EQ(jsFunc.isObject(), true);
//     jsFunc.asObject(ctx).asFunction(ctx).call(ctx);
//     EXPECT_EQ(called, 1);
// }

//
// ------------------------------------------------------------------------------------------------- Combinator types (simple)
//
TEST(TestConvFromJS, FromOptionalInt)
{
    auto host = Runtime::create();
    auto&& ctx = host->mainEnvironment();

    jsi::Value jsValue { nullptr };
    auto&& val = conv::fromJS<std::optional<bool>>(jsValue, ctx);

    EXPECT_EQ(val.has_value(), false);
}

TEST(TestConvFromJS, FromOptionalInt2)
{
    auto host = Runtime::create();
    auto&& ctx = host->mainEnvironment();

    jsi::Value jsValue { 10 };
    auto&& val = conv::fromJS<std::optional<int>>(jsValue, ctx);

    EXPECT_EQ(val.has_value(), true);
    EXPECT_EQ(val.value(), 10);
}

TEST(TestConvFromJS, FromOptionalBool)
{
    auto host = Runtime::create();
    auto&& ctx = host->mainEnvironment();

    jsi::Value jsValue { false };
    auto&& val = conv::fromJS<std::optional<bool>>(jsValue, ctx);

    EXPECT_EQ(val.has_value(), true);
    EXPECT_EQ(val.value(), false);
}

TEST(TestConvFromJS, FromOptionalNull)
{
    auto host = Runtime::create();
    auto&& ctx = host->mainEnvironment();

    jsi::Value jsValue { nullptr };
    auto&& val = conv::fromJS<std::optional<bool>>(jsValue, ctx);

    EXPECT_EQ(val.has_value(), false);
}

TEST(TestConvFromJS, FromOptionalUndefined)
{
    auto host = Runtime::create();
    auto&& ctx = host->mainEnvironment();

    jsi::Value jsValue = jsi::Value::undefined();
    auto&& val = conv::fromJS<std::optional<bool>>(jsValue, ctx);

    EXPECT_EQ(val.has_value(), false);
}

TEST(TestConvFromJS, FromVector1)
{
    auto host = Runtime::create();
    auto&& ctx = host->mainEnvironment();

    jsi::Value jsValue { jsi::Array::createWithElements(ctx, { 1, 2, 3}) };
    auto&& val = conv::fromJS<std::vector<int>>(jsValue, ctx);

    EXPECT_EQ(val.size(), 3);
    EXPECT_EQ(val[0], 1);
    EXPECT_EQ(val[1], 2);
    EXPECT_EQ(val[2], 3);
}

TEST(TestConvFromJS, FromVector2)
{
    auto host = Runtime::create();
    auto&& ctx = host->mainEnvironment();

    jsi::Value jsValue { false };

    auto&& val = conv::tryFromJS<std::vector<int>>(jsValue, ctx);
    EXPECT_EQ(val.hasValue(), false);
    EXPECT_EQ(val.hasError(), true);
}

TEST(TestConvFromJS, FromArrayMatchingSize)
{
    auto host = Runtime::create();
    auto&& ctx = host->mainEnvironment();

    jsi::Value jsValue { jsi::Array::createWithElements(ctx, { 1, 2, 3}) };

    auto&& val = conv::fromJS<std::array<int, 3>>(jsValue, ctx);
    EXPECT_EQ(val.size(), 3);
    EXPECT_EQ(val[0], 1);
    EXPECT_EQ(val[1], 2);
    EXPECT_EQ(val[2], 3);
}

TEST(TestConvFromJS, FromArrayTooSmall)
{
    auto host = Runtime::create();
    auto&& ctx = host->mainEnvironment();

    jsi::Value jsValue { jsi::Array::createWithElements(ctx, { 1, 2, 3}) };

    auto&& val = conv::tryFromJS<std::array<int, 5>>(jsValue, ctx);
    EXPECT_EQ(val.hasValue(), false);
    EXPECT_EQ(val.hasError(), true);
}

TEST(TestConvFromJS, FromArrayTooSmallOptional)
{
    auto host = Runtime::create();
    auto&& ctx = host->mainEnvironment();

    jsi::Value jsValue { jsi::Array::createWithElements(ctx, { 1, 2, 3}) };

    auto&& val = conv::fromJS<std::optional<std::array<int, 5>>>(jsValue, ctx);
    EXPECT_EQ(val.has_value(), false);
}

//
// ------------------------------------------------------------------------------------------------- Combinator types (nested)
//

TEST(TestConvFromJS, FromVectorOrOptionals)
{
    auto host = Runtime::create();
    auto&& ctx = host->mainEnvironment();

    jsi::Value jsValue { jsi::Array::createWithElements(ctx, { 1, false, 3, nullptr}) };

    auto&& val = conv::fromJS<std::vector<std::optional<int>>>(jsValue, ctx);
    EXPECT_EQ(val.size(), 4);
    EXPECT_EQ(val[0].has_value(), true);
    EXPECT_EQ(val[1].has_value(), false);
    EXPECT_EQ(val[2].has_value(), true);
    EXPECT_EQ(val[3].has_value(), false);

    EXPECT_EQ(val[0].value(), 1);
    EXPECT_EQ(val[2].value(), 3);
}