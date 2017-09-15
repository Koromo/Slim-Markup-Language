#include <sml.h>
#include <CppUTest/CommandLineTestRunner.h>

using namespace sml;

TEST_GROUP(EXAMPLE_TABLE)
{
};

TEST(EXAMPLE_TABLE, ReadFile)
{
    const auto sml = parse("example.sml");
    CHECK(!!sml);
}

TEST(EXAMPLE_TABLE, length)
{
    const auto sml = parse("example.sml");
    CHECK(sml->length() == 10);
}

TEST(EXAMPLE_TABLE, contains)
{
    const auto sml = parse("example.sml");

    CHECK(sml->contains("v_int"));
    CHECK(sml->contains("v_iarr"));
    CHECK(sml->contains("v_arr_rec"));
    CHECK(sml->contains("t_singer"));

    CHECK_FALSE(sml->contains("notexists"));
    CHECK_FALSE(sml->contains("t_singer.child"));
}

TEST(EXAMPLE_TABLE, valueIs)
{
    const auto sml = parse("example.sml");

    CHECK(valueIs<integer_t>("v_int", sml));
    CHECK(valueIs<real_t>("v_real", sml));
    CHECK(valueIs<string_t>("v_str", sml));
    CHECK(valueIs<array_t>("v_iarr", sml));
    CHECK(valueIs<array_t>("v_arr_rec", sml));
    CHECK(valueIs<table_t>("t_singer", sml));

    CHECK_FALSE(valueIs<integer_t>("notexists", sml));
    CHECK_FALSE(valueIs<real_t>("v_int", sml));
}

TEST(EXAMPLE_TABLE, valueAs)
{
    const auto sml = parse("example.sml");

    CHECK(valueAs<integer_t>("v_int", sml) == 5);
    CHECK(valueAs<real_t>("v_real", sml) - 10.2 < 0.001);
    CHECK(valueAs<string_t>("v_str", sml) == "Example String.");
}

TEST(EXAMPLE_TABLE, GetTable)
{
    const auto sml = parse("example.sml");

    const auto& singer = valueAs<table_t>("t_singer", sml);
    CHECK(valueAs<integer_t>("size", singer) == 72);

    const auto& child = valueAs<table_t>("child", singer);
    CHECK(valueAs<string_t>("color", child) == "orange");
    CHECK(valueAs<integer_t>("size", child) == 75);
    CHECK(valueAs<string_t>( "food", child) == "lol");
}

TEST(EXAMPLE_TABLE, GetArray)
{
    const auto sml = parse("example.sml");

    const auto& iarr = valueAs<array_t>("v_iarr", sml);
    CHECK(iarr.length() == 3);

    const auto& arr_rec = valueAs<array_t>("v_arr_rec", sml);
    CHECK(arr_rec.length() == 3);
}

TEST_GROUP(EXAMPLE_ARRAY)
{
};

TEST(EXAMPLE_ARRAY, length)
{
    const auto sml = parse("example.sml");

    const auto& iarr = valueAs<array_t>("v_iarr", sml);
    CHECK(iarr.length() == 3);

    const auto& arr_rec = valueAs<array_t>("v_arr_rec", sml);
    CHECK(arr_rec.length() == 3);
}

TEST(EXAMPLE_ARRAY, arrayIs)
{
    const auto sml = parse("example.sml");

    const auto& iarr = valueAs<array_t>("v_iarr", sml);
    CHECK(arrayIs<integer_t>(iarr));
    CHECK_FALSE(arrayIs<real_t>(iarr));
    CHECK_FALSE(arrayIs<array_t>(iarr));

    const auto& arr_rec = valueAs<array_t>("v_arr_rec", sml);
    CHECK(arrayIs<array_t>(arr_rec));
    CHECK_FALSE(arrayIs<integer_t>(arr_rec));
}

TEST(EXAMPLE_ARRAY, valueAs)
{
    const auto sml = parse("example.sml");

    const auto& iarr = valueAs<array_t>("v_iarr", sml);
    CHECK(valueAs<integer_t>(0, iarr) == 4);
    CHECK(valueAs<integer_t>(1, iarr) == 2);
    CHECK(valueAs<integer_t>(2, iarr) == 5);

    const auto& arr_rec = valueAs<array_t>("v_arr_rec", sml);
    const auto& arr_rec_1 = valueAs<array_t>(1, arr_rec);
    CHECK(arrayIs<string_t>(arr_rec_1));
    CHECK(arr_rec_1.length() == 3);
    CHECK(valueAs<string_t>(2, arr_rec_1) == "str");
}

TEST_GROUP(EXAMPLE_TABLE_ARRAY)
{
};

TEST(EXAMPLE_TABLE_ARRAY, GetTableArray)
{
    const auto sml = parse("example.sml");

    const auto& arr = valueAs<array_t>("tarr", sml);
    CHECK(arr.length() == 2);
    CHECK(arrayIs<table_t>(arr));

    const auto& arr_0 = valueAs<table_t>(0, arr);
    CHECK(arr_0.length() == 1);
    CHECK(valueIs<integer_t>("id", arr_0));
    CHECK(valueAs<integer_t>("id", arr_0) == 10);

    const auto& arr_1 = valueAs<table_t>(1, arr);
    CHECK(arr_1.length() == 1);
    CHECK(valueIs<integer_t>("kcal", arr_1));
    CHECK(valueAs<integer_t>("kcal", arr_1) == 44);
}

TEST(EXAMPLE_TABLE_ARRAY, GetChildTableArray)
{
    const auto sml = parse("example.sml");

    const auto& singer = valueAs<table_t>("t_singer", sml);
    CHECK(valueIs<array_t>("cute", singer));

    const auto& arr = valueAs<array_t>("cute", singer);
    CHECK(arr.length() == 2);
    CHECK(arrayIs<table_t>(arr));

    const auto& arr_0 = valueAs<table_t>(0, arr);
    CHECK(arr_0.length() == 1);
    CHECK(valueIs<string_t>("type", arr_0));
    CHECK(valueAs<string_t>("type", arr_0) == "Cool");

    const auto& arr_1 = valueAs<table_t>(1, arr);
    CHECK(arr_1.length() == 1);
    CHECK(valueIs<string_t>("who", arr_1));
    CHECK(valueAs<string_t>("who", arr_1) == "superman");
}

TEST_GROUP(EXAMPLE_TABLE_PATH)
{
};

TEST(EXAMPLE_TABLE_PATH, Priority)
{
    const auto sml = parse("example.sml");

	const auto& usa = valueAs<array_t>("usa", sml);

    const auto& usa_0 = valueAs<table_t>(0, usa);
    CHECK(usa_0.length() == 0);

    const auto& usa_1 = valueAs<table_t>(1, usa);
    CHECK(usa_1.length() == 1);
    CHECK(valueIs<table_t>("min", usa_1));

    const auto& min = valueAs<table_t>("min", usa_1);
    CHECK(min.length() == 1);
    CHECK(valueIs<integer_t>("age", min));
    CHECK(valueAs<integer_t>("age", min) == 27);
}