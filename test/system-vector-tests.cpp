#include "simit-test.h"

#include "graph.h"
#include "program.h"
#include "function.h"
#include "error.h"
#include "types.h"

using namespace std;
using namespace simit;

TEST(System, vector_add) {
  Set<> points;
  FieldRef<simit_float> x = points.addField<simit_float>("x");

  ElementRef p0 = points.add();
  x.set(p0, 42.0);

  std::unique_ptr<Function> f = getFunction(TEST_FILE_NAME, "main");
  if (!f) FAIL();
  f->bind("points", &points);

  f->runSafe();

  ASSERT_EQ(84.0, (int)x.get(p0));
}

TEST(System, vector_add_blocked) {
  Set<> points;
  FieldRef<simit_float,3> x = points.addField<simit_float,3>("x");

  ElementRef p0 = points.add();
  ElementRef p1 = points.add();

  x.set(p0, {1.0, 2.0, 3.0});
  x.set(p1, {4.0, 5.0, 6.0});

  std::unique_ptr<Function> f = getFunction(TEST_FILE_NAME, "main");
  if (!f) FAIL();
  f->bind("points", &points);

  f->runSafe();

  ASSERT_EQ(2.0, x.get(p0)(0));
  ASSERT_EQ(4.0, x.get(p0)(1));
  ASSERT_EQ(6.0, x.get(p0)(2));
  ASSERT_EQ(8.0, x.get(p1)(0));
  ASSERT_EQ(10.0, x.get(p1)(1));
  ASSERT_EQ(12.0, x.get(p1)(2));
}

TEST(System, vector_dot) {
  Set<> points;
  FieldRef<simit_float> x = points.addField<simit_float>("x");
  FieldRef<simit_float> z = points.addField<simit_float>("z");

  ElementRef p0 = points.add();
  ElementRef p1 = points.add();
  ElementRef p2 = points.add();
  x.set(p0, 1.0);
  x.set(p1, 2.0);
  x.set(p2, 3.0);

  std::unique_ptr<Function> f = getFunction(TEST_FILE_NAME, "main");
  if (!f) FAIL();
  f->bind("points", &points);

  f->runSafe();
  ASSERT_EQ(14.0, (int)z.get(p0));
}

TEST(System, vector_dot_blocked) {
  Set<> points;
  FieldRef<simit_float,3> x = points.addField<simit_float,3>("x");
  FieldRef<simit_float> z = points.addField<simit_float>("z");

  ElementRef p0 = points.add();
  ElementRef p1 = points.add();
  ElementRef p2 = points.add();
  x.set(p0, {1.0,2.0,3.0});
  x.set(p1, {4.0,5.0,6.0});
  x.set(p2, {7.0,8.0,9.0});

  std::unique_ptr<Function> f = getFunction(TEST_FILE_NAME, "main");
  if (!f) FAIL();
  f->bind("points", &points);

  f->runSafe();
  ASSERT_EQ(285.0, (simit_float)z.get(p0));
}

TEST(System, vector_assign_blocked) {
  Set<> points;
  FieldRef<simit_float,2> x = points.addField<simit_float,2>("x");

  ElementRef p0 = points.add();
  ElementRef p1 = points.add();
  x.set(p0, {1.0, 2.0});
  x.set(p1, {3.0, 4.0});

  std::unique_ptr<Function> f = getFunction(TEST_FILE_NAME, "main");
  if (!f) FAIL();
  f->bind("points", &points);

  f->runSafe();

  ASSERT_EQ(2.0, x.get(p0)(0));
  ASSERT_EQ(4.0, x.get(p0)(1));
  ASSERT_EQ(6.0, x.get(p1)(0));
  ASSERT_EQ(8.0, x.get(p1)(1));
}

TEST(System, vector_add_large_system) {
  Set<> points;
  FieldRef<simit_float> x = points.addField<simit_float>("x");

  std::vector<ElementRef> ps;
  for(int i = 0; i < 2557; ++i) {
    ps.push_back(points.add());
    x.set(ps.back(), (simit_float)i);
  }

  std::unique_ptr<Function> f = getFunction(TEST_FILE_NAME, "main");
  if (!f) FAIL();
  f->bind("points", &points);

  f->runSafe();

  for(size_t i = 0; i < ps.size(); ++i) {
    ASSERT_EQ(i*2, (int)x.get(ps[i]));
  }
}
