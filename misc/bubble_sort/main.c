#include <stddef.h>

bool sorted(const unsigned int* array, const size_t size) {
    for(size_t i = 0; i < size - 1; i++) {
        if(array[i] > array[i+1]) {
            return false;
        }
    }
    return true;
}

void bubble_sort(unsigned int* array, const size_t size) {
    while(sorted(array, size) == false) {
        for(size_t i = 0; i < size - 1; i++) {
            if(array[i] > array[i+1]) {
                const unsigned int temp = array[i];
                array[i] = array[i+1];
                array[i+1] = temp;
            }
        }
    }
}

int main(void) {
    unsigned int array[8] = { 0x2, 0x5, 0x1, 0xF, 0x7, 0x3, 0xA, 0x0 };
    bubble_sort(array, sizeof(array) / sizeof(*array));
    return 0;
}