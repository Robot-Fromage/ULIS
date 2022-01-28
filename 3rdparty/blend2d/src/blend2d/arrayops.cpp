// Blend2D - 2D Vector Graphics Powered by a JIT Compiler
//
//  * Official Blend2D Home Page: https://blend2d.com
//  * Official Github Repository: https://github.com/blend2d/blend2d
//
// Copyright (c) 2017-2020 The Blend2D Authors
//
// This software is provided 'as-is', without any express or implied
// warranty. In no event will the authors be held liable for any damages
// arising from the use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it
// freely, subject to the following restrictions:
//
// 1. The origin of this software must not be misrepresented; you must not
//    claim that you wrote the original software. If you use this software
//    in a product, an acknowledgment in the product documentation would be
//    appreciated but is not required.
// 2. Altered source versions must be plainly marked as such, and must not be
//    misrepresented as being the original software.
// 3. This notice may not be removed or altered from any source distribution.

#include "./api-build_p.h"
#include "./arrayops_p.h"
#include "./math_p.h"

// ============================================================================
// [BLArrayOps - Unit Tests]
// ============================================================================

#ifdef BL_TEST
template<typename T>
static void expectSorted(const T* a, const T* b, size_t size) noexcept {
  for (size_t i = 0; i < size; i++)
    EXPECT(a[i] == b[i], "Mismatch at %u", unsigned(i));
}

UNIT(arrayops, -9) {
  INFO("blQuickSort() - Testing qsort and isort of predefined arrays");
  {
    constexpr size_t kArraySize = 11;

    int ref_[kArraySize] = { -4, -2, -1, 0, 1, 9, 12, 13, 14, 19, 22 };
    int arr1[kArraySize] = { 0, 1, -1, 19, 22, 14, -4, 9, 12, 13, -2 };
    int arr2[kArraySize];

    memcpy(arr2, arr1, kArraySize * sizeof(int));

    blInsertionSort(arr1, kArraySize);
    blQuickSort(arr2, kArraySize);
    expectSorted(arr1, ref_, kArraySize);
    expectSorted(arr2, ref_, kArraySize);
  }

  INFO("blQuickSort() - Testing qsort and isort of artificial arrays");
  {
    constexpr size_t kArraySize = 200;

    int arr1[kArraySize];
    int arr2[kArraySize];
    int ref_[kArraySize];

    for (size_t size = 2; size < kArraySize; size++) {
      for (size_t i = 0; i < size; i++) {
        arr1[i] = int(size - 1 - i);
        arr2[i] = int(size - 1 - i);
        ref_[i] = int(i);
      }

      blInsertionSort(arr1, size);
      blQuickSort(arr2, size);
      expectSorted(arr1, ref_, size);
      expectSorted(arr2, ref_, size);
    }
  }

  INFO("blQuickSort() - Testing qsort and isort having unstable compare function");
  {
    constexpr size_t kArraySize = 5;

    float arr1[kArraySize] = { 1.0f, 0.0f, 3.0f, -1.0f, blNaN<float>() };
    float arr2[kArraySize] = { };

    memcpy(arr2, arr1, kArraySize * sizeof(float));

    // We don't test as it's undefined where the NaN would be.
    blInsertionSort(arr1, kArraySize);
    blQuickSort(arr2, kArraySize);
  }

  INFO("blBinarySearch() - Testing binary search");
  {
    static const int arr[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };

    for (size_t size = BL_ARRAY_SIZE(arr); size > 0; size--) {
      for (size_t i = 0; i < size; i++) {
        int value = arr[i];
        EXPECT(blBinarySearch(arr, size, value) == i);
        EXPECT(blBinarySearchClosestFirst(arr, size, value) == i);
        EXPECT(blBinarySearchClosestLast(arr, size, value) == i);
      }
    }
  }
}
#endif
