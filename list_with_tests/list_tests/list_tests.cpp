#include "stdafx.h"
#include "../list/MyList.h"

using namespace std;
using namespace std::placeholders;

namespace
{
    struct EmptyListFixure
    {
        CMyList<int> list;
        CMyList<int> const & constList = list;
    };

    struct ThrowErrorInDefaultConstructor
    {
        ThrowErrorInDefaultConstructor()
        {
            throw runtime_error("from default constructor");
        }
    };

    struct StlListWithSeveralIntsFixture
    {
        std::list<int> stlList{ 1, 2, 3 };
    };

    struct ListWithSeveralIntsFixture : public EmptyListFixure
    {
        ListWithSeveralIntsFixture()
        {
            list.Emplace(list.cend(), 1);
            list.Emplace(list.cend(), 2);
            list.Emplace(list.cend(), 3);
            list.Emplace(list.cend(), 4);
        }

        const size_t listInitialSize = 4;
    };

    struct CanThrowOnCopy
    {
        CanThrowOnCopy(int id, bool needThrow = false)
            : m_needThrow(needThrow)
            , m_id(id)
        {}
        CanThrowOnCopy(const CanThrowOnCopy & rhs)
            : CanThrowOnCopy(rhs.m_id, rhs.m_needThrow)
        {
            if (m_needThrow)
            {
                throw runtime_error("make copy");
            }
        }

        CanThrowOnCopy(CanThrowOnCopy && rhs)
            : CanThrowOnCopy(rhs.m_id, rhs.m_needThrow)
        {}

        bool operator == (const CanThrowOnCopy & rhs) const
        {
            return m_id == rhs.m_id;
        }

    private:
        int m_id;
        bool m_needThrow;
    };

    struct ListWithSeveralThrowOnCopyElementsFixture
    {
        ListWithSeveralThrowOnCopyElementsFixture()
        {
            list.EmplaceBack(1, true);
            list.EmplaceBack(2, true);
            list.EmplaceBack(3, true);
        }

        CMyList<CanThrowOnCopy> list;
    };

    template <typename T>
    bool operator == (const CMyList<T> & lhs, const std::list<T> & rhs)
    {
        return std::equal(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
    }
}

BOOST_AUTO_TEST_SUITE(default_contructor) // ------------------- default_contructor -------------------

BOOST_AUTO_TEST_CASE(default_constructor_creates_empty_list_with_size_0)
{
    auto list = CMyList<int>();
    BOOST_CHECK(list.IsEmpty());
    BOOST_CHECK_EQUAL(list.GetSize(), 0);
}

BOOST_AUTO_TEST_CASE(default_constructor_does_not_call_value_default_constructor)
{
    auto test = [] {
        return CMyList<ThrowErrorInDefaultConstructor>();
    };

    BOOST_CHECK_NO_THROW(test());
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_FIXTURE_TEST_SUITE(range_contructor, StlListWithSeveralIntsFixture) // ------------------- range_contructor -------------------

BOOST_AUTO_TEST_CASE(list_constructs_from_two_iterators)
{
    auto list = CMyList<int>(stlList.begin(), stlList.end());
    BOOST_CHECK_EQUAL(list.GetSize(), 3);
    BOOST_CHECK((list == stlList));
}

BOOST_AUTO_TEST_CASE(list_constructs_from_initializer_list)
{
    auto list = CMyList<int>{ 1, 2, 3 };
    BOOST_CHECK_EQUAL(list.GetSize(), 3);
    BOOST_CHECK((list == stlList));
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_FIXTURE_TEST_SUITE(another_constructors, ListWithSeveralIntsFixture) // ------------------- another_constructors -------------------

BOOST_AUTO_TEST_CASE(copy_constructor_make_a_copy)
{
    CMyList<int> copy(list);
    BOOST_CHECK_EQUAL(list.GetSize(), copy.GetSize());
    BOOST_CHECK(equal(list.begin(), list.end(), copy.begin(), copy.end()));
}

BOOST_AUTO_TEST_CASE(copy_constructor_call_values_copy_constructor)
{
    auto test = [] {
        auto list1 = CMyList<CanThrowOnCopy>();
        list1.EmplaceBack(1, true);
        return CMyList<CanThrowOnCopy>(list1);
    };

    BOOST_CHECK_THROW(test(), runtime_error);
}

BOOST_AUTO_TEST_CASE(move_constructor_steals_values)
{
    auto movedList = CMyList<int>(move(list));
    BOOST_CHECK(list.IsEmpty());
    BOOST_CHECK((movedList == std::list<int>{1, 2, 3, 4}));
}

BOOST_AUTO_TEST_SUITE_END()


BOOST_FIXTURE_TEST_SUITE(empty_list_iterator, EmptyListFixure) // ------------------- empty_list_iterator -------------------

BOOST_AUTO_TEST_CASE(cbegin_equals_to_begin)
{
    BOOST_CHECK(list.begin() == constList.begin());
    BOOST_CHECK(list.begin() == list.cbegin());
}

BOOST_AUTO_TEST_CASE(cend_equals_to_end)
{
    BOOST_CHECK(list.end() == constList.end());
    BOOST_CHECK(list.end() == list.cend());
}

BOOST_AUTO_TEST_CASE(begin_on_empty_list_equals_end_iterator)
{
    BOOST_CHECK(list.begin() == list.end());
    BOOST_CHECK(constList.begin() == constList.end());
    BOOST_CHECK(list.cbegin() == list.cend());

    BOOST_CHECK(list.rbegin() == list.rend());
    BOOST_CHECK(constList.rbegin() == constList.rend());
    BOOST_CHECK(list.crbegin() == list.crend());
}

BOOST_AUTO_TEST_CASE(decrement_begin_iterator_throws_logic_error)
{
    auto CheckThrowLogicError = [](auto && it) {
        BOOST_CHECK_THROW(--it, logic_error);
        BOOST_CHECK_THROW(it--, logic_error);
    };

    CheckThrowLogicError(list.begin());
    CheckThrowLogicError(list.cbegin());
    CheckThrowLogicError(constList.begin());
    CheckThrowLogicError(list.rbegin());
    CheckThrowLogicError(list.crbegin());
    CheckThrowLogicError(constList.rbegin());
}

BOOST_AUTO_TEST_CASE(increment_end_iterator_throws_logic_error)
{
    auto CheckThrowLogicError = [](auto && it) {
        BOOST_CHECK_THROW(++it, logic_error);
        BOOST_CHECK_THROW(it++, logic_error);
    };

    CheckThrowLogicError(list.end());
    CheckThrowLogicError(list.cend());
    CheckThrowLogicError(constList.end());
    CheckThrowLogicError(list.rend());
    CheckThrowLogicError(list.crend());
    CheckThrowLogicError(constList.rend());
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_FIXTURE_TEST_SUITE(emplace_to_list, EmptyListFixure) // ------------------- emplace_to_list -------------------

BOOST_AUTO_TEST_CASE(emplace_to_list_increase_size_by_1)
{
    auto it = list.Emplace(list.cend(), 42);
    BOOST_CHECK_EQUAL(list.GetSize(), 1);

    list.Emplace(it, 43);
    BOOST_CHECK_EQUAL(list.GetSize(), 2);
}

BOOST_AUTO_TEST_CASE(emplace_to_list_returns_iterator_to_inserted_element)
{
    auto it = list.Emplace(list.cend(), 42);
    BOOST_CHECK_EQUAL(*it, 42);

    it = list.Emplace(it, 2);
    BOOST_CHECK_EQUAL(*it, 2);
}

BOOST_AUTO_TEST_CASE(emplace_to_end_creates_elements_coherently)
{
    list.Emplace(list.cend(), 1);
    list.Emplace(list.cend(), 2);
    list.Emplace(list.cend(), 3);

    BOOST_CHECK((list == std::list<int>{1, 2, 3}));
}

BOOST_AUTO_TEST_CASE(emplace_to_begin_creates_elements_reversed)
{
    list.Emplace(list.cbegin(), 1);
    list.Emplace(list.cbegin(), 2);
    list.Emplace(list.cbegin(), 3);
    list.Emplace(list.cbegin(), 4);

    BOOST_CHECK((list == std::list<int>{4, 3, 2, 1}));
}

BOOST_AUTO_TEST_CASE(emplace_by_foreign_iterator_throws_logic_error)
{
    auto anotherList = CMyList<int>();
    BOOST_CHECK_THROW(list.Emplace(anotherList.cend(), 42), logic_error);
}

BOOST_AUTO_TEST_CASE(emplace_back_inserts_element_to_the_end_of_list)
{
    list.EmplaceBack(1);
    BOOST_CHECK((list == std::list<int>{1}));
    list.EmplaceBack(2);
    BOOST_CHECK((list == std::list<int>{1, 2}));
    list.EmplaceBack(3);
    BOOST_CHECK((list == std::list<int>{1, 2, 3}));
}

BOOST_AUTO_TEST_CASE(emplace_back_returns_iterator_to_inserted_element)
{
    auto it = list.EmplaceBack(1);
    BOOST_CHECK_EQUAL(*it, 1);
    it = list.EmplaceBack(2);
    BOOST_CHECK_EQUAL(*it, 2);
    it = list.EmplaceBack(3);
    BOOST_CHECK_EQUAL(*it, 3);
}

BOOST_AUTO_TEST_CASE(emplace_front_inserts_element_before_first_element)
{
    list.EmplaceFront(1);
    BOOST_CHECK((list == std::list<int>{1}));
    list.EmplaceFront(2);
    BOOST_CHECK((list == std::list<int>{2, 1}));
    list.EmplaceFront(3);
    BOOST_CHECK((list == std::list<int>{3, 2, 1}));
}

BOOST_AUTO_TEST_CASE(emplace_front_returns_iterator_to_inserted_element)
{
    auto it = list.EmplaceFront(1);
    BOOST_CHECK_EQUAL(*it, 1);
    it = list.EmplaceFront(2);
    BOOST_CHECK_EQUAL(*it, 2);
    it = list.EmplaceFront(3);
    BOOST_CHECK_EQUAL(*it, 3);
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_FIXTURE_TEST_SUITE(insert_range_to_list, ListWithSeveralIntsFixture) // ------------------- insert_range_to_list -------------------

BOOST_AUTO_TEST_CASE(insert_empty_range_doing_nothing)
{
    CMyList<int> anotherList;
    list.Insert(list.end(), anotherList.begin(), anotherList.end());

    BOOST_CHECK_EQUAL(list.GetSize(), listInitialSize);
}

BOOST_AUTO_TEST_CASE(inserted_values_took_place_coherently_before_insertion_position)
{
    CMyList<int> anotherList;
    anotherList.Emplace(anotherList.end(), 15);
    anotherList.Insert(anotherList.begin(), list.begin(), list.end());

    BOOST_CHECK((anotherList == std::list<int>{1, 2, 3, 4, 15}));
    BOOST_CHECK_EQUAL(anotherList.GetSize(), 5);
}

BOOST_AUTO_TEST_CASE(insert_range_with_error_rollback_changes)
{
    std::list<CanThrowOnCopy> stlList;
    stlList.emplace_back(1);
    stlList.emplace_back(2);
    stlList.emplace_back(3);
    stlList.emplace_back(4, true);
    stlList.emplace_back(5);
    stlList.emplace_back(6);

    CMyList<CanThrowOnCopy> myList;
    myList.EmplaceBack(42, false);
    auto it = myList.EmplaceBack(43, false);
    myList.EmplaceBack( 44, false);

    BOOST_CHECK_THROW(myList.Insert(it, stlList.begin(), stlList.end()), runtime_error);
    BOOST_CHECK_EQUAL(myList.GetSize(), 3);

    stlList.clear();
    stlList.emplace_back(42);
    stlList.emplace_back(43);
    stlList.emplace_back(44);
    BOOST_CHECK((myList == stlList));
}

BOOST_AUTO_TEST_CASE(insert_range_throw_error_if_specified_foreign_iterator_as_position)
{
    CMyList<int> myList;
    BOOST_CHECK_THROW(myList.Insert(list.begin(), list.begin(), list.end()), logic_error);
}

BOOST_AUTO_TEST_CASE(insert_range_returns_iterator_to_first_inserted_element)
{
    CMyList<int> myList{42};
    auto it = myList.Insert(myList.end(), list.begin(), list.end());
    BOOST_CHECK_EQUAL(*it, 1);
}

BOOST_AUTO_TEST_CASE(insert_empty_range_returns_iterator_specified_to_insert_as_position)
{
    CMyList<int> myList;
    auto it = list.Insert(list.end(), myList.begin(), myList.end());
    BOOST_CHECK((it == list.end()));
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_FIXTURE_TEST_SUITE(list_iterator, ListWithSeveralIntsFixture) // ------------------- list_iterator -------------------

BOOST_AUTO_TEST_CASE(begin_iterator_refers_to_first_element)
{
    BOOST_CHECK_EQUAL(*list.begin(), 1);
    BOOST_CHECK_EQUAL(*list.cbegin(), 1);
    BOOST_CHECK_EQUAL(*constList.begin(), 1);
}

BOOST_AUTO_TEST_CASE(end_iterator_refers_to_node_after_last_element)
{
    BOOST_CHECK_EQUAL(*(--list.end()), 4);
    BOOST_CHECK_EQUAL(*(--list.cend()), 4);
    BOOST_CHECK_EQUAL(*(--constList.end()), 4);
}

BOOST_AUTO_TEST_CASE(old_end_iterator_stay_end_after_emplace_back)
{
    auto it = list.end();
    --it;
    BOOST_CHECK_EQUAL(*it, 4);
    ++it;
    list.Emplace(list.cend(), 42);
    --it;
    BOOST_CHECK_EQUAL(*it, 42);
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_FIXTURE_TEST_SUITE(erase_from_list, ListWithSeveralIntsFixture) // ------------------- erase_from_list -------------------

BOOST_AUTO_TEST_CASE(erase_element_decrease_size_by_1)
{
    list.Erase(list.begin());
    BOOST_CHECK_EQUAL(list.GetSize(), listInitialSize - 1);

    list.Erase(list.begin());
    BOOST_CHECK_EQUAL(list.GetSize(), listInitialSize - 2);
}

BOOST_AUTO_TEST_CASE(erase_begin_element_remove_first_element)
{
    list.Erase(list.begin());
    BOOST_CHECK((list == std::list<int>{2, 3, 4}));
}

BOOST_AUTO_TEST_CASE(erase_pre_end_element_remove_one_element_from_back)
{
    list.Erase(--list.end());
    BOOST_CHECK((list == std::list<int>{1, 2, 3}));
}

BOOST_AUTO_TEST_CASE(erase_range_of_element_from_begin_to_middle)
{
    list.Erase(list.begin(), --(--list.end()));
    BOOST_CHECK((list == std::list<int>{3, 4}));
    BOOST_CHECK_EQUAL(list.GetSize(), 2);
}

BOOST_AUTO_TEST_CASE(erase_range_of_element_from_middle_to_end)
{
    list.Erase(++(++list.begin()), list.end());
    BOOST_CHECK((list == std::list<int>{1, 2}));
    BOOST_CHECK_EQUAL(list.GetSize(), 2);
}

BOOST_AUTO_TEST_CASE(erase_range_of_element_in_middle)
{
    list.Erase(++list.begin(), --list.end());
    BOOST_CHECK((list == std::list<int>{1, 4}));
    BOOST_CHECK_EQUAL(list.GetSize(), 2);
}

BOOST_AUTO_TEST_CASE(clear_remove_all_elements)
{
    list.Clear();
    BOOST_CHECK(list.IsEmpty());
    BOOST_CHECK_EQUAL(list.GetSize(), 0);
    BOOST_CHECK((list.begin() == list.end()));
}

BOOST_AUTO_TEST_CASE(erase_by_foreign_iterator_throws_logic_error)
{
    auto anotherList = CMyList<int>();
    anotherList.Emplace(anotherList.cend(), 42);
    BOOST_CHECK_THROW(list.Erase(anotherList.cbegin()), logic_error);
    BOOST_CHECK_THROW(list.Erase(anotherList.cbegin(), anotherList.cend()), logic_error);
}

BOOST_AUTO_TEST_CASE(erase_by_foreign_and_self_iterator_throws_logic_error)
{
    auto anotherList = CMyList<int>();
    anotherList.Emplace(anotherList.cend(), 42);
    BOOST_CHECK_THROW(list.Erase(anotherList.cbegin(), list.cend()), logic_error);
    BOOST_CHECK_THROW(list.Erase(list.cbegin(), anotherList.cend()), logic_error);
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_FIXTURE_TEST_SUITE(list_modifiers, ListWithSeveralIntsFixture) // ------------------- list_modifiers -------------------

BOOST_AUTO_TEST_CASE(swap_do_swap_of_contents)
{
    CMyList<int> anotherInts;
    anotherInts.Swap(list);
    BOOST_CHECK((list == std::list<int>()));
    BOOST_CHECK((anotherInts == std::list<int>{1, 2, 3, 4}));
}

BOOST_AUTO_TEST_CASE(swap_dont_call_value_copy_constructor)
{
    ListWithSeveralThrowOnCopyElementsFixture fix;
    CMyList<CanThrowOnCopy> list2;
    BOOST_CHECK_NO_THROW(list2.Swap(fix.list));
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_FIXTURE_TEST_SUITE(relational_operators, ListWithSeveralIntsFixture) // ------------------- relational_operators -------------------

BOOST_AUTO_TEST_CASE(equals_operator_work_correctly_on_same_lists)
{
    CMyList<int> copy(list);
    BOOST_CHECK((copy == list));
}

BOOST_AUTO_TEST_CASE(not_equals_operator_work_correctly_on_different_lists)
{
    CMyList<int> emptyList;
    BOOST_CHECK((emptyList != list));
}

BOOST_AUTO_TEST_SUITE_END()