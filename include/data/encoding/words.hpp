// Copyright (c) 2019 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_ENCODING_WORDS
#define DATA_ENCODING_WORDS

#include <type_traits>
#include <data/types.hpp>
#include <data/encoding/halves.hpp>
#include <data/tools/index_iterator.hpp>
#include <data/math/sign.hpp>
#include <data/meta/equal.hpp>
#include <data/slice.hpp>
#include <data/io/unimplemented.hpp>

namespace data::arithmetic {
    
    template <typename bit32, endian::order o> struct words;
    
    template <typename bit32> struct words<bit32, endian::big> {
        slice<byte> Data;
        size_t Bytes;
        size_t Size;
        
        words(slice<byte> d) : Data{d}, Bytes{d.size()}, Size{Bytes / 4} {}
        
        static const words make(const slice<byte> d) {
            return words(d);
        }
        
        using index = uint32;
        
        index last() const {
            return Size - 1;
        }
        
        using element = boost::endian::endian_arithmetic<endian::big, bit32, 32>;
        
        const element default_value() const {
            return std::is_signed<bit32>::value && Data[last()] < 0 ? -1 : 0;
        }
        
        const element operator[](index i) const {
            if (i > last()) return default_value();
            element x{0};
            if (i <= last()) std::copy_n(&Data[4 * (last() - i)], 4, x.data());
        }
        
        const element set(index i, element x) {
            if (i > last()) return default_value();
            std::copy_n(x.data(), 4, &Data[4 * (last() - i)]);
        }
    };
    
    template <typename bit32> struct words<bit32, endian::little> {
        slice<byte> Data;
        size_t Bytes;
        size_t Size;
        
        words(slice<byte> d) : Data{d}, Bytes{d.size()}, Size{Bytes / 4} {}
        
        static const words make(const slice<byte> d) {
            return words(d);
        }
        
        using index = uint32;
        
        index last() const {
            return Size - 1;
        }
        
        using element = boost::endian::endian_arithmetic<endian::little, bit32, 32>;
        
        const element default_value() const {
            return std::is_signed<bit32>::value && Data[last()] < 0 ? -1 : 0;
        }
        
        const element operator[](index i) const {
            if (i > last()) return default_value();
            element x{0};
            std::copy_n(&Data[4 * i], 4, x.data());
        }
        
        const element set(index i, element x) {
            if (i > last()) return default_value();
            std::copy_n(x.data(), 4, &Data[4 * i]);
        }
    };
    
    template <typename words, typename word>
    struct unoriented {
        using twice = typename data::twice<word>::type;
        
        static twice extend(word w) {
            return data::twice<word>::extend(w);
        }
        
        static bool less(uint32 last, const words, const words);
        
        static bool less_equal(uint32 last, const words, const words);
        
        static void bit_not(uint32 last, const words, words);
        
        static void bit_and(uint32 last, const words, const words, words);
        
        static void bit_or(uint32 last, const words, const words, words);
        
        static void plus(uint32 last, const words, const words, words);
        
        static void plus(uint32 last, const words, const word, words);
        
        static void plus(uint32 last, const words, const uint64, words) {
            throw method::unimplemented{"unoriented::plus"};
        }
        
        static void minus(uint32 last, const words, const words, words);
        
        static void minus(uint32 last, const words, const word, words);
        
        static void times(uint32 last, const words, const words, words);
        
        static void times(uint32 last, const words, const word, words);
        
        using index = uint32;
        
        static void down(uint32 last, const words a, const uint32 i, words out) {
            throw method::unimplemented{"methods::down"};
        }
        
        static void up(uint32 last, const words a, const uint32 i, words out) {
            throw method::unimplemented{"methods::up"};
        }
    };
    
    template <typename words, typename word>
    bool unoriented<words, word>::less(uint32 last, const words a, const words b) {
        if (a[last] < b[last]) return true;
        if (a[last] > b[last]) return false;
        
        for (int i = last - 1; i >= 0; i--) {
            if (uint32(a[i]) < uint32(b[i])) return true;
            if (uint32(a[i]) > uint32(b[i])) return false;
        }
        
        return false;
    }
    
    template <typename words, typename word>
    bool unoriented<words, word>::less_equal(uint32 last, const words a, const words b) {
        if (a[last] < b[last]) return true;
        if (a[last] > b[last]) return false;
        
        for (int i = last - 1; i >= 0; i--) {
            if (uint32(a[i]) < uint32(b[i])) return true;
            if (uint32(a[i]) > uint32(b[i])) return false;
        }
        
        return true;
    }
    
    template <typename words, typename word>
    void unoriented<words, word>::bit_not(uint32 last, const words xx, words out) {
        for (uint32 i = 0; i <= last; i++) out.set(i, ~xx[i]);
    }
    
    template <typename words, typename word>
    void unoriented<words, word>::bit_and(uint32 last, const words x, const words y, words out) {
        for (uint32 i = 0; i <= last; i++) out.set(i, x[i]^y[i]);
    }
    
    template <typename words, typename word>
    void unoriented<words, word>::bit_or(uint32 last, const words x, const words y, words out) {
        for (uint32 i = 0; i <= last; i++) out.set(i, x[i]|y[i]);
    }
    
    template <typename words, typename word>
    void unoriented<words, word>::plus(uint32 last, const words a, const words b, words result) {
        word remainder{0};
        for (int32 i = 0; i <= last; i++) {
            twice w = extend(a[i]) + extend(b[i]);
            result.set(i, remainder + lesser_half(w));
            remainder = greater_half(w);
        };
    }
    
    template <typename words, typename word>
    void unoriented<words, word>::plus(uint32 last, const words a, const word b, words result) {
        word remainder{0};
        
        twice w = extend(a[0]) + extend(b);
        result.set(0, remainder + lesser_half(w));
        remainder = greater_half(w);
        
        for (int32 i = 1; i <= last; i++) {
            w = extend(a[i]);
            result.set(i, remainder + lesser_half(w));
            remainder = greater_half(w);
        };
    }
    
    template <typename words, typename word>
    void unoriented<words, word>::minus(uint32 last, const words a, const words b, words result) {
        word remainder{0};
        for (int32 i = last; i >= 0; i--) {
            twice w = extend(a[i]) - extend(b[i]);
            result.set(i, remainder + lesser_half(w));
            remainder = greater_half(w);
        };
    }
    
    template <typename words, typename word>
    void unoriented<words, word>::minus(uint32 last, const words a, const word b, words result) {
        int i = 0;
        word remainder = b;
        
        while (i <= last && remainder != word{0}) {
            word wo = a[i];
            twice w = extend(wo) - extend(remainder);
            result.set(i, lesser_half(w));
            remainder = -greater_half(w);
            i++;
        }
    }
    
    template <typename words, typename word>
    void unoriented<words, word>::times(uint32 last, const words a, const words b, words result) {
        throw method::unimplemented{"unoriented::times"};
        /*auto from_end = [last](uint32 i)->uint32{return last - i;};
        word remainder{0};
        for (int i = 0; i <= last; i ++) {
            twice sum = extend(remainder);
            for (int j = 0; j <= i; j++) sum += a[from_end(j)]*b[from_end(i - j)];
            remainder = greater_half(sum);
            result[from_end(i)] = lesser_half(sum);
        }*/
    }
}

#endif
