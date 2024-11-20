#include <unordered_set>
#include <algorithm>
#include <string_view>
#include "gtest/gtest.h"
#include "esport_data.h"
#include "faker-cxx/esport.h"

using namespace ::testing;
using namespace faker;
using namespace faker::esport;

namespace
{
const struct EsportDefinition& getEsportDefinition(Locale locale)
{
    switch (locale)
    {
    default:
        return enUSEsportDefinition;
    }
}
}

class EsportTest : public TestWithParam<Locale>
{
public:
};

// Ensure that generated players are valid for the given locale
TEST_P(EsportTest, shouldGenerateValidPlayerForLocale)
{
    const auto locale = GetParam();
    const auto& esportDefinition = getEsportDefinition(locale);

    const auto generatedPlayer = player(locale);

    ASSERT_FALSE(generatedPlayer.empty()) << "Generated player is empty.";
    ASSERT_TRUE(std::ranges::any_of(esportDefinition.players, [generatedPlayer](const std::string_view& player)
                                    { return generatedPlayer == player; }))
        << "Generated player is not valid for the locale.";
}

// Ensure unique players for a locale
TEST_P(EsportTest, shouldGenerateUniquePlayersForLocale)
{
    const auto locale = GetParam();
    std::unordered_set<std::string> uniquePlayers;
    constexpr int iterations = 100;

    for (int i = 0; i < iterations; ++i)
    {
        uniquePlayers.insert(player(locale));
    }

    ASSERT_GT(uniquePlayers.size(), 1) << "Player generation is not sufficiently unique for the locale.";
}

// Test for invalid or unsupported locale
TEST(EsportInvalidLocaleTest, shouldHandleInvalidLocaleGracefully)
{
    const Locale invalidLocale = static_cast<Locale>(999); // Invalid locale
    const auto generatedPlayer = player(invalidLocale);

    ASSERT_FALSE(generatedPlayer.empty()) << "Generated player should fallback to a default locale, but is empty.";
}

// Ensure all data types are generated consistently for a locale
TEST_P(EsportTest, shouldGenerateAllTypesForLocale)
{
    const auto locale = GetParam();
    const auto& esportDefinition = getEsportDefinition(locale);

    const auto generatedPlayer = player(locale);
    const auto generatedTeam = team(locale);
    const auto generatedLeague = league(locale);
    const auto generatedEvent = event(locale);
    const auto generatedGame = game(locale);

    ASSERT_FALSE(generatedPlayer.empty()) << "Generated player is empty.";
    ASSERT_FALSE(generatedTeam.empty()) << "Generated team is empty.";
    ASSERT_FALSE(generatedLeague.empty()) << "Generated league is empty.";
    ASSERT_FALSE(generatedEvent.empty()) << "Generated event is empty.";
    ASSERT_FALSE(generatedGame.empty()) << "Generated game is empty.";

    ASSERT_TRUE(std::ranges::any_of(esportDefinition.players, [generatedPlayer](const std::string_view& player)
                                    { return generatedPlayer == player; }));
    ASSERT_TRUE(std::ranges::any_of(esportDefinition.teams, [generatedTeam](const std::string_view& team)
                                    { return generatedTeam == team; }));
    ASSERT_TRUE(std::ranges::any_of(esportDefinition.leagues, [generatedLeague](const std::string_view& league)
                                    { return generatedLeague == league; }));
    ASSERT_TRUE(std::ranges::any_of(esportDefinition.events, [generatedEvent](const std::string_view& event)
                                    { return generatedEvent == event; }));
    ASSERT_TRUE(std::ranges::any_of(esportDefinition.games, [generatedGame](const std::string_view& game)
                                    { return generatedGame == game; }));
}

// Test bulk generation for all esport data types
TEST_P(EsportTest, shouldBulkGenerateDataForLocale)
{
    const auto locale = GetParam();
    constexpr int iterations = 500;

    for (int i = 0; i < iterations; ++i)
    {
        ASSERT_FALSE(player(locale).empty());
        ASSERT_FALSE(team(locale).empty());
        ASSERT_FALSE(league(locale).empty());
        ASSERT_FALSE(event(locale).empty());
        ASSERT_FALSE(game(locale).empty());
    }
}

INSTANTIATE_TEST_SUITE_P(TestEsportByLocale, EsportTest, ValuesIn(locales),
                         [](const TestParamInfo<Locale>& paramInfo) { return toString(paramInfo.param); });
