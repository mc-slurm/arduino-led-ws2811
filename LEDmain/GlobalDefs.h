#ifndef GLOBAL_DEFS_H
#define GLOBAL_DEFS_H

#pragma once

// make_unique
template<typename T, typename... Args>
std::unique_ptr<T> make_unique(Args&&... args) {
    return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
}

#endif