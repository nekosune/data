// Copyright (c) 2019-2020 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_INTERFACE
#define DATA_INTERFACE

#include <data/types.hpp>
#include <type_traits>

namespace data {
    
    namespace meta {
        
        template <typename X>
        class has_empty_method {
            template <typename U> static auto test(int) -> typename 
                std::enable_if<std::is_same<decltype(std::declval<const U>().empty()), bool>::value, yes>::type;
            template <typename> static no test(...);
        public:
            static constexpr bool value = std::is_same<decltype(test<X>(0)), yes>::value;
        };
        
        template <typename X>
        class has_size_method {
            template <typename U> static auto test(int) -> typename 
                std::enable_if<std::is_same<decltype(std::declval<const U>().size()), size_t>::value, yes>::type;
            template <typename> static no test(...);
        public:
            static constexpr bool value = std::is_same<decltype(test<X>(0)), yes>::value;
        };
        
        template <typename X, typename it>
        class is_iterable {
            template <typename T, typename i> static auto test(int) -> typename 
                std::enable_if<std::is_same<decltype(std::declval<T>().begin()), i>::value && 
                    std::is_same<decltype(std::declval<T>().end()), i>::value, yes>::type;
            template <typename, typename> static no test(...);
        public:
            static constexpr bool value = std::is_same<decltype(test<X, it>(0)), yes>::value;
        };
        
        template <typename X, typename it>
        class is_const_iterable {
            template <typename T, typename i> static auto test(int) -> typename 
                std::enable_if<std::is_same<decltype(std::declval<const T>().begin()), i>::value && 
                    std::is_same<decltype(std::declval<const T>().end()), i>::value, yes>::type;
            template <typename, typename> static no test(...);
        public:
            static constexpr bool value = std::is_same<decltype(test<X, it>(0)), yes>::value;
        };
        
        template <typename list, typename element>
        class has_first_method {
            template <typename X> static auto test(int) -> typename 
                std::enable_if<std::is_same<decltype(std::declval<const X>().first()), const element>::value, yes>::type;
            template <typename> static no test(...);
        public:
            static constexpr bool value = std::is_same<decltype(test<list>(0)), yes>::value;
        };
        
        template <typename list>
        class has_rest_method {
            template <typename X> static auto test(int) -> typename 
                std::enable_if<std::is_same<decltype(std::declval<const X>().rest()), list>::value, yes>::type;
            template <typename> static no test(...);
        public:
            static constexpr bool value = std::is_same<decltype(test<list>(0)), yes>::value;
        };
        
        template <typename...> class has_insert_method;
        
        template <typename list, typename element>
        class has_insert_method<list, element> {
            template <typename X> static auto test(int) -> typename 
                std::enable_if<std::is_same<decltype(std::declval<const X>().insert(std::declval<element>())), list>::value, yes>::type;
            template <typename> static no test(...);
        public:
            static constexpr bool value = std::is_same<decltype(test<list>(0)), yes>::value;
        };
        
        template <typename map, typename key, typename val>
        class has_insert_method<map, key, val> {
            template <typename X> static auto test(int) -> typename 
                std::enable_if<std::is_same<decltype(std::declval<const X>().insert(std::declval<key>(), std::declval<val>())), map>::value, yes>::type;
            template <typename> static no test(...);
        public:
            static constexpr bool value = std::is_same<decltype(test<map>(0)), yes>::value;
        };
        
        template <typename set, typename element>
        class has_contains_method {
            template <typename X> static auto test(int) -> decltype((void)(std::declval<const X>().contains(std::declval<const element>()) == true), yes());
            template <typename> static no test(...);
        public:
            static constexpr bool value = std::is_same<decltype(test<set>(0)), yes>::value;
        };
        
        template <typename set, typename key>
        class has_remove_method {
            template <typename X> static auto test(int) -> typename std::enable_if<
                std::is_same<decltype(std::declval<const X>().remove(std::declval<const key>())), set>::value, yes>::type;
            template <typename> static no test(...);
        public:
            static constexpr bool value = std::is_same<decltype(test<set>(0)), yes>::value;
        };
        
        template <typename list, typename element>
        class has_prepend_method {
            template <typename X> static auto test(int) -> decltype((void)(std::declval<const X>().prepend(std::declval<const element>()) == std::declval<list>()), yes());
            template <typename> static no test(...);
        public:
            static constexpr bool value = std::is_same<decltype(test<list>(0)), yes>::value;
        };
        
        template <typename list, typename element>
        class has_append_method {
            template <typename X> static auto test(int) -> decltype((void)(std::declval<const X>().append(std::declval<const element>()) == std::declval<list>()), yes());
            template <typename> static no test(...);
        public:
            static constexpr bool value = std::is_same<decltype(test<list>(0)), yes>::value;
        };
        
        template <typename tree>
        class has_left_method {
            template <typename X> static auto test(int) -> decltype((void)(std::declval<const X>().left() == std::declval<tree>()), yes());
            template <typename> static no test(...);
        public:
            static constexpr bool value = std::is_same<decltype(test<tree>(0)), yes>::value;
        };
        
        template <typename tree>
        class has_right_method {
            template <typename X> static auto test(int) -> decltype((void)(std::declval<const X>().right() == std::declval<tree>()), yes());
            template <typename> static no test(...);
        public:
            static constexpr bool value = std::is_same<decltype(test<tree>(0)), yes>::value;
        };
        
        template <typename con, typename seq>
        class has_values_method {
            template <typename X> static auto test(int) -> decltype((void)(std::declval<const X>().values() == std::declval<seq>()), yes());
            template <typename> static no test(...);
        public:
            static constexpr bool value = std::is_same<decltype(test<con>(0)), yes>::value;
        };
        
        template <typename X, bool has_empty_method> struct is_empty {
            bool operator()(const X&) {
                return true;
            }
            
            bool operator()(const X*) {
                return true;
            }
        };
        
        template <typename X> struct is_empty<X, true> {
            bool operator()(const X& x) {
                return x.empty();
            }
            
            bool operator()(const X* x) {
                return x == nullptr ? true : x->empty();
            }
        };
        
        template <typename X, bool has_size_method> struct size {
            size_t operator()(const X&) {
                return 0;
            }
            
            size_t operator()(const X*) {
                return 0;
            }
        };
        
        template <typename X> struct size<X, true> {
            size_t operator()(const X& x) {
                return x.size();
            }
            
            size_t operator()(const X* x){
                return x == nullptr ? 0 : x->size();
            }
        };
        
        template <typename X, bool has_rest_method> struct rest {
            X operator()(const X& x) {
                return x;
            }
            
            const X* operator()(const X* x) {
                return x;
            }
        };
        
        template <typename X> struct rest<X, true> {
            X operator()(const X& x) {
                return x.rest();
            }
            
            const X* operator()(const X* x) {
                return x == nullptr ? nullptr : x->rest();
            }
        };
        
        template <typename X, bool has_right_method> struct right {
            X operator()(const X& x) {
                return x;
            }
            
            const X* const operator()(const X* x){
                return x;
            }
        };
        
        template <typename X> struct right<X, true> {
            X operator()(const X& x) {
                return x.right();
            }
        };
        
        template <typename X, bool has_left_method> struct left {
            X operator()(const X& x) {
                return x;
            }
            
            const X* const operator()(const X* x){
                return x;
            }
        };
        
        template <typename X> struct left<X, true> {
            X operator()(const X& x) {
                return x.left();
            }
            
            const X* operator()(const X* x){
                return x == nullptr ? nullptr : x->rest();
            }
        };
        
        template <typename X, typename E, bool has_contains_method> struct contains {
            bool operator()(const X& x, const E& e) {
                return false;
            }
        };
        
        template <typename X, typename E> struct contains<X, E, true> {
            X operator()(const X& x, const E& e) {
                return x.contains(e);
            }
        };
        
        template <typename X, typename E, bool has_insert_method> struct insert;
        
        template <typename X, typename E> struct insert<X, E, true> {
            X operator()(const X& x, const E& e) {
                return x.insert(e);
            }
        };
        
    }
    
    namespace interface {
        
        template <typename L, 
            typename elem = decltype(std::declval<const L>().first()), 
            typename require_empty_method = typename std::enable_if<meta::has_empty_method<L>::value, void>::type,
            typename require_size_method = typename std::enable_if<meta::has_size_method<L>::value, void>::type,
            typename require_rest_method = typename std::enable_if<meta::has_rest_method<L>::value, void>::type>
        struct sequence {
            using element = elem;
        }; 
        
        template <typename L, 
            typename vals = decltype(std::declval<const L>().values()), 
            typename elem = typename sequence<vals>::element, 
            typename require_empty_method = typename std::enable_if<meta::has_empty_method<L>::value, void>::type,  
            typename require_size_method = typename std::enable_if<meta::has_size_method<L>::value, void>::type, 
            typename require_contains_method = typename std::enable_if<meta::has_contains_method<L, elem>::value, void>::type>
        struct container {
            using values = vals;
            using element = elem;
        }; 
        
        template <typename M, typename key, typename val = decltype(std::declval<const M>()[std::declval<const key>()])>
        struct indexed {
            using value = val;
        }; 
        
    }

    template <typename X>
    inline bool empty(const X& x) {
        return meta::is_empty<X, meta::has_empty_method<X>::value>{}(x);
    }

    template <typename X>
    inline size_t size(const X& x) {
        return meta::size<X, meta::has_size_method<X>::value>{}(x);
    }

    template <typename X>
    inline const decltype(std::declval<const X>().first()) first(const X& x) {
        return x.first();
    }

    template <typename L, typename X>
    inline L append(const L& l, const X& x) {
        return l.append(x);
    }

    template <typename X>
    inline X rest(const X& x) {
        return meta::rest<X, meta::has_rest_method<X>::value>{}(x);
    }

    template <typename X>
    inline X right(const X& x) {
        return meta::right<X, meta::has_right_method<X>::value>{}(x);
    }

    template <typename X>
    inline X left(const X& x) {
        return meta::left<X, meta::has_left_method<X>::value>{}(x);
    }

    template <typename X, typename E>
    inline bool contains(const X& x, const E& e) {
        return meta::contains<X, E, meta::has_contains_method<X, E>::value>{}(x, e);
    }

    template <typename X, typename E>
    inline X insert(const X& x, const E& e) {
        return meta::insert<X, E, meta::has_insert_method<X, E>::value>{}(x, e);
    }

}

#endif
