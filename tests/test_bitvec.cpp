#include <algorithm>
#include <set>
#include <string>
#include <vector>
#include "bitvec.hpp"
#include "gtest/gtest.h"

using namespace std;


string binstring(uint64_t value, size_t len) {
  string res;
  do {
    uint64_t rem = value % 2;
    char ch = (rem == 0) ? '0' : '1';
    res.push_back(ch);
    value /= 2;
  } while (value > 0);
  return res;
}


const size_t set1[] = { 0, 7, 8, 21, 33, 50, 78, 99 };
const size_t set2[] = { 0, 7, 9, 12, 33, 44, 62, 98 };
const size_t n = sizeof(set1) / sizeof(*set1);


TEST(Bitvec, ConstructorSmall) {
  size_t len = 5;
  Bitvec bitvec(len);
  string resvec(len, '0');
  //cout << "bitvec: " << bitvec.to_string() << endl;
  //cout << "resvec: " << resvec << endl;
  ASSERT_EQ(bitvec.size(), len);
  ASSERT_EQ(bitvec.to_string(), resvec);
}


TEST(Bitvec, ConstructorLong) {
  size_t len = 1000;
  Bitvec bitvec(len);
  string resvec(len, '0');
  //cout << "bitvec: " << bitvec.to_string() << endl;
  //cout << "resvec: " << resvec << endl;
  ASSERT_EQ(bitvec.size(), len);
  ASSERT_EQ(bitvec.to_string(), resvec);
}


TEST(Bitvec, BitAccess) {
  size_t len = 1000;
  set<size_t> active(&set1[0], &set1[n]);
  Bitvec bitvec(len);
  for (set<size_t>::const_iterator it = active.begin(); it != active.end(); ++it) {
    bitvec.set_bit(*it);
  }
  for (size_t i = 0; i < bitvec.size(); ++i) {
    uint64_t res = bitvec.get_bit(i);

    if (active.find(i) != active.end())
      ASSERT_EQ(res, 1);
    else 
      ASSERT_EQ(res, 0);

  }
  for (set<size_t>::const_iterator it = active.begin(); it != active.end(); ++it) {
    bitvec.unset_bit(*it);
  }
  for (size_t i = 0; i < bitvec.size(); ++i) {
    uint64_t res = bitvec.get_bit(i);
    ASSERT_EQ(res, 0);
  }
}


TEST(Bitvec, InplaceLShift) {
  size_t len = 1000;
  set<size_t> active(&set1[0], &set1[n]);
  Bitvec bitvec(len);
  for (set<size_t>::const_iterator it = active.begin(); it != active.end(); ++it) {
    bitvec.set_bit(*it);
  }
  size_t wid = 28;
  //cout << "bitvec: " << bitvec.to_string() << endl;
  bitvec <<= wid;
  //cout << "bitvec: " << bitvec.to_string() << " (after left shift)" << endl;
  for (size_t i = 0; i < bitvec.size(); ++i) {
    uint64_t res = bitvec.get_bit(i);

    if (active.find(i - wid) != active.end())
      ASSERT_EQ(res, 1);
    else
      ASSERT_EQ(res, 0);

  }
}


TEST(Bitvec, InplaceRShift) {
  size_t len = 1000;
  set<size_t> active(&set1[0], &set1[n]);
  Bitvec bitvec(len);
  for (set<size_t>::const_iterator it = active.begin(); it != active.end(); ++it) {
    bitvec.set_bit(*it);
  }
  size_t wid = 37;
  //cout << "bitvec: " << bitvec.to_string() << endl;
  bitvec >>= wid;
  //cout << "bitvec: " << bitvec.to_string() << " (after right shift)" << endl;
  for (size_t i = 0; i < bitvec.size(); ++i) {
    uint64_t res = bitvec.get_bit(i);

    if (active.find(i + wid) != active.end())
      ASSERT_EQ(res, 1);
    else
      ASSERT_EQ(res, 0);

  }
}


TEST(Bitvec, InplaceNOT) {
  size_t len = 1000;
  set<size_t> active(&set1[0], &set1[n]);
  Bitvec bitvec(len);
  for (set<size_t>::const_iterator it = active.begin(); it != active.end(); ++it) {
    bitvec.set_bit(*it);
  }
  //cout << "bitvec: " << bitvec.to_string() << endl;
  ~bitvec;
  //cout << "bitvec: " << bitvec.to_string() << " (after inplace not)" << endl;
  for (size_t i = 0; i < bitvec.size(); ++i) {
    uint64_t res = bitvec.get_bit(i);

    if (active.find(i) != active.end())
      ASSERT_EQ(res, 0);
    else
      ASSERT_EQ(res, 1);

  }
}


TEST(Bitvec, InplaceAND) {
  size_t len = 1000;
  Bitvec bitvec1(len);
  set<size_t> active1(&set1[0], &set1[n]);
  for (set<size_t>::const_iterator it = active1.begin(); it != active1.end(); ++it) {
    bitvec1.set_bit(*it);
  }
  Bitvec bitvec2(len);
  set<size_t> active2(&set2[0], &set2[n]);
  for (set<size_t>::const_iterator it = active2.begin(); it != active2.end(); ++it) {
    bitvec2.set_bit(*it);
  }
  //cout << "bitvec1: " << bitvec1.to_string() << endl;
  //cout << "bitvec2: " << bitvec2.to_string() << endl;
  bitvec1 &= bitvec2;
  //cout << "bitvec1: " << bitvec1.to_string() << " (after inplace and)" << endl;
  // bitvec1 is changed.
  for (size_t i = 0; i < bitvec1.size(); ++i) {
    uint64_t res = bitvec1.get_bit(i);    

    if ((active1.find(i) != active1.end()) && (active2.find(i) != active2.end()))
      ASSERT_EQ(res, 1);
    else
      ASSERT_EQ(res, 0);

  }
  // bitvec2 is NOT changed.
  for (size_t i = 0; i < bitvec2.size(); ++i) {
    uint64_t res = bitvec2.get_bit(i);

    if (active2.find(i) != active2.end())
      ASSERT_EQ(res, 1);
    else
      ASSERT_EQ(res, 0);

  }
}


TEST(Bitvec, InplaceOR) {
  size_t len = 1000;
  Bitvec bitvec1(len);
  set<size_t> active1(&set1[0], &set1[n]);
  for (set<size_t>::const_iterator it = active1.begin(); it != active1.end(); ++it) {
    bitvec1.set_bit(*it);
  }
  Bitvec bitvec2(len);
  set<size_t> active2(&set2[0], &set2[n]);
  for (set<size_t>::const_iterator it = active2.begin(); it != active2.end(); ++it) {
    bitvec2.set_bit(*it);
  }
  //cout << "bitvec1: " << bitvec1.to_string() << endl;
  //cout << "bitvec2: " << bitvec2.to_string() << endl;
  bitvec1 |= bitvec2;
  //cout << "bitvec1: " << bitvec1.to_string() << " (after inplace or)" << endl;
  // bitvec1 is changed.
  for (size_t i = 0; i < bitvec1.size(); ++i) {
    uint64_t res = bitvec1.get_bit(i);    

    if ((active1.find(i) != active1.end()) || (active2.find(i) != active2.end()))
      ASSERT_EQ(res, 1);
    else
      ASSERT_EQ(res, 0);

  }
  // bitvec2 is NOT changed.
  for (size_t i = 0; i < bitvec2.size(); ++i) {
    uint64_t res = bitvec2.get_bit(i);

    if (active2.find(i) != active2.end())
      ASSERT_EQ(res, 1);
    else
      ASSERT_EQ(res, 0);

  }
}


TEST(Bitvec, InplaceXOR) {
  size_t len = 1000;
  Bitvec bitvec1(len);
  set<size_t> active1(&set1[0], &set1[n]);
  for (set<size_t>::const_iterator it = active1.begin(); it != active1.end(); ++it) {
    bitvec1.set_bit(*it);
  }
  Bitvec bitvec2(len);
  set<size_t> active2(&set2[0], &set2[n]);
  for (set<size_t>::const_iterator it = active2.begin(); it != active2.end(); ++it) {
    bitvec2.set_bit(*it);
  }
  //cout << "bitvec1: " << bitvec1.to_string() << endl;
  //cout << "bitvec2: " << bitvec2.to_string() << endl;
  bitvec1 ^= bitvec2;
  //cout << "bitvec1: " << bitvec1.to_string() << " (after inplace xor)" << endl;
  // bitvec1 is changed.
  for (size_t i = 0; i < bitvec1.size(); ++i) {
    uint64_t res = bitvec1.get_bit(i);    

    if (((active1.find(i) != active1.end()) && (active2.find(i) == active2.end())) ||
        ((active1.find(i) == active1.end()) && (active2.find(i) != active2.end())))
      ASSERT_EQ(res, 1);
    else
      ASSERT_EQ(res, 0);

  }
  // bitvec2 is NOT changed.
  for (size_t i = 0; i < bitvec2.size(); ++i) {
    uint64_t res = bitvec2.get_bit(i);

    if (active2.find(i) != active2.end())
      ASSERT_EQ(res, 1);
    else
      ASSERT_EQ(res, 0);

  }
}


TEST(Bitvec, InplaceADD) {
  size_t len = 1000;
  uint64_t val1 = 9758613597ULL;
  string binstring1 = binstring(val1, len);
  Bitvec bitvec1(len);
  for (size_t i = 0; i < binstring1.size(); ++i) {
    if (binstring1[i] == '1') bitvec1.set_bit(i);
  }
  uint64_t val2 = 4799104567ULL;
  string binstring2 = binstring(val2, len);
  Bitvec bitvec2(len);
  for (size_t i = 0; i < binstring2.size(); ++i) {
    if (binstring2[i] == '1') bitvec2.set_bit(i);
  }
  uint64_t val3 = val1 + val2;
  string binstring3 = binstring(val3, len);
  Bitvec bitvec3(len);
  for (size_t i = 0; i < binstring3.size(); ++i) {
    if (binstring3[i] == '1') bitvec3.set_bit(i);
  }
  //cout << "bitvec1: " << bitvec1.to_string() << endl;
  //cout << "bitvec2: " << bitvec2.to_string() << endl;  
  bitvec1 += bitvec2;
  //cout << "bitvec1: " << bitvec1.to_string() << " (after inplace add)" << endl;
  // bitvec1 is changed.
  for (size_t i = 0; i < bitvec1.size(); ++i) {
    uint64_t res1 = bitvec1.get_bit(i);    
    uint64_t res3 = bitvec3.get_bit(i);
    ASSERT_EQ(res1, res3);
  }
}

TEST(Bitvec, InplaceSUB) {
  size_t len = 1000;
  uint64_t val1 = 9758613597ULL;
  string binstring1 = binstring(val1, len);
  Bitvec bitvec1(len);
  for (size_t i = 0; i < binstring1.size(); ++i) {
    if (binstring1[i] == '1') bitvec1.set_bit(i);
  }
  uint64_t val2 = 4799104567ULL;
  string binstring2 = binstring(val2, len);
  Bitvec bitvec2(len);
  for (size_t i = 0; i < binstring2.size(); ++i) {
    if (binstring2[i] == '1') bitvec2.set_bit(i);
  }
  uint64_t val3 = val1 - val2;
  string binstring3 = binstring(val3, len);
  Bitvec bitvec3(len);
  for (size_t i = 0; i < binstring3.size(); ++i) {
    if (binstring3[i] == '1') bitvec3.set_bit(i);
  }
  //cout << "bitvec1: " << bitvec1.to_string() << endl;
  //cout << "bitvec2: " << bitvec2.to_string() << endl;  
  bitvec1 -= bitvec2;
  //cout << "bitvec1: " << bitvec1.to_string() << " (after inplace add)" << endl;
  // bitvec1 is changed.
  for (size_t i = 0; i < bitvec1.size(); ++i) {
    uint64_t res1 = bitvec1.get_bit(i);    
    uint64_t res3 = bitvec3.get_bit(i);
    ASSERT_EQ(res1, res3);
  }
}

