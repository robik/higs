//
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree.
//

#pragma once

#include <atomic>
#include <limits>
#include <memory>

#include <boost/smart_ptr/intrusive_ref_counter.hpp>
#include <higs/utility/NonCopyable.hpp>
#include <higs/utility/NonMoveable.hpp>
#include <jsrt/jsrt.hpp>

#include "higs/concepts.hpp"

namespace boost {
template <typename X>
class intrusive_ptr;
}

namespace higs {

template<typename Derived>
class RefCounted : public jsrt::RefCounted {
public:
    using Ptr = boost::intrusive_ptr<Derived>;
    HIGS_MAKE_NON_COPYABLE(RefCounted);
    HIGS_MAKE_NON_MOVEABLE(RefCounted);

protected:
    RefCounted() = default;
    ~RefCounted() = default;

public:
    /**
     * Atomically increases number of references of this object.
     */
    void retain() const noexcept
    {
        [[maybe_unused]] auto oldCount = _refCount.fetch_add(1, std::memory_order_relaxed);
        assert(oldCount > 0);
        assert(oldCount < std::numeric_limits<decltype(oldCount)>::max());
    }

    /**
     * Atomically decreases number of references of this object.
     */
    void release() const noexcept
    {
        auto oldCount = _refCount.fetch_sub(1, std::memory_order_release);
        assert(oldCount > 0);
        if (oldCount == 1) {
            std::atomic_thread_fence(std::memory_order_acquire);
            delete static_cast<const Derived*>(this);
        }
    }

    /**
     * Number of currently active references to this object.
     *
     * @return
     */
    auto refCount() const noexcept -> std::size_t
    {
        return this->_refCount.load(std::memory_order_relaxed);
    }

    /**
     * Helper method that creates a manager ptr from this.
     *
     * @return
     */
    auto asRef(bool increaseRef = true) noexcept -> Ptr
    {
        return boost::intrusive_ptr(static_cast<Derived*>(this), increaseRef);
    }

    template <typename... Args>
    static auto create(Args&&... args) -> Ptr
    {
        return Ptr(new Derived(std::forward<Args>(args)...), false);
    }

private:
    mutable std::atomic<std::size_t> _refCount = 1;
    template <typename X>
    friend class boost::intrusive_ptr;
    friend void intrusive_ptr_add_ref(const RefCounted* p) BOOST_SP_NOEXCEPT;
    friend void intrusive_ptr_release(const RefCounted* p) BOOST_SP_NOEXCEPT;
    template <typename T, typename... Args>
    std::shared_ptr<T> make_shared_from_refcounted(Args&&... args) noexcept;
};

}

template <typename DerivedT>
inline void intrusive_ptr_add_ref(higs::RefCounted<DerivedT>* p) BOOST_SP_NOEXCEPT
{
    p->retain();
}

template <typename DerivedT>
inline void intrusive_ptr_release(higs::RefCounted<DerivedT>* p) BOOST_SP_NOEXCEPT
{
    p->release();
}


namespace std {

template <typename T, typename... Args>
std::shared_ptr<T> make_shared_from_refcounted(Args&&... args)
{
    T* instance = new T(std::forward<Args>(args)...);
    return {instance, intrusive_ptr_release};
}

template <typename DerivedT>
std::shared_ptr<DerivedT> make_shared(higs::RefCounted<DerivedT>* ptr) noexcept
{
    return {ptr, intrusive_ptr_release};
}

}


#include <boost/smart_ptr/intrusive_ptr.hpp>