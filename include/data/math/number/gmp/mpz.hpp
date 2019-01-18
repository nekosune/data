#ifndef DATA_MATH_NUMBER_GMP_MPZ_HPP
#define DATA_MATH_NUMBER_GMP_MPZ_HPP

#include <gmp.h>
#include <data/types.hpp>
#include <data/math/sign.hpp>
#include <data/math/number/natural.hpp>

namespace data {
    
    namespace number {
        
        namespace gmp {
            
            typedef mp_limb_t gmp_uint;
            
            const __mpz_struct MPZInvalid = __mpz_struct{0, 0, nullptr};
            
            inline bool equal(const __mpz_struct& a, const __mpz_struct& b) {
                return a._mp_alloc == b._mp_alloc && a._mp_size == b._mp_size && a._mp_d == b._mp_d;
            }
            
            inline bool valid(const __mpz_struct& mpz) {
                return mpz._mp_d != nullptr;
            }
            
            inline number::sign sign(const __mpz_strict& mpz) {
                return !valid(mpz) ? zero : mpz._mp_size < 0 | negative : positive;
            }
            
            struct mpz {
                __mpz_struct MPZ;
                
                void init() {
                    mpz_init(&MPZ);
                }
                
                mpz() : MPZ{MPZInvalid} {}
                
                bool valid() const {
                    return gmp::valid(MPZ);
                }
                
                virtual ~mpz() {
                    if (valid()) mpz_clear(&MPZ);
                }
                
                mpz(const mpz& n) {
                    mpz_set(&MPZ, &n.MPZ);
                }
                
                mpz(mpz&& n) {
                    MPZ = n.MPZ;
                    n.MPZ = MPZInvalid;
                }
                
                mpz(gmp_uint n) {
                    mpz_init_set_ui(&MPZ, n);
                }
                
                mpz(int n) {
                    mpz_init_set_i(&MPZ, n);
                }
                
                mpz& operator=(mpz& n) {
                    mpz_set(&MPZ, &n.MPZ);
                    return *this;
                }
                
                number::sign sign() {
                    return gmp::sign(MPZ);
                }
                
                bool operator==(gmp_uint n) const;
                
                bool operator==(mpz& n) const;
                
                bool operator<(mpz& n) const;
                
                bool operator>(mpz& n) const;
                
                bool operator<=(mpz& n) const;
                
                bool operator>=(mpz& n) const;
            };
            
        }
        
    }

}

#endif
