#pragma once

#include <set>

namespace lab {
    class Region final {
    public:
        // constructors (non-copy and non-move) and destructor
        Region() = default;
        ~Region() = default;

        // non-copyable
        Region(const Region& other) = delete;
        Region& operator=(const Region& other) = delete;

        // moveable
        Region(Region&& other) = default;
        Region& operator=(Region&& other) = default;

        // region operations
        using id_t = size_t;
        bool add(const id_t objectID);
        bool remove(const id_t objectID);
        bool contains(const id_t objectID) const;
        size_t size() const;

    private:
        std::set<id_t> m_contents;
    };
}
