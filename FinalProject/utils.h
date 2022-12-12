#pragma once

#define offsetof(s,m) (size_t)&reinterpret_cast<const volatile char&>((((s *)0)->m))
