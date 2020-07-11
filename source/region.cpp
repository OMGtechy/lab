#include "lab/region.h"

bool lab::Region::add(const id_t objectID) {
    return m_contents.insert(objectID).second;
}

bool lab::Region::remove(const id_t objectID) {
    return m_contents.erase(objectID) > 0;
}

bool lab::Region::contains(const id_t objectID) const {
    return m_contents.find(objectID) != m_contents.cend();
}

size_t lab::Region::size() const {
    return m_contents.size();
}
