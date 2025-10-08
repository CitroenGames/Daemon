#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "CommandSystem.h"

namespace Cmd {
namespace {
using ::testing::Eq;

TEST(NamespaceFold, Empty)
{
    CompletionResult input = {};
    CompletionResult expected = {};
    NamespaceFold(input);
    ASSERT_THAT(input, Eq(expected));
}

TEST(NamespaceFold, NoFoldSharedPrefix)
{
    CompletionResult input = {{"a.c", "description of a.c"}, {"a.b", "description of a.b"}};
    CompletionResult expected = {{"a.b", "description of a.b"}, {"a.c", "description of a.c"}};
    NamespaceFold(input);
    ASSERT_THAT(input, Eq(expected));
}

TEST(NamespaceFold, MultipleNamespacesDots)
{
    CompletionResult input =
        {{"dretch.speed", "dretch speed"}, {"mara.voltage", "mara voltage"},
        {"dretch.strength", "dretch strength"}, {"granger.size.x", "granger size x"},
        {"granger.size.z", "granger size z"}};
    CompletionResult expected =
        {{"dretch.", ""}, {"granger.", ""}, {"mara.voltage", "mara voltage"}};
    NamespaceFold(input);
    ASSERT_THAT(input, Eq(expected));
}

TEST(NamespaceFold, MultipleNamespacesUnderscores)
{
    CompletionResult input =
        {{"g_bot_skill_climbing", "bot climbing skill"},
        {"g_bot_skill_crawling", "bot crawling skill"}, {"g_bot_buy", "bot buy"},
        {"g_bot_infinite_funds", "bot infinite funds"}};
    CompletionResult expected =
        {{"g_bot_buy", "bot buy"}, {"g_bot_infinite_funds", "bot infinite funds"},
        {"g_bot_skill_", ""}};
    NamespaceFold(input);
    ASSERT_THAT(input, Eq(expected));
}

} // namespace
} // namespace Cmd

