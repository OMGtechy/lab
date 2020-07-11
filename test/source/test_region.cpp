#include "test_lab/test.h"
#include "lab/region.h"

SCENARIO("lab::Region", "[region]") {
    GIVEN("an empty region") {
        lab::Region region;
        WHEN("its size is queried") {
            const auto size = region.size();
            THEN("it is 0") {
                REQUIRE(size == 0);
            }
        }
    }

    GIVEN("an empty region") {
        lab::Region region;
        WHEN("an objectID is added to it") {
            const lab::Region::id_t added = 42;
            region.add(added);

            THEN("its size is 1") {
                REQUIRE(region.size() == 1);
            }

            THEN("it contains the objectID added") {
                REQUIRE(region.contains(added));
            }

            THEN("it does not contain another objectID") {
                REQUIRE(!region.contains(0));
                REQUIRE(!region.contains(1));
                REQUIRE(!region.contains(4242));
                REQUIRE(!region.contains(24));
                REQUIRE(!region.contains(999999999));
            }
        }
    }

    GIVEN("a region with 3 things in it") {
        lab::Region region;
        region.add(1);
        region.add(2);
        region.add(3);

        // not strictly part of this test, but is an assumption
        REQUIRE(region.contains(1));
        REQUIRE(region.contains(2));
        REQUIRE(region.contains(3));
        REQUIRE(region.size() == 3);

        WHEN("something that is currently in the region is removed") {
            const bool wasRemoved = region.remove(2);

            THEN("the function returns that it was removed") {
                REQUIRE(wasRemoved);
            }

            THEN("the size is reduced by 1") {
                REQUIRE(region.size() == 2);
            }

            THEN("the region no longer contains the removed thing") {
                REQUIRE(!region.contains(2));
            }

            THEN("the region still contains everything else") {
                REQUIRE(region.contains(1));
                REQUIRE(region.contains(3));
            }
        }
    }

    GIVEN("a region with 3 things in it") {
        lab::Region region;
        region.add(1);
        region.add(2);
        region.add(3);

        // not strictly part of this test, but is an assumption
        REQUIRE(region.contains(1));
        REQUIRE(region.contains(2));
        REQUIRE(region.contains(3));
        REQUIRE(!region.contains(4));
        REQUIRE(region.size() == 3);

        WHEN("something that is not currently in the region is removed") {
            const bool wasRemoved = region.remove(4);

            THEN("the function returns that it was not removed") {
                REQUIRE(!wasRemoved);
            }

            THEN("the size stays the same") {
                REQUIRE(region.size() == 3);
            }

            THEN("the region still doesn't contain the thing") {
                REQUIRE(!region.contains(4));
            }

            THEN("the region still contains everything else") {
                REQUIRE(region.contains(1));
                REQUIRE(region.contains(2));
                REQUIRE(region.contains(3));
            }
        }
    }
}
