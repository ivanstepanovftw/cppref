#include <iostream>
#include <list>
#include <algos/range.hh>
#include <algos/list.hh>
#include <algos/vector.hh>
#include <boost/test/auto_unit_test.hpp>


BOOST_AUTO_TEST_SUITE(algos_data)

BOOST_AUTO_TEST_CASE(range_functional) {
    // cout<<"range(0).stop: "<<range(0).stop<<endl;
    // cout<<"range(0,6,3).stop: "<<range(0,6,3).stop<<endl;
    // cout<<"range(0,7,3).stop: "<<range(0,7,3).stop<<endl;
    // cout<<"range(0,8,3).stop: "<<range(0,8,3).stop<<endl;
    // BOOST_CHECK_EQUAL(range(0,6,3).stop, 9);

    BOOST_CHECK_EQUAL(*range(6).end(), 6);
    BOOST_CHECK_EQUAL(*range(0,6,3).end(), 6);
    BOOST_CHECK_EQUAL(*range(0,7,3).end(), 9);
    BOOST_CHECK_EQUAL(*range(0,8,3).end(), 9);
    BOOST_CHECK_EQUAL(*range(8,0,-3).end(), -1);
    BOOST_CHECK_EQUAL(*range(7,0,-3).end(), -2);
    BOOST_CHECK_EQUAL(*range(6,0,-3).end(), 0);
    auto r1 = range(0, 3, 1);
    auto ir1 = r1.begin();
    auto ir2 = r1.end();
    BOOST_CHECK_EQUAL(*ir1++, 0);
    BOOST_CHECK_EQUAL(*++ir1, 2);
    BOOST_CHECK(++ir1 == ir2);

}

BOOST_AUTO_TEST_CASE(list_functional) {
    using std::cout, std::endl;

    // std::vector<int> l;
    // std::list<int> l;
    list<int> l;
    BOOST_CHECK(l.begin() == l.end());

    l.push_back(10); // [10]
    l.push_back(20); // [10,20]

    auto it = l.begin();
    it = l.insert(it++, 1);   // [1,10,20]
    it = l.insert(it++, 2);   // [2,1,10,20]
    it = l.end();
    it = l.insert(it++, 100); // [2,1,10,20,100]
    it = l.insert(it++, 101); // [2,1,10,20,101,100]

    l.push_back(999);   // [2,1,10,20,101,100,999]
    l.push_back(9999);  // [2,1,10,20,101,100,999,9999]
    l.erase(--l.end()); // [2,1,10,20,101,100,999]
    l.erase(l.begin()); // [1,10,20,101,100,999]
    for (int& i : l) {
        cout<<i<<endl;
    }

    {
        auto cur = l.begin();
        auto stop = l.end();
        BOOST_CHECK(cur != stop);
        BOOST_CHECK_EQUAL(*cur++, 1);
        BOOST_CHECK(cur != stop);
        BOOST_CHECK_EQUAL(*cur++, 10);
        BOOST_CHECK(cur != stop);
        BOOST_CHECK_EQUAL(*cur++, 20);
        BOOST_CHECK(cur != stop);
        BOOST_CHECK_EQUAL(*cur++, 101);
        BOOST_CHECK(cur != stop);
        BOOST_CHECK_EQUAL(*cur++, 100);
        BOOST_CHECK(cur != stop);
        BOOST_CHECK_EQUAL(*cur++, 999);
        BOOST_CHECK(cur == stop);
    }
}


BOOST_AUTO_TEST_CASE(vector_functional) {
    using std::cout, std::endl;

    // std::vector<int> v;
    vector<int> v;
    v.push_back(10); // [10]
    v.push_back(20); // [10,20]

    auto it = v.begin();
    it = v.insert(it++, 1);   // [1,10,20]
    it = v.insert(it++, 2);   // [2,1,10,20]
    it = v.end();
    it = v.insert(it++, 100); // [2,1,10,20,100]
    it = v.insert(it++, 101); // [2,1,10,20,101,100]

    v.push_back(999);   // [2,1,10,20,101,100,999]
    v.push_back(9999);  // [2,1,10,20,101,100,999,9999]
    // cout<<"size:"<<v.size()<<endl;
    // cout<<"capacity:"<<v.capacity()<<endl;
    // v.erase(--v.end()); // [2,1,10,20,101,100,999]
    // v.erase(v.begin()); // [1,10,20,101,100,999]
    for (int& i : v) {
        cout<<i<<endl;
    }
    cout<<"*it:"<<*it<<endl;
    cout<<"size:"<<v.size()<<endl;
    cout<<"capacity:"<<v.capacity()<<endl;

    // {
    //     auto cur = v.begin();
    //     auto stop = v.end();
    //     BOOST_CHECK(cur != stop);
    //     BOOST_CHECK_EQUAL(*cur++, 1);
    //     BOOST_CHECK(cur != stop);
    //     BOOST_CHECK_EQUAL(*cur++, 10);
    //     BOOST_CHECK(cur != stop);
    //     BOOST_CHECK_EQUAL(*cur++, 20);
    //     BOOST_CHECK(cur != stop);
    //     BOOST_CHECK_EQUAL(*cur++, 101);
    //     BOOST_CHECK(cur != stop);
    //     BOOST_CHECK_EQUAL(*cur++, 100);
    //     BOOST_CHECK(cur != stop);
    //     BOOST_CHECK_EQUAL(*cur++, 999);
    //     BOOST_CHECK(cur == stop);
    // }
}

BOOST_AUTO_TEST_SUITE_END()
