#ifndef OPERATIONS_WITH_SETS_H
#define OPERATIONS_WITH_SETS_H

#include <set>
#include <cstdio>
/**
 * \brief In this file, set-theoretic operations with
 *        standard containers std :: set are defined.
*/
namespace operations_with_sets{
    /**
     *  \brief The function single_elem returns a set consisting of one element.
     *  \param [in] x element
     *  \return       a set consisting of one element, x.
     */
    template<typename T>
    std::set<T> single_elem(const T& x){
        std::set<T> s;
        s.insert(x);
        return s;
    }

    /**
     *  \brief Prints a set.
     *  \param [in] a          Printed set.
     *  \param [in] print_elem Print function of the set element.
     */
    template<typename T>
    void print_set(const std::set<T>& a, void (*print_elem)(const T&)){
        if(a.empty()){
            printf("{}");
            return;
        }
        auto first       = a.begin();
        auto before_last = --a.end();
        putchar('{');
        for(auto i = first; i != before_last; ++i){
            print_elem(*i);
            putchar(',');
        }
        print_elem(*before_last);
        putchar('}');
    }


    /**
     *  \brief Prints a set.
     *  \param [in] a          Printed set.
     *  \param [in] print_elem Print function of the set element.
     */
    template<typename T>
    void print_set(const std::set<T>& a, void (*print_elem)(const T)){
        if(a.empty()){
            printf("{}");
            return;
        }
        auto first       = a.begin();
        auto before_last = --a.end();
        putchar('{');
        for(auto i = first; i != before_last; ++i){
            print_elem(*i);
            putchar(',');
        }
        print_elem(*before_last);
        putchar('}');
    }

    /**
     *  \brief Checking the membership of x for a set a.
     *  \param [in] a The set a.
     *  \param [in] x The element x.
     *  \return       true if the element x belongs to the set a, and false otherwise
     */
    template<typename T>
    bool is_elem(const T& x, const std::set<T>& a){
        return a.find(x) != a.end();
    }

    /**
     *  \brief The union of the sets a and b.
     *  \param [in] a The set a.
     *  \param [in] b The set b.
     *  \return       The union of the sets a and b. */
    template<typename T>
    std::set<T> operator + (const std::set<T>& a, const std::set<T>& b){
        std::set<T> s = a;
        s.insert(b.begin(), b.end());
        return s;
    }

    /**
     *  \brief   The set-theoretic difference of the sets a and b.
     *  \details The set-theoretic difference of the sets a and b (denoted in a set
     *           theory as a \ b), that is, a set consisting only of those elements
     *           of the set a that do not belong to the set b.
     *  \param [in] a The set a.
     *  \param [in] b The set b.
     *  \return       The set-theoretic difference of the sets a and b.
     */
    template<typename T>
    std::set<T> operator - (const std::set<T>& a, const std::set<T>& b){
        std::set<T> s = a;
        for(const auto x : b){
            s.erase(x);
        }
        return s;
    }

    /**
     *  \brief   The intersection of the sets a and b.
     *  \details The intersection of the sets a and b, that is, the set consisting
     *           precisely of those elements that belong to both a and b.
     *  \param [in] a The set a.
     *  \param [in] b The set b.
     *  \return       The intersection of the sets a and b.
     */
    template<typename T>
    std::set<T> operator * (const std::set<T>& a, const std::set<T>& b){
        std::set<T> s;
        for(const auto& x : a){
            if(is_elem(x, b)){
                s.insert(x);
            }
        }
        return s;
    }

    /**
     *  \brief   The symmetric difference of the sets a and b.
     *  \details The symmetric difference of the sets a and b, that is, the union
     *           of these sets, but with the common elements thrown out.
     *  \param [in] a The set a.
     *  \param [in] b The set b.
     *  \return       The symmetric difference of the sets a and b.
     */
    template<typename T>
    std::set<T> operator ^ (const std::set<T>& a, const std::set<T>& b){
        return (a - b) + (b - a);
    }

    /**
     *  \brief Checks whether the set a is a subset of the set b,
     *         possibly coinciding with b.
     *  \param [in] a The set a.
     *  \param [in] b The set b.
     *  \return       true if the set a is a subset of the set b, and false otherwise
     */
    template<typename T>
    bool is_subseteq(const std::set<T>& a, const std::set<T>& b){
        std::set<T> s = (a * b) ^ a;
        return s.empty();
    }
};
#endif
