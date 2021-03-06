// Copyright (c) 2019-2020 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_MAP
#define DATA_MAP

#include "types.hpp"
#include "set.hpp"

namespace data {

    template <typename K, typename V>
    struct entry {
        const K Key;
        V Value;
        
        entry(const K k, V v) : Key(k), Value(v) {}
        
        // an invalid instance must exist for type value.
        entry(const K k) : Key(k), Value{} {}
        
        bool valid() const {
            return data::valid(Key) && data::valid(Value);
        }
        
        const K key() const {
            return Key;
        }
        
        V value() const {
            return Value;
        }
        
        V operator[](const K k) {
            if (Key == k) return Value;
            return V{};
        }
        
        bool operator==(const entry e) const {
            return Key == e.Key && Value == e.Value;
        }
        
        bool operator!=(const entry e) const {
            return !(*this == e);
        }
        
        bool operator<(const entry e) const {
            return Key < e.Key;
        }
        
        bool operator>(const entry e) const {
            return Key > e.Key;
        }
        
        bool operator<=(const entry e) const {
            return Key < e.Key;
        }
        
        bool operator>=(const entry e) const {
            return Key > e.Key;
        }
    };
    
    template <typename K, typename V>
    inline std::ostream& operator<<(std::ostream& o, const entry<K, V>& e) {
        return o << e.Key << " => " << e.Value;
    }
    
    namespace meta {
        
        template <typename map, typename seq>
        class has_keys_method {
            template <typename X> static auto test(int) -> typename 
                std::enable_if<std::is_same<decltype(std::declval<X>().keys()), seq>::value, yes>::type;
            template <typename> static no test(...);
        public:
            static constexpr bool value = std::is_same<decltype(test<map>(0)), yes>::value;
        };
        
    }
    
    namespace interface {
        
        template <typename M>
        struct map {
            using values = typename container<M>::values;
            using keys = decltype(std::declval<M>().keys());
            using key = typename std::remove_const<typename std::remove_reference<decltype(std::declval<keys>().first())>::type>::type;
            using value = typename indexed<M, key>::value;
            using element = typename sequence<values>::element;
            // TODO ensure element is entry<key, value>
        private:
            using require_default_constructable = typename std::enable_if<std::is_default_constructible<M>::value, void>::type;
            using require_insert_method = typename std::enable_if<
                meta::has_insert_method<M, key, value>::value, void>::type;
            using require_remove_method = typename std::enable_if<
                meta::has_remove_method<M, key>::value, void>::type;
        }; 
    }
    
}

#endif
