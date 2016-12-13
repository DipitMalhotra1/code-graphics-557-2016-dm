#include <fstream>
#include <assert.h>
#include "image.h"

Image::Image(char* ps, int w, int h) : pixels(ps), width(w), height(h) {}

Image::~Image() {
    delete[] pixels;
}

namespace {
    int convert_to_integer(const char* bytes) {
        return (int)(((unsigned char)bytes[3] << 24) |
                     ((unsigned char)bytes[2] << 16) |
                     ((unsigned char)bytes[1] << 8) |
                     (unsigned char)bytes[0]);
    }
    
    short convert_to_short(const char* bytes) {
        return (short)(((unsigned char)bytes[1] << 8) |
                       (unsigned char)bytes[0]);
    }
    
    int read_integer(std::ifstream &input) {
        char buffer[4];
        input.read(buffer, 4);
        return convert_to_integer(buffer);
    }
    
    short read_short(std::ifstream &input) {
        char buffer[2];
        input.read(buffer, 2);
        return convert_to_short(buffer);
    }
    
    template<class T>
    class auto_array
    {
    private:
        T* array;
        mutable bool is_rel;
    public:
        explicit auto_array(T* array_ = NULL) :
        array(array_), is_rel(false) {}
        
        auto_array(const auto_array<T> &aarray) {
            array = aarray.array;
            is_rel = aarray.is_rel;
            aarray.is_rel = true;
        }
        
        ~auto_array() {
            if (!is_rel && array != NULL) {
                delete[] array;
            }
        }
        
        T* get() const {
            return array;
        }
        
        T &operator*() const {
            return *array;
        }
        
        void operator=(const auto_array<T> &aarray) {
            if (!is_rel && array != NULL) {
                delete[] array;
            }
            array = aarray.array;
            is_rel = aarray.is_rel;
            aarray.is_rel = true;
        }
        
        T* operator->() const
        {
            return array;
        }
        
        T* release() {
            is_rel = true;
            return array;
        }
        
        void reset(T* array_ = NULL) {
            if (!is_rel && array != NULL) {
                delete[] array;
            }
            array = array_;
        }
        
        T* operator+(int i) {
            return array + i;
        }
        
        T &operator[](int i) {
            return array[i];
        }
    };
}

Image* loadBMP(const char* filename) {
    std::ifstream input;
    input.open(filename, std::ifstream::binary);
    assert(!input.fail() || !"Could not find file");
    char buffer[2];
    input.read(buffer, 2);
    assert(buffer[0] == 'B' && buffer[1] == 'M' || !"Not a bitmap file");
    input.ignore(8);
    int dataOffset = read_integer(input);
    
    
    int headerSize = read_integer(input);
    int width;
    int height;
    switch (headerSize) {
        case 40:
            width = read_integer(input);
            height = read_integer(input);
            input.ignore(2);
            assert(read_short(input) == 24 || !"Image not in right format");
            assert(read_short(input) == 0 || !"Image size compressed");
            break;
        case 12:
            width = read_short(input);
            height = read_short(input);
            input.ignore(2);
            assert(read_short(input) == 24 || !"Image not in right format");
            break;
        case 64:
            assert(!"Could not load image");
            break;
        case 108:
            assert(!"Could not load image");
            break;
        case 124:
            assert(!"Could not load image");
            break;
        default:
            assert(!"Unknown bitmap format");
    }
    
    int bytesPerRow = ((width * 3 + 3) / 4) * 4 - (width * 3 % 4);
    int size = bytesPerRow * height;
    auto_array<char> pixels(new char[size]);
    input.seekg(dataOffset, std::ios_base::beg);
    input.read(pixels.get(), size);
    
    auto_array<char> pixels2(new char[width * height * 3]);
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            for (int c = 0; c < 3; c++) {
                pixels2[3 * (width * y + x) + c] =
                pixels[bytesPerRow * y + 3 * x + (2 - c)];
            }
        }
    }
    
    input.close();
    return new Image(pixels2.release(), width, height);
}
