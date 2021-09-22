/*
 * Copyright (c) 2013, 2014, 2017 Nicira, Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at:
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#ifndef ATHR_OVS_ATOMIC_H
#define ATHR_OVS_ATOMIC_H

#if _MSC_VER
#include "athr/ovs_atomic_msvc.h"
#else
#include <stdatomic.h>
#include <stdbool.h>
static inline bool atomic_load_bool(atomic_bool *x) { return atomic_load(x); }
static inline unsigned long atomic_load_ul(atomic_ulong *x)
{
    return atomic_load(x);
}

static inline unsigned long atomic_fetch_add_ul(atomic_ulong *x,
                                                unsigned long arg)
{
    return atomic_fetch_add(x, arg);
}
#endif

#endif
