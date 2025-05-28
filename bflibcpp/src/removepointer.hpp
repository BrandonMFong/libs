/**
 * author: brando
 * date: 5/27/25
 */

#ifndef BF_REMOVE_POINTER_HPP
#define BF_REMOVE_POINTER_HPP

template <typename T>
struct RemovePointer {
    using type = T;
};

template <typename T>
struct RemovePointer<T*> {
    using type = T;
};

#endif // BF_REMOVE_POINTER_HPP

