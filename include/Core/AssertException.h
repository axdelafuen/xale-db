#ifndef CORE_ASSERT_EXCEPTION_H
#define CORE_ASSERT_EXCEPTION_H

#include <iostream>

#include "Core/ExceptionHandler.h"

#define DE_ASSERT(x, msg) \
        { \
            if (!(x)) { \
                std::cerr << "ASSERTION FAILED: " << msg << "\n" \
                            << "File: " << __FILE__ << "\n" \
                            << "Line: " << __LINE__ << "\n"; \
            } \
        }

#define DE_THROW(code, msg) throw Xale::Core::DbException(code, msg, __FILE__, __LINE__)

#endif // CORE_ASSERT_EXCEPTION_H
