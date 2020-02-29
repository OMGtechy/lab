#pragma once

#include <unordered_map>
#include <memory>
#include <queue>
#include <functional>

namespace lab {
    template <typename T>
    struct DefaultInstanceConstructor {
        T* operator()() const {
            return new T;
        }
    };

    template <typename T, typename InstanceConstructor = DefaultInstanceConstructor<T>>
    class Pool final {
    public:
        // constructors (non-copy and non-move) and destructor
        Pool();
        ~Pool() = default;

        // non-copyable
        Pool(const Pool& other) = delete;
        Pool& operator=(const Pool& other) = delete;

        // moveable
        Pool(Pool&& other) = default;
        Pool& operator=(Pool&& other) = default;

        // pool operations

        // the internal pool needs to be kept alive until all
        // instances have been returned, otherwise they'll be writing
        // into potentially freed memory and undefined behaviour ensues!
        class SharedPool final {
        public:
            T* takeInstance();
            void returnInstance(T* const);

            size_t getSize() const noexcept;
            size_t getTaken() const noexcept;
            size_t getAvailable() const noexcept;

        private:
            InstanceConstructor m_instanceConstructor;
            std::queue<std::unique_ptr<T>> m_available;
            std::unordered_map<T*, std::unique_ptr<T>> m_taken;
        };

        class InstanceDeleter final {
        public:
            InstanceDeleter(std::shared_ptr<SharedPool> pool);
            void operator()(T* const instancePointer) const;

        private:
            std::shared_ptr<SharedPool> m_sharedPool;
        };

        using Pooled = std::unique_ptr<T, InstanceDeleter>;

        Pooled takeInstance();
        void returnInstance(std::unique_ptr<T> instance);

        size_t getSize() const noexcept;
        size_t getTaken() const noexcept;
        size_t getAvailable() const noexcept;

    private:
        std::shared_ptr<SharedPool> m_sharedPool;
        void returnInstance(T* const);
    };

    // constructors (non-copy and non-move) and destructor

    template <typename T, typename InstanceConstructor>
    Pool<T, InstanceConstructor>::Pool() : m_sharedPool(std::make_shared<SharedPool>()) { }

    // pool operations

    template <typename T, typename InstanceConstructor>
    T* Pool<T, InstanceConstructor>::SharedPool::takeInstance() {
        if (m_available.size() < 1) {
            m_available.push(std::unique_ptr<T>(m_instanceConstructor()));
        }

        auto instance = std::move(m_available.back());
        const auto address = instance.get();
        m_taken[address] = std::move(instance);
        m_available.pop();
        return address;
    }

    template <typename T, typename InstanceConstructor>
    void Pool<T, InstanceConstructor>::SharedPool::returnInstance(T* const instance) {
        auto node = m_taken.extract(instance);
        m_available.push(std::move(node.mapped()));
    }

    template <typename T, typename InstanceConstructor>
    size_t Pool<T, InstanceConstructor>::SharedPool::getSize() const noexcept {
        return getAvailable() + getTaken();
    }

    template <typename T, typename InstanceConstructor>
    size_t Pool<T, InstanceConstructor>::SharedPool::getAvailable() const noexcept {
        return m_available.size();
    }

    template <typename T, typename InstanceConstructor>
    size_t Pool<T, InstanceConstructor>::SharedPool::getTaken() const noexcept {
        return m_taken.size();
    }



    template <typename T, typename InstanceConstructor>
    Pool<T, InstanceConstructor>::InstanceDeleter::InstanceDeleter(std::shared_ptr<SharedPool> sharedPool) : m_sharedPool(sharedPool) { }

    template <typename T, typename InstanceConstructor>
    void Pool<T, InstanceConstructor>::InstanceDeleter::operator()(T* const instancePointer) const {
        m_sharedPool->returnInstance(instancePointer);
    }

    template <typename T, typename InstanceConstructor>
    typename Pool<T, InstanceConstructor>::Pooled Pool<T, InstanceConstructor>::takeInstance() { return { m_sharedPool->takeInstance(), {m_sharedPool} }; }

    template <typename T, typename InstanceConstructor>
    void Pool<T, InstanceConstructor>::returnInstance(std::unique_ptr<T> instance) { returnInstance(instance.release()); }

    template <typename T, typename InstanceConstructor>
    void Pool<T, InstanceConstructor>::returnInstance(T* const instance) { }

    template <typename T, typename InstanceConstructor>
    size_t Pool<T, InstanceConstructor>::getSize() const noexcept { return m_sharedPool->getSize(); }

    template <typename T, typename InstanceConstructor>
    size_t Pool<T, InstanceConstructor>::getAvailable() const noexcept { return m_sharedPool->getAvailable(); }

    template <typename T, typename InstanceConstructor>
    size_t Pool<T, InstanceConstructor>::getTaken() const noexcept { return m_sharedPool->getTaken(); }
}