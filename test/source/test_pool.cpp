#include "test_lab/test.h"
#include "lab/pool.h"

SCENARIO("lab::Pool" , "[pool]") {
    GIVEN("a pool of a POD type with no initial size") {
        struct TestPOD {
            int m_data;
        };

        lab::Pool<TestPOD> pool;
        WHEN("I take an instance from the pool") {
            {
                auto instance = pool.takeInstance();
                THEN("the instance can be modified") {
                    instance->m_data = 42;
                    REQUIRE(instance->m_data == 42);
                    instance->m_data = 123;
                    REQUIRE(instance->m_data == 123);
                }

                THEN("the pool has a size of at least 1") {
                    REQUIRE(pool.getSize() >= 1);
                }

                THEN("the number of taken instances is 1") {
                    REQUIRE(pool.getTaken() == 1);
                }
            }

            AND_WHEN("I take check the number of instances taken after the previous went out of scope") {
                THEN("it is 0") {
                    REQUIRE(pool.getTaken() == 0);
                }
            }
        }
    }

    GIVEN("a pool of a non-copyable but moveable type with no initial size") {
        struct NonCopyableButMoveable {
            // default construtible
            NonCopyableButMoveable() = default;

            // non-copyable
            NonCopyableButMoveable(const NonCopyableButMoveable&) = delete;
            NonCopyableButMoveable& operator=(const NonCopyableButMoveable&) = delete;

            // moveable
            NonCopyableButMoveable(NonCopyableButMoveable&& other) { *this = std::move(other); }
            NonCopyableButMoveable& operator=(NonCopyableButMoveable&& other) { other.m_movedFrom = true; }

            bool m_movedFrom = false;
        };

        lab::Pool<NonCopyableButMoveable> pool;
        WHEN("I take an instance from the pool") {

            auto instance = pool.takeInstance();            
            THEN("it has not been moved from") {
                REQUIRE(!instance->m_movedFrom);
            }
        }
    }

    GIVEN("a pool of a non-copyable non-moveable type with no initial size") {
        struct NonCopyableNonMoveable {
            // default construtible
            NonCopyableNonMoveable() = default;

            // non-copyable
            NonCopyableNonMoveable(const NonCopyableNonMoveable&) = delete;
            NonCopyableNonMoveable& operator=(const NonCopyableNonMoveable&) = delete;

            // moveable
            NonCopyableNonMoveable(NonCopyableNonMoveable&& other) = delete;
            NonCopyableNonMoveable& operator=(NonCopyableNonMoveable&& other) = delete;

            int32_t m_data = 0;
        };

        lab::Pool<NonCopyableNonMoveable> pool;
        WHEN("I take an instance from the pool") {

            auto instance = pool.takeInstance();            
            THEN("the instance can be modified") {
                instance->m_data = 180;
                REQUIRE(instance->m_data == 180);
                instance->m_data = -42;
                REQUIRE(instance->m_data == -42);
            }
        }
    }

    GIVEN("a pool of a non-default constructible type with no initial size") {
        struct NonDefaultConstructible  {
            // I know it's not required, but it does make what I'm doing more explicit
            NonDefaultConstructible() = delete;
            NonDefaultConstructible(float data) : m_data(data) { }

            float m_data;
        };

        struct InstanceConstructor { NonDefaultConstructible* operator()() const { return new NonDefaultConstructible(2.0f); } };
        lab::Pool<NonDefaultConstructible, InstanceConstructor> pool;
        WHEN("I take an instance from it") {

            auto instance = pool.takeInstance();
            THEN("the instance can be modified") {
                REQUIRE(instance->m_data == 2.0f);
                instance->m_data = 123.0f;
                REQUIRE(instance->m_data == 123.0f);
                instance->m_data = -456.5f;
                REQUIRE(instance->m_data == -456.5f);
            }
        }
    }

    GIVEN("a pool of large objects with no initial size") {
        // a large type is used here because this is exactly the kind of object you'd want to pool,
        // and if it wasn't pooled, your RAM would cry. A lot.
        struct LargeMcLargeFace {
            size_t m_data[10'000'000] = {};
        };

        lab::Pool<LargeMcLargeFace> pool;
        WHEN("I take repeatedly take instances and let them go out of scope") {
            for (size_t i = 0; i < 10; ++i) {

                for(size_t numberToTakeAtATime = 1; numberToTakeAtATime < 50; ++numberToTakeAtATime) {
                    std::vector<lab::Pool<LargeMcLargeFace>::Pooled> instances;

                    for(size_t instancesTaken = 0; instancesTaken < numberToTakeAtATime; ++instancesTaken) {
                        THEN("the correct number of instances are marked as taken") {
                            REQUIRE(instancesTaken == pool.getTaken());
                            instances.push_back(pool.takeInstance());
                            REQUIRE(instancesTaken + 1 == pool.getTaken());
                        }
                    }
                }

                THEN("when all have gone out of scope, the number taken is 0") {
                    REQUIRE(pool.getTaken() == 0);
                }
            }
        }
    }
}